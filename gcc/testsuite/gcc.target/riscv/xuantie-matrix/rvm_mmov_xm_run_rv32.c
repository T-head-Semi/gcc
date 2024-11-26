/* { dg-do run } */
/* { dg-skip-if "test rv32 matrix" { *-*-* } { "*" } { "-march=rv32*xtheadmatrix*" } } */


#include <stdio.h>
#include "rvm_common.h"
#define N 8
#define MROW 4
#define MCOL 2
#define STRIDE 16

int main ()
{
  /* init data */
  int64_t src[N] = {0x10000000100, 0x10000000101, 0x10000000102, 0x10000000103, 0x10000000104, 0x10000000105, 0x10000000106, 0x10000000107};
  mint64_t ans = __riscv_th_mld (src, STRIDE, MROW, MCOL);
  int64_t value = __riscv_th_mmov_x_m (ans, 2);
  printf ("0x%0llx\n",value);

  return 0;
}
/* { dg-output "0x10000000102\r\n" } */
