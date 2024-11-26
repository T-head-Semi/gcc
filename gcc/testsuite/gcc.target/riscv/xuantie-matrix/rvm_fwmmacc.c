/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

mfloat32_t test_mfloat32_fwmmacc (mfloat32_t dest, mfloat16_t src1, mfloat16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_fwmmacc (dest, src1, src2, row1, row2, col);
}

mfloat64x2_t test_mfloat64x2_fwmmacc (mfloat64x2_t dest, mfloat32_t src1, mfloat32_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_fwmmacc (dest, src1, src2, row1, row2, col);
}

/* { dg-final { scan-assembler-times "fwmmacc.h\t" 1 } } */
/* { dg-final { scan-assembler-times "fwmmacc.s\t" 1 } } */
