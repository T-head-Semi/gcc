/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d  -O2 -mlcm-mcfg -fno-schedule-insns -fno-schedule-insns2 -fno-schedule-fusion" } */

#include "rvm_common.h"

extern void dosomething();
extern int cond1;
extern int cond2;

mint32_t
test4(mint32_t m1, uint16_t msize_m,  uint16_t msize_n, uint16_t msize_k)
{
  while (cond1++)
    {
      m1 = __riscv_th_mmul_mm (m1, m1, msize_m, msize_k);
      dosomething();
    }

  return m1;
}

/* { dg-final { scan-assembler-times "mcfgk\tzero,\[a-z\]+\[0-9\]+\n\tmmul.s.mm\t" 1 } } */
