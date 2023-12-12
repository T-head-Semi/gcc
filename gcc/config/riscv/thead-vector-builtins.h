/* function_base declaration for RISC-V 'V' Extension for GNU compiler.
   Copyright (C) 2022-2023 Free Software Foundation, Inc.
   Contributed by Ju-Zhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_THEAD_VECTOR_BUILTINS_H
#define GCC_THEAD_VECTOR_BUILTINS_H

namespace riscv_vector {

namespace bases {
extern const function_base *const th_vsetvl;
extern const function_base *const th_vsetvlmax;
extern const function_base *const th_vle;
extern const function_base *const th_vse;
extern const function_base *const th_vlm;
extern const function_base *const th_vsm;
extern const function_base *const th_vlse;
extern const function_base *const th_vsse;
extern const function_base *const th_vluxei8;
extern const function_base *const th_vluxei16;
extern const function_base *const th_vluxei32;
extern const function_base *const th_vluxei64;
extern const function_base *const th_vloxei8;
extern const function_base *const th_vloxei16;
extern const function_base *const th_vloxei32;
extern const function_base *const th_vloxei64;
extern const function_base *const th_vsuxei8;
extern const function_base *const th_vsuxei16;
extern const function_base *const th_vsuxei32;
extern const function_base *const th_vsuxei64;
extern const function_base *const th_vsoxei8;
extern const function_base *const th_vsoxei16;
extern const function_base *const th_vsoxei32;
extern const function_base *const th_vsoxei64;
extern const function_base *const th_vneg;
extern const function_base *const th_vnot;
extern const function_base *const th_vnsrl;
extern const function_base *const th_vnsra;
extern const function_base *const th_vncvt_x;
extern const function_base *const th_vnclip;
extern const function_base *const th_vnclipu;
extern const function_base *const th_vcpop;
extern const function_base *const th_vfirst;
extern const function_base *const th_vmadc;
extern const function_base *const th_vmsbc;
extern const function_base *const th_vfncvt_x;
extern const function_base *const th_vfncvt_x_frm;
extern const function_base *const th_vfncvt_xu;
extern const function_base *const th_vfncvt_xu_frm;
extern const function_base *const th_vfncvt_f;
extern const function_base *const th_vfncvt_f_frm;
extern const function_base *const th_vfredusum;
extern const function_base *const th_vfredusum_frm;
extern const function_base *const th_vfredosum;
extern const function_base *const th_vfredosum_frm;
extern const function_base *const th_vfwredusum;
extern const function_base *const th_vfwredusum_frm;
extern const function_base *const th_vfwredosum;
extern const function_base *const th_vfwredosum_frm;
extern const function_base *const th_vleff;
extern const function_base *const th_vlseg;
extern const function_base *const th_vsseg;
extern const function_base *const th_vlsseg;
extern const function_base *const th_vssseg;
extern const function_base *const th_vluxseg;
extern const function_base *const th_vloxseg;
extern const function_base *const th_vsuxseg;
extern const function_base *const th_vsoxseg;
extern const function_base *const th_vlsegff;
extern const function_base *const th_vlb;
extern const function_base *const th_vlh;
extern const function_base *const th_vlw;
extern const function_base *const th_vlbu;
extern const function_base *const th_vlhu;
extern const function_base *const th_vlwu;
extern const function_base *const th_vsb;
extern const function_base *const th_vsh;
extern const function_base *const th_vsw;
extern const function_base *const th_vlsb;
extern const function_base *const th_vlsh;
extern const function_base *const th_vlsw;
extern const function_base *const th_vlsbu;
extern const function_base *const th_vlshu;
extern const function_base *const th_vlswu;
extern const function_base *const th_vssb;
extern const function_base *const th_vssh;
extern const function_base *const th_vssw;
extern const function_base *const th_vlxb;
extern const function_base *const th_vlxh;
extern const function_base *const th_vlxw;
extern const function_base *const th_vlxbu;
extern const function_base *const th_vlxhu;
extern const function_base *const th_vlxwu;
extern const function_base *const th_vsxb;
extern const function_base *const th_vsxh;
extern const function_base *const th_vsxw;
extern const function_base *const th_vsuxb;
extern const function_base *const th_vsuxh;
extern const function_base *const th_vsuxw;
extern const function_base *const th_vext_x_v;
}

} // end namespace riscv_vector

#endif
