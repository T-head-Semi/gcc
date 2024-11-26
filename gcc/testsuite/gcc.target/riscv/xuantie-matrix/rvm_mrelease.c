/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

void test___riscv_th_mrelease ()
{
  return __riscv_th_mrelease ();
}

/* { dg-final { scan-assembler-times "\tmrelease\n" 1 } } */
