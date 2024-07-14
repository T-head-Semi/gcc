
#ifndef GCC_RISCV_THEAD_H
#define GCC_RISCV_THEAD_H


#define TARGET_XTHEAD_LDR ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_ldr)
#define TARGET_XTHEAD_LDI ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_ldi)

#define TARGET_XTHEAD_EXT ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_ext)
#define TARGET_XTHEAD_MULA ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_mula)
#define TARGET_XTHEAD_ADDSL ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_addsl)
#define TARGET_XTHEAD_ADDSLUW (TARGET_XTHEAD_C && riscv_insn_addsluw && TARGET_64BIT)
#define TARGET_XTHEAD_REV ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_rev)
#define TARGET_XTHEAD_REVW (TARGET_XTHEAD_C && riscv_insn_revw && TARGET_64BIT)
#define TARGET_XTHEAD_SRRI ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_srri)
#define TARGET_XTHEAD_SRRIW (TARGET_XTHEAD_C && riscv_insn_srriw && TARGET_64BIT)
#define TARGET_XTHEAD_CONDMV ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_condmv)
#define TARGET_XTHEAD_LDD (TARGET_XTHEAD_C && riscv_insn_ldd)
#define TARGET_XTHEAD_IPUSH (TARGET_XTHEAD_E && riscv_insn_ipush)
#define TARGET_XTHEAD_FMVHW32 ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_insn_fmvhw32 && !TARGET_64BIT)

#define TARGET_XTHEAD_XOR_COMBINE ((TARGET_XTHEAD_C || TARGET_XTHEAD_E) && riscv_xor_combine)
#define TARGET_XTHEAD_COMBINE_COPY (TARGET_XTHEAD ? riscv_combine_copy : true)
#define TARGET_XTHEAD_DUP_LOOP_HEADER (TARGET_XTHEAD ? riscv_dup_loop_header : false)
#define TARGET_XTHEAD_UNHOT_INLINE (TARGET_XTHEAD ? riscv_ipa_unhot_inline : false)
#define TARGET_XTHEAD_INVARIANT_AS_CONSTANT (TARGET_XTHEAD ? riscv_invariant_as_constant : false)
#define TARGET_XTHEAD_RNREG_DELETE_NOOP_MOVE (TARGET_XTHEAD ? riscv_rnreg_delete_noop_move : false)
#define TARGET_XTHEAD_COMBINE_CSE (TARGET_XTHEAD ? riscv_combine_cse : false)

#define TARGET_XTHEAD_SIGNEDNESS_COMPARISON_IV (TARGET_XTHEAD ? riscv_signedness_comparison_iv : false)
#define TARGET_XTHEAD_IV_ADJUST_ADDR_COST (TARGET_XTHEAD ? riscv_iv_adjust_addr_cost : true)
#define TARGET_XTHEAD_EXPAND_SPLIT_IMM (TARGET_XTHEAD ? riscv_expand_split_imm : true)
#define TARGET_XTHEAD_THREAD1 (TARGET_XTHEAD ? riscv_thread_jumps1 : true)

#define TARGET_XTHEAD_FLDR (TARGET_HARD_FLOAT && TARGET_XTHEAD_C && riscv_insn_fldr)
#define TARGET_XTHEAD_FP16 (TARGET_HARD_FLOAT && TARGET_XTHEAD_C && riscv_insn_fp16)
#define TARGET_XTHEAD_FCLASS (TARGET_HARD_FLOAT && TARGET_XTHEAD && riscv_insn_fclass)

#define TARGET_XTHEAD_DREAD_VLENB (TARGET_VECTOR && riscv_insn_dread_vlenb \
				   && !target_subset_version_p ("v", 0, 7) \
				   && cfun->machine->has_vector_ops_p      \
				   && optimize > 0)

#define HAVE_POST_MODIFY_DISP TARGET_XTHEAD_LDI
#define HAVE_PRE_MODIFY_DISP  TARGET_XTHEAD_LDI

#define TARGET_XTHEAD_VSPLIT (reload_completed && (!sched_finish_executed || sched_finish_after_reload))

#define RISCV_TUNE_STRING_DEFAULT	\
  (TARGET_XTHEAD_C ? "c910" :		\
   TARGET_XTHEAD_E ? "e906" :		\
   TARGET_XTHEAD_SE ? "e902" :		\
   "rocket")

#define RISCV_TUNE_C908_P (riscv_microarchitecture == c908)

/* True if VALUE is an unsigned 16-bit number.  */
#define SMALL_OPERAND_UNSIGNED16(VALUE) \
  (((VALUE) & ~(unsigned HOST_WIDE_INT) 0xffff) == 0)

