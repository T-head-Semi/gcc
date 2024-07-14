/* { dg-do assemble } */
/* { dg-options "-march=rv64gczfh -mabi=lp64d --save-temps" } */

_Float16 func(_Float16 a, _Float16 b)
{
  return a * b;
}

/* { dg-final { scan-assembler "fmul.h" } }*/
