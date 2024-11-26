/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

mint32_t test_mint32_msub_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mm (src1, src2, row, col);
}

mint64_t test_mint64_msub_mm (mint64_t src1, mint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mm (src1, src2, row, col);
}

mint32_t test_mint32_msub_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mv (src1, src2, index, row, col);
}

mint64_t test_mint64_msub_mv (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mv (src1, src2, index, row, col);
}

mint32_t test_mint32_msub_mv_i (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mv (src1, src2, 1, row, col);
}

mint64_t test_mint64_msub_mv_i (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mv (src1, src2, 1, row, col);
}

mint32_t test_mint32_msub_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mx (src1, src2, row, col);
}

mint64_t test_mint64_msub_mx (mint64_t src1, int64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msub_mx (src1, src2, row, col);
}


/* { dg-final { scan-assembler-times "msub\\.s\\.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "msub\\.s\\.mv\\.x\t" 1 } } */
/* { dg-final { scan-assembler-times "msub\\.s\\.mv\\.i\t" 1 } } */
/* { dg-final { scan-assembler-times "msub\\.s\\.mx\t" 1 } } */
/* { dg-final { scan-assembler-times "msub\\.d\\.mm\t" 2 } } */
/* { dg-final { scan-assembler-times "msub\\.d\\.mv\\.x\t" 1 } } */
/* { dg-final { scan-assembler-times "msub\\.d\\.mv\\.i\t" 1 } } */
/* { dg-final { scan-assembler-times "msub\\.d\\.mx\t" 0 } } */
