/* { dg-do assemble } */
/* { dg-skip-if "test ipush/ipop insns" { *-*-* } { "*" } { "-march=*xtheade*" } } */
/* { dg-options "--save-temps -O2" } */

extern void f(void);

__attribute__((interrupt)) void func(void)
{
  f ();
}

/* { dg-final { scan-assembler "ipush" } }*/
/* { dg-final { scan-assembler "ipop" } }*

/* { dg-final { cleanup-saved-temps } } */
