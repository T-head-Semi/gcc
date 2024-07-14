/* { dg-do assemble } */
/* { dg-skip-if "test srriw insns" { *-*-* } { "*" } { "-march=*xtheadc*" } } */
/* { dg-options " -O2 --save-temps" } */

#define  SHR(x,n) (x >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))

unsigned int w, x;

void fun()
{
  w = ROTR(x,1);
}

/* { dg-final { scan-assembler "srriw" } }*/

/* { dg-final { cleanup-saved-temps } } */

