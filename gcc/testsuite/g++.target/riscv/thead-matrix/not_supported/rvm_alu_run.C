/* { dg-do run } */
/* { dg-skip-if "test rv64 matrix" { *-*-* } { "*" } { "-march=rv64*xtheadmatrix*" } } */


#include "rvm_common.h"
#define N 16

#define VI 1
#define X 1

#define __TEST_DEFINE(instr)                        \
void __attribute__ ((noinline)) test_##instr(mrow_t row, mcol_t col) \
{                      \
  int32_t x[N] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};                    \
  int32_t y[N] = {0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11};                    \
                    \
  int32_t tmp[N] = {0};                    \
                    \
  volatile mint32_t src1 = __riscv_th_mld(x, 8, row, col);                    \
  __riscv_th_mst(tmp, 8, src1, row, col);                    \
                    \
  volatile mint32_t src2 = __riscv_th_mld(y, 8, row, col);                    \
  __riscv_th_mst(tmp, 8, src2, row, col);                    \
                                          \
  volatile mint32_t dest;                    \
  dest = __riscv_th_##instr##_mm(src1, src2, row, col);            \
  __riscv_th_mst(tmp, 8, dest, row, col);                \
  debug_data(""#instr" mm dest", tmp);    \
                                            \
  dest = __riscv_th_##instr##_mv(src1, src2, VI, row, col);  \
  __riscv_th_mst(tmp, 8, dest, row, col);                \
  debug_data(""#instr" mv dest", tmp);    \
                                            \
  dest = __riscv_th_##instr##_mx(src1, X, row, col);         \
  __riscv_th_mst(tmp, 8, dest, row, col);                \
  debug_data(""#instr" mx dest", tmp);    \
}

#define __TEST_RUN(instr) \
  test_##instr(row, col);

#define __TEST_ITERATOR(MACRO, ...) \
  MACRO(madd)                        \
  MACRO(msub)                        \
  MACRO(mmul)         

#define __TEST_MULH_DEFINE(instr)                        \
void __attribute__ ((noinline)) test_##instr(mrow_t row, mcol_t col) \
{                      \
  int32_t x[N] = {0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000, 0x70000, 0x80000};                    \
  int32_t y[N] = {0x80000, 0x70000, 0x60000, 0x50000, 0x40000, 0x30000, 0x20000, 0x10000};                    \
                    \
  int32_t tmp[N] = {0};                    \
                    \
  volatile mint32_t src1 = __riscv_th_mld(x, 8, row, col);                    \
  __riscv_th_mst(tmp, 8, src1, row, col);                    \
                    \
  volatile mint32_t src2 = __riscv_th_mld(y, 8, row, col);                    \
  __riscv_th_mst(tmp, 8, src2, row, col);                    \
                                          \
  volatile mint32_t dest;                    \
  dest = __riscv_th_##instr##_mm(src1, src2, row, col);            \
  __riscv_th_mst(tmp, 8, dest, row, col);                \
  debug_data(""#instr" mm dest", tmp);    \
                                            \
  dest = __riscv_th_##instr##_mv(src1, src2, VI, row, col);  \
  __riscv_th_mst(tmp, 8, dest, row, col);                \
  debug_data(""#instr" mv dest", tmp);    \
                                            \
  dest = __riscv_th_##instr##_mx(src1, 0x20000, row, col);         \
  __riscv_th_mst(tmp, 8, dest, row, col);                \
  debug_data(""#instr" mx dest", tmp);    \
}

#define __TEST_MULH_ITERATOR(MACRO, ...) \
  MACRO(mmulh)     

__TEST_ITERATOR(__TEST_DEFINE)

__TEST_MULH_ITERATOR(__TEST_MULH_DEFINE)

int main()
{
  mrow_t row = 2;
  mcol_t col = 2;

  __TEST_ITERATOR(__TEST_RUN);
  __TEST_MULH_ITERATOR(__TEST_RUN);
  return 0;
}

/* { dg-output "madd mm dest        : 41  41  41  41\\s+\n" } */
/* { dg-output "madd mv dest        : 39  39  41  41\\s+\n" } */
/* { dg-output "madd mx dest        : 18  19  20  21\\s+\n" } */
/* { dg-output "msub mm dest        : -7  -5  -3  -1\\s+\n" } */
/* { dg-output "msub mv dest        : -5  -3  -3  -1\\s+\n" } */
/* { dg-output "msub mx dest        : 16  17  18  19\\s+\n" } */
/* { dg-output "mmul mm dest        : 408 414 418 420\\s+\n" } */
/* { dg-output "mmul mv dest        : 374 378 418 420\\s+\n" } */
/* { dg-output "mmul mx dest        : 17  18  19  20\\s+\n" } */
/* { dg-output "mmulh mm dest       : 8   14  18  20\\s+\n" } */
/* { dg-output "mmulh mv dest       : 6   10  18  20\\s+\n" } */
/* { dg-output "mmulh mx dest       : 2   4   6   8\\s+\n" } */
