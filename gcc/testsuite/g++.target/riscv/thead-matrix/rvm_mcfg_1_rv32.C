/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

void test_mcfg(size_t *temp[12], size_t mm, size_t mn, size_t mk){
  unsigned i = 0;
  *temp[i++] = __riscv_th_msetmrow_m(mm);
  *temp[i++] = __riscv_th_msetmrow_n(mn);
  *temp[i++] = __riscv_th_msetmcol_e8(mk);
  *temp[i++] = __riscv_th_msetmcol_e16(mk);
  *temp[i++] = __riscv_th_msetmcol_e32(mk);
  *temp[i++] = __riscv_th_msetmcol_e64(mk);

  *temp[i++] = __riscv_th_msetmrow_m(2);
  *temp[i++] = __riscv_th_msetmrow_n(2);
  *temp[i++] = __riscv_th_msetmcol_e8(4);
  *temp[i++] = __riscv_th_msetmcol_e16(4);
  *temp[i++] = __riscv_th_msetmcol_e32(4);
  *temp[i++] = __riscv_th_msetmcol_e64(4);
}

/* { dg-final { scan-assembler-times "mcfgm\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgmi\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgn\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgni\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgk\t" 4 } } */
/* { dg-final { scan-assembler-times "mcfgki\t" 4 } } */
