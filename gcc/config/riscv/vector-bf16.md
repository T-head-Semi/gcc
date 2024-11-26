(define_mode_iterator BFVMODE [
  VNx2BF VNx4BF VNx8BF VNx16BF VNx32BF VNx64BF])

(define_mode_iterator BFVMODE1 [
  VNx2BF VNx4BF VNx8BF VNx16BF VNx32BF])

(define_mode_attr BFVWMODE [
  (VNx2BF "VNx2SF") (VNx4BF "VNx4SF") (VNx8BF "VNx8SF")
  (VNx16BF "VNx16SF") (VNx32BF "VNx32SF")])

(define_mode_attr bfvwmode [
  (VNx2BF "vnx2sf") (VNx4BF "vnx4sf") (VNx8BF "vnx8sf")
  (VNx16BF "vnx16sf") (VNx32BF "vnx32sf")
])

(define_mode_attr BFVINTEQUIV [
  (VNx2BF "VNx2HI") (VNx4BF "VNx4HI") (VNx8BF "VNx8HI")
  (VNx16BF "VNx16HI") (VNx32BF "VNx32HI") (VNx64BF "VNx64HI")])

(define_mode_attr bfvintequiv [
  (VNx2BF "vnx2hi") (VNx4BF "vnx4hi") (VNx8BF "vnx8hi")
  (VNx16BF "vnx16hi") (VNx32BF "vnx32hi") (VNx64BF "vnx64hi")])

(define_mode_attr BFVSUBMODE [
  (VNx2BF "BF") (VNx4BF "BF") (VNx8BF "BF")
  (VNx16BF "BF") (VNx32BF "BF") (VNx64BF "BF")])

(define_mode_attr BFVLMODE [
  (VNx2BF "VNx2HI") (VNx4BF "VNx4HI") (VNx8BF "VNx8HI")
  (VNx16BF "VNx16HI") (VNx32BF "VNx32HI") (VNx64BF "VNx64HI")])

(define_mode_attr bfvlmode [
  (VNx2BF "vnx2hi") (VNx4BF "vnx4hi") (VNx8BF "vnx8hi")
  (VNx16BF "vnx16hi") (VNx32BF "vnx32hi") (VNx64BF "vnx64hi")])

(define_mode_attr BFVCMPEQUIV [
  (VNx2BF "VNx2BI") (VNx4BF "VNx4BI") (VNx8BF "VNx8BI")
  (VNx16BF "VNx16BI") (VNx32BF "VNx32BI") (VNx64BF "VNx64BI")])

(define_mode_attr bfxlmul [
  (VNx2BF "1") (VNx4BF "1") (VNx8BF "1")
  (VNx16BF "2") (VNx32BF "4") (VNx64BF "8")])

(define_mode_attr bfsew [
  (VNx2BF "16") (VNx4BF "16") (VNx8BF "16")
  (VNx16BF "16") (VNx32BF "16") (VNx64BF "16")])

