
#define GET_UNSPEC(pat) (GET_CODE (pat) != PARALLEL ? XINT (SET_SRC (pat), 1) \
			 : XINT (SET_SRC (XVECEXP (pat, 0, 0)), 1))
#define GET_UNSPEC_MODE(pat) (GET_CODE (pat) != PARALLEL ? GET_MODE (SET_SRC (pat)) \
			      : GET_MODE (SET_SRC (XVECEXP (pat, 0, 0))))

#define GET_VSETVLI_MODE(pat)                 \
  ((GET_UNSPEC_MODE (pat) == VNx2BImode \
    || GET_UNSPEC_MODE (pat) == VNx4BImode \
    || GET_UNSPEC_MODE (pat) == VNx8BImode \
    || GET_UNSPEC_MODE (pat) == VNx16BImode \
    || GET_UNSPEC_MODE (pat) == VNx32BImode \
    || GET_UNSPEC_MODE (pat) == VNx64BImode \
    || GET_UNSPEC_MODE (pat) == VNx128BImode) \
   ? VNx16QImode \
   : GET_UNSPEC_MODE (pat) == VNx2HFmode ? VNx2HImode	\
   : GET_UNSPEC_MODE (pat) == VNx4HFmode ? VNx4HImode	\
   : GET_UNSPEC_MODE (pat) == VNx8HFmode ? VNx8HImode	\
   : GET_UNSPEC_MODE (pat) == VNx16HFmode ? VNx16HImode	\
   : GET_UNSPEC_MODE (pat) == VNx32HFmode ? VNx32HImode	\
   : GET_UNSPEC_MODE (pat) == VNx64HFmode ? VNx64HImode	\
   : GET_UNSPEC_MODE (pat) == VNx2SFmode ? VNx2SImode	\
   : GET_UNSPEC_MODE (pat) == VNx4SFmode ? VNx4SImode	\
   : GET_UNSPEC_MODE (pat) == VNx8SFmode ? VNx8SImode	\
   : GET_UNSPEC_MODE (pat) == VNx16SFmode ? VNx16SImode	\
   : GET_UNSPEC_MODE (pat) == VNx32SFmode ? VNx32SImode	\
   : GET_UNSPEC_MODE (pat) == VNx2DFmode ? VNx2DImode	\
   : GET_UNSPEC_MODE (pat) == VNx4DFmode ? VNx4DImode	\
   : GET_UNSPEC_MODE (pat) == VNx8DFmode ? VNx8DImode	\
   : GET_UNSPEC_MODE (pat) == VNx16DFmode ? VNx16DImode	\
   : GET_UNSPEC_MODE (pat))

#define PAT_VSETVLI_X0(pat) (gen_rtx_SET (gen_rtx_REG (GET_VSETVLI_MODE (pat), VTYPE_REGNUM),\
					  gen_rtx_UNSPEC (GET_VSETVLI_MODE (pat), gen_rtvec (1, const0_rtx), UNSPEC_VSETVL)))
#define PAT_READ_VL(pat) (gen_rtx_SET (TARGET_VECTOR_TEMP2 (Pmode),\
				       gen_rtx_UNSPEC_VOLATILE (Pmode, gen_rtvec (1, gen_rtx_REG (Pmode, VL_REGNUM)), UNSPEC_READ_VL)))
#define PAT_READ_VTYPE(pat) (gen_rtx_SET (TARGET_VECTOR_TEMP (Pmode),\
					  gen_rtx_UNSPEC (Pmode, gen_rtvec (1, gen_rtx_REG (Pmode, VTYPE_REGNUM)), UNSPEC_READ_VTYPE)))

#define PAT_WRITE_VL_VTYPE(pat) (gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,\
  gen_rtx_SET (gen_rtx_REG (Pmode, VL_REGNUM), gen_rtx_UNSPEC (Pmode, gen_rtvec (1, TARGET_VECTOR_TEMP2 (Pmode)), UNSPEC_VSETVL)),\
  gen_rtx_SET (gen_rtx_REG (Pmode, VTYPE_REGNUM), gen_rtx_UNSPEC (Pmode, gen_rtvec (1, TARGET_VECTOR_TEMP (Pmode)), UNSPEC_VSETVL)))))

#define SET_UNSPEC(pat) (XINT (SET_SRC (pat), 1))


