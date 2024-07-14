/* { dg-do compile } */
/* { dg-skip-if "-march given" { *-*-* } { "-march=*" } } */
/* { dg-options "-mcpu=c906" } */
/* c906 = rv64imacxtheadc -mabi=lp64*/

#if !((__riscv_xlen == 64)		 \
      && defined(__riscv_float_abi_soft) \
      && defined(__riscv_mul)		 \
      && defined(__riscv_atomic)	 \
      && defined(__riscv_compressed)	 \
      && defined(__riscv_xthead)	 \
      && defined(__riscv_xtheadc))
#error "unexpected arch"
#endif
