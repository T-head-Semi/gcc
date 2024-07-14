/* { dg-do compile } */
/* { dg-skip-if "-mabi given" { *-*-* } { "-mabi=*" } } */
/* { dg-options "-mcpu=e906 -march=rv64imafd" } */

#if !((__riscv_xlen == 64) && defined(__riscv_float_abi_double))
#error "unexpected abi"
#endif
