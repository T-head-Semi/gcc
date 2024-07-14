/* { dg-do assemble } */
/* { dg-skip-if "test float16 insns" { *-*-* } { "*" } { "-march=rv64*zfh*" } } */
/* { dg-options "--save-temps" } */

__fp16 func3(long a)
{
  return (__fp16)a;
}

__fp16 func4(unsigned long a)
{
  return (__fp16)a;
}

long func8(__fp16 a)
{
  return (long)a;
}

unsigned long func9(__fp16 a)
{
  return (unsigned long)a;
}

/* { dg-final { scan-assembler "fcvt.h.l" } }*/
/* { dg-final { scan-assembler "fcvt.h.lu" } }*/
/* { dg-final { scan-assembler "fcvt.l.h" } }*/
/* { dg-final { scan-assembler "fcvt.lu.h" } }*/