#define TARGET_XTHEAD_INTERRUPT_HANDLER_P  \
  (cfun->machine->interrupt_handler_p \
   && (cfun->machine->interrupt_flags.interrupt_mode == USER_MODE \
       || cfun->machine->interrupt_flags.interrupt_mode == MACHINE_MODE))

#define TARGET_XTHEAD_INTERRUPT_NESTING \
  (TARGET_XTHEAD_INTERRUPT_HANDLER_P \
   && (TARGET_XTHEAD_IPUSH \
       || cfun->machine->interrupt_flags.thead_clic_preemptible_p))

#define TARGET_XTHEAD_INTERRUPT_NESTING_IPUSH \
  (TARGET_XTHEAD_INTERRUPT_NESTING \
   && TARGET_XTHEAD_IPUSH)

#define TARGET_XTHEAD_INTERRUPT_NESTING_CSR \
  (TARGET_XTHEAD_INTERRUPT_NESTING \
   && !(TARGET_XTHEAD_INTERRUPT_NESTING_IPUSH \
	&& riscv_insert_ipush_p ()))

#define TARGET_XTHEAD_DSP (TARGET_DSP || TARGET_ZPN)
#define TARGET_XTHEAD_ZPN TARGET_ZPN
#define TARGET_XTHEAD_ZPSFOPERAND TARGET_ZPSFOPERAND
#define TARGET_XTHEAD_ZPRVSFEXTRA TARGET_ZPRVSFEXTRA

#define TARGET_VECTOR_TEMP_REGNUM	(GP_REG_FIRST + 31)
#define TARGET_VECTOR_TEMP(MODE)	gen_rtx_REG (MODE, TARGET_VECTOR_TEMP_REGNUM)
#define TARGET_VECTOR_TEMP2_REGNUM	(GP_REG_FIRST + 30)
#define TARGET_VECTOR_TEMP2(MODE)	gen_rtx_REG (MODE, TARGET_VECTOR_TEMP2_REGNUM)
#define TARGET_MATRIX_TEMP_REGNUM	(GP_REG_FIRST + 29)
#define TARGET_MATRIX_TEMP(MODE)	gen_rtx_REG (MODE, TARGET_MATRIX_TEMP_REGNUM)

#define TARGET_VECTOR_FIXED (TARGET_VECTOR && riscv_rvv_chunks.is_constant ())

#ifdef IN_TARGET_CODE
extern const struct riscv_tune_param xt_c908_tune_info;
extern const struct riscv_tune_param xt_c906v_tune_info;
extern const struct riscv_tune_param xt_c910_tune_info;
extern const struct riscv_tune_param xt_e9_tune_info;

void
riscv_xthead_option_override (const struct riscv_tune_param *tune_param,
			      struct gcc_options *opts,
			      struct gcc_options *opts_set);

bool
riscv_xthead_rtx_costs (rtx x, machine_mode mode, int outer_code,
			int opno ATTRIBUTE_UNUSED, int *total, bool speed);
bool
riscv_classify_address_index (struct riscv_address_info *info, rtx x,
			      machine_mode mode, bool strict_p);
bool
riscv_classify_address_modify (struct riscv_address_info *info, rtx x,
			       machine_mode mode, bool strict_p);

const char *
riscv_output_move_index (rtx x, machine_mode mode, bool ldr);
const char *
riscv_output_move_modify (rtx x, machine_mode mode, bool ldi);
const char *
riscv_output_move_index_float (rtx x, machine_mode mode, bool ldr);

bool
riscv_legitimize_address_index_p (rtx x, machine_mode mode, bool uindex);
bool
riscv_legitimize_address_modify_p (rtx x, machine_mode mode, bool post);
bool
riscv_legitimize_address_pair_p (rtx, machine_mode, bool);
bool
riscv_legitimize_address_classic_p (rtx, machine_mode);

int
riscv_expand_movcc (rtx *operands);

bool
riscv_save_reg_ipush_p (int regno);
bool
riscv_insert_ipush_p (void);
rtx
riscv_adjust_ipush_cfi_prologue ();

/* Implement Vector extension.  */
bool
riscv_classify_address_vector (struct riscv_address_info *info, rtx x,
			       machine_mode mode, bool strict_p);

/* Implement DSP extension.  */
bool riscv_dsp_mode (machine_mode mode);
machine_mode riscv_dsp_preferred_mode (scalar_mode mode);

/* Implement Matrix extension.  */
bool
riscv_matrix_mode (machine_mode mode);
bool
riscv_matrix_x2_mode (machine_mode mode);

void
emit_libcall_insn_for_convert (rtx dest, rtx src, bool unsigned_p = false);

#endif
#endif
