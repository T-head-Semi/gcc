
/* There are two delete sext passes, one before ree,
   and one after ree. Set it to true when it's the last one.
   The first pass aims to delete the redundancy sext,
   The second pass will change the redundancy sext to mov if
   it cannot be deleted.  */
static bool last_dsext = false;
static sbitmap sext_marked;

static void mark_insn (rtx_insn *insn)
{
  bitmap_set_bit (sext_marked, INSN_UID (insn));

  if (dump_file)
    fprintf (dump_file, "Add insn %d to sext queue.\n",
	     INSN_UID (insn));
}

static void init_dsext_data (void)
{
  basic_block bb;
  sext_marked = sbitmap_alloc (get_max_uid () + 1);
  bitmap_clear (sext_marked);

  if (dump_file)
    fprintf (dump_file, "-------- Initialize dsext data --------\n\n");

  FOR_EACH_BB_FN (bb, cfun)
    {
      rtx_insn *insn;

      FOR_BB_INSNS (bb, insn)
	{
	  if (NONJUMP_INSN_P (insn)
	      && GET_CODE (PATTERN (insn)) == SET
	      && REG_P (SET_DEST (PATTERN (insn))))
	    {
	      rtx source = SET_SRC (PATTERN (insn));
	      enum rtx_code code = GET_CODE (source);
	      machine_mode mode = GET_MODE (SET_DEST (PATTERN (insn)));

	      if (mode == SImode)
		{
		  if (code == PLUS || code == MINUS
		      || code == MULT || code == DIV
		      || code == UDIV || code == BSWAP
		      || code == ASHIFT || code == ASHIFTRT
		      || code == LSHIFTRT || code == ROTATERT
		      || code == CONST_INT)
		    mark_insn (insn);
		}
	      else if (CONST_INT_P (source))
		{
		  /* If the mode is DI, or when the sign bit is 0 for other mode,
		     we assume the dest reg is sign extened.  */
		  if (mode == DImode && !(INTVAL (source) &
					  HOST_WIDE_INT_C (0xffffffff80000000)))
		    mark_insn (insn);
		  else if (!(INTVAL (source) &
			     ~((HOST_WIDE_INT_C (1) <<
				(GET_MODE_SIZE (mode).to_constant () - 1)) - 1)))
		    mark_insn (insn);
		}
	      else if (mode == DImode && code == SIGN_EXTEND
		       && GET_MODE (XEXP (source, 0)) == SImode)
		mark_insn (insn);
	    }
	}
    }

  if (dump_file)
    fprintf (dump_file, "\n-------- Finish initializing --------\n\n");
}

/* Return TRUE if EXP can be propagated when
   their input operands are all sign extended.  */

static bool
can_propagate_3addr_code (rtx exp)
{
  enum rtx_code code = GET_CODE (exp);

  switch (code)
  {
    case IF_THEN_ELSE:
      if (REG_P (XEXP (exp, 1)) && REG_P (XEXP (exp, 2)))
	return true;
      else
	return false;

    case AND:
    case IOR:
      if (REG_P (XEXP (exp, 0)) && REG_P (XEXP (exp, 1)))
	return true;
      else
	return false;

    default:
      return false;
  }
}

/* Scan insns to find which registers have been sign extended.
   For example,
	(set (reg:DI a) (sign_extend:DI (reg:SI b)))
	(set (reg:DI c) (reg:DI a))
   then, the register 'c' is sign extended and the second insn
   will be propagated to have sext attribute. */

