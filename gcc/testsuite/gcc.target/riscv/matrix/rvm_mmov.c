/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mmov_##TYPE(m##TYPE_T *temp1, m##TYPE_T *temp2, m##TYPE_T *temp3, m##TYPE_T *temp4, m##TYPE_T x, TYPE_T y, uint8_t stride) {  \
    *temp1 = mmov_m##STYPE(x);  \
    *temp2 = mmov_m##STYPE##v(x, stride);  \
    *temp3 = mmov_m##STYPE##v(x, 2);  \
    *temp4 = mmov_##STYPE(y);  \
  }

#define MTESTX2(TYPE_T, STYPE, TYPE)  \
  void test_mmov_x2_##TYPE(m##TYPE##x2_t *temp, m##TYPE_T x, m##TYPE_T y) {  \
    *temp = mmov_m##STYPE##x2(x, y);  \
  }

RVM_ALL_TYPE_TEST(MTEST)
RVM_FLOAT_TYPE_TEST(MTESTX2)

/* { dg-final { scan-assembler-times "mmov.mm\t" 17 } } */
/* { dg-final { scan-assembler-times "mmov.mv.x\t" 11 } } */
/* { dg-final { scan-assembler-times "mmov.mv.i\t" 11 } } */
/* { dg-final { scan-assembler-times "mmov.mx\t" 11 } } */