(define_insn "vec_duplicate<mode>_nosetvl"
  [(set (match_operand:BFVMODE 0 "register_operand" "=vr,*vr")
	(unspec:BFVMODE
	  [(vec_duplicate:BFVMODE
	     (match_operand:<BFVSUBMODE> 1 "register_operand" "f,r"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "@
   vfmv.v.f\t%0,%1
   vmv.v.x\t%0,%1"
  [(set_attr "type" "vmove")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

;; mov 
(define_expand "mov<mode>"
  [(set (match_operand:BFVMODE 0 "reg_or_mem_operand")
		(match_operand:BFVMODE 1 "vector_move_operand"))]
  "TARGET_VECTOR"
{
  /* Need to force register if mem <- !reg.  */
  if (MEM_P (operands[0]) && !REG_P (operands[1]))
    operands[1] = force_reg (<MODE>mode, operands[1]);

  rtx dup_value;

  /* If we have a const vector, then we have to load it's value into the
     scratch reg, and then create a vec_duplicate of it.  */
  if (const_vec_duplicate_p (operands[1], &dup_value))
    {
      gcc_assert (can_create_pseudo_p ());
      rtx tmp_reg = gen_reg_rtx (<BFVSUBMODE>mode);
      emit_move_insn (tmp_reg, dup_value);
      if (TARGET_64BIT)
	emit_insn (gen_vsetvlmax<bfvlmode>_di (gen_reg_rtx (Pmode)));
      else
	emit_insn (gen_vsetvlmax<bfvlmode>_si (gen_reg_rtx (Pmode)));
      emit_insn (gen_vec_duplicate<mode>_nosetvl (operands[0], tmp_reg));
      DONE;
    }
})

(define_insn "*mov<mode>"
  [(set (match_operand:BFVMODE 0 "reg_or_mem_operand"  "=vr,vr,m")
	    (match_operand:BFVMODE 1 "reg_or_mem_operand"  "vr,m,vr"))]
  "TARGET_VECTOR"
  "@
   vmv<bfxlmul>r.v\t%0,%1
   vl<bfxlmul>r.v\t%0,%1
   vs<bfxlmul>r.v\t%1,%0"
  [(set_attr "type" "vmove,vload,vstore")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

;; BF vundefined
(define_expand "vundefined_<mode>"
  [(clobber (match_operand:BFVMODE 0 "register_operand"))]
  "TARGET_VECTOR"
{
})

;; BF load
(define_expand "vle<BFVMODE:mode>"
  [(parallel [(set (match_operand:BFVMODE 0 "register_operand")
		   (unspec:BFVMODE
		     [(mem:BFVMODE (match_operand 1 "register_operand"))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR"
{
})

(define_insn "*vle<mode>_nosetvl"
  [(set (match_operand:BFVMODE 0 "register_operand" "=vr")
	(unspec:BFVMODE
	  [(match_operand:BFVMODE 1 "memory_operand" "m")
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR"
  { return target_subset_version_p ("v", 0, 7) ? "vle.v\t%0,%1" : "vle<bfsew>.v\t%0,%1"; }
  [(set_attr "type" "vload")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

(define_expand "vle<BFVMODE:mode>_mask"
  [(parallel [(set (match_operand:BFVMODE 0 "register_operand")
		   (unspec:BFVMODE
		     [(if_then_else:BFVMODE
			(match_operand:<BFVCMPEQUIV> 1 "register_operand")
			(mem:BFVMODE
			  (match_operand 3 "register_operand"))
			(match_operand:BFVMODE 2 "register_operand"))
		       (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR"
{
})

(define_insn "*vle<BFVMODE:mode>_mask_nosetvl"
  [(set (match_operand:BFVMODE 0 "register_operand" "=vr")
	(unspec:BFVMODE
	  [(if_then_else:BFVMODE
	     (match_operand:<BFVCMPEQUIV> 2 "register_operand" "vm")
	     (match_operand:BFVMODE 1 "memory_operand" "m")
	     (match_operand:BFVMODE 3 "register_operand" "0"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR"
  { return target_subset_version_p ("v", 0, 7) ? "vle.v\t%0,%1,%2.t" : "vle<bfsew>.v\t%0,%1,%2.t"; }
  [(set_attr "type" "vload")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVMODE:BFVSUBMODE>")])

;; BF store
(define_expand "vse<BFVMODE:mode>"
  [(parallel [(set (mem:BFVMODE (match_operand 1 "register_operand"))
		   (unspec:BFVMODE
		     [(match_operand:BFVMODE 0 "register_operand")
		      (mem:BFVMODE (match_dup 1))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR"
{
})

(define_insn "*vse<mode>_nosetvl"
  [(set (match_operand:BFVMODE 1 "memory_operand" "+m")
	(unspec:BFVMODE
	  [(match_operand:BFVMODE 0 "register_operand" "vr")
	   (match_dup 1)
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR"
  { return target_subset_version_p ("v", 0, 7) ? "vse.v\t%0,%1" : "vse<bfsew>.v\t%0,%1"; }
  [(set_attr "type" "vstore")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

(define_expand "vse<BFVMODE:mode>_mask"
  [(parallel [(set (mem:BFVMODE (match_operand 2 "register_operand"))
		   (unspec:BFVMODE
		     [(unspec:BFVMODE
			[(match_operand:<BFVCMPEQUIV> 0 "register_operand")
			 (match_operand:BFVMODE 1 "register_operand")
			 (mem:BFVMODE (match_dup 2))]
		       UNSPEC_MASKED_STORE)
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR"
{
})

(define_insn "*vse<BFVMODE:mode>_mask_nosetvl"
  [(set (match_operand:BFVMODE 0 "memory_operand" "+m")
	(unspec:BFVMODE
	  [(unspec:BFVMODE
	     [(match_operand:<BFVCMPEQUIV> 1 "register_operand" "vm")
	      (match_operand:BFVMODE 2 "register_operand" "vr")
	      (match_dup 0)]
	    UNSPEC_MASKED_STORE)
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR"
  { return target_subset_version_p ("v", 0, 7) ? "vse.v\t%2,%0,%1.t" : "vse<bfsew>.v\t%2,%0,%1.t"; }
  [(set_attr "type" "vstore")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

;; BF16 to FP32
(define_expand "extend<mode><bfvwmode>2"
  [(parallel [(set (match_operand:<BFVWMODE> 0 "register_operand")
		   (unspec:<BFVWMODE>
		     [(float_extend:<BFVWMODE>
			(match_operand:BFVMODE1 1 "register_operand"))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*extend<mode><bfvwmode>2_nosetvl"
  [(set (match_operand:<BFVWMODE> 0 "register_operand" "=&vr")
	(unspec:<BFVWMODE>
	  [(float_extend:<BFVWMODE>
	     (match_operand:BFVMODE1 1 "register_operand" "vr"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfwcvtbf16.f.f.v\t%0,%1"
  [(set_attr "type" "vfcvt")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

(define_expand "extend<mode><bfvwmode>2_mask"
  [(parallel [(set (match_operand:<BFVWMODE> 0 "register_operand")
		   (unspec:<BFVWMODE>
		     [(match_operand:<BFVCMPEQUIV> 1 "register_operand")
		      (float_extend:<BFVWMODE>
			  (match_operand:BFVMODE1 2 "register_operand"))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*extend<mode><bfvwmode>2_mask_nosetvl"
  [(set (match_operand:<BFVWMODE> 0 "register_operand" "=&vr")
	(unspec:<BFVWMODE>
	  [(match_operand:<BFVCMPEQUIV> 1 "register_operand" "vm")
	   (float_extend:<BFVWMODE>
	       (match_operand:BFVMODE1 2 "register_operand" "vr"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfwcvtbf16.f.f.v\t%0,%2,%1.t"
  [(set_attr "type" "vfcvt")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

;; FP32 to BF16
(define_expand "trunc<bfvwmode><mode>2"
  [(parallel [(set (match_operand:BFVMODE1 0 "register_operand")
		   (unspec:BFVMODE1
		     [(float_truncate:BFVMODE1
			(match_operand:<BFVWMODE> 1 "register_operand"))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*trunc<bfvwmode><mode>2_nosetvl"
  [(set (match_operand:BFVMODE1 0 "register_operand" "=&vr")
	(unspec:BFVMODE1
	  [(float_truncate:BFVMODE1
	     (match_operand:<BFVWMODE> 1 "register_operand" "vr"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfncvtbf16.f.f.w\t%0,%1"
  [(set_attr "type" "vfcvt")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

(define_expand "trunc<bfvwmode><mode>2_mask"
  [(parallel [(set (match_operand:BFVMODE1 0 "register_operand")
		   (unspec:BFVMODE1
		     [(match_operand:<BFVCMPEQUIV> 1 "register_operand")
		      (float_truncate:BFVMODE1
			  (match_operand:<BFVWMODE> 2 "register_operand"))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*trunc<bfvwmode><mode>2_mask_nosetvl"
  [(set (match_operand:BFVMODE1 0 "register_operand" "=&vr")
	(unspec:BFVMODE1
	  [(match_operand:<BFVCMPEQUIV> 1 "register_operand" "vm")
	     (float_truncate:BFVMODE1
	     (match_operand:<BFVWMODE> 2 "register_operand" "vr"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfncvtbf16.f.f.w\t%0,%2,%1.t"
  [(set_attr "type" "vfcvt")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

;; INT to BF16
(define_expand "reinterpret_<mode><bfvintequiv>"
  [(set (match_operand:BFVMODE 0 "register_operand")
	(subreg:BFVMODE (match_operand:<BFVINTEQUIV> 1 "register_operand") 0))]
  "TARGET_VECTOR"
{
  emit_insn (gen_mov<mode> (operands[0],
	     simplify_gen_subreg (<MODE>mode, operands[1],
				  <BFVINTEQUIV>mode, 0)));
  DONE;
})

;; BF16 to INT
(define_expand "reinterpret_<bfvintequiv><mode>"
  [(set (match_operand:<BFVINTEQUIV> 0 "register_operand")
	(subreg:<BFVINTEQUIV> (match_operand:BFVMODE 1 "register_operand") 0))]
  "TARGET_VECTOR"
{
  emit_insn (gen_mov<bfvintequiv> (operands[0],
	     simplify_gen_subreg (<BFVINTEQUIV>mode, operands[1],
				  <MODE>mode, 0)));
  DONE;
})

;; BF16 mul-add
(define_expand "vfwmacc<mode>"
  [(parallel [(set (match_operand:<BFVWMODE> 0 "register_operand")
		   (unspec:<BFVWMODE>
		     [(plus:<BFVWMODE>
			(mult:<BFVWMODE>
			  (float_extend:<BFVWMODE>
			    (match_operand:BFVMODE1 2 "register_operand"))
			  (float_extend:<BFVWMODE>
			    (match_operand:BFVMODE1 3 "register_operand")))
			 (match_operand:<BFVWMODE> 1 "register_operand"))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*vfwmacc<mode>_nosetvl"
  [(set (match_operand:<BFVWMODE> 0 "register_operand" "=&vr")
	(unspec:<BFVWMODE>
	  [(plus:<BFVWMODE>
	     (mult:<BFVWMODE>
	       (float_extend:<BFVWMODE>
		 (match_operand:BFVMODE1 1 "register_operand" "vr"))
	       (float_extend:<BFVWMODE>
		 (match_operand:BFVMODE1 2 "register_operand" "vr")))
	     (match_operand:<BFVWMODE> 3 "register_operand" "0"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfwmaccbf16.vv\t%0,%1,%2"
  [(set_attr "type" "vfwmadd")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

(define_expand "vfwmacc<mode>_scalar"
  [(parallel [(set (match_operand:<BFVWMODE> 0 "register_operand")
		   (unspec:<BFVWMODE>
		     [(plus:<BFVWMODE>
			(mult:<BFVWMODE>
			  (vec_duplicate:<BFVWMODE>
			    (float_extend:<BFVWMODE>
			      (match_operand:<BFVSUBMODE> 2 "register_operand")))
			  (float_extend:<BFVWMODE>
			    (match_operand:BFVMODE1 3 "register_operand")))
			(match_operand:<BFVWMODE> 1 "register_operand"))
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*vfwmacc<mode>_scalar_nosetvl"
  [(set (match_operand:<BFVWMODE> 0 "register_operand" "=&vr")
	(unspec:<BFVWMODE>
	  [(plus:<BFVWMODE>
	     (mult:<BFVWMODE>
	       (vec_duplicate:<BFVWMODE>
		 (float_extend:<BFVWMODE>
		   (match_operand:<BFVSUBMODE> 2 "register_operand" "f")))
	       (float_extend:<BFVWMODE>
		 (match_operand:BFVMODE1 1 "register_operand" "vr")))
	     (match_operand:<BFVWMODE> 3 "register_operand" "0"))
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfwmaccbf16.vf\t%0,%2,%1"
  [(set_attr "type" "vfwmadd")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

(define_expand "vfwmacc<mode>_mask"
  [(parallel [(set (match_operand:<BFVWMODE> 0 "register_operand")
		   (unspec:<BFVWMODE>
		     [(unspec:<BFVWMODE>
			[(match_operand:<BFVCMPEQUIV> 1 "register_operand")
			 (plus:<BFVWMODE>
			   (mult:<BFVWMODE>
			     (float_extend:<BFVWMODE>
			       (match_operand:BFVMODE1 3 "register_operand"))
			     (float_extend:<BFVWMODE>
			       (match_operand:BFVMODE1 4 "register_operand")))
			   (match_operand:<BFVWMODE> 2 "register_operand"))]
		       UNSPEC_MASK_VFWMACC)
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*vfwmacc<mode>_mask_nosetvl"
  [(set (match_operand:<BFVWMODE> 0 "register_operand" "=&vr")
	(unspec:<BFVWMODE>
	  [(unspec:<BFVWMODE>
	     [(match_operand:<BFVCMPEQUIV> 1 "register_operand" "vm")
	      (plus:<BFVWMODE>
		(mult:<BFVWMODE>
		  (float_extend:<BFVWMODE>
		    (match_operand:BFVMODE1 3 "register_operand" "vr"))
		  (float_extend:<BFVWMODE>
		    (match_operand:BFVMODE1 4 "register_operand" "vr")))
		(match_operand:<BFVWMODE> 2 "register_operand" "0"))]
	    UNSPEC_MASK_VFWMACC)
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfwmaccbf16.vv\t%0,%3,%4,%1.t"
  [(set_attr "type" "vfwmadd")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])

(define_expand "vfwmacc<mode>_scalar_mask"
  [(parallel [(set (match_operand:<BFVWMODE> 0 "register_operand")
		   (unspec:<BFVWMODE>
		     [(unspec:<BFVWMODE>
			[(match_operand:<BFVCMPEQUIV> 1 "register_operand")
			 (plus:<BFVWMODE>
			   (mult:<BFVWMODE>
			     (vec_duplicate:<BFVWMODE>
			       (float_extend:<BFVWMODE>
				 (match_operand:<BFVSUBMODE> 3 "register_operand")))
			       (float_extend:<BFVWMODE>
			    (match_operand:BFVMODE1 4 "register_operand")))
			(match_operand:<BFVWMODE> 2 "register_operand"))]
		       UNSPEC_MASK_VFWMACC)
		      (reg:SI VL_REGNUM)]
		    UNSPEC_USEVL))
	      (use (reg:<BFVLMODE> VTYPE_REGNUM))])]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
{
})

(define_insn "*vfwmacc<mode>_scalar_mask_nosetvl"
  [(set (match_operand:<BFVWMODE> 0 "register_operand" "=&vr")
	(unspec:<BFVWMODE>
	  [(unspec:<BFVWMODE>
	     [(match_operand:<BFVCMPEQUIV> 1 "register_operand" "vm")
	      (plus:<BFVWMODE>
		(mult:<BFVWMODE>
		  (vec_duplicate:<BFVWMODE>
		    (float_extend:<BFVWMODE>
		      (match_operand:<BFVSUBMODE> 4 "register_operand" "f")))
		    (float_extend:<BFVWMODE>
		      (match_operand:BFVMODE1 3 "register_operand" "vr")))
		(match_operand:<BFVWMODE> 2 "register_operand" "0"))]
	    UNSPEC_MASK_VFWMACC)
	   (reg:SI VL_REGNUM)]
	 UNSPEC_USEVL))
   (use (reg:<BFVLMODE> VTYPE_REGNUM))]
  "TARGET_VECTOR && TARGET_HARD_FLOAT"
  "vfwmaccbf16.vf\t%0,%4,%3,%1.t"
  [(set_attr "type" "vfwmadd")
   (set_attr "mode" "none")
   (set_attr "emode" "<BFVSUBMODE>")])
