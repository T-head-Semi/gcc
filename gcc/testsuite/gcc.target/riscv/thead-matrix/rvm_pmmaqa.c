/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

mint32_t test_mint32_pmmaqa (mint32_t dest, mint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_pmmaqa (dest, src1, src2, row1, row2, col);
}

mint32_t test_muint32_pmmaqau (mint32_t dest, muint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_pmmaqau (dest, src1, src2, row1, row2, col);
}

mint32_t test_mint32_pmmaqaus (mint32_t dest, muint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_pmmaqaus (dest, src1, src2, row1, row2, col);
}

mint32_t test_mint32_pmmaqasu (mint32_t dest, mint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_pmmaqasu (dest, src1, src2, row1, row2, col);
}

/* { dg-final { scan-assembler-times "pmmaqa.b\t" 1 } } */
/* { dg-final { scan-assembler-times "pmmaqau.b\t" 1 } } */
/* { dg-final { scan-assembler-times "pmmaqaus.b\t" 1 } } */
/* { dg-final { scan-assembler-times "pmmaqasu.b\t" 1 } } */
