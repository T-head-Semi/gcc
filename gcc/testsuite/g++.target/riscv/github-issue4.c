/* { dg-do compile } */
/* { dg-options "-march=rv64gcv0p7 -mabi=lp64d -O0" } */

/* The issue is coming from
     https://github.com/T-head-Semi/xuantie-gnu-toolchain/issues/5 */

#include <riscv_vector.h>

struct v_int32x4
{
  enum
  { nlanes = 4 };
  explicit v_int32x4 (vint32m1_t v)
  {
    vse32_v_i32m1 (val, v, nlanes);
  }
  operator  vint32m1_t () const
  {
    return vle32_v_i32m1 (val, nlanes);
  }
  int val[4];
};

struct v_int64x2
{
  enum
  { nlanes = 2 };
  explicit v_int64x2 (vint64m1_t v)
  {
    vse64_v_i64m1 (val, v, nlanes);
  }
  operator  vint64m1_t () const
  {
    return vle64_v_i64m1 (val, nlanes);
  }
  int64_t get0 () const
  {
    return val[0];
  }
  int64_t val[2];
};

int
v_reduce_sum (const v_int32x4 & a)
{
  vint64m1_t zero = vmv_v_x_i64m1 (0, 4);
  vint64m1_t res = vmv_v_x_i64m1 (0, 4);
  res = vwredsum_vs_i32m1_i64m1 (res, a, zero, 4);
  return (int) (v_int64x2 (res).get0 ());
}

int
test (int *src, int len)
{
  int i = 0, nz = 0;
  v_int32x4 v_zero = v_int32x4 (vmv_v_x_i32m1 (0, 4));
  v_int32x4 v_sum32 = v_int32x4 (vmv_v_x_i32m1 (0, 4));
  nz = i - v_reduce_sum (v_sum32);
  for (; i <= len - 4; i += 4)
    nz += src[i];
  return nz;
}

int
main ()
{
  return 0;
}

/* { dg-final { scan-assembler-times "vwredsum.vs" 1 } } */
