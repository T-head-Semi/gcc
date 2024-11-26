#define IN_TARGET_CODE 1
#define INCLUDE_ALGORITHM
#define INCLUDE_FUNCTIONAL

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "backend.h"
#include "rtl.h"
#include "target.h"
#include "tree-pass.h"
#include "df.h"
#include "rtl-ssa.h"
#include "cfgcleanup.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "insn-opinit.h"
#include "tm-constrs.h"
#include "cfgrtl.h"
#include "cfganal.h"
#include "lcm.h"
#include "predict.h"
#include "profile-count.h"
#include "gcse.h"
#include "cfgloop.h"
#include "rtl-iter.h"

using namespace rtl_ssa;

/* Set the bitmap DST to the union of SRC of predecessors of
   basic block B.
   It's a bit different from bitmap_union_of_preds in cfganal.cc. This function
   takes into account the case where pred is ENTRY basic block. The main reason
   for this difference is to make it easier to insert some special value into
   the ENTRY base block. For example, mcfg_info with a status of UNKNOW.  */
static void
bitmap_union_of_preds_with_entry (sbitmap dst, sbitmap *src, basic_block b)
{
  unsigned int set_size = dst->size;
  edge e;
  unsigned ix;

  for (ix = 0; ix < EDGE_COUNT (b->preds); ix++)
    {
      e = EDGE_PRED (b, ix);
      bitmap_copy (dst, src[e->src->index]);
      break;
    }

  if (ix == EDGE_COUNT (b->preds))
    bitmap_clear (dst);
  else
    for (ix++; ix < EDGE_COUNT (b->preds); ix++)
      {
	unsigned int i;
	SBITMAP_ELT_TYPE *p, *r;

	e = EDGE_PRED (b, ix);
	p = src[e->src->index]->elms;
	r = dst->elms;
	for (i = 0; i < set_size; i++)
	  *r++ |= *p++;
      }
}

/* Compute the reaching defintion in and out based on the gen and KILL
   informations in each Base Blocks.
   This function references the compute_avaiable implementation in lcm.cc  */
static void
compute_reaching_defintion (sbitmap *gen, sbitmap *kill, sbitmap *in,
			    sbitmap *out)
{
  edge e;
  basic_block *worklist, *qin, *qout, *qend, bb;
  unsigned int qlen;
  edge_iterator ei;

  /* Allocate a worklist array/queue.  Entries are only added to the
     list if they were not already on the list.  So the size is
     bounded by the number of basic blocks.  */
  qin = qout = worklist
    = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS);

  /* Put every block on the worklist; this is necessary because of the
     optimistic initialization of AVOUT above.  Use reverse postorder
     to make the forward dataflow problem require less iterations.  */
  int *rpo = XNEWVEC (int, n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS);
  int n = pre_and_rev_post_order_compute_fn (cfun, NULL, rpo, false);
  for (int i = 0; i < n; ++i)
    {
      bb = BASIC_BLOCK_FOR_FN (cfun, rpo[i]);
      *qin++ = bb;
      bb->aux = bb;
    }
  free (rpo);

  qin = worklist;
  qend = &worklist[n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS];
  qlen = n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS;

  /* Mark blocks which are successors of the entry block so that we
     can easily identify them below.  */
  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR_FOR_FN (cfun)->succs)
    e->dest->aux = ENTRY_BLOCK_PTR_FOR_FN (cfun);

  /* Iterate until the worklist is empty.  */
  while (qlen)
    {
      /* Take the first entry off the worklist.  */
      bb = *qout++;
      qlen--;

      if (qout >= qend)
	qout = worklist;

      /* Do not clear the aux field for blocks which are successors of the
	 ENTRY block.  That way we never add then to the worklist again.  */
      if (bb->aux != ENTRY_BLOCK_PTR_FOR_FN (cfun))
	bb->aux = NULL;

      bitmap_union_of_preds_with_entry (in[bb->index], out, bb);

      if (bitmap_ior_and_compl (out[bb->index], gen[bb->index], in[bb->index],
				kill[bb->index]))
	/* If the out state of this block changed, then we need
	   to add the successors of this block to the worklist
	   if they are not already on the worklist.  */
	FOR_EACH_EDGE (e, ei, bb->succs)
	  if (!e->dest->aux && e->dest != EXIT_BLOCK_PTR_FOR_FN (cfun))
	    {
	      *qin++ = e->dest;
	      e->dest->aux = e;
	      qlen++;

	      if (qin >= qend)
		qin = worklist;
	    }
    }

  clear_aux_for_edges ();
  clear_aux_for_blocks ();
  free (worklist);
}

/* Change insn and Assert the change always happens.  */
void
validate_change_or_fail (rtx object, rtx *loc, rtx new_rtx, bool in_group)
{
  bool change_p = validate_change (object, loc, new_rtx, in_group);
  gcc_assert (change_p);
}

/* Count the number of REGNO in RINSN.  */
int
count_regno_occurrences (rtx_insn *rinsn, unsigned int regno)
{
  int count = 0;
  extract_insn (rinsn);
  for (int i = 0; i < recog_data.n_operands; i++)
    if (refers_to_regno_p (regno, recog_data.operand[i]))
      count++;
  return count;
}

static rtx
get_fist_clobber_rtx (rtx_insn *rinsn)
{
  subrtx_iterator::array_type array;
  FOR_EACH_SUBRTX (iter, array, PATTERN (rinsn), ALL)
    if (*iter && GET_CODE (*iter) == CLOBBER
	&& REG_P (XEXP (*iter, 0)))
      return XEXP (*iter, 0);

  return NULL_RTX;
}

enum emit_type
{
  /* emit_insn directly.  */
  EMIT_DIRECT,
  EMIT_BEFORE,
  EMIT_AFTER,
};

enum attr_type g_current_mcfg_type = TYPE_UNKNOWN;
rtx g_mcfg_reg = NULL_RTX;

static void set_current_mcfg_status (enum attr_type type)
{
  switch (type)
  {
    case TYPE_MCFGM:
      g_current_mcfg_type = TYPE_MCFGM;
      g_mcfg_reg = gen_rtx_REG (Pmode, MSIZE_M_REGNUM);
      break;
    case TYPE_MCFGN:
      g_current_mcfg_type = TYPE_MCFGN;
      g_mcfg_reg = gen_rtx_REG (Pmode, MSIZE_N_REGNUM);
      break;
    case TYPE_MCFGK:
      g_current_mcfg_type = TYPE_MCFGK;
      g_mcfg_reg = gen_rtx_REG (Pmode, MSIZE_K_REGNUM);
      break;
    default:
      g_current_mcfg_type = TYPE_UNKNOWN;
      g_mcfg_reg = NULL_RTX;
      break;
  }
}

static bool current_mcfgm_p ()
{
  return g_current_mcfg_type == TYPE_MCFGM;
}

static bool current_mcfgn_p ()
{
  return g_current_mcfg_type == TYPE_MCFGN;
}

static bool current_mcfgk_p ()
{
  return g_current_mcfg_type == TYPE_MCFGK;
}

/* Return true if it is a mcfg instruction.  */
static bool
matrix_config_insn_p (rtx_insn *rinsn)
{
  return recog_memoized (rinsn) >= 0 && get_attr_type (rinsn) == g_current_mcfg_type;
}

/* Return true if it is mcfgdi or mcfgsi.  */
static bool
mcfg_insn_p (rtx_insn *rinsn)
{
  if (!rinsn || !matrix_config_insn_p (rinsn))
    return false;

  return (INSN_CODE (rinsn) == CODE_FOR_matrix_mcfgmdi
	  || INSN_CODE (rinsn) == CODE_FOR_matrix_mcfgmsi
	  || INSN_CODE (rinsn) == CODE_FOR_matrix_mcfgndi
	  || INSN_CODE (rinsn) == CODE_FOR_matrix_mcfgnsi
	  || INSN_CODE (rinsn) == CODE_FOR_matrix_mcfgkdi
	  || INSN_CODE (rinsn) == CODE_FOR_matrix_mcfgksi);
}

/* Return true if it is the bogus mcfg_pre instruction.  */
static bool
real_insn_and_same_bb_p (const insn_info *insn, const bb_info *bb)
{
  return insn != nullptr && insn->is_real () && insn->bb () == bb;
}

/* Helper function to get MCFG operand for CMFG insn.  */
static rtx
get_mcfg (rtx_insn *rinsn)
{
  if (!matrix_config_insn_p (rinsn))
   return NULL_RTX;

  return g_mcfg_reg;
}

/* Helper function to get AMCFG operand.  */
static rtx
get_amcfg (rtx_insn *rinsn)
{
  extract_insn_cached (rinsn);
  if (mcfg_insn_p (rinsn))
    return recog_data.operand[1];

  int index = INVALID_ATTRIBUTE;
  if (current_mcfgm_p ())
    index = get_attr_matrix_mcfgm_op_idx (rinsn);
  else if (current_mcfgn_p ())
    index = get_attr_matrix_mcfgn_op_idx (rinsn);
  else if (current_mcfgk_p ())
    index = get_attr_matrix_mcfgk_op_idx (rinsn);
  else
    gcc_unreachable ();

  if (index == INVALID_ATTRIBUTE)
      return NULL_RTX;

  return recog_data.operand[index];
}

/* Return true if it is an RVM instruction depends on MCFG global
   status register.  */
static bool
has_mcfg_op (rtx_insn *rinsn)
{
  return recog_memoized (rinsn) >= 0
	 && get_attr_th_m_has_mcfg_op (rinsn);
}

/* Return true if FN has a matrix instruction that use MCFG.  */
static bool
has_matrix_insn (function *fn)
{
  basic_block bb;
  rtx_insn *rinsn;
  FOR_ALL_BB_FN (bb, fn)
    FOR_BB_INSNS (bb, rinsn)
      if (NONDEBUG_INSN_P (rinsn) && has_mcfg_op (rinsn))
	return true;
  return false;
}

enum def_type
{
  REAL_SET = 1 << 0,
  PHI_SET = 1 << 1,
  BB_HEAD_SET = 1 << 2,
  BB_END_SET = 1 << 3,
  /* ??? TODO: In RTL_SSA framework, we have REAL_SET,
     PHI_SET, BB_HEAD_SET, BB_END_SET and
     CLOBBER_DEF def_info types. Currently,
     we conservatively do not optimize clobber
     def since we don't see the case that we
     need to optimize it.  */
  CLOBBER_DEF = 1 << 4
};

