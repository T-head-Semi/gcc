/* { dg-do assemble } */
/* { dg-skip-if "test float16 insns" { *-*-* } { "*" } { "-march=*zfh*" } } */
/* { dg-options "--save-temps" } */

_Float16 func(_Float16 a, _Float16 b)
{
  return a / b;
}

/* { dg-final { scan-assembler "fdiv.h" } }*/
