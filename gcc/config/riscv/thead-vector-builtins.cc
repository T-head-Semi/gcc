/* function_base implementation for RISC-V 'V' Extension for GNU compiler.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "memmodel.h"
#include "insn-codes.h"
#include "optabs.h"
#include "recog.h"
#include "expr.h"
#include "basic-block.h"
#include "function.h"
#include "fold-const.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "gimplify.h"
#include "explow.h"
#include "emit-rtl.h"
#include "tree-vector-builder.h"
#include "rtx-vector-builder.h"
#include "riscv-vector-builtins.h"
#include "riscv-vector-builtins-shapes.h"
#include "riscv-vector-builtins-bases.h"
#include "thead-vector-builtins.h"

using namespace riscv_vector;

namespace riscv_vector {

/* Implements vsetvl<mode> && vsetvlmax<mode>.  */
template<bool VLMAX_P>
class th_vsetvl : public function_base
{
public:
  bool apply_vl_p () const override
  {
    return false;
  }

  rtx expand (function_expander &e) const override
  {
    if (VLMAX_P)
      e.add_input_operand (Pmode, gen_rtx_REG (Pmode, 0));
    else
      e.add_input_operand (0);

    tree type = builtin_types[e.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    machine_mode inner_mode = GET_MODE_INNER (mode);
    /* SEW.  */
    e.add_input_operand (Pmode,
      gen_int_mode (GET_MODE_BITSIZE (inner_mode), Pmode));

    /* LMUL.  */
    e.add_input_operand (Pmode,
      gen_int_mode (get_vlmul (mode), Pmode));

    /* TAIL_ANY.  */
    e.add_input_operand (Pmode,
			 gen_int_mode (get_prefer_tail_policy (), Pmode));

    /* MASK_ANY.  */
    e.add_input_operand (Pmode,
			 gen_int_mode (get_prefer_mask_policy (), Pmode));
    return e.generate_insn (code_for_th_vsetvl_no_side_effects (Pmode));
  }
};


/* Implements
 * th.vl(b/h/w)[u].v/th.vs(b/h/w)[u].v/th.vls(b/h/w)[u].v/th.vss(b/h/w)[u].v/
 * th.vlx(b/h/w)[u].v/th.vs[u]x(b/h/w).v
 * codegen.  */
template<bool STORE_P, lst_type LST_TYPE, int UNSPEC>
class th_loadstore_width : public function_base
{
public:
  bool apply_tail_policy_p () const override { return !STORE_P; }
  bool apply_mask_policy_p () const override { return !STORE_P; }

  unsigned int call_properties (const function_instance &) const override
  {
    if (STORE_P)
      return CP_WRITE_MEMORY;
    else
      return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    if (STORE_P || LST_TYPE == LST_INDEXED)
      return true;
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    if (LST_TYPE == LST_INDEXED)
      {
	if (STORE_P)
	  return e.use_exact_insn (
	    code_for_pred_indexed_store_width (UNSPEC, UNSPEC,
					       e.vector_mode ()));
	else
	  return e.use_exact_insn (
	    code_for_pred_indexed_load_width (UNSPEC, e.vector_mode ()));
      }
    else if (LST_TYPE == LST_STRIDED)
      {
	if (STORE_P)
	  return e.use_contiguous_store_insn (
	    code_for_pred_strided_store_width (UNSPEC, e.vector_mode ()));
	else
	  return e.use_contiguous_load_insn (
	    code_for_pred_strided_load_width (UNSPEC, e.vector_mode ()));
      }
    else
      {
	if (STORE_P)
	  return e.use_contiguous_store_insn (
	    code_for_pred_store_width (UNSPEC, e.vector_mode ()));
	else
	  return e.use_contiguous_load_insn (
	    code_for_pred_mov_width (UNSPEC, e.vector_mode ()));
      }
  }
};


/* Implements
 * vle.v/vse.v/vlm.v/vsm.v/vlse.v/vsse.v/vluxei.v/vloxei.v/vsuxei.v/vsoxei.v
 * codegen.  */
template<bool STORE_P, lst_type LST_TYPE, bool ORDERED_P>
class th_loadstore : public function_base
{
public:
  bool apply_tail_policy_p () const override { return !STORE_P; }
  bool apply_mask_policy_p () const override { return !STORE_P; }