static bool
insn_should_be_added_p (const insn_info *insn, unsigned int types)
{
  if (insn->is_real () && (types & REAL_SET))
    return true;
  if (insn->is_phi () && (types & PHI_SET))
    return true;
  if (insn->is_bb_head () && (types & BB_HEAD_SET))
    return true;
  if (insn->is_bb_end () && (types & BB_END_SET))
    return true;
  return false;
}

/* Recursively find all define instructions. The kind of instruction is
   specified by the DEF_TYPE.  */
static hash_set<set_info *>
get_all_sets (phi_info *phi, unsigned int types)
{
  hash_set<set_info *> insns;
  auto_vec<phi_info *> work_list;
  hash_set<phi_info *> visited_list;
  if (!phi)
    return hash_set<set_info *> ();
  work_list.safe_push (phi);

  while (!work_list.is_empty ())
    {
      phi_info *phi = work_list.pop ();
      visited_list.add (phi);
      for (use_info *use : phi->inputs ())
	{
	  def_info *def = use->def ();
	  set_info *set = safe_dyn_cast<set_info *> (def);
	  if (!set)
	    return hash_set<set_info *> ();

	  gcc_assert (!set->insn ()->is_debug_insn ());

	  if (insn_should_be_added_p (set->insn (), types))
	    insns.add (set);
	  if (set->insn ()->is_phi ())
	    {
	      phi_info *new_phi = as_a<phi_info *> (set);
	      if (!visited_list.contains (new_phi))
		work_list.safe_push (new_phi);
	    }
	}
    }
  return insns;
}

static hash_set<set_info *>
get_all_sets (set_info *set, bool /* get_real_inst */ real_p,
	      bool /*get_phi*/ phi_p, bool /* get_function_parameter*/ param_p)
{
  if (real_p && phi_p && param_p)
    return get_all_sets (safe_dyn_cast<phi_info *> (set),
			 REAL_SET | PHI_SET | BB_HEAD_SET | BB_END_SET);

  else if (real_p && param_p)
    return get_all_sets (safe_dyn_cast<phi_info *> (set),
			 REAL_SET | BB_HEAD_SET | BB_END_SET);

  else if (real_p)
    return get_all_sets (safe_dyn_cast<phi_info *> (set), REAL_SET);
  return hash_set<set_info *> ();
}

static bool
source_equal_p (insn_info *insn1, insn_info *insn2)
{
  if (!insn1 || !insn2)
    return false;
  rtx_insn *rinsn1 = insn1->rtl ();
  rtx_insn *rinsn2 = insn2->rtl ();
  if (!rinsn1 || !rinsn2)
    return false;

  rtx note1 = find_reg_equal_equiv_note (rinsn1);
  rtx note2 = find_reg_equal_equiv_note (rinsn2);
  /* We could handle the case of similar-looking REG_EQUALs as well but
     would need to verify that no insn in between modifies any of the source
     operands.  */
  if (note1 && note2 && rtx_equal_p (note1, note2)
      && REG_NOTE_KIND (note1) == REG_EQUIV)
    return true;
  return false;
}

static insn_info *
extract_single_source (set_info *set)
{
  if (!set)
    return nullptr;
  if (set->insn ()->is_real ())
    return set->insn ();
  if (!set->insn ()->is_phi ())
    return nullptr;
  hash_set<set_info *> sets = get_all_sets (set, true, false, true);
  if (sets.is_empty ())
    return nullptr;

  insn_info *first_insn = (*sets.begin ())->insn ();
  if (first_insn->is_artificial ())
    return nullptr;
  for (const set_info *set : sets)
    {
      /* If there is a head or end insn, we conservative return
	 NULL so that MCFG PASS will insert mcfg directly.  */
      if (set->insn ()->is_artificial ())
	return nullptr;
      if (set != *sets.begin () && !source_equal_p (set->insn (), first_insn))
	return nullptr;
    }

  return first_insn;
}

static bool
same_equiv_note_p (set_info *set1, set_info *set2)
{
  insn_info *insn1 = extract_single_source (set1);
  insn_info *insn2 = extract_single_source (set2);
  if (!insn1 || !insn2)
    return false;
  return source_equal_p (insn1, insn2);
}

/* Return true if the SET result is not used by any instructions.  */
static bool
has_no_uses (basic_block cfg_bb, rtx_insn *rinsn, int regno)
{
  if (bitmap_bit_p (df_get_live_out (cfg_bb), regno))
    return false;

  rtx_insn *iter;
  for (iter = NEXT_INSN (rinsn); iter && iter != NEXT_INSN (BB_END (cfg_bb));
       iter = NEXT_INSN (iter))
    if (df_find_use (iter, regno_reg_rtx[regno]))
      return false;

  return true;
}

/* Return true for the special block that we can't apply LCM optimization.  */
static bool
invalid_opt_bb_p (basic_block cfg_bb)
{
  edge e;
  edge_iterator ei;

  /* We don't do LCM optimizations on complex edges.  */
  FOR_EACH_EDGE (e, ei, cfg_bb->preds)
    if (e->flags & EDGE_COMPLEX)
      return true;

  /* We only do LCM optimizations on blocks that are post dominated by
     EXIT block, that is, we don't do LCM optimizations on infinite loop.  */
  FOR_EACH_EDGE (e, ei, cfg_bb->succs)
    if (e->flags & EDGE_FAKE)
      return true;

  return false;
}

/* Get all predecessors of BB.  */
static hash_set<basic_block>
get_all_predecessors (basic_block bb)
{
  hash_set<basic_block> blocks;
  auto_vec<basic_block> work_list;
  hash_set<basic_block> visited_list;
  work_list.safe_push (bb);

  while (!work_list.is_empty ())
    {
      basic_block new_bb = work_list.pop ();
      visited_list.add (new_bb);
      edge e;
      edge_iterator ei;
      FOR_EACH_EDGE (e, ei, new_bb->preds)
	{
	  if (!visited_list.contains (e->src))
	    work_list.safe_push (e->src);
	  blocks.add (e->src);
	}
    }
  return blocks;
}


class mcfg_info
{
private:
  insn_info *m_insn;
  bb_info *m_bb;
  rtx m_amcfg;
  rtx m_scratch;
  int m_sew;
  set_info *m_amcfg_def;

  enum class state_type
  {
    UNINITIALIZED,
    VALID,
    UNKNOWN,
    EMPTY,
  };
  state_type m_state;

  bool m_delete;

public:
  mcfg_info ()
    : m_insn (nullptr), m_bb (nullptr), m_amcfg (NULL_RTX), m_scratch (NULL_RTX), m_sew (0),
      m_amcfg_def (nullptr), m_state (state_type::UNINITIALIZED),
      m_delete (false)
  {}

  mcfg_info (insn_info *insn) : mcfg_info () { parse_insn (insn); }

  mcfg_info (rtx_insn *insn) : mcfg_info () { parse_insn (insn); }

  void set_amcfg (rtx amcfg) { m_amcfg = amcfg; }
  void set_amcfg_def (set_info *amcfg_def) { m_amcfg_def = amcfg_def; }
  void set_delete () { m_delete = true; }
  void set_bb (bb_info *bb) { m_bb = bb; }
  void set_sew (int sew) { m_sew = sew; }

  rtx get_amcfg () const { return m_amcfg; }
  set_info *get_amcfg_def () const { return m_amcfg_def; }
  insn_info *get_insn () const { return m_insn; }
  bool delete_p () const { return m_delete; }
  bb_info *get_bb () const { return m_bb; }
  int get_sew () const { return m_sew; }

  bool has_imm_amcfg () const { return m_amcfg && CONST_INT_P (m_amcfg); }
  bool has_nonmcfg_reg_amcfg () const
  {
    return m_amcfg && REG_P (m_amcfg);
  }

  /* The block of INSN isn't always same as the block of the MCFG_INFO,
      meaning we may have 'get_insn ()->bb () != get_bb ()'.

	E.g.  BB 2 (Empty) ---> BB 3 (VALID, has RVM insn 1)

     BB 2 has empty MCFG_INFO, wheras BB 3 has MCFG_INFO that satisfies
     get_insn ()->bb () == get_bb (). In earliest fusion, we may fuse bb 3 and
     bb 2 so that the 'get_bb ()' of BB2 MCFG_INFO will be BB2 wheras the
     'get_insn ()' of BB2 MCFG INFO will be the RVM insn 1 (which is located
     at BB3).  */
  bool insn_inside_bb_p () const { return get_insn ()->bb () == get_bb (); }
  void update_amcfg (const mcfg_info &other)
  {
    m_amcfg = other.get_amcfg ();
    m_amcfg_def = other.get_amcfg_def ();
  }

  bool uninit_p () const { return m_state == state_type::UNINITIALIZED; }
  bool valid_p () const { return m_state == state_type::VALID; }
  bool unknown_p () const { return m_state == state_type::UNKNOWN; }
  bool empty_p () const { return m_state == state_type::EMPTY; }

  void set_valid () { m_state = state_type::VALID; }
  void set_unknown () { m_state = state_type::UNKNOWN; }
  void set_empty () { m_state = state_type::EMPTY; }

  void parse_insn (rtx_insn *rinsn)
  {
    if (!NONDEBUG_INSN_P (rinsn))
      return;
    if (optimize == 0 && !has_mcfg_op (rinsn))
      return;

    set_valid ();
    extract_insn_cached (rinsn);
    m_amcfg = ::get_amcfg (rinsn);
    m_sew = get_attr_matrix_ebytes (rinsn);
    m_scratch = get_fist_clobber_rtx (rinsn);
  }

