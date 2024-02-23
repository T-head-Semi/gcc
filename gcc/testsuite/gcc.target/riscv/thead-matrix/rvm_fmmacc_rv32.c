/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

mfloat16_t test_mfloat16_fmmacc (mfloat16_t dest, mfloat16_t src1, mfloat16x2_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_fmmacc (dest, src1, src2, row1, row2, col);
}

mfloat32_t test_mfloat32_fmmacc (mfloat32_t dest, mfloat32_t src1, mfloat32_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_fmmacc (dest, src1, src2, row1, row2, col);
}

mfloat64x2_t test_mfloat64x2_fmmacc (mfloat64x2_t dest, mfloat64_t src1, mfloat64_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_fmmacc (dest, src1, src2, row1, row2, col);
}

/* { dg-final { scan-assembler-times "fmmacc\\.h\t" 1 } } */
/* { dg-final { scan-assembler-times "fmmacc\\.s\t" 1 } } */
/* { dg-final { scan-assembler-times "fmmacc\\.d\t" 1 } } */