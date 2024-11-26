/* { dg-do compile } */
/* { dg-options "-march=rv64imafdc_xtheadc -mabi=lp64d -O2 -mtune=c907" } */

long long  a1=2;
long long  a2=3;
long long  array[100]={1,2,3,4,5};
int cc=0;
int foo(void)
{
    a1=array[4];
    a2=array[3];
    cc = cc&0x7;
    return 2;
}

/* { dg-final { scan-assembler-times "ldd" 1 } } */