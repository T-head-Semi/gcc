/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d" } */

#include "rvm_common.h"

#define __MREINTERPRET_FUNCLIST(MTYPE, SUFFIX)												\
  MTYPE test_mreinterpret_i8_##SUFFIX  (mint8_t    src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_i16_##SUFFIX (mint16_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_i32_##SUFFIX (mint32_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_i64_##SUFFIX (mint64_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u8_##SUFFIX  (muint8_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u16_##SUFFIX (muint16_t  src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u32_##SUFFIX (muint32_t  src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u64_##SUFFIX (muint64_t  src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_f16_##SUFFIX (mfloat16_t src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_f32_##SUFFIX (mfloat32_t src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_f64_##SUFFIX (mfloat64_t src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\

#define __MREINTERPRET_ALL_TYPE_ITERATOR(MACRO, ...)						\
  MACRO(mint8_t, i8 )										\
  MACRO(mint16_t, i16)										\
  MACRO(mint32_t, i32)										\
  MACRO(mint64_t, i64)										\
  MACRO(muint8_t, u8 )										\
  MACRO(muint16_t, u16)										\
  MACRO(muint32_t, u32)										\
  MACRO(muint64_t, u64)										\
  MACRO(mfloat16_t, f16)									\
  MACRO(mfloat32_t, f32)									\
  MACRO(mfloat64_t, f64)

__MREINTERPRET_ALL_TYPE_ITERATOR(__MREINTERPRET_FUNCLIST)

#define __MREINTERPRET_X2_FUNCLIST(MTYPE, SUFFIX)											\
  MTYPE test_mreinterpret_i8x2_##SUFFIX  (mint8x2_t    src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_i16x2_##SUFFIX (mint16x2_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_i32x2_##SUFFIX (mint32x2_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_i64x2_##SUFFIX (mint64x2_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u8x2_##SUFFIX  (muint8x2_t   src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u16x2_##SUFFIX (muint16x2_t  src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u32x2_##SUFFIX (muint32x2_t  src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_u64x2_##SUFFIX (muint64x2_t  src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_f16x2_##SUFFIX (mfloat16x2_t src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_f32x2_##SUFFIX (mfloat32x2_t src){  return __riscv_th_mreinterpret_##SUFFIX (src);}				\
  MTYPE test_mreinterpret_f64x2_##SUFFIX (mfloat64x2_t src){  return __riscv_th_mreinterpret_##SUFFIX (src);}

#define __MREINTERPRET_ALL_X2_TYPE_ITERATOR(MACRO, ...)						\
  MACRO(mint8x2_t, i8x2)									\
  MACRO(mint16x2_t, i16x2)									\
  MACRO(mint32x2_t, i32x2)									\
  MACRO(mint64x2_t, i64x2)									\
  MACRO(muint8x2_t, u8x2)									\
  MACRO(muint16x2_t, u16x2)									\
  MACRO(muint32x2_t, u32x2)									\
  MACRO(muint64x2_t, u64x2)									\
  MACRO(mfloat16x2_t, f16x2)									\
  MACRO(mfloat32x2_t, f32x2)									\
  MACRO(mfloat64x2_t, f64x2)

__MREINTERPRET_ALL_X2_TYPE_ITERATOR(__MREINTERPRET_X2_FUNCLIST)