/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv64*" } } */
/* { dg-options "-march=rv64gc_xtheadmatrix -mabi=lp64d -O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mn4clipu_##TYPE(muint8_t *temp1, muint8_t *temp2, muint8_t *temp3, muint8_t *temp4, m##TYPE_T x, m##TYPE_T y, TYPE_T z, uint8_t stride) {  \
    *temp1 = mn4clipu_m##STYPE(x, y);  \
    *temp2 = mn4clipu_m##STYPE##_m##STYPE##v(x, y, stride);  \
    *temp3 = mn4clipu_m##STYPE##_m##STYPE##v(x, y, 2);  \
    *temp4 = mn4clipu_m##STYPE##_##STYPE(x, z);  \
  }

RVM_INT_USD_TYPE_TEST(MTEST)

/* { dg-final { scan-assembler-times "mn4clipu.s.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clipu.s.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clipu.s.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clipu.s.mx\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clipu.d.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clipu.d.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clipu.d.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clipu.d.mx\t" 1 } } */
