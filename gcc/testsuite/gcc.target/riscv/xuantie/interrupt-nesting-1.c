/* { dg-do compile } */
/* { dg-options "-march=rv32ec_xtheadse -mabi=ilp32e -O" } */

extern int interrupt_count;

void __attribute__ ((interrupt ("THead-interrupt-nesting")))
sub (void)
{
  interrupt_count++;
}

/* { dg-final { scan-assembler-times "csrr\tt0,mcause" 1 } } */
/* { dg-final { scan-assembler-times "csrr\tt0,mepc" 1 } } */
/* { dg-final { scan-assembler-times "csrr\tt0,mstatus" 1 } } */
/* { dg-final { scan-assembler-times "csrw\tmcause,t0" 1 } } */
/* { dg-final { scan-assembler-times "csrw\tmepc,t0" 1 } } */
/* { dg-final { scan-assembler-times "csrw\tmstatus,t0" 1 } } */
/* { dg-final { scan-assembler-times "csrsi" 1 } } */
