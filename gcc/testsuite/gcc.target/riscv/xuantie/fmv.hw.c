/* { dg-do assemble } */
/* { dg-skip-if "test fmv.hw insns" { *-*-* } { "-march=*zfa*" } { "" } } */
/* { dg-options "-march=rv32g_xtheadc -mabi=ilp32 --save-temps" } */

double func(long long a)
{
  return (double)(a + 3);
}

/* { dg-final { scan-assembler "fmv.hw.x" } }*/
/* { dg-final { scan-assembler "fmv.x.hw" } }*/

/* { dg-final { cleanup-saved-temps } } */