  void parse_insn (insn_info *insn)
  {
    m_insn = insn;
    m_bb = insn->bb ();
    /* Return if it is debug insn for the consistency with optimize == 0.  */
    if (insn->is_debug_insn ())
      return;

    /* We set it as unknown since we don't what will happen in CALL or ASM.  */
    if (insn->is_call () || insn->is_asm ())
      {
	set_unknown ();
	return;
      }

    /* If this is something that updates MCFG that we don't know about, set
       the state to unknown.  */
    if (!matrix_config_insn_p (insn->rtl ()) && !has_mcfg_op (insn->rtl ())
	&& (find_access (insn->defs (), REGNO (g_mcfg_reg))))
      {
	set_unknown ();
	return;
      }

    if (!matrix_config_insn_p (insn->rtl ()) && !has_mcfg_op (insn->rtl ()))
      /* uninitialized */
      return;

    m_amcfg = ::get_amcfg (insn->rtl ());
    if (m_amcfg)
      {
	set_valid ();
	if (has_nonmcfg_reg_amcfg ())
	  m_amcfg_def = find_access (insn->uses (), REGNO (m_amcfg))->def ();

	m_sew = get_attr_matrix_ebytes (insn->rtl ());
	m_scratch = get_fist_clobber_rtx (insn->rtl ());
      }
  }

  /* Returns the corresponding mcfg rtx pat.  */
  rtx get_mcfg_pat () const
  {
    rtx amcfg = get_amcfg ();
    if (!amcfg)
      return NULL_RTX;

    if (current_mcfgm_p ())
      return gen_matrix_mcfgm_discard_result (Pmode, amcfg);
    else if (current_mcfgn_p ())
      return gen_matrix_mcfgn_discard_result (Pmode, amcfg);
    else if (current_mcfgk_p ())
      {
	int shift = exact_log2 (m_sew);
	if (CONST_INT_P (amcfg))
	  return gen_matrix_mcfgk_discard_result (Pmode, GEN_INT (INTVAL (amcfg) << shift));
	else
	  {
	    if (shift != 0)
	      {
		gcc_assert (m_scratch);
		return gen_matrix_mcfgk_with_shift_discard_result (Pmode, amcfg, m_scratch, GEN_INT (exact_log2 (m_sew)));
	      }
	    else
	      return gen_matrix_mcfgk_discard_result (Pmode, amcfg);
	  }
      }
    else
      gcc_unreachable ();

    return NULL_RTX;
  }

  bool operator== (const mcfg_info &other) const
  {
    gcc_assert (!uninit_p () && !other.uninit_p ()
		&& "Uninitialization should not happen");

    if (empty_p ())
      return other.empty_p ();
    if (unknown_p ())
      return other.unknown_p ();

    return get_insn () == other.get_insn () && get_bb () == other.get_bb ()
	   && get_amcfg () == other.get_amcfg ()
	   && get_amcfg_def () == other.get_amcfg_def ()
	   && get_sew() == other.get_sew();
  }

  void dump (FILE *file, const char *indent = "") const
  {
    if (uninit_p ())
      {
	fprintf (file, "UNINITIALIZED.\n");
	return;
      }
    else if (unknown_p ())
      {
	fprintf (file, "UNKNOWN.\n");
	return;
      }
    else if (empty_p ())
      {
	fprintf (file, "EMPTY.\n");
	return;
      }
    else if (valid_p ())
      fprintf (file, "VALID (insn %u, bb %u)%s\n", get_insn ()->uid (),
	       get_bb ()->index (), delete_p () ? " (deleted)" : "");
    else
      gcc_unreachable ();

    fprintf (file, "%sAMCFG=", indent);
    print_rtl_single (file, get_amcfg ());
  }
};

class mcfg_block_info
{
public:
  /* The static execute probability of the demand info.  */
  profile_probability probability;

  auto_vec<mcfg_info> local_infos;
  mcfg_info global_info;
  bb_info *bb;

  mcfg_block_info () : bb (nullptr)
  {
    local_infos.safe_grow_cleared (0);
    global_info.set_empty ();
  }
  mcfg_block_info (const mcfg_block_info &other)
    : probability (other.probability),
      global_info (other.global_info), bb (other.bb)
  {
     (vec<mcfg_info>) local_infos =other.local_infos.copy ();
  }

  mcfg_info &get_entry_info ()
  {
    gcc_assert (!empty_p ());
    return local_infos.is_empty () ? global_info : local_infos[0];
  }
  mcfg_info &get_exit_info ()
  {
    gcc_assert (!empty_p ());
    return local_infos.is_empty () ? global_info
				   : local_infos[local_infos.length () - 1];
  }
  const mcfg_info &get_entry_info () const
  {
    gcc_assert (!empty_p ());
    return local_infos.is_empty () ? global_info : local_infos[0];
  }
  const mcfg_info &get_exit_info () const
  {
    gcc_assert (!empty_p ());
    return local_infos.is_empty () ? global_info
				   : local_infos[local_infos.length () - 1];
  }

  bool empty_p () const { return local_infos.is_empty () && !has_info (); }
  bool has_info () const { return !global_info.empty_p (); }
  void set_info (const mcfg_info &info)
  {
    gcc_assert (local_infos.is_empty ());
    global_info = info;
    global_info.set_bb (bb);
  }
  void set_empty_info () { global_info.set_empty (); }
};

/* Demand system is the RVM-based MCFG info analysis tools wrapper.
   It defines compatible rules for SEW/LMUL, POLICY and AMCFG.
   Also, it provides 3 iterfaces avaiable_p, compatible_p and
   merge for the MCFG PASS analysis and optimization.

     - avaiable_p: Determine whether the next info can get the
       avaiable MCFG status from previous info.
       e.g. bb 2 (demand SEW = 32, LMUL = M2) -> bb 3 (demand RATIO = 16).
       Since bb 2 demand info (SEW/LMUL = 32/2 = 16) satisfies the bb 3
       demand, the MCFG instruction in bb 3 can be elided.
       avaiable_p (previous, next) is true in such situation.
     - compatible_p: Determine whether prev_info is compatible with next_info
       so that we can have a new merged info that is avaiable to both of them.
     - merge: Merge the stricter demand information from
       next_info into prev_info so that prev_info becomes available to
       next_info.  */
class demand_system
{
private:
  /* predictors for amcfg */

  inline bool modify_amcfg_p (insn_info *i, const mcfg_info &info)
  {
    return info.has_nonmcfg_reg_amcfg ()
	   && find_access (i->defs (), REGNO (info.get_amcfg ()));
  }

  inline bool modify_reg_between_p (insn_info *prev_insn, insn_info *curr_insn,
				    unsigned regno)
  {
    gcc_assert (prev_insn->compare_with (curr_insn) < 0);
    for (insn_info *i = curr_insn->prev_nondebug_insn (); i != prev_insn;
	 i = i->prev_nondebug_insn ())
      {
	// no def of regno
	if (find_access (i->defs (), regno))
	  return true;
      }
    return false;
  }

  inline bool reg_amcfg_equal_p (const mcfg_info &prev, const mcfg_info &next)
  {
    if (!prev.has_nonmcfg_reg_amcfg () || !next.has_nonmcfg_reg_amcfg ())
      return false;

    if (same_equiv_note_p (prev.get_amcfg_def (), next.get_amcfg_def ()))
      return true;

    if (REGNO (prev.get_amcfg ()) != REGNO (next.get_amcfg ()))
      return false;

    insn_info *prev_insn = prev.get_insn ();
    if (prev.get_bb () != prev_insn->bb ())
      prev_insn = prev.get_bb ()->end_insn ();

    insn_info *next_insn = next.get_insn ();
    if (next.get_bb () != next_insn->bb ())
      next_insn = next.get_bb ()->end_insn ();

    return amcfg_mcfg_unmodified_between_p (prev_insn, next_insn, next, false);
  }

  inline bool amcfg_equal_p (const mcfg_info &prev, const mcfg_info &next)
  {
    gcc_assert (prev.valid_p () && next.valid_p ());

    if (current_mcfgk_p () && prev.get_sew () != next.get_sew ())
      return false;

    if (matrix_config_insn_p (prev.get_insn ()->rtl ()) && next.get_amcfg_def ()
	&& next.get_amcfg_def ()->insn () == prev.get_insn ())
      return true;

    if (prev == next && prev.has_nonmcfg_reg_amcfg ())
      {
	insn_info *insn = prev.get_insn ();
	bb_info *bb = insn->bb ();
	for (insn_info *i = insn; real_insn_and_same_bb_p (i, bb);
	     i = i->next_nondebug_insn ())
	  if (find_access (i->defs (), REGNO (prev.get_amcfg ())))
	    return false;
      }

    if (prev.has_imm_amcfg () && next.has_imm_amcfg ())
      return INTVAL (prev.get_amcfg ()) == INTVAL (next.get_amcfg ());
    else if (prev.has_nonmcfg_reg_amcfg () && next.has_nonmcfg_reg_amcfg ())
      return reg_amcfg_equal_p (prev, next);

    return false;
  }

  inline bool can_use_next_amcfg_p (const mcfg_info &prev,
				  const mcfg_info &next)
  {
    if (!next.has_nonmcfg_reg_amcfg ())
      return true;

    insn_info *prev_insn = prev.get_insn ();
    if (prev.get_bb () != prev_insn->bb ())
      prev_insn = prev.get_bb ()->end_insn ();

    insn_info *next_insn = next.get_insn ();
    if (next.get_bb () != next_insn->bb ())
      next_insn = next.get_bb ()->end_insn ();

    return amcfg_mcfg_unmodified_between_p (prev_insn, next_insn, next);
  }

  inline bool amcfg_equal_or_next_amcfg_non_zero_and_can_use_next_amcfg_p (
    const mcfg_info &prev, const mcfg_info &next)
  {
    return amcfg_equal_p (prev, next);
  }

  /* modifiers for amcfg */

