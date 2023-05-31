
#define INCLUDE_STRING
#include "opts.h"
#include "riscv-thead-tune.h"
#include "insn-attr.h"
#include "riscv-subset.h"
#include "sched-int.h"

bool
target_subset_version_p (const char *subset, int major, int minor)
{
  return TARGET_SUBSET_VERSION_P (subset, major, minor);
}

void
riscv_xthead_option_override (const struct riscv_tune_param *tune_param,
			      struct gcc_options *opts,
			      struct gcc_options *opts_set)
{
  if (!optimize_size)
    {
      SET_OPTION_IF_UNSET (opts, opts_set,
			   param_max_inline_insns_single,
			   400);
      SET_OPTION_IF_UNSET (opts, opts_set,
			   param_max_inline_insns_auto,
			   30);
    }

  SET_OPTION_IF_UNSET (opts, opts_set,
		       g_switch_value,
		       0);

  if (optimize == 2)
    SET_OPTION_IF_UNSET (opts, opts_set,
			 flag_tree_loop_distribute_patterns,
			 0);

  if (optimize == 3)
    SET_OPTION_IF_UNSET (opts, opts_set,
			 riscv_ipa_unhot_inline,
			 1);

  /* If not optimizing for size, set the default
     alignment to what the target wants.  */
  if (!opts->x_optimize_size)
    {
      if (opts->x_flag_align_loops && !opts->x_str_align_loops)
	opts->x_str_align_loops = tune_param->loop_align;
      if (opts->x_flag_align_jumps && !opts->x_str_align_jumps)
	opts->x_str_align_jumps = tune_param->jump_align;
      if (opts->x_flag_align_functions && !opts->x_str_align_functions)
	opts->x_str_align_functions = tune_param->function_align;
    }

  if (!opts_set->x_riscv_4k_optimize && riscv_microarchitecture == c910)
    opts->x_riscv_4k_optimize = 1;

  if (riscv_libcc_runtime && TARGET_64BIT)
    warning (0, "mccrt is only supported in rv32* arch");

  if (target_subset_version_p ("v", 0, 7))
    SET_OPTION_IF_UNSET (opts, opts_set,
			 riscv_rvv_vector_bits,
			 RVV_128);
}

bool
riscv_xthead_rtx_costs (rtx x, machine_mode mode ATTRIBUTE_UNUSED, int outer_code,
			int opno ATTRIBUTE_UNUSED, int *total,
			bool speed ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (x))
    {
    case IF_THEN_ELSE:
      if (!optimize_size && TARGET_XTHEAD_CONDMV
	  && GET_CODE (XEXP (x, 1)) == REG && GET_CODE (XEXP (x, 2)) == REG)
	{
	  *total = COSTS_N_INSNS (1);
	  return true;
	}
      return false;
    case ZERO_EXTRACT:
      if (outer_code == SET && TARGET_XTHEAD_EXT)
	{
	  *total = COSTS_N_INSNS (SINGLE_SHIFT_COST);
	  return true;
	}
      return false;
    default:
      return false;
    }
}

/* Return true if address offset is a valid index.  If it is, fill in INFO
   appropriately.  STRICT_P is true if REG_OK_STRICT is in effect.  */

