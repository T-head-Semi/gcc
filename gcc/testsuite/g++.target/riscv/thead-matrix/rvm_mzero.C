/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-Os" "-Oz" } } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

#define DEFINE_MATRIX_TYPE(PREFIX)		\
  void foo_i8##PREFIX   (mint8##PREFIX##_t       *var){*var =__riscv_th_mzero_i8##PREFIX  ();}	\
  void foo_i16##PREFIX  (mint16##PREFIX##_t      *var){*var =__riscv_th_mzero_i16##PREFIX ();}	\
  void foo_i32##PREFIX  (mint32##PREFIX##_t      *var){*var =__riscv_th_mzero_i32##PREFIX ();}	\
  void foo_i64##PREFIX  (mint64##PREFIX##_t      *var){*var =__riscv_th_mzero_i64##PREFIX ();}	\
  void foo_u8##PREFIX   (muint8##PREFIX##_t      *var){*var =__riscv_th_mzero_u8##PREFIX  ();}	\
  void foo_u16##PREFIX  (muint16##PREFIX##_t     *var){*var =__riscv_th_mzero_u16##PREFIX ();}	\
  void foo_u32##PREFIX  (muint32##PREFIX##_t     *var){*var =__riscv_th_mzero_u32##PREFIX ();}	\
  void foo_u64##PREFIX  (muint64##PREFIX##_t     *var){*var =__riscv_th_mzero_u64##PREFIX ();}	\
  void foo_f16##PREFIX  (mfloat16##PREFIX##_t    *var){*var =__riscv_th_mzero_f16##PREFIX ();}	\
  void foo_f32##PREFIX  (mfloat32##PREFIX##_t    *var){*var =__riscv_th_mzero_f32##PREFIX ();}	\
  void foo_f64##PREFIX  (mfloat64##PREFIX##_t    *var){*var =__riscv_th_mzero_f64##PREFIX ();}

DEFINE_MATRIX_TYPE()
DEFINE_MATRIX_TYPE(x2)

/* { dg-final { scan-assembler-times "mzero\tm\[0-9\]+\n" 33 } } */