  unsigned int call_properties (const function_instance &) const override
  {
    if (STORE_P)
      return CP_WRITE_MEMORY;
    else
      return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    if (STORE_P || LST_TYPE == LST_INDEXED)
      return true;
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    if (LST_TYPE == LST_INDEXED)
      {
	int unspec = ORDERED_P ? UNSPEC_ORDERED : UNSPEC_UNORDERED;
	if (STORE_P)
	    return e.use_exact_insn (
	      code_for_pred_th_indexed_store (unspec, e.vector_mode (),
					      e.index_mode ()));
	else
	  {
	    unsigned src_eew_bitsize
	      = GET_MODE_BITSIZE (GET_MODE_INNER (e.index_mode ()));
	    unsigned dst_eew_bitsize
	      = GET_MODE_BITSIZE (GET_MODE_INNER (e.vector_mode ()));
	    if (dst_eew_bitsize == src_eew_bitsize)
	      {
		return e.use_exact_insn (
		  code_for_pred_th_indexed_load_same_eew (
		    unspec, e.vector_mode ()));
	      }
	    else if (dst_eew_bitsize > src_eew_bitsize)
	      {
		unsigned factor = dst_eew_bitsize / src_eew_bitsize;
		switch (factor)
		  {
		  case 2:
		    return e.use_exact_insn (
		      code_for_pred_th_indexed_load_x2_greater_eew (
			unspec, e.vector_mode ()));
		  case 4:
		    return e.use_exact_insn (
		      code_for_pred_th_indexed_load_x4_greater_eew (
			unspec, e.vector_mode ()));
		  case 8:
		    return e.use_exact_insn (
		      code_for_pred_th_indexed_load_x8_greater_eew (
			unspec, e.vector_mode ()));
		  default:
		    gcc_unreachable ();
		  }
	      }
	    else
	      {
		unsigned factor = src_eew_bitsize / dst_eew_bitsize;
		switch (factor)
		  {
		  case 2:
		    return e.use_exact_insn (
		      code_for_pred_th_indexed_load_x2_smaller_eew (
			unspec, e.vector_mode ()));
		  case 4:
		    return e.use_exact_insn (
		      code_for_pred_th_indexed_load_x4_smaller_eew (
			unspec, e.vector_mode ()));
		  case 8:
		    return e.use_exact_insn (
		      code_for_pred_th_indexed_load_x8_smaller_eew (
			unspec, e.vector_mode ()));
		  default:
		    gcc_unreachable ();
		  }
	      }
	  }
      }
    else if (LST_TYPE == LST_STRIDED)
      {
	if (STORE_P)
	  return e.use_contiguous_store_insn (
	    code_for_pred_th_strided_store (e.vector_mode ()));
	else
	  return e.use_contiguous_load_insn (
	    code_for_pred_th_strided_load (e.vector_mode ()));
      }
    else
      {
	if (STORE_P)
	  return e.use_contiguous_store_insn (
	    code_for_pred_th_store (e.vector_mode ()));
	else
	  return e.use_contiguous_load_insn (
	    code_for_pred_mov (e.vector_mode ()));
      }
  }
};

/* Implements vneg/vnot.  */
template<rtx_code CODE, enum frm_op_type FRM_OP = NO_FRM>
class th_unop : public function_base
{
public:
  bool has_rounding_mode_operand_p () const override
  {
    return FRM_OP == HAS_FRM;
  }

  bool may_require_frm_p () const override { return true; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_th (CODE, e.vector_mode ()));
  }
};

/* Implements vnsrl/vnsra.  */
template<rtx_code CODE>
class th_vnshift : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_wx:
	return e.use_exact_insn (
	  code_for_pred_th_narrow_scalar (CODE, e.vector_mode ()));
      case OP_TYPE_wv:
	return e.use_exact_insn (
	  code_for_pred_th_narrow (CODE, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vncvt.  */
class th_vncvt_x : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_trunc (e.vector_mode ()));
  }
};

/* Implements vnclip/vnclipu.  */
template<int UNSPEC>
class th_vnclip : public function_base
{
public:
  bool has_rounding_mode_operand_p () const override { return true; }

