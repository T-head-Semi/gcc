/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv64*" } } */
/* { dg-options "-march=rv64gc_xtheadmatrix -mabi=lp64d -O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mn4clip_##TYPE(mint8_t *temp1, mint8_t *temp2, mint8_t *temp3, mint8_t *temp4, m##TYPE_T x, mu##TYPE_T y, u##TYPE_T z, uint8_t stride) {  \
    *temp1 = mn4clip_m##STYPE##_mu##STYPE(x, y);  \
    *temp2 = mn4clip_m##STYPE##_mu##STYPE##v(x, y, stride);  \
    *temp3 = mn4clip_m##STYPE##_mu##STYPE##v(x, y, 2);  \
    *temp4 = mn4clip_m##STYPE##_u##STYPE(x, z);  \
  }

RVM_INT_SD_TYPE_TEST(MTEST)

/* { dg-final { scan-assembler-times "mn4clip.s.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clip.s.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clip.s.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clip.s.mx\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clip.d.mm\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clip.d.mv.x\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clip.d.mv.i\t" 1 } } */
/* { dg-final { scan-assembler-times "mn4clip.d.mx\t" 1 } } */
