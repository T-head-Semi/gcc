/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

void test___riscv_th_mrelease ()
{
  return __riscv_th_mrelease ();
}

/* { dg-final { scan-assembler-times "\tmrelease\n" 1 } } */