bool
riscv_classify_address_index (struct riscv_address_info *info, rtx x,
      machine_mode mode, bool strict_p)
{
  enum riscv_address_type type = ADDRESS_REG_REG;;
  rtx index;
  int shift = 0;

  if (!TARGET_XTHEAD_LDR)
    return false;

  if (!TARGET_64BIT && mode == DImode)
    return false;

  if (SCALAR_FLOAT_MODE_P (mode))
    {
      if (!TARGET_HARD_FLOAT)
	return false;
      if (!TARGET_XTHEAD_FLDR)
	return false;
      if (GET_MODE_SIZE (mode).to_constant () == 2)
	return false;
    }

  /* (reg:P) */
  if ((REG_P (x) || GET_CODE (x) == SUBREG)
      && GET_MODE (x) == Pmode)
    {
      index = x;
      shift = 0;
    }
  /* (zero_extend:DI (reg:SI)) */
  else if (GET_CODE (x) == ZERO_EXTEND
	   && GET_MODE (x) == DImode
	   && GET_MODE (XEXP (x, 0)) == SImode)
    {
      type = ADDRESS_REG_UREG;
      index = XEXP (x, 0);
      shift = 0;
    }
  /* (mult:DI (zero_extend:DI (reg:SI)) (const_int scale)) */
  else if (GET_CODE (x) == MULT
	   && GET_CODE (XEXP (x, 0)) == ZERO_EXTEND
	   && GET_MODE (XEXP (x, 0)) == DImode
	   && GET_MODE (XEXP (XEXP (x, 0), 0)) == SImode
	   && CONST_INT_P (XEXP (x, 1)))
    {
      type = ADDRESS_REG_UREG;
      index = XEXP (XEXP (x, 0), 0);
      shift = exact_log2 (INTVAL (XEXP (x, 1)));
    }
  /* (ashift:DI (zero_extend:DI (reg:SI)) (const_int shift)) */
  else if (GET_CODE (x) == ASHIFT
	   && GET_CODE (XEXP (x, 0)) == ZERO_EXTEND
	   && GET_MODE (XEXP (x, 0)) == DImode
	   && GET_MODE (XEXP (XEXP (x, 0), 0)) == SImode
	   && CONST_INT_P (XEXP (x, 1)))
    {
      type = ADDRESS_REG_UREG;
      index = XEXP (XEXP (x, 0), 0);
      shift = INTVAL (XEXP (x, 1));
    }
  /* (mult:P (reg:P) (const_int scale)) */
  else if (GET_CODE (x) == MULT
	   && GET_MODE (x) == Pmode
	   && GET_MODE (XEXP (x, 0)) == Pmode
	   && CONST_INT_P (XEXP (x, 1)))
    {
      index = XEXP (x, 0);
      shift = exact_log2 (INTVAL (XEXP (x, 1)));
    }
  /* (ashift:P (reg:P) (const_int shift)) */
  else if (GET_CODE (x) == ASHIFT
	   && GET_MODE (x) == Pmode
	   && GET_MODE (XEXP (x, 0)) == Pmode
	   && CONST_INT_P (XEXP (x, 1)))
    {
      index = XEXP (x, 0);
      shift = INTVAL (XEXP (x, 1));
    }
  else
    return false;

  if (shift != 0 && !IN_RANGE (shift, 1, 3))
    return false;

  if (!strict_p
      && GET_CODE (index) == SUBREG
      && contains_reg_of_mode[GENERAL_REGS][GET_MODE (SUBREG_REG (index))])
    index = SUBREG_REG (index);

  if (riscv_valid_base_register_p (index, mode, strict_p))
    {
      info->type = type;
      info->offset = index;
      info->shift = shift;
      return true;
    }

  return false;
}

/* Return true if address is a valid modify.  If it is, fill in INFO
   appropriately.  STRICT_P is true if REG_OK_STRICT is in effect.  */

bool
riscv_classify_address_modify (struct riscv_address_info *info, rtx x,
      machine_mode mode, bool strict_p)
{

#define AM_IMM(BIT) (1LL << (5 + (BIT)))
#define AM_OFFSET(VALUE, SHIFT) (\
  ((unsigned HOST_WIDE_INT) (VALUE) + AM_IMM (SHIFT)/2 < AM_IMM (SHIFT)) \
  && !((unsigned HOST_WIDE_INT) (VALUE) & ((1 << (SHIFT)) - 1)) \
  ? (SHIFT) + 1 \
  : 0)

  if (!TARGET_XTHEAD_LDI)
    return false;

  if (!(INTEGRAL_MODE_P (mode) && GET_MODE_SIZE (mode).to_constant () <= 8))
    return false;

  if (!TARGET_64BIT && mode == DImode)
    return false;

  if (GET_CODE (x) != POST_MODIFY
      && GET_CODE (x) != PRE_MODIFY)
    return false;

  info->type = ADDRESS_REG_WB;
  info->reg = XEXP (x, 0);

  if (GET_CODE (XEXP (x, 1)) == PLUS
      && CONST_INT_P (XEXP (XEXP (x, 1), 1))
      && rtx_equal_p (XEXP (XEXP (x, 1), 0), info->reg)
      && riscv_valid_base_register_p (info->reg, mode, strict_p))
    {
      info->offset = XEXP (XEXP (x, 1), 1);
      int shift = AM_OFFSET (INTVAL (info->offset), 0);
      if (!shift)
	shift = AM_OFFSET (INTVAL (info->offset), 1);
      if (!shift)
	shift = AM_OFFSET (INTVAL (info->offset), 2);
      if (!shift)
	shift = AM_OFFSET (INTVAL (info->offset), 3);
      if (shift)
	{
	  info->shift = shift - 1;
	  return true;
	}
    }

  return false;
}

/* Return TRUE if X is a legitimate address modify.  */

bool
riscv_legitimize_address_modify_p (rtx x, machine_mode mode, bool post)
{
  struct riscv_address_info addr;
  return riscv_classify_address_modify (&addr, x, mode, false)
	 && (!post || GET_CODE (x) == POST_MODIFY);
}

/* Return the LDIB/LDIA and STIB/STIA instructions.  Assume
   that X is MEM operand.  */

