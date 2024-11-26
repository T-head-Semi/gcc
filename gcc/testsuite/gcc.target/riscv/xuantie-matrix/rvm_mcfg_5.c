/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2 -mlcm-mcfg" } */

#include "rvm_common.h"

extern void dosomething();
extern int cond1;
extern int cond2;

mint32_t
test0(mint32_t m1, uint16_t msize_m,  uint16_t msize_n, uint16_t msize_k)
{
  if (cond1)
    {
      m1 = __riscv_th_mmul_mm (m1, m1, msize_m, msize_k);
      m1 = __riscv_th_mmul_mm (m1, m1, msize_m, msize_k);
    }

  return m1;
}


/* { dg-final { scan-assembler-times "mcfgm\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgk\t" 1 } } */
