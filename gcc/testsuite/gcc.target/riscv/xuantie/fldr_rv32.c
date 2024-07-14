/* { dg-do assemble } */
/* { dg-options "-march=rv32imafc_xtheadc -mabi=ilp32f -O2 --save-temps" } */

float funcf(float *a, int b)
{
    return a[b];
}
/* { dg-final { scan-assembler "flrw" } } */

double funcd(double *a, int b)
{
    return a[b];
}
/* { dg-final { scan-assembler-not "flrd" } } */

float funcsf(float *a, int b, float c)
{
    a[b] = c;
    return a[b];
}
/* { dg-final { scan-assembler "fsrw" } } */

double funcsd(double *a, int b, double c)
{
    a[b] = c;
    return a[b];
}
/* { dg-final { scan-assembler "swd" } } */

/* { dg-final { cleanup-saved-temps } } */
