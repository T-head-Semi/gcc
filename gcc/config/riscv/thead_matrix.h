/* Thead Matrix Extension instructions include file.

   Copyright (C) 2022-2024 Free Software Foundation, Inc.
   Contributed by Jin Ma <jinma@linux.alibaba.com>, T-head-Semi.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
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

typedef int16_t mrow_t;
typedef int16_t mcol_t;
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
{return (mcol_t)((__builtin_riscv_msetmcol (c * 1) >> 16 & 0xffff));}

FUNC_PREFIX
mcol_t __riscv_th_msetmcol_e16 (mcol_t c)
{return (mcol_t)((__builtin_riscv_msetmcol (c * 2) >> 16 & 0xffff)/2);}

FUNC_PREFIX
mcol_t __riscv_th_msetmcol_e32 (mcol_t c)
{return (mcol_t)((__builtin_riscv_msetmcol (c * 4) >> 16 & 0xffff)/4);}

FUNC_PREFIX
mcol_t __riscv_th_msetmcol_e64 (mcol_t c)
{return (mcol_t)((__builtin_riscv_msetmcol (c * 8) >> 16 & 0xffff)/8);}

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

#define __MATRIX_LOAD_STORE_RELOAD_MCFGK(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)			\
FUNC_PREFIX												\
MTYPE __riscv_th_mld_## SUFFIX (PTRTYPE base, long stride, mrow_t row, mcol_t col)			\
{return __builtin_riscv_mld_## SUFFIX(base, stride, row, col * sizeof (TYPE));}				\
FUNC_PREFIX												\
MTYPE __riscv_th_msld_## SUFFIX (PTRTYPE base, long stride, mrow_t row, mcol_t col)			\
{return __builtin_riscv_msld_## SUFFIX(base, stride, row, col * sizeof (TYPE));}			\
FUNC_PREFIX												\
void __riscv_th_mst_## SUFFIX (PTRTYPE base, long stride, MTYPE value, mrow_t row, mcol_t col)		\
{return __builtin_riscv_mst_## SUFFIX(base, stride, value, row, col * sizeof (TYPE));}			\
FUNC_PREFIX												\
void __riscv_th_msst_## SUFFIX (PTRTYPE base, long stride, MTYPE value, mrow_t row, mcol_t col)		\
{return __builtin_riscv_msst_## SUFFIX(base, stride, value, row, col * sizeof (TYPE));}

__MATRIX_ALL_TYPE_ITERATOR(__MATRIX_LOAD_STORE_RELOAD_MCFGK, )

#define __MATRIX_ALU_RELOAD_MCFGK(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)				\
FUNC_PREFIX												\
MTYPE __riscv_th_## NAME ##_mm_## SUFFIX (MTYPE src1, MTYPE src2, mrow_t row, mcol_t col)		\
{return __builtin_riscv_## NAME ##_mm_## SUFFIX(src1, src2, row, col * sizeof (TYPE));}			\
FUNC_PREFIX												\
MTYPE __riscv_th_## NAME ##_mv_## SUFFIX (MTYPE src1, MTYPE src2, size_t index, mrow_t row, mcol_t col)	\
{return __builtin_riscv_## NAME ##_mv_## SUFFIX(src1, src2, index, row, col * sizeof (TYPE));}

#define __MATRIX_ALU_MX_RELOAD_MCFGK(MTYPE, TYPE, PTRTYPE, SUFFIX, TAIL, NAME)				\
FUNC_PREFIX												\
MTYPE __riscv_th_## NAME ##_mx_## SUFFIX (MTYPE src1, TYPE src2, mrow_t row, mcol_t col)		\
{return __builtin_riscv_## NAME ##_mx_## SUFFIX(src1, src2, row, col * sizeof (TYPE));}

__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_RELOAD_MCFGK, madd)
__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_RELOAD_MCFGK, msub)
__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_RELOAD_MCFGK, mmul)
__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_RELOAD_MCFGK, mmulh)

#if defined(__riscv_xlen) && __riscv_xlen == 64
__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, madd)
__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, msub)
__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, mmul)
__MATRIX_SI_USI_DI_UDI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, mmulh)
#else
__MATRIX_SI_USI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, madd)
__MATRIX_SI_USI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, msub)
__MATRIX_SI_USI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, mmul)
__MATRIX_SI_USI_TYPE_ITERATOR(__MATRIX_ALU_MX_RELOAD_MCFGK, mmulh)
#endif

FUNC_PREFIX
mint32_t __riscv_th_msra_mm_i32 (mint32_t src1, muint32_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_msra_mm_i32(src1, src2, row, col * sizeof (int32_t)); }
FUNC_PREFIX
mint64_t __riscv_th_msra_mm_i64 (mint64_t src1, muint64_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_msra_mm_i64(src1, src2, row, col * sizeof (int64_t)); }
FUNC_PREFIX
mint32_t __riscv_th_msra_mv_i32 (mint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col)
{return __builtin_riscv_msra_mv_i32(src1, src2, index, row, col * sizeof (int32_t)); }
FUNC_PREFIX
mint64_t __riscv_th_msra_mv_i64 (mint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col)
{return __builtin_riscv_msra_mv_i64(src1, src2, index, row, col * sizeof (int64_t)); }
FUNC_PREFIX
mint32_t __riscv_th_msra_mx_i32 (mint32_t src1, uint32_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_msra_mx_i32(src1, src2, row, col * sizeof (int32_t)); }

FUNC_PREFIX
mint8_t  __riscv_th_mn4clip_mm_i32 (mint32_t src1, muint32_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clip_mm_i32 (src1, src2, row, col * sizeof (int32_t));}
FUNC_PREFIX
mint8_t  __riscv_th_mn4clip_mm_i64 (mint64_t src1, muint64_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clip_mm_i64 (src1, src2, row, col * sizeof (int64_t));}

FUNC_PREFIX
mint8_t  __riscv_th_mn4clip_mv_i32 (mint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clip_mv_i32 (src1, src2, index, row, col * sizeof (int32_t));}
FUNC_PREFIX
mint8_t  __riscv_th_mn4clip_mv_i64 (mint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clip_mv_i64 (src1, src2, index, row, col * sizeof (int64_t));}

FUNC_PREFIX
mint8_t  __riscv_th_mn4clip_mx_i32 (mint32_t src1, uint32_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clip_mx_i32 (src1, src2, row, col * sizeof (int32_t));}

FUNC_PREFIX
muint8_t  __riscv_th_mn4clipu_mm_u32 (muint32_t src1, muint32_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clipu_mm_u32 (src1, src2, row, col * sizeof (uint32_t));}
FUNC_PREFIX
muint8_t  __riscv_th_mn4clipu_mm_u64 (muint64_t src1, muint64_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clipu_mm_u64 (src1, src2, row, col * sizeof (uint64_t));}

FUNC_PREFIX
muint8_t  __riscv_th_mn4clipu_mv_u32 (muint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clipu_mv_u32 (src1, src2, index, row, col * sizeof (uint32_t));}
FUNC_PREFIX
muint8_t  __riscv_th_mn4clipu_mv_u64 (muint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clipu_mv_u64 (src1, src2, index, row, col * sizeof (uint64_t));}

FUNC_PREFIX
muint8_t  __riscv_th_mn4clipu_mx_u32 (muint32_t src1, uint32_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clipu_mx_u32 (src1, src2, row, col * sizeof (uint32_t));}

#if defined(__riscv_xlen) && __riscv_xlen == 64
FUNC_PREFIX
mint64_t __riscv_th_msra_mx_i64 (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_msra_mx_i64(src1, src2, row, col * sizeof (int64_t)); }

FUNC_PREFIX
mint8_t  __riscv_th_mn4clip_mx_i64 (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clip_mx_i64 (src1, src2, row, col * sizeof (int64_t));}

FUNC_PREFIX
muint8_t  __riscv_th_mn4clipu_mx_u64 (muint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{return __builtin_riscv_mn4clipu_mx_u64 (src1, src2, row, col * sizeof (uint64_t));}
#endif

FUNC_PREFIX
mfloat16_t __riscv_th_fmmacc_f16 (mfloat16_t dest, mfloat16_t src1, mfloat16x2_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_fmmacc_f16 (dest, src1, src2, row1, row2, col * sizeof (float16_t));} 
FUNC_PREFIX
mfloat32_t __riscv_th_fmmacc_f32 (mfloat32_t dest, mfloat32_t src1, mfloat32_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_fmmacc_f32 (dest, src1, src2, row1, row2, col * sizeof (float32_t));} 
FUNC_PREFIX
mfloat64x2_t __riscv_th_fmmacc_f64x2 (mfloat64x2_t dest, mfloat64_t src1, mfloat64_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_fmmacc_f64x2 (dest, src1, src2, row1, row2, col * sizeof (float64_t));} 

FUNC_PREFIX
mfloat32_t __riscv_th_fwmmacc_f32 (mfloat32_t dest, mfloat16_t src1, mfloat16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_fwmmacc_f32  (dest, src1, src2, row1, row2, col * sizeof (float16_t));} 
FUNC_PREFIX
mfloat64x2_t __riscv_th_fwmmacc_f64x2 (mfloat64x2_t dest, mfloat32_t src1, mfloat32_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_fwmmacc_f64x2 (dest, src1, src2, row1, row2, col * sizeof (float32_t));} 

FUNC_PREFIX
mint32_t  __riscv_th_mmaqa_i32 (mint32_t dest, mint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqa_i32 (dest, src1, src2, row1, row2, col* sizeof (int8_t));}
FUNC_PREFIX
mint64x2_t  __riscv_th_mmaqa_i64x2 (mint64x2_t dest, mint16_t src1, mint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqa_i64x2 (dest, src1, src2, row1, row2, col* sizeof (int16_t));}

FUNC_PREFIX
mint32_t  __riscv_th_mmaqau_i32 (mint32_t dest, muint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqau_i32 (dest, src1, src2, row1, row2, col* sizeof (uint8_t));}
FUNC_PREFIX
mint64x2_t  __riscv_th_mmaqau_i64x2 (mint64x2_t dest, muint16_t src1, muint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqau_i64x2 (dest, src1, src2, row1, row2, col* sizeof (uint16_t));}

FUNC_PREFIX
mint32_t  __riscv_th_mmaqaus_i32 (mint32_t dest, muint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqaus_i32 (dest, src1, src2, row1, row2, col* sizeof (int8_t));}
FUNC_PREFIX
mint64x2_t  __riscv_th_mmaqaus_i64x2 (mint64x2_t dest, muint16_t src1, mint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqaus_i64x2 (dest, src1, src2, row1, row2, col* sizeof (int16_t));}

FUNC_PREFIX
mint32_t  __riscv_th_mmaqasu_i32 (mint32_t dest, mint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqasu_i32 (dest, src1, src2, row1, row2, col* sizeof (int8_t));}
FUNC_PREFIX
mint64x2_t  __riscv_th_mmaqasu_i64x2 (mint64x2_t dest, mint16_t src1, muint16_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_mmaqasu_i64x2 (dest, src1, src2, row1, row2, col* sizeof (int16_t));}

FUNC_PREFIX
mint32_t  __riscv_th_pmmaqa_i32 (mint32_t dest, mint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_pmmaqa_i32 (dest, src1, src2, row1, row2, col* sizeof (int8_t));}

FUNC_PREFIX
mint32_t  __riscv_th_pmmaqau_i32 (mint32_t dest, muint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_pmmaqau_i32 (dest, src1, src2, row1, row2, col* sizeof (uint8_t));}

FUNC_PREFIX
mint32_t  __riscv_th_pmmaqaus_i32 (mint32_t dest, muint8_t src1, mint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_pmmaqaus_i32 (dest, src1, src2, row1, row2, col* sizeof (int8_t));}

FUNC_PREFIX
mint32_t  __riscv_th_pmmaqasu_i32 (mint32_t dest, mint8_t src1, muint8_t src2, mrow_t row1, mrow_t row2, mcol_t col)
{return __builtin_riscv_pmmaqasu_i32 (dest, src1, src2, row1, row2, col* sizeof (int8_t));}

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
  __riscv_th_mst_##SUFFIX((uint64_t *)&res, 8, temp, row, 1);			\
  return __riscv_th_mld_##SUFFIX((uint64_t *)&res, 0, row, col);		\
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

#endif
#endif /* _GCC_RISCV_MATRIX_H */