const char *
riscv_output_move_modify (rtx x, machine_mode mode, bool ldi)
{
  static char buf[128] = {0};

  int index = exact_log2 (GET_MODE_SIZE (mode).to_constant ());
  if (!IN_RANGE (index, 0, 3))
    return NULL;

  if (!riscv_legitimize_address_modify_p (x, mode, false))
    return NULL;

  bool post = riscv_legitimize_address_modify_p (x, mode, true);

  const char *const insn[][4] =
  {
    {
      "sbi%s\t%%z1,%%0",
      "shi%s\t%%z1,%%0",
      "swi%s\t%%z1,%%0",
      "sdi%s\t%%z1,%%0"
    },
    {
      "lbui%s\t%%0,%%1",
      "lhui%s\t%%0,%%1",
      "lwi%s\t%%0,%%1",
      "ldi%s\t%%0,%%1"
    }
  };

  snprintf (buf, sizeof (buf), insn[ldi][index], post ? "a" : "b");
  return buf;
}

/* Return TRUE if X is a legitimate address index.  */

bool
riscv_legitimize_address_index_p (rtx x, machine_mode mode, bool uindex)
{
  struct riscv_address_info addr;
  rtx op0, op1;

  if (GET_CODE (x) != PLUS)
    return false;

  op0 = XEXP (x, 0);
  op1 = XEXP (x, 1);

  return ((riscv_valid_base_register_p (op0, mode, false)
	   && riscv_classify_address_index (&addr, op1, mode, false))
	  || (riscv_valid_base_register_p (op1, mode, false)
	      && riscv_classify_address_index (&addr, op0, mode, false)))
	 && (!uindex || addr.type == ADDRESS_REG_UREG);
}

/* Return the LDR or STR instructions.  Assume
   that X is MEM operand.  */

const char *
riscv_output_move_index (rtx x, machine_mode mode, bool ldr)
{
  static char buf[128] = {0};

  int index = exact_log2 (GET_MODE_SIZE (mode).to_constant ());
  if (!IN_RANGE (index, 0, 3))
    return NULL;

  if (!riscv_legitimize_address_index_p (x, mode, false))
    return NULL;

  bool uindex = riscv_legitimize_address_index_p (x, mode, true);

  const char *const insn[][4] =
  {
    {
      "s%srb\t%%z1,%%0",
      "s%srh\t%%z1,%%0",
      "s%srw\t%%z1,%%0",
      "s%srd\t%%z1,%%0"
    },
    {
      "l%srbu\t%%0,%%1",
      "l%srhu\t%%0,%%1",
      "l%srw\t%%0,%%1",
      "l%srd\t%%0,%%1"
    }
  };

  snprintf (buf, sizeof (buf), insn[ldr][index], uindex ? "u" : "");

  return buf;
}

const char *
riscv_output_move_index_float (rtx x, machine_mode mode, bool ldr)
{
  static char buf[128] = {0};

  int index = exact_log2 (GET_MODE_SIZE (mode).to_constant ());
  if (!IN_RANGE (index, 0, 3))
    return NULL;

  if (!riscv_legitimize_address_index_p (x, mode, false))
    return NULL;

  bool uindex = riscv_legitimize_address_index_p (x, mode, true);

  const char *const insn[][4] =
  {
    {
      "fs%srb\t%%z1,%%0",
      "fs%srh\t%%z1,%%0",
      "fs%srw\t%%z1,%%0",
      "fs%srd\t%%z1,%%0"
    },
    {
      "fl%srb\t%%0,%%1",
      "fl%srh\t%%0,%%1",
      "fl%srw\t%%0,%%1",
      "fl%srd\t%%0,%%1"
    }
  };

  snprintf (buf, sizeof (buf), insn[ldr][index], uindex ? "u" : "");

  return buf;
}

/* Return true if address is a valid pair.  If it is, fill in INFO
   appropriately.  STRICT_P is true if REG_OK_STRICT is in effect.  */

static bool
riscv_classify_address_pair (struct riscv_address_info *info, rtx x,
      machine_mode mode, bool strict_p, bool ldd ATTRIBUTE_UNUSED)
{

#define AP_IMM (1LL << 7)
#define AP_OFFSET(VALUE) \
  ((unsigned HOST_WIDE_INT) (VALUE) + AP_IMM/2 < AP_IMM)

  if (GET_CODE (x) == PLUS)
    {
      info->reg = XEXP (x, 0);
      info->offset = XEXP (x, 1);
      if (INTVAL (info->offset) % (2 * GET_MODE_SIZE (mode).to_constant ()) != 0)
	return false;
      return (riscv_valid_base_register_p (info->reg, mode, strict_p)
	      && CONST_INT_P (info->offset)
	      && AP_OFFSET (INTVAL (info->offset)));
    }

  if (GET_CODE (x) == REG
      || GET_CODE (x) == SUBREG)
    {
      info->reg = x;
      info->offset = const0_rtx;
      return riscv_valid_base_register_p (info->reg, mode, strict_p);
    }

  return false;
}

/* Return TRUE if X is a legitimate address pair.  */

