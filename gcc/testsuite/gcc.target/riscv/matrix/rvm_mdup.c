/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv64*" } } */
/* { dg-options "-march=rv64gc_xtheadmatrix -mabi=lp64d -O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mmov_##TYPE(m##TYPE_T *temp, TYPE_T x) {  \
    *temp = mdup_##STYPE(x);  \
  }

RVM_INT_TYPE_TEST(MTEST)

/* { dg-final { scan-assembler-times "mdupb.m.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mduph.m.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mdupw.m.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mdupd.m.x\t" 2 } } */