  bool may_require_vxrm_p () const override { return true; }

  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_wx:
	return e.use_exact_insn (
	  code_for_pred_th_narrow_clip_scalar (UNSPEC, e.vector_mode ()));
      case OP_TYPE_wv:
	return e.use_exact_insn (
	  code_for_pred_th_narrow_clip (UNSPEC, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vcpop.  */
class th_vcpop : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_popcount (e.vector_mode (), Pmode));
  }
};

/* Implements vfirst.  */
class th_vfirst : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_ffs (e.vector_mode (), Pmode));
  }
};

/* Implements vmadc.  */
class th_vmadc : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vvm:
	return e.use_exact_insn (code_for_pred_th_madc (e.vector_mode ()));
      case OP_TYPE_vxm:
	return e.use_exact_insn (code_for_pred_th_madc_scalar (e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_th_madc_overflow (e.vector_mode ()));
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_th_madc_overflow_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vmsbc.  */
class th_vmsbc : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vvm:
	return e.use_exact_insn (code_for_pred_th_msbc (e.vector_mode ()));
      case OP_TYPE_vxm:
	return e.use_exact_insn (code_for_pred_th_msbc_scalar (e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_th_msbc_overflow (e.vector_mode ()));
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_th_msbc_overflow_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vfncvt.x.  */
template<int UNSPEC, enum frm_op_type FRM_OP = NO_FRM>
class th_vfncvt_x : public function_base
{
public:
  bool has_rounding_mode_operand_p () const override
  {
    return FRM_OP == HAS_FRM;
  }

  bool may_require_frm_p () const override { return true; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_narrow_fcvt_x_f (UNSPEC, e.arg_mode (0)));
  }
};

template<enum frm_op_type FRM_OP = NO_FRM>
class th_vfncvt_f : public function_base
{
public:
  bool has_rounding_mode_operand_p () const override
  {
    return FRM_OP == HAS_FRM;
  }

  bool may_require_frm_p () const override { return true; }

  rtx expand (function_expander &e) const override
  {
    if (e.op_info->op == OP_TYPE_f_w)
      return e.use_exact_insn (
	code_for_pred_th_trunc (e.vector_mode ()));
    if (e.op_info->op == OP_TYPE_x_w)
      return e.use_exact_insn (
	code_for_pred_th_narrow (FLOAT, e.arg_mode (0)));
    if (e.op_info->op == OP_TYPE_xu_w)
      return e.use_exact_insn (
	code_for_pred_th_narrow (UNSIGNED_FLOAT, e.arg_mode (0)));
    gcc_unreachable ();
  }
};

/* Implements floating-point reduction instructions.  */
template<unsigned UNSPEC, enum frm_op_type FRM_OP = NO_FRM>
class th_freducop : public function_base
{
public:
  bool has_rounding_mode_operand_p () const override
  {
    return FRM_OP == HAS_FRM;
  }

  bool may_require_frm_p () const override { return true; }

  bool apply_mask_policy_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_th (UNSPEC, e.vector_mode ()));
  }
};

class th_vleff : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY | CP_WRITE_CSR;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  gimple *fold (gimple_folder &f) const override
  {
    return fold_fault_load (f);
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_contiguous_load_insn (
      code_for_pred_th_fault_load (e.vector_mode ()));
  }
};

/* Implements vlseg.v.  */
class th_vlseg : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_unit_strided_load (e.vector_mode ()));
  }
};

/* Implements vsseg.v.  */
class th_vsseg : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_unit_strided_store (e.vector_mode ()));
  }
};

/* Implements vlsseg.v.  */
class th_vlsseg : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_strided_load (e.vector_mode ()));
  }
};

/* Implements vssseg.v.  */
class th_vssseg : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_strided_store (e.vector_mode ()));
  }
};

template<int UNSPEC>
class th_seg_indexed_load : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_indexed_load (
	UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

template<int UNSPEC>
class th_seg_indexed_store : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_indexed_store (
	UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

/* Implements vlsegff.v.  */
class th_vlsegff : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY | CP_WRITE_CSR;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  gimple *fold (gimple_folder &f) const override
  {
    return fold_fault_load (f);
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_th_fault_load (e.vector_mode ()));
  }
};

/* Implements vext.x.v.  */
class th_extract : public function_base
{
public:
  bool apply_vl_p () const override { return false; }
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (code_for_pred_th_extract (e.vector_mode ()));
  }
};

