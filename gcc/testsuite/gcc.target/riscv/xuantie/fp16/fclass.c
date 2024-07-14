/* { dg-do assemble } */
/* { dg-skip-if "test float16 insns" { *-*-* } { "*" } { "-march=*zfh*" } } */
/* { dg-options "--save-temps" } */

/* Copy from gcc.dg/torture/float16-tg-3.c */
volatile _Float16 negtrue_min = -5.96046447753906250000000000000000000e-8F16;

int
main ()
{
  if (__builtin_fpclassify (0, 1, 4, 3, 2, (negtrue_min)) != (3))
    return -1;
  return 0;
}

/* { dg-final { scan-assembler "fclass.h" } }*/