  inline void use_next_amcfg (mcfg_info &prev, const mcfg_info &next)
  {
    gcc_assert (can_use_next_amcfg_p (prev, next));
    prev.update_amcfg (next);
  }


public:
  /* Can we move mcfg info between prev_insn and next_insn safe? */
  bool amcfg_mcfg_unmodified_between_p (insn_info *prev_insn, insn_info *next_insn,
				    const mcfg_info &info,
				    bool ignore_mcfg = false)
  {
    gcc_assert ((ignore_mcfg && info.has_nonmcfg_reg_amcfg ())
		|| (info.has_nonmcfg_reg_amcfg ()));

    gcc_assert (!prev_insn->is_debug_insn () && !next_insn->is_debug_insn ());
    if (prev_insn->bb () == next_insn->bb ()
	&& prev_insn->compare_with (next_insn) < 0)
      {
	for (insn_info *i = next_insn->prev_nondebug_insn (); i != prev_insn;
	     i = i->prev_nondebug_insn ())
	  {
	    // no def of amcfg
	    if (modify_amcfg_p (i, info))
	      return false;
	  }
	return true;
      }
    else
      {
	/* Find set_info at location of PREV_INSN and NEXT_INSN, Return
	   false if those 2 set_info are different.

	     PREV_INSN --- multiple nested blocks --- NEXT_INSN.

	   Return false if there is any modifications of AMCFG inside those
	   multiple nested blocks.  */
	if (info.has_nonmcfg_reg_amcfg ())
	  {
	    resource_info resource = full_register (REGNO (info.get_amcfg ()));
	    def_lookup dl1 = crtl->ssa->find_def (resource, prev_insn);
	    def_lookup dl2 = crtl->ssa->find_def (resource, next_insn);
	    if (dl2.matching_set ())
	      return false;

	    auto is_phi_or_real
	      = [&] (insn_info *h) { return h->is_real () || h->is_phi (); };

	    def_info *def1 = dl1.matching_set_or_last_def_of_prev_group ();
	    def_info *def2 = dl2.prev_def (next_insn);
	    set_info *set1 = safe_dyn_cast<set_info *> (def1);
	    set_info *set2 = safe_dyn_cast<set_info *> (def2);
	    if (!set1 || !set2)
	      return false;

	    auto is_same_ultimate_def = [&] (set_info *s1, set_info *s2) {
	      return s1->insn ()->is_phi () && s2->insn ()->is_phi ()
		     && look_through_degenerate_phi (s1)
			  == look_through_degenerate_phi (s2);
	    };

	    if (set1 != set2 && !is_same_ultimate_def (set1, set2))
	      {
		if (!is_phi_or_real (set1->insn ())
		    || !is_phi_or_real (set2->insn ()))
		  return false;

		if (set1->insn ()->is_real () && set2->insn ()->is_phi ())
		  {
		    hash_set<set_info *> sets
		      = get_all_sets (set2, true, false, true);
		    if (!sets.contains (set1))
		      return false;
		  }
		else
		  {
		    insn_info *def_insn1 = extract_single_source (set1);
		    insn_info *def_insn2 = extract_single_source (set2);
		    if (!def_insn1 || !def_insn2 || def_insn1 != def_insn2)
		      return false;
		  }
	      }
	  }

	for (insn_info *i = next_insn; i != next_insn->bb ()->head_insn ();
	     i = i->prev_nondebug_insn ())
	  {
	    // no def of amcfg
	    if (modify_amcfg_p (i, info))
	      return false;
	  }

	for (insn_info *i = prev_insn->bb ()->end_insn (); i != prev_insn;
	     i = i->prev_nondebug_insn ())
	  {
	    // no def of amcfg
	    if (modify_amcfg_p (i, info))
	      return false;
	  }
      }
    return true;
  }

  bool compatible_p (const mcfg_info &prev, const mcfg_info &next)
  {
    return available_p (prev, next);
  }

  bool amcfg_available_p (const mcfg_info &prev, const mcfg_info &next)
  {
    gcc_assert (prev.valid_p () && next.valid_p ());

    return amcfg_equal_p (prev, next);
  }

  bool available_p (const mcfg_info &prev, const mcfg_info &next)
  {
    bool available_p = amcfg_available_p (prev, next);
    return available_p;
  }

  void merge (mcfg_info &prev, const mcfg_info &next)
  {
    gcc_assert (prev.valid_p () && next.valid_p ());
  }
};


class pre_mcfg
{
private:
  demand_system m_dem;
  auto_vec<mcfg_block_info> m_matrix_block_infos;

  /* data for amcfg reaching defintion.  */
  sbitmap *m_reg_def_loc;

  /* data for mcfg info reaching defintion.  */
  mcfg_info m_unknow_info;
  auto_vec<mcfg_info *> m_mcfg_def_exprs;
  sbitmap *m_mcfg_def_in;
  sbitmap *m_mcfg_def_out;

  /* data for lcm */
  auto_vec<mcfg_info *> m_exprs;
  sbitmap *m_avloc;
  sbitmap *m_avin;
  sbitmap *m_avout;
  sbitmap *m_kill;
  sbitmap *m_antloc;
  sbitmap *m_transp;
  sbitmap *m_insert;
  sbitmap *m_del;
  struct edge_list *m_edges;

  auto_vec<mcfg_info> m_delete_list;

  mcfg_block_info &get_block_info (const bb_info *bb)
  {
    return m_matrix_block_infos[bb->index ()];
  }
  const mcfg_block_info &get_block_info (const basic_block bb) const
  {
    return m_matrix_block_infos[bb->index];
  }

  mcfg_block_info &get_block_info (const basic_block bb)
  {
    return m_matrix_block_infos[bb->index];
  }

  void add_expr (auto_vec<mcfg_info *> &m_exprs, mcfg_info &info)
  {
    for (mcfg_info *item : m_exprs)
      {
	if (*item == info)
	  return;
      }
    m_exprs.safe_push (&info);
  }

  unsigned get_expr_index (auto_vec<mcfg_info *> &m_exprs,
			   const mcfg_info &info)
  {
    for (size_t i = 0; i < m_exprs.length (); i += 1)
      {
	if (*m_exprs[i] == info)
	  return i;
      }
    gcc_unreachable ();
  }

  bool anticipated_exp_p (const mcfg_info &header_info)
  {
    if (!header_info.has_nonmcfg_reg_amcfg ())
      return true;

    bb_info *bb = header_info.get_bb ();
    insn_info *prev_insn = bb->head_insn ();
    insn_info *next_insn = header_info.insn_inside_bb_p ()
			     ? header_info.get_insn ()
			     : header_info.get_bb ()->end_insn ();

    return m_dem.amcfg_mcfg_unmodified_between_p (prev_insn, next_insn,
					      header_info);
  }

  bool available_exp_p (const mcfg_info &prev_info,
			const mcfg_info &next_info)
  {
    return m_dem.available_p (prev_info, next_info);
  }

  void compute_probabilities ()
  {
    edge e;
    edge_iterator ei;

    for (const bb_info *bb : crtl->ssa->bbs ())
      {
	basic_block cfg_bb = bb->cfg_bb ();
	auto &curr_prob = get_block_info (cfg_bb).probability;

	/* GCC assume entry block (bb 0) are always so
	   executed so set its probability as "always".  */
	if (ENTRY_BLOCK_PTR_FOR_FN (cfun) == cfg_bb)
	  curr_prob = profile_probability::always ();
	/* Exit block (bb 1) is the block we don't need to process.  */
	if (EXIT_BLOCK_PTR_FOR_FN (cfun) == cfg_bb)
	  continue;

	gcc_assert (curr_prob.initialized_p ());
	FOR_EACH_EDGE (e, ei, cfg_bb->succs)
	  {
	    auto &new_prob = get_block_info (e->dest).probability;
	    /* Normally, the edge probability should be initialized.
	       However, some special testing code which is written in
	       GIMPLE IR style force the edge probility uninitialized,
	       we conservatively set it as never so that it will not
	       affect PRE (Phase 3 && Phse 4).  */
	    if (!e->probability.initialized_p ())
	      new_prob = profile_probability::never ();
	    else if (!new_prob.initialized_p ())
	      new_prob = curr_prob * e->probability;
	    else if (new_prob == profile_probability::always ())
	      continue;
	    else
	      new_prob += curr_prob * e->probability;
	  }
      }
  }

  void insert_mcfg_insn (enum emit_type emit_type, const mcfg_info &info)
  {
    rtx pat = info.get_mcfg_pat ();
    rtx_insn *rinsn = info.get_insn ()->rtl ();

    if (emit_type == EMIT_DIRECT)
      {
	emit_insn (pat);
	if (dump_file)
	  {
	    fprintf (dump_file, "  Insert mcfg insn %d:\n",
		     INSN_UID (get_last_insn ()));
	    print_rtl_single (dump_file, get_last_insn ());
	  }
      }
    else if (emit_type == EMIT_BEFORE)
      {
	emit_insn_before (pat, rinsn);
	if (dump_file)
	  {
	    fprintf (dump_file, "  Insert mcfg insn before insn %d:\n",
		     INSN_UID (rinsn));
	    print_rtl_single (dump_file, PREV_INSN (rinsn));
	  }
      }
    else
      {
	emit_insn_after (pat, rinsn);
	if (dump_file)
	  {
	    fprintf (dump_file, "  Insert mcfg insn after insn %d:\n",
		     INSN_UID (rinsn));
	    print_rtl_single (dump_file, NEXT_INSN (rinsn));
	  }
      }
  }

  void change_mcfg_insn (const mcfg_info &info)
  {
    rtx_insn *rinsn = info.get_insn ()->rtl ();
    rtx new_pat = info.get_mcfg_pat ();

    if (dump_file)
      {
	fprintf (dump_file, "  Change insn %d from:\n", INSN_UID (rinsn));
	print_rtl_single (dump_file, rinsn);
      }

    validate_change_or_fail (rinsn, &PATTERN (rinsn), new_pat, false);

    if (dump_file)
      {
	fprintf (dump_file, "\n  to:\n");
	print_rtl_single (dump_file, rinsn);
      }
  }

  void remove_mcfg_insn (rtx_insn *rinsn)
  {
    if (dump_file)
      {
	fprintf (dump_file, "  Eliminate insn %d:\n", INSN_UID (rinsn));
	print_rtl_single (dump_file, rinsn);
      }
    if (in_sequence_p ())
      remove_insn (rinsn);
    else
      delete_insn (rinsn);
  }

  bool successors_probability_equal_p (const basic_block cfg_bb) const
  {
    edge e;
    edge_iterator ei;
    profile_probability prob = profile_probability::uninitialized ();
    FOR_EACH_EDGE (e, ei, cfg_bb->succs)
      {
	if (prob == profile_probability::uninitialized ())
	  prob = m_matrix_block_infos[e->dest->index].probability;
	else if (prob == m_matrix_block_infos[e->dest->index].probability)
	  continue;
	else
	  /* We pick the highest probability among those incompatible MCFG
	     infos. When all incompatible MCFG infos have same probability, we
	     don't pick any of them.  */
	  return false;
      }
    return true;
  }

