/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-march=rv64*" } } */
/* { dg-options "-march=rv64gc_xtheadmatrix -mabi=lp64d -O2" } */

#include <riscv_matrix.h>
#include <stddef.h>
#include "rvm-common.h"

void test_pmmaqa_mi8(mint32_t *temp, mint32_t a, mint8_t b, mint8_t c) { *temp = pmmaqa_mi8(a, b, c); }
void test_pmmaqau_mui8(mint32_t *temp, mint32_t a, muint8_t b, muint8_t c) { *temp = pmmaqau_mui8(a, b, c); }
void test_pmmaqaus_mui8_mi8(mint32_t *temp, mint32_t a, muint8_t b, mint8_t c) { *temp = pmmaqaus_mui8_mi8(a, b, c); }
void test_pmmaqasu_mi8_mui8(mint32_t *temp, mint32_t a, mint8_t b, muint8_t c) { *temp = pmmaqasu_mi8_mui8(a, b, c); }

/* { dg-final { scan-assembler-times "pmmaqa.b\t" 1 } } */
/* { dg-final { scan-assembler-times "pmmaqau.b\t" 1 } } */
/* { dg-final { scan-assembler-times "pmmaqaus.b\t" 1 } } */
/* { dg-final { scan-assembler-times "pmmaqasu.b\t" 1 } } */
