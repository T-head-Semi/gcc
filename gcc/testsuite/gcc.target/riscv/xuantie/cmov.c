/* { dg-do assemble } */
/* { dg-skip-if "test condition mov insns" { *-*-* } { "*" } { "-march=rv64*xtheadc*" "-march=rv64*xtheade*" } } */
/* { dg-options "--save-temps -O2" } */

long long func (long long a, long long b, long long c, long long d)
{
  long long ret;
  if (a > b)
    ret = c + d;
  else
    ret = c - d;

  return ret;
}

/* { dg-final { scan-assembler "mveqz" } } */

long func2 (long a, long b, long c, long d)
{
  return (a == b) ? c : d;
}

/* { dg-final { scan-assembler "mveqz" } } */

long func3 (long a, long b, long c, long d)
{
  return (a != b) ? c : d;
}

/* { dg-final { scan-assembler "mvnez" } } */

/* { dg-final { cleanup-saved-temps } } */

