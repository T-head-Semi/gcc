/* { dg-do assemble } */
/* { dg-options "-march=rv64gczfh -mabi=lp64d -O2 --save-temps" } */

_Float16 func(_Float16 a, _Float16 b, _Float16 c)
{
  return a * b - c;
}

/* { dg-final { scan-assembler "fmsub.h" } }*/