bool
riscv_legitimize_address_pair_p (rtx x, machine_mode mode, bool ldd)
{
  struct riscv_address_info addr;
  return riscv_classify_address_pair (&addr, x, mode, false, ldd);
}

int
riscv_expand_movcc (rtx *operands)
{
  enum rtx_code code = GET_CODE (operands[1]);
  machine_mode cmp0_mode = GET_MODE (XEXP (operands[1], 0));
  rtx cmp_op0 = XEXP (operands[1], 0);
  rtx cmp_op1 = XEXP (operands[1], 1);
  rtx tmp;
  bool reverse = false;

  if (GET_MODE_CLASS (cmp0_mode) != MODE_INT)
    return -1;
  else
    {
      riscv_extend_comparands (code, &cmp_op0, &cmp_op1);
      cmp_op0 = force_reg (word_mode, cmp_op0);

      if (code == EQ || code == NE)
	{
	  if (cmp_op1 == const0_rtx)
	    {
	      XEXP (operands[1], 0) = cmp_op0;
	      /* If the operands[1] rtx is already (eq X 0) or (ne X 0),
		 use the original rtx.  */
	      return 1;
	    }
	  tmp = riscv_zero_if_equal (cmp_op0, cmp_op1);
	  reverse = (code == EQ) ? true : false;
	}
      else
	{
	  tmp = gen_reg_rtx (TARGET_64BIT ? DImode : SImode);
	  riscv_emit_int_order_test (code, &reverse, tmp, cmp_op0, cmp_op1);
	}

      operands[1] = gen_rtx_fmt_ee (reverse ? EQ : NE,
				    VOIDmode, tmp, const0_rtx);
    }

  return 1;
}

/* Return TRUE if X is a legitimate address reg + offset.  */

bool
riscv_legitimize_address_classic_p (rtx x, machine_mode mode)
{
  struct riscv_address_info addr;
  return riscv_classify_address (&addr, x, mode, false)
	 && addr.type == ADDRESS_REG;
}

/* Return true if address is a valid vector.  If it is, fill in INFO
   appropriately.  STRICT_P is true if REG_OK_STRICT is in effect.  */

bool
riscv_classify_address_vector (struct riscv_address_info *info, rtx x,
  machine_mode mode, bool strict_p)
{
  switch (GET_CODE (x))
    {
    case REG:
    case SUBREG:
      info->type = ADDRESS_REG;
      info->reg = x;
      info->offset = const0_rtx;
      return riscv_valid_base_register_p (x, mode, strict_p);

    case PLUS:
      info->type = ADDRESS_REG;
      info->reg = XEXP (x, 0);
      info->offset = XEXP (x, 1);
      return INTVAL (info->offset) == 0;

    default:
      return false;
    }
}

bool
riscv_save_reg_ipush_p (int regno)
{
  int regs[] = {
    RETURN_ADDR_REGNUM,
    T0_REGNUM, T1_REGNUM, T2_REGNUM,
    A0_REGNUM, A1_REGNUM, A2_REGNUM, A3_REGNUM,
    A4_REGNUM, A5_REGNUM, A6_REGNUM, A7_REGNUM,
    T3_REGNUM, T4_REGNUM, T5_REGNUM, T6_REGNUM,
  };

  for (unsigned int i = 0; i < ARRAY_SIZE (regs); i++)
    {
      if (GP_REG_P (regno) && regno == regs[i] + GP_REG_FIRST)
	return true;
    }
  return false;
}

rtx
riscv_adjust_ipush_cfi_prologue ()
{
  rtx dwarf = NULL_RTX;
  rtx adjust_sp_rtx, reg, mem, insn;
  int saved_size = cfun->machine->frame.save_ipush_adjustment;
  int offset = saved_size - UNITS_PER_WORD * 2;

  for (int regno = GP_REG_FIRST; regno <= GP_REG_LAST; regno++)
    if (BITSET_P (cfun->machine->frame.imask, regno - GP_REG_FIRST))
      {
	offset -= UNITS_PER_WORD;
	reg = gen_rtx_REG (SImode, regno);
	mem = gen_frame_mem (SImode, plus_constant (Pmode,
						    stack_pointer_rtx,
						    offset));

	insn = gen_rtx_SET (mem, reg);
	dwarf = alloc_reg_note (REG_CFA_OFFSET, insn, dwarf);
      }

  /* Debug info for adjust sp.  */
  adjust_sp_rtx = gen_rtx_SET (stack_pointer_rtx,
	gen_rtx_PLUS (GET_MODE(stack_pointer_rtx), stack_pointer_rtx, GEN_INT (-saved_size)));
  dwarf = alloc_reg_note (REG_CFA_ADJUST_CFA, adjust_sp_rtx, dwarf);

  return dwarf;
}

/* Implement DSP extension.  */

