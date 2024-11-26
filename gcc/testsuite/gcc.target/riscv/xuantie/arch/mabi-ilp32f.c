/* { dg-do compile } */
/* { dg-do compile } */
/* { dg-options "-march=rv32imaf" } */

#if !((__riscv_xlen == 32) && defined(__riscv_float_abi_single))
#error "unexpected abi"
#endif
