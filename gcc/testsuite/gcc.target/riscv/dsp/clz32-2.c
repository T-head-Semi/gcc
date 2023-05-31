/* { dg-do compile } */
/* { dg-options "-march=rv32gc_zbb -mabi=ilp32 -O2" } */

unsigned int
clzl (unsigned int i)
{
  return __builtin_clz (i);
}

/* { dg-final { scan-assembler-times "clz32\t" 1 } } */

