/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"

mint8_t test_mint8_mmov_mv (mint8_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

muint8_t test_muint8_mmov_mv (muint8_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

mint16_t test_mint16_mmov_mv (mint16_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

muint16_t test_muint16_mmov_mv (muint16_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

mint32_t test_mint32_mmov_mv (mint32_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

muint32_t test_muint32_mmov_mv (muint32_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

mint64_t test_mint64_mmov_mv (mint64_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

muint64_t test_muint64_mmov_mv (muint64_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

mfloat16_t test_mfloat16_mmov_mv (mfloat16_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

mfloat32_t test_mfloat32_mmov_mv (mfloat32_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

mfloat64_t test_mfloat64_mmov_mv (mfloat64_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, index);
}

mint8_t test_mint8_mmov_mv_i (mint8_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

muint8_t test_muint8_mmov_mv_i (muint8_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

mint16_t test_mint16_mmov_mv_i (mint16_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

muint16_t test_muint16_mmov_mv_i (muint16_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

mint32_t test_mint32_mmov_mv_i (mint32_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

muint32_t test_muint32_mmov_mv_i (muint32_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

mint64_t test_mint64_mmov_mv_i (mint64_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

muint64_t test_muint64_mmov_mv_i (muint64_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

mfloat16_t test_mfloat16_mmov_mv_i (mfloat16_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

mfloat32_t test_mfloat32_mmov_mv_i (mfloat32_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

mfloat64_t test_mfloat64_mmov_mv_i (mfloat64_t src, size_t index)
{
  return __riscv_th_mmov_mv (src, 1);
}

mint8_t test_mint8_mmov_m_x (mint8_t dest, int8_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

muint8_t test_muint8_mmov_m_x (muint8_t dest, uint8_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

mint16_t test_mint16_mmov_m_x (mint16_t dest, int16_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

muint16_t test_muint16_mmov_m_x (muint16_t dest, uint16_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

mint32_t test_mint32_mmov_m_x (mint32_t dest, int32_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

muint32_t test_muint32_mmov_m_x (muint32_t dest, uint32_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

mint64_t test_mint64_mmov_m_x (mint64_t dest, int64_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

muint64_t test_muint64_mmov_m_x (muint64_t dest, uint64_t src, size_t index)
{
  return __riscv_th_mmov_m_x (dest, src, index);
}

int8_t test_mint8_mmov_x_m (mint8_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

uint8_t test_muint8_mmov_x_m (muint8_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

int16_t test_mint16_mmov_x_m (mint16_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

uint16_t test_muint16_mmov_x_m (muint16_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

int32_t test_mint32_mmov_x_m (mint32_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

uint32_t test_muint32_mmov_x_m (muint32_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

int64_t test_mint64_mmov_x_m (mint64_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

uint64_t test_muint64_mmov_x_m (muint64_t src, size_t index)
{
  return __riscv_th_mmov_x_m (src, index);
}

/* { dg-final { scan-assembler-times "mmov\\.mv\\.x\t" 11 } } */
/* { dg-final { scan-assembler-times "mmov\\.mv\\.i\t" 11 } } */
/* { dg-final { scan-assembler-times "mmovb\\.m\\.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovh\\.m\\.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovw\\.m\\.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovd\\.m\\.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovb\\.x\\.m\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovh\\.x\\.m\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovw\\.x\\.m\t" 2 } } */
/* { dg-final { scan-assembler-times "mmovd\\.x\\.m\t" 2 } } */
