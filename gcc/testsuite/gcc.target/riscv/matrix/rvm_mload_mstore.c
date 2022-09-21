/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mload_store_##TYPE(long stride, TYPE_T *x, TYPE_T *y) {  \
    m##TYPE_T temp1 = mld_##STYPE (x, stride);  \
    mst_##STYPE##_m##STYPE (y, stride, temp1);  \
    m##TYPE_T temp2 = msld_##STYPE (x, stride);  \
    msst_##STYPE##_m##STYPE (y, stride, temp2);  \
  }

#define MTESTX2(TYPE_T, STYPE, TYPE)  \
  void test_mload_store_x2_##TYPE(long stride, TYPE_T *x, TYPE_T *y, TYPE_T *z1, TYPE_T *z2) {  \
    m##TYPE##x2_t temp1 = mld_##STYPE##x2(x, y, stride);  \
    mst_##STYPE##x2_m##STYPE##x2(z1, z2, stride, temp1);  \
    m##TYPE##x2_t temp2 = msld_##STYPE##x2(x, y, stride);  \
    msst_##STYPE##x2_m##STYPE##x2(z1, z2, stride, temp2);  \
  }

RVM_ALL_TYPE_TEST(MTEST)
RVM_FLOAT_TYPE_TEST(MTESTX2)

/* { dg-final { scan-assembler-times "mldb\t" 2 } } */
/* { dg-final { scan-assembler-times "mstb\t" 2 } } */
/* { dg-final { scan-assembler-times "mldh\t" 5 } } */
/* { dg-final { scan-assembler-times "msth\t" 5 } } */
/* { dg-final { scan-assembler-times "mldw\t" 5 } } */
/* { dg-final { scan-assembler-times "mstw\t" 5 } } */
/* { dg-final { scan-assembler-times "mldd\t" 5 } } */
/* { dg-final { scan-assembler-times "mstd\t" 5 } } */
/* { dg-final { scan-assembler-times "msldb\t" 2 } } */
/* { dg-final { scan-assembler-times "msstb\t" 2 } } */
/* { dg-final { scan-assembler-times "msldh\t" 5 } } */
/* { dg-final { scan-assembler-times "mssth\t" 5 } } */
/* { dg-final { scan-assembler-times "msldw\t" 5 } } */
/* { dg-final { scan-assembler-times "msstw\t" 5 } } */
/* { dg-final { scan-assembler-times "msldd\t" 5 } } */
/* { dg-final { scan-assembler-times "msstd\t" 5 } } */