static CONSTEXPR const th_vsetvl<false> th_vsetvl_obj;
static CONSTEXPR const th_vsetvl<true> th_vsetvlmax_obj;
static CONSTEXPR const th_loadstore<false, LST_UNIT_STRIDE, false> th_vle_obj;
static CONSTEXPR const th_loadstore<true, LST_UNIT_STRIDE, false> th_vse_obj;
static CONSTEXPR const th_loadstore<false, LST_UNIT_STRIDE, false> th_vlm_obj;
static CONSTEXPR const th_loadstore<true, LST_UNIT_STRIDE, false> th_vsm_obj;
static CONSTEXPR const th_loadstore<false, LST_STRIDED, false> th_vlse_obj;
static CONSTEXPR const th_loadstore<true, LST_STRIDED, false> th_vsse_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, false> th_vluxei8_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, false> th_vluxei16_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, false> th_vluxei32_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, false> th_vluxei64_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, true> th_vloxei8_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, true> th_vloxei16_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, true> th_vloxei32_obj;
static CONSTEXPR const th_loadstore<false, LST_INDEXED, true> th_vloxei64_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, false> th_vsuxei8_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, false> th_vsuxei16_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, false> th_vsuxei32_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, false> th_vsuxei64_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, true> th_vsoxei8_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, true> th_vsoxei16_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, true> th_vsoxei32_obj;
static CONSTEXPR const th_loadstore<true, LST_INDEXED, true> th_vsoxei64_obj;
static CONSTEXPR const th_unop<NEG> th_vneg_obj;
static CONSTEXPR const th_unop<NOT> th_vnot_obj;
static CONSTEXPR const th_vnshift<LSHIFTRT> th_vnsrl_obj;
static CONSTEXPR const th_vnshift<ASHIFTRT> th_vnsra_obj;
static CONSTEXPR const th_vncvt_x th_vncvt_x_obj;
static CONSTEXPR const th_vnclip<UNSPEC_VNCLIP> th_vnclip_obj;
static CONSTEXPR const th_vnclip<UNSPEC_VNCLIPU> th_vnclipu_obj;
static CONSTEXPR const th_vcpop th_vcpop_obj;
static CONSTEXPR const th_vfirst th_vfirst_obj;
static CONSTEXPR const th_vmadc th_vmadc_obj;
static CONSTEXPR const th_vmsbc th_vmsbc_obj;
static CONSTEXPR const th_vfncvt_x<UNSPEC_VFCVT> th_vfncvt_x_obj;
static CONSTEXPR const th_vfncvt_x<UNSPEC_VFCVT, HAS_FRM> th_vfncvt_x_frm_obj;
static CONSTEXPR const th_vfncvt_x<UNSPEC_UNSIGNED_VFCVT> th_vfncvt_xu_obj;
static CONSTEXPR const th_vfncvt_x<UNSPEC_UNSIGNED_VFCVT, HAS_FRM> th_vfncvt_xu_frm_obj;
static CONSTEXPR const th_vfncvt_f<NO_FRM> th_vfncvt_f_obj;
static CONSTEXPR const th_vfncvt_f<HAS_FRM> th_vfncvt_f_frm_obj;
static CONSTEXPR const th_freducop<UNSPEC_REDUC_SUM_UNORDERED> th_vfredusum_obj;
static CONSTEXPR const th_freducop<UNSPEC_REDUC_SUM_UNORDERED, HAS_FRM> th_vfredusum_frm_obj;
static CONSTEXPR const th_freducop<UNSPEC_REDUC_SUM_ORDERED> th_vfredosum_obj;
static CONSTEXPR const th_freducop<UNSPEC_REDUC_SUM_ORDERED, HAS_FRM> th_vfredosum_frm_obj;
static CONSTEXPR const th_freducop<UNSPEC_WREDUC_SUM_UNORDERED> th_vfwredusum_obj;
static CONSTEXPR const th_freducop<UNSPEC_WREDUC_SUM_UNORDERED, HAS_FRM> th_vfwredusum_frm_obj;
static CONSTEXPR const th_freducop<UNSPEC_WREDUC_SUM_ORDERED> th_vfwredosum_obj;
static CONSTEXPR const th_freducop<UNSPEC_WREDUC_SUM_ORDERED, HAS_FRM> th_vfwredosum_frm_obj;
static CONSTEXPR const th_vleff th_vleff_obj;
static CONSTEXPR const th_vlseg th_vlseg_obj;
static CONSTEXPR const th_vsseg th_vsseg_obj;
static CONSTEXPR const th_vlsseg th_vlsseg_obj;
static CONSTEXPR const th_vssseg th_vssseg_obj;
static CONSTEXPR const th_seg_indexed_load<UNSPEC_UNORDERED> th_vluxseg_obj;
static CONSTEXPR const th_seg_indexed_load<UNSPEC_ORDERED> th_vloxseg_obj;
static CONSTEXPR const th_seg_indexed_store<UNSPEC_UNORDERED> th_vsuxseg_obj;
static CONSTEXPR const th_seg_indexed_store<UNSPEC_ORDERED> th_vsoxseg_obj;
static CONSTEXPR const th_vlsegff th_vlsegff_obj;
static CONSTEXPR const th_loadstore_width<false, LST_UNIT_STRIDE, UNSPEC_TH_VLB> th_vlb_obj;
static CONSTEXPR const th_loadstore_width<false, LST_UNIT_STRIDE, UNSPEC_TH_VLBU> th_vlbu_obj;
static CONSTEXPR const th_loadstore_width<false, LST_UNIT_STRIDE, UNSPEC_TH_VLH> th_vlh_obj;
static CONSTEXPR const th_loadstore_width<false, LST_UNIT_STRIDE, UNSPEC_TH_VLHU> th_vlhu_obj;
static CONSTEXPR const th_loadstore_width<false, LST_UNIT_STRIDE, UNSPEC_TH_VLW> th_vlw_obj;
static CONSTEXPR const th_loadstore_width<false, LST_UNIT_STRIDE, UNSPEC_TH_VLWU> th_vlwu_obj;
static CONSTEXPR const th_loadstore_width<true, LST_UNIT_STRIDE, UNSPEC_TH_VLB> th_vsb_obj;
static CONSTEXPR const th_loadstore_width<true, LST_UNIT_STRIDE, UNSPEC_TH_VLH> th_vsh_obj;
static CONSTEXPR const th_loadstore_width<true, LST_UNIT_STRIDE, UNSPEC_TH_VLW> th_vsw_obj;
static CONSTEXPR const th_loadstore_width<false, LST_STRIDED, UNSPEC_TH_VLSB> th_vlsb_obj;
static CONSTEXPR const th_loadstore_width<false, LST_STRIDED, UNSPEC_TH_VLSBU> th_vlsbu_obj;
static CONSTEXPR const th_loadstore_width<false, LST_STRIDED, UNSPEC_TH_VLSH> th_vlsh_obj;
static CONSTEXPR const th_loadstore_width<false, LST_STRIDED, UNSPEC_TH_VLSHU> th_vlshu_obj;
static CONSTEXPR const th_loadstore_width<false, LST_STRIDED, UNSPEC_TH_VLSW> th_vlsw_obj;
static CONSTEXPR const th_loadstore_width<false, LST_STRIDED, UNSPEC_TH_VLSWU> th_vlswu_obj;
static CONSTEXPR const th_loadstore_width<true, LST_STRIDED, UNSPEC_TH_VLSB> th_vssb_obj;
static CONSTEXPR const th_loadstore_width<true, LST_STRIDED, UNSPEC_TH_VLSH> th_vssh_obj;
static CONSTEXPR const th_loadstore_width<true, LST_STRIDED, UNSPEC_TH_VLSW> th_vssw_obj;
static CONSTEXPR const th_loadstore_width<false, LST_INDEXED, UNSPEC_TH_VLXB> th_vlxb_obj;
static CONSTEXPR const th_loadstore_width<false, LST_INDEXED, UNSPEC_TH_VLXBU> th_vlxbu_obj;
static CONSTEXPR const th_loadstore_width<false, LST_INDEXED, UNSPEC_TH_VLXH> th_vlxh_obj;
static CONSTEXPR const th_loadstore_width<false, LST_INDEXED, UNSPEC_TH_VLXHU> th_vlxhu_obj;
static CONSTEXPR const th_loadstore_width<false, LST_INDEXED, UNSPEC_TH_VLXW> th_vlxw_obj;
static CONSTEXPR const th_loadstore_width<false, LST_INDEXED, UNSPEC_TH_VLXWU> th_vlxwu_obj;
static CONSTEXPR const th_loadstore_width<true, LST_INDEXED, UNSPEC_TH_VLXB> th_vsxb_obj;
static CONSTEXPR const th_loadstore_width<true, LST_INDEXED, UNSPEC_TH_VLXH> th_vsxh_obj;
static CONSTEXPR const th_loadstore_width<true, LST_INDEXED, UNSPEC_TH_VLXW> th_vsxw_obj;
static CONSTEXPR const th_loadstore_width<true, LST_INDEXED, UNSPEC_TH_VSUXB> th_vsuxb_obj;
static CONSTEXPR const th_loadstore_width<true, LST_INDEXED, UNSPEC_TH_VSUXH> th_vsuxh_obj;
static CONSTEXPR const th_loadstore_width<true, LST_INDEXED, UNSPEC_TH_VSUXW> th_vsuxw_obj;
static CONSTEXPR const th_extract th_vext_x_v_obj;

