/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv64*" } } */
/* { dg-options "-march=rv64gc_xtheadmatrix -mabi=lp64d -O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mmov_##TYPE(m##TYPE_T *temp1, m##TYPE_T *temp2, m##TYPE_T *temp3, m##TYPE_T x, uint8_t stride) {  \
    *temp1 = mmov_m##STYPE(x);  \
    *temp2 = mmov_m##STYPE##v(x, stride);  \
    *temp3 = mmov_m##STYPE##v(x, 2);  \
  }

#define MTEST_MX_XM(TYPE_T, STYPE, TYPE)  \
  void test_mmov_mx_xm_##TYPE(m##TYPE_T *temp1, TYPE_T *temp2, TYPE_T x, m##TYPE_T y, uint8_t stride) {  \
    *temp1 = mmov_mx_##STYPE(*temp1, x, stride);  \
    *temp2 = mmov_xm_##STYPE(y, stride);  \
  }

#define MTESTX2(TYPE_T, STYPE, TYPE)  \
  void test_mmov_x2_##TYPE(m##TYPE##x2_t *temp, m##TYPE_T x, m##TYPE_T y) {  \
    *temp = mmov_m##STYPE##x2(x, y);  \
  }

RVM_ALL_TYPE_TEST(MTEST)
RVM_INT_TYPE_TEST(MTEST_MX_XM)
RVM_FLOAT_TYPE_TEST(MTESTX2)

/* { dg-final { scan-assembler-times "mmov.mm\t" 17 } } */
/* { dg-final { scan-assembler-times "mmov.mv.x\t" 11 } } */
/* { dg-final { scan-assembler-times "mmov.mv.i\t" 11 } } */
/* { dg-final { scan-assembler-times "mmovb.m.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovh.m.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovw.m.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovd.m.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovb.x.m\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovh.x.m\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovw.x.m\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovd.x.m\t" 2 } } */
