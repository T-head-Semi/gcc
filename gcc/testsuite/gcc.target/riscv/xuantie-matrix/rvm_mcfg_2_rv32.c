/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2 -mlcm-mcfg" } */

#include "rvm_common.h"

void foo0 (int32_t* base, int stride, mint32_t *temp1, mint32_t *temp2)
{
  *temp1 = __riscv_th_mld(base, stride, 2, 2);
  *temp2 = __riscv_th_mld(base, stride, 2, 2);
}

void foo1 (int32_t* base, int stride, mint32_t *temp1, mint32_t *temp2)
{
  *temp1 = __riscv_th_mld(base, stride, 2, 2);
  *temp2 = __riscv_th_mld(base, stride, 2, 4);
}

void foo2 (int32_t* base, int stride, mint32_t *temp1, mint32_t *temp2, size_t msize_m, size_t msize_k)
{
  *temp1 = __riscv_th_mld(base, stride, msize_m, msize_k);
  *temp2 = __riscv_th_mld(base, stride, msize_m, msize_k);
}

void foo3 (int32_t* base, int stride, mint32_t *temp1, mint32_t *temp2, size_t msize_m, size_t msize_k)
{
  *temp1 = __riscv_th_mld(base, stride, msize_m, msize_k);
  *temp2 = __riscv_th_mld(base, stride, msize_m, msize_k * 2);
}

void foo5 (int32_t* base, int stride, mint32_t *temp1, mint32_t *temp2, size_t msize_m, size_t msize_k)
{
  *temp1 = __riscv_th_mld(base, stride, msize_m, msize_k);
   __riscv_th_msetmrow_m(msize_m);
  *temp2 = __riscv_th_mld(base, stride, msize_m, msize_k);
}

void foo6 (mfloat32_t *dest1, mfloat32_t *dest2, mfloat32_t src1, mfloat32_t src2, mrow_t row1, mrow_t row2, mcol_t col, size_t msize_k)
{
  *dest1 = __riscv_th_fmmacc (*dest1, src1, src2, row1, row2, col);
  __riscv_th_msetmcol_e32(msize_k);
  *dest2 = __riscv_th_fmmacc (*dest2, src1, src2, row1, row2, col);
}

/* { dg-final { scan-assembler-times "mcfgm\t" 4 } } */
/* { dg-final { scan-assembler-times "mcfgmi\t" 2 } } */
/* { dg-final { scan-assembler-times "mcfgn\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgk\t" 6 } } */
/* { dg-final { scan-assembler-times "mcfgki\t" 3 } } */
