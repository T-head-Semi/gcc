/* { dg-do assemble } */
/* { dg-skip-if "test fmv.hw insns" { *-*-* } { "*" } { "-march=rv32*g*xthead*" "-march=rv32*fd*xthead*" } } */
/* { dg-options "-mabi=ilp32 --save-temps" } */

double func(long long a)
{
  return (double)(a + 3);
}

/* { dg-final { scan-assembler "fmv.hw.x" } }*/
/* { dg-final { scan-assembler "fmv.x.hw" } }*/

/* { dg-final { cleanup-saved-temps } } */
