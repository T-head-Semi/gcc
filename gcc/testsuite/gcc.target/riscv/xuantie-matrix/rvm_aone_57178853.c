/* { dg-do compile } */
/* { dg-options "-march=rv64gc_xtheadmatrix -mabi=lp64d -O3" } */

#include <stdint.h>
#include <thead_matrix.h>

#define MATRIX_SIZE 64
#define BLOCK_SIZE_4 4
#define BLOCK_SIZE_8 8

int rvm_4x8_macc_fp16(float16_t *cc, float16_t *aa, float16_t *bb, int sa, int sb, int sc)
{
    long stride_a = sa * sizeof(float16_t);
    long stride_b = sb * sizeof(float16_t);
    long stride_c = sc * sizeof(float16_t);

    mfloat16_t ma = __riscv_th_mld(aa, stride_a, 4, 8);
    mfloat16_t mb0 = __riscv_th_msld(bb, stride_b, 4, 8);
    mfloat16_t mb1 = __riscv_th_msld(bb + sb * 4, stride_b, 4, 8);
    mfloat16x2_t mb = __riscv_th_mzero_f16x2();
    mb = __riscv_th_mset(mb, 0, mb0);
    mb = __riscv_th_mset(mb, 1, mb1);
    mfloat16_t mc = __riscv_th_mld(cc, stride_c, 4, 8);

    mc = __riscv_th_fmmacc(mc, ma, mb, 4, 8, 8);
    __riscv_th_mst(cc, stride_c, mc, 4, 8);

    return 0;
}

void test(float16_t *A, float16_t *B, float16_t *C_1)
{
  for (int iter = 0; iter < 10; iter++)
    rvm_4x8_macc_fp16(C_1, B, A, MATRIX_SIZE, MATRIX_SIZE, MATRIX_SIZE);
}