static void do_vec_elt_lowering (rtx_insn *elt) {
    if (dump_file)
      fprintf (dump_file, "Lowering insn %d.\n", INSN_UID (elt));
    rtx ipat = PATTERN (elt);
    rtx pat = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (2));
    rtx src = SET_SRC (ipat);
    rtx dest = SET_DEST (ipat);
    rtx new_mov = gen_rtx_SET (dest, gen_rtx_UNSPEC (GET_MODE (src),
						     gen_rtvec(2,
							       XVECEXP (src, 0, 0),
							       gen_rtx_REG (SImode, VL_REGNUM)),
						     UNSPEC_USEVL));
    XVECEXP (pat, 0, 0) = new_mov;
    XVECEXP (pat, 0, 1) = gen_rtx_USE (VOIDmode, gen_rtx_REG (GET_VSETVLI_MODE (ipat), VTYPE_REGNUM));
    emit_insn_before(pat, elt);
    delete_insn (elt);
}

static vec<rtx_insn *> vwldst_insns;
static rtx_insn *g_vsetvl_insn;

static bool
check_unspec (rtx pat)
{
  if (GET_CODE (pat) == PARALLEL)
    pat = XVECEXP (pat, 0, 0);
  if (GET_CODE (pat) != SET)
    return false;
  pat = SET_SRC (pat);
  if (GET_CODE (pat) != UNSPEC)
    return false;
  return true;
}

static void
try_to_hoist_vwldst()
{
  basic_block bb;
  rtx_insn *insn;

  rtx pat;
  rtx_insn *vsetvl_insn;

  FOR_EACH_BB_FN (bb, cfun)
    {
      vsetvl_insn = NULL;

      FOR_BB_INSNS (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  if (!NONJUMP_INSN_P (insn))
	    vsetvl_insn = NULL;

	  pat = PATTERN (insn);
	  if (GET_CODE (pat) == PARALLEL)
	    pat = XVECEXP (pat, 0, 0);

	  if (GET_CODE (pat) != SET)
	    continue;
	  pat = SET_SRC (pat);

	  if (GET_CODE (pat) != UNSPEC)
	    continue;

	  if (GET_UNSPEC (PATTERN (insn)) == UNSPEC_VSETVL)
	    vsetvl_insn = insn;
	  else if (GET_UNSPEC (PATTERN (insn)) >= UNSPEC_FIRST && GET_UNSPEC (PATTERN (insn)) < UNSPEC_VSETVL)
	    vsetvl_insn = NULL;

	  if (GET_UNSPEC (PATTERN (insn)) == UNSPEC_VWLDST
	      && vsetvl_insn)
	    {
	      rtx vsetvl = XVECEXP (PATTERN (vsetvl_insn), 0, 0);
	      rtx src = XVECEXP (SET_SRC (vsetvl), 0, 0);;
	      rtx dst = SET_DEST (vsetvl);

	      if (!reg_set_between_p (src, vsetvl_insn, insn)
		  && !reg_used_between_p (dst, vsetvl_insn, insn)
		  && !reg_set_between_p (dst, vsetvl_insn, insn))
		{
		  if (dump_file)
		    fprintf (dump_file, "Hoisting insn %d.\n", INSN_UID (insn));
		  remove_insn (vsetvl_insn);
		  add_insn_after (vsetvl_insn, insn, BLOCK_FOR_INSN (insn));
		}
	    }
	}
    }
}

