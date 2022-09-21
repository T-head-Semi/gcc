/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_msra_##TYPE(m##TYPE_T *temp1, m##TYPE_T *temp2, m##TYPE_T *temp3, m##TYPE_T *temp4, m##TYPE_T x, mu##TYPE_T y, u##TYPE_T z, uint8_t stride) {  \
    *temp1 = msra_m##STYPE##_mu##STYPE(x, y);  \
    *temp2 = msra_m##STYPE##_mu##STYPE##v(x, y, stride);  \
    *temp3 = msra_m##STYPE##_mu##STYPE##v(x, y, 2);  \
    *temp4 = msra_m##STYPE##_u##STYPE(x, z);  \
  }

RVM_INT_SD_TYPE_TEST(MTEST)

/* { dg-final { scan-assembler-times "msra.s.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "msra.s.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "msra.s.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "msra.s.mx\t" 1 } } */
/* { dg-final { scan-assembler-times "msra.d.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "msra.d.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "msra.d.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "msra.d.mx\t" 1 } } */
