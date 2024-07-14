/* { dg-do assemble } */
/* { dg-skip-if "test mula insns" { *-*-* } { "*" } { "-march=rv64*xtheadc*" "-march=rv64*xtheade*" } } */
/* { dg-options "-O2 --save-temps" } */

typedef unsigned char e_u8;
typedef e_u8 Byte;

#define FYR (19595)
#define FYG (38470)
#define FYB (7471)

Byte                    xr, xg, xb;
Byte                    xy;


int t_run_test()
{
  xy = (Byte) (((FYR * xr) + (FYG * xg) + (FYB * xb) +
                  (1 << (16 - 1))) >> 16);

}

/* { dg-final { scan-assembler-times "mulaw" 2} }*/

/* { dg-final { cleanup-saved-temps } } */

