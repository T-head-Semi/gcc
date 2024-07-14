/* { dg-do assemble } */
/* { dg-skip-if "test float16 insns" { *-*-* } { "*" } { "-march=*zfh*" } } */
/* { dg-options "--save-temps" } */

double func1(__fp16 a)
{
  return (double)a;
}

__fp16 func2(double a)
{
  return (__fp16)a;
}

/* { dg-final { scan-assembler "fcvt.d.h" } }*/
/* { dg-final { scan-assembler "fcvt.h.d" } }*/
