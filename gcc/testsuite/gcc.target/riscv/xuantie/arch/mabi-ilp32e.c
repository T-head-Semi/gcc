/* { dg-do compile } */
/* { dg-options "-march=rv32e" } */

#if !((__riscv_xlen == 32) && defined(__riscv_abi_rve))
#error "unexpected abi"
#endif
