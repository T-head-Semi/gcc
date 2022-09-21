/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

void test_fwmmacc_mf16 (mfloat32_t *temp, mfloat16_t a, mfloat16_t b) {
  *temp = fwmmacc_mf16(*temp, a, b);
}

void test_fwmmacc_mf32 (mfloat64x2_t *temp, mfloat32_t a, mfloat32_t b) {
  *temp = fwmmacc_mf32(*temp, a, b);
}

/* { dg-final { scan-assembler-times "fwmmacc.h\t" 1 } } */
/* { dg-final { scan-assembler-times "fwmmacc.s\t" 1 } } */
