/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadc -mabi=lp64d -O2" } */

int a;

int foo(char *src, int size)
{
  char *start = src;
  char *dest = src;

  while (size)
    {
      if (size > 4 && src[0] == '\\')
	{
	  src++;
	  size--;
	}
      else
	{
	  *dest++ = *src++;
	  size--;
	}
    }
  return (dest - start);
}

/* { dg-final { scan-assembler "sext.w" } }*/
