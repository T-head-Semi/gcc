/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d" } */

#include "rvm_common.h"

#define DEFINE_MATRIX_TYPE(PREFIX)		\
  mint8##PREFIX##_t    __riscv_th_mundefined_i8##PREFIX ();	\
  mint16##PREFIX##_t   __riscv_th_mundefined_i16##PREFIX ();	\
  mint32##PREFIX##_t   __riscv_th_mundefined_i32##PREFIX ();	\
  mint64##PREFIX##_t   __riscv_th_mundefined_i64##PREFIX ();	\
  muint8##PREFIX##_t   __riscv_th_mundefined_u8##PREFIX ();	\
  muint16##PREFIX##_t  __riscv_th_mundefined_u16##PREFIX ();	\
  muint32##PREFIX##_t  __riscv_th_mundefined_u32##PREFIX ();	\
  muint64##PREFIX##_t  __riscv_th_mundefined_u64##PREFIX ();	\
  mfloat16##PREFIX##_t __riscv_th_mundefined_f16##PREFIX ();	\
  mfloat32##PREFIX##_t __riscv_th_mundefined_f32##PREFIX ();	\
  mfloat64##PREFIX##_t __riscv_th_mundefined_f64##PREFIX ();

void foo ()
{
  DEFINE_MATRIX_TYPE()
  DEFINE_MATRIX_TYPE(x2)
  DEFINE_MATRIX_TYPE(x4)
  DEFINE_MATRIX_TYPE(x8)
}
