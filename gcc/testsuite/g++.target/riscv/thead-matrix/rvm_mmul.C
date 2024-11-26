/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

mint32_t test_mint32_mmul_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mm (src1, src2, row, col);
}

mint64_t test_mint64_mmul_mm (mint64_t src1, mint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mm (src1, src2, row, col);
}

mint32_t test_mint32_mmul_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mv (src1, src2, index, row, col);
}

mint64_t test_mint64_mmul_mv (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mv (src1, src2, index, row, col);
}

mint32_t test_mint32_mmul_mv_i (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mv (src1, src2, 1, row, col);
}

mint64_t test_mint64_mmul_mv_i (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mv (src1, src2, 1, row, col);
}

mint32_t test_mint32_mmul_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mx (src1, src2, row, col);
}

mint64_t test_mint64_mmul_mx (mint64_t src1, int64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mmul_mx (src1, src2, row, col);
}

/* { dg-final { scan-assembler-times "mmul\\.s\\.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul\\.s\\.mv\\.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul\\.s\\.mv\\.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul\\.s\\.mx\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul\\.d\\.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul\\.d\\.mv\\.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul\\.d\\.mv\\.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul\\.d\\.mx\t" 1 } } */
