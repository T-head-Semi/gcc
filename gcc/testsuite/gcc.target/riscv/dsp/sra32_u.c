
/* { dg-do assemble } */
/* { dg-skip-if "test P-ext insns" { *-*-* } { "*" } { "-march=rv64*p*" } } */
/* { dg-options "-O2 --save-temps" } */

#include <riscv-dsp.h>

uintXLEN_t test_sra32_u(uintXLEN_t a, uint32_t b) {
    return __rv__sra32_u (a, b);
}

/* { dg-final { scan-assembler "\tsra32.u" } }*/

/* { dg-final { cleanup-saved-temps } } */
