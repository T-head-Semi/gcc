
/* MCFG pass for Thead Matrix Extension for GNU compiler.

   Copyright (C) 2022-2024 Free Software Foundation, Inc.
   Contributed by Jin Ma <jinma@linux.alibaba.com>, T-head-Semi.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* Return true if mode is the RVM enabled mode.  */
bool
th_m_ext_mode_p (machine_mode mode)
{
#undef ENTRY
#define ENTRY(MODE, REQUIREMENT, ...)					\
  case MODE##mode:							\
    return REQUIREMENT;
  switch (mode)
    {
#include "thead-matrix-switch.def"
    default:
      return false;
    }

  return false;
}

/* Call from ADJUST_NUNITS in riscv-modes.def. Return the correct
   NUNITS size for corresponding machine_mode.  */
poly_int64
th_m_adjust_nunits (machine_mode mode, int lmul, int nf)
{
  if (th_m_ext_mode_p (mode))
    {
      return riscv_matrix_chunks * lmul * nf;
    }

  /* Set the disabled RVM modes size as 1 by default.  */
  return 1;
}

enum mlmul_type
th_m_get_mlmul (machine_mode mode)
{
#undef ENTRY
  if (th_m_ext_mode_p (mode))
#define ENTRY(MODE, REQUIREMENT, LMUL)					\
  case MODE##mode:							\
    return LMUL;
  switch (mode)
    {
#include "thead-matrix-switch.def"
    default:
      gcc_unreachable ();
    }
  return LMUL_NONE;
}

/* Return true if RINSN is an matrix instruction depends on msize.  */
static bool
th_m_has_mcfg_op (rtx_insn *rinsn)
{
  return recog_memoized (rinsn) >= 0 && get_attr_th_m_has_mcfg_op (rinsn);
}

/* Return true if the function has any matrix instruction that depends on msize.  */
static bool
th_m_has_matrix_insn (function *fn)
{
  basic_block cfg_bb;
  rtx_insn *rinsn;
  FOR_ALL_BB_FN (cfg_bb, fn)
    FOR_BB_INSNS (cfg_bb, rinsn)
      if (NONDEBUG_INSN_P (rinsn) && th_m_has_mcfg_op (rinsn))
	return true;
  return false;
}

class th_mcfg_info
{
public:
  rtx m_mcfg[3]; /* mcfgm, mcfgk, mcfgn.  */
  unsigned m_ebytes;
  rtx_insn *m_insn;
  basic_block m_bb;

  th_mcfg_info ()
    : m_mcfg {NULL_RTX, NULL_RTX, NULL_RTX},
      m_ebytes (INVALID_ATTRIBUTE), m_insn (nullptr), m_bb (nullptr)
  {}

  th_mcfg_info (rtx_insn *insn) : th_mcfg_info () { parse_insn (insn); }

  /* Parse the given rtx_insn and populate th_mcfg_info attributes. */
  void parse_insn (rtx_insn *insn)
  {
    int index;
    m_insn = insn;

    if (!NONDEBUG_INSN_P (insn) || !th_m_has_mcfg_op (insn))
      return;

    extract_insn_cached (insn);
    m_bb = BLOCK_FOR_INSN (insn);
    m_ebytes = get_attr_matrix_ebytes (insn);

    index = get_attr_matrix_mcfgm_op_idx (insn);
    m_mcfg[0] = recog_data.operand[index];

    index = get_attr_matrix_mcfgk_op_idx (insn);
    m_mcfg[2]= recog_data.operand[index];

    index = get_attr_matrix_mcfgn_op_idx (insn);
    if (index != INVALID_ATTRIBUTE)
      m_mcfg[1] = recog_data.operand[index];
  }

  th_mcfg_info get_last_mcfg_info (const vec<th_mcfg_info> info_vec, int index)
  {
    th_mcfg_info tmp = th_mcfg_info();
    for (auto &info : info_vec)
      {
	if (rtx_equal_p (m_insn, info.m_insn))
	  break;
	
	if (info.m_mcfg[index])
	  tmp = info;
      }
    return tmp;
  }

  void eliminate_mcfg (const vec<th_mcfg_info> info_vec)
  {
    for (size_t i = 0; i < 3; i++)
      {
	th_mcfg_info tmp = get_last_mcfg_info (info_vec, i);
	if (m_mcfg[i] && rtx_equal_p (tmp.m_mcfg[i], m_mcfg[i])
	    && !modified_between_p (gen_rtx_REG (QImode, MSIZE_M_REGNUM + i), tmp.m_insn, m_insn))
	  m_mcfg[i] = NULL_RTX;
      }
  }