static bool
propagate_sext_attribute (void)
{
  basic_block bb;
  bool changed = false;

  FOR_EACH_BB_FN (bb, cfun)
    {
      rtx_insn *insn;

      FOR_BB_INSNS (bb, insn)
	{
	  if (single_set (insn)
	      && REG_P (SET_DEST (PATTERN (insn))))
	    {
	      rtx source = SET_SRC (PATTERN (insn));
	      machine_mode src_mode = GET_MODE (source);
	      enum rtx_code code = GET_CODE (source);
	      df_ref use;
	      bool can_propagate = true;

	      if ((src_mode != SImode && src_mode != DImode)
		  || (code != REG && !can_propagate_3addr_code (source)))
		continue;

	      if (bitmap_bit_p (sext_marked, INSN_UID (insn)))
		continue;

	      if (dump_file)
		fprintf (dump_file, "Processing insn %d\n", INSN_UID (insn));

	      FOR_EACH_INSN_USE (use, insn)
		{
		  struct df_link *defs;
		  rtx reg = DF_REF_REG (use);
		  int unartificial_refs = 0;

		  if (!REG_P (reg))
		    continue;

		  if (code == IF_THEN_ELSE
		      && REGNO (reg) != REGNO (XEXP (source, 1))
		      && REGNO (reg) != REGNO (XEXP (source, 2)))
		    {
		      if (dump_file)
			fprintf (dump_file, "Skip reg %d\n", REGNO (reg));
		      continue;
		    }

		  if (dump_file)
		    fprintf (dump_file, "Use reg %d\n", REGNO (reg));

		  for (defs = DF_REF_CHAIN (use); defs; defs = defs->next)
		    {
		      rtx_insn *ref_insn;

		      if (DF_REF_IS_ARTIFICIAL (defs->ref))
			continue;
		      unartificial_refs ++;
		      ref_insn = DF_REF_INSN (defs->ref);

		      if (!bitmap_bit_p (sext_marked, INSN_UID (ref_insn)))
			can_propagate = false;

		      if (dump_file)
			{
			  if (bitmap_bit_p (sext_marked, INSN_UID (ref_insn)))
			    fprintf (dump_file, "Def insn %d is sexted\n", INSN_UID (ref_insn));
			  else
			    fprintf (dump_file, "Def insn %d is not sexted\n", INSN_UID (ref_insn));
			}
		    }

		  /* If the DEF cannot found, it is most likely a parameter.
		     It need to promote and cannot mark it sexted.  */
		  if (!unartificial_refs)
		    can_propagate = false;
		}

		if (can_propagate)
		  {
		    bitmap_set_bit (sext_marked, INSN_UID (insn));
		    changed = true;

		    if (dump_file)
		      fprintf (dump_file, "Propagate successfully!\n\n");
		  }
	    }
	}
    }

  return changed;
}

