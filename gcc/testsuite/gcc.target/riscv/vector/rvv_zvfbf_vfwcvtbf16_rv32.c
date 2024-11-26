/* { dg-do compile } */
/* { dg-options "-march=rv32gcv_zvfbfmin -mabi=ilp32d -O2" } */

#include <riscv_vector.h>

vfloat32mf2_t testvfwcvtbf16_f_f_v_f32mf2(vbfloat16mf4_t src, size_t vl){ return vfwcvtbf16_f_f_v_f32mf2(src,vl);}
vfloat32m1_t testvfwcvtbf16_f_f_v_f32m1(vbfloat16mf2_t src, size_t vl){ return vfwcvtbf16_f_f_v_f32m1(src,vl);}
vfloat32m2_t testvfwcvtbf16_f_f_v_f32m2(vbfloat16m1_t src, size_t vl){ return vfwcvtbf16_f_f_v_f32m2(src,vl);}
vfloat32m4_t testvfwcvtbf16_f_f_v_f32m4(vbfloat16m2_t src, size_t vl){ return vfwcvtbf16_f_f_v_f32m4(src,vl);}
vfloat32m8_t testvfwcvtbf16_f_f_v_f32m8(vbfloat16m4_t src, size_t vl){ return vfwcvtbf16_f_f_v_f32m8(src,vl);}
// masked functions
vfloat32mf2_t test_vfwcvtbf16_f_f_v_f32mf2_m (vbool64_t mask, vbfloat16mf4_t src, size_t vl){  return vfwcvtbf16_f_f_v_f32mf2_m (mask, src, vl);}
vfloat32m1_t test_vfwcvtbf16_f_f_v_f32m1_m (vbool32_t mask, vbfloat16mf2_t src, size_t vl){  return vfwcvtbf16_f_f_v_f32m1_m (mask, src, vl);}
vfloat32m2_t test_vfwcvtbf16_f_f_v_f32m2_m (vbool16_t mask, vbfloat16m1_t src, size_t vl){  return vfwcvtbf16_f_f_v_f32m2_m (mask, src, vl);}
vfloat32m4_t test_vfwcvtbf16_f_f_v_f32m4_m (vbool8_t mask, vbfloat16m2_t src, size_t vl){  return vfwcvtbf16_f_f_v_f32m4_m (mask, src, vl);}
vfloat32m8_t test_vfwcvtbf16_f_f_v_f32m8_m (vbool4_t mask, vbfloat16m4_t src, size_t vl){  return vfwcvtbf16_f_f_v_f32m8_m (mask, src, vl);}

vbfloat16mf4_t testvfncvtbf16_f_f_w_bf16mf4(vfloat32mf2_t src, size_t vl){ return vfncvtbf16_f_f_w_bf16mf4(src,vl);}
vbfloat16mf2_t testvfncvtbf16_f_f_w_bf16mf2(vfloat32m1_t src, size_t vl){ return vfncvtbf16_f_f_w_bf16mf2(src,vl);}
vbfloat16m1_t testvfncvtbf16_f_f_w_bf16m1(vfloat32m2_t src, size_t vl){ return vfncvtbf16_f_f_w_bf16m1(src,vl);}
vbfloat16m2_t testvfncvtbf16_f_f_w_bf16m2(vfloat32m4_t src, size_t vl){ return vfncvtbf16_f_f_w_bf16m2(src,vl);}
vbfloat16m4_t testvfncvtbf16_f_f_w_bf16m4(vfloat32m8_t src, size_t vl){ return vfncvtbf16_f_f_w_bf16m4(src,vl);}
// masked functions
vbfloat16mf4_t test_vfncvtbf16_f_f_w_bf16mf4_m (vbool64_t mask, vfloat32mf2_t src, size_t vl){  return vfncvtbf16_f_f_w_bf16mf4_m (mask, src, vl);}
vbfloat16mf2_t test_vfncvtbf16_f_f_w_bf16mf2_m (vbool32_t mask, vfloat32m1_t src, size_t vl){  return vfncvtbf16_f_f_w_bf16mf2_m (mask, src, vl);}
vbfloat16m1_t test_vfncvtbf16_f_f_w_bf16m1_m (vbool16_t mask, vfloat32m2_t src, size_t vl){  return vfncvtbf16_f_f_w_bf16m1_m (mask, src, vl);}
vbfloat16m2_t test_vfncvtbf16_f_f_w_bf16m2_m (vbool8_t mask, vfloat32m4_t src, size_t vl){  return vfncvtbf16_f_f_w_bf16m2_m (mask, src, vl);}
vbfloat16m4_t test_vfncvtbf16_f_f_w_bf16m4_m (vbool4_t mask, vfloat32m8_t src, size_t vl){  return vfncvtbf16_f_f_w_bf16m4_m (mask, src, vl);}

/* { dg-final { scan-assembler-times "vfncvtbf16.f.f.w\tv\[0-9]+,v\[0-9]+\n" 5 } } */
/* { dg-final { scan-assembler-times "vfwcvtbf16.f.f.v\tv\[0-9]+,v\[0-9]+\n" 5 } } */
/* { dg-final { scan-assembler-times "vfncvtbf16.f.f.w\tv\[0-9]+,v\[0-9]+,v0.t\n" 5 } } */
/* { dg-final { scan-assembler-times "vfwcvtbf16.f.f.v\tv\[0-9]+,v\[0-9]+,v0.t\n" 5 } } */
