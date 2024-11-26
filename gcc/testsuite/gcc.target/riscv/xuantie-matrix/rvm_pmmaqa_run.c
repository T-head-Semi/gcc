/* { dg-do run } */
/* { dg-skip-if "test rv64 matrix" { *-*-* } { "*" } { "-march=rv64*xtheadmatrix*" } } */


#include "rvm_common.h"
#define N 16

#define VI 1
#define X 1

void __attribute__ ((noinline)) test_pmmaqa_mi8(mrow_t row, mcol_t col)
{
  int8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  int8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int8_t tmp[N] = {0};
  int32_t ans[N] = {0};

  volatile mint8_t src1 = __riscv_th_mld(x, 2, row, col);
  __riscv_th_mst(tmp, 2, src1, row, col);
  debug_data("src1", tmp);

  volatile mint8_t src2 = __riscv_th_mld(y, 2, row, col);
  __riscv_th_mst(tmp, 2, src2, row, col);
  debug_data("src2", tmp);

  mint32_t dest = __riscv_th_mld(ans, 8, row, col);
  dest = __riscv_th_pmmaqa (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("pmmaqa_mi8 dest", ans);
}

void __attribute__ ((noinline)) test_pmmaqau_mui8(mrow_t row, mcol_t col)
{
  uint8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  uint8_t tmp[N] = {0};
  int32_t ans[N] = {0};

  volatile muint8_t src1 = __riscv_th_mld(x, 2, row, col);
  __riscv_th_mst(tmp, 2, src1, row, col);
  debug_data("src1", tmp);

  volatile muint8_t src2 = __riscv_th_mld(y, 2, row, col);
  __riscv_th_mst(tmp, 2, src2, row, col);
  debug_data("src2", tmp);

  mint32_t dest = __riscv_th_mld(ans, 8, row, col);
  dest = __riscv_th_pmmaqau (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("pmmaqau_mui8 dest", ans);
}

void __attribute__ ((noinline)) test_pmmaqaus_mui8_mi8(mrow_t row, mcol_t col)
{

  uint8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  int8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  uint8_t tmp[N] = {0};
  int8_t tmp_i[N] = {0};
  int32_t ans[N] = {0};

  volatile muint8_t src1 = __riscv_th_mld(x, 2, row, col);
  __riscv_th_mst(tmp, 2, src1, row, col);
  debug_data("src1", tmp);

  volatile mint8_t src2 = __riscv_th_mld(y, 2, row, col);
  __riscv_th_mst(tmp_i, 2, src2, row, col);
  debug_data("src2", tmp_i);

  mint32_t dest = __riscv_th_mld(ans, 8, row, col);
  dest = __riscv_th_pmmaqaus (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("pmmaqaus_mui8_mi8 dest", ans);
}

void __attribute__ ((noinline)) test_pmmaqasu_mi8_mui8(mrow_t row, mcol_t col)
{
  int8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int8_t tmp[N] = {0};
  uint8_t tmp_u[N] = {0};
  int32_t ans[N] = {0};

  volatile mint8_t src1 = __riscv_th_mld(x, 2, row, col);
  __riscv_th_mst(tmp, 2, src1, row, col);
  debug_data("src1", tmp);

  volatile muint8_t src2 = __riscv_th_mld(y, 2, row, col);
  __riscv_th_mst(tmp_u, 2, src2, row, col);
  debug_data("src2", tmp_u);

  mint32_t dest = __riscv_th_mld(ans, 8, row, col);
  dest = __riscv_th_pmmaqasu (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 8, dest, row, col);
  debug_data("pmmaqasu_mi8_mui8 dest", ans);
}

int main()
{
  mrow_t row = 2;
  mcol_t col = 2;

  test_pmmaqa_mi8(row, col);
  test_pmmaqau_mui8(row, col);
  test_pmmaqaus_mui8_mi8(row, col);
  test_pmmaqasu_mi8_mui8(row, col);

  return 0;
}

/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "pmmaqa_mi8 dest     : 10  24  4   10\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "pmmaqau_mui8 dest   : 10  24  4   10\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "pmmaqaus_mui8_mi8 dest: 10  24  4   10\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "pmmaqasu_mi8_mui8 dest: 10  24  4   10\\s+\n" } */
