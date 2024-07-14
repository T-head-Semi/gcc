
enum riscv_builtin_imm {
  RISCV_BUILTIN_IMM2 = 2,
  RISCV_BUILTIN_IMM_STRAT = RISCV_BUILTIN_IMM2,
  RISCV_BUILTIN_IMM3,
  RISCV_BUILTIN_IMM4,
  RISCV_BUILTIN_IMM5,
  RISCV_BUILTIN_IMM6,
  RISCV_BUILTIN_IMM_END
};

/* Builtin types for the short immediately number.  */

static tree
riscv_build_imm_type (enum riscv_builtin_imm prec, signop unsigned_p)
{
  tree *ret;

  static tree riscv_const_imm_nodes[(RISCV_BUILTIN_IMM_END
				     - RISCV_BUILTIN_IMM_STRAT) * 2];

  ret = &riscv_const_imm_nodes[(prec - RISCV_BUILTIN_IMM_STRAT)
				   * (unsigned_p + 1)];
  if (*ret == NULL_TREE)
    {
      *ret = unsigned_p ? make_unsigned_type (BITS_PER_UNIT * 4)
			: make_signed_type (BITS_PER_UNIT * 4);

      set_min_and_max_values_for_integral_type (*ret, prec, unsigned_p);
    }

  return *ret;
}

/* DSP types.  */

#define RISCV_ATYPE_V4QI build_vector_type_for_mode (intQI_type_node, V4QImode)
#define RISCV_ATYPE_V8QI build_vector_type_for_mode (intQI_type_node, V8QImode)
#define RISCV_ATYPE_V2HI build_vector_type_for_mode (intHI_type_node, V2HImode)
#define RISCV_ATYPE_V4HI build_vector_type_for_mode (intHI_type_node, V4HImode)
#define RISCV_ATYPE_V2SI build_vector_type_for_mode (intSI_type_node, V2SImode)

#define RISCV_ATYPE_UV4QI build_vector_type_for_mode (unsigned_intQI_type_node, V4QImode)
#define RISCV_ATYPE_UV8QI build_vector_type_for_mode (unsigned_intQI_type_node, V8QImode)
#define RISCV_ATYPE_UV2HI build_vector_type_for_mode (unsigned_intHI_type_node, V2HImode)
#define RISCV_ATYPE_UV4HI build_vector_type_for_mode (unsigned_intHI_type_node, V4HImode)
#define RISCV_ATYPE_UV2SI build_vector_type_for_mode (unsigned_intSI_type_node, V2SImode)

#define RISCV_ATYPE_UIMM6 riscv_build_imm_type (RISCV_BUILTIN_IMM6, UNSIGNED)
#define RISCV_ATYPE_UIMM5 riscv_build_imm_type (RISCV_BUILTIN_IMM5, UNSIGNED)
#define RISCV_ATYPE_UIMM4 riscv_build_imm_type (RISCV_BUILTIN_IMM4, UNSIGNED)
#define RISCV_ATYPE_UIMM3 riscv_build_imm_type (RISCV_BUILTIN_IMM3, UNSIGNED)
#define RISCV_ATYPE_UIMM2 riscv_build_imm_type (RISCV_BUILTIN_IMM2, UNSIGNED)
#define RISCV_ATYPE_CUIMM6 RISCV_ATYPE_UIMM6
#define RISCV_ATYPE_CUIMM5 RISCV_ATYPE_UIMM5
#define RISCV_ATYPE_CUIMM4 RISCV_ATYPE_UIMM4
#define RISCV_ATYPE_CUIMM3 RISCV_ATYPE_UIMM3
#define RISCV_ATYPE_CUIMM2 RISCV_ATYPE_UIMM2
