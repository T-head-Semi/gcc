/* RISC-V Martix extension instructions include file.

   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by T-HEAD.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _GCC_RISCV_MATRIX_H
#define _GCC_RISCV_MATRIX_H 1

#include <stdint.h>
#include <stddef.h>

#ifndef __riscv_matrix
#error "Matrix intrinsics require the matrix extension."
#else

typedef __fp16 float16_t;
typedef float float32_t;
typedef double float64_t;

typedef __fp16 __float16_t;
typedef float __float32_t;
typedef double __float64_t;

enum RVM_CSR
{
  RVM_XMRSTART = 0,
  RVM_XMCSR,
  RVM_XMSIZE,
  RVM_XMLENB,
  RVM_XRLENB,
  RVM_XMISA,
  RVM_NULL
};

__extension__ extern __inline
__attribute__ ((__always_inline__, __gnu_inline__, __artificial__))
unsigned long mread_csr(enum RVM_CSR csr)
{
  unsigned long rm = 0;
  switch (csr)
    {
    case RVM_XMRSTART:
      __asm__ __volatile__ ("csrr\t%0,xmrstart" : "=r"(rm) : : "memory");
      break;
    case RVM_XMCSR:
      __asm__ __volatile__ ("csrr\t%0,xmcsr" : "=r"(rm) : : "memory");
      break;
    case RVM_XMSIZE:
      __asm__ __volatile__ ("csrr\t%0,xmsize" : "=r"(rm) : : "memory");
      break;
    case RVM_XMLENB:
      __asm__ __volatile__ ("csrr\t%0,xmlenb" : "=r"(rm) : : "memory");
      break;
    case RVM_XRLENB:
      __asm__ __volatile__ ("csrr\t%0,xrlenb" : "=r"(rm) : : "memory");
      break;
    case RVM_XMISA:
      __asm__ __volatile__ ("csrr\t%0,xmisa" : "=r"(rm) : : "memory");
      break;
    }
  return rm;
}

__extension__ extern __inline
__attribute__((__always_inline__, __gnu_inline__, __artificial__))
void mwrite_csr(enum RVM_CSR csr, unsigned long value)
{
  switch (csr)
    {
    case RVM_XMRSTART:
      __asm__ __volatile__ ("csrw\txmrstart,%z0" : : "rJ"(value) : "memory");
      break;
    case RVM_XMCSR:
      __asm__ __volatile__ ("csrw\txmcsr,%z0" : : "rJ"(value) : "memory");
      break;
    case RVM_XMSIZE:
      __asm__ __volatile__ ("csrw\txmsize,%z0" : : "rJ"(value) : "memory");
      break;
    }
}

__extension__ extern __inline uint32_t
__attribute__((__always_inline__, __gnu_inline__, __artificial__))
mcfg(uint32_t msize)
{
  return __builtin_riscv_matrix_mcfg(msize);
}

__extension__ extern __inline uint32_t
__attribute__((__always_inline__, __gnu_inline__, __artificial__))
mcfgm(uint8_t m)
{
  return __builtin_riscv_matrix_mcfgm(m);
}

__extension__ extern __inline uint32_t
__attribute__((__always_inline__, __gnu_inline__, __artificial__))
mcfgn(uint8_t n)
{
  return __builtin_riscv_matrix_mcfgn(n);
}

__extension__ extern __inline uint32_t
__attribute__((__always_inline__, __gnu_inline__, __artificial__))
mcfgk(uint16_t k)
{
  return __builtin_riscv_matrix_mcfgk(k);
}

__extension__ extern __inline void
__attribute__((__always_inline__, __gnu_inline__, __artificial__))
mrelease(void)
{
  __builtin_riscv_matrix_mrelease();
}

#define _RVM_ALL_TYPE_ITERATOR(MACRO, ...) \
  MACRO(int8_t, i8, int8)                  \
  MACRO(int16_t, i16, int16)               \
  MACRO(int32_t, i32, int32)               \
  MACRO(int64_t, i64, int64)               \
  MACRO(uint8_t, ui8, uint8)               \
  MACRO(uint16_t, ui16, uint16)            \
  MACRO(uint32_t, ui32, uint32)            \
  MACRO(uint64_t, ui64, uint64)            \
  MACRO(float16_t, f16, float16)           \
  MACRO(float32_t, f32, float32)           \
  MACRO(float64_t, f64, float64)

#define _RVM_INT_TYPE_ITERATOR(MACRO, ...) \
  MACRO(int8_t, i8, int8)                  \
  MACRO(int16_t, i16, int16)               \
  MACRO(int32_t, i32, int32)               \
  MACRO(int64_t, i64, int64)               \
  MACRO(uint8_t, ui8, uint8)               \
  MACRO(uint16_t, ui16, uint16)            \
  MACRO(uint32_t, ui32, uint32)            \
  MACRO(uint64_t, ui64, uint64)

#define _RVM_FLOAT_TYPE_ITERATOR(MACRO, ...) \
  MACRO(float16_t, f16, float16)             \
  MACRO(float32_t, f32, float32)             \
  MACRO(float64_t, f64, float64)

#define _RVM_FLOATX2_TYPE_ITERATOR(MACRO, ...) \
  MACRO(float16x2_t, f16x2, float16x2)             \
  MACRO(float32x2_t, f32x2, float32x2)             \
  MACRO(float64x2_t, f64x2, float64x2)

#define _RVM_INT_SD_TYPE_ITERATOR(MACRO, ...) \
  MACRO(int32_t, i32, int32)                  \
  MACRO(int64_t, i64, int64)

#define __MATRIX_UNDEFINED_ZERO(TYPE_T, STYPE, TYPE)   \
__extension__ extern __inline m##TYPE_T  \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))  \
mundefined_m##STYPE(void) \
{ \
  return __builtin_riscv_matrix_mundefined_m##STYPE(); \
} \
__extension__ extern __inline m##TYPE_T  \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))  \
mzero_m##STYPE(void) \
{ \
  return __builtin_riscv_matrix_mzero_m##STYPE(); \
}

_RVM_INT_TYPE_ITERATOR(__MATRIX_UNDEFINED_ZERO)
_RVM_FLOAT_TYPE_ITERATOR(__MATRIX_UNDEFINED_ZERO)
_RVM_FLOATX2_TYPE_ITERATOR(__MATRIX_UNDEFINED_ZERO)

#define __MATRIX_LOAD_STORE(TYPE_T, STYPE, TYPE)                         \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mld_##STYPE(TYPE_T *a, long b)                                     \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    return __builtin_riscv_matrix_mld_si_##TYPE(a, b);                 \
  else                                                                 \
    return __builtin_riscv_matrix_mld_di_##TYPE(a, b);                 \
}                                                                      \
								\
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
msld_##STYPE(TYPE_T *a, long b)                                    \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    return __builtin_riscv_matrix_msld_si_##TYPE(a, b);                \
  else                                                                 \
    return __builtin_riscv_matrix_msld_di_##TYPE(a, b);                \
}                                                                      \
								\
__extension__ extern __inline void                                     \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mst_##STYPE##_m##STYPE(TYPE_T *a, long b, m##TYPE_T c)             \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    __builtin_riscv_matrix_mst_si_##TYPE(c, a, b);                     \
  else                                                                 \
    __builtin_riscv_matrix_mst_di_##TYPE(c, a, b);                     \
}                                                                      \
__extension__ extern __inline void                                     \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
msst_##STYPE##_m##STYPE(TYPE_T *a, long b, m##TYPE_T c)            \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    __builtin_riscv_matrix_msst_si_##TYPE(c, a, b);                    \
  else                                                                 \
    __builtin_riscv_matrix_msst_di_##TYPE(c, a, b);                    \
}                                                                      \
  \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mld1m_##STYPE(TYPE_T *a)                                     \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    return __builtin_riscv_matrix_mld1m_si_##TYPE(a);                 \
  else                                                                 \
    return __builtin_riscv_matrix_mld1m_di_##TYPE(a);                 \
}                                                                      \
  \
__extension__ extern __inline void                                     \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mst1m_##STYPE##_m##STYPE(TYPE_T *a, m##TYPE_T b)             \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    __builtin_riscv_matrix_mst1m_si_##TYPE(b, a);                     \
  else                                                                 \
    __builtin_riscv_matrix_mst1m_di_##TYPE(b, a);                     \
} \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmov_m##STYPE(m##TYPE_T a)                                         \
{                                                                      \
  return __builtin_riscv_matrix_mmov_mm_##TYPE(a);                     \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmov_m##STYPE##v(m##TYPE_T a, uint8_t b)                           \
{                                                                      \
  return __builtin_riscv_matrix_mmov_mv_##TYPE(a, b);                  \
}

#define __MATRIX_FLOAT_X2(TYPE_T, STYPE, TYPE)                                                                   \
__extension__ extern __inline m##TYPE##x2_t __attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mld_##STYPE##x2(TYPE_T *a, TYPE_T *b, long c)                                                                  \
{                                                                                                              \
  if (__riscv_xlen == 32)                                                                                      \
    return __builtin_riscv_matrix_mld_si_x2_##TYPE(a, b, c);                                                   \
  else                                                                                                         \
    return __builtin_riscv_matrix_mld_di_x2_##TYPE(a, b, c);                                                   \
}                                                                                                              \
__extension__ extern __inline m##TYPE##x2_t __attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
msld_##STYPE##x2(TYPE_T *a, TYPE_T *b, long c)                                                                 \
{                                                                                                              \
  if (__riscv_xlen == 32)                                                                                      \
    return __builtin_riscv_matrix_msld_si_x2_##TYPE(a, b, c);                                                  \
  else                                                                                                         \
    return __builtin_riscv_matrix_msld_di_x2_##TYPE(a, b, c);                                                  \
} \
__extension__ extern __inline m##TYPE##x2_t                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mld2m_##STYPE(TYPE_T *a)                                     \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    return __builtin_riscv_matrix_mld2m_si_##TYPE(a);                 \
  else                                                                 \
    return __builtin_riscv_matrix_mld2m_di_##TYPE(a);                 \
}                                                                     \
__extension__ extern __inline void __attribute__((__always_inline__, __gnu_inline__, __artificial__))          \
mst_##STYPE##x2_m##STYPE##x2(TYPE_T *a, TYPE_T *b, long c, m##TYPE##x2_t d)                            \
{                                                                                                              \
  if (__riscv_xlen == 32)                                                                                      \
    __builtin_riscv_matrix_mst_si_x2_##TYPE(d, a, b, c);                                                       \
  else                                                                                                         \
    __builtin_riscv_matrix_mst_di_x2_##TYPE(d, a, b, c);                                                       \
}                                                                                                              \
__extension__ extern __inline void __attribute__((__always_inline__, __gnu_inline__, __artificial__))          \
msst_##STYPE##x2_m##STYPE##x2(TYPE_T *a, TYPE_T *b, long c, m##TYPE##x2_t d)                           \
{                                                                                                              \
  if (__riscv_xlen == 32)                                                                                      \
    __builtin_riscv_matrix_msst_si_x2_##TYPE(d, a, b, c);                                                      \
  else                                                                                                         \
    __builtin_riscv_matrix_msst_di_x2_##TYPE(d, a, b, c);                                                      \
}\
__extension__ extern __inline void                                     \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mst2m_##STYPE##_m##STYPE##x2(TYPE_T *a, m##TYPE##x2_t b)             \
{                                                                      \
  if (__riscv_xlen == 32)                                              \
    __builtin_riscv_matrix_mst2m_si_##TYPE(b, a);                     \
  else                                                                 \
    __builtin_riscv_matrix_mst2m_di_##TYPE(b, a);                     \
}                                                                                                            \
__extension__ extern __inline m##TYPE##x2_t                                                                    \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))                                         \
mmov_m##STYPE##x2(m##TYPE_T a, m##TYPE_T b)                                                                \
{                                                                                                              \
  return __builtin_riscv_matrix_mmov_mm_x2_##TYPE(a, b);                                                       \
}

_RVM_FLOAT_TYPE_ITERATOR(__MATRIX_FLOAT_X2)

#define __MATRIX_ALU(TYPE_T, STYPE, TYPE)                                \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
madd_m##STYPE(m##TYPE_T a, m##TYPE_T b)                            \
{                                                                      \
  return __builtin_riscv_matrix_madd_mm_##TYPE(a, b);                  \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
madd_m##STYPE##_m##STYPE##v(m##TYPE_T a, m##TYPE_T b, uint8_t c)   \
{                                                                      \
  return __builtin_riscv_matrix_madd_mv_##TYPE(a, b, c);               \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
madd_m##STYPE##_##STYPE(m##TYPE_T a, TYPE_T b)                     \
{                                                                      \
  return __builtin_riscv_matrix_madd_mx_##TYPE(a, b);                  \
}                                                                      \
								\
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
msub_m##STYPE(m##TYPE_T a, m##TYPE_T b)                            \
{                                                                      \
  return __builtin_riscv_matrix_msub_mm_##TYPE(a, b);                  \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
msub_m##STYPE##_m##STYPE##v(m##TYPE_T a, m##TYPE_T b, uint8_t c)   \
{                                                                      \
  return __builtin_riscv_matrix_msub_mv_##TYPE(a, b, c);               \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
msub_m##STYPE##_##STYPE(m##TYPE_T a, TYPE_T b)                     \
{                                                                      \
  return __builtin_riscv_matrix_msub_mx_##TYPE(a, b);                  \
}                                                                      \
								\
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmul_m##STYPE(m##TYPE_T a, m##TYPE_T b)                            \
{                                                                      \
  return __builtin_riscv_matrix_mmul_mm_##TYPE(a, b);                  \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmul_m##STYPE##_m##STYPE##v(m##TYPE_T a, m##TYPE_T b, uint8_t c)   \
{                                                                      \
  return __builtin_riscv_matrix_mmul_mv_##TYPE(a, b, c);               \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmul_m##STYPE##_##STYPE(m##TYPE_T a, TYPE_T b)                     \
{                                                                      \
  return __builtin_riscv_matrix_mmul_mx_##TYPE(a, b);                  \
}                                                                      \
								\
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmulh_m##STYPE(m##TYPE_T a, m##TYPE_T b)                           \
{                                                                      \
  return __builtin_riscv_matrix_mmulh_mm_##TYPE(a, b);                 \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmulh_m##STYPE##_m##STYPE##v(m##TYPE_T a, m##TYPE_T b, uint8_t c)  \
{                                                                      \
  return __builtin_riscv_matrix_mmulh_mv_##TYPE(a, b, c);              \
}                                                                      \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmulh_m##STYPE##_##STYPE(m##TYPE_T a, TYPE_T b)                    \
{                                                                      \
  return __builtin_riscv_matrix_mmulh_mx_##TYPE(a, b);                 \
}

#define __MATRIX_SHIFT(TYPE_T, STYPE, TYPE)                                    \
__extension__ extern __inline m##TYPE_T                                        \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
msra_m##STYPE##_mu##STYPE(m##TYPE_T a, mu##TYPE_T b)                                  \
{                                                                            \
  return __builtin_riscv_matrix_msra_mm_##TYPE(a, b);                        \
}                                                                            \
__extension__ extern __inline m##TYPE_T                                       \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
msra_m##STYPE##_mu##STYPE##v(m##TYPE_T a, mu##TYPE_T b, uint8_t c)         \
{                                                                            \
  return __builtin_riscv_matrix_msra_mv_##TYPE(a, b, c);                     \
}                                                                            \
__extension__ extern __inline m##TYPE_T                                       \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
msra_m##STYPE##_u##STYPE(m##TYPE_T a, u##TYPE_T b)                           \
{                                                                            \
  return __builtin_riscv_matrix_msra_mx_##TYPE(a, b);                        \
}                                                                            \
								      \
__extension__ extern __inline mint8_t                                        \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
mn4clip_m##STYPE##_mu##STYPE(m##TYPE_T a, mu##TYPE_T b)                               \
{                                                                            \
  return __builtin_riscv_matrix_mn4clip_mm_##TYPE(a, b);                     \
}                                                                            \
__extension__ extern __inline mint8_t                                        \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
mn4clip_m##STYPE##_mu##STYPE##v(m##TYPE_T a, mu##TYPE_T b, uint8_t c)      \
{                                                                            \
  return __builtin_riscv_matrix_mn4clip_mv_##TYPE(a, b, c);                  \
}                                                                            \
__extension__ extern __inline mint8_t                                        \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
mn4clip_m##STYPE##_u##STYPE(m##TYPE_T a, u##TYPE_T b)                        \
{                                                                            \
  return __builtin_riscv_matrix_mn4clip_mx_##TYPE(a, b);                     \
}                                                                            \
								      \
__extension__ extern __inline muint8_t                                       \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
mn4clipu_mu##STYPE(mu##TYPE_T a, mu##TYPE_T b)                           \
{                                                                            \
  return __builtin_riscv_matrix_mn4clipu_mm_##TYPE(a, b);                    \
}                                                                            \
__extension__ extern __inline muint8_t                                       \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
mn4clipu_mu##STYPE##_mu##STYPE##v(mu##TYPE_T a, mu##TYPE_T b, uint8_t c) \
{                                                                            \
  return __builtin_riscv_matrix_mn4clipu_mv_##TYPE(a, b, c);                 \
}                                                                            \
__extension__ extern __inline muint8_t                                       \
__attribute__((__always_inline__, __gnu_inline__, __artificial__))       \
mn4clipu_mu##STYPE##_u##STYPE(mu##TYPE_T a, u##TYPE_T b)                 \
{                                                                            \
  return __builtin_riscv_matrix_mn4clipu_mx_##TYPE(a, b);                    \
}

#define __MATRIX_MOV(TYPE_T, STYPE, TYPE)                                    \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmov_mx_##STYPE(m##TYPE_T a, TYPE_T b, uint8_t c)                                             \
{                                                                      \
  return __builtin_riscv_matrix_mmov_mx_##TYPE(a, b, c);                     \
} \
__extension__ extern __inline TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mmov_xm_##STYPE(m##TYPE_T a, uint8_t b)                                             \
{                                                                      \
  return __builtin_riscv_matrix_mmov_xm_##TYPE(a, b);                     \
} \
__extension__ extern __inline m##TYPE_T                                \
__attribute__((__always_inline__, __gnu_inline__, __artificial__)) \
mdup_##STYPE(TYPE_T a)                                             \
{                                                                      \
  return __builtin_riscv_matrix_mdup_mx_##TYPE(a);                     \
}

_RVM_ALL_TYPE_ITERATOR(__MATRIX_LOAD_STORE)
_RVM_INT_TYPE_ITERATOR(__MATRIX_ALU)
_RVM_INT_SD_TYPE_ITERATOR(__MATRIX_SHIFT)
_RVM_INT_TYPE_ITERATOR(__MATRIX_MOV)

__extension__ extern __inline mfloat16_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
fmmacc_mf16x2_mf16(mfloat16_t a, mfloat16x2_t b, mfloat16_t c)
{
  return __builtin_riscv_matrix_fmmacc_float16(a, c, b);
}

__extension__ extern __inline mfloat32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
fmmacc_mf32(mfloat32_t a, mfloat32_t b, mfloat32_t c)
{
  return __builtin_riscv_matrix_fmmacc_float32(a, b, c);
}

__extension__ extern __inline mfloat64x2_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
fmmacc_mf64(mfloat64x2_t a, mfloat64_t b, mfloat64_t c)
{
  return __builtin_riscv_matrix_fmmacc_float64(a, b, c);
}

__extension__ extern __inline mfloat32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
fwmmacc_mf16(mfloat32_t a, mfloat16_t b, mfloat16_t c)
{
  return __builtin_riscv_matrix_fwmmacc_float16(a, b, c);
}

__extension__ extern __inline mfloat64x2_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
fwmmacc_mf32(mfloat64x2_t a, mfloat32_t b, mfloat32_t c)
{
  return __builtin_riscv_matrix_fwmmacc_float32(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqa_mi8(mint32_t a, mint8_t b, mint8_t c)
{
  return __builtin_riscv_matrix_mmaqa_int8(a, b, c);
}

__extension__ extern __inline mint64_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqa_mi16(mint64_t a, mint16_t b, mint16_t c)
{
  return __builtin_riscv_matrix_mmaqa_int16(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqau_mui8(mint32_t a, muint8_t b, muint8_t c)
{
  return __builtin_riscv_matrix_mmaqau_int8(a, b, c);
}

__extension__ extern __inline mint64_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqau_mui16(mint64_t a, muint16_t b, muint16_t c)
{
  return __builtin_riscv_matrix_mmaqau_int16(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqaus_mui8_mi8(mint32_t a, muint8_t b, mint8_t c)
{
  return __builtin_riscv_matrix_mmaqaus_int8(a, b, c);
}

__extension__ extern __inline mint64_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqaus_mui16_mi16(mint64_t a, muint16_t b, mint16_t c)
{
  return __builtin_riscv_matrix_mmaqaus_int16(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqasu_mi8_mui8(mint32_t a, mint8_t b, muint8_t c)
{
  return __builtin_riscv_matrix_mmaqasu_int8(a, b, c);
}

__extension__ extern __inline mint64_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
mmaqasu_mi16_mui16(mint64_t a, mint16_t b, muint16_t c)
{
  return __builtin_riscv_matrix_mmaqasu_int16(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
pmmaqa_mi8(mint32_t a, mint8_t b, mint8_t c)
{
  return __builtin_riscv_matrix_pmmaqa_int8(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
pmmaqau_mui8(mint32_t a, muint8_t b, muint8_t c)
{
  return __builtin_riscv_matrix_pmmaqau_int8(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
pmmaqaus_mui8_mi8(mint32_t a, muint8_t b, mint8_t c)
{
  return __builtin_riscv_matrix_pmmaqaus_int8(a, b, c);
}

__extension__ extern __inline mint32_t __attribute__((__always_inline__, __gnu_inline__, __artificial__))
pmmaqasu_mi8_mui8(mint32_t a, mint8_t b, muint8_t c)
{
  return __builtin_riscv_matrix_pmmaqasu_int8(a, b, c);
}

#endif
#endif /* _GCC_RISCV_MATRIX_H */
