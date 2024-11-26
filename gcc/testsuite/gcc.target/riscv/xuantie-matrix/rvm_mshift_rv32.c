/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheadmatrix -mabi=ilp32d -O2" } */

#include "rvm_common.h"

mint32_t test_mint32_msra_mm (mint32_t src1, muint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msra_mm (src1, src2, row, col);
}

mint64_t test_mint64_msra_mm (mint64_t src1, muint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msra_mm (src1, src2, row, col);
}

mint32_t test_mint32_msra_mv (mint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_msra_mv (src1, src2, index, row, col);
}

mint64_t test_mint64_msra_mv (mint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_msra_mv (src1, src2, index, row, col);
}

mint32_t test_mint32_msra_mx (mint32_t src1, uint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msra_mx (src1, src2, row, col);
}

mint64_t test_mint64_msra_mx (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_msra_mx (src1, src2, row, col);
}

mint8_t test_mint32_mn4clip_mm (mint32_t src1, muint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clip_mm (src1, src2, row, col);
}

mint8_t test_mint64_mn4clip_mm (mint64_t src1, muint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clip_mm (src1, src2, row, col);
}


mint8_t test_mint32_mn4clip_mv (mint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clip_mv (src1, src2, index, row, col);
}

mint8_t test_mint64_mn4clip_mv (mint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clip_mv (src1, src2, index, row, col);
}

mint8_t test_mint32_mn4clip_mx (mint32_t src1, uint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clip_mx (src1, src2, row, col);
}

mint8_t test_mint64_mn4clip_mx (mint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clip_mx (src1, src2, row, col);
}

muint8_t test_muint32_mn4clipu_mm (muint32_t src1, muint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clipu_mm (src1, src2, row, col);
}

muint8_t test_muint64_mn4clipu_mm (muint64_t src1, muint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clipu_mm (src1, src2, row, col);
}

muint8_t test_muint32_mn4clipu_mv (muint32_t src1, muint32_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clipu_mv (src1, src2, index, row, col);
}

muint8_t test_muint64_mn4clipu_mv (muint64_t src1, muint64_t src2, size_t index, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clipu_mv (src1, src2, index, row, col);
}

muint8_t test_muint32_mn4clipu_mx (muint32_t src1, uint32_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clipu_mx (src1, src2, row, col);
}

muint8_t test_muint64_mn4clipu_mx (muint64_t src1, uint64_t src2, mrow_t row, mcol_t col)
{
  return __riscv_th_mn4clipu_mx (src1, src2, row, col);
}

/* { dg-final { scan-assembler-times "msra\\.\[sd\]\\.mm\tm\[0-9\]+" 3 } } */
/* { dg-final { scan-assembler-times "msra\\.\[sd\]\\.mv.x\tm\[0-9\]+" 2 } } */
/* { dg-final { scan-assembler-times "msra\\.\[sd\]\\.mx\tm\[0-9\]+" 1 } } */

/* { dg-final { scan-assembler-times "mn4clip\\.\[sd\]\\.mm\tm\[0-9\]+" 3 } } */
/* { dg-final { scan-assembler-times "mn4clip\\.\[sd\]\\.mv.x\tm\[0-9\]+" 2 } } */
/* { dg-final { scan-assembler-times "mn4clip\\.\[sd\]\\.mx\tm\[0-9\]+" 1 } } */

/* { dg-final { scan-assembler-times "mn4clipu\\.\[sd\]\\.mm\tm\[0-9\]+" 3 } } */
/* { dg-final { scan-assembler-times "mn4clipu\\.\[sd\]\\.mv.x\tm\[0-9\]+" 2 } } */
/* { dg-final { scan-assembler-times "mn4clipu\\.\[sd\]\\.mx\tm\[0-9\]+" 1 } } */
