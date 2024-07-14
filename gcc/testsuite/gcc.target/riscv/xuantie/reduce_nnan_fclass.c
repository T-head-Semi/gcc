/* { dg-do assemble } */
/* { dg-skip-if "test fclass.s insns" { *-*-* } { "*" } { "-march=*fd*xtheadc*" "-march=*fd*xtheade*" } } */
/* { dg-options "-O2 --save-temps" } */

extern void abort(void);
extern int a;

void 
foo(float x, float y)
{
  {
    if (__builtin_islessequal(x, y))
      abort();
  }
  {
    if (__builtin_islessequal(x, 0.0f))
      abort();
  }
  {
    if (__builtin_islessequal(1.0f, x))
      abort();
  }
}

void
loop1(float x)
{
  
  float y = 1.0f;
  for (int i = 1; i < 1024; i++)
    {
      if(__builtin_islessequal (x, y))
        abort();
      if(a == 1)
        y = 2.0f;
    }
}

void
loop2(float x)
{
  float y = 1.0f;
  for (int i = 1; i < 1024; i++)
    {
      if(__builtin_islessequal (x, y))
        abort();
      if(a == 1)
        y = __builtin_nanf ("");
    }
}

/* { dg-final { scan-assembler-times "fclass.s" 7 } } */
/* { dg-final { cleanup-saved-temps } } */