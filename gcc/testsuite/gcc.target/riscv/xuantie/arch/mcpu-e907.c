/* { dg-do link } */
/* { dg-options "-mcpu=e907" } */
/* e907 = rv32imacxtheade -mabi=ilp32 */

#if !((__riscv_xlen == 32)		 \
      && defined(__riscv_float_abi_soft) \
      && defined(__riscv_mul)		 \
      && defined(__riscv_atomic)	 \
      && defined(__riscv_compressed)	 \
      && defined(__riscv_xthead)	 \
      && defined(__riscv_xtheade))
#error "unexpected arch"
#endif

int main()
{
  return 0;
}
