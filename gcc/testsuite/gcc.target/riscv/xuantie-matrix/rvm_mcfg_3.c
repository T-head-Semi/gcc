/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2 -mlcm-mcfg" } */

#include "rvm_common.h"

extern int a;
extern int b;
extern int c;
mint32_t test(mint32_t ma, int32_t *data1, int32_t *data2, long stride, mrow_t row, mcol_t col)
{
  mint32_t mb, ans;

 if (a)
    ma = __riscv_th_mld(data1, stride, row, col);

 if (b)
    mb = __riscv_th_mld(data2, stride, row, col);
 
 if (c)
    ans = __riscv_th_mmul_mm(ma, mb, row, col);
 
 return ans;
}

/* { dg-final { scan-assembler-times "mcfgm\t" 2 } } */
/* { dg-final { scan-assembler-times "mcfgk\t" 2 } } */
