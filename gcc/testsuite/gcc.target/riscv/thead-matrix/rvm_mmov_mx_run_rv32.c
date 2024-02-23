/* { dg-do run } */
/* { dg-skip-if "test rv32 matrix" { *-*-* } { "*" } { "-march=rv32*xtheadmatrix*" } } */
/* { dg-options " -O2 " } */

#include <stdio.h>
#include "rvm_common.h"
#define N 8
#define MROW 4
#define MCOL 2
#define STRIDE 16

void
print_data (mint64_t ma)
{
  int64_t tmp_ma[N] = {-1, -1, -1, -1, -1, -1, -1, -1};

  __riscv_th_mst (tmp_ma, STRIDE, ma, MROW, MCOL);
  for (int i=0;i<N;i++)
    printf ("0x%0llx ", tmp_ma[i]);

  printf ("\n");
}

int main ()
{
  /* init data */
  uint64_t src = (uint64_t)0x10000000101;

  __volatile__ mint64_t ans = __riscv_th_mundefined_i64 ();
  ans = __riscv_th_mmov_m_x (ans, src, 1);
  print_data (ans);

  return 0;
}

/* { dg-output "0x0 0x10000000101 0x0 0x0 0x0 0x0 0x0 0x0 \r\n" } */
