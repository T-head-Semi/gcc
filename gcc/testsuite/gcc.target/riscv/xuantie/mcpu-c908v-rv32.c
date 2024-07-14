/* { dg-do compile } */
/* { dg-skip-if "-march given" { *-*-* } { "-march=*" } } */
/* { dg-options "-mcpu=c908v-rv32" } */
/* c908v-rv32 = rv32imafdcv_zihintpause_zfh_zba_zbb_zbc_zbs_xtheadc -mabi=ilp32d */

#if !((__riscv_xlen == 32)		   \
      && defined(__riscv_float_abi_double) \
      && defined(__riscv_mul)		   \
      && defined(__riscv_atomic)	   \
      && defined(__riscv_compressed)	   \
      && defined(__riscv_flen)		   \
      && defined(__riscv_vector)	   \
      && defined(__riscv_zihintpause)	   \
      && defined(__riscv_zfh)		   \
      && defined(__riscv_zba)		   \
      && defined(__riscv_zbb)		   \
      && defined(__riscv_zbc)		   \
      && defined(__riscv_zbs)		   \
      && __riscv_v ==  1000000		   \
      && defined(__riscv_xthead)	   \
      && defined(__riscv_xtheadc))
#error "unexpected arch"
#endif