  bool has_compatible_reaching_mcfg_p (mcfg_info info)
  {
    unsigned int index;
    sbitmap_iterator sbi;
    EXECUTE_IF_SET_IN_BITMAP (m_mcfg_def_in[info.get_bb ()->index ()], 0,
			      index, sbi)
      {
	const auto prev_info = *m_mcfg_def_exprs[index];
	if (!prev_info.valid_p ())
	  continue;
	if (m_dem.compatible_p (prev_info, info))
	  return true;
      }
    return false;
  }

  bool preds_all_same_amcfg_and_ratio_p (const mcfg_info &curr_info)
  {
    gcc_assert (
      !bitmap_empty_p (m_mcfg_def_in[curr_info.get_bb ()->index ()]));

    unsigned expr_index;
    sbitmap_iterator sbi;
    EXECUTE_IF_SET_IN_BITMAP (m_mcfg_def_in[curr_info.get_bb ()->index ()], 0,
			      expr_index, sbi)
      {
	const mcfg_info &prev_info = *m_mcfg_def_exprs[expr_index];
	if (!prev_info.valid_p ()
	    || !m_dem.amcfg_available_p (prev_info, curr_info))
	  return false;
      }

    return true;
  }

public:
  pre_mcfg ()
    : m_mcfg_def_in (nullptr), m_mcfg_def_out (nullptr), m_avloc (nullptr),
      m_avin (nullptr), m_avout (nullptr), m_kill (nullptr), m_antloc (nullptr),
      m_transp (nullptr), m_insert (nullptr), m_del (nullptr), m_edges (nullptr)
  {
    /* Initialization of RTL_SSA.  */
    calculate_dominance_info (CDI_DOMINATORS);
    loop_optimizer_init (LOOPS_NORMAL);
    /* Create FAKE edges for infinite loops.  */
    connect_infinite_loops_to_exit ();
    df_analyze ();
    crtl->ssa = new function_info (cfun);
    m_matrix_block_infos.safe_grow_cleared (last_basic_block_for_fn (cfun));
    compute_probabilities ();
    m_unknow_info.set_unknown ();
  }

  void finish ()
  {
    free_dominance_info (CDI_DOMINATORS);
    loop_optimizer_finalize ();
    if (crtl->ssa->perform_pending_updates ())
      cleanup_cfg (0);
    delete crtl->ssa;
    crtl->ssa = nullptr;

    if (m_reg_def_loc)
      sbitmap_vector_free (m_reg_def_loc);

    if (m_mcfg_def_in)
      sbitmap_vector_free (m_mcfg_def_in);
    if (m_mcfg_def_out)
      sbitmap_vector_free (m_mcfg_def_out);

    if (m_avloc)
      sbitmap_vector_free (m_avloc);
    if (m_kill)
      sbitmap_vector_free (m_kill);
    if (m_antloc)
      sbitmap_vector_free (m_antloc);
    if (m_transp)
      sbitmap_vector_free (m_transp);
    if (m_insert)
      sbitmap_vector_free (m_insert);
    if (m_del)
      sbitmap_vector_free (m_del);
    if (m_avin)
      sbitmap_vector_free (m_avin);
    if (m_avout)
      sbitmap_vector_free (m_avout);

    if (m_edges)
      free_edge_list (m_edges);
  }

  void compute_mcfg_def_data ();
  void compute_transparent (const bb_info *);
  void compute_lcm_local_properties ();

  void fuse_local_mcfg_info ();
  bool earliest_fuse_mcfg_info (int iter);
  void pre_global_mcfg_info ();
  void emit_mcfg ();
  void cleaup ();
  void remove_amcfg_operand ();
  void remove_unused_dest_operand ();

  void dump (FILE *file, const char *title) const
  {
    fprintf (file, "\nMCFG infos after %s\n\n", title);
    for (const bb_info *bb : crtl->ssa->bbs ())
      {
	const auto &block_info = m_matrix_block_infos[bb->index ()];
	fprintf (file, "  bb %d:\n", bb->index ());
	fprintf (file, "    probability: ");
	block_info.probability.dump (file);
	fprintf (file, "\n");
	if (!block_info.empty_p ())
	  {
	    fprintf (file, "    Header mcfg info:");
	    block_info.get_entry_info ().dump (file, "      ");
	    fprintf (file, "    Footer mcfg info:");
	    block_info.get_exit_info ().dump (file, "      ");
	    for (const auto &info : block_info.local_infos)
	      {
		fprintf (file,
			 "    insn %d mcfg info:", info.get_insn ()->uid ());
		info.dump (file, "      ");
	      }
	  }
      }
  }
};

void
pre_mcfg::compute_mcfg_def_data ()
{
  m_mcfg_def_exprs.truncate (0);
  add_expr (m_mcfg_def_exprs, m_unknow_info);
  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      mcfg_block_info &block_info = get_block_info (bb);
      if (block_info.empty_p ())
	continue;
      mcfg_info &footer_info = block_info.get_exit_info ();
      gcc_assert (footer_info.valid_p () || footer_info.unknown_p ());
      add_expr (m_mcfg_def_exprs, footer_info);
    }

  if (m_mcfg_def_in)
    sbitmap_vector_free (m_mcfg_def_in);
  if (m_mcfg_def_out)
    sbitmap_vector_free (m_mcfg_def_out);

  sbitmap *def_loc = sbitmap_vector_alloc (last_basic_block_for_fn (cfun),
					   m_mcfg_def_exprs.length ());
  sbitmap *m_kill = sbitmap_vector_alloc (last_basic_block_for_fn (cfun),
					  m_mcfg_def_exprs.length ());

  m_mcfg_def_in = sbitmap_vector_alloc (last_basic_block_for_fn (cfun),
					  m_mcfg_def_exprs.length ());
  m_mcfg_def_out = sbitmap_vector_alloc (last_basic_block_for_fn (cfun),
					   m_mcfg_def_exprs.length ());

  bitmap_vector_clear (def_loc, last_basic_block_for_fn (cfun));
  bitmap_vector_clear (m_kill, last_basic_block_for_fn (cfun));
  bitmap_vector_clear (m_mcfg_def_out, last_basic_block_for_fn (cfun));

  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      mcfg_block_info &block_info = get_block_info (bb);
      if (block_info.empty_p ())
	{
	  for (unsigned i = 0; i < m_mcfg_def_exprs.length (); i += 1)
	    {
	      auto *info = m_mcfg_def_exprs[i];
	      if (info->has_nonmcfg_reg_amcfg ()
		  && bitmap_bit_p (m_reg_def_loc[bb->index ()],
				   REGNO (info->get_amcfg ())))
		{
		  bitmap_set_bit (m_kill[bb->index ()], i);
		  bitmap_set_bit (def_loc[bb->index ()],
				  get_expr_index (m_mcfg_def_exprs,
						  m_unknow_info));
		}
	    }
	  continue;
	}

      mcfg_info &footer_info = block_info.get_exit_info ();
      bitmap_ones (m_kill[bb->index ()]);
      bitmap_set_bit (def_loc[bb->index ()],
		      get_expr_index (m_mcfg_def_exprs, footer_info));
    }

  /* Set the def_out of the ENTRY basic block to m_unknow_info expr.  */
  basic_block entry = ENTRY_BLOCK_PTR_FOR_FN (cfun);
  bitmap_set_bit (m_mcfg_def_out[entry->index],
		  get_expr_index (m_mcfg_def_exprs, m_unknow_info));

  compute_reaching_defintion (def_loc, m_kill, m_mcfg_def_in,
			      m_mcfg_def_out);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file,
	       "\n  Compute mcfg info reaching defition data:\n\n");
      fprintf (dump_file, "    Expression List (%d):\n",
	       m_mcfg_def_exprs.length ());
      for (unsigned i = 0; i < m_mcfg_def_exprs.length (); i++)
	{
	  const auto &info = *m_mcfg_def_exprs[i];
	  fprintf (dump_file, "      Expr[%u]: ", i);
	  info.dump (dump_file, "        ");
	}
      fprintf (dump_file, "\n    bitmap data:\n");
      for (const bb_info *bb : crtl->ssa->bbs ())
	{
	  unsigned int i = bb->index ();
	  fprintf (dump_file, "      BB %u:\n", i);
	  fprintf (dump_file, "        def_loc: ");
	  dump_bitmap_file (dump_file, def_loc[i]);
	  fprintf (dump_file, "        kill: ");
	  dump_bitmap_file (dump_file, m_kill[i]);
	  fprintf (dump_file, "        mcfg_def_in: ");
	  dump_bitmap_file (dump_file, m_mcfg_def_in[i]);
	  fprintf (dump_file, "        mcfg_def_out: ");
	  dump_bitmap_file (dump_file, m_mcfg_def_out[i]);
	}
    }

  sbitmap_vector_free (def_loc);
  sbitmap_vector_free (m_kill);
}

/* Subroutine of compute_lcm_local_properties which Compute local transparent
   BB. Note that the compile time is very sensitive to compute_transparent and
   compute_lcm_local_properties, any change of these 2 functions should be
   aware of the compile time changing of the program which has a large number of
   blocks, e.g SPEC 2017 wrf.

   Current compile time profile of SPEC 2017 wrf:

     1. scheduling - 27%
     2. machine dep reorg (MCFG PASS) - 18%

   MCFG pass should not spend more time than scheduling in compilation.  */
void
pre_mcfg::compute_transparent (const bb_info *bb)
{
  int num_exprs = m_exprs.length ();
  unsigned bb_index = bb->index ();
  for (int i = 0; i < num_exprs; i++)
    {
      auto *info = m_exprs[i];
      if (info->has_nonmcfg_reg_amcfg ()
	  && bitmap_bit_p (m_reg_def_loc[bb_index], REGNO (info->get_amcfg ())))
	bitmap_clear_bit (m_transp[bb_index], i);
    }
}

/* Compute the local properties of each recorded expression.

   Local properties are those that are defined by the block, irrespective of
   other blocks.

   An expression is transparent in a block if its operands are not modified
   in the block.

   An expression is computed (locally available) in a block if it is computed
   at least once and expression would contain the same value if the
   computation was moved to the end of the block.

   An expression is locally anticipatable in a block if it is computed at
   least once and expression would contain the same value if the computation
   was moved to the beginning of the block.  */
