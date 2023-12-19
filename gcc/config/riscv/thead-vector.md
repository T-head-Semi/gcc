(define_int_attr th_order [
  (UNSPEC_ORDERED "") (UNSPEC_UNORDERED "u")
])

(define_int_attr th_reduc_op [
  (UNSPEC_REDUC_SUM "redsum")
  (UNSPEC_REDUC_SUM_ORDERED "redosum") (UNSPEC_REDUC_SUM_UNORDERED "redsum")
  (UNSPEC_REDUC_MAXU "redmaxu") (UNSPEC_REDUC_MAX "redmax") (UNSPEC_REDUC_MINU "redminu") (UNSPEC_REDUC_MIN "redmin")
  (UNSPEC_REDUC_AND "redand") (UNSPEC_REDUC_OR "redor") (UNSPEC_REDUC_XOR "redxor")
  (UNSPEC_WREDUC_SUM "wredsum") (UNSPEC_WREDUC_SUMU "wredsumu")
  (UNSPEC_WREDUC_SUM_ORDERED "wredosum") (UNSPEC_WREDUC_SUM_UNORDERED "wredsum")
])

(define_code_iterator neg_unop [neg])
(define_code_iterator not_unop [not])

(define_code_iterator any_float_unop_neg [neg])
(define_code_iterator any_float_unop_abs [abs])

(define_int_iterator UNSPEC_TH_VSMEM_OP [
  UNSPEC_TH_VLB
  UNSPEC_TH_VLH
  UNSPEC_TH_VLW
])

(define_int_iterator UNSPEC_TH_VSSMEM_OP [
  UNSPEC_TH_VLSB
  UNSPEC_TH_VLSH
  UNSPEC_TH_VLSW
])

(define_int_iterator UNSPEC_TH_VSXMEM_OP [
  UNSPEC_TH_VLXB
  UNSPEC_TH_VLXH
  UNSPEC_TH_VLXW
  UNSPEC_TH_VSUXB
  UNSPEC_TH_VSUXH
  UNSPEC_TH_VSUXW
])

(define_mode_iterator V_VLS_VT [V VLS VT])
(define_mode_iterator V_VB_VLS_VT [V VB VLS VT])

(define_split
  [(set (match_operand:V_VB_VLS_VT 0 "reg_or_mem_operand")
	(match_operand:V_VB_VLS_VT 1 "reg_or_mem_operand"))]
  "TARGET_XTHEADVECTOR"
  [(const_int 0)]
  {
    emit_insn (gen_pred_th_whole_mov (<MODE>mode, operands[0], operands[1],
				      RVV_VLMAX, GEN_INT(riscv_vector::VLMAX)));
    DONE;
  })

