/* RISC-V Martix extension instructions include file.

   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Alibaba Damo Academy Xuantie Team..

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

#ifndef __riscv_matrix
#error "Matrix intrinsics require the matrix extension."
#else

#include <stdint.h>
#include <stddef.h>

#define FUNC_PREFIX						\
__extension__ extern __inline					\
__attribute__ ((__always_inline__, __gnu_inline__, __artificial__))

typedef __fp16 float16_t;
typedef float float32_t;
typedef double float64_t;

typedef __fp16 __float16_t;
typedef float __float32_t;
typedef double __float64_t;

typedef size_t mrow_t;
typedef size_t mcol_t;
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

FUNC_PREFIX
unsigned long __riscv_th_mread_csr(enum RVM_CSR csr)
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

FUNC_PREFIX
void __riscv_th_mwrite_csr(enum RVM_CSR csr, unsigned long value)
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

FUNC_PREFIX
mrow_t __riscv_th_msetmrow_m (mrow_t m)
{return (mrow_t)(__builtin_riscv_msetmrow_m(m) & 0xff);}

FUNC_PREFIX
mrow_t __riscv_th_msetmrow_n (mrow_t n)
{return (mrow_t)(__builtin_riscv_msetmrow_n(n) >> 8 & 0xff);}

FUNC_PREFIX
mcol_t __riscv_th_msetmcol_e8 (mcol_t c)
{return (mcol_t)((__builtin_riscv_msetmcol (c) >> 16 & 0xffff));}

FUNC_PREFIX
mcol_t __riscv_th_msetmcol_e16 (mcol_t c)
{return (mcol_t)((__builtin_riscv_msetmcol (c << 1) >> 16 & 0xffff)/2);}

FUNC_PREFIX
mcol_t __riscv_th_msetmcol_e32 (mcol_t c)
{return (mcol_t)((__builtin_riscv_msetmcol (c << 2) >> 16 & 0xffff)/4);}

FUNC_PREFIX
mcol_t __riscv_th_msetmcol_e64 (mcol_t c)
{return (mcol_t)((__builtin_riscv_msetmcol (c << 3) >> 16 & 0xffff)/8);}

#define IDENTITY(...) __VA_ARGS__

#define __MATRIX_ALL_TYPE_ITERATOR(MACRO, ...)						\
  MACRO(mint8_t,    int8_t,    const int8_t*,    i8,  IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint16_t,   int16_t,   const int16_t*,   i16, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint32_t,   int32_t,   const int32_t*,   i32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint64_t,   int64_t,   const int64_t*,   i64, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint8_t,   uint8_t,   const uint8_t*,   u8,  IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint16_t,  uint16_t,  const uint16_t*,  u16, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint32_t,  uint32_t,  const uint32_t*,  u32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint64_t,  uint64_t,  const uint64_t*,  u64, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat16_t, float16_t, const float16_t*, f16, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat32_t, float32_t, const float32_t*, f32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat64_t, float64_t, const float64_t*, f64, IDENTITY(),  __VA_ARGS__)

#define __MATRIX_INT_TYPE_ITERATOR(MACRO, ...)						\
  MACRO(mint8_t,    int8_t,   cosnt int8_t*,    i8,  IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint16_t,   int16_t,  cosnt int16_t*,   i16, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint32_t,   int32_t,  cosnt int32_t*,   i32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint64_t,   int64_t,  cosnt int64_t*,   i64, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint8_t,   uint8_t,  cosnt uint8_t*,   u8,  IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint16_t,  uint16_t, cosnt uint16_t*,  u16, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint32_t,  uint32_t, cosnt uint32_t*,  u32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint64_t,  uint64_t, cosnt uint64_t*,  u64, IDENTITY(),  __VA_ARGS__)

#define __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mint32_t,   int32_t,  const int32_t*,   i32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint64_t,   int64_t,  const int64_t*,   i64, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint32_t,  uint32_t, const uint32_t*,  u32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint64_t,  uint64_t, const uint64_t*,  u64, IDENTITY(),  __VA_ARGS__)

#define __MATRIX_SI_USI_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mint32_t,   int32_t,  const int32_t*,   i32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint32_t,  uint32_t, const uint32_t*,  u32, IDENTITY(,), __VA_ARGS__)

#define __MATRIX_DI_UDI_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mint64_t,   int64_t,  const int64_t*,   i64, IDENTITY(,), __VA_ARGS__)	\
  MACRO(muint64_t,  uint64_t, const uint64_t*,  u64, IDENTITY(,), __VA_ARGS__)

#define __MATRIX_SI_DI_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mint32_t,   int32_t,  const int32_t*,   i32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint64_t,   int64_t,  const int64_t*,   i64, IDENTITY(), __VA_ARGS__)

#define __MATRIX_USI_UDI__TYPE_ITERATOR(MACRO, ...)					\
  MACRO(muint32_t,  uint32_t, const uint32_t*,  u32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint64_t,  uint64_t, const uint64_t*,  u64, IDENTITY(),  __VA_ARGS__)

#define __MATRIX_SI_2DI_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mint32_t,   int32_t,   int32_t*,   i32, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint64x2_t,   int64_t,   int64_t*, i64x2, IDENTITY(), __VA_ARGS__)

#define __MATRIX_2ALL_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mint8x2_t,    int8_t,    const int8_t*,    i8x2,  IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint16x2_t,   int16_t,   const int16_t*,   i16x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint32x2_t,   int32_t,   const int32_t*,   i32x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mint64x2_t,   int64_t,   const int64_t*,   i64x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint8x2_t,   uint8_t,   const uint8_t*,   u8x2,  IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint16x2_t,  uint16_t,  const uint16_t*,  u16x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint32x2_t,  uint32_t,  const uint32_t*,  u32x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(muint64x2_t,  uint64_t,  const uint64_t*,  u64x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat16x2_t, float16_t, const float16_t*, f16x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat32x2_t, float32_t, const float32_t*, f32x2, IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat64x2_t, float64_t, const float64_t*, f64x2, IDENTITY(),  __VA_ARGS__)

#define __MATRIX_HF_SF_2DF_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mfloat16_t,   float16_t, const float16_t*, f16,   IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat32_t,   float32_t, const float32_t*, f32,   IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat64x2_t, float64_t, const float64_t*, f64x2, IDENTITY(),  __VA_ARGS__)

#define __MATRIX_SF_2DF_TYPE_ITERATOR(MACRO, ...)					\
  MACRO(mfloat32_t,   float32_t, const float32_t*, f32,   IDENTITY(,), __VA_ARGS__)		\
  MACRO(mfloat64x2_t, float64_t, const float64_t*, f64x2, IDENTITY(),  __VA_ARGS__)

#define __MATRIX_SI_TYPE_ITERATOR(MACRO, ...)						\
  MACRO(mint32_t,   int32_t,   int32_t*,   i32, IDENTITY(), __VA_ARGS__)

#define __MATRIX_UNDEFINED(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)		\
FUNC_PREFIX									\
MTYPE __riscv_th_mundefined_##SUFFIX(){ MTYPE a; return a; }

__MATRIX_ALL_TYPE_ITERATOR(__MATRIX_UNDEFINED,)
__MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_UNDEFINED,)

#if defined(__riscv_xlen) && __riscv_xlen != 64

#define __MATRIX_MOV_FOR_RV32(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)		\
FUNC_PREFIX									\
MTYPE __riscv_th_mmov_m_x_##SUFFIX(MTYPE dest, TYPE src, size_t index)		\
{										\
  muint32_t temp = __riscv_th_mreinterpret_u32_##SUFFIX (dest);			\
  temp = __riscv_th_mmov_m_x_u32(temp, src & 0xffffffff, index * 2);		\
  temp = __riscv_th_mmov_m_x_u32(temp, src >> 32, index * 2 + 1);		\
  return __riscv_th_mreinterpret_##SUFFIX##_u32(temp);				\
}										\
FUNC_PREFIX									\
TYPE __riscv_th_mmov_x_m_##SUFFIX(MTYPE src, size_t index)			\
{										\
  muint32_t temp = __riscv_th_mreinterpret_u32_##SUFFIX(src);			\
  uint32_t t1 = __riscv_th_mmov_x_m_u32(temp, index * 2);			\
  uint32_t t2 = __riscv_th_mmov_x_m_u32(temp, index * 2 + 1);			\
  uint64_t low = t1;								\
  uint64_t high = t2;								\
  return (high << 32 | low);							\
}										\
FUNC_PREFIX									\
MTYPE __riscv_th_mdup_m_x_##SUFFIX (TYPE src)					\
{										\
  MTYPE temp = __riscv_th_mundefined_##SUFFIX ();				\
  MTYPE res = __riscv_th_mundefined_##SUFFIX ();				\
  temp = __riscv_th_mmov_m_x_##SUFFIX(temp, src, 0);				\
  temp = __riscv_th_mmov_mv_##SUFFIX(temp, 0);					\
  mrow_t row = __riscv_th_mread_csr(RVM_XRLENB) / 4;				\
  mrow_t col = row / 2;								\
  __riscv_th_mst_##SUFFIX((const TYPE *)&res, 8, temp, row, 1);			\
  return __riscv_th_mld_##SUFFIX((const TYPE *)&res, 0, row, col);		\
}

__MATRIX_DI_UDI_TYPE_ITERATOR(__MATRIX_MOV_FOR_RV32, )

#define __MATRIX_ALU_FOR_RV32(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)			\
FUNC_PREFIX										\
MTYPE __riscv_th_##NAME##_mx_##SUFFIX (MTYPE src1, TYPE src2, mrow_t row, mcol_t col)	\
{											\
  MTYPE temp = __riscv_th_mdup_m_x_##SUFFIX(src2);					\
  return __riscv_th_##NAME##_mm_##SUFFIX(src1, temp, row, col);				\
}

__MATRIX_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_FOR_RV32, madd)
__MATRIX_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_FOR_RV32, msub)
__MATRIX_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_FOR_RV32, mmul)
__MATRIX_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_FOR_RV32, mmulh)

FUNC_PREFIX
mint64_t __riscv_th_msra_mx_i64 (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{
  muint64_t temp = __riscv_th_mdup_m_x_u64(src2);
  return __riscv_th_msra_mm_i64(src1, temp, row, col);
}

FUNC_PREFIX
mint8_t __riscv_th_mn4clip_mx_i64 (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{
  muint64_t temp = __riscv_th_mdup_m_x_u64(src2);
  return __riscv_th_mn4clip_mm_i64(src1, temp, row, col);
}

FUNC_PREFIX
muint8_t __riscv_th_mn4clipu_mx_u64 (muint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{
  muint64_t temp = __riscv_th_mdup_m_x_u64(src2);
  return __riscv_th_mn4clipu_mm_u64(src1, temp, row, col);
}
#endif /* #if defined(__riscv_xlen) && __riscv_xlen != 64 */

