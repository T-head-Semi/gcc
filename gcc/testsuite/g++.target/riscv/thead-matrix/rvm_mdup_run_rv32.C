/* { dg-do run } */
/* { dg-skip-if "test rv32 matrix" { *-*-* } { "*" } { "-march=rv32*xtheadmatrix*" } } */

#include <stdio.h>
#include "rvm_common.h"
#define N 8
#define MROW 4
#define MCOL 2
#define STRIDE 16

void
print_data (mint64_t ma)
{
  int64_t tmp_ma[N] =  {-1, -1, -1, -1, -1, -1, -1, -1};

  __riscv_th_mst (tmp_ma, STRIDE, ma, MROW, MCOL);
  for (int i=0;i<N;i++)
    printf ("0x%0llx ", tmp_ma[i]);

  printf ("\n");
}

void test (int64_t src)
{
  mint64_t ans = __riscv_th_mzero_i64 ();
  ans = __riscv_th_mdup_m_x (src);
  print_data (ans);        
}

int main ()
{
  /* init data */
  uint64_t src = (uint64_t)0x10000000101;

  test (src);

  return 0;
}

/* { dg-output "0x10000000101 0x10000000101 0x10000000101 0x10000000101 0x10000000101 0x10000000101 0x10000000101 0x10000000101 \r\n" } */
