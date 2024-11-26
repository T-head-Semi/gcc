/* { dg-do run } */
/* { dg-skip-if "test rv32 matrix" { *-*-* } { "*" } { "-march=rv32*xtheadmatrix*" } } */

#include "rvm_common.h"
#define N 16

#define VI 1
#define X 1

void __attribute__ ((noinline)) test_mmaqa_mi8(mrow_t row, mcol_t col)
{
  int8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  int8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int8_t tmp[N] = {0};
  int32_t ans[N] = {0};

  volatile mint8_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile mint8_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp, 4, src2, row, col);
  debug_data("src2", tmp);

  volatile mint32_t dest = __riscv_th_mzero_i32();

  dest = __riscv_th_mmaqa (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, dest, row, col);
  debug_data("mmaqa dest", ans);
}

void __attribute__ ((noinline)) test_mmaqau_mui8(mrow_t row, mcol_t col)
{
  uint8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  uint8_t tmp[N] = {0};
  int32_t ans[N] = {0};

  volatile muint8_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile muint8_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp, 4, src2, row, col);
  debug_data("src2", tmp);

  volatile mint32_t dest = __riscv_th_mzero_i32();

  dest = __riscv_th_mmaqau (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, dest, row, col);
  debug_data("mmaqau dest", ans);
}

void __attribute__ ((noinline)) test_mmaqaus_mui8_mi8(mrow_t row, mcol_t col)
{
  uint8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  int8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  uint8_t tmp[N] = {0};
  int8_t tmp_i[N] = {0};
  int32_t ans[N] = {0};

  volatile muint8_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile mint8_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp_i, 4, src2, row, col);
  debug_data("src2", tmp_i);

  volatile mint32_t dest = __riscv_th_mzero_i32();

  dest = __riscv_th_mmaqaus (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, dest, row, col);
  debug_data("mmaqaus dest", ans);
}

void __attribute__ ((noinline)) test_mmaqasu_mi8_mui8(mrow_t row, mcol_t col)
{
  int8_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  uint8_t y[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  int8_t tmp[N] = {0};
  uint8_t tmp_u[N] = {0};
  int32_t ans[N] = {0};

  volatile mint8_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile muint8_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp_u, 4, src2, row, col);
  debug_data("src2", tmp_u);

  volatile mint32_t dest = __riscv_th_mzero_i32();

  dest = __riscv_th_mmaqasu (dest, src1, src2, row, row, col);
  __riscv_th_mst(ans, 16, dest, row, col);
  debug_data("mmaqasu dest", ans);
}

int main()
{
  mrow_t row = 2;
  mcol_t col = 2;

  test_mmaqa_mi8(row, col);
  test_mmaqau_mui8(row, col);
  test_mmaqaus_mui8_mi8(row, col);
  test_mmaqasu_mi8_mui8(row, col);

  return 0;
}

/* { dg-output "src1                : 36  35  0   0\\s+\n" } */
/* { dg-output "src2                : 1   2   0   0\\s+\n" } */
/* { dg-output "mmaqa dest          : 106 106 0   0\\s+\n" } */
/* { dg-output "src1                : 36  35  0   0\\s+\n" } */
/* { dg-output "src2                : 1   2   0   0\\s+\n" } */
/* { dg-output "mmaqau dest         : 106 106 0   0\\s+\n" } */
/* { dg-output "src1                : 36  35  0   0\\s+\n" } */
/* { dg-output "src2                : 1   2   0   0\\s+\n" } */
/* { dg-output "mmaqaus dest        : 106 106 0   0\\s+\n" } */
/* { dg-output "src1                : 36  35  0   0\\s+\n" } */
/* { dg-output "src2                : 1   2   0   0\\s+\n" } */
/* { dg-output "mmaqasu dest        : 106 106 0   0\\s+\n" } */