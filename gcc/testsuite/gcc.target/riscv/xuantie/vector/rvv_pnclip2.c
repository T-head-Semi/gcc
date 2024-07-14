/* { dg-do compile } */
/* { dg-skip-if "test rvv" { *-*-* } { "*" } { "-march=rv*xtheadvdot*" } } */

#include <riscv_vector.h>
#include <stddef.h>
#include "../../vector/rvv-common.h"

#define RVV_NINT_SHIFT_BUILTIN_VEC_SCALAR_MASKED_TEST(STYPE, VCLASST, VCLASS, EM, MLEN,\
						      WSTYPE, WEM, STYPEC, SEW, WSEW, OP)\
  void rvv##OP##VCLASS##EM##_v_m_builtin_test(\
	 size_t n, WSTYPE *x, STYPE *y, uint##SEW##_t *z, word_type vl)\
  {								\
    v##VCLASST##WEM##_t vx;					\
    v##VCLASST##EM##_t vy;					\
    vuint##EM##_t vz;						\
    vbool##MLEN##_t mask;					\
    vx = VLOAD(VCLASS, WSEW, WEM, x);				\
    vy = VLOAD(VCLASS, SEW, EM, y);				\
    vz = VULOAD(SEW, EM, z);					\
    mask = MSET (MLEN);						\
    vy = v##OP##_wv_##VCLASS##EM##_m (mask, vy, vx, vz, vl);	\
    VSTORE(VCLASS, SEW, EM, y, vy);				\
  }								\
  RVV_NINT_BIN_BUILTIN_SCALAR_MASKED_TEST(STYPE, VCLASST, VCLASS, EM, MLEN, WSTYPE, WEM, STYPEC, SEW, WSEW, OP)

RVV_WINT_TEST_ARG(RVV_NINT_SHIFT_BUILTIN_VEC_SCALAR_MASKED_TEST, pnclip)
RVV_WUINT_TEST_ARG(RVV_NINT_SHIFT_BUILTIN_VEC_SCALAR_MASKED_TEST, pnclipu)

/* { dg-final { scan-assembler-times "vpnclip.wv" 15 } } */
/* { dg-final { scan-assembler-times "vpnclip.wx" 15 } } */
/* { dg-final { scan-assembler-times "vpnclipu.wv" 15 } } */
/* { dg-final { scan-assembler-times "vpnclipu.wx" 15 } } */