static bool
can_delete_sext (rtx_insn *insn)
{
  df_ref use = DF_INSN_USES (insn);
  df_ref def = DF_INSN_DEFS (insn);
  rtx src_reg = DF_REF_REG (use);
  rtx dest_reg = DF_REF_REG (def);
  struct df_link *use_of_def;
  auto_vec<rtx_insn *> use_insns_list;

  if (dump_file)
    fprintf (dump_file, "Check whether all uses of the define of insn %d can be repalced\n",
	     INSN_UID (insn));

  for (use_of_def = DF_REF_CHAIN (def); use_of_def; use_of_def = use_of_def->next)
    {
      struct df_link *defs;
      rtx_insn *use_insn;

      if (!DF_REF_INSN_INFO (use_of_def->ref))
	continue;

      use_insn = DF_REF_INSN (use_of_def->ref);

      if (DEBUG_INSN_P (use_insn))
	continue;

      if (!NONJUMP_INSN_P (use_insn) || GET_CODE (PATTERN (use_insn)) != SET)
	{
	  if (dump_file)
	    fprintf (dump_file, "Use insn %d is not a single set insn, failed!\n",
		     INSN_UID (use_insn));
	  return false;
	}

      if (GET_CODE (SET_SRC (PATTERN (use_insn))) == ASM_OPERANDS)
	{
	  if (dump_file)
	    fprintf (dump_file, "Use insn %d is a asm insn, failed!\n",
		     INSN_UID (use_insn));
	  return false;
	}

      for (defs = DF_REF_CHAIN (use_of_def->ref); defs; defs = defs->next)
	{
	  if (!DF_REF_INSN_INFO (defs->ref))
	    continue;

	  rtx_insn *defs_insn = DF_REF_INSN (defs->ref);
	  if (defs_insn != insn)
	    {
	      if (dump_file)
		fprintf (dump_file, "Use insn %d is defined by other insn, failed!\n",
			 INSN_UID (use_insn));
	      return false;
	    }
	}

      /* If the USE_INSN's dest is not reg and it define the
	 same register INSN defines, we cannot handle this
	 situation.  */
      if (!REG_P (SET_DEST (PATTERN (use_insn))))
	{
	  df_ref def_of_use_insn;

	  df_insn_info *insn_info = DF_INSN_INFO_GET (use_insn);
	  FOR_EACH_INSN_INFO_DEF (def_of_use_insn, insn_info)
	    {
	      if (DF_REF_REGNO (def_of_use_insn) == REGNO (dest_reg))
		{
		  if (dump_file)
		    fprintf (dump_file, "Use insn %d define the same reg but the dest is not reg, failed!.\n",
			     INSN_UID (use_insn));
		  return false;
		}
	    }
	}

      if (reg_set_trace_p (src_reg, insn, use_insn))
	{
	  if (dump_file)
	    fprintf (dump_file, "The source reg may be changed between sext and insn %d, failed.\n",
		     INSN_UID (use_insn));
	  return false;
	}

      use_insns_list.safe_push (use_insn);
    }

  /* If there is no USE_INSN, we can delete INSN directly.  */
  if (use_insns_list.is_empty ())
    {
      if (dump_file)
	fprintf (dump_file, "No use insn, delete it.\n");
      return true;
    }
  else
    {
      rtx_insn *member;
      unsigned int i;

      FOR_EACH_VEC_ELT (use_insns_list, i, member)
	{
	  rtx new_pattern = copy_rtx ((rtx)PATTERN (member));
	  if (REG_P (SET_DEST (new_pattern)))
	    replace_insn_regnum (&SET_SRC (new_pattern),
				 dest_reg, REGNO (src_reg));
	  else
	    replace_insn_regnum (&new_pattern,
				 dest_reg, REGNO (src_reg));
	  validate_change (member, &PATTERN (member), new_pattern, true);
	  if (dump_file)
	    fprintf (dump_file, "\tUse insn %d has been replaced.\n",
		     INSN_UID (member));
	}
    }

  if (apply_change_group ())
    {
      rtx_insn *member;
      unsigned int i;

      FOR_EACH_VEC_ELT (use_insns_list, i, member)
	df_insn_rescan (member);
      df_analyze ();
      if (dump_file)
	fprintf (dump_file, "All uses has been replaced.\n");
      return true;
    }
  else
    {
      if (dump_file)
	fprintf (dump_file, "Found illegal replacement, cancel changes.\n");
      return false;
    }
}

static bool
is_lbu_insn (rtx_insn * insn)
{
  rtx pattern = PATTERN (insn);
  if (GET_CODE (pattern) == SET)
    {
      rtx src = SET_SRC (pattern);
      rtx dest = SET_DEST (pattern);

      if (GET_MODE (dest) == DImode && GET_CODE (src) == ZERO_EXTEND)
	{
	  rtx inner_src = XEXP (src, 0);
	    if (GET_CODE (inner_src) == MEM && GET_MODE (inner_src) == QImode)
	      return true;
	}
    }

  return false;
}

