/* { dg-do run } */
/* { dg-skip-if "test rv64 matrix" { *-*-* } { "*" } { "-march=rv64*xtheadmatrix*" } } */


#include "rvm_common.h"
#define N 16

#define VI 1
#define X 1

void __attribute__ ((noinline)) test_mmaqa_mi16(mrow_t row, mcol_t col)
{
  int16_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  int16_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int16_t tmp[N] = {0};
  int64_t ans[N] = {0};

  volatile mint16_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile mint16_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp, 4, src2, row, col);
  debug_data("src2", tmp);

  volatile mint64_t dest1 = __riscv_th_mld(ans, 16, row, col);
  volatile mint64_t dest2 = __riscv_th_mld(ans + N, 16, row, col);
  volatile mint64x2_t dest = __riscv_th_mzero_i64x2();
  dest = __riscv_th_mset(dest, 0, dest1);
  dest = __riscv_th_mset(dest, 1, dest2);

  dest = __riscv_th_mmaqa (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, __riscv_th_mget(dest, 0), row, col);
  debug_data("mmaqa dest1", ans);
  __riscv_th_mst(ans + N, 16, __riscv_th_mget(dest, 1), row, col);
  debug_data("mmaqa dest2", ans + N);
}

void __attribute__ ((noinline)) test_mmaqau_mui16(mrow_t row, mcol_t col)
{
  uint16_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint16_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  uint16_t tmp[N] = {0};
  int64_t ans[N] = {0};

  volatile muint16_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile muint16_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp, 4, src2, row, col);
  debug_data("src2", tmp);

  volatile mint64_t dest1 = __riscv_th_mld(ans, 16, row, col);
  volatile mint64_t dest2 = __riscv_th_mld(ans + N, 16, row, col);
  volatile mint64x2_t dest = __riscv_th_mzero_i64x2();
  dest = __riscv_th_mset(dest, 0, dest1);
  dest = __riscv_th_mset(dest, 1, dest2);

  dest = __riscv_th_mmaqau (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, __riscv_th_mget(dest, 0), row, col);
  debug_data("mmaqau dest1", ans);
  __riscv_th_mst(ans + N, 16, __riscv_th_mget(dest, 1), row, col);
  debug_data("mmaqau dest2", ans + N);
}

void __attribute__ ((noinline)) test_mmaqaus_mui16_mi16(mrow_t row, mcol_t col)
{
  uint16_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  int16_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  uint16_t tmp[N] = {0};
  int16_t tmp_i[N] = {0};
  int64_t ans[N] = {0};

  volatile muint16_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile mint16_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp_i, 4, src2, row, col);
  debug_data("src2", tmp_i);

  volatile mint64_t dest1 = __riscv_th_mld(ans, 16, row, col);
  volatile mint64_t dest2 = __riscv_th_mld(ans + N, 16, row, col);
  volatile mint64x2_t dest = __riscv_th_mzero_i64x2();
  dest = __riscv_th_mset(dest, 0, dest1);
  dest = __riscv_th_mset(dest, 1, dest2);

  dest = __riscv_th_mmaqaus (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, __riscv_th_mget(dest, 0), row, col);
  debug_data("mmaqaus dest1", ans);
  __riscv_th_mst(ans + N, 16, __riscv_th_mget(dest, 1), row, col);
  debug_data("mmaqaus dest2", ans + N);
}

void __attribute__ ((noinline)) test_mmaqasu_mi16_mui16(mrow_t row, mcol_t col)
{
  int16_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint16_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int16_t tmp[N] = {0};
  uint16_t tmp_u[N] = {0};
  int64_t ans[N] = {0};

  volatile mint16_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile muint16_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp_u, 4, src2, row, col);
  debug_data("src2", tmp_u);

  volatile mint64_t dest1 = __riscv_th_mld(ans, 16, row, col);
  volatile mint64_t dest2 = __riscv_th_mld(ans + N, 16, row, col);
  volatile mint64x2_t dest = __riscv_th_mzero_i64x2();
  dest = __riscv_th_mset(dest, 0, dest1);
  dest = __riscv_th_mset(dest, 1, dest2);

  dest = __riscv_th_mmaqasu (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, __riscv_th_mget(dest, 0), row, col);
  debug_data("mmaqasu dest1", ans);
  __riscv_th_mst(ans + N, 16, __riscv_th_mget(dest, 1), row, col);
  debug_data("mmaqasu dest2", ans + N);
}

int main()
{
  mrow_t row = 2;
  mcol_t col = 2;

  test_mmaqa_mi16(row, col);
  test_mmaqau_mui16(row, col);
  test_mmaqaus_mui16_mi16(row, col);
  test_mmaqasu_mi16_mui16(row, col);

  return 0;
}

/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "mmaqa dest1         : 106 248 100 234\\s+\n" } */
/* { dg-output "mmaqa dest2         : 0   0   0   0\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "mmaqau dest1        : 106 248 100 234\\s+\n" } */
/* { dg-output "mmaqau dest2        : 0   0   0   0\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "mmaqaus dest1       : 106 248 100 234\\s+\n" } */
/* { dg-output "mmaqaus dest2       : 0   0   0   0\\s+\n" } */
/* { dg-output "src1                : 36  35  34  33\\s+\n" } */
/* { dg-output "src2                : 1   2   3   4\\s+\n" } */
/* { dg-output "mmaqasu dest1       : 106 248 100 234\\s+\n" } */
/* { dg-output "mmaqasu dest2       : 0   0   0   0\\s+\n" } */