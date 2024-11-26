/* { dg-do run } */
/* { dg-skip-if "test rv64 matrix" { *-*-* } { "*" } { "-march=rv64*xtheadmatrix*" } } */


#include "rvm_common.h"
#define N 16

#define VI 1
#define X 1

void __attribute__ ((noinline)) test_fwmmacc_mf16(mrow_t row, mcol_t col)
{
  float16_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  float16_t y[N] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  float16_t z[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  float16_t tmp[N] = {0};
  float32_t tmp_32[N] = {0};

  volatile mfloat16_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile mfloat16_t src2 = __riscv_th_mld(y, 4, row, col);
  __riscv_th_mst(tmp, 4, src2, row, col);
  debug_data("src2", tmp);

  mfloat32_t dest = __riscv_th_mld(tmp_32, 8, row, col);
  dest = __riscv_th_fwmmacc (dest, src1, src2, row, row, col);
  __riscv_th_mst(tmp_32, 8, dest, row, col);
  debug_data("fwmmacc_mf16 dest", tmp_32);
}

void __attribute__ ((noinline)) test_fwmmacc_mf32(mrow_t row, mcol_t col)
{
  float32_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  float32_t y[N] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  float32_t z[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  float32_t tmp[N] = {0};
  float64_t tmp_64[N] = {0};

  volatile mfloat32_t src1 = __riscv_th_mld(x, 8, row, col);
  __riscv_th_mst(tmp, 8, src1, row, col);
  debug_data("src1", tmp);

  volatile mfloat32_t src2 = __riscv_th_mld(y, 8, row, col);
  __riscv_th_mst(tmp, 8, src2, row, col);
  debug_data("src2", tmp);

  volatile mfloat64_t dest1 = __riscv_th_mld(tmp_64, 16, row, col);
  volatile mfloat64_t dest2 = __riscv_th_mld(tmp_64 + N, 16, row, col);
  volatile mfloat64x2_t dest = __riscv_th_mzero_f64x2();
  dest = __riscv_th_mset(dest, 0, dest1);
  dest = __riscv_th_mset(dest, 1, dest2);

  dest = __riscv_th_fwmmacc (dest, src1, src2, row, row, col);
  __riscv_th_mst(tmp_64, 16, __riscv_th_mget(dest, 0), row, col);
  debug_data("fwmmacc_mf64 dest1", tmp_64);
  __riscv_th_mst(tmp_64 + N, 16, __riscv_th_mget(dest, 1), row, col);
  debug_data("fwmmacc_mf64 dest2", tmp_64 + N);
}

int main()
{
  mrow_t row = 2;
  mcol_t col = 2;

  test_fwmmacc_mf16(row, col);
  test_fwmmacc_mf32(row, col);

  return 0;
}

/* { dg-output "src1                : 36.000000   35.000000   34.000000   33.000000\\s+\n" } */
/* { dg-output "src2                : 16.000000   15.000000   14.000000   13.000000\\s+\n" } */
/* { dg-output "fwmmacc_mf16 dest   : 1101.000000 959.000000  1039.000000 905.000000\\s+\n" } */
/* { dg-output "src1                : 36.000000   35.000000   34.000000   33.000000\\s+\n" } */
/* { dg-output "src2                : 16.000000   15.000000   14.000000   13.000000\\s+\n" } */
/* { dg-output "fwmmacc_mf64 dest1  : 1101.000000 959.000000  1039.000000 905.000000\\s+\n" } */
/* { dg-output "fwmmacc_mf64 dest2  : 0.000000    0.000000    0.000000    0.000000\\s+\n" } */
