/* Entry/exit vsetvl insn of a basic block.
     entry: the first vsetvl insn, may be NULL.
     exit: the last vsetvl insn, may be NULL.
   Actually, there are only three cases:
     1. entry&exit are different insn, which means there are more than 2 different
	vsetvl insns that can't be eliminated. Entry&exit can be equal, but there
	may have different vsetvls between them or they have uses.
     2. entry&exit are the same insn, which means there is only 1 vsetvl in this
	basic block.
     3. entry&exit are NULL, which means there is no vsetvl in this basic block.
   It is impossible that one of entry/exit is NULL while another is non-null.  */

typedef struct GTY(()) bb_vsetvl_info_def
{
  rtx_insn *entry;
  rtx_insn *exit;
} bb_vsetvl_info_t;

/* A map to record basic block's vsetvl insn info.  */

typedef hash_map<basic_block, bb_vsetvl_info_t> bb_vsetvl_info_map_t;

static GTY(()) bb_vsetvl_info_map_t *info_map;

/* Return TRUE if PRED_VSETVL_INSN and SUCC_VSETVL_INSN has same effect.
   If DEST or SRC is not NULL, set the dest reg and source reg of SUCC_VSETVL_INSN to them.  */

static bool
vsetvl_rtx_equal_p (rtx_insn *pred_vsetvl_insn, rtx_insn *succ_vsetvl_insn, rtx *src, rtx *dest)
{
  machine_mode mode_succ, mode_pred;
  const_rtx pat_succ = PATTERN (succ_vsetvl_insn);
  const_rtx pat_pred = PATTERN (pred_vsetvl_insn);

  /* If only one of them is vsetvli_x0_*, return FALSE.  */
  if (GET_CODE (pat_succ) == SET || GET_CODE (pat_pred) == SET)
    {
      if (rtx_equal_p (pat_succ, pat_pred))
	{
	  if (dest)
	    *dest = gen_rtx_REG (Pmode, GP_REG_FIRST);
	  if (src)
	    *src = gen_rtx_REG (Pmode, GP_REG_FIRST);
	  return true;
	}
      else
	return false;
    }

  rtx src_succ, src_pred;
  rtx dest_succ = SET_DEST (XVECEXP (pat_succ, 0, 0));
  rtx dest_pred = SET_DEST (XVECEXP (pat_pred, 0, 0));

  gcc_assert (REG_P (dest_succ));
  /* vsetvlmax  */
  if (REGNO (dest_succ) == VTYPE_REGNUM)
    {
      mode_succ = GET_MODE (dest_succ);
      dest_succ = SET_DEST (XVECEXP (pat_succ, 0, 2));
      src_succ = gen_rtx_REG (Pmode, GP_REG_FIRST);
    }
  else
    {
      src_succ = XVECEXP (SET_SRC (XVECEXP (pat_succ, 0, 0)), 0, 0);
      mode_succ = GET_MODE (SET_DEST (XVECEXP (pat_succ, 0, 2)));
    }

  /* vsetvlmax  */
  if (REGNO (dest_pred) == VTYPE_REGNUM)
    {
      mode_pred = GET_MODE (dest_pred);
      dest_pred = SET_DEST (XVECEXP (pat_pred, 0, 2));
      src_pred = gen_rtx_REG (Pmode, GP_REG_FIRST);
    }
  else
    {
      src_pred = XVECEXP (SET_SRC (XVECEXP (pat_pred, 0, 0)), 0, 0);
      mode_pred = GET_MODE (SET_DEST (XVECEXP (pat_pred, 0, 2)));
    }

  /* vsetvli a4,a5,e64,m1
     ......
     vsetvli a3,a4,e64,m1
     or
     vsetvli a4,a5,e64,m1
     ......
     vsetvli a3,a5,e64,m1*/
  if (mode_succ == mode_pred
      && (rtx_equal_p (dest_pred, src_succ)
	  || rtx_equal_p (src_pred, src_succ)))
    {
      if (dest)
	*dest = dest_succ;
      if (src)
	*src = src_succ;
      return true;
    }

  return false;
}

/* Try to delete successive vsetvl if two vsetvls are equal and
   successive vsetvl has no use.  */

