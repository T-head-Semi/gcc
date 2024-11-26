/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d" } */

#include "rvm_common.h"

mint8x2_t test_mint8x2 (mint8x2_t    src, size_t index, mint8_t    value){  return __riscv_th_mset (src, 0, value);}
mint16x2_t test_mint16x2 (mint16x2_t   src, size_t index, mint16_t   value){  return __riscv_th_mset (src, 0, value);}
mint32x2_t test_mint32x2 (mint32x2_t   src, size_t index, mint32_t   value){  return __riscv_th_mset (src, 0, value);}
mint64x2_t test_mint64x2 (mint64x2_t   src, size_t index, mint64_t   value){  return __riscv_th_mset (src, 0, value);}
muint8x2_t test_muint8x2 (muint8x2_t   src, size_t index, muint8_t   value){  return __riscv_th_mset (src, 0, value);}
muint16x2_t test_muint16x2 (muint16x2_t  src, size_t index, muint16_t  value){  return __riscv_th_mset (src, 0, value);}
muint32x2_t test_muint32x2 (muint32x2_t  src, size_t index, muint32_t  value){  return __riscv_th_mset (src, 0, value);}
muint64x2_t test_muint64x2 (muint64x2_t  src, size_t index, muint64_t  value){  return __riscv_th_mset (src, 0, value);}
mfloat16x2_t test_mfloat16x2 (mfloat16x2_t src, size_t index, mfloat16_t value){  return __riscv_th_mset (src, 0, value);}
mfloat32x2_t test_mfloat32x2 (mfloat32x2_t src, size_t index, mfloat32_t value){  return __riscv_th_mset (src, 0, value);}
mfloat64x2_t test_mfloat64x2 (mfloat64x2_t src, size_t index, mfloat64_t value){  return __riscv_th_mset (src, 0, value);}

mint32x2_t test_mint32x2_error1 (mint32x2_t   src, size_t index, mint32_t   value){  return __riscv_th_mset (src, index, value);}
/* { dg-error "argument 2 of '__riscv_th_mset_i32x2' must be an integer constant expression*" "" { target *-*-* } 0 } */
mint32x2_t test_mint32x2_error2 (mint32x2_t   src, size_t index, mint32_t   value){  return __riscv_th_mset (src, 2, value);}
/* { dg-error "passing 2 to argument 2 of '__riscv_th_mset_i32x2', which expects a value in the range \[0, 1\]*" "" { target *-*-* } 0 } */

mint8_t test_mint8 (mint8x2_t    src, size_t index){  return __riscv_th_mget (src, 1);}
mint16_t test_mint16 (mint16x2_t   src, size_t index){  return __riscv_th_mget (src, 1);}
mint32_t test_mint32 (mint32x2_t   src, size_t index){  return __riscv_th_mget (src, 1);}
mint64_t test_mint64 (mint64x2_t   src, size_t index){  return __riscv_th_mget (src, 1);}
muint8_t test_muint8 (muint8x2_t   src, size_t index){  return __riscv_th_mget (src, 1);}
muint16_t test_muint16 (muint16x2_t  src, size_t index){  return __riscv_th_mget (src, 1);}
muint32_t test_muint32 (muint32x2_t  src, size_t index){  return __riscv_th_mget (src, 1);}
muint64_t test_muint64 (muint64x2_t  src, size_t index){  return __riscv_th_mget (src, 1);}
mfloat16_t test_mfloat16 (mfloat16x2_t src, size_t index){  return __riscv_th_mget (src, 1);}
mfloat32_t test_mfloat32 (mfloat32x2_t src, size_t index){  return __riscv_th_mget (src, 1);}
mfloat64_t test_mfloat64 (mfloat64x2_t src, size_t index){  return __riscv_th_mget (src, 1);}

mint32_t test_mint32_error1 (mint32x2_t   src, size_t index){  return __riscv_th_mget (src, index);}
/* { dg-error "argument 2 of '__riscv_th_mget_i32x2' must be an integer constant expression*" "" { target *-*-* } 0 } */
mint32_t test_mint32_error2 (mint32x2_t   src, size_t index){  return __riscv_th_mget (src, 2);}
/* { dg-error "passing 2 to argument 2 of '__riscv_th_mget_i32x2', which expects a value in the range \[0, 1\]*" "" { target *-*-* } 0 } */
