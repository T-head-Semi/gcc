/* Verify that fcsr instructions emitted.  */
/* { dg-do compile } */
/* { dg-options "-march=rv32gcp -mabi=ilp32d -O" } */

extern int foo (void);

void __attribute__ ((interrupt))
sub (void)
{
  foo ();
}

/* { dg-final { scan-assembler-times "csrr\tt0,vxsat" 1 } } */
/* { dg-final { scan-assembler-times "csrw\tvxsat,t0" 1 } } */
