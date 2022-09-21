/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

void test_fmmacc_float16 (mfloat16_t *temp, mfloat16x2_t a, mfloat16_t b) {
  *temp = fmmacc_mf16x2_mf16(*temp, a, b);
}

void test_fmmacc_float32 (mfloat32_t *temp, mfloat32_t a, mfloat32_t b) {
  *temp = fmmacc_mf32(*temp, a, b);
}

void test_fmmacc_float64 (mfloat64x2_t *temp, mfloat64_t a, mfloat64_t b) {
  *temp = fmmacc_mf64(*temp, a, b);
}

/* { dg-final { scan-assembler-times "fmmacc.h\t" 1 } } */
/* { dg-final { scan-assembler-times "fmmacc.s\t" 1 } } */
/* { dg-final { scan-assembler-times "fmmacc.d\t" 1 } } */
