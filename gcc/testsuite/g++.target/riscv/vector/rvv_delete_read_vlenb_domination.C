/* { dg-do compile } */
/* { dg-options "-Wno-return-type -O3 -march=rv64gcv -mabi=lp64d" } */

#include <riscv_vector.h>

class o;
namespace aa {
class p {
public:
  int ae;
};
class ag {
  int ah(p &, const o &) const;
};
} // namespace aa
void ai(vfloat32m8_t, vfloat32m8_t *aj, vfloat32m8_t *, int k) {
  vuint32m8_t ak;
  vbool4_t al;
  vbool4_t am = vmsne_vx_u32m8_b4(vand_vx_u32m8(ak, 2, k), 0, k);
  vfloat32m8_t an, ao;
  vfloat32m8_t ap = vmerge_vvm_f32m8(am, ao, an, k);
  *aj = vmerge_vvm_f32m8(al, ap, vfneg_v_f32m8(ap, k), k);
}
vfloat32m8_t aq(vfloat32m8_t q, int k) {
  vfloat32m8_t aj, ar;
  ai(q, &aj, &ar, k);
  return aj;
}
namespace aa {
template <typename as> int at(p a, const o &) {
  as au;
  int av;
  int ae = a.ae;
  for (; av;) {
    float *aw;
    int n = ae;
    if (n) {
      int k = vsetvl_e32m8(n);
      vfloat32m8_t ax = au(ax, k);
      vse32_v_f32m8(aw, ax, k);
    }
  }
}
namespace ay {
struct az {
  vfloat32m8_t operator()(vfloat32m8_t q, int k) { return aq(q, k); }
};
} // namespace ay
int ag::ah(p &ba, const o &bb) const {
  using namespace ay;
  at<az>(ba, bb);
}
} // namespace aa

/* { dg-final { scan-assembler-times "csrr\ts11,vlenb" 1} } */
/* The RE below means a mul with s11 before csrr  s11,vlenb, and they are in different basicblock.
   The previous delete read vlenb pass will generate instructions like this, while the s11 is used
   before define, which may cause the segment fault. */ 
/* { dg-final { scan-assembler-not mul\t.*,s11.*(\n.*)*(\n\..*:)+(\n.*)*csrr\ts11,vlenb} } */
