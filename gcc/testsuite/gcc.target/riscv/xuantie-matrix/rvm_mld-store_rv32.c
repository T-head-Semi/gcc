/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

mint8_t test_int8_mld (int8_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

muint8_t test_uint8_mld (uint8_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

mint16_t test_int16_mld (int16_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

muint16_t test_uint16_mld (uint16_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

mint32_t test_int32_mld (int32_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

muint32_t test_uint32_mld (uint32_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

mint64_t test_int64_mld (int64_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

muint64_t test_uint64_mld (uint64_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

mfloat16_t test_float16_mld (float16_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

mfloat32_t test_float32_mld (float32_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

mfloat64_t test_float64_mld (float64_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_mld (base, stride, row, col);
}

mint8_t test_int8_msld (int8_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

muint8_t test_uint8_msld (uint8_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

mint16_t test_int16_msld (int16_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

muint16_t test_uint16_msld (uint16_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

mint32_t test_int32_msld (int32_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

muint32_t test_uint32_msld (uint32_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

mint64_t test_int64_msld (int64_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

muint64_t test_uint64_msld (uint64_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

mfloat16_t test_float16_msld (float16_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

mfloat32_t test_float32_msld (float32_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

mfloat64_t test_float64_msld (float64_t *base, long stride, mrow_t row, mcol_t col)
{
  return __riscv_th_msld (base, stride, row, col);
}

void test_int8_mst (int8_t *base, long stride, mint8_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_uint8_mst (uint8_t *base, long stride, muint8_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_int16_mst (int16_t *base, long stride, mint16_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_uint16_mst (uint16_t *base, long stride, muint16_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_int32_mst (int32_t *base, long stride, mint32_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_uint32_mst (uint32_t *base, long stride, muint32_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_int64_mst (int64_t *base, long stride, mint64_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_uint64_mst (uint64_t *base, long stride, muint64_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_float16_mst (float16_t *base, long stride, mfloat16_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_float32_mst (float32_t *base, long stride, mfloat32_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_float64_mst (float64_t *base, long stride, mfloat64_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_mst (base, stride, value, row, col);
}

void test_int8_msst (int8_t *base, long stride, mint8_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_uint8_msst (uint8_t *base, long stride, muint8_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_int16_msst (int16_t *base, long stride, mint16_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_uint16_msst (uint16_t *base, long stride, muint16_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_int32_msst (int32_t *base, long stride, mint32_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_uint32_msst (uint32_t *base, long stride, muint32_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_int64_msst (int64_t *base, long stride, mint64_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_uint64_msst (uint64_t *base, long stride, muint64_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_float16_msst (float16_t *base, long stride, mfloat16_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_float32_msst (float32_t *base, long stride, mfloat32_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

void test_float64_msst (float64_t *base, long stride, mfloat64_t value, mrow_t row, mcol_t col)
{
  return __riscv_th_msst (base, stride, value, row, col);
}

/* { dg-final { scan-assembler-times "mldb\t" 2 } } */
/* { dg-final { scan-assembler-times "mstb\t" 2 } } */
/* { dg-final { scan-assembler-times "mldh\t" 3 } } */
/* { dg-final { scan-assembler-times "msth\t" 3 } } */
/* { dg-final { scan-assembler-times "mldw\t" 3 } } */
/* { dg-final { scan-assembler-times "mstw\t" 3 } } */
/* { dg-final { scan-assembler-times "mldd\t" 3 } } */
/* { dg-final { scan-assembler-times "mstd\t" 3 } } */
/* { dg-final { scan-assembler-times "msldb\t" 2 } } */
/* { dg-final { scan-assembler-times "msstb\t" 2 } } */
/* { dg-final { scan-assembler-times "msldh\t" 3 } } */
/* { dg-final { scan-assembler-times "mssth\t" 3 } } */
/* { dg-final { scan-assembler-times "msldw\t" 3 } } */
/* { dg-final { scan-assembler-times "msstw\t" 3 } } */
/* { dg-final { scan-assembler-times "msldd\t" 3 } } */
/* { dg-final { scan-assembler-times "msstd\t" 3 } } */
