/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv*xtheadmatrix*" } } */
/* { dg-options "-O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

void test_mmaqa_mi8(mint32_t *temp, mint32_t a, mint8_t b, mint8_t c) { *temp = mmaqa_mi8(a, b, c); }
void test_mmaqa_mi16(mint64_t *temp, mint64_t a, mint16_t b, mint16_t c) { *temp = mmaqa_mi16(a, b, c); }
void test_mmaqau_mui8(mint32_t *temp, mint32_t a, muint8_t b, muint8_t c) { *temp = mmaqau_mui8(a, b, c); }
void test_mmaqau_mui16(mint64_t *temp, mint64_t a, muint16_t b, muint16_t c) { *temp = mmaqau_mui16(a, b, c); }
void test_mmaqaus_mui8_mi8(mint32_t *temp, mint32_t a, muint8_t b, mint8_t c) { *temp = mmaqaus_mui8_mi8(a, b, c); }
void test_mmaqaus_mui16_mi16(mint64_t *temp, mint64_t a, muint16_t b, mint16_t c) { *temp = mmaqaus_mui16_mi16(a, b, c); }
void test_mmaqasu_mi8_mui8(mint32_t *temp, mint32_t a, mint8_t b, muint8_t c) { *temp = mmaqasu_mi8_mui8(a, b, c); }
void test_mmaqasu_mi16_mui16(mint64_t *temp, mint64_t a, mint16_t b, muint16_t c) { *temp = mmaqasu_mi16_mui16(a, b, c); }

/* { dg-final { scan-assembler-times "mmaqa.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqa.h\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqau.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqau.h\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqaus.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqaus.h\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqasu.b\t" 1 } } */
/* { dg-final { scan-assembler-times "mmaqasu.h\t" 1 } } */
