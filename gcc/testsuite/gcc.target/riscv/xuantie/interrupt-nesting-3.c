/* Verify that csr instructions are not emitted.  */
/* { dg-do compile } */
/* { dg-options "-march=rv32ec_xtheadse -mabi=ilp32e -O" } */

void __attribute__ ((interrupt ("THead-interrupt-nesting")))
sub (void)
{
}

/* { dg-final { scan-assembler-times "csrr\tt0,mcause" 1 } } */
/* { dg-final { scan-assembler-times "csrr\tt0,mepc" 1 } } */
/* { dg-final { scan-assembler-times "csrr\tt0,mstatus" 1 } } */
/* { dg-final { scan-assembler-times "csrw\tmcause,t0" 1 } } */
/* { dg-final { scan-assembler-times "csrw\tmepc,t0" 1 } } */
/* { dg-final { scan-assembler-times "csrw\tmstatus,t0" 1 } } */
/* { dg-final { scan-assembler-times "csrsi" 1 } } */
