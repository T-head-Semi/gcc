/* { dg-do compile } */
/* { dg-skip-if "-march given" { *-*-* } { "-march=*" } } */
/* { dg-options "-mcpu=e907f" } */
/* e907f = rv32imafcxtheade -mabi=ilp32f */

#if !((__riscv_xlen == 32)		   \
      && defined(__riscv_float_abi_single) \
      && defined(__riscv_mul)		   \
      && defined(__riscv_atomic)	   \
      && defined(__riscv_compressed)	   \
      && defined(__riscv_flen)		   \
      && defined(__riscv_xthead)	   \
      && defined(__riscv_xtheade))
#error "unexpected arch"
#endif
