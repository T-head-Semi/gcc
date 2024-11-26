/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

void test_mcfg(unsigned long *temp[3]){
  unsigned i = 0;
  *temp[i++] = __riscv_th_xmlenb();
  *temp[i++] = __riscv_th_xrlenb();
  *temp[i++] = __riscv_th_xmsize();
}

/* { dg-final { scan-assembler-times "csrr\ta\[0-9\]+,xmlenb" 1 } } */
/* { dg-final { scan-assembler-times "csrr\ta\[0-9\]+,xrlenb" 1 } } */
/* { dg-final { scan-assembler-times "csrr\ta\[0-9\]+,xmsize" 1 } } */
