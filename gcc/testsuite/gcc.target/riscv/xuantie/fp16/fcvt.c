/* { dg-do assemble } */
/* { dg-skip-if "test float16 insns" { *-*-* } { "*" } { "-march=*zfh*" } } */
/* { dg-options "--save-temps" } */

__fp16 func5(float a)
{
  return (__fp16)a;
}

__fp16 func6(int a)
{
  return (__fp16)a;
}

__fp16 func7(unsigned int a)
{
  return (__fp16)a;
}

float func10(__fp16 a)
{
  return (float)a;
}

int func11(__fp16 a)
{
  return (int)a;
}

unsigned int func12(__fp16 a)
{
  return (unsigned int)a;
}

/* { dg-final { scan-assembler "fcvt.h.s" } }*/
/* { dg-final { scan-assembler "fcvt.h.w" } }*/
/* { dg-final { scan-assembler "fcvt.h.wu" } }*/
/* { dg-final { scan-assembler "fcvt.s.h" } }*/
/* { dg-final { scan-assembler "fcvt.w.h" } }*/
/* { dg-final { scan-assembler "fcvt.wu.h" } }*/