(define_insn_and_split "@pred_th_whole_mov<mode>"
  [(set (match_operand:V_VLS_VT 0 "reg_or_mem_operand"  "=vr,vr, m")
	(unspec:V_VLS_VT
	  [(match_operand:V_VLS_VT 1 "reg_or_mem_operand" " vr, m,vr")
	   (match_operand 2 "vector_length_operand"   " rK, rK, rK")
	   (match_operand 3 "const_1_operand"         "  i, i, i")
	   (reg:SI VL_REGNUM)
	   (reg:SI VTYPE_REGNUM)]
	UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
  "@
   vmv.v.v\t%0,%1
   vle.v\t%0,%1
   vse.v\t%1,%0"
  "&& REG_P (operands[0]) && REG_P (operands[1])
   && REGNO (operands[0]) == REGNO (operands[1])"
  [(const_int 0)]
  ""
  [(set_attr "type" "vimov,vlds,vlds")
   (set_attr "mode" "<MODE>")
   (set (attr "ta") (symbol_ref "riscv_vector::TAIL_UNDISTURBED"))
   (set (attr "ma") (symbol_ref "riscv_vector::MASK_UNDISTURBED"))
   (set (attr "avl_type_idx") (const_int 3))
   (set_attr "vl_op_idx" "2")])

(define_insn_and_split "@pred_th_whole_mov<mode>"
  [(set (match_operand:VB 0 "reg_or_mem_operand"  "=vr,vr, m")
	(unspec:VB
	  [(match_operand:VB 1 "reg_or_mem_operand" " vr, m,vr")
	   (match_operand 2 "vector_length_operand"   " rK, rK, rK")
	   (match_operand 3 "const_1_operand"         "  i, i, i")
	   (reg:SI VL_REGNUM)
	   (reg:SI VTYPE_REGNUM)]
	UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
  "@
   vmv.v.v\t%0,%1
   vle.v\t%0,%1
   vse.v\t%1,%0"
  "&& REG_P (operands[0]) && REG_P (operands[1])
   && REGNO (operands[0]) == REGNO (operands[1])"
  [(const_int 0)]
  ""
  [(set_attr "type" "vimov,vlds,vlds")
   (set_attr "mode" "<MODE>")
   (set (attr "ta") (symbol_ref "riscv_vector::TAIL_UNDISTURBED"))
   (set (attr "ma") (symbol_ref "riscv_vector::MASK_UNDISTURBED"))
   (set (attr "avl_type_idx") (const_int 3))
   (set_attr "vl_op_idx" "2")
   (set (attr "sew") (const_int 8))
   (set (attr "vlmul") (symbol_ref "riscv_vector::LMUL_1"))])

(define_expand "@pred_th_mov<mode>"
  [(set (match_operand:V_VLS 0 "nonimmediate_operand")
    (if_then_else:V_VLS
      (unspec:<VM>
        [(match_operand:<VM> 1 "vector_mask_operand")
         (match_operand 4 "vector_length_operand")
         (match_operand 5 "const_int_operand")
         (match_operand 6 "const_int_operand")
         (match_operand 7 "const_int_operand")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
      (match_operand:V_VLS 3 "vector_move_operand")
      (match_operand:V_VLS 2 "vector_merge_operand")))]
  "TARGET_XTHEADVECTOR"
  {})

(define_insn_and_split "*pred_broadcast<mode>"
  [(set (match_operand:V_VLSI 0 "register_operand"                 "=vr, vr, vd, vd, vr, vr, vr, vr")
	(if_then_else:V_VLSI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_broadcast_mask_operand" "Wc1,Wc1, vm, vm,Wc1,Wc1,Wb1,Wb1")
	     (match_operand 4 "vector_length_operand"              " rK, rK, rK, rK, rK, rK, rK, rK")
	     (match_operand 5 "const_int_operand"                  "  i,  i,  i,  i,  i,  i,  i,  i")
	     (match_operand 6 "const_int_operand"                  "  i,  i,  i,  i,  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"                  "  i,  i,  i,  i,  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (vec_duplicate:V_VLSI
	    (match_operand:<VEL> 3 "direct_broadcast_operand"       " r,  r,Wdm,Wdm,Wdm,Wdm,  r,  r"))
	  (match_operand:V_VLSI 2 "vector_merge_operand"            "vu,  0, vu,  0, vu,  0, vu,  0")))]
  "TARGET_XTHEADVECTOR"
  "@
   vmv.v.x\t%0,%3
   vmv.v.x\t%0,%3
   vlse.v\t%0,%3,zero,%1.t
   vlse.v\t%0,%3,zero,%1.t
   vlse.v\t%0,%3,zero
   vlse.v\t%0,%3,zero
   vmv.s.x\t%0,%3
   vmv.s.x\t%0,%3"
  "(register_operand (operands[3], <VEL>mode)
  || CONST_POLY_INT_P (operands[3]))
  && GET_MODE_BITSIZE (<VEL>mode) > GET_MODE_BITSIZE (Pmode)"
  [(set (match_dup 0)
	(if_then_else:V_VLSI (unspec:<VM> [(match_dup 1) (match_dup 4)
	     (match_dup 5) (match_dup 6) (match_dup 7)
	     (reg:SI VL_REGNUM) (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (vec_duplicate:V_VLSI (match_dup 3))
	  (match_dup 2)))]
  {
    gcc_assert (can_create_pseudo_p ());
    if (CONST_POLY_INT_P (operands[3]))
      {
	rtx tmp = gen_reg_rtx (<VEL>mode);
	emit_move_insn (tmp, operands[3]);
	operands[3] = tmp;
      }
    rtx m = assign_stack_local (<VEL>mode, GET_MODE_SIZE (<VEL>mode),
				GET_MODE_ALIGNMENT (<VEL>mode));
    m = validize_mem (m);
    emit_move_insn (m, operands[3]);
    m = gen_rtx_MEM (<VEL>mode, force_reg (Pmode, XEXP (m, 0)));
    operands[3] = m;

    /* For SEW = 64 in RV32 system, we expand vmv.s.x:
       andi a2,a2,1
       vsetvl zero,a2,e64
       vlse64.v  */
    if (satisfies_constraint_Wb1 (operands[1]))
      {
	operands[4] = riscv_vector::gen_avl_for_scalar_move (operands[4]);
	operands[1] = CONSTM1_RTX (<VM>mode);
      }
  }
  [(set_attr "type" "vimov,vimov,vlds,vlds,vlds,vlds,vimovxv,vimovxv")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_broadcast<mode>"
  [(set (match_operand:V_VLSF_ZVFHMIN 0 "register_operand"         "=vr, vr, vr, vr, vr, vr, vr, vr")
	(if_then_else:V_VLSF_ZVFHMIN
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_broadcast_mask_operand" "Wc1,Wc1, vm, vm,Wc1,Wc1,Wb1,Wb1")
	     (match_operand 4 "vector_length_operand"              " rK, rK, rK, rK, rK, rK, rK, rK")
	     (match_operand 5 "const_int_operand"                  "  i,  i,  i,  i,  i,  i,  i,  i")
	     (match_operand 6 "const_int_operand"                  "  i,  i,  i,  i,  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"                  "  i,  i,  i,  i,  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (vec_duplicate:V_VLSF_ZVFHMIN
	    (match_operand:<VEL> 3 "direct_broadcast_operand"       " f,  f,Wdm,Wdm,Wdm,Wdm,  f,  f"))
	  (match_operand:V_VLSF_ZVFHMIN 2 "vector_merge_operand"    "vu,  0, vu,  0, vu,  0, vu,  0")))]
  "TARGET_XTHEADVECTOR"
  "@
   vfmv.v.f\t%0,%3
   vfmv.v.f\t%0,%3
   vlse.v\t%0,%3,zero,%1.t
   vlse.v\t%0,%3,zero,%1.t
   vlse.v\t%0,%3,zero
   vlse.v\t%0,%3,zero
   vfmv.s.f\t%0,%3
   vfmv.s.f\t%0,%3"
  [(set_attr "type" "vfmov,vfmov,vlds,vlds,vlds,vlds,vfmovfv,vfmovfv")
   (set_attr "mode" "<MODE>")])

;; vle.v/vse.v,vmv.v.v
(define_insn_and_split "*pred_th_mov<mode>"
  [(set (match_operand:V_VLS 0 "nonimmediate_operand"            "=vr,    vr,    vd,     m,    vr,    vr")
    (if_then_else:V_VLS
      (unspec:<VM>
        [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1,   Wc1,    vm, vmWc1,   Wc1,   Wc1")
         (match_operand 4 "vector_length_operand"              "   rK,    rK,    rK,    rK,    rK,    rK")
         (match_operand 5 "const_int_operand"                  "    i,     i,     i,     i,     i,     i")
         (match_operand 6 "const_int_operand"                  "    i,     i,     i,     i,     i,     i")
         (match_operand 7 "const_int_operand"                  "    i,     i,     i,     i,     i,     i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
      (match_operand:V_VLS 3 "reg_or_mem_operand"              "    m,     m,     m,    vr,    vr,    vr")
      (match_operand:V_VLS 2 "vector_merge_operand"            "    0,    vu,    vu,    vu,    vu,     0")))]
  "(TARGET_XTHEADVECTOR
    && (register_operand (operands[0], <MODE>mode)
        || register_operand (operands[3], <MODE>mode)))"
  "@
   vle.v\t%0,%3%p1
   vle.v\t%0,%3
   vle.v\t%0,%3,%1.t
   vse.v\t%3,%0%p1
   vmv.v.v\t%0,%3
   vmv.v.v\t%0,%3"
  "&& register_operand (operands[0], <MODE>mode)
   && register_operand (operands[3], <MODE>mode)
   && satisfies_constraint_vu (operands[2])
   && INTVAL (operands[7]) == riscv_vector::VLMAX"
  [(set (match_dup 0) (match_dup 3))]
  ""
  [(set_attr "type" "vlde,vlde,vlde,vste,vimov,vimov")
   (set_attr "mode" "<MODE>")])

(define_insn_and_split "@pred_th_mov<mode>"
  [(set (match_operand:VB_VLS 0 "nonimmediate_operand"               "=vr,   m,  vr,  vr,  vr")
	(if_then_else:VB_VLS
	  (unspec:VB_VLS
	    [(match_operand:VB_VLS 1 "vector_all_trues_mask_operand" "Wc1, Wc1, Wc1, Wc1, Wc1")
	     (match_operand 4 "vector_length_operand"            " rK,  rK,  rK,  rK,  rK")
	     (match_operand 5 "const_int_operand"                "  i,   i,   i,   i,   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operand:VB_VLS 3 "vector_move_operand"              "  m,  vr,  vr, Wc0, Wc1")
	  (match_operand:VB_VLS 2 "vector_undef_operand"             " vu,  vu,  vu,  vu,  vu")))]
  "TARGET_XTHEADVECTOR"
  "@
   #
   #
   vmcpy.m\t%0,%3
   vmclr.m\t%0
   vmset.m\t%0"
  "&& !reload_completed"
  [(const_int 0)]
  {
    if ((MEM_P (operands[0]) || MEM_P (operands[3]))
        || (REG_P (operands[0]) && REG_P (operands[3])
	    && INTVAL (operands[5]) == riscv_vector::VLMAX))
      {
	emit_move_insn (operands[0], operands[3]);
	DONE;
      }

    FAIL;
  }
  [(set_attr "type" "vldm,vstm,vmalu,vmalu,vmalu")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_store<mode>"
  [(set (match_operand:V 0 "memory_operand"                 "+m")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
	     (match_operand 3 "vector_length_operand"    "   rK")
	     (match_operand 4 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operand:V 2 "register_operand"         "    vr")
	  (match_dup 0)))]
  "TARGET_XTHEADVECTOR"
  "vse.v\t%2,%0%p1"
  [(set_attr "type" "vste")
   (set_attr "mode" "<MODE>")
   (set (attr "avl_type_idx") (const_int 4))
   (set_attr "vl_op_idx" "3")])

(define_insn "@pred_th_strided_load<mode>"
  [(set (match_operand:V 0 "register_operand"              "=vr,    vr,    vd,    vr,    vr,    vd")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm,    vmWc1,   Wc1,    vm")
	     (match_operand 5 "vector_length_operand"    "   rK,    rK,    rK,       rK,    rK,    rK")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i,        i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i,        i,     i,     i")
	     (match_operand 8 "const_int_operand"        "    i,     i,     i,        i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V
	    [(match_operand:V 3 "memory_operand"         "     m,     m,     m,    m,     m,     m")
	     (match_operand 4 "<V:stride_predicate>"     "<V:stride_load_constraint>")] UNSPEC_STRIDED)
	  (match_operand:V 2 "vector_merge_operand"      "     0,    vu,    vu,    0,    vu,    vu")))]
  "TARGET_XTHEADVECTOR"
  "@
  vlse.v\t%0,%3,%z4%p1
  vlse.v\t%0,%3,%z4
  vlse.v\t%0,%3,%z4,%1.t
  vle.v\t%0,%3%p1
  vle.v\t%0,%3
  vle.v\t%0,%3,%1.t"
  [(set_attr "type" "vlds")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_strided_store<mode>"
  [(set (match_operand:V 0 "memory_operand"                 "+m,    m")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,    vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK,       rK")
	     (match_operand 5 "const_int_operand"        "    i,        i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V
	    [(match_operand 2 "<V:stride_predicate>"     "<V:stride_store_constraint>")
	     (match_operand:V 3 "register_operand"       "   vr,       vr")] UNSPEC_STRIDED)
	  (match_dup 0)))]
  "TARGET_XTHEADVECTOR"
  "@
  vsse.v\t%3,%0,%z2%p1
  vse.v\t%3,%0%p1"
  [(set_attr "type" "vsts")
   (set_attr "mode" "<MODE>")
   (set (attr "avl_type_idx") (const_int 5))])


(define_insn "@pred_th_indexed_<order>load<mode>_same_eew"
  [(set (match_operand:V 0 "register_operand"             "=vd, vr,vd, vr")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  " vm,Wc1,vm,Wc1")
	     (match_operand 5 "vector_length_operand"     " rK, rK,rK, rK")
	     (match_operand 6 "const_int_operand"         "  i,  i, i,  i")
	     (match_operand 7 "const_int_operand"         "  i,  i, i,  i")
	     (match_operand 8 "const_int_operand"         "  i,  i, i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V
	    [(match_operand 3 "pmode_reg_or_0_operand"    " rJ, rJ,rJ, rJ")
	     (mem:BLK (scratch))
	     (match_operand:<VINDEX> 4 "register_operand" " vr, vr,vr, vr")] ORDER)
	  (match_operand:V 2 "vector_merge_operand"       " vu, vu, 0,  0")))]
  "TARGET_XTHEADVECTOR"
  "vlxe.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<MODE>")])

;; DEST eew is greater than SOURCE eew.
(define_insn "@pred_th_indexed_<order>load<mode>_x2_greater_eew"
  [(set (match_operand:VEEWEXT2 0 "register_operand"                    "=&vr,  &vr")
	(if_then_else:VEEWEXT2
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  "   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "    i,    i")
	     (match_operand 7 "const_int_operand"                      "    i,    i")
	     (match_operand 8 "const_int_operand"                      "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VEEWEXT2
	    [(match_operand 3 "pmode_reg_or_0_operand"                 "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:<VINDEX_DOUBLE_TRUNC> 4 "register_operand" "   vr,   vr")] ORDER)
	  (match_operand:VEEWEXT2 2 "vector_merge_operand"             "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxe.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_indexed_<order>load<mode>_x4_greater_eew"
  [(set (match_operand:VEEWEXT4 0 "register_operand"                    "=&vr,  &vr")
	(if_then_else:VEEWEXT4
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  "   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "    i,    i")
	     (match_operand 7 "const_int_operand"                      "    i,    i")
	     (match_operand 8 "const_int_operand"                      "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VEEWEXT4
	    [(match_operand 3 "pmode_reg_or_0_operand"                 "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:<VINDEX_QUAD_TRUNC> 4 "register_operand"   "   vr,   vr")] ORDER)
	  (match_operand:VEEWEXT4 2 "vector_merge_operand"             "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxe.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_indexed_<order>load<mode>_x8_greater_eew"
  [(set (match_operand:VEEWEXT8 0 "register_operand"                    "=&vr,  &vr")
	(if_then_else:VEEWEXT8
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  "   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "    i,    i")
	     (match_operand 7 "const_int_operand"                      "    i,    i")
	     (match_operand 8 "const_int_operand"                      "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VEEWEXT8
	    [(match_operand 3 "pmode_reg_or_0_operand"                 "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:<VINDEX_OCT_TRUNC> 4 "register_operand"    "   vr,   vr")] ORDER)
	  (match_operand:VEEWEXT8 2 "vector_merge_operand"             "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxe.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<MODE>")])

;; DEST eew is smaller than SOURCE eew.
(define_insn "@pred_th_indexed_<order>load<mode>_x2_smaller_eew"
  [(set (match_operand:VEEWTRUNC2 0 "register_operand"               "=vd, vd, vr, vr,  &vr,  &vr")
	(if_then_else:VEEWTRUNC2
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                    "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                    "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                    "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VEEWTRUNC2
	    [(match_operand 3 "pmode_reg_or_0_operand"               " rJ, rJ, rJ, rJ,   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:<VINDEX_DOUBLE_EXT> 4 "register_operand" "  0,  0,  0,  0,   vr,   vr")] ORDER)
	  (match_operand:VEEWTRUNC2 2 "vector_merge_operand"         " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxe.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_indexed_<order>load<mode>_x4_smaller_eew"
  [(set (match_operand:VEEWTRUNC4 0 "register_operand"             "=vd, vd, vr, vr,  &vr,  &vr")
	(if_then_else:VEEWTRUNC4
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"              " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VEEWTRUNC4
	    [(match_operand 3 "pmode_reg_or_0_operand"             " rJ, rJ, rJ, rJ,   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:<VINDEX_QUAD_EXT> 4 "register_operand" "  0,  0,  0,  0,   vr,   vr")] ORDER)
	  (match_operand:VEEWTRUNC4 2 "vector_merge_operand"       " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxe.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_indexed_<order>load<mode>_x8_smaller_eew"
  [(set (match_operand:VEEWTRUNC8 0 "register_operand"            "=vd, vd, vr, vr,  &vr,  &vr")
	(if_then_else:VEEWTRUNC8
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"          " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"             " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                 "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                 "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                 "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VEEWTRUNC8
	    [(match_operand 3 "pmode_reg_or_0_operand"            " rJ, rJ, rJ, rJ,   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:<VINDEX_OCT_EXT> 4 "register_operand" "  0,  0,  0,  0,   vr,   vr")] ORDER)
	  (match_operand:VEEWTRUNC8 2 "vector_merge_operand"      " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxe.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vld<order>x")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<RATIO64:mode><RATIO64I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"      "  rJ")
	   (match_operand:RATIO64I 2 "register_operand" "  vr")
	   (match_operand:RATIO64 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xe.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<RATIO64:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<RATIO32:mode><RATIO32I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"      "  rJ")
	   (match_operand:RATIO32I 2 "register_operand" "  vr")
	   (match_operand:RATIO32 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xe.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<RATIO32:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<RATIO16:mode><RATIO16I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"      "  rJ")
	   (match_operand:RATIO16I 2 "register_operand" "  vr")
	   (match_operand:RATIO16 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xe.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<RATIO16:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<RATIO8:mode><RATIO8I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"      "  rJ")
	   (match_operand:RATIO8I 2 "register_operand" "  vr")
	   (match_operand:RATIO8 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xe.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<RATIO8:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<RATIO4:mode><RATIO4I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"      "  rJ")
	   (match_operand:RATIO4I 2 "register_operand" "  vr")
	   (match_operand:RATIO4 3 "register_operand"  "  vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xe.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<RATIO4:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<RATIO2:mode><RATIO2I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"       "  rJ")
	   (match_operand:RATIO2I 2 "register_operand"  "  vr")
	   (match_operand:RATIO2 3 "register_operand"   "  vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xe.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<RATIO2:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<RATIO1:mode><RATIO1:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"       "  rJ")
	   (match_operand:RATIO1 2 "register_operand"   "  vr")
	   (match_operand:RATIO1 3 "register_operand"    "  vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xe.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<RATIO1:MODE>")])

(define_insn "@pred_th_popcount<VB:mode><P:mode>"
  [(set (match_operand:P 0 "register_operand"               "=r")
	(popcount:P
	  (unspec:VB
	    [(and:VB
	       (match_operand:VB 1 "vector_mask_operand" "vmWc1")
	       (match_operand:VB 2 "register_operand"    "   vr"))
	     (match_operand 3 "vector_length_operand"    "   rK")
	     (match_operand 4 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)))]
  "TARGET_XTHEADVECTOR"
  "vmpopc.m\t%0,%2%p1"
  [(set_attr "type" "vmpop")
   (set_attr "mode" "<VB:MODE>")])

(define_insn "@pred_th_ffs<VB:mode><P:mode>"
  [(set (match_operand:P 0 "register_operand"                 "=r")
	(plus:P
	  (ffs:P
	    (unspec:VB
	      [(and:VB
	         (match_operand:VB 1 "vector_mask_operand" "vmWc1")
	         (match_operand:VB 2 "register_operand"    "   vr"))
	       (match_operand 3 "vector_length_operand"    "   rK")
	       (match_operand 4 "const_int_operand"        "    i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE))
	  (const_int -1)))]
  "TARGET_XTHEADVECTOR"
  "vmfirst.m\t%0,%2%p1"
  [(set_attr "type" "vmffs")
   (set_attr "mode" "<VB:MODE>")])

(define_insn "@pred_th_narrow_fcvt_x<v_su>_f<mode>"
  [(set (match_operand:<VNCONVERT> 0 "register_operand"        "=&vd, &vd, &vr, &vr,  &vr,  &vr")
	(if_then_else:<VNCONVERT>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"          " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 5 "const_int_operand"              "  i,  i,  i,  i,    i,    i")
	     (match_operand 6 "const_int_operand"              "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"              "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"              "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<VNCONVERT>
	     [(match_operand:V_VLSF 3 "register_operand"       "  vd,  vd,  vr,  vr,   vr,   vr")] VFCVTS)
	  (match_operand:<VNCONVERT> 2 "vector_merge_operand"  " vu,  vd, vu,  vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vfncvt.x<v_su>.f.v\t%0,%3%p1"
  [(set_attr "type" "vfncvtftoi")
   (set_attr "mode" "<VNCONVERT>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_th_narrow_<float_cvt><mode>"
  [(set (match_operand:<VNCONVERT> 0 "register_operand"       "=&vd, &vd, &vr, &vr,  &vr,  &vr")
	(if_then_else:<VNCONVERT>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"         " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 5 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (match_operand 6 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (any_float:<VNCONVERT>
	     (match_operand:VWCONVERTI 3 "register_operand"   "  vd,  vd,  vr,  vr,   vr,   vr"))
	  (match_operand:<VNCONVERT> 2 "vector_merge_operand" " vu,  vd, vu,  vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vfncvt.f.x<u>.v\t%0,%3%p1"
  [(set_attr "type" "vfncvtitof")
   (set_attr "mode" "<VNCONVERT>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_th_narrow_<optab><mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=&vd,&vd, &vr, &vr,&vd, &vr,  &vr,  &vr, vd, vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,vm,Wc1,Wc1,vm,Wc1,vmWc1,vmWc1, vm,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK,rK, rK, rK,rK, rK,   rK,   rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (truncate:<V_DOUBLE_TRUNC>
	    (any_shiftrt:VWEXTI
	     (match_operand:VWEXTI 3 "register_operand"                " vr,vr, vr, vr, vd,  vr,   vr,   vr,  vd,  vr,   vr,   vr")
	     (match_operand:<V_DOUBLE_TRUNC> 4 "vector_shift_operand"  "  vd, vd,  vr,  vr,vr, vr,   vr,   vr, vk, vk,   vk,   vk")))
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     "  vd,vu,  vr, vu,vu, vu,   vu,    vr, vu, vu,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vn<insn>.v%o4\t%0,%3,%v4%p1"
  [(set_attr "type" "vnshift")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_th_narrow_<optab><mode>_scalar"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=&vd, &vd, &vr, &vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (truncate:<V_DOUBLE_TRUNC>
	    (any_shiftrt:VWEXTI
	     (match_operand:VWEXTI 3 "register_operand"                "  vd,  vd,  vr,  vr,   vr,   vr")
	     (match_operand 4 "pmode_reg_or_uimm5_operand"             " rK, rK, rK, rK,   rK,   rK")))
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     " vu,  vd, vu,  vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vn<insn>.v%o4\t%0,%3,%4%p1"
  [(set_attr "type" "vnshift")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_th_trunc<mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=&vd, &vd, &vr, &vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"                  " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 5 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 6 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (truncate:<V_DOUBLE_TRUNC>
	    (match_operand:VWEXTI 3 "register_operand"                 "  vd,  vd,  vr,  vr,   vr,   vr"))
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     " vu,  vd, vu,  vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vnsrl.vx\t%0,%3,x0%p1"
  [(set_attr "type" "vnshift")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

(define_insn "@pred_th_trunc<mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"       "=&vd, &vd, &vr, &vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"              " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 5 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 6 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (float_truncate:<V_DOUBLE_TRUNC>
	     (match_operand:VWEXTF_ZVFHMIN 3 "register_operand"            "  vd,  vd,  vr,  vr,   vr,   vr"))
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand" " vu,  vd, vu,  vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vfncvt.f.f.v\t%0,%3%p1"
  [(set_attr "type" "vfncvtftof")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_th_fault_load<mode>"
  [(set (match_operand:V 0 "register_operand"              "=vd,    vd,    vr,    vr")
	(if_then_else:V
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "   vm,    vm,   Wc1,   Wc1")
	     (match_operand 4 "vector_length_operand"    "   rK,    rK,    rK,    rK")
	     (match_operand 5 "const_int_operand"        "    i,     i,     i,     i")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V
	    [(match_operand:V 3 "memory_operand"         "    m,     m,     m,     m")] UNSPEC_VLEFF)
	  (match_operand:V 2 "vector_merge_operand"      "   vu,     0,    vu,     0")))
   (set (reg:SI VL_REGNUM)
	  (unspec:SI
	    [(if_then_else:V
	       (unspec:<VM>
		[(match_dup 1) (match_dup 4) (match_dup 5)
		 (match_dup 6) (match_dup 7)
		 (reg:SI VL_REGNUM) (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	       (unspec:V [(match_dup 3)] UNSPEC_VLEFF)
	       (match_dup 2))] UNSPEC_MODIFY_VL))]
  "TARGET_XTHEADVECTOR"
  "vleff.v\t%0,%3%p1"
  [(set_attr "type" "vldff")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_unit_strided_load<mode>"
  [(set (match_operand:VT 0 "register_operand"             "=vr,    vr,    vd")
	(if_then_else:VT
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm")
	     (match_operand 4 "vector_length_operand"    "   rK,    rK,    rK")
	     (match_operand 5 "const_int_operand"        "    i,     i,     i")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VT
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")
	     (mem:BLK (scratch))] UNSPEC_UNIT_STRIDED)
	  (match_operand:VT 2 "vector_merge_operand"     "    0,    vu,    vu")))]
  "TARGET_XTHEADVECTOR"
  "vlseg<nf>e.v\t%0,(%z3)%p1"
  [(set_attr "type" "vlsegde")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_unit_strided_store<mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	     [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	      (match_operand 3 "vector_length_operand"    "   rK")
	      (match_operand 4 "const_int_operand"        "    i")
	      (reg:SI VL_REGNUM)
	      (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"      "   rJ")
	   (match_operand:VT 2 "register_operand"         "   vr")
	   (mem:BLK (scratch))] UNSPEC_UNIT_STRIDED))]
  "TARGET_XTHEADVECTOR"
  "vsseg<nf>e.v\t%2,(%z1)%p0"
  [(set_attr "type" "vssegte")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_strided_load<mode>"
  [(set (match_operand:VT 0 "register_operand"             "=vr,    vr,    vd")
	(if_then_else:VT
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm")
	     (match_operand 5 "vector_length_operand"    "   rK,    rK,    rK")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i")
	     (match_operand 8 "const_int_operand"        "    i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VT
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")
	     (match_operand 4 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")
	     (mem:BLK (scratch))] UNSPEC_STRIDED)
	  (match_operand:VT 2 "vector_merge_operand"     "    0,    vu,    vu")))]
  "TARGET_XTHEADVECTOR"
  "vlsseg<nf>e.v\t%0,(%z3),%z4%p1"
  [(set_attr "type" "vlsegds")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_strided_store<mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	     [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	      (match_operand 4 "vector_length_operand"    "   rK")
	      (match_operand 5 "const_int_operand"        "    i")
	      (reg:SI VL_REGNUM)
	      (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"      "   rJ")
	   (match_operand 2 "pmode_reg_or_0_operand"      "   rJ")
	   (match_operand:VT 3 "register_operand"         "   vr")
	   (mem:BLK (scratch))] UNSPEC_STRIDED))]
  "TARGET_XTHEADVECTOR"
  "vssseg<nf>e.v\t%3,(%z1),%z2%p0"
  [(set_attr "type" "vssegts")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_fault_load<mode>"
  [(set (match_operand:VT 0 "register_operand"             "=vr,    vr,    vd")
	(if_then_else:VT
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm")
	     (match_operand 4 "vector_length_operand"    "   rK,    rK,    rK")
	     (match_operand 5 "const_int_operand"        "    i,     i,     i")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VT
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")
	     (mem:BLK (scratch))] UNSPEC_VLEFF)
	  (match_operand:VT 2 "vector_merge_operand"     "    0,    vu,    vu")))
   (set (reg:SI VL_REGNUM)
        (unspec:SI
          [(if_then_else:VT
	     (unspec:<VM>
	       [(match_dup 1) (match_dup 4) (match_dup 5)
	        (match_dup 6) (match_dup 7)
	        (reg:SI VL_REGNUM)
	        (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	     (unspec:VT
	        [(match_dup 3) (mem:BLK (scratch))] UNSPEC_VLEFF)
	     (match_dup 2))] UNSPEC_MODIFY_VL))]
  "TARGET_XTHEADVECTOR"
  "vlseg<nf>eff.v\t%0,(%z3)%p1"
  [(set_attr "type" "vlsegdff")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_indexed_<order>load<V1T:mode><RATIO64I:mode>"
  [(set (match_operand:V1T 0 "register_operand"           "=&vr,  &vr")
	(if_then_else:V1T
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    "   rK,   rK")
	     (match_operand 6 "const_int_operand"        "    i,    i")
	     (match_operand 7 "const_int_operand"        "    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V1T
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:RATIO64I 4 "register_operand"     "   vr,   vr")] ORDER)
	  (match_operand:V1T 2 "vector_merge_operand"    "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxseg<nf>e.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vlsegd<order>x")
   (set_attr "mode" "<V1T:MODE>")])

(define_insn "@pred_th_indexed_<order>load<V2T:mode><RATIO32I:mode>"
  [(set (match_operand:V2T 0 "register_operand"           "=&vr,  &vr")
	(if_then_else:V2T
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    "   rK,   rK")
	     (match_operand 6 "const_int_operand"        "    i,    i")
	     (match_operand 7 "const_int_operand"        "    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V2T
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:RATIO32I 4 "register_operand"     "   vr,   vr")] ORDER)
	  (match_operand:V2T 2 "vector_merge_operand"    "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxseg<nf>e.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vlsegd<order>x")
   (set_attr "mode" "<V2T:MODE>")])

(define_insn "@pred_th_indexed_<order>load<V4T:mode><RATIO16I:mode>"
  [(set (match_operand:V4T 0 "register_operand"           "=&vr,  &vr")
	(if_then_else:V4T
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    "   rK,   rK")
	     (match_operand 6 "const_int_operand"        "    i,    i")
	     (match_operand 7 "const_int_operand"        "    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V4T
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:RATIO16I 4 "register_operand"     "   vr,   vr")] ORDER)
	  (match_operand:V4T 2 "vector_merge_operand"    "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxseg<nf>e.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vlsegd<order>x")
   (set_attr "mode" "<V4T:MODE>")])

(define_insn "@pred_th_indexed_<order>load<V8T:mode><RATIO8I:mode>"
  [(set (match_operand:V8T 0 "register_operand"           "=&vr,  &vr")
	(if_then_else:V8T
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    "   rK,   rK")
	     (match_operand 6 "const_int_operand"        "    i,    i")
	     (match_operand 7 "const_int_operand"        "    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V8T
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:RATIO8I 4 "register_operand"     "   vr,   vr")] ORDER)
	  (match_operand:V8T 2 "vector_merge_operand"    "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxseg<nf>e.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vlsegd<order>x")
   (set_attr "mode" "<V8T:MODE>")])

(define_insn "@pred_th_indexed_<order>load<V16T:mode><RATIO4I:mode>"
  [(set (match_operand:V16T 0 "register_operand"          "=&vr,  &vr")
	(if_then_else:V16T
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    "   rK,   rK")
	     (match_operand 6 "const_int_operand"        "    i,    i")
	     (match_operand 7 "const_int_operand"        "    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V16T
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:RATIO4I 4 "register_operand"    "   vr,   vr")] ORDER)
	  (match_operand:V16T 2 "vector_merge_operand"   "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxseg<nf>e.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vlsegd<order>x")
   (set_attr "mode" "<V16T:MODE>")])

(define_insn "@pred_th_indexed_<order>load<V32T:mode><RATIO2I:mode>"
  [(set (match_operand:V32T 0 "register_operand"          "=&vr,  &vr")
	(if_then_else:V32T
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    "   rK,   rK")
	     (match_operand 6 "const_int_operand"        "    i,    i")
	     (match_operand 7 "const_int_operand"        "    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:V32T
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:RATIO2I 4 "register_operand"    "   vr,   vr")] ORDER)
	  (match_operand:V32T 2 "vector_merge_operand"   "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxseg<nf>e.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vlsegd<order>x")
   (set_attr "mode" "<V32T:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<V1T:mode><RATIO64I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"     "   rJ")
	   (match_operand:RATIO64I 2 "register_operand"       "   vr")
	   (match_operand:V1T 3 "register_operand"       "   vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xseg<nf>e.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vssegtux")
   (set_attr "mode" "<V1T:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<V2T:mode><RATIO32I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"     "   rJ")
	   (match_operand:RATIO32I 2 "register_operand"       "   vr")
	   (match_operand:V2T 3 "register_operand"       "   vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xseg<nf>e.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vssegtux")
   (set_attr "mode" "<V2T:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<V4T:mode><RATIO16I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"     "   rJ")
	   (match_operand:RATIO16I 2 "register_operand"       "   vr")
	   (match_operand:V4T 3 "register_operand"       "   vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xseg<nf>e.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vssegtux")
   (set_attr "mode" "<V4T:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<V8T:mode><RATIO8I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"     "   rJ")
	   (match_operand:RATIO8I 2 "register_operand"       "   vr")
	   (match_operand:V8T 3 "register_operand"       "   vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xseg<nf>e.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vssegtux")
   (set_attr "mode" "<V8T:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<V16T:mode><RATIO4I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"     "   rJ")
	   (match_operand:RATIO4I 2 "register_operand"      "   vr")
	   (match_operand:V16T 3 "register_operand"      "   vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xseg<nf>e.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vssegtux")
   (set_attr "mode" "<V16T:MODE>")])

(define_insn "@pred_th_indexed_<th_order>store<V32T:mode><RATIO2I:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"        "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	   (match_operand 1 "pmode_reg_or_0_operand"     "   rJ")
	   (match_operand:RATIO2I 2 "register_operand"      "   vr")
	   (match_operand:V32T 3 "register_operand"      "   vr")] ORDER))]
  "TARGET_XTHEADVECTOR"
  "vs<th_order>xseg<nf>e.v\t%3,(%z1),%2%p0";
  [(set_attr "type" "vssegtux")
   (set_attr "mode" "<V32T:MODE>")])

(define_insn "@pred_th_<optab><mode>"
  [(set (match_operand:V_VLSF 0 "register_operand"           "=vd, vd, vr, vr")
	(if_then_else:V_VLSF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm, vm,Wc1,Wc1")
	     (match_operand 4 "vector_length_operand"    " rK, rK, rK, rK")
	     (match_operand 5 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 6 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_float_unop_neg:V_VLSF
	    (match_operand:V_VLSF 3 "register_operand"       " vr, vr, vr, vr"))
	  (match_operand:V_VLSF 2 "vector_merge_operand"     " vu,  0, vu,  0")))]
  "TARGET_XTHEADVECTOR"
  "vfsgnjn.vv\t%0,%3,%3%p1"
  [(set_attr "type" "<float_insn_type>")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

(define_insn "@pred_th_<optab><mode>"
  [(set (match_operand:V_VLSF 0 "register_operand"           "=vd, vd, vr, vr")
	(if_then_else:V_VLSF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm, vm,Wc1,Wc1")
	     (match_operand 4 "vector_length_operand"    " rK, rK, rK, rK")
	     (match_operand 5 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 6 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_float_unop_abs:V_VLSF
	    (match_operand:V_VLSF 3 "register_operand"       " vr, vr, vr, vr"))
	  (match_operand:V_VLSF 2 "vector_merge_operand"     " vu,  0, vu,  0")))]
  "TARGET_XTHEADVECTOR"
  "vfsgnjx.vv\t%0,%3,%3%p1"
  [(set_attr "type" "<float_insn_type>")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

(define_insn "@pred_th_<optab><mode>"
  [(set (match_operand:V_VLSI 0 "register_operand"          "=vd,vd, vr, vr")
	(if_then_else:V_VLSI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vm,vm,Wc1,Wc1")
	     (match_operand 4 "vector_length_operand"    "rK,rK, rK, rK")
	     (match_operand 5 "const_int_operand"        " i, i,  i,  i")
	     (match_operand 6 "const_int_operand"        " i, i,  i,  i")
	     (match_operand 7 "const_int_operand"        " i, i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (not_unop:V_VLSI
	    (match_operand:V_VLSI 3 "register_operand"       "vr,vr, vr, vr"))
	  (match_operand:V_VLSI 2 "vector_merge_operand"     "vu, 0, vu,  0")))]
  "TARGET_XTHEADVECTOR"
  "vnot.v\t%0,%3%p1"
  [(set_attr "type" "vialu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta (operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma (operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

(define_insn "@pred_th_<optab><mode>"
  [(set (match_operand:V_VLSI 0 "register_operand"	 "=vd,vd, vr, vr")
	(if_then_else:V_VLSI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vm,vm,Wc1,Wc1")
	     (match_operand 4 "vector_length_operand"    "rK,rK, rK, rK")
	     (match_operand 5 "const_int_operand"	 " i, i,  i,  i")
	     (match_operand 6 "const_int_operand"	 " i, i,  i,  i")
	     (match_operand 7 "const_int_operand"	 " i, i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (neg_unop:V_VLSI
	    (match_operand:V_VLSI 3 "register_operand"       "vr,vr, vr, vr"))
	  (match_operand:V_VLSI 2 "vector_merge_operand"     "vu, 0, vu,  0")))]
  "TARGET_XTHEADVECTOR"
  "vrsub.vx\t%0,%3,x0%p1"
  [(set_attr "type" "vialu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

(define_insn "@pred_th_<optab><mode>"
  [(set (match_operand:V_VLSF 0 "register_operand"           "=vd, vd, vr, vr")
	(if_then_else:V_VLSF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm, vm,Wc1,Wc1")
	     (match_operand 4 "vector_length_operand"    " rK, rK, rK, rK")
	     (match_operand 5 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 6 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (any_float_unop:V_VLSF
	    (match_operand:V_VLSF 3 "register_operand"       " vr, vr, vr, vr"))
	  (match_operand:V_VLSF 2 "vector_merge_operand"     " vu,  0, vu,  0")))]
  "TARGET_XTHEADVECTOR"
  "vf<insn>.v\t%0,%3%p1"
  [(set_attr "type" "<float_insn_type>")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_th_narrow_clip<v_su><mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=&vd,&vd, &vr, &vr,&vd, &vr,  &vr,  &vr, &vd, &vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,vm,Wc1,Wc1,vm,Wc1,vmWc1,vmWc1, vm,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK,rK, rK, rK,rK, rK,   rK,   rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 9 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI VXRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<V_DOUBLE_TRUNC>
	    [(match_operand:VWEXTI 3 "register_operand"                " vr,vr, vr, vr, vd,  vr,   vr,   vr,  vd,  vr,   vr,   vr")
	     (match_operand:<V_DOUBLE_TRUNC> 4 "vector_shift_operand"  "  vd, vd,  vr,  vr,vr, vr,   vr,   vr, vk, vk,   vk,   vk")] VNCLIP)
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     "  vd,vu,  vr, vu,vu, vu,   vu,    vr, vu, vu,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vnclip<v_su>.v%o4\t%0,%3,%v4%p1"
  [(set_attr "type" "vnclip")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_th_narrow_clip<v_su><mode>_scalar"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=&vd, &vd, &vr, &vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 9 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI VXRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<V_DOUBLE_TRUNC>
	    [(match_operand:VWEXTI 3 "register_operand"                "  vd,  vd,  vr,  vr,   vr,   vr")
	     (match_operand 4 "pmode_reg_or_uimm5_operand"             " rK, rK, rK, rK,   rK,   rK")] VNCLIP)
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     " vu,  vd, vu,  vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR"
  "vnclip<v_su>.v%o4\t%0,%3,%4%p1"
  [(set_attr "type" "vnclip")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

;; Float Reduction Sum (vfred[ou]sum.vs)
(define_insn "@pred_th_<th_reduc_op><mode>"
  [(set (match_operand:<V_LMUL1>           0 "register_operand"      "=vr,vr")
	(unspec:<V_LMUL1>
	  [(unspec:<VM>
	    [(match_operand:<VM>          1 "vector_mask_operand"   "vmWc1,vmWc1")
	     (match_operand               5 "vector_length_operand" "   rK,   rK")
	     (match_operand               6 "const_int_operand"     "    i,    i")
	     (match_operand               7 "const_int_operand"     "    i,    i")
	     (match_operand               8 "const_int_operand"     "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
           (unspec:<V_LMUL1> [
             (match_operand:V_VLSF        3 "register_operand"      "   vr,   vr")
             (match_operand:<V_LMUL1>     4 "register_operand"      "   vr,   vr")
           ] ANY_FREDUC_SUM)
	   (match_operand:<V_LMUL1>       2 "vector_merge_operand"  "   vu,    0")] UNSPEC_REDUC))]
  "TARGET_XTHEADVECTOR"
  "vf<th_reduc_op>.vs\t%0,%3,%4%p1"
  [(set_attr "type" "vfred<order>")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

;; Float Widen Reduction Sum (vfwred[ou]sum.vs)
(define_insn "@pred_th_<th_reduc_op><mode>"
  [(set (match_operand:<V_EXT_LMUL1>         0 "register_operand"      "=&vr, &vr")
	(unspec:<V_EXT_LMUL1>
	  [(unspec:<VM>
	    [(match_operand:<VM>           1 "vector_mask_operand"   "vmWc1,vmWc1")
	     (match_operand                5 "vector_length_operand" "   rK,   rK")
	     (match_operand                6 "const_int_operand"     "    i,    i")
	     (match_operand                7 "const_int_operand"     "    i,    i")
	     (match_operand                8 "const_int_operand"     "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
           (unspec:<V_EXT_LMUL1> [
	     (match_operand:VF_HS          3 "register_operand"      "   vr,   vr")
	     (match_operand:<V_EXT_LMUL1>  4 "register_operand"      "  vr0,  vr0")
           ] ANY_FWREDUC_SUM)
	   (match_operand:<V_EXT_LMUL1>    2 "vector_merge_operand"  "   vu,    0")] UNSPEC_REDUC))]
  "TARGET_XTHEADVECTOR"
  "vf<th_reduc_op>.vs\t%0,%3,%4%p1"
  [(set_attr "type" "vfwred<order>")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_th_madc<mode>"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr, &vr, &vr")
	(unspec:<VM>
	   [(plus:VI
	     (match_operand:VI 1 "register_operand"     "  %vr,  vr,  vr")
	     (match_operand:VI 2 "vector_arith_operand" "vrvi,  vr,  vi"))
	    (match_operand:<VM> 3 "register_operand"    "  vm,  vm,  vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand" "  rK,  rK,  rK")
	       (match_operand 5 "const_int_operand"     "   i,   i,   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_XTHEADVECTOR"
  "vmadc.v%o2m\t%0,%1,%v2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_insn "@pred_th_msbc<mode>"
  [(set (match_operand:<VM> 0 "register_operand"        "=&vr")
	(unspec:<VM>
	   [(minus:VI
	     (match_operand:VI 1 "register_operand"     "  vr")
	     (match_operand:VI 2 "register_operand"     " vr"))
	    (match_operand:<VM> 3 "register_operand"    " vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand" " rK")
	       (match_operand 5 "const_int_operand"     "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vvm\t%0,%1,%2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_insn "@pred_th_madc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "register_operand" "  r"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (match_operand:<VM> 3 "register_operand"     " vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  " rK")
	       (match_operand 5 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_XTHEADVECTOR"
  "vmadc.vxm\t%0,%1,%2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_insn "@pred_th_msbc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "reg_or_0_operand" " rJ"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (match_operand:<VM> 3 "register_operand"     " vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  " rK")
	       (match_operand 5 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_expand "@pred_th_madc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (match_operand:<VM> 3 "register_operand")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand")
	       (match_operand 5 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_XTHEADVECTOR"
{
  if (riscv_vector::sew64_scalar_helper (
	operands,
	/* scalar op */&operands[2],
	/* vl */operands[4],
	<MODE>mode,
	riscv_vector::simm5_p (operands[2]),
	[] (rtx *operands, rtx boardcast_scalar) {
	  emit_insn (gen_pred_th_madc<mode> (operands[0], operands[1],
	       boardcast_scalar, operands[3], operands[4], operands[5]));
        },
	(riscv_vector::avl_type) INTVAL (operands[5])))
    DONE;
})

(define_insn "*pred_th_madc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_0_operand" " rJ"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (match_operand:<VM> 3 "register_operand"     " vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  " rK")
	       (match_operand 5 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_XTHEADVECTOR"
  "vmadc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_insn "*pred_th_madc<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"             "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "reg_or_0_operand" " rJ")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (match_operand:<VM> 3 "register_operand"          " vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"       " rK")
	       (match_operand 5 "const_int_operand"           "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMADC))]
  "TARGET_XTHEADVECTOR"
  "vmadc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_expand "@pred_th_msbc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (match_operand:<VM> 3 "register_operand")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand")
	       (match_operand 5 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_XTHEADVECTOR"
{
  if (riscv_vector::sew64_scalar_helper (
	operands,
	/* scalar op */&operands[2],
	/* vl */operands[4],
	<MODE>mode,
	false,
	[] (rtx *operands, rtx boardcast_scalar) {
	  emit_insn (gen_pred_th_msbc<mode> (operands[0], operands[1],
	       boardcast_scalar, operands[3], operands[4], operands[5]));
        },
	(riscv_vector::avl_type) INTVAL (operands[5])))
    DONE;
})

(define_insn "*pred_th_msbc<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_0_operand" " rJ"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (match_operand:<VM> 3 "register_operand"     " vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"  " rK")
	       (match_operand 5 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_insn "*pred_th_msbc<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"              "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "reg_or_0_operand" " rJ")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (match_operand:<VM> 3 "register_operand"          " vm")
	    (unspec:<VM>
	      [(match_operand 4 "vector_length_operand"       " rK")
	       (match_operand 5 "const_int_operand"           "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_VMSBC))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vxm\t%0,%1,%z2,%3"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "avl_type_idx") (const_int 5))])

(define_insn "@pred_th_madc<mode>_overflow"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr, &vr, &vr")
	(unspec:<VM>
	   [(plus:VI
	     (match_operand:VI 1 "register_operand"     "  %vr,  vr,  vr")
	     (match_operand:VI 2 "vector_arith_operand" "vrvi,  vr,  vi"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand" "  rK,  rK,  rK")
	       (match_operand 4 "const_int_operand"     "   i,   i,   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmadc.v%o2\t%0,%1,%v2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_insn "@pred_th_msbc<mode>_overflow"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI
	     (match_operand:VI 1 "register_operand"     "   vr")
	     (match_operand:VI 2 "register_operand"     "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand" "  rK")
	       (match_operand 4 "const_int_operand"     "   i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vv\t%0,%1,%2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_insn "@pred_th_madc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "reg_or_0_operand" " rJ"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  " rK")
	       (match_operand 4 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmadc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_insn "@pred_th_msbc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_QHS
	     (vec_duplicate:VI_QHS
	       (match_operand:<VEL> 2 "reg_or_0_operand" " rJ"))
	     (match_operand:VI_QHS 1 "register_operand"  "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  " rK")
	       (match_operand 4 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_expand "@pred_th_madc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand")
	       (match_operand 4 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
{
  if (riscv_vector::sew64_scalar_helper (
	operands,
	/* scalar op */&operands[2],
	/* vl */operands[3],
	<MODE>mode,
	riscv_vector::simm5_p (operands[2]),
	[] (rtx *operands, rtx boardcast_scalar) {
	  emit_insn (gen_pred_th_madc<mode>_overflow (operands[0], operands[1],
	       boardcast_scalar, operands[3], operands[4]));
        },
	(riscv_vector::avl_type) INTVAL (operands[4])))
    DONE;
})

(define_insn "*pred_th_madc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_0_operand" " rJ"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  " rK")
	       (match_operand 4 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmadc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_insn "*pred_th_madc<mode>_overflow_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"             "=&vr")
	(unspec:<VM>
	   [(plus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "reg_or_0_operand" " rJ")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"       " rK")
	       (match_operand 4 "const_int_operand"           "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmadc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_expand "@pred_th_msbc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_int_operand"))
	     (match_operand:VI_D 1 "register_operand"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand")
	       (match_operand 4 "const_int_operand")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
{
  if (riscv_vector::sew64_scalar_helper (
	operands,
	/* scalar op */&operands[2],
	/* vl */operands[3],
	<MODE>mode,
	false,
	[] (rtx *operands, rtx boardcast_scalar) {
	  emit_insn (gen_pred_th_msbc<mode>_overflow (operands[0], operands[1],
	       boardcast_scalar, operands[3], operands[4]));
        },
	(riscv_vector::avl_type) INTVAL (operands[4])))
    DONE;
})

(define_insn "*pred_th_msbc<mode>_overflow_scalar"
  [(set (match_operand:<VM> 0 "register_operand"         "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (match_operand:<VEL> 2 "reg_or_0_operand" " rJ"))
	     (match_operand:VI_D 1 "register_operand"    "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"  " rK")
	       (match_operand 4 "const_int_operand"      "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_insn "*pred_th_msbc<mode>_overflow_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"             "=&vr")
	(unspec:<VM>
	   [(minus:VI_D
	     (vec_duplicate:VI_D
	       (sign_extend:<VEL>
	         (match_operand:<VSUBEL> 2 "reg_or_0_operand" " rJ")))
	     (match_operand:VI_D 1 "register_operand"         "  vr"))
	    (unspec:<VM>
	      [(match_operand 3 "vector_length_operand"      " rK")
	       (match_operand 4 "const_int_operand"          "  i")
	       (reg:SI VL_REGNUM)
	       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)] UNSPEC_OVERFLOW))]
  "TARGET_XTHEADVECTOR"
  "vmsbc.vx\t%0,%1,%z2"
  [(set_attr "type" "vicalu")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "3")
   (set (attr "avl_type_idx") (const_int 4))])

(define_insn "*th_vsetvl<mode>"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P [(match_operand:P 1 "vector_csr_operand" "rK")
		   (match_operand 2 "const_int_operand" "i")
		   (match_operand 3 "const_int_operand" "i")
		   (match_operand 4 "const_int_operand" "i")
		   (match_operand 5 "const_int_operand" "i")] UNSPEC_VSETVL))
   (set (reg:SI VL_REGNUM)
	(unspec:SI [(match_dup 1)
		    (match_dup 2)
		    (match_dup 3)] UNSPEC_VSETVL))
   (set (reg:SI VTYPE_REGNUM)
	(unspec:SI [(match_dup 2)
		    (match_dup 3)
		    (match_dup 4)
		    (match_dup 5)] UNSPEC_VSETVL))]
  "TARGET_XTHEADVECTOR"
  "vsetvli\t%0,%1,e%2,%m3"
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "<MODE>")
   (set (attr "sew") (symbol_ref "INTVAL (operands[2])"))
   (set (attr "vlmul") (symbol_ref "INTVAL (operands[3])"))
   (set (attr "ta") (symbol_ref "INTVAL (operands[4])"))
   (set (attr "ma") (symbol_ref "INTVAL (operands[5])"))])

;; vsetvl zero,zero,vtype instruction.
;; This pattern has no side effects and does not set X0 register.
(define_insn "*th_vsetvl_vtype_change_only"
  [(set (reg:SI VTYPE_REGNUM)
	(unspec:SI
	  [(match_operand 0 "const_int_operand" "i")
	   (match_operand 1 "const_int_operand" "i")
	   (match_operand 2 "const_int_operand" "i")
	   (match_operand 3 "const_int_operand" "i")] UNSPEC_VSETVL))]
  "TARGET_XTHEADVECTOR"
  "vsetvli\tzero,zero,e%0,%m1"
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "SI")
   (set (attr "sew") (symbol_ref "INTVAL (operands[0])"))
   (set (attr "vlmul") (symbol_ref "INTVAL (operands[1])"))
   (set (attr "ta") (symbol_ref "INTVAL (operands[2])"))
   (set (attr "ma") (symbol_ref "INTVAL (operands[3])"))])

;; vsetvl zero,rs1,vtype instruction.
;; The reason we need this pattern since we should avoid setting X0 register
;; in vsetvl instruction pattern.
(define_insn "*th_vsetvl_discard_result<mode>"
  [(set (reg:SI VL_REGNUM)
	(unspec:SI [(match_operand:P 0 "vector_csr_operand" "rK")
		    (match_operand 1 "const_int_operand" "i")
		    (match_operand 2 "const_int_operand" "i")] UNSPEC_VSETVL))
   (set (reg:SI VTYPE_REGNUM)
	(unspec:SI [(match_dup 1)
		    (match_dup 2)
		    (match_operand 3 "const_int_operand" "i")
		    (match_operand 4 "const_int_operand" "i")] UNSPEC_VSETVL))]
  "TARGET_XTHEADVECTOR"
  "vsetvli\tzero,%0,e%1,%m2"
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "<MODE>")
   (set (attr "sew") (symbol_ref "INTVAL (operands[1])"))
   (set (attr "vlmul") (symbol_ref "INTVAL (operands[2])"))
   (set (attr "ta") (symbol_ref "INTVAL (operands[3])"))
   (set (attr "ma") (symbol_ref "INTVAL (operands[4])"))])

;; It's emit by vsetvl/vsetvlmax intrinsics with no side effects.
;; Since we have many optmization passes from "expand" to "reload_completed",
;; such pattern can allow us gain benefits of these optimizations.
(define_insn_and_split "@th_vsetvl<mode>_no_side_effects"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P [(match_operand:P 1 "vector_csr_operand" "rK")
		   (match_operand 2 "const_int_operand" "i")
		   (match_operand 3 "const_int_operand" "i")
		   (match_operand 4 "const_int_operand" "i")
		   (match_operand 5 "const_int_operand" "i")] UNSPEC_VSETVL))]
  "TARGET_XTHEADVECTOR"
  "#"
  "&& epilogue_completed"
  [(parallel
    [(set (match_dup 0)
	  (unspec:P [(match_dup 1) (match_dup 2) (match_dup 3)
		     (match_dup 4) (match_dup 5)] UNSPEC_VSETVL))
     (set (reg:SI VL_REGNUM)
	  (unspec:SI [(match_dup 1) (match_dup 2) (match_dup 3)] UNSPEC_VSETVL))
     (set (reg:SI VTYPE_REGNUM)
	  (unspec:SI [(match_dup 2) (match_dup 3) (match_dup 4)
		      (match_dup 5)] UNSPEC_VSETVL))])]
  ""
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "SI")])

(define_insn "*pred_th_cmp<mode>_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"              "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"        "   0")
	     (match_operand 5 "vector_length_operand"        "  rK")
	     (match_operand 6 "const_int_operand"            "   i")
	     (match_operand 7 "const_int_operand"            "   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "comparison_except_ltge_operator"
	     [(match_operand:V_VLSI 3 "register_operand"         "  vr")
	      (match_operand:V_VLSI 4 "vector_arith_operand"     "vrvi")])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.v%o4\t%0,%3,%v4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_cmp<mode>"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr,   &vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_ltge_operator"
	     [(match_operand:V_VLSI 4 "register_operand"          "   vr,   vr,   vr,   vr")
	      (match_operand:V_VLSI 5 "vector_arith_operand"      "   vr,   vr,   vi,   vi")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_cmp<mode>_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,   &vr,   &vr,   &vr,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "   0,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK,   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i,    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i,    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_ltge_operator"
	     [(match_operand:V_VLSI 4 "register_operand"          "   vr,    vr,   vr,    vr,    vr,   vr,    vr,   vr,   vr")
	      (match_operand:V_VLSI 5 "vector_arith_operand"      " vrvi, vrvi,    vr,    vr, vrvi,    vr,    vr, vrvi, vrvi")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,   vu,   vu,   vu,    vr,    vr,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_ltge<mode>_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"              "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"        "   0")
	     (match_operand 5 "vector_length_operand"        "  rK")
	     (match_operand 6 "const_int_operand"            "   i")
	     (match_operand 7 "const_int_operand"            "   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "ltge_operator"
	     [(match_operand:V_VLSI 3 "register_operand"         "  vr")
	      (match_operand:V_VLSI 4 "vector_neg_arith_operand" "vrvj")])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.v%o4\t%0,%3,%v4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_ltge<mode>"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr,   &vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "ltge_operator"
	     [(match_operand:V_VLSI 4 "register_operand"          "   vr,   vr,   vr,   vr")
	      (match_operand:V_VLSI 5 "vector_neg_arith_operand"  "   vr,   vr,   vj,   vj")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_ltge<mode>_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,   &vr,   &vr,   &vr,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "    0,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK,   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i,    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i,    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "ltge_operator"
	     [(match_operand:V_VLSI 4 "register_operand"          "   vr,    vr,   vr,    vr,    vr,   vr,    vr,   vr,   vr")
	      (match_operand:V_VLSI 5 "vector_neg_arith_operand"  " vrvj, vrvj,    vr,    vr, vrvj,    vr,    vr, vrvj, vrvj")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,   vu,   vu,   vu,    vr,    vr,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.v%o5\t%0,%4,%v5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_cmp<mode>_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"               "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"          "  0")
	     (match_operand 5 "vector_length_operand"          " rK")
	     (match_operand 6 "const_int_operand"              "  i")
	     (match_operand 7 "const_int_operand"              "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_QHS 3 "register_operand"       " vr")
	      (vec_duplicate:V_VLSI_QHS
	        (match_operand:<VEL> 4 "register_operand"      "  r"))])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.vx\t%0,%3,%4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_cmp<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_QHS 4 "register_operand"      "   vr,   vr")
	      (vec_duplicate:V_VLSI_QHS
	        (match_operand:<VEL> 5 "register_operand"     "    r,    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_cmp<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"             "=vm,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"   "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"      "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"          "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"          "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_QHS 4 "register_operand"   "   vr,    vr,    vr,   vr,   vr")
	      (vec_duplicate:V_VLSI_QHS
	        (match_operand:<VEL> 5 "register_operand"  "    r,    r,    r,    r,    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"     "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_eqne<mode>_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"           "  0")
	     (match_operand 5 "vector_length_operand"           " rK")
	     (match_operand 6 "const_int_operand"               "  i")
	     (match_operand 7 "const_int_operand"               "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "equality_operator"
	     [(vec_duplicate:V_VLSI_QHS
	        (match_operand:<VEL> 4 "register_operand"       "  r"))
	      (match_operand:V_VLSI_QHS 3 "register_operand"        " vr")])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.vx\t%0,%3,%4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_eqne<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSI_QHS
	        (match_operand:<VEL> 5 "register_operand"     "    r,    r"))
	      (match_operand:V_VLSI_QHS 4 "register_operand"      "   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_eqne<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSI_QHS
	        (match_operand:<VEL> 5 "register_operand"     "    r,    r,    r,    r,    r"))
	      (match_operand:V_VLSI_QHS 4 "register_operand"      "   vr,    vr,    vr,   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_cmp<mode>_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"           "  0")
	     (match_operand 5 "vector_length_operand"           " rK")
	     (match_operand 6 "const_int_operand"               "  i")
	     (match_operand 7 "const_int_operand"               "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_D 3 "register_operand"          " vr")
	      (vec_duplicate:V_VLSI_D
	        (match_operand:<VEL> 4 "register_operand"       "  r"))])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.vx\t%0,%3,%4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

(define_insn "*pred_th_eqne<mode>_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"           "  0")
	     (match_operand 5 "vector_length_operand"           " rK")
	     (match_operand 6 "const_int_operand"               "  i")
	     (match_operand 7 "const_int_operand"               "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "equality_operator"
	     [(vec_duplicate:V_VLSI_D
	        (match_operand:<VEL> 4 "register_operand"       "  r"))
	      (match_operand:V_VLSI_D 3 "register_operand"          " vr")])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.vx\t%0,%3,%4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_cmp<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_D 4 "register_operand"        "   vr,   vr")
	      (vec_duplicate:V_VLSI_D
	        (match_operand:<VEL> 5 "register_operand"     "    r,    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_cmp<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"             "=vm,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"   "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"      "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"          "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"          "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_D 4 "register_operand"     "   vr,    vr,    vr,   vr,   vr")
	      (vec_duplicate:V_VLSI_D
	        (match_operand:<VEL> 5 "register_operand"  "    r,    r,    r,    r,    r"))])
	  (match_operand:<VM> 2 "vector_merge_operand"     "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_eqne<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSI_D
	        (match_operand:<VEL> 5 "register_operand"     "    r,    r"))
	      (match_operand:V_VLSI_D 4 "register_operand"        "   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_eqne<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSI_D
	        (match_operand:<VEL> 5 "register_operand"     "    r,    r,    r,    r,    r"))
	      (match_operand:V_VLSI_D 4 "register_operand"        "   vr,    vr,    vr,   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_cmp<mode>_extended_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"               "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"          "  0")
	     (match_operand 5 "vector_length_operand"          " rK")
	     (match_operand 6 "const_int_operand"              "  i")
	     (match_operand 7 "const_int_operand"              "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_D 3 "register_operand"         " vr")
	      (vec_duplicate:V_VLSI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 4 "register_operand" "  r")))])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.vx\t%0,%3,%4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_cmp<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                 "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK,   rK")
	     (match_operand 7 "const_int_operand"              "    i,    i")
	     (match_operand 8 "const_int_operand"              "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_D 4 "register_operand"         "   vr,   vr")
	      (vec_duplicate:V_VLSI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r,    r")))])
	  (match_operand:<VM> 2 "vector_merge_operand"         "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_cmp<mode>_extended_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                 "=vm,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"              "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"              "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "comparison_except_eqge_operator"
	     [(match_operand:V_VLSI_D 4 "register_operand"         "   vr,    vr,    vr,   vr,   vr")
	      (vec_duplicate:V_VLSI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r,    r,    r,    r,    r")))])
	  (match_operand:<VM> 2 "vector_merge_operand"         "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_eqne<mode>_extended_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"                 "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"            "  0")
	     (match_operand 5 "vector_length_operand"            " rK")
	     (match_operand 6 "const_int_operand"                "  i")
	     (match_operand 7 "const_int_operand"                "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "equality_operator"
	     [(vec_duplicate:V_VLSI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 4 "register_operand"   "  r")))
	      (match_operand:V_VLSI_D 3 "register_operand"           " vr")])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vms%B2.vx\t%0,%3,%4,v0.t"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_eqne<mode>_extended_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                 "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK,   rK")
	     (match_operand 7 "const_int_operand"              "    i,    i")
	     (match_operand 8 "const_int_operand"              "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r,    r")))
	      (match_operand:V_VLSI_D 4 "register_operand"         "   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"         "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_eqne<mode>_extended_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"          "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"              "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"              "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSI_D
	        (sign_extend:<VEL>
	          (match_operand:<VSUBEL> 5 "register_operand" "    r,    r,    r,    r,    r")))
	      (match_operand:V_VLSI_D 4 "register_operand"         "   vr,    vr,    vr,   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"         "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vms%B3.vx\t%0,%4,%5%p1"
  [(set_attr "type" "vicmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_cmp<mode>"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "signed_order_operator"
	     [(match_operand:V_VLSF 4 "register_operand"      "   vr,   vr")
	      (match_operand:V_VLSF 5 "register_operand"      "   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vmf%B3.vv\t%0,%4,%5%p1"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_cmp<mode>_narrow_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"               "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"          "  0")
	     (match_operand 5 "vector_length_operand"          " rK")
	     (match_operand 6 "const_int_operand"              "  i")
	     (match_operand 7 "const_int_operand"              "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "signed_order_operator"
	     [(match_operand:V_VLSF 3 "register_operand"           " vr")
	      (match_operand:V_VLSF 4 "register_operand"           " vr")])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vmf%B2.vv\t%0,%3,%4,v0.t"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_cmp<mode>_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,   &vr,   &vr,   &vr,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "    0,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK,   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i,    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i,    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "signed_order_operator"
	     [(match_operand:V_VLSF 4 "register_operand"      "   vr,    vr,   vr,    vr,    vr,   vr,    vr,   vr,   vr")
	      (match_operand:V_VLSF 5 "register_operand"      "   vr,   vr,    vr,    vr,   vr,    vr,    vr,   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,   vu,   vu,   vu,    vr,    vr,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vmf%B3.vv\t%0,%4,%5%p1"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_cmp<mode>_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"              "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"         "  0")
	     (match_operand 5 "vector_length_operand"         " rK")
	     (match_operand 6 "const_int_operand"             "  i")
	     (match_operand 7 "const_int_operand"             "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "signed_order_operator"
	     [(match_operand:V_VLSF 3 "register_operand"      " vr")
	      (vec_duplicate:V_VLSF
	        (match_operand:<VEL> 4 "register_operand"     "  f"))])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vmf%B2.vf\t%0,%3,%4,v0.t"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_cmp<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "signed_order_operator"
	     [(match_operand:V_VLSF 4 "register_operand"      "   vr,   vr")
	      (vec_duplicate:V_VLSF
	        (match_operand:<VEL> 5 "register_operand"     "    f,    f"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vmf%B3.vf\t%0,%4,%5%p1"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_cmp<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,  &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "signed_order_operator"
	     [(match_operand:V_VLSF 4 "register_operand"      "   vr,    vr,    vr,   vr,   vr")
	      (vec_duplicate:V_VLSF
	        (match_operand:<VEL> 5 "register_operand"     "    f,    f,    f,    f,    f"))])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vmf%B3.vf\t%0,%4,%5%p1"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "*pred_th_eqne<mode>_scalar_merge_tie_mask"
  [(set (match_operand:<VM> 0 "register_operand"              "=vm")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "register_operand"         "  0")
	     (match_operand 5 "vector_length_operand"         " rK")
	     (match_operand 6 "const_int_operand"             "  i")
	     (match_operand 7 "const_int_operand"             "  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 2 "equality_operator"
	     [(vec_duplicate:V_VLSF
	        (match_operand:<VEL> 4 "register_operand"     "  f"))
	      (match_operand:V_VLSF 3 "register_operand"      " vr")])
	  (match_dup 1)))]
  "TARGET_XTHEADVECTOR"
  "vmf%B2.vf\t%0,%3,%4,v0.t"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "1")
   (set_attr "vl_op_idx" "5")
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))])

;; We don't use early-clobber for LMUL <= 1 to get better codegen.
(define_insn "*pred_th_eqne<mode>_scalar"
  [(set (match_operand:<VM> 0 "register_operand"                "=&vr,   &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSF
	        (match_operand:<VEL> 5 "register_operand"     "    f,    f"))
	      (match_operand:V_VLSF 4 "register_operand"      "   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
  "vmf%B3.vf\t%0,%4,%5%p1"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")])

;; We use early-clobber for source LMUL > dest LMUL.
(define_insn "*pred_th_eqne<mode>_scalar_narrow"
  [(set (match_operand:<VM> 0 "register_operand"                "=vm,   &vr,   &vr,  &vr,  &vr")
	(if_then_else:<VM>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "    0,vmWc1,vmWc1,vmWc1,vmWc1")
	     (match_operand 6 "vector_length_operand"         "   rK,   rK,   rK,   rK,   rK")
	     (match_operand 7 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i,    i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operator:<VM> 3 "equality_operator"
	     [(vec_duplicate:V_VLSF
	        (match_operand:<VEL> 5 "register_operand"     "    f,    f,    f,    f,    f"))
	      (match_operand:V_VLSF 4 "register_operand"      "   vr,    vr,    vr,   vr,   vr")])
	  (match_operand:<VM> 2 "vector_merge_operand"        "   vu,   vu,    vr,   vu,    vr")))]
  "TARGET_XTHEADVECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vmf%B3.vf\t%0,%4,%5%p1"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")])