bool
riscv_dsp_mode (machine_mode mode)
{
  if (TARGET_XTHEAD_ZPN || TARGET_XTHEAD_ZPSFOPERAND)
    {
      switch (mode)
      {
	case E_V2HImode:
	case E_V4QImode:
	case E_V2SImode:
	case E_V4HImode:
	case E_V8QImode:
	  return true;
	default:
	  break;
      }
    }
  return false;
}

machine_mode
riscv_dsp_preferred_mode (scalar_mode mode)
{
  if (TARGET_XTHEAD_ZPN)
    {
      if (TARGET_64BIT)
	switch (mode)
	  {
	  case E_SImode:
	    return V2SImode;
	  case E_HImode:
	    return V4HImode;
	  case E_QImode:
	    return V8QImode;
	  default:
	    break;
	  }
      else
	switch (mode)
	  {
	  case E_HImode:
	    return V2HImode;
	  case E_QImode:
	    return V4QImode;
	  default:
	    break;
	  }
    }

  if (TARGET_XTHEAD_ZPSFOPERAND)
    if (mode == E_HImode) return E_V2HImode;

  return VOIDmode;
}

/* Implement TARGET_IV_GEN_LOAD_INDEX.
   Don't generate index for induction variables for the loop when
   4k-optimize is enable and loop size is larger than 4k.  */

static bool
riscv_iv_gen_load_index (HOST_WIDE_INT loop_niter,
			 machine_mode mem_mode)
{
  HOST_WIDE_INT mem_size = GET_MODE_SIZE (mem_mode).to_constant ();

  if (!riscv_4k_optimize)
    return true;

  if (loop_niter * mem_size > 4048)
    return false;
  return true;
}


/* Nonzero if MODE is a matrix mode. */

bool
riscv_matrix_mode(machine_mode mode)
{
  switch (mode)
  {
  case E_M64QImode:
  case E_M32HImode:
  case E_M16SImode:
  case E_M8DImode:
  case E_M4TImode:
  case E_M32HFmode:
  case E_M16SFmode:
  case E_M8DFmode:
  case E_M4TFmode:
  case E_M2x32HFmode:
  case E_M2x16SFmode:
  case E_M2x8DFmode:
  case E_M2x4TFmode:
    return true;
  default:
    break;
  }

  return false;
}

/* Nonzero if MODE is a matrix mode with 2 nregs. */

bool
riscv_matrix_x2_mode(machine_mode mode)
{
  switch (mode)
  {
  case E_M2x32HFmode:
  case E_M2x16SFmode:
  case E_M2x8DFmode:
  case E_M2x4TFmode:
    return true;
  default:
    break;
  }
  return false;
}

/* Implement TARGET_REGISTER_REJECT_INIT_P.  */

static bool
riscv_register_reject_init_p (rtx reg)
{
  if (TARGET_VECTOR && VECTOR_MODE_P (GET_MODE (reg)))
    return true;

  if (TARGET_MATRIX && riscv_matrix_mode (GET_MODE (reg)))
    return true;

  return false;
}

static bool
function_has_vector_ops_p (tree fntype)
{
  /* Check function's return type.  */
  if(builtin_vector_type_p (TREE_TYPE (fntype)))
    return true;

  /* Check function args' type.  */
  tree argtype;
  function_args_iterator iter;
  FOREACH_FUNCTION_ARGS (fntype, argtype, iter)
    if(builtin_vector_type_p (argtype))
      return true;

  return false;
}

/* Implement TARGET_EXPAND_TO_RTL_HOOK.  */

static void
riscv_expand_to_rtl_hook (void)
{
  /* If any of function's args or return type is vector type, we set
     has_vector_ops_p to true and reinit regs.  */
  if (function_has_vector_ops_p (TREE_TYPE (cfun->decl)))
    {
      cfun->machine->has_vector_ops_p = true;
      reinit_regs ();
    }
}

#include "backend.h"
#include "pass_manager.h"
#include "context.h"
#include "cfgrtl.h"
#include "cfghooks.h"
#include "cfganal.h"

static void
replace_insn_regnum(rtx *pattern_ref,
                    rtx old_reg,
                    unsigned int new_regnum)
{
  const char *fmt;
  int i, j;
  enum rtx_code code = GET_CODE(*pattern_ref);

  fmt = GET_RTX_FORMAT(code);
  for (i = GET_RTX_LENGTH(code) - 1; i >= 0; i--)
    if (fmt[i] == 'e')
      replace_insn_regnum(&XEXP(*pattern_ref, i), old_reg, new_regnum);
    else if (fmt[i] == 'E')
      for (j = 0; j < XVECLEN(*pattern_ref, i); j++)
        replace_insn_regnum(&XVECEXP(*pattern_ref, i, j), old_reg,
                            new_regnum);
    else if (fmt[i] == 'r')
    {
      if (REGNO(*pattern_ref) == REGNO(old_reg))
        *pattern_ref = gen_rtx_REG(GET_MODE(*pattern_ref), new_regnum);
    }
}