void
pre_mcfg::compute_lcm_local_properties ()
{
  m_exprs.truncate (0);
  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      mcfg_block_info &block_info = get_block_info (bb);
      if (block_info.empty_p ())
	continue;
      mcfg_info &header_info = block_info.get_entry_info ();
      mcfg_info &footer_info = block_info.get_exit_info ();
      gcc_assert (footer_info.valid_p () || footer_info.unknown_p ());
      if (header_info.valid_p ())
	add_expr (m_exprs, header_info);
      if (footer_info.valid_p ())
	add_expr (m_exprs, footer_info);
    }

  int num_exprs = m_exprs.length ();
  if (m_avloc)
    sbitmap_vector_free (m_avloc);
  if (m_kill)
    sbitmap_vector_free (m_kill);
  if (m_antloc)
    sbitmap_vector_free (m_antloc);
  if (m_transp)
    sbitmap_vector_free (m_transp);
  if (m_avin)
    sbitmap_vector_free (m_avin);
  if (m_avout)
    sbitmap_vector_free (m_avout);

  m_avloc = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);
  m_kill = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);
  m_antloc = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);
  m_transp = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);
  m_avin = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);
  m_avout = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);

  bitmap_vector_clear (m_avloc, last_basic_block_for_fn (cfun));
  bitmap_vector_clear (m_antloc, last_basic_block_for_fn (cfun));
  bitmap_vector_ones (m_transp, last_basic_block_for_fn (cfun));

  /* -  If T is locally available at the end of a block, then T' must be
	available at the end of the same block. Since some optimization has
	occurred earlier, T' might not be locally available, however, it must
	have been previously computed on all paths. As a formula, T at AVLOC(B)
	implies that T' at AVOUT(B).
	An "available occurrence" is one that is the last occurrence in the
	basic block and the operands are not modified by following statements in
	the basic block [including this insn].

     -  If T is locally anticipated at the beginning of a block, then either
	T', is locally anticipated or it is already available from previous
	blocks. As a formula, this means that T at ANTLOC(B) implies that T' at
	ANTLOC(B) at AVIN(B).
	An "anticipatable occurrence" is one that is the first occurrence in the
	basic block, the operands are not modified in the basic block prior
	to the occurrence and the output is not used between the start of
	the block and the occurrence.  */
  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      unsigned bb_index = bb->index ();
      mcfg_block_info &block_info = get_block_info (bb);

      /* Compute m_transp */
      if (block_info.empty_p ())
	compute_transparent (bb);
      else
	{
	  bitmap_clear (m_transp[bb_index]);
	  mcfg_info &header_info = block_info.get_entry_info ();
	  mcfg_info &footer_info = block_info.get_exit_info ();

	  if (header_info.valid_p () && anticipated_exp_p (header_info))
	    bitmap_set_bit (m_antloc[bb_index],
			    get_expr_index (m_exprs, header_info));

	  if (footer_info.valid_p ())
	    for (int i = 0; i < num_exprs; i += 1)
	      {
		const mcfg_info &info = *m_exprs[i];
		if (!info.valid_p ())
		  continue;
		if (available_exp_p (footer_info, info))
		  bitmap_set_bit (m_avloc[bb_index], i);
	      }
	}

      if (invalid_opt_bb_p (bb->cfg_bb ()))
	{
	  bitmap_clear (m_antloc[bb_index]);
	  bitmap_clear (m_transp[bb_index]);
	}

      /* Compute ae_kill for each basic block using:

	 ~(TRANSP | COMP)
      */
      bitmap_ior (m_kill[bb_index], m_transp[bb_index], m_avloc[bb_index]);
      bitmap_not (m_kill[bb_index], m_kill[bb_index]);
    }
}

void
pre_mcfg::fuse_local_mcfg_info ()
{
  m_reg_def_loc
    = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), GP_REG_LAST + 1);
  bitmap_vector_clear (m_reg_def_loc, last_basic_block_for_fn (cfun));
  bitmap_ones (m_reg_def_loc[ENTRY_BLOCK_PTR_FOR_FN (cfun)->index]);

  for (bb_info *bb : crtl->ssa->bbs ())
    {
      auto &block_info = get_block_info (bb);
      block_info.bb = bb;
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "  Try fuse basic block %d\n", bb->index ());
	}
      auto_vec<mcfg_info> infos;
      for (insn_info *insn : bb->real_nondebug_insns ())
	{
	  mcfg_info curr_info = mcfg_info (insn);
	  if (curr_info.valid_p () || curr_info.unknown_p ())
	    infos.safe_push (curr_info);

	  /* Collecting GP registers modified by the current bb.  */
	  if (insn->is_real ())
	    for (def_info *def : insn->defs ())
	      if (def->is_reg () && GP_REG_P (def->regno ()))
		bitmap_set_bit (m_reg_def_loc[bb->index ()], def->regno ());
	}

      mcfg_info prev_info = mcfg_info ();
      prev_info.set_empty ();
      for (auto &curr_info : infos)
	{
	  if (prev_info.empty_p ())
	    prev_info = curr_info;
	  else if ((curr_info.unknown_p () && prev_info.valid_p ())
		   || (curr_info.valid_p () && prev_info.unknown_p ()))
	    {
	      block_info.local_infos.safe_push (prev_info);
	      prev_info = curr_info;
	    }
	  else if (curr_info.valid_p () && prev_info.valid_p ())
	    {
	      if (m_dem.available_p (prev_info, curr_info))
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file,
			       "    Ignore curr info since prev info "
			       "available with it:\n");
		      fprintf (dump_file, "      prev_info: ");
		      prev_info.dump (dump_file, "        ");
		      fprintf (dump_file, "      curr_info: ");
		      curr_info.dump (dump_file, "        ");
		      fprintf (dump_file, "\n");
		    }

		  if (mcfg_insn_p (curr_info.get_insn ()->rtl ()))
		    m_delete_list.safe_push (curr_info);

		}
	      else
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file,
			       "    Cannot fuse uncompatible infos:\n");
		      fprintf (dump_file, "      prev_info: ");
		      prev_info.dump (dump_file, "       ");
		      fprintf (dump_file, "      curr_info: ");
		      curr_info.dump (dump_file, "       ");
		    }
		  block_info.local_infos.safe_push (prev_info);
		  prev_info = curr_info;
		}
	    }
	}

      if (prev_info.valid_p () || prev_info.unknown_p ())
	block_info.local_infos.safe_push (prev_info);
    }
}


