/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

void test_mcfg(uint32_t *temp1, uint32_t *temp2, uint32_t *temp3, uint32_t *temp4,
    uint32_t *temp5,uint32_t *temp6, uint32_t *temp7, uint32_t msize,
    uint8_t mm, uint8_t mn, uint16_t mk){
  *temp1 = mcfg(msize);
  *temp2 = mcfgm(mm);
  *temp3 = mcfgn(mn);
  *temp4 = mcfgk(mk);
  *temp5 = mcfgm(2);
  *temp6 = mcfgn(2);
  *temp7 = mcfgk(4);
}

#define MTEST(TYPE_T, STYPE, TYPE)  \
  void test_zero_##TYPE(m##TYPE_T *temp) {  \
    *temp = mzero_m##STYPE ();  \
  }

#define MTESTX2(TYPE_T, STYPE, TYPE)  \
  void test_zero_x2_##TYPE(m##TYPE##x2_t *temp) {  \
    *temp = mzero_m##STYPE##x2 ();  \
  }


RVM_ALL_TYPE_TEST(MTEST)
RVM_FLOAT_TYPE_TEST(MTESTX2)

void test_release(){
  mrelease();
}

/* { dg-final { scan-assembler-times "mcfg\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgm\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgmi\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgn\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgni\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgk\t" 1 } } */
/* { dg-final { scan-assembler-times "mcfgki\t" 1 } } */
/* { dg-final { scan-assembler-times "mrelease" 1 } } */
/* { dg-final { scan-assembler-times "mzero\t" 17 } } */