static void
do_ud_du_analysis(void)
{
  basic_block bb;
  rtx_insn *insn;
  FOR_EACH_BB_FN (bb, cfun)
    FOR_BB_INSNS (bb, insn)
      set_block_for_insn (insn, bb);

  df_set_flags (DF_RD_PRUNE_DEAD_DEFS);
  df_chain_add_problem (DF_UD_CHAIN | DF_DU_CHAIN);
  df_insn_rescan_all ();
  df_analyze ();
  df_set_flags (DF_DEFER_INSN_RESCAN);
}

rtx riscv_di_high_part_subreg(rtx reg)
{
  poly_uint64 high_part_offset = subreg_highpart_offset (SImode, DImode);

  return simplify_gen_subreg (
	   SImode, reg,
	   DImode, high_part_offset);
}

rtx riscv_di_low_part_subreg(rtx reg)
{
  poly_uint64 low_part_offset = subreg_lowpart_offset (SImode, DImode);

  return simplify_gen_subreg (
	   SImode, reg,
	   DImode, low_part_offset);
}

static void
riscv_split_shiftrtdi3 (rtx dst, rtx src, rtx shiftamount, bool logic_shift_p)
{
  rtx src_high_part;
  rtx dst_high_part, dst_low_part;

  dst_high_part = riscv_di_high_part_subreg (dst);
  src_high_part = riscv_di_high_part_subreg (src);
  dst_low_part = riscv_di_low_part_subreg (dst);

  if (INTVAL (shiftamount) < 32)
    {
      if (logic_shift_p)
	{
	  emit_insn (gen_riscv_uwexti_si (dst_low_part, src,
					  shiftamount));
	  emit_insn (gen_lshrsi3 (dst_high_part, src_high_part,
				  shiftamount));
	}
      else
	{
	  emit_insn (gen_riscv_wexti_si (dst_low_part, src,
					 shiftamount));
	  emit_insn (gen_ashrsi3 (dst_high_part, src_high_part,
				  shiftamount));
	}
    }
  else
    {
      rtx new_shift_amout = gen_int_mode(INTVAL (shiftamount) - 32, SImode);

      if (logic_shift_p)
	{
	  emit_insn (gen_lshrsi3 (dst_low_part, src_high_part,
				  new_shift_amout));
	  emit_move_insn (dst_high_part, const0_rtx);
	}
      else
	{
	  emit_insn (gen_ashrsi3 (dst_low_part, src_high_part,
				  new_shift_amout));
	  emit_insn (gen_ashrsi3 (dst_high_part, src_high_part,
				  GEN_INT (31)));
	}
    }
}

void
riscv_split_ashiftdi3 (rtx dst, rtx src, rtx shiftamount)
{
  rtx src_low_part;
  rtx dst_high_part, dst_low_part;

  dst_high_part = riscv_di_high_part_subreg (dst);
  dst_low_part = riscv_di_low_part_subreg (dst);

  src_low_part = riscv_di_low_part_subreg (src);

  if (INTVAL (shiftamount) < 32)
    {
      rtx ext_start;
      ext_start = gen_int_mode(32 - INTVAL (shiftamount), SImode);
      emit_insn (gen_riscv_wexti_si (dst_high_part, src, ext_start));
      emit_insn (gen_ashlsi3 (dst_low_part, src_low_part, shiftamount));
    }
  else
    {
      rtx new_shift_amout = gen_int_mode(INTVAL (shiftamount) - 32, SImode);
      emit_insn (gen_ashlsi3 (dst_high_part, src_low_part,
			      new_shift_amout));
      emit_move_insn (dst_low_part, GEN_INT (0));
    }
}

void
riscv_split_ashiftrtdi3 (rtx dst, rtx src, rtx shiftamount)
{
  riscv_split_shiftrtdi3 (dst, src, shiftamount, false);
}

void
riscv_split_lshiftrtdi3 (rtx dst, rtx src, rtx shiftamount)
{
  riscv_split_shiftrtdi3 (dst, src, shiftamount, true);
}

/* Depth first traverse the preds of CUR_BB, if a block set REG
   is found, return TRUE. VISITED_BB record the blocks that has
   been traversed.  */

static bool
reg_set_by_preds (basic_block cur_bb, bitmap visited_bb, const_rtx reg)
{
  edge_iterator ei;
  edge e;

  FOR_EACH_EDGE (e, ei, cur_bb->preds)
    {
      basic_block bb = e->src;
      class df_lr_bb_info *bb_info = df_lr_get_bb_info (bb->index);

      if (bitmap_bit_p (visited_bb, bb->index))
	continue;

      if (!bb_info)
	return true;

      if (bitmap_bit_p (&bb_info->def, REGNO (reg)))
	return true;

      bitmap_set_bit (visited_bb, bb->index);
      if (reg_set_by_preds (bb, visited_bb, reg))
	return true;
    }
  return false;
}

