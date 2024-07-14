/* { dg-do compile } */
/* { dg-skip-if "-mabi given" { *-*-* } { "-mabi=*" } } */
/* { dg-options "-march=rv32e" } */

#if !((__riscv_xlen == 32) && defined(__riscv_abi_rve))
#error "unexpected abi"
#endif
