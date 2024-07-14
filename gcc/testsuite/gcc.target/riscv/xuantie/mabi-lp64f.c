/* { dg-do compile } */
/* { dg-skip-if "-mabi given" { *-*-* } { "-mabi=*" } } */
/* { dg-options "-march=rv64imaf" } */

#if !((__riscv_xlen == 64) && defined(__riscv_float_abi_single))
#error "unexpected abi"
#endif
