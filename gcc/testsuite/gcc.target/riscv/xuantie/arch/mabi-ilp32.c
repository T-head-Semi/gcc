/* { dg-do compile } */
/* { dg-options "-march=rv32ima" } */

#if !((__riscv_xlen == 32) && defined(__riscv_float_abi_soft))
#error "unexpected abi"
#endif
