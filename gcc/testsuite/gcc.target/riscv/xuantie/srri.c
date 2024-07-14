/* { dg-do assemble } */
/* { dg-skip-if "test srri insns" { *-*-* } { "*" } { "-march=rv64*xtheadc*" "-march=rv64*xtheade*" } } */
/* { dg-options "--save-temps" } */

#define  SHR(x,n) (x >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (64 - n)))

unsigned long w, x;


void fun()
{
  w = ROTR(x,1);
}

/* { dg-final { scan-assembler "srri" } }*/

/* { dg-final { cleanup-saved-temps } } */

