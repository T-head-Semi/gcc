/*  NOTE: The reason why there are many reads of vlenb is because
    vector registers are passed by stack currently since the lack
    of calling convention. When RVV calling convention is ratified,
    we may need to disable this pass.
    References:
    1. https://github.com/riscv-non-isa/riscv-elf-psabi-doc/pull/171  */

#define VLENB_X_REGISTER 27

static void
replace_reg_uses(rtx_insn *read_vlenb)
{
  struct df_link *def;
  df_ref def_ref = DF_INSN_DEFS (read_vlenb);

  for (def = DF_REF_CHAIN (def_ref); def; def = def->next)
    {
      if (DF_REF_IS_ARTIFICIAL (def->ref))
	continue;

      rtx_insn *def_insn = DF_REF_INSN (def->ref);
      rtx pat = PATTERN (def_insn);
      if (GET_CODE (pat) != SET)
	/* TODO: Maybe in parallel?  */
	continue;

      replace_insn_regnum (&SET_SRC (pat), SET_DEST (PATTERN (read_vlenb)),
			    VLENB_X_REGISTER);
    }
}

/* Return TRUE is INSN is stack tie pattern.  */
static bool
is_stack_tie (rtx_insn *insn)
{
  rtx pat = single_set (insn);

  if (pat)
    {
      rtx src = SET_SRC (pat);
      if (GET_CODE (src) == UNSPEC && XINT (src, 1) == UNSPEC_TIE)
	return true;
    }

  return false;
}

/* Delete redundant reads of vlenb.
   returns TRUE if we have deleted some reads. */
static bool
delete_redundant_read_vlenb(void)
{
  basic_block bb;
  rtx_insn *insn, *curr;

  int deleted_read_vlenb_count = 0;
  int frame_related_insn_count = 0;
  bool sp_use_by_nonframe = false;

  rtx_insn *first_read_vlenb = NULL;
  rtx_insn *prologue_save = NULL;
  rtx_insn *epilogue_restore = NULL;
  rtx pat, src, dest;

  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS_SAFE (bb, insn, curr)
	{
	  if (!NONDEBUG_INSN_P (insn) || !NONJUMP_INSN_P (insn))
	    continue;

	  if (RTX_FRAME_RELATED_P (insn))
	    frame_related_insn_count ++;
	  else if (reg_mentioned_p (stack_pointer_rtx, insn)
		   && !is_stack_tie (insn))
	    sp_use_by_nonframe = true;

	  pat = PATTERN (insn);
	  if (GET_CODE (pat) != SET)
	    continue;

	  src = SET_SRC (pat);
	  dest = SET_DEST (pat);

	  if (!prologue_save && GET_CODE (dest) == MEM
	      && GET_CODE (src) == REG && REGNO (src) == VLENB_X_REGISTER)
	    {
	      prologue_save = insn;
	      continue;
	    }

	  if (!epilogue_restore && find_reg_note (insn, REG_CFA_RESTORE, dest)
	      && GET_CODE (dest) == REG && REGNO (dest) == VLENB_X_REGISTER)
	    {
	      epilogue_restore = insn;
	      continue;
	    }

	  /* Refer to riscv.c:riscv_output_move. */
	  if (GET_CODE (dest) == REG && GP_REG_P (REGNO (dest))
	      && GET_CODE (src) == CONST_POLY_INT && satisfies_constraint_vp (src))
	    {
	      if (!first_read_vlenb)
		{
		  /* Found first insn of reading vlenb.  */
		  first_read_vlenb = insn;
		  if (dump_file)
		    fprintf (dump_file, "Found first insn of reading vlenb: %d.\n",
			     INSN_UID (insn));
		}
	      else
		{
		  if (dump_file)
		    fprintf (dump_file, "Delete insn %d.\n", INSN_UID (insn));

		  /* Replace all uses of new read with register s11 and then delete it.  */
		  replace_reg_uses (insn);

		  /* Delete redundant reads.  */
		  delete_insn (insn);
		  deleted_read_vlenb_count++;
		}
	    }
	}
    }

  if (dump_file)
    fprintf (dump_file, "Deleted %d redundant insn of reading vlenb .\n",
	     deleted_read_vlenb_count);

  /* If we have deleted some reads of vlenb. */
  if (deleted_read_vlenb_count > 0)
    {
      /* Replace uses of first read and store its value to register s11.  */
      replace_reg_uses (first_read_vlenb);
      rtx pat = PATTERN (first_read_vlenb);
      replace_insn_regnum (&pat, SET_DEST (pat), VLENB_X_REGISTER);
      return true;
    }
  else
    {
      /* Since there is no change, just remove s11 from prologue/epilogue. */
      if (prologue_save)
	delete_insn (prologue_save);
      if (epilogue_restore)
	delete_insn (epilogue_restore);

      /* If there are 4 frame related insns, which are store/load to
	 save/restore callsite and two operations on stack pointer.
	 We have deleted s11, which means we don't really need to
	 push/pop stack, we can delete these two insns too.  */
      if (frame_related_insn_count == 4 && !sp_use_by_nonframe)
	{
	  FOR_EACH_BB_FN (bb, cfun)
	    FOR_BB_INSNS_SAFE (bb, insn, curr)
	      {
		if (!NONDEBUG_INSN_P (insn)
		    || !NONJUMP_INSN_P (insn)
		    || !RTX_FRAME_RELATED_P (insn))
		  continue;

		pat = PATTERN (insn);
		if (GET_CODE (pat) != SET)
		  continue;

		src = SET_SRC (pat);
		dest = SET_DEST (pat);

		if (GET_CODE (dest) != REG || REGNO (dest) != STACK_POINTER_REGNUM)
		  continue;

		if (GET_CODE (src) != PLUS
		    || GET_CODE (XEXP(src, 0)) != REG
		    || REGNO (XEXP(src, 0)) != STACK_POINTER_REGNUM)
		  continue;

		delete_insn(insn);
	      }
	}
      return false;
    }
}

