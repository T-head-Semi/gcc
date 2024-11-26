/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d  -O2 -mlcm-mcfg -fno-schedule-insns -fno-schedule-insns2 -fno-schedule-fusion" } */

#include "rvm_common.h"

extern void dosomething();
extern int cond1;
extern int cond2;

mint32_t
test(mint32_t m1, mint8_t m2, mint8_t m3, int8_t *k_ptr, mrow_t msize_m,  mrow_t msize_n, mrow_t msize_k)
{
   while (cond1--)
    {
      m2 = __riscv_th_msld(k_ptr, msize_k * sizeof(int8_t), msize_m, msize_k);
      msize_k++;
      if (cond2)
	m1 = __riscv_th_mmaqa(m1, m2, m3, msize_m, msize_n, msize_k); 
    }

    return m1;
}

/* { dg-final { scan-assembler-times "mcfgm\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgn\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgk\t" 3 } } */
