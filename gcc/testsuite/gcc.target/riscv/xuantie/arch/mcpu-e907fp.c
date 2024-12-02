/* { dg-do link } */
/* { dg-options "-mcpu=e907fp" } */
/* e907fp = rv32imafcpzpsfoperand_xtheade -mabi=ilp32f */

#if !((__riscv_xlen == 32)		   \
      && defined(__riscv_float_abi_single) \
      && defined(__riscv_mul)		   \
      && defined(__riscv_atomic)	   \
      && defined(__riscv_compressed)	   \
      && defined(__riscv_flen)		   \
      && defined(__riscv_dsp)		   \
      && defined(__riscv_zpn)		   \
      && defined(__riscv_zpsfoperand)	   \
      && defined(__riscv_xthead)	   \
      && defined(__riscv_xtheade))
#error "unexpected arch"
#endif

int main()
{
  return 0;
}
