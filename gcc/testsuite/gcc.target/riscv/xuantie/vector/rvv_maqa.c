/* { dg-do compile } */
/* { dg-skip-if "test rvv" { *-*-* } { "*" } { "-march=rv*xtheadvdot*" } } */

#include <riscv_vector.h>
#include <stddef.h>
#include "../../vector/rvv-common.h"

#define RVV_QINT_SEW_TEST_ARG(MACRO, ...)					\
  MACRO( int8_t, int, i,  8mf2, 16,int32_t, 32mf2,x,  8, 32, __VA_ARGS__)	\
  MACRO( int8_t, int, i,  8m1,  8, int32_t, 32m1, x,  8, 32, __VA_ARGS__)	\
  MACRO( int8_t, int, i,  8m2,  4, int32_t, 32m2, x,  8, 32, __VA_ARGS__)	\
  MACRO( int8_t, int, i,  8m4,  2, int32_t, 32m4, x,  8, 32, __VA_ARGS__)	\
  MACRO( int8_t, int, i,  8m8,  1, int32_t, 32m8, x,  8, 32, __VA_ARGS__)	\

/* Takes the scalar type STYPE, vector class VCLASS (int or float), and
   the e and m value.  */
#define VWMAC_VV(STYPE, VCLASST, VCLASS, EM, MLEN, WSTYPE, WEM, STYPEC, SEW, WSEW, OP, OP0U, OP1U, OP2U, OP0T, OP1T, OP2T)   \
  void v##OP##VCLASS##EM##OP0U##_##OP1U##_##OP2U(size_t n,                     \
                                                 OP1T##STYPE *op1,             \
                                                 OP2T##STYPE *op2,             \
                                                 OP0T##WSTYPE * op0, word_type vl) {         \
    v##OP1T##int##EM##_t vop1;                                           \
    v##OP2T##int##EM##_t vop2;                                           \
    v##OP0T##int##WEM##_t vop0;                                          \
    vop0 = VLOAD(OP0U, WSEW, WEM, op0);                                    \
    vop1 = VLOAD(OP1U, SEW, EM, op1);                                     \
    vop2 = VLOAD(OP2U, SEW, EM, op2);                                     \
    vop0 = v##OP##_vv_##OP0U##WEM (vop0, vop1, vop2, vl);                          \
    VSTORE(OP0U, WSEW, WEM, op0, vop0);                                     \
  }

#define VWMAC_VX(STYPE, VCLASST, VCLASS, EM, MLEN, WSTYPE, WEM, STYPEC, SEW, WSEW, OP, OP0U, OP1U, OP2U, OP0T, OP1T, OP2T)   \
  void x##OP##VCLASS##EM##OP0U##_##OP1U##_##OP2U(size_t n,                     \
                                                 OP1T##WSTYPE  op1,             \
                                                 OP2T##STYPE *op2,             \
                                                 OP0T##WSTYPE * op0, word_type vl) {         \
    v##OP2T##int##EM##_t vop2;                                           \
    v##OP0T##int##WEM##_t vop0;                                          \
    vop0 = VLOAD(OP0U, WSEW, WEM, op0);                                    \
    vop2 = VLOAD(OP2U, SEW, EM, op2);                                     \
    vop0 = v##OP##_vx_##OP0U##WEM (vop0, op1, vop2, vl);		          \
    VSTORE(OP0U, WSEW, WEM, op0, vop0);                                     \
  }

#define VWMAC(STYPE, VCLASST, VCLASS, EM, MLEN, WSTYPE, WEM, STYPEC, SEW, WSEW, OP, OP0U, OP1U, OP2U, OP0T, OP1T, OP2T)   \
  VWMAC_VV(STYPE, VCLASST, VCLASS, EM, MLEN, WSTYPE, WEM, STYPEC, SEW, WSEW, OP, OP0U, OP1U, OP2U, OP0T, OP1T, OP2T)      \
  VWMAC_VX(STYPE, VCLASST, VCLASS, EM, MLEN, WSTYPE, WEM, STYPEC, SEW, WSEW, OP, OP0U, OP1U, OP2U, OP0T, OP1T, OP2T)

RVV_QINT_SEW_TEST_ARG(VWMAC,      maqa, i, i, i,  ,  ,  )
RVV_QINT_SEW_TEST_ARG(VWMAC,     maqau, u, u, u, u, u, u)
RVV_QINT_SEW_TEST_ARG(VWMAC,    maqasu, i, i, u,  ,  , u)
RVV_QINT_SEW_TEST_ARG(VWMAC_VX, maqaus, i, u, i,  , u,  )

/* { dg-final { scan-assembler-times "vmaqa.vv" 5 } } */
/* { dg-final { scan-assembler-times "vmaqa.vx" 5 } } */
/* { dg-final { scan-assembler-times "vmaqau.vv" 5 } } */
/* { dg-final { scan-assembler-times "vmaqau.vx" 5 } } */
/* { dg-final { scan-assembler-times "vmaqasu.vv" 5 } } */
/* { dg-final { scan-assembler-times "vmaqasu.vx" 5 } } */
/* { dg-final { scan-assembler-times "vmaqaus.vx" 5 } } */

RVV_QINT_SEW_TEST_ARG(VWMAC,      pmaqa, i, i, i,  ,  ,  )
RVV_QINT_SEW_TEST_ARG(VWMAC,     pmaqau, u, u, u, u, u, u)
RVV_QINT_SEW_TEST_ARG(VWMAC,    pmaqasu, i, i, u,  ,  , u)
RVV_QINT_SEW_TEST_ARG(VWMAC_VX, pmaqaus, i, u, i,  , u,  )

/* { dg-final { scan-assembler-times "vpmaqa.vv" 5 } } */
/* { dg-final { scan-assembler-times "vpmaqa.vx" 5 } } */
/* { dg-final { scan-assembler-times "vpmaqau.vv" 5 } } */
/* { dg-final { scan-assembler-times "vpmaqau.vx" 5 } } */
/* { dg-final { scan-assembler-times "vpmaqasu.vv" 5 } } */
/* { dg-final { scan-assembler-times "vpmaqasu.vx" 5 } } */
/* { dg-final { scan-assembler-times "vpmaqaus.vx" 5 } } */
