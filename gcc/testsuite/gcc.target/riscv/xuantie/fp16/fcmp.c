/* { dg-do assemble } */
/* { dg-options "-march=rv64gczfh -mabi=lp64d --save-temps" } */

int func1(_Float16 a, _Float16 b)
{
  return a == b;
}

int func2(_Float16 a, _Float16 b)
{
  return a <= b;
}

int func3(_Float16 a, _Float16 b)
{
  return a > b;
}

/* { dg-final { scan-assembler "feq.h" } }*/
/* { dg-final { scan-assembler "fle.h" } }*/
/* { dg-final { scan-assembler "fgt.h" } }*/
