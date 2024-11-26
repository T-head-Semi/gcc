/* { dg-do link } */
/* { dg-options "-mcpu=c907fd" } */
/* c907 = rv64imafdc_zicbom_zicbop_zicboz_zicond_zihintntl_zihintpause_zawrs_zfa_zfbfmin_zfh_zcb_zba_zbb_zbc_zbs_svinval_svnapot_svpbmt_xtheadc -mabi=lp64d */

#if !((__riscv_xlen == 64)		   \
      && defined(__riscv_float_abi_double) \
      && defined(__riscv_mul)		   \
      && defined(__riscv_atomic)	   \
      && defined(__riscv_compressed)	   \
      && defined(__riscv_flen)		 \
      && defined(__riscv_zfbfmin)	   \
      && defined(__riscv_zicond)	   \
      && defined(__riscv_zicbom)	   \
      && defined(__riscv_zicbop)	   \
      && defined(__riscv_zicboz)	   \
      && defined(__riscv_svinval)	   \
      && defined(__riscv_svnapot)	   \
      && defined(__riscv_svpbmt)	   \
      && defined(__riscv_zihintpause)	   \
      && defined(__riscv_zihintntl)	   \
      && defined(__riscv_zawrs)	   \
      && defined(__riscv_zfh)		   \
      && defined(__riscv_zfa)		   \
       && defined(__riscv_zcb)		   \
       && defined(__riscv_zba)		   \
      && defined(__riscv_zbb)		   \
      && defined(__riscv_zbc)		   \
      && defined(__riscv_zbs)		   \
      && defined(__riscv_xthead)	   \
      && defined(__riscv_xtheadc))
#error "unexpected arch"
#endif

int main()
{
  return 0;
}
