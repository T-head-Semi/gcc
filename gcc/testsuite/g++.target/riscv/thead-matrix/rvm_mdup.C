/* { dg-do compile } */
/* { dg-options "-march=rv64gcxtheadmatrix -mabi=lp64d -O2" } */

#include "rvm_common.h"


mint8_t test_int8_mdup_m_x (int8_t src)
{
  return __riscv_th_mdup_m_x (src);
}

muint8_t test_uint8_mdup_m_x (uint8_t src)
{
  return __riscv_th_mdup_m_x (src);
}

mint16_t test_int16_mdup_m_x (int16_t src)
{
  return __riscv_th_mdup_m_x (src);
}

muint16_t test_uint16_mdup_m_x (uint16_t src)
{
  return __riscv_th_mdup_m_x (src);
}

mint32_t test_int32_mdup_m_x (int32_t src)
{
  return __riscv_th_mdup_m_x (src);
}

muint32_t test_uint32_mdup_m_x (uint32_t src)
{
  return __riscv_th_mdup_m_x (src);
}

mint64_t test_int64_mdup_m_x (int64_t src)
{
  return __riscv_th_mdup_m_x (src);
}

muint64_t test_uint64_mdup_m_x (uint64_t src)
{
  return __riscv_th_mdup_m_x (src);
}

/* { dg-final { scan-assembler-times "mdupb\\.m\\.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mduph\\.m\\.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mdupw\\.m\\.x\t" 2 } } */
/* { dg-final { scan-assembler-times "mdupd\\.m\\.x\t" 2 } } */
