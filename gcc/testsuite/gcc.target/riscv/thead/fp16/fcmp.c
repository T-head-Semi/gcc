/* { dg-do assemble } */
/* { dg-skip-if "test float16 insns" { *-*-* } { "*" } { "-march=*g*xtheadc*" "-march=*f*xtheadc*" } } */
/* { dg-options "--save-temps" } */

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
/* { dg-final { scan-assembler "flt.h" } }*/
/* { dg-final { scan-assembler "fle.h" } }*/
