/* { dg-do run } */
/* { dg-skip-if "test rv32 matrix" { *-*-* } { "*" } { "-march=rv32*xtheadmatrix*" } } */


#include "rvm_common.h"
#define N 16

#define VI 1
#define X 1

void __attribute__ ((noinline)) test_msra(mrow_t row, mcol_t col)
{  
  int32_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint32_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int32_t tmp[N] = {0};
  uint32_t tmp_u[N] = {0};

  volatile mint32_t src1 = __riscv_th_mld(x, 8, row, col);
  __riscv_th_mst(tmp, 8, src1, row, col);
  debug_data("src1", tmp);

  volatile muint32_t src2 = __riscv_th_mld(y, 8, row, col);
  __riscv_th_mst(tmp_u, 8, src2, row, col);
  debug_data("src2", tmp_u);

  volatile mint32_t dest;
  dest = __riscv_th_msra_mm (src1, src2, row, col);
  __riscv_th_mst(tmp, 8, dest, row, col);
  debug_data("sra mm dest", tmp);

  dest = __riscv_th_msra_mv (src1, src2, VI, row, col);
  __riscv_th_mst(tmp, 8, dest, row, col);
  debug_data("sra mv dest", tmp);

  dest = __riscv_th_msra_mx (src1, X, row, col);
  __riscv_th_mst(tmp, 8, dest, row, col);
  debug_data("sra mx dest", tmp);
}

void __attribute__ ((noinline)) test_mn4clip(mrow_t row, mcol_t col)
{
  int32_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint32_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int32_t tmp[N] = {0};
  uint32_t tmp_u[N] = {0};
  int8_t ans[N] = {0};

  volatile mint32_t src1 = __riscv_th_mld(x, 8, row, col);
  __riscv_th_mst(tmp, 8, src1, row, col);
  debug_data("src1", tmp);

  volatile muint32_t src2 = __riscv_th_mld(y, 8, row, col);
  __riscv_th_mst(tmp_u, 8, src2, row, col);
  debug_data("src2", tmp_u);

  volatile mint8_t dest;
  dest = __riscv_th_mn4clip_mm (src1, src2, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("mn4clip mm dest", ans);

  dest = __riscv_th_mn4clip_mv (src1, src2, VI, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("mn4clip mv dest", ans);

  dest = __riscv_th_mn4clip_mx (src1, X, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("mn4clip mx dest", ans);
}

void __attribute__ ((noinline)) test_mn4clipu(mrow_t row, mcol_t col)
{
  uint32_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint32_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  uint32_t tmp[N] = {0};
  uint8_t ans[N] = {0};

  volatile muint32_t src1 = __riscv_th_mld(x, 8, row, col);
  __riscv_th_mst(tmp, 8, src1, row, col);
  debug_data("src1", tmp);

  volatile muint32_t src2 = __riscv_th_mld(y, 8, row, col);
  __riscv_th_mst(tmp, 8, src2, row, col);
  debug_data("src2", tmp);

  volatile muint8_t dest;
  dest = __riscv_th_mn4clipu_mm(src1, src2, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("mn4clipu mm dest", ans);

  dest = __riscv_th_mn4clipu_mv (src1, src2, VI, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("mn4clipu mv dest", ans);

  dest = __riscv_th_mn4clipu_mx (src1, X, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("mn4clipu mx dest", ans);
}

int main()
{
  mrow_t row = 2;
  mcol_t col = 2;

  test_msra(row, col);
  test_mn4clip(row, col);
  test_mn4clipu(row, col);

  return 0;
}

/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "sra mm dest         : 18  9   4   2\\s+\n" } */
/* { dg-output "sra mv dest         : 5   2   4   2\\s+\n" } */
/* { dg-output "sra mx dest         : 18  18  17  17\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "mn4clip mm dest     : 18  9   0   0\\s+\n" } */
/* { dg-output "mn4clip mv dest     : 5   2   0   0\\s+\n" } */
/* { dg-output "mn4clip mx dest     : 18  18  0   0\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "mn4clipu mm dest    : 18  9   0   0\\s+\n" } */
/* { dg-output "mn4clipu mv dest    : 5   2   0   0\\s+\n" } */
/* { dg-output "mn4clipu mx dest    : 18  18  0   0\\s+\n" } */