bool
pre_mcfg::earliest_fuse_mcfg_info (int iter)
{
  compute_mcfg_def_data ();
  compute_lcm_local_properties ();

  unsigned num_exprs = m_exprs.length ();
  struct edge_list *m_edges = create_edge_list ();
  unsigned num_edges = NUM_EDGES (m_edges);
  sbitmap *antin
    = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);
  sbitmap *antout
    = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), num_exprs);

  sbitmap *earliest = sbitmap_vector_alloc (num_edges, num_exprs);

  compute_available (m_avloc, m_kill, m_avout, m_avin);
  compute_antinout_edge (m_antloc, m_transp, antin, antout);
  compute_earliest (m_edges, num_exprs, antin, antout, m_avout, m_kill,
		    earliest);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\n  Compute LCM earliest insert data (lift %d):\n\n",
	       iter);
      fprintf (dump_file, "    Expression List (%u):\n", num_exprs);
      for (unsigned i = 0; i < num_exprs; i++)
	{
	  const auto &info = *m_exprs[i];
	  fprintf (dump_file, "      Expr[%u]: ", i);
	  info.dump (dump_file, "        ");
	}
      fprintf (dump_file, "\n    bitmap data:\n");
      for (const bb_info *bb : crtl->ssa->bbs ())
	{
	  unsigned int i = bb->index ();
	  fprintf (dump_file, "      BB %u:\n", i);
	  fprintf (dump_file, "        avloc: ");
	  dump_bitmap_file (dump_file, m_avloc[i]);
	  fprintf (dump_file, "        kill: ");
	  dump_bitmap_file (dump_file, m_kill[i]);
	  fprintf (dump_file, "        antloc: ");
	  dump_bitmap_file (dump_file, m_antloc[i]);
	  fprintf (dump_file, "        transp: ");
	  dump_bitmap_file (dump_file, m_transp[i]);

	  fprintf (dump_file, "        avin: ");
	  dump_bitmap_file (dump_file, m_avin[i]);
	  fprintf (dump_file, "        avout: ");
	  dump_bitmap_file (dump_file, m_avout[i]);
	  fprintf (dump_file, "        antin: ");
	  dump_bitmap_file (dump_file, antin[i]);
	  fprintf (dump_file, "        antout: ");
	  dump_bitmap_file (dump_file, antout[i]);
	}
      fprintf (dump_file, "\n");
      fprintf (dump_file, "      earliest:\n");
      for (unsigned ed = 0; ed < num_edges; ed++)
	{
	  edge eg = INDEX_EDGE (m_edges, ed);

	  if (bitmap_empty_p (earliest[ed]))
	    continue;
	  fprintf (dump_file, "        Edge(bb %u -> bb %u): ", eg->src->index,
		   eg->dest->index);
	  dump_bitmap_file (dump_file, earliest[ed]);
	}
      fprintf (dump_file, "\n");
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "    Fused global info result (lift %d):\n", iter);
    }

  bool changed = false;
  for (unsigned ed = 0; ed < num_edges; ed++)
    {
      sbitmap e = earliest[ed];
      if (bitmap_empty_p (e))
	continue;

      unsigned int expr_index;
      sbitmap_iterator sbi;
      EXECUTE_IF_SET_IN_BITMAP (e, 0, expr_index, sbi)
	{
	  mcfg_info &curr_info = *m_exprs[expr_index];
	  edge eg = INDEX_EDGE (m_edges, ed);
	  mcfg_block_info &src_block_info = get_block_info (eg->src);
	  mcfg_block_info &dest_block_info = get_block_info (eg->dest);

	  if (!curr_info.valid_p ()
	      || eg->probability == profile_probability::never ()
	      || src_block_info.probability
		   == profile_probability::uninitialized ()
	      /* When multiple set bits in earliest edge, such edge may
		 have infinite loop in preds or succs or multiple conflict
		 mcfg expression which make such edge is unrelated.  We
		 don't perform fusion for such situation.  */
	      || bitmap_count_bits (e) != 1)
	    continue;

	  if (src_block_info.empty_p ())
	    {
	      mcfg_info new_curr_info = curr_info;
	      new_curr_info.set_bb (crtl->ssa->bb (eg->dest));
	      bool has_compatible_p
		= has_compatible_reaching_mcfg_p (new_curr_info);
	      if (!has_compatible_p)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file,
			       "      Forbidden lift up mcfg info into bb %u "
			       "since there is no mcfg info that reaching in "
			       "is compatible with it:",
			       eg->src->index);
		      curr_info.dump (dump_file, "        ");
		    }
		  continue;
		}

	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file,
			   "      Set empty bb %u to info:", eg->src->index);
		  curr_info.dump (dump_file, "        ");
		}
	      src_block_info.set_info (curr_info);
	      src_block_info.probability = dest_block_info.probability;
	      changed = true;
	    }
	  else if (src_block_info.has_info ())
	    {
	      mcfg_info &prev_info = src_block_info.get_exit_info ();
	      gcc_assert (prev_info.valid_p ());

	      if (m_dem.compatible_p (prev_info, curr_info))
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file, "    Fuse curr info since prev info "
					  "compatible with it:\n");
		      fprintf (dump_file, "      prev_info: ");
		      prev_info.dump (dump_file, "        ");
		      fprintf (dump_file, "      curr_info: ");
		      curr_info.dump (dump_file, "        ");
		    }
		  m_dem.merge (prev_info, curr_info);
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file, "      prev_info after fused: ");
		      prev_info.dump (dump_file, "        ");
		      fprintf (dump_file, "\n");
		    }
		  changed = true;
		  if (src_block_info.has_info ())
		    src_block_info.probability += dest_block_info.probability;
		}
	      else
		{
		  /* Cancel lift up if probabilities are equal.  */
		  if (successors_probability_equal_p (eg->src)
		      || (dest_block_info.probability
			    > src_block_info.probability
			  && !has_compatible_reaching_mcfg_p (curr_info)))
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  fprintf (dump_file,
				   "      Reset bb %u:",
				   eg->src->index);
			  prev_info.dump (dump_file, "        ");
			  fprintf (dump_file, "	due to (same probability or no "
					      "compatible reaching):");
			  curr_info.dump (dump_file, "        ");
			}
		      src_block_info.set_empty_info ();
		      src_block_info.probability
			= profile_probability::uninitialized ();
		      /* See PR113696, we should reset immediate dominator to
			 empty since we may uplift ineffective mcfg which
			 locate at low probability block.  */
		      basic_block dom
			= get_immediate_dominator (CDI_DOMINATORS, eg->src);
		      auto &dom_block_info = get_block_info (dom);
		      if (dom_block_info.has_info ()
			  && !m_dem.compatible_p (
			    dom_block_info.get_exit_info (), curr_info))
			{
			  dom_block_info.set_empty_info ();
			  dom_block_info.probability
			    = profile_probability::uninitialized ();
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file,
				       "      Reset dominator bb %u:",
				       dom->index);
			      prev_info.dump (dump_file, "        ");
			      fprintf (dump_file,
				       "	due to (same probability or no "
				       "compatible reaching):");
			      curr_info.dump (dump_file, "        ");
			    }
			}
		      changed = true;
		    }
		  /* Choose the one with higher probability. */
		  else if (dest_block_info.probability
			   > src_block_info.probability)
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  fprintf (dump_file,
				   "      Change bb %u from:",
				   eg->src->index);
			  prev_info.dump (dump_file, "        ");
			  fprintf (dump_file,
				   "        to (higher probability):");
			  curr_info.dump (dump_file, "        ");
			}
		      src_block_info.set_info (curr_info);
		      src_block_info.probability = dest_block_info.probability;
		      changed = true;
		    }
		}
	    }
	  else
	    {
	      mcfg_info &prev_info = src_block_info.get_exit_info ();
	      if (!prev_info.valid_p ()
		  || m_dem.available_p (prev_info, curr_info)
		  || !m_dem.compatible_p (prev_info, curr_info))
		continue;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "    Fuse curr info since prev info "
				      "compatible with it:\n");
		  fprintf (dump_file, "      prev_info: ");
		  prev_info.dump (dump_file, "        ");
		  fprintf (dump_file, "      curr_info: ");
		  curr_info.dump (dump_file, "        ");
		}
	      m_dem.merge (prev_info, curr_info);
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "      prev_info after fused: ");
		  prev_info.dump (dump_file, "        ");
		  fprintf (dump_file, "\n");
		}
	      changed = true;
	    }
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\n");
    }

  sbitmap_vector_free (antin);
  sbitmap_vector_free (antout);
  sbitmap_vector_free (earliest);
  free_edge_list (m_edges);

  return changed;
}

void
pre_mcfg::pre_global_mcfg_info ()
{
  compute_mcfg_def_data ();
  compute_lcm_local_properties ();

  unsigned num_exprs = m_exprs.length ();
  m_edges = pre_edge_lcm_avs (num_exprs, m_transp, m_avloc, m_antloc, m_kill,
			      m_avin, m_avout, &m_insert, &m_del);
  unsigned num_edges = NUM_EDGES (m_edges);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\n  Compute LCM insert and delete data:\n\n");
      fprintf (dump_file, "    Expression List (%u):\n", num_exprs);
      for (unsigned i = 0; i < num_exprs; i++)
	{
	  const auto &info = *m_exprs[i];
	  fprintf (dump_file, "      Expr[%u]: ", i);
	  info.dump (dump_file, "        ");
	}
      fprintf (dump_file, "\n    bitmap data:\n");
      for (const bb_info *bb : crtl->ssa->bbs ())
	{
	  unsigned i = bb->index ();
	  fprintf (dump_file, "      BB %u:\n", i);
	  fprintf (dump_file, "        avloc: ");
	  dump_bitmap_file (dump_file, m_avloc[i]);
	  fprintf (dump_file, "        kill: ");
	  dump_bitmap_file (dump_file, m_kill[i]);
	  fprintf (dump_file, "        antloc: ");
	  dump_bitmap_file (dump_file, m_antloc[i]);
	  fprintf (dump_file, "        transp: ");
	  dump_bitmap_file (dump_file, m_transp[i]);

	  fprintf (dump_file, "        avin: ");
	  dump_bitmap_file (dump_file, m_avin[i]);
	  fprintf (dump_file, "        avout: ");
	  dump_bitmap_file (dump_file, m_avout[i]);
	  fprintf (dump_file, "        del: ");
	  dump_bitmap_file (dump_file, m_del[i]);
	}
      fprintf (dump_file, "\n");
      fprintf (dump_file, "      insert:\n");
      for (unsigned ed = 0; ed < num_edges; ed++)
	{
	  edge eg = INDEX_EDGE (m_edges, ed);

	  if (bitmap_empty_p (m_insert[ed]))
	    continue;
	  fprintf (dump_file, "        Edge(bb %u -> bb %u): ", eg->src->index,
		   eg->dest->index);
	  dump_bitmap_file (dump_file, m_insert[ed]);
	}
    }

  /* Remove mcfg infos as LCM suggest */
  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      sbitmap d = m_del[bb->index ()];
      if (bitmap_count_bits (d) == 0)
	continue;
      gcc_assert (bitmap_count_bits (d) == 1);
      unsigned expr_index = bitmap_first_set_bit (d);
      mcfg_info &info = *m_exprs[expr_index];
      gcc_assert (info.valid_p ());
      gcc_assert (info.get_bb () == bb);
      const mcfg_block_info &block_info = get_block_info (info.get_bb ());
      gcc_assert (block_info.get_entry_info () == info);
      info.set_delete ();
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file,
		   "\nLCM deleting mcfg of block %d, it has predecessors: \n",
		   bb->index ());
	  hash_set<basic_block> all_preds
	    = get_all_predecessors (bb->cfg_bb ());
	  int i = 0;
	  for (const auto pred : all_preds)
	    {
	      fprintf (dump_file, "%d ", pred->index);
	      i++;
	      if (i % 32 == 0)
		fprintf (dump_file, "\n");
	    }
	  fprintf (dump_file, "\n");
	}
    }

  /* Remove mcfg infos if all precessors are available to the block.  */
  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      mcfg_block_info &block_info = get_block_info (bb);
      if (block_info.empty_p ())
	continue;
      mcfg_info &curr_info = block_info.get_entry_info ();
      if (!curr_info.valid_p ())
	continue;

      unsigned int expr_index;
      sbitmap_iterator sbi;
      gcc_assert (
	!bitmap_empty_p (m_mcfg_def_in[curr_info.get_bb ()->index ()]));
      bool full_available = true;
      EXECUTE_IF_SET_IN_BITMAP (m_mcfg_def_in[bb->index ()], 0, expr_index,
				sbi)
	{
	  mcfg_info &prev_info = *m_mcfg_def_exprs[expr_index];
	  if (!prev_info.valid_p ()
	      || !m_dem.available_p (prev_info, curr_info))
	    {
	      full_available = false;
	      break;
	    }
	}
      if (full_available)
	curr_info.set_delete ();
    }

  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      mcfg_block_info &block_info = get_block_info (bb);
      if (block_info.empty_p ())
	continue;
      mcfg_info &curr_info = block_info.get_entry_info ();
      if (curr_info.delete_p ())
	{
	  if (block_info.local_infos.is_empty ())
	    continue;
	  curr_info = block_info.local_infos[0];
	}

      mcfg_info prev_info = mcfg_info ();
      prev_info.set_empty ();
      for (auto &curr_info : block_info.local_infos)
	{
	  prev_info = curr_info;
	}
    }
}

