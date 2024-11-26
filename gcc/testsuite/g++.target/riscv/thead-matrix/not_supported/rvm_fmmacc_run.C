/* { dg-do run } */
/* { dg-skip-if "test rv64 matrix" { *-*-* } { "*" } { "-march=rv64*xtheadmatrix*" } } */


#include "rvm_common.h"
#define N 16

#define VI 1
#define X 1

void __attribute__ ((noinline)) test_fmmacc_mf16x2_mf16(mrow_t row, mcol_t col)
{
  float16_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  float16_t y[N] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  float16_t z[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  float16_t tmp[N] = {0};

  volatile mfloat16_t src1 = __riscv_th_mld(x, 4, row, col);
  __riscv_th_mst(tmp, 4, src1, row, col);
  debug_data("src1", tmp);

  volatile mfloat16_t src2_1 = __riscv_th_mld(y, 4, row, col);
  volatile mfloat16_t src2_2 = __riscv_th_mld(z, 4, row, col);
  volatile mfloat16x2_t src2 = __riscv_th_mzero_f16x2();
  src2 = __riscv_th_mset(src2, 0, src2_1);
  src2 = __riscv_th_mset(src2, 1, src2_1);

  __riscv_th_mst(tmp, 4, src2_1, row, col);
  debug_data("src2_1", tmp);
  __riscv_th_mst(tmp, 4, src2_2, row, col);
  debug_data("src2_2", tmp);

  mfloat16_t dest = __riscv_th_mld(tmp, 4, row, col);
  __riscv_th_mst(tmp, 4, dest, row, col);
  debug_data("dest", tmp);

  dest = __riscv_th_fmmacc (dest, src1, src2, row, row, col);
  __riscv_th_mst(tmp, 4, dest, row, col);
  debug_data("fmmacc_mf16 dest", tmp);
}

void __attribute__ ((noinline)) test_fmmacc_mf32(mrow_t row, mcol_t col)
{
  float32_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  float32_t y[N] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  float32_t z[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  float32_t tmp[N] = {0};

  volatile mfloat32_t src1 = __riscv_th_mld(x, 8, row, col);
  __riscv_th_mst(tmp, 8, src1, row, col);
  debug_data("src1", tmp);

  volatile mfloat32_t src2 = __riscv_th_mld(y, 8, row, col);
  __riscv_th_mst(tmp, 8, src2, row, col);
  debug_data("src2", tmp);

  mfloat32_t dest = __riscv_th_mzero_f32();
  __riscv_th_mst(tmp, 8, dest, row, col);
  debug_data("dest", tmp);
  dest = __riscv_th_fmmacc (dest, src1, src2, row, row, col);
  __riscv_th_mst(tmp, 8, dest, row, col);
  debug_data("fmmacc_mf32 dest", tmp);
}

void __attribute__ ((noinline)) test_fmmacc_mf64(mrow_t row, mcol_t col)
{
  float64_t x[N] = {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
  float64_t y[N] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  float64_t z[N] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  float64_t tmp[N*2] = {0};

  volatile mfloat64_t src1 = __riscv_th_mld(x, 16, row, col);
  __riscv_th_mst(tmp, 16, src1, row, col);
  debug_data("src1", tmp);

  volatile mfloat64_t src2 = __riscv_th_mld(y, 16, row, col);
  __riscv_th_mst(tmp, 16, src2, row, col);
  debug_data("src2", tmp);

  volatile mfloat64_t dest1 = __riscv_th_mld(tmp, 16, row, col);
  volatile mfloat64_t dest2 = __riscv_th_mld(tmp + N, 16, row, col);
  volatile mfloat64x2_t dest = __riscv_th_mzero_f64x2();
  dest = __riscv_th_mset(dest, 0, dest1);
  dest = __riscv_th_mset(dest, 1, dest2);

  dest = __riscv_th_fmmacc (dest, src1, src2, row, row, col);
  __riscv_th_mst(tmp, 16, __riscv_th_mget(dest, 0), row, col);
  debug_data("fmmacc_mf64 dest1", tmp);
  __riscv_th_mst(tmp + N, 16, __riscv_th_mget(dest, 1), row, col);
  debug_data("fmmacc_mf64 dest2", tmp + N);
}

int main()
{
  mrow_t row = 2;
  mcol_t col = 2;

  test_fmmacc_mf16x2_mf16(row, col);
  test_fmmacc_mf32(row, col);
  test_fmmacc_mf64(row, col);

  return 0;
}

/* { dg-output "src1                : 36.000000   35.000000   34.000000   33.000000\\s+\n" } */
/* { dg-output "src2_1              : 16.000000   15.000000   14.000000   13.000000\\s+\n" } */
/* { dg-output "src2_2              : 1.000000    2.000000    3.000000    4.000000\\s+\n" } */
/* { dg-output "dest                : 0.000000    0.000000    0.000000    0.000000\\s+\n" } */
/* { dg-output "fmmacc_mf16 dest    : 1101.000000 959.000000  1039.000000 905.000000\\s+\n" } */
/* { dg-output "src1                : 36.000000   35.000000   34.000000   33.000000\\s+\n" } */
/* { dg-output "src2                : 16.000000   15.000000   14.000000   13.000000\\s+\n" } */
/* { dg-output "dest                : 0.000000    0.000000    0.000000    0.000000\\s+\n" } */
/* { dg-output "fmmacc_mf32 dest    : 1101.000000 959.000000  1039.000000 905.000000\\s+\n" } */
/* { dg-output "src1                : 36.000000   35.000000   34.000000   33.000000\\s+\n" } */
/* { dg-output "src2                : 16.000000   15.000000   14.000000   13.000000\\s+\n" } */
/* { dg-output "fmmacc_mf64 dest1   : 1101.000000 959.000000  1039.000000 905.000000\\s+\n" } */
/* { dg-output "fmmacc_mf64 dest2   : 0.000000    0.000000    0.000000    0.000000\\s+\n" } */