static bool
try_to_delete_vsetvl(rtx_insn* pred_vsetvl_insn, rtx_insn* succ_vsetvl_insn, bool real = true)
{
  rtx src_reg, dst_reg;

  if (!pred_vsetvl_insn || !succ_vsetvl_insn)
    return false;

  if (dump_file)
    fprintf (dump_file, "Compare insn %d vs %d.\n", \
      INSN_UID (pred_vsetvl_insn), INSN_UID (succ_vsetvl_insn));

  if (vsetvl_rtx_equal_p (pred_vsetvl_insn, succ_vsetvl_insn, &src_reg, &dst_reg))
    {
      rtx reg_vl = gen_rtx_REG (VOIDmode, VL_REGNUM);
      rtx reg_vtype = gen_rtx_REG (VOIDmode, VTYPE_REGNUM);
      if (!reg_set_trace_p (src_reg, pred_vsetvl_insn, succ_vsetvl_insn)
	  && !reg_set_trace_p (reg_vl, pred_vsetvl_insn, succ_vsetvl_insn)
	  && !reg_set_trace_p (reg_vtype, pred_vsetvl_insn, succ_vsetvl_insn)
	  && !reg_used_following (succ_vsetvl_insn, dst_reg))
	{
	  if (real)
	    {
	      if (dump_file)
		fprintf (dump_file, "Delete insn %d.\n", INSN_UID (succ_vsetvl_insn));
	      delete_insn (succ_vsetvl_insn);
	    }
	  return true;
	}
    }
  return false;
}

static void
xthead_dvsetvl_locally ()
{
  basic_block bb;
  rtx_insn *insn, *next;

  rtx pat, pat_i;
  rtx_insn *vsetvl_insn;
  int unspec;

  do_ud_du_analysis ();

  FOR_EACH_BB_FN (bb, cfun)
    {
      vsetvl_insn = NULL;
      bb_vsetvl_info_t *info = ggc_cleared_alloc<bb_vsetvl_info_t> ();

      FOR_BB_INSNS_SAFE (bb, insn, next)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  if (!NONJUMP_INSN_P (insn))
	    vsetvl_insn = NULL;

	  pat = pat_i = PATTERN (insn);

	  if (GET_CODE (pat) == PARALLEL)
	    {
	      pat = XVECEXP (pat, 0, 0);
	    }

	  if (GET_CODE (pat) != SET)
	    continue;

	  pat = SET_SRC (pat);

	  /* Skip UNSPEC_WRITE_VTYPE.  */
          if (GET_CODE (pat) == UNSPEC
              && XINT (pat, 1) == UNSPEC_WRITE_VTYPE)
	    {
	      vsetvl_insn = NULL;
	      continue;
	    }

	  if (GET_CODE (pat) != UNSPEC)
	    continue;

	  unspec = XINT (pat, 1);

	  if (unspec == UNSPEC_VSETVL)
	    {
	      /* Skip vsetvl.  */
	      if (XVECLEN (pat_i, 0) == 2 && REG_P (SET_DEST (XVECEXP (pat_i, 0, 0)))
		  && REGNO (SET_DEST (XVECEXP (pat_i, 0, 0))) == VL_REGNUM
		  && REG_P (SET_DEST (XVECEXP (pat_i, 0, 1)))
		  && REGNO (SET_DEST (XVECEXP (pat_i, 0, 1))) == VTYPE_REGNUM)
		{
		  vsetvl_insn = NULL;
		  continue;
		}
	      if (vsetvl_insn)
		{
		  bool deleted = try_to_delete_vsetvl (vsetvl_insn, insn);

		  if (!deleted)
		    vsetvl_insn = insn;
		}
	      else
		vsetvl_insn = insn;

	      /* Set entry to first vsetvl.  */
	      if (riscv_insn_dvsetvl_globally)
		{
		  if (!info->entry)
		    info->entry = insn;
		  info->exit = insn;
		}
	    }
	}

	if (riscv_insn_dvsetvl_globally)
	  info_map->put (bb, *info);
    }
}

