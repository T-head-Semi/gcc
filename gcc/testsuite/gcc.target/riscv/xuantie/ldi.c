/* { dg-do assemble } */
/* { dg-skip-if "test ldia/ldib and stia/stib insns" { *-*-* } { "*" } { "-march=rv64*xtheadc*" "-march=rv64*xtheade*" } } */
/* { dg-options "--save-temps -O2" } */

#include "addr-modes.h"

typedef signed char schar;
typedef unsigned short ushort;
typedef long long ll;

/* no special function attribute required */
#define ATTR /* */

PRE_STORE(char, ATTR)
/* { dg-final { scan-assembler "sbib.*1,0" } } */
PRE_STORE(short, ATTR)
/* { dg-final { scan-assembler "shib.*2,0" } } */
PRE_STORE(int, ATTR)
/* { dg-final { scan-assembler "swib.*4,0" } } */
PRE_STORE(ll, ATTR)
/* { dg-final { scan-assembler "sdib.*8,0" } } */

POST_STORE(char, ATTR)
/* { dg-final { scan-assembler "sbia.*1,0" } } */
POST_STORE(short, ATTR)
/* { dg-final { scan-assembler "shia.*2,0" } } */
POST_STORE(int, ATTR)
/* { dg-final { scan-assembler "swia.*4,0" } } */
POST_STORE(ll, ATTR)
/* { dg-final { scan-assembler "sdia.*8,0" } } */

PRE_LOAD(char, ATTR)
/* { dg-final { scan-assembler "lbuib.*1,0" } } */
PRE_LOAD(schar, ATTR)
/* { dg-final { scan-assembler "lbib.*1,0" } } */
PRE_LOAD(ushort, ATTR)
/* { dg-final { scan-assembler "lhuib.*2,0" } } */
PRE_LOAD(short, ATTR)
/* { dg-final { scan-assembler "lhib.*2,0" } } */
PRE_LOAD(int, ATTR)
/* { dg-final { scan-assembler "lwib.*4,0" } } */
PRE_LOAD(ll, ATTR)
/* { dg-final { scan-assembler "ldib.*8,0" } } */

POST_LOAD(char, ATTR)
/* { dg-final { scan-assembler "lbuia.*1,0" } } */
POST_LOAD(schar, ATTR)
/* { dg-final { scan-assembler "lbia.*1,0" } } */
POST_LOAD(ushort, ATTR)
/* { dg-final { scan-assembler "lhuia.*2,0" } } */
POST_LOAD(short, ATTR)
/* { dg-final { scan-assembler "lhia.*2,0" } } */
POST_LOAD(int, ATTR)
/* { dg-final { scan-assembler "lwia.*4,0" } } */
POST_LOAD(ll, ATTR)
/* { dg-final { scan-assembler "ldia.*8,0" } } */

/* { dg-final { scan-assembler-not "\taddi" } } */

/* { dg-final { cleanup-saved-temps } } */

