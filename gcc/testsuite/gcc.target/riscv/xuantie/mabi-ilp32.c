/* { dg-do compile } */
/* { dg-skip-if "-mabi given" { *-*-* } { "-mabi=*" } } */
/* { dg-options "-march=rv32ima" } */

#if !((__riscv_xlen == 32) && defined(__riscv_float_abi_soft))
#error "unexpected abi"
#endif