static void
xthead_dvsetvl_globally ()
{
  int *postorder = XNEWVEC (int, n_basic_blocks_for_fn (cfun));
  int postorder_num = post_order_compute (postorder, false, true);

  for (int i = postorder_num - 1; i >= 0; i--)
    {
      edge e;
      edge_iterator ei;
      basic_block bb = BASIC_BLOCK_FOR_FN (cfun, postorder[i]);
      bb_vsetvl_info_t *curr_info = info_map->get (bb);

      /* If current bb has multiple predecessors, try to delete entry.  */
      if (!single_pred_p (bb))
	{
	  bool equal = true;
	  bb_vsetvl_info_t *first_pred_bb_info = NULL;

	  /* Try to find out if predecessors have equal vsetvl.  */
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    {
	      basic_block pred_bb = e->src;
	      bb_vsetvl_info_t *pred_bb_info = info_map->get (pred_bb);

	      /* If predecessor is ENTRY or has no vsetvl, no way to delete.  */
	      if (pred_bb == ENTRY_BLOCK_PTR_FOR_FN (cfun)
		  || !pred_bb_info->exit)
		{
		  equal = false;
		  break;
		}

	      if (!first_pred_bb_info)
		{
		  first_pred_bb_info = pred_bb_info;
		  continue;
		}

	      if (!vsetvl_rtx_equal_p (first_pred_bb_info->exit,
				       pred_bb_info->exit,
				       NULL, NULL))
		{
		  equal = false;
		  break;
		}
	    }

	  /* If predecessors have equal vsetvl, try to delete entry.  */
	  if (equal)
	    {
	      bool deletable = true;

	      /* Try to find out if we can delete entry.  */
	      FOR_EACH_EDGE (e, ei, bb->preds)
		{
		  bb_vsetvl_info_t* pred_bb_info = info_map->get (e->src);
		  if (!try_to_delete_vsetvl (pred_bb_info->exit, curr_info->entry, false))
		    {
		      deletable = false;
		      break;
		    }
		}

	      if (deletable)
		{
		  delete_insn (curr_info->entry);
		}

	      /* If there is no vsetvl for now, then we pass vsetvl info forward.  */
	      if ((deletable && curr_info->entry == curr_info->exit)
		  || !curr_info->exit )
		{
		  info_map->put (bb, *first_pred_bb_info);
		  curr_info = first_pred_bb_info;
		}
	    }
	}

      /* Skip if there no vsetvl.  */
      if (!curr_info->exit)
	continue;

      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  basic_block succ_bb = e->dest;
	  if (succ_bb == EXIT_BLOCK_PTR_FOR_FN (cfun))
	    continue;

	  /* If successor has multiple predecessors, skip it currently.  */
	  if (!single_pred_p (succ_bb))
	    continue;

	  bb_vsetvl_info_t* succ_info = info_map->get (succ_bb);

	  /* If successor have no vsetvl then pass vsetvl info forward.  */
	  if (!succ_info->entry)
	    {
	      info_map->put (succ_bb, *curr_info);
	      continue;
	    }

	  if (try_to_delete_vsetvl (curr_info->exit, succ_info->entry))
	    {
	      /* If entry&exit are the same insn, then we pass vsetvl info forward.  */
	      if(succ_info->entry == succ_info->exit)
		info_map->put (succ_bb, *curr_info);
	    }
	}
    }

  XDELETEVEC(postorder);
}

static unsigned int
xthead_dvsetvl ()
{
  info_map = bb_vsetvl_info_map_t::create_ggc ();

  xthead_dvsetvl_locally ();

  if (riscv_insn_dvsetvl_globally)
    xthead_dvsetvl_globally ();

  return 0;
}

const pass_data pass_data_xthead_dvsetvl =
{
  RTL_PASS,		/* type */
  "dvsetvl",		/* name */
  OPTGROUP_NONE,	/* optinfo_flags */
  TV_NONE,		/* tv_id */
  0,			/* properties_required */
  0,			/* properties_provided */
  0,			/* properties_destroyed */
  0,			/* todo_flags_start */
  TODO_df_finish,	/* todo_flags_finish */
};

class pass_xthead_dvsetvl : public rtl_opt_pass
{
public:
  pass_xthead_dvsetvl (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_xthead_dvsetvl, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) { return TARGET_VECTOR && optimize > 0 && riscv_insn_dvsetvl; }
  unsigned int execute (function *) { return xthead_dvsetvl (); }
};

rtl_opt_pass *
make_pass_xthead_dvsetvl (gcc::context *ctxt)
{
  return new pass_xthead_dvsetvl (ctxt);
}

const pass_data pass_data_xthead_dvsetvl2 =
{
  RTL_PASS,		/* type */
  "dvsetvl2",		/* name */
  OPTGROUP_NONE,	/* optinfo_flags */
  TV_NONE,		/* tv_id */
  0,			/* properties_required */
  0,			/* properties_provided */
  0,			/* properties_destroyed */
  0,			/* todo_flags_start */
  TODO_df_finish,	/* todo_flags_finish */
};

class pass_xthead_dvsetvl2 : public rtl_opt_pass
{
public:
  pass_xthead_dvsetvl2 (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_xthead_dvsetvl2, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) { return TARGET_VECTOR && optimize > 0 && riscv_insn_dvsetvl2; }
  unsigned int execute (function *) { return xthead_dvsetvl (); }
};

rtl_opt_pass *
make_pass_xthead_dvsetvl2 (gcc::context *ctxt)
{
  return new pass_xthead_dvsetvl2 (ctxt);
}
