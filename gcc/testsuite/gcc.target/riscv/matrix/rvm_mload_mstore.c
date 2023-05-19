/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv64*" } } */
/* { dg-options "-march=rv64gc_xtheadmatrix -mabi=lp64d -O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_mload_store_##TYPE(long stride, TYPE_T *x, TYPE_T *y, TYPE_T *x1, TYPE_T *y1) {  \
    m##TYPE_T temp1 = mld_##STYPE (x, stride);  \
    mst_##STYPE##_m##STYPE (y, stride, temp1);  \
    m##TYPE_T temp2 = msld_##STYPE (x, stride);  \
    msst_##STYPE##_m##STYPE (y, stride, temp2);  \
                                                \
    m##TYPE_T temp3 = mld1m_##STYPE (x1);  \
    mst1m_##STYPE##_m##STYPE (y1, temp3);  \
  }

RVM_ALL_TYPE_TEST(MTEST)

#define MTESTX2(TYPE_T, STYPE, TYPE)  \
  void test_mload_store_x2_##TYPE(TYPE_T *x, TYPE_T *y) {  \
    m##TYPE##x2_t temp3 = mld2m_##STYPE (x);  \
    mst2m_##STYPE##_m##STYPE##x2 (y, temp3);  \
  }

RVM_FLOAT_TYPE_TEST(MTESTX2)

/* { dg-final { scan-assembler-times "mldb\t" 2 } } */
/* { dg-final { scan-assembler-times "mstb\t" 2 } } */
/* { dg-final { scan-assembler-times "mldh\t" 3 } } */
/* { dg-final { scan-assembler-times "msth\t" 3 } } */
/* { dg-final { scan-assembler-times "mldw\t" 3 } } */
/* { dg-final { scan-assembler-times "mstw\t" 3 } } */
/* { dg-final { scan-assembler-times "mldd\t" 3 } } */
/* { dg-final { scan-assembler-times "mstd\t" 3 } } */
/* { dg-final { scan-assembler-times "msldb\t" 2 } } */
/* { dg-final { scan-assembler-times "msstb\t" 2 } } */
/* { dg-final { scan-assembler-times "msldh\t" 3 } } */
/* { dg-final { scan-assembler-times "mssth\t" 3 } } */
/* { dg-final { scan-assembler-times "msldw\t" 3 } } */
/* { dg-final { scan-assembler-times "msstw\t" 3 } } */
/* { dg-final { scan-assembler-times "msldd\t" 3 } } */
/* { dg-final { scan-assembler-times "msstd\t" 3 } } */
/* { dg-final { scan-assembler-times "mld1mb\t" 2 } } */
/* { dg-final { scan-assembler-times "mst1mb\t" 2 } } */
/* { dg-final { scan-assembler-times "mld1mh\t" 3 } } */
/* { dg-final { scan-assembler-times "mst1mh\t" 3 } } */
/* { dg-final { scan-assembler-times "mld1mw\t" 3 } } */
/* { dg-final { scan-assembler-times "mst1mw\t" 3 } } */
/* { dg-final { scan-assembler-times "mld1md\t" 3 } } */
/* { dg-final { scan-assembler-times "mst1md\t" 3 } } */
/* { dg-final { scan-assembler-times "mld2mh\t" 1 } } */
/* { dg-final { scan-assembler-times "mst2mh\t" 1 } } */
/* { dg-final { scan-assembler-times "mld2mw\t" 1 } } */
/* { dg-final { scan-assembler-times "mst2mw\t" 1 } } */
/* { dg-final { scan-assembler-times "mld2md\t" 1 } } */
/* { dg-final { scan-assembler-times "mst2md\t" 1 } } */