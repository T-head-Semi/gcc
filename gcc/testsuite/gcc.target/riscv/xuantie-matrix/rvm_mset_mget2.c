/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

void test (float64_t *dest, float64_t *src1, float64_t *src2, mfloat64_t a, mfloat64_t b)
{
  mfloat64_t temp1 = __riscv_th_mundefined_f64 ();
  mfloat64_t temp2 = __riscv_th_mundefined_f64 ();
  temp1 = __riscv_th_mld (src1, 8, 2, 2);
  temp2 = __riscv_th_mld (src2, 8, 2, 2);
  mfloat64x2_t temp3 = __riscv_th_mundefined_f64x2 ();

  temp3 = __riscv_th_mset (temp3, 0, temp1);
  temp3 = __riscv_th_mset (temp3, 1, temp2);
  temp3 = __riscv_th_fmmacc (temp3, a, b, 2, 2, 2);
  temp1 = __riscv_th_mget (temp3, 0);
  temp2 = __riscv_th_mget (temp3, 1);

  __riscv_th_mst (src1, 8, temp1, 2, 2);
  __riscv_th_mst (src2, 8, temp2, 2, 2); 
}

/* { dg-final { scan-assembler-not "mmov.mm\tm\[0-9]+,m\[0-9]+\n" } } */