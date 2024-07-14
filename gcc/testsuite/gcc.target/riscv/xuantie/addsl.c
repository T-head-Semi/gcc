/* { dg-do assemble } */
/* { dg-skip-if "test addsl insns" { *-*-* } { "*" } { "-march=*xtheadc*" "-march=*xtheade*" } } */
/* { dg-options "-O2 --save-temps" } */

unsigned long a;

void fun(unsigned long b,unsigned long c)
{
  a = b + (c << 2);
}

/* { dg-final { scan-assembler "addsl" } }*/

/* { dg-final { cleanup-saved-temps } } */