/* Check if the RGE is set between FROM and TO, return TRUE if it is set.  */

static bool
reg_set_trace_p (const_rtx reg, rtx_insn *from, rtx_insn *to)
{
  bool set = false;
  rtx_insn *insn;
  basic_block from_bb = BLOCK_FOR_INSN (from);
  basic_block to_bb = BLOCK_FOR_INSN (to);
  bitmap visited_blocks = BITMAP_ALLOC (NULL);

  /* The x0 can never be set.  */
  if (REGNO (reg) == GP_REG_FIRST)
    return false;

  if (from == to)
    return false;

  /* Case 1: FROM and TO are in the same block, and FROM is before TO.  */
  if (from_bb == to_bb && DF_INSN_LUID (from) < DF_INSN_LUID (to))
    {
      for (insn = NEXT_INSN (from); insn != to; insn = NEXT_INSN (insn))
	if (INSN_P (insn) && reg_set_p (reg, insn))
	  return true;
      return false;
    }

  /* Case 2: FROM and TO are in different blocks
	     or they are in same block and FROM follows TO. */

  /* Check if the REG is set from FROM to the end of the block.  */
  for (insn = NEXT_INSN (from); insn != NEXT_INSN (BB_END (from_bb));
       insn = NEXT_INSN (insn))
    if (INSN_P (insn) && reg_set_p (reg, insn))
      return true;

  /* Check if the REG is set from the start of block to TO.  */
  for (insn = BB_HEAD (to_bb); insn != to; insn = NEXT_INSN (insn))
    if (INSN_P (insn) && reg_set_p (reg, insn))
      return true;

  /* Check if the REG is set in every path from FROM_BB to TO_BB.  */
  bitmap_set_bit (visited_blocks, from_bb->index);
  set = reg_set_by_preds (to_bb, visited_blocks, reg);
  BITMAP_FREE (visited_blocks);
  return set;
}

/* If REG is defined in INSN and used by other insns, return TRUE.
   Else return FALSE.  */

static bool
reg_used_following (rtx_insn *insn, rtx reg, bool samebb = true)
{
  df_ref def;

  /* Ignore the use of x0.  */
  if (REGNO (reg) == GP_REG_FIRST)
    return false;

  FOR_EACH_INSN_DEF (def, insn)
    {
      struct df_link *reg_use;

      if (DF_REF_REGNO (def) == REGNO (reg))
	for (reg_use = DF_REF_CHAIN (def); reg_use; reg_use = reg_use->next)
	  {
	    if (!samebb && DF_REF_BB(reg_use->ref) == BLOCK_FOR_INSN (insn))
	      continue;
	    if (DF_REF_IS_ARTIFICIAL (reg_use->ref))
	      continue;
	    else
	      return true;
	  }
    }
  return false;
}

/* Scheduling pass is now finished.  */
bool sched_finish_after_reload = false;
bool sched_finish_executed = false;

static void
riscv_sched_finish_global (FILE *dump ATTRIBUTE_UNUSED,
			   int sched_verbose ATTRIBUTE_UNUSED)
{
  if (reload_completed)
    {
      if (optimize > 0 && flag_schedule_insns_after_reload)
	{
	  if (!sched_fusion)
	    sched_finish_after_reload = true;
	}
      else
	sched_finish_after_reload = true;
    }
  sched_finish_executed = true;
}

static void
riscv_asm_function_prologue (FILE *)
{
  sched_finish_after_reload = false;
  sched_finish_executed = false;
}

/* Implement TARGET_FWPROP_LEGITIMIZE_SET.
   Try to legitimize DEF_SET to fit forward propagration.  */

static void
riscv_fwprop_legitimize_set (rtx *def_set)
{
  rtx set = *def_set;
  if (riscv_fwprop_vmov_v0p7 &&
      GET_CODE (SET_SRC (set)) == UNSPEC && XINT (SET_SRC (set), 1) == UNSPEC_VWLDST)
    {
      *def_set = gen_rtx_SET (SET_DEST (set), XVECEXP (SET_SRC (set), 0, 0));
    }
}

/* Return nozero if OPERAND is a nnan const double. */

static int
riscv_is_nnan_rtx (rtx operand)
{
  if (operand && CONST_DOUBLE_P (operand)
      && !REAL_VALUE_ISNAN (*CONST_DOUBLE_REAL_VALUE (operand)))
    return 1;
  return 0;
}

/* Return true if find the insn with nnan REG_EQUAL or REG_EQUIV note form MEM or
   CONST_DOUBLE 0.If it is a register, try to look up. When Nan or not found, it
   means that instruction 'fclass' deletion cannot be performed, and only false is
   returned. */

