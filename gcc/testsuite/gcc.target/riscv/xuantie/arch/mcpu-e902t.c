/* { dg-do link } */
/* { dg-options "-mcpu=e902t" } */
/* e902t = rv32ecxtheadse -mabi=ilp32e */

#if !((__riscv_xlen == 32)		\
      && defined(__riscv_32e)		\
      && defined(__riscv_abi_rve)	\
      && defined(__riscv_compressed)	\
      && defined(__riscv_xthead)	\
      && defined(__riscv_xtheadse))
#error "unexpected arch"
#endif

int main()
{
  return 0;
}
