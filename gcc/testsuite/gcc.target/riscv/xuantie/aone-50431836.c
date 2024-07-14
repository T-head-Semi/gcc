/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadc -mabi=lp64d -O2" } */

int a;

int foo(int *p)
{
  int ret;

  a = * p++;
  ret = (int)((long long)p);
  return ret;
}

/* { dg-final { scan-assembler "sext.w" } }*/
