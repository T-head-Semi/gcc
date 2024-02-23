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

void test (int64_t value)
{
  int64_t date[8] = {0x10000000100, 0x10000000101, 0x10000000102, 0x10000000103, 0x10000000104, 0x10000000105, 0x10000000106, 0x10000000107};
  mint64_t src = __riscv_th_mld (date, STRIDE, MROW, MCOL);
  mint64_t ans = __riscv_th_madd_mx (src, value, MROW, MCOL);
  print_data (ans);        
}

int main ()
{
  /* init data */
  uint64_t src = (uint64_t)0x10000000101;

  test (src);
  
  return 0;
}

/* { dg-output "0x20000000201 0x20000000202 0x20000000203 0x20000000204 0x20000000205 0x20000000206 0x20000000207 0x20000000208 \r\n" } */
