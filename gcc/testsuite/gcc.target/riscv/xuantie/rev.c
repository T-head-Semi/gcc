/* { dg-do assemble } */
/* { dg-skip-if "test rev insns" { *-*-* } { "*" } { "-march=*xtheadc*" "-march=*xtheade*" } } */
/* { dg-options "-O2 --save-temps" } */

#define GET_ULONGLONG_BE(n,b)                          \
{                                                      \
    (n) = ( (unsigned long long) (b)[0] << 56 )        \
        | ( (unsigned long long) (b)[1] << 48 )        \
        | ( (unsigned long long) (b)[2] << 40 )        \
        | ( (unsigned long long) (b)[3] << 32 )        \
        | ( (unsigned long long) (b)[4] << 24 )        \
        | ( (unsigned long long) (b)[5] << 16 )        \
        | ( (unsigned long long) (b)[6] <<  8 )        \
        | ( (unsigned long long) (b)[7]       );       \
}

unsigned long long W;

void process(const unsigned char data[64] )
{
    GET_ULONGLONG_BE( W, data);
}

/* { dg-final { scan-assembler "rev" } }*/

/* { dg-final { cleanup-saved-temps } } */