/* Declare the function base NAME, pointing it to an instance
   of class <NAME>_obj.  */
#define BASE(NAME) \
  namespace bases { const function_base *const NAME = &NAME##_obj; }

BASE (th_vsetvl)
BASE (th_vsetvlmax)
BASE (th_vle)
BASE (th_vse)
BASE (th_vlm)
BASE (th_vsm)
BASE (th_vlse)
BASE (th_vsse)
BASE (th_vluxei8)
BASE (th_vluxei16)
BASE (th_vluxei32)
BASE (th_vluxei64)
BASE (th_vloxei8)
BASE (th_vloxei16)
BASE (th_vloxei32)
BASE (th_vloxei64)
BASE (th_vsuxei8)
BASE (th_vsuxei16)
BASE (th_vsuxei32)
BASE (th_vsuxei64)
BASE (th_vsoxei8)
BASE (th_vsoxei16)
BASE (th_vsoxei32)
BASE (th_vsoxei64)
BASE (th_vneg)
BASE (th_vnot)
BASE (th_vnsrl)
BASE (th_vnsra)
BASE (th_vncvt_x)
BASE (th_vnclip)
BASE (th_vnclipu)
BASE (th_vcpop)
BASE (th_vfirst)
BASE (th_vmadc)
BASE (th_vmsbc)
BASE (th_vfncvt_x)
BASE (th_vfncvt_x_frm)
BASE (th_vfncvt_xu)
BASE (th_vfncvt_xu_frm)
BASE (th_vfncvt_f)
BASE (th_vfncvt_f_frm)
BASE (th_vfredusum)
BASE (th_vfredusum_frm)
BASE (th_vfredosum)
BASE (th_vfredosum_frm)
BASE (th_vfwredusum)
BASE (th_vfwredusum_frm)
BASE (th_vfwredosum)
BASE (th_vfwredosum_frm)
BASE (th_vleff)
BASE (th_vlseg)
BASE (th_vsseg)
BASE (th_vlsseg)
BASE (th_vssseg)
BASE (th_vluxseg)
BASE (th_vloxseg)
BASE (th_vsuxseg)
BASE (th_vsoxseg)
BASE (th_vlsegff)
BASE (th_vlb)
BASE (th_vlh)
BASE (th_vlw)
BASE (th_vlbu)
BASE (th_vlhu)
BASE (th_vlwu)
BASE (th_vsb)
BASE (th_vsh)
BASE (th_vsw)
BASE (th_vlsb)
BASE (th_vlsh)
BASE (th_vlsw)
BASE (th_vlsbu)
BASE (th_vlshu)
BASE (th_vlswu)
BASE (th_vssb)
BASE (th_vssh)
BASE (th_vssw)
BASE (th_vlxb)
BASE (th_vlxh)
BASE (th_vlxw)
BASE (th_vlxbu)
BASE (th_vlxhu)
BASE (th_vlxwu)
BASE (th_vsxb)
BASE (th_vsxh)
BASE (th_vsxw)
BASE (th_vsuxb)
BASE (th_vsuxh)
BASE (th_vsuxw)
BASE (th_vext_x_v)

} // end namespace riscv_vector
