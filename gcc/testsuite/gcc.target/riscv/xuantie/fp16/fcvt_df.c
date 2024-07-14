/* { dg-do assemble } */
/* { dg-options "-march=rv64gczfh -mabi=lp64d --save-temps" } */

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
