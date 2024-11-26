/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

mint32_t test_mint32_mmaqa (mint32_t dest, mint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqa (dest, src1, src2, row1, row2, col);
}

mint64x2_t test_mint64x2_mmaqa (mint64x2_t dest, mint16_t src1, mint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqa (dest, src1, src2, row1, row2, col);
}

mint32_t test_muint32_mmaqau (mint32_t dest, muint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqau (dest, src1, src2, row1, row2, col);
}

mint64x2_t test_muint64_mmaqau (mint64x2_t dest, muint16_t src1, muint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqau (dest, src1, src2, row1, row2, col);
}

mint32_t test_mint32_mmaqaus (mint32_t dest, muint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqaus (dest, src1, src2, row1, row2, col);
}

mint64x2_t test_mint64x2_mmaqaus (mint64x2_t dest, muint16_t src1, mint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqaus (dest, src1, src2, row1, row2, col);
}

mint32_t test_mint32_mmaqasu (mint32_t dest, mint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqasu (dest, src1, src2, row1, row2, col);
}

mint64x2_t test_mint64x2_mmaqasu (mint64x2_t dest, mint16_t src1, muint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{
  return __riscv_th_mmaqasu (dest, src1, src2, row1, row2, col);
}

/* { dg-final { scan-assembler-times "mmaqa.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqa.h\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqau.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqau.h\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqaus.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqaus.h\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqasu.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqasu.h\t" 1 } } */