static bool
riscv_find_nnan_eq_note (rtx_insn *insn, unsigned int regno)
{
  rtx insn_set = single_set (insn);
  if (!(insn_set && REG_P (SET_DEST (insn_set))
      && REGNO (SET_DEST (insn_set)) == regno))
    return false;

  rtx src = SET_SRC  (insn_set);
  if (REG_P (src))
    {
      df_ref use;
      rtx_insn *def_insn = NULL;

      unsigned int use_regno = REGNO (src);
      FOR_EACH_INSN_USE (use, insn)
	{
	  unsigned int def_regno = DF_REF_REGNO (use);
	  if (use_regno != def_regno)
	    continue;

	  struct df_link *temp = DF_REF_CHAIN (use);
	  for (; temp; temp = temp->next)
	    {
	      if (!DF_REF_INSN_INFO (temp->ref))
		continue;

	      def_insn = DF_REF_INSN (temp->ref);

	      if (!riscv_find_nnan_eq_note (def_insn, use_regno))
		return false;
	    }
	}

      return def_insn ? true : false;
    }

  if (MEM_P (src))
    {
      rtx link = find_reg_equal_equiv_note (insn);

      if (link && riscv_is_nnan_rtx (XEXP (link, 0)))
	return true;
    }

  if (const_0_operand (src, GET_MODE (src)))
    return true;

  return false;
}

/* Try to set non Nan constant note for register, which makes it possible
   to optimize the register instructions later. */

void
riscv_optimize_quiet_comparison ()
{
  rtx_insn *insn;
  rtx src;
  df_ref use;

  do_ud_du_analysis ();

  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (!INSN_P (insn) || !single_set (insn))
	continue;

      src = SET_SRC (single_set (insn));

      if (GET_CODE (src) != UNSPEC || !(XINT (src, 1) == UNSPEC_FLT_QUIET || XINT (src, 1) == UNSPEC_FLE_QUIET))
	continue;

      unsigned int nnan_regno = 0;
      FOR_EACH_INSN_USE (use, insn)
	{
	  struct df_link *temp = DF_REF_CHAIN (use);
	  for (; temp; temp = temp->next)
	    {
	      if (!DF_REF_INSN_INFO (temp->ref))
		continue;

	      rtx_insn *def_insn = DF_REF_INSN (temp->ref);

	      if (!riscv_find_nnan_eq_note (def_insn, DF_REF_REGNO (use)))
		{
		  nnan_regno = 0;
		  break;
		}else
		  nnan_regno = DF_REF_REGNO (use);
	    }
	  if (nnan_regno)
	    {
	      /* TODO: clobber unused. */
	      rtx new_pat = copy_rtx (PATTERN (insn));
	      rtx new_set = XVECEXP (new_pat, 0, 0);

	      rtvec temp = rtvec_alloc (3);
	      RTVEC_ELT (temp, 0) = XVECEXP (src, 0, 0);
	      RTVEC_ELT (temp, 1) = XVECEXP (src, 0, 1);
	      if (REGNO (XVECEXP (src, 0, 0)) == nnan_regno)
		RTVEC_ELT (temp, 2) = const0_rtx;
	      else
		RTVEC_ELT (temp, 2) = const1_rtx;

	      XVEC (XEXP (new_set, 1), 0) = temp;
	      rtx_insn * new_insn = emit_insn_before (new_pat, insn);
	      if (REG_NOTES (insn))
		REG_NOTES (new_insn) = copy_rtx (REG_NOTES (insn));

	      extract_insn (new_insn);
	      remove_insn (insn);
	      break;
	    }
	}
    }

  df_finish_pass (false);
}


#include "riscv-thead-dsext.c"
#include "riscv-thead-dvsetvl.c"
#include "riscv-thead-dvsetvl-v0p7.c"
#include "riscv-thead-dread-vlenb.c"

#undef TARGET_IV_GEN_LOAD_INDEX
#define TARGET_IV_GEN_LOAD_INDEX riscv_iv_gen_load_index

#undef TARGET_REGISTER_REJECT_INIT_P
#define TARGET_REGISTER_REJECT_INIT_P riscv_register_reject_init_p

#undef TARGET_EXPAND_TO_RTL_HOOK
#define TARGET_EXPAND_TO_RTL_HOOK riscv_expand_to_rtl_hook

#undef TARGET_SCHED_FINISH_GLOBAL
#define TARGET_SCHED_FINISH_GLOBAL riscv_sched_finish_global

#undef  TARGET_ASM_FUNCTION_PROLOGUE
#define TARGET_ASM_FUNCTION_PROLOGUE riscv_asm_function_prologue

#undef TARGET_FWPROP_LEGITIMIZE_SET
#define TARGET_FWPROP_LEGITIMIZE_SET riscv_fwprop_legitimize_set

