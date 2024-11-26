/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-Os" "-Oz" } } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include <thead_matrix.h>

void
foo(mint32_t x, mfloat32x2_t y)
{
  __asm("nop");
}

/* { dg-final { scan-assembler-times "foou15__xthm_mint32_tu19__xthm_mfloat32x2_t:" 1 } } */
