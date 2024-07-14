/* { dg-do compile } */
/* { dg-options "-march=rv32imac -mabi=ilp32" } */

_Float16 func(_Float16 a, _Float16 b, _Float16 c)
{
  return a * b + c;
}

/* { dg-final { scan-assembler-not "call\t__addhf3" } } */
