/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -O0" } */

#include <riscv_vector.h>

void
foo(char s2)
{
  char c[4096];
  __volatile__ vfloat64m1_t vec_t;
}
