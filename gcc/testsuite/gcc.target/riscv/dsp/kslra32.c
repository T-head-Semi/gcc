
/* { dg-do assemble } */
/* { dg-skip-if "test P-ext insns" { *-*-* } { "*" } { "-march=rv64*p*" } } */
/* { dg-options "-O2 --save-temps" } */

#include <riscv-dsp.h>

uintXLEN_t test_kslra32(uintXLEN_t a, int32_t b) {
    return __rv__kslra32 (a, b);
}

/* { dg-final { scan-assembler "\tkslra32" } }*/

/* { dg-final { cleanup-saved-temps } } */
