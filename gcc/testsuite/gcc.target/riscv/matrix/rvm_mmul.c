/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mmul_##TYPE(m##TYPE_T *temp1, m##TYPE_T *temp2, m##TYPE_T *temp3, m##TYPE_T *temp4, m##TYPE_T x, m##TYPE_T y, TYPE_T z, uint8_t stride) {  \
    *temp1 = mmul_m##STYPE(x, y);  \
    *temp2 = mmul_m##STYPE##_m##STYPE##v(x, y, stride);  \
    *temp3 = mmul_m##STYPE##_m##STYPE##v(x, y, 2);  \
    *temp4 = mmul_m##STYPE##_##STYPE(x, z);  \
  }

RVM_INT_SD_TYPE_TEST(MTEST)


/* { dg-final { scan-assembler-times "mmul.s.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul.s.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul.s.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul.s.mx\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul.d.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul.d.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul.d.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mmul.d.mx\t" 1 } } */
