/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

mint32_t test_mint32_mmulh_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mmulh_mm (src1, src2, row, col);
}

mint32_t test_mint32_mmulh_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mmulh_mv (src1, src2, index, row, col);
}

mint32_t test_mint32_mmulh_mv_i (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mmulh_mv (src1, src2, 1, row, col);
}

mint32_t test_mint32_mmulh_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mmulh_mx (src1, src2, row, col);
}

/* { dg-final { scan-assembler-times "mmulh\\.s\\.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mmulh\\.s\\.mv\\.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mmulh\\.s\\.mv\\.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mmulh\\.s\\.mx\t" 1 } } */
