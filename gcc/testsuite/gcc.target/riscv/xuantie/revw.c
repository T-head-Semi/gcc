/* { dg-do assemble } */
/* { dg-skip-if "test revw insns" { *-*-* } { "*" } { "-march=*xtheadc*" } } */
/* { dg-options "-O2 --save-temps" } */

#define GET_ULONG_BE(n,b)                        \
{                                                \
    (n) = ( (unsigned long) (b)[0] << 24 )       \
        | ( (unsigned long) (b)[1] << 16 )       \
        | ( (unsigned long) (b)[2] <<  8 )       \
        | ( (unsigned long) (b)[3]       );      \
}

unsigned long W;

void process(const unsigned char data[32] )
{
    GET_ULONG_BE( W, data);
}

/* { dg-final { scan-assembler "revw" } }*/

/* { dg-final { cleanup-saved-temps } } */

