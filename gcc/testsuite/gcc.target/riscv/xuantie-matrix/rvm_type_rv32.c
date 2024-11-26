/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d" } */

#include "rvm_common.h"

#define DEFINE_MATRIX_TYPE(PREFIX)		\
  mint8##PREFIX##_t mint8##PREFIX##_var;	\
  mint16##PREFIX##_t mint16##PREFIX##_var;	\
  mint32##PREFIX##_t mint32##PREFIX##_var;	\
  mint64##PREFIX##_t mint64##PREFIX##_var;	\
  muint8##PREFIX##_t muint8##PREFIX##_var;	\
  muint16##PREFIX##_t muint16##PREFIX##_var;	\
  muint32##PREFIX##_t muint32##PREFIX##_var;	\
  muint64##PREFIX##_t muint64##PREFIX##_var;	\
  mfloat16##PREFIX##_t mfloat16##PREFIX##_var;	\
  mfloat32##PREFIX##_t mfloat32##PREFIX##_var;	\
  mfloat64##PREFIX##_t mfloat64##PREFIX##_var;

void foo ()
{
  DEFINE_MATRIX_TYPE()
  DEFINE_MATRIX_TYPE(x2)
  DEFINE_MATRIX_TYPE(x4)
  DEFINE_MATRIX_TYPE(x8)
}