#ifdef __cplusplus
FUNC_PREFIX mint8_t __riscv_th_mld (const int8_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_i8 (base, stride, row, col); }
FUNC_PREFIX muint8_t __riscv_th_mld (const uint8_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_u8 (base, stride, row, col); }
FUNC_PREFIX mint16_t __riscv_th_mld (const int16_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_i16 (base, stride, row, col); }
FUNC_PREFIX muint16_t __riscv_th_mld (const uint16_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_u16 (base, stride, row, col); }
FUNC_PREFIX mint32_t __riscv_th_mld (const int32_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_i32 (base, stride, row, col); }
FUNC_PREFIX muint32_t __riscv_th_mld (const uint32_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_u32 (base, stride, row, col); }
FUNC_PREFIX mint64_t __riscv_th_mld (const int64_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_i64 (base, stride, row, col); }
FUNC_PREFIX muint64_t __riscv_th_mld (const uint64_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_u64 (base, stride, row, col); }
FUNC_PREFIX mfloat16_t __riscv_th_mld (const float16_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_f16 (base, stride, row, col); }
FUNC_PREFIX mfloat32_t __riscv_th_mld (const float32_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_f32 (base, stride, row, col); }
FUNC_PREFIX mfloat64_t __riscv_th_mld (const float64_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_mld_f64 (base, stride, row, col); }
FUNC_PREFIX mint8_t __riscv_th_msld (const int8_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_i8 (base, stride, row, col); }
FUNC_PREFIX muint8_t __riscv_th_msld (const uint8_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_u8 (base, stride, row, col); }
FUNC_PREFIX mint16_t __riscv_th_msld (const int16_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_i16 (base, stride, row, col); }
FUNC_PREFIX muint16_t __riscv_th_msld (const uint16_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_u16 (base, stride, row, col); }
FUNC_PREFIX mint32_t __riscv_th_msld (const int32_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_i32 (base, stride, row, col); }
FUNC_PREFIX muint32_t __riscv_th_msld (const uint32_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_u32 (base, stride, row, col); }
FUNC_PREFIX mint64_t __riscv_th_msld (const int64_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_i64 (base, stride, row, col); }
FUNC_PREFIX muint64_t __riscv_th_msld (const uint64_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_u64 (base, stride, row, col); }
FUNC_PREFIX mfloat16_t __riscv_th_msld (const float16_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_f16 (base, stride, row, col); }
FUNC_PREFIX mfloat32_t __riscv_th_msld (const float32_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_f32 (base, stride, row, col); }
FUNC_PREFIX mfloat64_t __riscv_th_msld (const float64_t *base, long stride, mrow_t row, mcol_t col) { return __riscv_th_msld_f64 (base, stride, row, col); }
FUNC_PREFIX void __riscv_th_mst (const int8_t *base, long stride, mint8_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_i8 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const uint8_t *base, long stride, muint8_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_u8 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const int16_t *base, long stride, mint16_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_i16 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const uint16_t *base, long stride, muint16_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_u16 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const int32_t *base, long stride, mint32_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_i32 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const uint32_t *base, long stride, muint32_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_u32 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const int64_t *base, long stride, mint64_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_i64 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const uint64_t *base, long stride, muint64_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_u64 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const float16_t *base, long stride, mfloat16_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_f16 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const float32_t *base, long stride, mfloat32_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_f32 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_mst (const float64_t *base, long stride, mfloat64_t value, mrow_t row, mcol_t col) { return __riscv_th_mst_f64 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const int8_t *base, long stride, mint8_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_i8 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const uint8_t *base, long stride, muint8_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_u8 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const int16_t *base, long stride, mint16_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_i16 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const uint16_t *base, long stride, muint16_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_u16 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const int32_t *base, long stride, mint32_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_i32 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const uint32_t *base, long stride, muint32_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_u32 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const int64_t *base, long stride, mint64_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_i64 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const uint64_t *base, long stride, muint64_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_u64 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const float16_t *base, long stride, mfloat16_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_f16 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const float32_t *base, long stride, mfloat32_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_f32 (base, stride, value, row, col); }
FUNC_PREFIX void __riscv_th_msst (const float64_t *base, long stride, mfloat64_t value, mrow_t row, mcol_t col) { return __riscv_th_msst_f64 (base, stride, value, row, col); }
FUNC_PREFIX mint8_t __riscv_th_mmov_mv (mint8_t src, size_t index) { return __riscv_th_mmov_mv_i8 (src, index); }
FUNC_PREFIX muint8_t __riscv_th_mmov_mv (muint8_t src, size_t index) { return __riscv_th_mmov_mv_u8 (src, index); }
FUNC_PREFIX mint16_t __riscv_th_mmov_mv (mint16_t src, size_t index) { return __riscv_th_mmov_mv_i16 (src, index); }
FUNC_PREFIX muint16_t __riscv_th_mmov_mv (muint16_t src, size_t index) { return __riscv_th_mmov_mv_u16 (src, index); }
FUNC_PREFIX mint32_t __riscv_th_mmov_mv (mint32_t src, size_t index) { return __riscv_th_mmov_mv_i32 (src, index); }
FUNC_PREFIX muint32_t __riscv_th_mmov_mv (muint32_t src, size_t index) { return __riscv_th_mmov_mv_u32 (src, index); }
FUNC_PREFIX mint64_t __riscv_th_mmov_mv (mint64_t src, size_t index) { return __riscv_th_mmov_mv_i64 (src, index); }
FUNC_PREFIX muint64_t __riscv_th_mmov_mv (muint64_t src, size_t index) { return __riscv_th_mmov_mv_u64 (src, index); }
FUNC_PREFIX mfloat16_t __riscv_th_mmov_mv (mfloat16_t src, size_t index) { return __riscv_th_mmov_mv_f16 (src, index); }
FUNC_PREFIX mfloat32_t __riscv_th_mmov_mv (mfloat32_t src, size_t index) { return __riscv_th_mmov_mv_f32 (src, index); }
FUNC_PREFIX mfloat64_t __riscv_th_mmov_mv (mfloat64_t src, size_t index) { return __riscv_th_mmov_mv_f64 (src, index); }
FUNC_PREFIX mint8_t __riscv_th_mdup_m_x (int8_t src) { return __riscv_th_mdup_m_x_i8 (src); }
FUNC_PREFIX muint8_t __riscv_th_mdup_m_x (uint8_t src) { return __riscv_th_mdup_m_x_u8 (src); }
FUNC_PREFIX mint16_t __riscv_th_mdup_m_x (int16_t src) { return __riscv_th_mdup_m_x_i16 (src); }
FUNC_PREFIX muint16_t __riscv_th_mdup_m_x (uint16_t src) { return __riscv_th_mdup_m_x_u16 (src); }
FUNC_PREFIX mint32_t __riscv_th_mdup_m_x (int32_t src) { return __riscv_th_mdup_m_x_i32 (src); }
FUNC_PREFIX muint32_t __riscv_th_mdup_m_x (uint32_t src) { return __riscv_th_mdup_m_x_u32 (src); }
FUNC_PREFIX mint64_t __riscv_th_mdup_m_x (int64_t src) { return __riscv_th_mdup_m_x_i64 (src); }
FUNC_PREFIX muint64_t __riscv_th_mdup_m_x (uint64_t src) { return __riscv_th_mdup_m_x_u64 (src); }
FUNC_PREFIX mint8_t __riscv_th_mmov_m_x (mint8_t dest, int8_t src, size_t index) { return __riscv_th_mmov_m_x_i8 (dest, src, index); }
FUNC_PREFIX muint8_t __riscv_th_mmov_m_x (muint8_t dest, uint8_t src, size_t index) { return __riscv_th_mmov_m_x_u8 (dest, src, index); }
FUNC_PREFIX mint16_t __riscv_th_mmov_m_x (mint16_t dest, int16_t src, size_t index) { return __riscv_th_mmov_m_x_i16 (dest, src, index); }
FUNC_PREFIX muint16_t __riscv_th_mmov_m_x (muint16_t dest, uint16_t src, size_t index) { return __riscv_th_mmov_m_x_u16 (dest, src, index); }
FUNC_PREFIX mint32_t __riscv_th_mmov_m_x (mint32_t dest, int32_t src, size_t index) { return __riscv_th_mmov_m_x_i32 (dest, src, index); }
FUNC_PREFIX muint32_t __riscv_th_mmov_m_x (muint32_t dest, uint32_t src, size_t index) { return __riscv_th_mmov_m_x_u32 (dest, src, index); }
FUNC_PREFIX mint64_t __riscv_th_mmov_m_x (mint64_t dest, int64_t src, size_t index) { return __riscv_th_mmov_m_x_i64 (dest, src, index); }
FUNC_PREFIX muint64_t __riscv_th_mmov_m_x (muint64_t dest, uint64_t src, size_t index) { return __riscv_th_mmov_m_x_u64 (dest, src, index); }
FUNC_PREFIX int8_t __riscv_th_mmov_x_m (mint8_t src, size_t index) { return __riscv_th_mmov_x_m_i8 (src, index); }
FUNC_PREFIX uint8_t __riscv_th_mmov_x_m (muint8_t src, size_t index) { return __riscv_th_mmov_x_m_u8 (src, index); }
FUNC_PREFIX int16_t __riscv_th_mmov_x_m (mint16_t src, size_t index) { return __riscv_th_mmov_x_m_i16 (src, index); }
FUNC_PREFIX uint16_t __riscv_th_mmov_x_m (muint16_t src, size_t index) { return __riscv_th_mmov_x_m_u16 (src, index); }
FUNC_PREFIX int32_t __riscv_th_mmov_x_m (mint32_t src, size_t index) { return __riscv_th_mmov_x_m_i32 (src, index); }
FUNC_PREFIX uint32_t __riscv_th_mmov_x_m (muint32_t src, size_t index) { return __riscv_th_mmov_x_m_u32 (src, index); }
FUNC_PREFIX int64_t __riscv_th_mmov_x_m (mint64_t src, size_t index) { return __riscv_th_mmov_x_m_i64 (src, index); }
FUNC_PREFIX uint64_t __riscv_th_mmov_x_m (muint64_t src, size_t index) { return __riscv_th_mmov_x_m_u64 (src, index); }
FUNC_PREFIX mint8x2_t __riscv_th_mset (mint8x2_t    src, const long int index, mint8_t    value) { return __riscv_th_mset_i8x2 (src, index, value); }
FUNC_PREFIX mint16x2_t __riscv_th_mset (mint16x2_t   src, const long int index, mint16_t   value) { return __riscv_th_mset_i16x2 (src, index, value); }
FUNC_PREFIX mint32x2_t __riscv_th_mset (mint32x2_t   src, const long int index, mint32_t   value) { return __riscv_th_mset_i32x2 (src, index, value); }
FUNC_PREFIX mint64x2_t __riscv_th_mset (mint64x2_t   src, const long int index, mint64_t   value) { return __riscv_th_mset_i64x2 (src, index, value); }
FUNC_PREFIX muint8x2_t __riscv_th_mset (muint8x2_t   src, const long int index, muint8_t   value) { return __riscv_th_mset_u8x2 (src, index, value); }
FUNC_PREFIX muint16x2_t __riscv_th_mset (muint16x2_t  src, const long int index, muint16_t  value) { return __riscv_th_mset_u16x2 (src, index, value); }
FUNC_PREFIX muint32x2_t __riscv_th_mset (muint32x2_t  src, const long int index, muint32_t  value) { return __riscv_th_mset_u32x2 (src, index, value); }
FUNC_PREFIX muint64x2_t __riscv_th_mset (muint64x2_t  src, const long int index, muint64_t  value) { return __riscv_th_mset_u64x2 (src, index, value); }
FUNC_PREFIX mfloat16x2_t __riscv_th_mset (mfloat16x2_t src, const long int index, mfloat16_t value) { return __riscv_th_mset_f16x2 (src, index, value); }
FUNC_PREFIX mfloat32x2_t __riscv_th_mset (mfloat32x2_t src, const long int index, mfloat32_t value) { return __riscv_th_mset_f32x2 (src, index, value); }
FUNC_PREFIX mfloat64x2_t __riscv_th_mset (mfloat64x2_t src, const long int index, mfloat64_t value) { return __riscv_th_mset_f64x2 (src, index, value); }
FUNC_PREFIX mint8_t __riscv_th_mget (mint8x2_t    src, const long int index) { return __riscv_th_mget_i8x2 (src, index); }
FUNC_PREFIX mint16_t __riscv_th_mget (mint16x2_t   src, const long int index) { return __riscv_th_mget_i16x2 (src, index); }
FUNC_PREFIX mint32_t __riscv_th_mget (mint32x2_t   src, const long int index) { return __riscv_th_mget_i32x2 (src, index); }
FUNC_PREFIX mint64_t __riscv_th_mget (mint64x2_t   src, const long int index) { return __riscv_th_mget_i64x2 (src, index); }
FUNC_PREFIX muint8_t __riscv_th_mget (muint8x2_t   src, const long int index) { return __riscv_th_mget_u8x2 (src, index); }
FUNC_PREFIX muint16_t __riscv_th_mget (muint16x2_t  src, const long int index) { return __riscv_th_mget_u16x2 (src, index); }
FUNC_PREFIX muint32_t __riscv_th_mget (muint32x2_t  src, const long int index) { return __riscv_th_mget_u32x2 (src, index); }
FUNC_PREFIX muint64_t __riscv_th_mget (muint64x2_t  src, const long int index) { return __riscv_th_mget_u64x2 (src, index); }
FUNC_PREFIX mfloat16_t __riscv_th_mget (mfloat16x2_t src, const long int index) { return __riscv_th_mget_f16x2 (src, index); }
FUNC_PREFIX mfloat32_t __riscv_th_mget (mfloat32x2_t src, const long int index) { return __riscv_th_mget_f32x2 (src, index); }
FUNC_PREFIX mfloat64_t __riscv_th_mget (mfloat64x2_t src, const long int index) { return __riscv_th_mget_f64x2 (src, index); }
FUNC_PREFIX mint32_t __riscv_th_madd_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_madd_mm_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_madd_mm (mint64_t src1, mint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_madd_mm_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_madd_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_madd_mv_i32 (src1,  src2, index, row, col); }
FUNC_PREFIX mint64_t __riscv_th_madd_mv (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_madd_mv_i64 (src1,  src2, index, row, col); }
FUNC_PREFIX mint32_t __riscv_th_madd_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col) { return __riscv_th_madd_mx_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_madd_mx (mint64_t src1, int64_t src2, mrow_t row, mcol_t col) { return __riscv_th_madd_mx_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_msub_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_msub_mm_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_msub_mm (mint64_t src1, mint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_msub_mm_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_msub_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_msub_mv_i32 (src1,  src2, index, row, col); }
FUNC_PREFIX mint64_t __riscv_th_msub_mv (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_msub_mv_i64 (src1,  src2, index, row, col); }
FUNC_PREFIX mint32_t __riscv_th_msub_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col) { return __riscv_th_msub_mx_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_msub_mx (mint64_t src1, int64_t src2, mrow_t row, mcol_t col) { return __riscv_th_msub_mx_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_msra_mm (mint32_t src1, muint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_msra_mm_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_msra_mm (mint64_t src1, muint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_msra_mm_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_msra_mv (mint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_msra_mv_i32 (src1,  src2, index, row, col); }
FUNC_PREFIX mint64_t __riscv_th_msra_mv (mint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_msra_mv_i64 (src1,  src2, index, row, col); }
FUNC_PREFIX mint32_t __riscv_th_msra_mx (mint32_t src1, uint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_msra_mx_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_msra_mx (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_msra_mx_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint8_t __riscv_th_mn4clip_mm (mint32_t src1, muint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clip_mm_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint8_t __riscv_th_mn4clip_mm (mint64_t src1, muint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clip_mm_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint8_t __riscv_th_mn4clip_mv (mint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_mn4clip_mv_i32 (src1,  src2, index, row, col); }
FUNC_PREFIX mint8_t __riscv_th_mn4clip_mv (mint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_mn4clip_mv_i64 (src1,  src2, index, row, col); }
FUNC_PREFIX mint8_t __riscv_th_mn4clip_mx (mint32_t src1, uint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clip_mx_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint8_t __riscv_th_mn4clip_mx (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clip_mx_i64 (src1,  src2, row, col); }
FUNC_PREFIX muint8_t __riscv_th_mn4clipu_mm (muint32_t src1, muint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clipu_mm_u32 (src1,  src2, row, col); }
FUNC_PREFIX muint8_t __riscv_th_mn4clipu_mm (muint64_t src1, muint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clipu_mm_u64 (src1,  src2, row, col); }
FUNC_PREFIX muint8_t __riscv_th_mn4clipu_mv (muint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_mn4clipu_mv_u32 (src1,  src2, index, row, col); }
FUNC_PREFIX muint8_t __riscv_th_mn4clipu_mv (muint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_mn4clipu_mv_u64 (src1,  src2, index, row, col); }
FUNC_PREFIX muint8_t __riscv_th_mn4clipu_mx (muint32_t src1, uint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clipu_mx_u32 (src1,  src2, row, col); }
FUNC_PREFIX muint8_t __riscv_th_mn4clipu_mx (muint64_t src1, uint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_mn4clipu_mx_u64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_mmul_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mmul_mm_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_mmul_mm (mint64_t src1, mint64_t src2, mrow_t row, mcol_t col) { return __riscv_th_mmul_mm_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_mmul_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_mmul_mv_i32 (src1,  src2, index, row, col); }
FUNC_PREFIX mint64_t __riscv_th_mmul_mv (mint64_t src1, mint64_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_mmul_mv_i64 (src1,  src2, index, row, col); }
FUNC_PREFIX mint32_t __riscv_th_mmul_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mmul_mx_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint64_t __riscv_th_mmul_mx (mint64_t src1, int64_t src2, mrow_t row, mcol_t col) { return __riscv_th_mmul_mx_i64 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_mmulh_mm (mint32_t src1, mint32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mmulh_mm_i32 (src1,  src2, row, col); }
FUNC_PREFIX mint32_t __riscv_th_mmulh_mv (mint32_t src1, mint32_t src2, size_t index, mrow_t row, mcol_t col) { return __riscv_th_mmulh_mv_i32 (src1,  src2, index, row, col); }
FUNC_PREFIX mint32_t __riscv_th_mmulh_mx (mint32_t src1, int32_t src2, mrow_t row, mcol_t col) { return __riscv_th_mmulh_mx_i32 (src1,  src2, row, col); }
FUNC_PREFIX mfloat16_t __riscv_th_fmmacc (mfloat16_t dest, mfloat16_t src1, mfloat16x2_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_fmmacc_f16 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mfloat32_t __riscv_th_fmmacc (mfloat32_t dest, mfloat32_t src1, mfloat32_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_fmmacc_f32 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mfloat64x2_t __riscv_th_fmmacc (mfloat64x2_t dest, mfloat64_t src1, mfloat64_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_fmmacc_f64x2 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mfloat32_t __riscv_th_fwmmacc (mfloat32_t dest, mfloat16_t src1, mfloat16_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_fwmmacc_f32 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mfloat64x2_t __riscv_th_fwmmacc (mfloat64x2_t dest, mfloat32_t src1, mfloat32_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_fwmmacc_f64x2 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint32_t __riscv_th_mmaqa (mint32_t dest, mint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqa_i32 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint64x2_t __riscv_th_mmaqa (mint64x2_t dest, mint16_t src1, mint16_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqa_i64x2 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint32_t __riscv_th_mmaqau (mint32_t dest, muint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqau_i32 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint64x2_t __riscv_th_mmaqau (mint64x2_t dest, muint16_t src1, muint16_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqau_i64x2 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint32_t __riscv_th_mmaqaus (mint32_t dest, muint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqaus_i32 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint64x2_t __riscv_th_mmaqaus (mint64x2_t dest, muint16_t src1, mint16_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqaus_i64x2 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint32_t __riscv_th_mmaqasu (mint32_t dest, mint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqasu_i32 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint64x2_t __riscv_th_mmaqasu (mint64x2_t dest, mint16_t src1, muint16_t src2, mrow_t row1, mrow_t row2, mcol_t col) { return __riscv_th_mmaqasu_i64x2 (dest,  src1, src2, row1, row2, col); }
FUNC_PREFIX mint32_t __riscv_th_pmmaqa (mint32_t dest, mint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col){ return __riscv_th_pmmaqa_i32 (dest, src1, src2, row1, row2, col);}
FUNC_PREFIX mint32_t __riscv_th_pmmaqau (mint32_t dest, muint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col){ return __riscv_th_pmmaqau_i32 (dest, src1, src2, row1, row2, col);}
FUNC_PREFIX mint32_t __riscv_th_pmmaqaus (mint32_t dest, muint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col){ return __riscv_th_pmmaqaus_i32 (dest, src1, src2, row1, row2, col);}
FUNC_PREFIX mint32_t __riscv_th_pmmaqasu (mint32_t dest, mint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col){ return __riscv_th_pmmaqasu_i32 (dest, src1, src2, row1, row2, col);}

#define __MATRIX_MREINTERPRET(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)	\
  FUNC_PREFIX mint8_t __riscv_th_mreinterpret_i8 (MTYPE src) { return __riscv_th_mreinterpret_i8_##SUFFIX (src); } \
  FUNC_PREFIX mint16_t __riscv_th_mreinterpret_i16 (MTYPE src) { return __riscv_th_mreinterpret_i16_##SUFFIX (src); } \
  FUNC_PREFIX mint32_t __riscv_th_mreinterpret_i32 (MTYPE src) { return __riscv_th_mreinterpret_i32_##SUFFIX (src); } \
  FUNC_PREFIX mint64_t __riscv_th_mreinterpret_i64 (MTYPE src) { return __riscv_th_mreinterpret_i64_##SUFFIX (src); } \
  FUNC_PREFIX muint8_t __riscv_th_mreinterpret_u8 (MTYPE src) { return __riscv_th_mreinterpret_u8_##SUFFIX (src); } \
  FUNC_PREFIX muint16_t __riscv_th_mreinterpret_u16 (MTYPE src) { return __riscv_th_mreinterpret_u16_##SUFFIX (src); } \
  FUNC_PREFIX muint32_t __riscv_th_mreinterpret_u32 (MTYPE src) { return __riscv_th_mreinterpret_u32_##SUFFIX (src); } \
  FUNC_PREFIX muint64_t __riscv_th_mreinterpret_u64 (MTYPE src) { return __riscv_th_mreinterpret_u64_##SUFFIX (src); } \
  FUNC_PREFIX mfloat16_t __riscv_th_mreinterpret_f16 (MTYPE src) { return __riscv_th_mreinterpret_f16_##SUFFIX (src); } \
  FUNC_PREFIX mfloat32_t __riscv_th_mreinterpret_f32 (MTYPE src) { return __riscv_th_mreinterpret_f32_##SUFFIX (src); } \
  FUNC_PREFIX mfloat64_t __riscv_th_mreinterpret_f64 (MTYPE src) { return __riscv_th_mreinterpret_f64_##SUFFIX (src); }

#define __MATRIX_MREINTERPRET_X2(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)	\
  FUNC_PREFIX mint8x2_t __riscv_th_mreinterpret_i8x2 (MTYPE src) { return __riscv_th_mreinterpret_i8x2_##SUFFIX (src); } \
  FUNC_PREFIX mint16x2_t __riscv_th_mreinterpret_i16x2 (MTYPE src) { return __riscv_th_mreinterpret_i16x2_##SUFFIX (src); } \
  FUNC_PREFIX mint32x2_t __riscv_th_mreinterpret_i32x2 (MTYPE src) { return __riscv_th_mreinterpret_i32x2_##SUFFIX (src); } \
  FUNC_PREFIX mint64x2_t __riscv_th_mreinterpret_i64x2 (MTYPE src) { return __riscv_th_mreinterpret_i64x2_##SUFFIX (src); } \
  FUNC_PREFIX muint8x2_t __riscv_th_mreinterpret_u8x2 (MTYPE src) { return __riscv_th_mreinterpret_u8x2_##SUFFIX (src); } \
  FUNC_PREFIX muint16x2_t __riscv_th_mreinterpret_u16x2 (MTYPE src) { return __riscv_th_mreinterpret_u16x2_##SUFFIX (src); } \
  FUNC_PREFIX muint32x2_t __riscv_th_mreinterpret_u32x2 (MTYPE src) { return __riscv_th_mreinterpret_u32x2_##SUFFIX (src); } \
  FUNC_PREFIX muint64x2_t __riscv_th_mreinterpret_u64x2 (MTYPE src) { return __riscv_th_mreinterpret_u64x2_##SUFFIX (src); } \
  FUNC_PREFIX mfloat16x2_t __riscv_th_mreinterpret_f16x2 (MTYPE src) { return __riscv_th_mreinterpret_f16x2_##SUFFIX (src); } \
  FUNC_PREFIX mfloat32x2_t __riscv_th_mreinterpret_f32x2 (MTYPE src) { return __riscv_th_mreinterpret_f32x2_##SUFFIX (src); } \
  FUNC_PREFIX mfloat64x2_t __riscv_th_mreinterpret_f64x2 (MTYPE src) { return __riscv_th_mreinterpret_f64x2_##SUFFIX (src); }

__MATRIX_ALL_TYPE_ITERATOR(__MATRIX_MREINTERPRET, )
__MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_MREINTERPRET_X2, )

#else
#define __MATRIX_GENERIC_BY_TYPEM(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)	\
  MTYPE: __riscv_th_## NAME ##_## SUFFIX					\
  TAIL

#define __MATRIX_GENERIC_BY_TYPE(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)	\
  TYPE: __riscv_th_## NAME ##_## SUFFIX						\
  TAIL

#define __MATRIX_GENERIC_BY_PTRTYPE(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)	\
  TYPE *: __riscv_th_## NAME ##_## SUFFIX,					\
  PTRTYPE: __riscv_th_## NAME ##_## SUFFIX					\
  TAIL

#define __riscv_th_mld(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_PTRTYPE, mld)) (base, ##__VA_ARGS__))
#define __riscv_th_msld(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_PTRTYPE, msld)) (base, ##__VA_ARGS__))
#define __riscv_th_mst(base, stride, value, row, col) (_Generic((value), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mst)) (base, stride, value, row, col))
#define __riscv_th_msst(base, stride, value, row, col) (_Generic((value), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, msst)) (base, stride, value, row, col))

#define __riscv_th_mmov_mv(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmov_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_mmov_m_x(base, ...) (_Generic((base), __MATRIX_INT_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmov_m_x)) (base, ##__VA_ARGS__))
#define __riscv_th_mmov_x_m(base, ...) (_Generic((base), __MATRIX_INT_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmov_x_m)) (base, ##__VA_ARGS__))
#define __riscv_th_mdup_m_x(base, ...) (_Generic((base), __MATRIX_INT_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPE, mdup_m_x)) (base, ##__VA_ARGS__))

#define __riscv_th_mset(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mset)) (base, ##__VA_ARGS__))
#define __riscv_th_mget(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mget)) (base, ##__VA_ARGS__))

#define __riscv_th_madd_mm(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, madd_mm)) (base, ##__VA_ARGS__))
#define __riscv_th_madd_mv(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, madd_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_madd_mx(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, madd_mx)) (base, ##__VA_ARGS__))

#define __riscv_th_msub_mm(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, msub_mm)) (base, ##__VA_ARGS__))
#define __riscv_th_msub_mv(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, msub_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_msub_mx(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, msub_mx)) (base, ##__VA_ARGS__))

#define __riscv_th_msra_mm(base, ...) (_Generic((base), __MATRIX_SI_DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, msra_mm)) (base, ##__VA_ARGS__))
#define __riscv_th_msra_mv(base, ...) (_Generic((base), __MATRIX_SI_DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, msra_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_msra_mx(base, ...) (_Generic((base), __MATRIX_SI_DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, msra_mx)) (base, ##__VA_ARGS__))

#define __riscv_th_mn4clip_mm(base, ...) (_Generic((base), __MATRIX_SI_DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mn4clip_mm)) (base, ##__VA_ARGS__))
#define __riscv_th_mn4clip_mv(base, ...) (_Generic((base), __MATRIX_SI_DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mn4clip_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_mn4clip_mx(base, ...) (_Generic((base), __MATRIX_SI_DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mn4clip_mx)) (base, ##__VA_ARGS__))

#define __riscv_th_mn4clipu_mm(base, ...) (_Generic((base), __MATRIX_USI_UDI__TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mn4clipu_mm)) (base, ##__VA_ARGS__))
#define __riscv_th_mn4clipu_mv(base, ...) (_Generic((base), __MATRIX_USI_UDI__TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mn4clipu_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_mn4clipu_mx(base, ...) (_Generic((base), __MATRIX_USI_UDI__TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mn4clipu_mx)) (base, ##__VA_ARGS__))

#define __riscv_th_mmul_mm(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmul_mm)) (base, ##__VA_ARGS__))
#define __riscv_th_mmul_mv(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmul_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_mmul_mx(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmul_mx)) (base, ##__VA_ARGS__))

#define __riscv_th_mmulh_mm(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmulh_mm)) (base, ##__VA_ARGS__))
#define __riscv_th_mmulh_mv(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmulh_mv)) (base, ##__VA_ARGS__))
#define __riscv_th_mmulh_mx(base, ...) (_Generic((base), __MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmulh_mx)) (base, ##__VA_ARGS__))

#define __riscv_th_fmmacc(base, ...) (_Generic((base), __MATRIX_HF_SF_2DF_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, fmmacc)) (base, ##__VA_ARGS__))
#define __riscv_th_fwmmacc(base, ...) (_Generic((base), __MATRIX_SF_2DF_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, fwmmacc)) (base, ##__VA_ARGS__))

#define __riscv_th_mmaqa(base, ...) (_Generic((base), __MATRIX_SI_2DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmaqa)) (base, ##__VA_ARGS__))
#define __riscv_th_mmaqau(base, ...) (_Generic((base), __MATRIX_SI_2DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmaqau)) (base, ##__VA_ARGS__))
#define __riscv_th_mmaqaus(base, ...) (_Generic((base), __MATRIX_SI_2DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmaqaus)) (base, ##__VA_ARGS__))
#define __riscv_th_mmaqasu(base, ...) (_Generic((base), __MATRIX_SI_2DI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mmaqasu)) (base, ##__VA_ARGS__))

#define __riscv_th_pmmaqa(base, ...) (_Generic((base), __MATRIX_SI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, pmmaqa)) (base, ##__VA_ARGS__))
#define __riscv_th_pmmaqau(base, ...) (_Generic((base), __MATRIX_SI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, pmmaqau)) (base, ##__VA_ARGS__))
#define __riscv_th_pmmaqaus(base, ...) (_Generic((base), __MATRIX_SI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, pmmaqaus)) (base, ##__VA_ARGS__))
#define __riscv_th_pmmaqasu(base, ...) (_Generic((base), __MATRIX_SI_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, pmmaqasu)) (base, ##__VA_ARGS__))

#define __riscv_th_mreinterpret_i8(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM,  mreinterpret_i8)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_i16(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_i16)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_i32(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_i32)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_i64(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_i64)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u8(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM,  mreinterpret_u8)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u16(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_u16)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u32(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_u32)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u64(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_u64)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_f16(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_f16)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_f32(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_f32)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_f64(base, ...) (_Generic((base), __MATRIX_ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_f64)) (base, ##__VA_ARGS__))

#define __riscv_th_mreinterpret_i8x2(base, ...) (_Generic((base),  __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM,  mreinterpret_i8x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_i16x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_i16x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_i32x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_i32x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_i64x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_i64x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u8x2(base, ...) (_Generic((base),  __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM,  mreinterpret_u8x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u16x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_u16x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u32x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_u32x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_u64x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_u64x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_f16x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_f16x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_f32x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_f32x2)) (base, ##__VA_ARGS__))
#define __riscv_th_mreinterpret_f64x2(base, ...) (_Generic((base), __MATRIX_2ALL_TYPE_ITERATOR(__MATRIX_GENERIC_BY_TYPEM, mreinterpret_f64x2)) (base, ##__VA_ARGS__))
#endif /* __cplusplus */

#endif
#endif /* _GCC_RISCV_MATRIX_H */