  void emit_mcfg_before () const
  {
    rtx tmp;

    for (size_t i = 0; i < 3; ++i)
      {
	if (!m_mcfg[i])
	  continue;

	switch (i)
	  {
	  case 0:
	    tmp = gen_matrix_mcfgm (RVM_X0_REG, m_mcfg[i]);
	    break;
	  case 2:
	    tmp = gen_matrix_mcfgk (RVM_X0_REG, m_mcfg[i]);
	    break;
	  case 1:
	    tmp = gen_matrix_mcfgn (RVM_X0_REG, m_mcfg[i]);
	    break;  
	  default:
	    gcc_unreachable();
	  }

	extract_insn (emit_insn_before (tmp, m_insn));
      }
  }
};

const pass_data pass_data_mcfg =
{
  RTL_PASS,		/* type */
  "mcfg",		/* name */
  OPTGROUP_NONE,	/* optinfo_flags */
  TV_NONE,		/* tv_id */
  0,			/* properties_required */
  0,			/* properties_provided */
  0,			/* properties_destroyed */
  0,			/* todo_flags_start */
  TODO_df_finish,	/* todo_flags_finish */
};

class pass_mcfg : public rtl_opt_pass
{
private:
  void th_insert_mcfg ();
  void th_record_mcfg_info ();
  void th_eliminate_mcfg_info_in_bb ();
  void th_eliminate_mcfg_info_in_func ();

public:
  pass_mcfg (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_mcfg, ctxt)
  {
    bb_infos.safe_grow_cleared (0);
  }

  /* opt_pass methods: */
  bool gate (function *) { return TARGET_MATRIX; }
  virtual unsigned int execute (function *fun);

  auto_vec<vec<th_mcfg_info>> bb_infos;
};

rtl_opt_pass *
make_pass_mcfg (gcc::context *ctxt)
{
  return new pass_mcfg (ctxt);
}

/* Traverses the entire function and records all insns
   that need to be inserted by mcfg.  */
void
pass_mcfg::th_record_mcfg_info ()
{
  basic_block cfg_bb;
  rtx_insn *rinsn;
  FOR_ALL_BB_FN (cfg_bb, cfun)
    {
      auto_vec<th_mcfg_info> infos (0);
      FOR_BB_INSNS (cfg_bb, rinsn)
	{
	  if (!NONDEBUG_INSN_P (rinsn))
	    continue;
	  if (th_m_has_mcfg_op (rinsn))
	    {
	      th_mcfg_info info = th_mcfg_info (rinsn);
	      infos.safe_push (info);
	    }
	}
      if (!infos.is_empty ())
	bb_infos.safe_push (infos.copy ());
    }
}

/* Insert the mcfg instructions into the function. */
void
pass_mcfg::th_insert_mcfg ()
{
  for (auto &bb_info : bb_infos)
    {
      for (auto &info : bb_info)
	info.emit_mcfg_before ();
      bb_info.release ();
    }
}

/* Analyze redundant mcfg insns in function and try to eliminate them.  */
void
pass_mcfg::th_eliminate_mcfg_info_in_func ()
{
  for (size_t i = 0; i < 3; ++i)
    {
      rtx tmp = NULL_RTX;
      bool first_p = true;
      for (auto &bb_info : bb_infos)
	for (auto &info : bb_info)
	  {
	    if (first_p)
	      {
		first_p = false;
		tmp = info.m_mcfg[i];
		continue;
	      }

	    if (!rtx_equal_p (tmp, info.m_mcfg[i]))
              {
                tmp = NULL_RTX;
                break;
              }
	  }

      if (!tmp || modified_between_p (gen_rtx_REG (QImode, MSIZE_M_REGNUM + i), get_insns (), get_last_insn()))
        continue;

      /* If all mcfg insns are the same, we can eliminate them.  */
      th_mcfg_info info = th_mcfg_info (get_insns ());
      info.m_mcfg[i] = tmp;
      info.emit_mcfg_before ();

      /* Reset all m_mcfg[i] insns in function.  */
      for (auto &bb_info : bb_infos)
	for (auto &info : bb_info)
	  info.m_mcfg[i] = NULL_RTX;
    }
}

/* Analyze redundant mcfg insns in BB and try to eliminate them.  */
void
pass_mcfg::th_eliminate_mcfg_info_in_bb ()
{
  for (auto &bb_info : bb_infos)
    for (auto &info : bb_info)
      info.eliminate_mcfg (bb_info);
}

unsigned int
pass_mcfg::execute (function *)
{
  if (n_basic_blocks_for_fn (cfun) <= 0)
    return 0;

  /* Early return for there is no matrix instructions.  */
  if (!th_m_has_matrix_insn (cfun))
    return 0;

  th_record_mcfg_info ();
  if (bb_infos.is_empty ())
    return 0;

  if (optimize)
    {
      th_eliminate_mcfg_info_in_func ();
      th_eliminate_mcfg_info_in_bb ();
    }

  th_insert_mcfg ();

  return 0;
}