/* { dg-do compile } */
/* { dg-do compile } */
/* { dg-skip-if "-mabi given" { *-*-* } { "-mabi=*" } } */
/* { dg-options "-march=rv32g" } */

#if !((__riscv_xlen == 32) && defined(__riscv_float_abi_double))
#error "unexpected abi"
#endif