static void
delect_redundancy_sext (void)
{
  basic_block bb;

  if (dump_file)
    fprintf (dump_file, "-------- Start delecting sext --------\n\n");

  FOR_EACH_BB_FN (bb, cfun)
    {
      rtx_insn *insn;

      FOR_BB_INSNS (bb, insn)
	{
	  if (!(NONJUMP_INSN_P (insn)
		&& GET_CODE (PATTERN (insn)) == SET))
	    continue;

	  rtx source = SET_SRC (PATTERN (insn));
	  rtx dest = SET_DEST (PATTERN (insn));

	  if (GET_MODE (dest) == DImode
	      && REG_P (dest)
	      && GET_CODE (source) == SIGN_EXTEND
	      && REG_P (XEXP (source, 0))
	      && GET_MODE (XEXP (source, 0)) == SImode)
	    {
	      bool can_delete = true;
	      struct df_link *defs;
	      df_ref use = DF_INSN_USES (insn);
	      int unatificial_defs = 0;
	      gcc_assert (use);

	      for (defs = DF_REF_CHAIN (use); defs; defs = defs->next)
		{
		  rtx_insn *ref_insn;

		  if (DF_REF_IS_ARTIFICIAL (defs->ref))
		    {
		      can_delete = false;
		      break;
		    }
		  ref_insn = DF_REF_INSN (defs->ref);

		  if (!bitmap_bit_p (sext_marked, INSN_UID (ref_insn)))
		    {
		      can_delete = false;
		      break;
		    }
		  else
		    unatificial_defs ++;
		}

	      if (can_delete && !unatificial_defs)
		can_delete = false;

	      /* Detect the sequence:
			lbu rx, (ry, n)
			sext.w rz, rx  */
	      if (!can_delete)
		{
		  can_delete = true;
		  unatificial_defs = 0;
		  for (defs = DF_REF_CHAIN (use); defs; defs = defs->next)
		    {
		      rtx_insn *ref_insn;

		      if (DF_REF_IS_ARTIFICIAL (defs->ref))
			{
			  can_delete = false;
			  break;
			}
		      ref_insn = DF_REF_INSN (defs->ref);

		      if (!is_lbu_insn (ref_insn))
			{
			  can_delete = false;
			  break;
			}
		      else
			unatificial_defs ++;
		    }

		  if (can_delete && !unatificial_defs)
		    can_delete = false;
		}

	      if (can_delete)
		{
		  if (REGNO (dest) == REGNO (XEXP (source, 0)))
		    {
		      if (dump_file)
			fprintf (dump_file, "The source reg and dest reg are some, delete it.\n");
		       delete_insn (insn);
		    }
		  else if (can_delete_sext (insn))
		    delete_insn (insn);
		  // TODO insn can be combined by prev insns.
		  else if (last_dsext)
		    {
		      SET_SRC (PATTERN (insn)) = gen_rtx_REG (GET_MODE (dest),
							      REGNO (XEXP (source, 0)));
		      INSN_CODE (insn) = -1;
		      df_insn_rescan (insn);
		    }
		}
	    }
	}
    }

  if (dump_file)
    fprintf (dump_file, "-------- Finish delecting sext --------\n\n");
}

static unsigned int
rest_of_handle_dsext (void)
{
  df_set_flags (DF_RD_PRUNE_DEAD_DEFS);
  df_chain_add_problem (DF_UD_CHAIN | DF_DU_CHAIN);
  df_analyze ();
  df_set_flags (DF_DEFER_INSN_RESCAN);

  init_dsext_data ();

  if (dump_file)
    fprintf (dump_file, "-------- Start propagation --------\n\n");

  while (propagate_sext_attribute ());

  if (dump_file)
    fprintf (dump_file, "-------- Finish propagation --------\n\n");

  delect_redundancy_sext ();

  sbitmap_free (sext_marked);
  return 0;
}

const pass_data pass_data_dsext =
{
    RTL_PASS, /* type */
    "dsext", /* name */
    OPTGROUP_NONE, /* optinfo_flags */
    TV_NONE, /* tv_id */
    0, /* properties_required */
    0, /* properties_provided */
    0, /* properties_destroyed */
    0, /* todo_flags_start */
    TODO_df_finish, /* todo_flags_finish */
};

class pass_delete_redundancy_sext1 : public rtl_opt_pass
{
  public:
      pass_delete_redundancy_sext1 (gcc::context *ctxt)
	  : rtl_opt_pass (pass_data_dsext, ctxt)
      {}
  /* opt_pass methods: */
  virtual bool gate (function *)
  { return riscv_insn_dsext1 && optimize > 0; }

  virtual unsigned int execute (function *)
  { last_dsext = false; return rest_of_handle_dsext ();}
};

rtl_opt_pass *
make_pass_delete_redundancy_sext1 (gcc::context *ctxt)
{
    return new pass_delete_redundancy_sext1 (ctxt);
}

class pass_delete_redundancy_sext2 : public rtl_opt_pass
{
  public:
      pass_delete_redundancy_sext2 (gcc::context *ctxt)
	  : rtl_opt_pass (pass_data_dsext, ctxt)
      {}
  /* opt_pass methods: */
  virtual bool gate (function *)
  { return riscv_insn_dsext2 && optimize > 0; }

  virtual unsigned int execute (function *)
  { last_dsext = true; return rest_of_handle_dsext ();}
};

rtl_opt_pass *
make_pass_delete_redundancy_sext2 (gcc::context *ctxt)
{
    return new pass_delete_redundancy_sext2 (ctxt);
}
