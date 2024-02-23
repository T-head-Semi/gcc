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
print_data (muint8_t ma)
{
  uint8_t tmp_ma[N] = {-1, -1, -1, -1, -1, -1, -1, -1};

  __riscv_th_mst (tmp_ma, 2, ma, MROW, MCOL);
  for (int i=0;i<N;i++)
    printf ("0x%0x ", tmp_ma[i]);

  printf ("\n");
}

void test (uint64_t value)
{
  uint64_t date[8] = {0x10000000100, 0x10000000101, 0x10000000102, 0x10000000103, 0x10000000104, 0x10000000105, 0x10000000106, 0x10000000107};
  muint64_t src = __riscv_th_mld (date, STRIDE, MROW, MCOL);
  muint8_t ans = __riscv_th_mn4clipu_mx (src, value, MROW, MCOL);
  print_data (ans);        
}

int main ()
{
  /* init data */
  uint64_t src = (uint64_t)0x10000000101;

  test (src);

  return 0;
}

/* { dg-output "0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff \r\n" } */