/* After this pass, we should reinit regs info to reuse register s11 for
   functions without vector operations.  */
static void
restore_regs(bool deleted)
{
  /* Set has_vector_ops_p to DELETED, so that epilogue (defined in target hook
     EPILOGUE_USES) will not use s11 if there is no read deleted. */
  cfun->machine->has_vector_ops_p = deleted;

  /* Rescan all insn and update entry&exit block. */
  df_insn_rescan_all ();
  df_compute_regs_ever_live (true);
  df_update_entry_exit_and_calls ();
  cfun->machine->has_vector_ops_p = false;
  reinit_regs ();
}

static unsigned int
xthead_dread_vlenb(void)
{
  do_ud_du_analysis ();
  bool deleted = delete_redundant_read_vlenb ();
  restore_regs (deleted);
  return 0;
}

const pass_data pass_data_xthead_dread_vlenb =
{
  RTL_PASS,       /* type */
  "dread_vlenb",  /* name */
  OPTGROUP_NONE,  /* optinfo_flags */
  TV_NONE,        /* tv_id */
  0,              /* properties_required */
  0,              /* properties_provided */
  0,              /* properties_destroyed */
  0,              /* todo_flags_start */
  TODO_df_finish, /* todo_flags_finish */
};

class pass_xthead_dread_vlenb : public rtl_opt_pass
{
public:
  pass_xthead_dread_vlenb(gcc::context *ctxt)
      : rtl_opt_pass(pass_data_xthead_dread_vlenb, ctxt)
  {
  }

  /* opt_pass methods: */
  bool gate(function *) {
    return TARGET_XTHEAD_DREAD_VLENB && !flag_rename_registers;
  }

  unsigned int execute(function *) { return xthead_dread_vlenb (); }
};

rtl_opt_pass *
make_pass_xthead_dread_vlenb(gcc::context *ctxt)
{
  return new pass_xthead_dread_vlenb (ctxt);
}

const pass_data pass_data_xthead_dread_vlenb_after_rnreg =
{
  RTL_PASS,       /* type */
  "dread_vlenb",  /* name */
  OPTGROUP_NONE,  /* optinfo_flags */
  TV_NONE,        /* tv_id */
  0,              /* properties_required */
  0,              /* properties_provided */
  0,              /* properties_destroyed */
  0,              /* todo_flags_start */
  TODO_df_finish, /* todo_flags_finish */
};

class pass_xthead_dread_vlenb_after_rnreg : public rtl_opt_pass
{
public:
  pass_xthead_dread_vlenb_after_rnreg(gcc::context *ctxt)
      : rtl_opt_pass(pass_data_xthead_dread_vlenb_after_rnreg, ctxt)
  {
  }

  /* opt_pass methods: */
  bool gate(function *) {
    return TARGET_XTHEAD_DREAD_VLENB && flag_rename_registers;
  }

  unsigned int execute(function *) { return xthead_dread_vlenb (); }
};

rtl_opt_pass *
make_pass_xthead_dread_vlenb_after_rnreg(gcc::context *ctxt)
{
  return new pass_xthead_dread_vlenb_after_rnreg (ctxt);
}
