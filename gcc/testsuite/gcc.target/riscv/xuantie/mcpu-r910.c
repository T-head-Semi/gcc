/* { dg-do compile } */
/* { dg-skip-if "-march given" { *-*-* } { "-march=*" } } */
/* { dg-options "-mcpu=r910" } */
/* r910 = rv64imafdcxtheadc -mabi=lp64d */

#if !((__riscv_xlen == 64)		   \
      && defined(__riscv_float_abi_double) \
      && defined(__riscv_mul)		   \
      && defined(__riscv_atomic)	   \
      && defined(__riscv_compressed)	   \
      && defined(__riscv_flen)		   \
      && defined(__riscv_xthead)	   \
      && defined(__riscv_xtheadc))
#error "unexpected arch"
#endif