static rtx_insn *
try_to_lower_vwldst(rtx_insn *pred_insn, rtx_insn *insn)
{
  int i;
  rtx pat = PATTERN (insn);
  rtx_insn *vwldst = vwldst_insns.is_empty () ? NULL : vwldst_insns[0];

  if (!NONJUMP_INSN_P (insn))
    {
      if (vwldst == NULL)
	return NULL;
      if (pred_insn && GET_UNSPEC (PATTERN (pred_insn)) == UNSPEC_VSETVL)
	return NULL;

      for (rtx_insn *in = NEXT_INSN (insn); in != NEXT_INSN (BB_END (BLOCK_FOR_INSN (insn))); in = NEXT_INSN (in))
	{
	  if (!INSN_P (in) || !check_unspec (PATTERN (in)))
	    continue;
	  if (GET_UNSPEC (PATTERN (in)) >= UNSPEC_FIRST && GET_UNSPEC (PATTERN (in)) <= UNSPEC_VSETVL)
	    {
	      if (GET_UNSPEC (PATTERN (in)) == UNSPEC_VSETVL)
		break;
	      return NULL;
	    }
	}

      if (g_vsetvl_insn == NULL)
	return NULL;
      if (g_vsetvl_insn
	  && (reg_used_following (g_vsetvl_insn, gen_rtx_REG (VOIDmode, VL_REGNUM), false)
	      || reg_used_following (g_vsetvl_insn, gen_rtx_REG (VOIDmode, VTYPE_REGNUM), false)))
	return NULL;

      emit_insn_before (PAT_VSETVLI_X0 (PATTERN (vwldst)), vwldst);
      FOR_EACH_VEC_ELT (vwldst_insns, i, vwldst)
	do_vec_elt_lowering (vwldst);

      vwldst_insns.release ();
      return NULL;
    }

  if (GET_UNSPEC (pat) == UNSPEC_VSETVL)
    {
      if (vwldst != NULL)
	{
	  emit_insn_before (PAT_VSETVLI_X0 (PATTERN (vwldst)), vwldst);
	  FOR_EACH_VEC_ELT (vwldst_insns, i, vwldst)
	    do_vec_elt_lowering (vwldst);
	}
      vwldst_insns.release ();
      g_vsetvl_insn = insn;
      return insn;
    }

  if (GET_UNSPEC (pat) == UNSPEC_VWLDST)
    {
      if (vwldst == NULL
	  || GET_UNSPEC_MODE (pat) == GET_UNSPEC_MODE (PATTERN (vwldst))
	  || GET_MODE_SIZE (GET_UNSPEC_MODE (pat)).to_constant () == GET_MODE_SIZE (GET_UNSPEC_MODE (PATTERN (vwldst))).to_constant ())
	vwldst_insns.safe_push (insn);
      else
	{
	  vwldst_insns.release ();
	  vwldst_insns.safe_push (insn);
	}
    }
  else if (GET_UNSPEC (pat) >= UNSPEC_FIRST && GET_UNSPEC (pat) < UNSPEC_VSETVL)
    {
      if (vwldst_insns.length () > 1)
	{
	  emit_insn_before (PAT_READ_VL (PATTERN (vwldst)), vwldst);
	  emit_insn_before (PAT_READ_VTYPE (PATTERN (vwldst)), vwldst);
	  emit_insn_before (PAT_VSETVLI_X0 (PATTERN (vwldst)), vwldst);
	  emit_insn_after (PAT_WRITE_VL_VTYPE (PATTERN (vwldst)), vwldst_insns.last());
	  FOR_EACH_VEC_ELT (vwldst_insns, i, vwldst)
	    do_vec_elt_lowering (vwldst);
	}
      vwldst_insns.release ();
      return insn;
    }

  return pred_insn;
}

static unsigned int
xthead_dvsetvl_v0p7 ()
{
  basic_block bb;
  rtx_insn *insn;

  rtx_insn *vsetvl_insn;
  vwldst_insns.create (0);

  try_to_hoist_vwldst ();

  df_set_flags (DF_RD_PRUNE_DEAD_DEFS);
  df_chain_add_problem (DF_UD_CHAIN | DF_DU_CHAIN);
  df_analyze ();
  df_set_flags (DF_DEFER_INSN_RESCAN);

  FOR_EACH_BB_FN (bb, cfun)
    {
      vsetvl_insn = NULL;
      vwldst_insns.release ();
      g_vsetvl_insn = vsetvl_insn;

      FOR_BB_INSNS (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  if (!NONJUMP_INSN_P (insn))
	    vsetvl_insn = try_to_lower_vwldst (vsetvl_insn, insn);

	  if (!check_unspec (PATTERN (insn)))
	    continue;

	  vsetvl_insn = try_to_lower_vwldst (vsetvl_insn, insn);
	}
    }
  return 0;
}

const pass_data pass_data_xthead_dvsetvl_v0p7 =
{
  RTL_PASS,		/* type */
  "dvsetvl_v0p7",	/* name */
  OPTGROUP_NONE,	/* optinfo_flags */
  TV_NONE,		/* tv_id */
  0,			/* properties_required */
  0,			/* properties_provided */
  0,			/* properties_destroyed */
  0,			/* todo_flags_start */
  TODO_df_finish,	/* todo_flags_finish */
};

class pass_xthead_dvsetvl_v0p7 : public rtl_opt_pass
{
public:
  pass_xthead_dvsetvl_v0p7 (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_xthead_dvsetvl_v0p7, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) { return TARGET_VECTOR && optimize > 0 && target_subset_version_p ("v", 0, 7) && riscv_insn_dvsetvl_v0p7; }
  unsigned int execute(function *) { return xthead_dvsetvl_v0p7(); }
};

rtl_opt_pass *
make_pass_xthead_dvsetvl_v0p7 (gcc::context *ctxt)
{
  return new pass_xthead_dvsetvl_v0p7 (ctxt);
}
