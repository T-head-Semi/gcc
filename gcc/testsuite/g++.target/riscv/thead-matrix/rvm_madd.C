/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

mint32_t test_mint32_madd_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mm (src1, src2, row, col);
}

mint64_t test_mint64_madd_mm (mint64_t src1, mint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mm (src1, src2, row, col);
}

mint32_t test_mint32_madd_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mv (src1, src2, index, row, col);
}

mint64_t test_mint64_madd_mv (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mv (src1, src2, index, row, col);
}

mint32_t test_mint32_madd_mv_i (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mv (src1, src2, 1, row, col);
}

mint64_t test_mint64_madd_mv_i (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mv (src1, src2, 1, row, col);
}

mint32_t test_mint32_madd_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mx (src1, src2, row, col);
}

mint64_t test_mint64_madd_mx (mint64_t src1, int64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_madd_mx (src1, src2, row, col);
}

/* { dg-final { scan-assembler-times "madd\\.s\\.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "madd\\.s\\.mv\\.x\t" 1 } } */
/* { dg-final { scan-assembler-times "madd\\.s\\.mv\\.i\t" 1 } } */
/* { dg-final { scan-assembler-times "madd\\.s\\.mx\t" 1 } } */
/* { dg-final { scan-assembler-times "madd\\.d\\.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "madd\\.d\\.mv\\.x\t" 1 } } */
/* { dg-final { scan-assembler-times "madd\\.d\\.mv\\.i\t" 1 } } */
/* { dg-final { scan-assembler-times "madd\\.d\\.mx\t" 1 } } */