void
pre_mcfg::emit_mcfg ()
{
  bool need_commit = false;

  /* Fake edge is created by connect infinite loops to exit function.
     We should commit mcfg edge after fake edges removes, otherwise,
     it will cause ICE.  */
  remove_fake_exit_edges ();
  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      for (const auto &curr_info : get_block_info (bb).local_infos)
	{
	  insn_info *insn = curr_info.get_insn ();
	  if (curr_info.delete_p ())
	    {
	      if (mcfg_insn_p (insn->rtl ()))
		remove_mcfg_insn (curr_info.get_insn ()->rtl ());
	      continue;
	    }
	  else if (curr_info.valid_p ())
	    {
	      if (mcfg_insn_p (insn->rtl ()))
		{
		  const mcfg_info temp = mcfg_info (insn);
		  if (!(curr_info == temp))
		    {
		      if (dump_file)
			{
			  fprintf (dump_file, "\n  Change mcfg info from: ");
			  temp.dump (dump_file, "    ");
			  fprintf (dump_file, "  to: ");
			  curr_info.dump (dump_file, "    ");
			}
		      change_mcfg_insn (curr_info);
		    }
		}
	      else
		{
		  if (dump_file)
		    {
		      fprintf (dump_file,
			       "\n  Insert mcfg info before insn %d: ",
			       insn->uid ());
		      curr_info.dump (dump_file, "    ");
		    }
		  insert_mcfg_insn (EMIT_BEFORE, curr_info);
		}
	    }
	}
    }

  for (const mcfg_info &item : m_delete_list)
    {
      gcc_assert (mcfg_insn_p (item.get_insn ()->rtl ()));
      remove_mcfg_insn (item.get_insn ()->rtl ());
    }

  /* Insert mcfg info that was not deleted after lift up.  */
  for (const bb_info *bb : crtl->ssa->bbs ())
    {
      const mcfg_block_info &block_info = get_block_info (bb);
      if (!block_info.has_info ())
	continue;

      const mcfg_info &footer_info = block_info.get_exit_info ();

      if (footer_info.delete_p ())
	continue;

      edge eg;
      edge_iterator eg_iterator;
      FOR_EACH_EDGE (eg, eg_iterator, bb->cfg_bb ()->succs)
	{
	  gcc_assert (!(eg->flags & EDGE_ABNORMAL));
	  if (dump_file)
	    {
	      fprintf (
		dump_file,
		"\n  Insert missed mcfg info at edge(bb %u -> bb %u): ",
		eg->src->index, eg->dest->index);
	      footer_info.dump (dump_file, "    ");
	    }
	  start_sequence ();
	  insert_mcfg_insn (EMIT_DIRECT, footer_info);
	  rtx_insn *rinsn = get_insns ();
	  end_sequence ();
	  default_rtl_profile ();
	  insert_insn_on_edge (rinsn, eg);
	  need_commit = true;
	}
    }

  /* m_insert mcfg as LCM suggest. */
  for (int ed = 0; ed < NUM_EDGES (m_edges); ed++)
    {
      edge eg = INDEX_EDGE (m_edges, ed);
      sbitmap i = m_insert[ed];
      if (bitmap_count_bits (i) != 1)
	/* For code with infinite loop (e.g. pr61634.c), The data flow is
	   completely wrong.  */
	continue;

      unsigned expr_index = bitmap_first_set_bit (i);
      const mcfg_info &info = *m_exprs[expr_index];
      gcc_assert (info.valid_p ());
      if (dump_file)
	{
	  fprintf (dump_file,
		   "\n  Insert mcfg info at edge(bb %u -> bb %u): ",
		   eg->src->index, eg->dest->index);
	  info.dump (dump_file, "    ");
	}
      rtl_profile_for_edge (eg);
      start_sequence ();

      insert_mcfg_insn (EMIT_DIRECT, info);
      rtx_insn *rinsn = get_insns ();
      end_sequence ();
      default_rtl_profile ();

      /* We should not get an abnormal edge here.  */
      gcc_assert (!(eg->flags & EDGE_ABNORMAL));
      need_commit = true;
      insert_insn_on_edge (rinsn, eg);
    }

  if (need_commit)
    commit_edge_insertions ();
}

void
pre_mcfg::cleaup ()
{
  remove_amcfg_operand ();
  remove_unused_dest_operand ();
}

void
pre_mcfg::remove_amcfg_operand ()
{
  basic_block cfg_bb;
  rtx_insn *rinsn;
  FOR_ALL_BB_FN (cfg_bb, cfun)
    FOR_BB_INSNS (cfg_bb, rinsn)
      if (NONDEBUG_INSN_P (rinsn) && has_mcfg_op (rinsn)
	  && get_mcfg (rinsn) && REG_P (get_mcfg (rinsn)))
	{
	  rtx amcfg = get_mcfg (rinsn);
	  if (count_regno_occurrences (rinsn, REGNO (amcfg)) == 1)
	    {
	      rtx set = single_set (rinsn);
	      rtx src
		= simplify_replace_rtx (SET_SRC (set), amcfg, const0_rtx);
	      rtx new_pat = gen_rtx_SET (SET_DEST (set), src);
	      if (dump_file)
		{
		  fprintf (dump_file, "  Cleanup insn %u's amcfg operand:\n",
			   INSN_UID (rinsn));
		  print_rtl_single (dump_file, rinsn);
		}
	      validate_change_or_fail (rinsn, &PATTERN (rinsn), new_pat, false);
	    }
	}
}

void
pre_mcfg::remove_unused_dest_operand ()
{
  df_analyze ();
  basic_block cfg_bb;
  rtx_insn *rinsn;
  FOR_ALL_BB_FN (cfg_bb, cfun)
    FOR_BB_INSNS (cfg_bb, rinsn)
      if (NONDEBUG_INSN_P (rinsn) && mcfg_insn_p (rinsn))
	{
	  rtx mcfg = get_mcfg (rinsn);
	  if (!mcfg)
	    continue;
	  mcfg_info info = mcfg_info (rinsn);
	  if (has_no_uses (cfg_bb, rinsn, REGNO (mcfg)))
	    {
	      rtx new_pat = info.get_mcfg_pat ();
	      if (dump_file)
		{
		  fprintf (dump_file,
			   "  Remove mcfg insn %u's dest(mcfg) operand since "
			   "it unused:\n",
			   INSN_UID (rinsn));
		  print_rtl_single (dump_file, rinsn);
		}
	      validate_change_or_fail (rinsn, &PATTERN (rinsn), new_pat, false);
	    }
	}
}

const pass_data pass_data_mcfg = {
  RTL_PASS,	 /* type */
  "mcfg",	 /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_MACH_DEP,	 /* tv_id */
  0,		 /* properties_required */
  0,		 /* properties_provided */
  0,		 /* properties_destroyed */
  0,		 /* todo_flags_start */
  0,		 /* todo_flags_finish */
};

class pass_mcfg : public rtl_opt_pass
{
private:
  void simple_mcfg ();
  void lazy_mcfg ();

public:
  pass_mcfg (gcc::context *ctxt) : rtl_opt_pass (pass_data_mcfg, ctxt) {}

  /* opt_pass methods: */
  virtual bool gate (function *) final override { return TARGET_MATRIX; }
  virtual unsigned int execute (function *) final override;
}; // class pass_mcfg

void
pass_mcfg::simple_mcfg ()
{
  if (dump_file)
    fprintf (dump_file, "\nEntering Simple MCFG PASS\n");

  basic_block cfg_bb;
  rtx_insn *rinsn;
  FOR_ALL_BB_FN (cfg_bb, cfun)
    {
      FOR_BB_INSNS (cfg_bb, rinsn)
	{
	  if (!NONDEBUG_INSN_P (rinsn))
	    continue;
	  if (has_mcfg_op (rinsn))
	    {
	      const auto &info = mcfg_info (rinsn);
	      rtx pat = info.get_mcfg_pat ();
	      emit_insn_before (pat, rinsn);
	      if (dump_file)
		{
		  fprintf (dump_file, "  Insert mcfg insn before insn %d:\n",
			   INSN_UID (rinsn));
		  print_rtl_single (dump_file, PREV_INSN (rinsn));
		}
	    }
	}
    }
}

/* Lazy mcfg insertion for optimize > 0. */
void
pass_mcfg::lazy_mcfg ()
{
  if (dump_file)
    fprintf (dump_file, "\nEntering Lazy MCFG PASS\n\n");

  pre_mcfg pre = pre_mcfg ();

  if (dump_file)
    fprintf (dump_file, "\nPhase 1: Fuse local mcfg infos.\n\n");
  pre.fuse_local_mcfg_info ();
  if (dump_file && (dump_flags & TDF_DETAILS))
    pre.dump (dump_file, "phase 1");

  /* Phase 2:  Fuse header and footer mcfg infos between basic blocks.  */
  if (dump_file)
    fprintf (dump_file, "\nPhase 2: Lift up mcfg info.\n\n");

  bool changed = true;
  int fused_count = 0;
  do
    {
      if (dump_file)
	fprintf (dump_file, "  Try lift up %d.\n\n", fused_count);
      changed = pre.earliest_fuse_mcfg_info (fused_count);
      fused_count += 1;
  } while (changed);

  if (dump_file && (dump_flags & TDF_DETAILS))
    pre.dump (dump_file, "phase 2");

  /* Phase 3: Reducing redundant mcfg infos using LCM.  */
  if (dump_file)
    fprintf (dump_file, "\nPhase 3: Reduce global mcfg infos.\n\n");
  pre.pre_global_mcfg_info ();
  if (dump_file && (dump_flags & TDF_DETAILS))
    pre.dump (dump_file, "phase 3");

  /* Phase 4: Insert, modify and remove mcfg insns.  */
  if (dump_file)
    fprintf (dump_file,
	     "\nPhase 4: Insert, modify and remove mcfg insns.\n\n");
  pre.emit_mcfg ();

  /* Phase 5: Cleaup */
  if (dump_file)
    fprintf (dump_file, "\nPhase 5: Cleaup\n\n");
  pre.cleaup ();

  pre.finish ();
}

/* Main entry point for this pass.  */
unsigned int
pass_mcfg::execute (function *)
{
  if (n_basic_blocks_for_fn (cfun) <= 0)
    return 0;

  /* Early return for there is no matrix instructions.  */
  if (!has_matrix_insn (cfun))
    return 0;

  set_current_mcfg_status (TYPE_MCFGM);
  if (optimize && riscv_lcm_mcfg)
    lazy_mcfg ();
  else
    simple_mcfg ();

  set_current_mcfg_status (TYPE_MCFGN);
  if (optimize && riscv_lcm_mcfg)
    lazy_mcfg ();
  else
    simple_mcfg ();

  set_current_mcfg_status (TYPE_MCFGK);
  if (optimize && riscv_lcm_mcfg)
    lazy_mcfg ();
  else
    simple_mcfg ();

  return 0;
}

rtl_opt_pass *
make_pass_mcfg (gcc::context *ctxt)
{
  return new pass_mcfg (ctxt);
}
