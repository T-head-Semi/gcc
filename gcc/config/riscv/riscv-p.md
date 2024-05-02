(define_mode_iterator DSP_ALL [
    V4QI (V8QI "TARGET_64BIT")
    V2HI (V4HI "TARGET_64BIT")
    (V2SI "TARGET_64BIT")
])

(define_mode_iterator DSP_ALLX [
    (V4QI "!TARGET_64BIT") (V8QI "TARGET_64BIT")
    (V2HI "!TARGET_64BIT") (V4HI "TARGET_64BIT")
    (V2SI "TARGET_64BIT")
])

(define_mode_iterator DSP_QI [V4QI (V8QI "TARGET_64BIT")])

(define_mode_iterator DSP_HI [V2HI (V4HI "TARGET_64BIT")])

(define_mode_iterator DSP_QIHI [
    V4QI (V8QI "TARGET_64BIT")
    V2HI (V4HI "TARGET_64BIT")
])

(define_mode_iterator DSP_SI [SI (V2SI "TARGET_64BIT")])

(define_mode_iterator DSP_QIHISI [
    V4QI (V8QI "TARGET_64BIT")
    V2HI (V4HI "TARGET_64BIT")
    SI (V2SI "TARGET_64BIT")
])

(define_mode_iterator DSP_QIHISIX [
    (V4QI "!TARGET_64BIT") (V8QI "TARGET_64BIT")
    (V2HI "!TARGET_64BIT") (V4HI "TARGET_64BIT")
    (SI "!TARGET_64BIT") (V2SI "TARGET_64BIT")
])

(define_mode_iterator DSP_V2E [V2HI (V2SI "TARGET_64BIT")])

(define_mode_iterator DSP_V2EX [(V2HI "!TARGET_64BIT") (V2SI "TARGET_64BIT")])

(define_mode_attr DSP_WIDEN [
    (V4QI "V4HI") (V2HI "V2SI")
    (V8QI "V8HI") (V4HI "V4SI")
    (V2SI "V2DI")
    (SI "DI") (DI "TI")]
)

(define_mode_attr DSP_EMODE [
    (V4QI "QI") (V2HI "HI")
    (V8QI "QI") (V4HI "HI")
    (V2SI "SI")
    (SI "SI") (DI "DI")]
)

(define_mode_attr DSP_EMODE_WIDEN [
    (V4QI "HI") (V2HI "SI")
    (V8QI "HI") (V4HI "SI")
    (V2SI "DI")
    (SI "DI") (DI "TI")]
)

(define_mode_attr dsp_ebits [
    (V8QI "8") (V4QI "8") (QI "8")
    (V4HI "16") (V2HI "16") (HI "16")
    (V2SI "32") (SI "32")
    (DI "64")]
)

(define_mode_attr DSP_WIDEN_SUM
  [(V2HI "SI") (V4HI "V2SI") (V4QI "V2HI") (V8QI "V4HI")])

(define_mode_attr DSP_NARROW_BISECTDS
  [(SI "V2HI") (V2SI "V4HI")])

(define_mode_attr DSP_NARROW_QUARTER
  [(SI "V4QI") (V2SI "V8QI")])

(define_mode_attr dsp_all_mode [
    (V4QI "_v4qi") (V8QI "_v8qi")
    (V2HI "_v2hi") (V4HI "_v4hi")
    (V2SI " ")
])

(define_code_attr zs
  [(sign_extend "s") (zero_extend "z")])

(define_c_enum "unspec" [
  ;; Rounding shift
  UNSPEC_SRA_U
  UNSPEC_SRL_U
  UNSPEC_SCLIP
  UNSPEC_UCLIP
  UNSPEC_CLO
  UNSPEC_SWAP
  UNSPEC_BITREV
])

(define_c_enum "unspecv" [
  ;; Read vxsat.OV flag
  UNSPECV_RDOV

  ;; Clear vxsat.OV flag
  UNSPECV_CLROV
])


;;======================= mov<DSP_ALL> pattern ================================

(define_expand "mov<mode>"
  [(set (match_operand:DSP_ALL 0 "nonimmediate_operand")
	(match_operand:DSP_ALL 1 "general_operand"))]
  "TARGET_XTHEAD_ZPN || TARGET_XTHEAD_ZPSFOPERAND"
  {
    if (can_create_pseudo_p ())
      {
	if (!REG_P (operands[0]))
	  operands[1] = force_reg (<MODE>mode, operands[1]);
      }
  }
)

(define_insn "*mov<mode>"
  [(set (match_operand:DSP_ALL 0 "nonimmediate_operand" "=r,m,r")
	(match_operand:DSP_ALL 1 "general_operand" " r,r,m"))]
  "TARGET_XTHEAD_ZPN || TARGET_XTHEAD_ZPSFOPERAND"
  {
    return riscv_output_move (operands[0], operands[1]);
  }
)

;; Auto-Vectorization

;; Public SPN
(define_code_iterator dsp_arith_op_code [
   plus minus
   ss_plus us_plus
   ss_minus us_minus
   smax smin
   umax umin
])

(define_code_iterator dsp_add_sub_code [
   plus minus
   ss_plus us_plus
   ss_minus us_minus
])

(define_code_iterator dsp_fixp_add_sub_code [
   ss_plus us_plus
   ss_minus us_minus
])

(define_code_iterator dsp_integer_add_sub_code [
   plus minus
])

(define_code_attr dsp_arith_inverse_op_code [
   (plus "minus") (minus "plus")
   (ss_plus "ss_minus") (us_plus "us_minus")
   (ss_minus "ss_plus") (us_minus "us_plus")
   (smax "smin") (smin "smax")
   (umax "umin") (umin "umax")
])

(define_code_attr dsp_arith_op_name [
    (plus "add") (minus "sub")
    (ss_plus "ssadd") (us_plus "usadd")
    (ss_minus "sssub") (us_minus "ussub")
    (smax "smax") (smin "smin")
    (umax "umax") (umin "umin")
])

(define_code_attr dsp_arith_intruction_name [
    (plus "add") (minus "sub")
    (ss_plus "kadd") (us_plus "ksub")
    (ss_minus "ukadd") (us_minus "uksub")
    (smax "smax") (smin "smin")
    (umax "umax") (umin "umin")
])

(define_expand "<dsp_arith_op_name><mode>3"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
	(dsp_arith_op_code:DSP_ALL
	  (match_operand:DSP_ALL 1 "register_operand" "r")
	  (match_operand:DSP_ALL 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  ""
)

(define_expand "<optab><mode>3"
  [(set (match_operand:DSP_ALL 0 "register_operand")
	(any_shift:DSP_ALL (match_operand:DSP_ALL 1 "register_operand")
			   (match_operand:SI 2 "nonmemory_operand")))]
  "TARGET_XTHEAD_ZPN"
  ""
)

(define_expand "<dsp_arith_op_name>hi3"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(dsp_fixp_add_sub_code:HI
	  (match_operand:HI 1 "register_operand" "r")
	  (match_operand:HI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  ""
)

(define_expand "<dsp_arith_op_name>di3"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(dsp_fixp_add_sub_code:DI
	  (match_operand:DI 1 "p_register_operand" "r")
	  (match_operand:DI 2 "p_register_operand" "r")))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  ""
)

(define_expand "ssashl<mode>3"
  [(set (match_operand:DSP_QIHISIX 0 "register_operand" "=r")
	(ss_ashift:DSP_QIHISIX
	  (match_operand:DSP_QIHISIX 1 "register_operand" "r")
	  (match_operand:SI 2 "nonmemory_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  ""
)

(define_expand "usashl<mode>3"
  [(set (match_operand:DSP_QIHISIX 0 "register_operand" "=r")
	(us_ashift:DSP_QIHISIX
	  (match_operand:DSP_QIHISIX 1 "register_operand" "r")
	  (match_operand:SI 2 "nonmemory_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  ""
)

(define_expand "avg<mode>3_floor"
  [(match_operand:DSP_ALL 0 "register_operand")
   (match_operand:DSP_ALL 1 "register_operand")
   (match_operand:DSP_ALL 2 "register_operand")]
  "TARGET_XTHEAD_ZPN"
  {
    emit_insn (gen_riscv_radd<dsp_ebits><dsp_all_mode> (
      operands[0], operands[1], operands[2]));
    DONE;
  }
)

(define_expand "uavg<mode>3_floor"
  [(match_operand:DSP_ALL 0 "register_operand")
   (match_operand:DSP_ALL 1 "register_operand")
   (match_operand:DSP_ALL 2 "register_operand")]
  "TARGET_XTHEAD_ZPN"
  {
    emit_insn (gen_riscv_uradd<dsp_ebits><dsp_all_mode> (
      operands[0], operands[1], operands[2]));
    DONE;
  }
)

(define_expand "clrsb<mode>2"
  [(set (match_operand:DSP_QIHISI 0 "register_operand" "=r")
	(clrsb:DSP_QIHISI
	  (match_operand:DSP_QIHISI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  ""
)

(define_expand "clzv2si2"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(clz:V2SI (match_operand:V2SI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  ""
)

(define_insn "bswaphi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(bswap:HI (match_operand:HI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "swap8\t%0,%1"
)

(define_expand "smulv2si3_highpart"
  [(match_operand:V2SI 0 "register_operand")
   (match_operand:V2SI 1 "register_operand")
   (match_operand:V2SI 2 "register_operand")]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  {
    emit_insn (gen_riscv_smmul_v2si (operands[0], operands[1], operands[2]));
    DONE;
  }
)

(define_expand "umaddsidi4"
  [(match_operand:DI 0 "p_register_operand")
   (match_operand:SI 1 "register_operand")
   (match_operand:SI 2 "register_operand")
   (match_operand:DI 3 "p_register_operand")]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  {
    emit_insn (gen_riscv_umar64_si (operands[0], operands[3], operands[1],
    				    operands[2]));
    DONE;
  }
)

(define_expand "msubsidi4"
  [(match_operand:DI 0 "p_register_operand")
   (match_operand:SI 1 "register_operand")
   (match_operand:SI 2 "register_operand")
   (match_operand:DI 3 "p_register_operand")]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  {
    emit_insn (gen_riscv_smsr64_si (operands[0], operands[3], operands[1],
    				    operands[2]));
    DONE;
  }
)

(define_expand "umsubsidi4"
  [(match_operand:DI 0 "p_register_operand")
   (match_operand:SI 1 "register_operand")
   (match_operand:SI 2 "register_operand")
   (match_operand:DI 3 "p_register_operand")]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  {
    emit_insn (gen_riscv_umsr64_si (operands[0], operands[3], operands[1],
    				    operands[2]));
    DONE;
  }
)

(define_expand "vec_set<mode>"
  [(match_operand:DSP_V2E 0 "register_operand" "")
   (match_operand:<DSP_EMODE> 1 "p_register_operand" "")
   (match_operand:SI 2 "immediate_operand" "")]
  "TARGET_XTHEAD_ZPN"
  {
    HOST_WIDE_INT pos = INTVAL (operands[2]);
    if (pos > 1)
      gcc_unreachable ();
    HOST_WIDE_INT elem = (HOST_WIDE_INT) 1 << pos;
    emit_insn (gen_vec_set<mode>_internal (operands[0], operands[1],
					   operands[0], GEN_INT (elem)));
    DONE;
  }
)

(define_expand "vec_set<mode>"
  [(match_operand:DSP_QI 0 "register_operand" "")
   (match_operand:QI 1 "register_operand" "")
   (match_operand:SI 2 "immediate_operand" "")]
  "TARGET_XTHEAD_ZPN"
  {
    HOST_WIDE_INT pos = INTVAL (operands[2]);
    if (pos > GET_MODE_SIZE (E_<MODE>mode).to_constant () - 1)
      gcc_unreachable ();
    HOST_WIDE_INT elem = (HOST_WIDE_INT) 1 << pos;
    emit_insn (gen_vec_set<mode>_internal (operands[0], operands[1],
					   operands[0], GEN_INT (elem)));
    DONE;
  }
)

(define_expand "insv<mode>"
  [(set (zero_extract:GPR (match_operand:GPR 0 "register_operand" "")
			  (match_operand:GPR 1 "const_int_operand" "")
			  (match_operand:GPR 2 "insv<dsp_ebits>_operand" ""))
	(match_operand:GPR 3 "register_operand" ""))]
  "TARGET_XTHEAD_ZPN"
  {
    if (INTVAL (operands[1]) != 8)
      FAIL;
  }
)

;;##################### RISC-V P Extension Instruction ########################

;;=================== SIMD Data Processing Instructions =======================

;;---------------- 16-bit Addition & Subtraction Instructions -----------------
;;Already implemented in previous: None

;;Implement: add16, sub16, kadd16, ksub16, ukadd16, uksub16
;;Extra implement: add8, sub8, kadd8, ksub8, ukadd8, uksub8,
;;                 add32, sub32, kadd32, ksub32, ukadd32, uksub32
(define_insn "riscv_<dsp_arith_intruction_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
	(dsp_integer_add_sub_code:DSP_ALL
	  (match_operand:DSP_ALL 1 "register_operand" "r")
	  (match_operand:DSP_ALL 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name><dsp_ebits>\\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_intruction_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALLX 0 "register_operand" "=r")
	(dsp_fixp_add_sub_code:DSP_ALLX
	  (match_operand:DSP_ALLX 1 "register_operand" "r")
	  (match_operand:DSP_ALLX 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name><dsp_ebits>\\t%0,%1,%2"
)

;;Implement: radd16, rsub16
;;Extra implement: radd8, rsub8, radd32, rsub32
(define_insn "riscv_r<dsp_arith_intruction_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
	(truncate:DSP_ALL
	  (ashiftrt:<DSP_WIDEN>
	    (dsp_integer_add_sub_code:<DSP_WIDEN>
	      (sign_extend:<DSP_WIDEN>
		(match_operand:DSP_ALL 1 "register_operand" "r"))
	      (sign_extend:<DSP_WIDEN>
		(match_operand:DSP_ALL 2 "register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPN"
  "r<dsp_arith_intruction_name><dsp_ebits>\\t%0,%1,%2"
)

;;Implement: uradd16, ursub16
;;Extra implement: uradd8, ursub8, uradd32, ursub32
(define_insn "riscv_ur<dsp_arith_intruction_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
	(truncate:DSP_ALL
	  (ashiftrt:<DSP_WIDEN>
	    (dsp_integer_add_sub_code:<DSP_WIDEN>
	      (zero_extend:<DSP_WIDEN>
		(match_operand:DSP_ALL 1 "register_operand" "r"))
	      (zero_extend:<DSP_WIDEN>
		(match_operand:DSP_ALL 2 "register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPN"
  "ur<dsp_arith_intruction_name><dsp_ebits>\\t%0,%1,%2"
)

(define_code_attr dsp_arith_cross_op_name [
   (plus "cras") (minus "crsa")
   (ss_plus "kcras") (us_plus "ukcras")
   (ss_minus "kcrsa") (us_minus "ukcrsa")
])

;;Implement: cras16, crsa16, kcras16, ukcras16, kcrsa16, ukcrsa16
;;Extra implement: cras32, crsa32, kcras32, ukcras32, kcrsa32, ukcrsa32
(define_insn "riscv_<dsp_arith_cross_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (<dsp_arith_inverse_op_code>:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2E 1 "register_operand" "r")
		(parallel [(const_int 0)]))
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2E 2 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:DSP_V2E
	    (dsp_integer_add_sub_code:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_dup 1)
		(parallel [(const_int 1)]))
	      (vec_select:<DSP_EMODE>
		(match_dup 2)
		(parallel [(const_int 0)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_cross_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_cross_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2EX 0 "register_operand" "=r")
	(vec_merge:DSP_V2EX
	  (vec_duplicate:DSP_V2EX
	    (<dsp_arith_inverse_op_code>:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2EX 1 "register_operand" "r")
		(parallel [(const_int 0)]))
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2EX 2 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:DSP_V2EX
	    (dsp_fixp_add_sub_code:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_dup 1)
		(parallel [(const_int 1)]))
	      (vec_select:<DSP_EMODE>
		(match_dup 2)
		(parallel [(const_int 0)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_cross_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_cross_op_name>16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (<dsp_arith_inverse_op_code>:HI
	      (vec_select:HI (match_operand:V4HI 1 "register_operand" "r")
			     (parallel [(const_int 0)]))
	      (vec_select:HI (match_operand:V4HI 2 "register_operand" "r")
			     (parallel [(const_int 1)])))
	    (dsp_add_sub_code:HI
	      (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
	      (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (vec_concat:V2HI
	    (<dsp_arith_inverse_op_code>:HI
	      (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
	      (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (dsp_add_sub_code:HI
	      (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
	      (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<dsp_arith_cross_op_name>16\t%0,%1,%2"
)

(define_code_attr dsp_arith_straight_op_name [
   (plus "stas") (minus "stsa")
   (ss_plus "kstas") (us_plus "ukstas")
   (ss_minus "kstsa") (us_minus "ukstsa")
])

;;Implement: stas16, stsa16, kstas16, ukstas16, kstsa16, ukstsa16
;;Extra implement: stas32, stsa32, kstas32, ukstas32, kstsa32, ukstsa32
(define_insn "riscv_<dsp_arith_straight_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (<dsp_arith_inverse_op_code>:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2E 1 "register_operand" "r")
		(parallel [(const_int 0)]))
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2E 2 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:DSP_V2E
	    (dsp_integer_add_sub_code:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_dup 1)
		(parallel [(const_int 1)]))
	      (vec_select:<DSP_EMODE>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_straight_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_straight_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2EX 0 "register_operand" "=r")
	(vec_merge:DSP_V2EX
	  (vec_duplicate:DSP_V2EX
	    (<dsp_arith_inverse_op_code>:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2EX 1 "register_operand" "r")
		(parallel [(const_int 0)]))
	      (vec_select:<DSP_EMODE>
		(match_operand:DSP_V2EX 2 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:DSP_V2EX
	    (dsp_fixp_add_sub_code:<DSP_EMODE>
	      (vec_select:<DSP_EMODE>
		(match_dup 1)
		(parallel [(const_int 1)]))
	      (vec_select:<DSP_EMODE>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_straight_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_straight_op_name>16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (<dsp_arith_inverse_op_code>:HI
	      (vec_select:HI (match_operand:V4HI 1 "register_operand" "r")
			     (parallel [(const_int 0)]))
	      (vec_select:HI (match_operand:V4HI 2 "register_operand" "r")
			     (parallel [(const_int 0)])))
	    (dsp_add_sub_code:HI
	      (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
	      (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	  (vec_concat:V2HI
	    (<dsp_arith_inverse_op_code>:HI
	      (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
	      (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	    (dsp_add_sub_code:HI
	      (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
	      (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<dsp_arith_straight_op_name>16\t%0,%1,%2"
)

;;Implement: rcras16, rcrsa16
;;Extra implement: rcras32, rcrsa32
(define_insn "riscv_r<dsp_arith_cross_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (ashiftrt:<DSP_EMODE_WIDEN>
		(<dsp_arith_inverse_op_code>:<DSP_EMODE_WIDEN>
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
	      (const_int 1))))
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (ashiftrt:<DSP_EMODE_WIDEN>
		(dsp_integer_add_sub_code:<DSP_EMODE_WIDEN>
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 2)
		      (parallel [(const_int 0)]))))
	      (const_int 1))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "r<dsp_arith_cross_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_r<dsp_arith_cross_op_name>16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (sign_extend:SI
		    (vec_select:HI
		      (match_operand:V4HI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:SI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "r<dsp_arith_cross_op_name>16\t%0,%1,%2"
)

;;Implement: urcras16, urcrsa16
;;Extra implement: urcras32, urcrsa32
(define_insn "riscv_ur<dsp_arith_cross_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (lshiftrt:<DSP_EMODE_WIDEN>
		(<dsp_arith_inverse_op_code>:<DSP_EMODE_WIDEN>
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
	      (const_int 1))))
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (lshiftrt:<DSP_EMODE_WIDEN>
		(dsp_integer_add_sub_code:<DSP_EMODE_WIDEN>
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 2)
		      (parallel [(const_int 0)]))))
	      (const_int 1))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "ur<dsp_arith_cross_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_ur<dsp_arith_cross_op_name>16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (zero_extend:SI
		    (vec_select:HI
		      (match_operand:V4HI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (zero_extend:SI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (zero_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (zero_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (zero_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (zero_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (zero_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (zero_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "ur<dsp_arith_cross_op_name>16\t%0,%1,%2"
)

;;Implement: rstas16, rstsa16
;;Extra implement: rstas32, rstsa32
(define_insn "riscv_r<dsp_arith_straight_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (ashiftrt:<DSP_EMODE_WIDEN>
		(<dsp_arith_inverse_op_code>:<DSP_EMODE_WIDEN>
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (ashiftrt:<DSP_EMODE_WIDEN>
		(dsp_integer_add_sub_code:<DSP_EMODE_WIDEN>
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 2)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "r<dsp_arith_straight_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_r<dsp_arith_straight_op_name>16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (sign_extend:SI
		    (vec_select:HI
		      (match_operand:V4HI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:SI
		    (vec_select:HI
		     (match_operand:V4HI 2 "register_operand" "r")
		     (parallel [(const_int 0)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "r<dsp_arith_straight_op_name>16\t%0,%1,%2"
)

;;Implement: urstas16, urstsa16
;;Extra implement: urstas32, urstsa32
(define_insn "riscv_ur<dsp_arith_straight_op_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (lshiftrt:<DSP_EMODE_WIDEN>
		(<dsp_arith_inverse_op_code>:<DSP_EMODE_WIDEN>
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_operand:DSP_V2E 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:DSP_V2E
	    (truncate:<DSP_EMODE>
	      (lshiftrt:<DSP_EMODE_WIDEN>
		(dsp_integer_add_sub_code:<DSP_EMODE_WIDEN>
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (zero_extend:<DSP_EMODE_WIDEN>
		    (vec_select:<DSP_EMODE>
		      (match_dup 2)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "ur<dsp_arith_straight_op_name><dsp_ebits>\t%0,%1,%2"
)

(define_insn "riscv_ur<dsp_arith_straight_op_name>16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (zero_extend:SI
		    (vec_select:HI
		      (match_operand:V4HI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (zero_extend:SI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (zero_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (zero_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(<dsp_arith_inverse_op_code>:SI
		  (zero_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (zero_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(dsp_integer_add_sub_code:SI
		  (zero_extend:SI
		    (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (zero_extend:SI
		    (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "ur<dsp_arith_straight_op_name>16\t%0,%1,%2"
)

;;---------------- 8-bit Addition & Subtraction Instructions ------------------
;;Already implemented in previous: add8, radd8, uradd8, kadd8, ukadd8,
;;                                 sub8, rsub8, ursub8, ksub8, uksub8

;;------------------------ 16-bit Shift Instructions --------------------------
;;Already implemented in previous: None

;;Implement: sll16, srl16, sra16
;;Extra implemented: sll8, srl8, sra8, sll32, srl32, sra32
(define_insn "riscv_<insn><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
     (any_shift:DSP_ALL
	(match_operand:DSP_ALL 1 "register_operand" "r")
	(match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "<insn><dsp_ebits>\\t%0,%1,%2"
)

;;Implement: slli16, srli16, srai16
;;Extra implemented: slli8, srli8, srai8, slli32, srli32, srai32
(define_insn "riscv_<insn>i<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
     (any_shift:DSP_ALL
	(match_operand:DSP_ALL 1 "register_operand" "r")
	(match_operand:SI 2 "immediate_operand" "i")))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & (<dsp_ebits> - 1));
    return "<insn>i<dsp_ebits>\\t%0,%1,%2";
  }
)

;;Implement: ksll16
;;Extra implemented: ksll8, ksll32
(define_insn "riscv_ksll<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALLX 0 "register_operand" "=r")
     (ss_ashift:DSP_ALLX
	(match_operand:DSP_ALLX 1 "register_operand" "r")
	(match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "ksll<dsp_ebits>\\t%0,%1,%2"
)

;;Implement: kslli16
;;Extra implemented: kslli8, kslli32
(define_insn "riscv_kslli<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALLX 0 "register_operand" "=r")
     (ss_ashift:DSP_ALLX
	(match_operand:DSP_ALLX 1 "register_operand" "r")
	(match_operand:SI 2 "immediate_operand" "i")))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0xf);
    return "kslli<dsp_ebits>\\t%0,%1,%2";
  }
)

;;Implement: kslra16
;;Extra implemented: kslra8, kslra32
(define_insn "riscv_kslra<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALLX 0 "register_operand" "=r")
	(if_then_else:DSP_ALLX
	  (lt:SI (match_operand:SI 2 "register_operand" "r")
		 (const_int 0))
	  (ashiftrt:DSP_ALLX (match_operand:DSP_ALLX 1 "register_operand" "r")
			     (neg:SI (match_dup 2)))
	  (ss_ashift:DSP_ALLX (match_dup 1)
			      (match_dup 2))))]
  "TARGET_XTHEAD_ZPN"
  "kslra<dsp_ebits>\t%0,%1,%2"
)

;;Implement: kslra16.u
;;Extra implemented: kslra8.u, kslra32.u
(define_insn "riscv_kslra<dsp_ebits>_u<dsp_all_mode>"
  [(set (match_operand:DSP_ALLX 0 "register_operand" "=r")
	(if_then_else:DSP_ALLX
	  (lt:SI (match_operand:SI 2 "register_operand" "r")
		 (const_int 0))
	  (unspec:DSP_ALLX [(match_operand:DSP_ALLX 1 "register_operand" "r")
			    (neg:SI (match_dup 2))] UNSPEC_SRA_U)
	  (ss_ashift:DSP_ALLX (match_dup 1)
			      (match_dup 2))))]
  "TARGET_XTHEAD_ZPN"
  "kslra<dsp_ebits>.u\t%0,%1,%2"
)

;;Implement: sra16.u
;;Extra implemented: sra8.u, sra32.u
(define_insn "riscv_sra<dsp_ebits>_u<dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
     (unspec:DSP_ALL
	[(match_operand:DSP_ALL 1 "register_operand" "r")
	 (match_operand:SI 2 "register_operand" "r")]
	 UNSPEC_SRA_U))]
  "TARGET_XTHEAD_ZPN"
  "sra<dsp_ebits>.u\\t%0,%1,%2"
)

;;Implement: srai16.u
;;Extra implemented: srai8.u, srai32.u
(define_insn "riscv_srai<dsp_ebits>_u<dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
     (unspec:DSP_ALL
	[(match_operand:DSP_ALL 1 "register_operand" "r")
	 (match_operand:SI 2 "immediate_operand" "i")]
	 UNSPEC_SRA_U))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & (<dsp_ebits> - 1));
    return "srai<dsp_ebits>.u\\t%0,%1,%2";
  }
)

;;Implement: srl16.u
;;Extra implemented: srl8.u, srl32.u
(define_insn "riscv_srl<dsp_ebits>_u<dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
     (unspec:DSP_ALL
	[(match_operand:DSP_ALL 1 "register_operand" "r")
	 (match_operand:SI 2 "register_operand" "r")]
	 UNSPEC_SRL_U))]
  "TARGET_XTHEAD_ZPN"
  "srl<dsp_ebits>.u\\t%0,%1,%2"
)

;;Implement: sril16.u
;;Extra implemented: sril8.u, sril32.u
(define_insn "riscv_srli<dsp_ebits>_u<dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
     (unspec:DSP_ALL
	[(match_operand:DSP_ALL 1 "register_operand" "r")
	 (match_operand:SI 2 "immediate_operand" "i")]
	 UNSPEC_SRL_U))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & (<dsp_ebits> - 1));
    return "srli<dsp_ebits>.u\\t%0,%1,%2";
  }
)

;;------------------------ 8-bit Shift Instructions ---------------------------
;;Already implemented in previous: sra8, sll8, srl8, srai8, slli8, srli8.u,
;;                                 sra8.u, srai8.u, srli8, srl8.u, ksll8,
;;                                 kslli8, kslra8, kslra8.u

;;----------------------- 16-bit Compare Instructions -------------------------
;;Already implemented in previous: None

(define_code_iterator dsp_cmp_code [
   le leu lt ltu eq
])

(define_code_attr dsp_cmp_name [
    (eq "cmpeq")
    (le "scmple")
    (lt "scmplt")
    (leu "ucmple")
    (ltu "ucmplt")
])

;;Implement: cmpeq16, scmplt16, scmpe16, ucmplt16, ucmple16
;;Extra implemented: cmpeq8, scmplt8, scmpe8, ucmplt8, ucmple8
(define_insn "riscv_<dsp_cmp_name><dsp_ebits>_<mode>"
  [(set (match_operand:DSP_QIHI 0 "register_operand" "=r")
	(dsp_cmp_code:DSP_QIHI
	  (match_operand:DSP_QIHI 1 "register_operand" "r")
	  (match_operand:DSP_QIHI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_cmp_name><dsp_ebits>\\t%0,%1,%2"
)

;;----------------------- 8-bit Compare Instructions --------------------------
;;Already implemented in previous: cmpeq8, scmplt8, scmpe8, ucmplt8, ucmple8

;;---------------------- 16-bit Multiply Instructions -------------------------
;;Already implemented in previous: None

;;Implement: smul16, smulx16, umul16, umulx16
(define_insn "riscv_<su>mul16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(mult:V2SI
	   (any_extend:V2SI (match_operand:V2HI 1 "register_operand" "r"))
	   (any_extend:V2SI (match_operand:V2HI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "<su>mul16\\t%0,%1,%2"
)

(define_insn "riscv_<su>mulx16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_merge:V2SI
	  (vec_duplicate:V2SI
	    (mult:SI
	      (any_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 1 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (any_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 1)])))))
	  (vec_duplicate:V2SI
	    (mult:SI
	      (any_extend:SI
		(vec_select:HI
		  (match_dup 1)
		  (parallel [(const_int 1)])))
	      (any_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "<su>mulx16\\t%0,%1,%2"
)

;;Implement: khm16
(define_insn "riscv_khm16_v2hi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_concat:V2HI
	  (truncate:HI
	    (ashiftrt:SI
	      (ss_mult:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:HI
		      (match_operand:V2HI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:SI
		    (vec_select:HI
		      (match_operand:V2HI 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 2))
	      (const_int 16)))
	  (truncate:HI
	    (ashiftrt:SI
	      (ss_mult:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1)
				   (parallel [(const_int 1)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2)
				   (parallel [(const_int 1)]))))
		(const_int 2))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "khm16\t%0,%1,%2"
)

(define_insn "riscv_khm16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(ss_mult:SI
		  (mult:SI
		    (sign_extend:SI
		      (vec_select:HI
			(match_operand:V4HI 1 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (sign_extend:SI
		      (vec_select:HI
			(match_operand:V4HI 2 "register_operand" "r")
			(parallel [(const_int 0)]))))
		  (const_int 2))
		(const_int 16)))
	    (truncate:HI
	      (ashiftrt:SI
		(ss_mult:SI
		  (mult:SI
		    (sign_extend:SI
		      (vec_select:HI (match_dup 1)
				     (parallel [(const_int 1)])))
		    (sign_extend:SI
		      (vec_select:HI (match_dup 2)
				     (parallel [(const_int 1)]))))
		(const_int 2))
	      (const_int 16))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(ss_mult:SI
		  (mult:SI
		    (sign_extend:SI
		      (vec_select:HI (match_dup 1)
				     (parallel [(const_int 2)])))
		    (sign_extend:SI
		      (vec_select:HI (match_dup 2)
				     (parallel [(const_int 2)]))))
		  (const_int 2))
		(const_int 16)))
	    (truncate:HI
	      (ashiftrt:SI
		(ss_mult:SI
		  (mult:SI
		    (sign_extend:SI
		      (vec_select:HI (match_dup 1)
				     (parallel [(const_int 3)])))
		    (sign_extend:SI
		      (vec_select:HI (match_dup 2)
				     (parallel [(const_int 3)]))))
		  (const_int 2))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "khm16\t%0,%1,%2"
)

;;Implement: khmx16
(define_insn "riscv_khmx16_v2hi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_concat:V2HI
	  (truncate:HI
	    (ashiftrt:SI
	      (ss_mult:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:HI
		      (match_operand:V2HI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:SI
		    (vec_select:HI
		      (match_operand:V2HI 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(const_int 2))
	      (const_int 16)))
	  (truncate:HI
	    (ashiftrt:SI
	      (ss_mult:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:HI (match_dup 1)
				   (parallel [(const_int 1)])))
		  (sign_extend:SI
		    (vec_select:HI (match_dup 2)
				   (parallel [(const_int 0)]))))
		(const_int 2))
	      (const_int 8)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "khmx16\t%0,%1,%2"
)

(define_insn "riscv_khmx16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(truncate:V4HI
	  (ashiftrt:V4SI
	    (ss_mult:V4SI
	      (mult:V4SI
		(sign_extend:V4SI
		  (match_operand:V4HI 1 "register_operand" "r"))
		(sign_extend:V4SI
		  (vec_select:V4HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 1)
			       (const_int 0)
			       (const_int 3)
			       (const_int 2)]))))
	      (const_int 2))
	  (const_int 8))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "khmx16\t%0,%1,%2"
)

;;---------------------- 8-bit Multiply Instructions --------------------------
;;Already implemented in previous: khm8, khmx8

;;Implement: smul8, smulx8, umul8, umulx8
(define_insn "riscv_<su>mul8"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(mult:V4HI
	  (any_extend:V4HI (match_operand:V4QI 1 "register_operand" "r"))
	  (any_extend:V4HI (match_operand:V4QI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "<su>mul8\\t%0,%1,%2"
)

(define_insn "riscv_<su>mulx8"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(mult:V4HI
	  (any_extend:V4HI (match_operand:V4QI 1 "register_operand" "r"))
	  (any_extend:V4HI
	    (vec_select:V4QI
	      (match_operand:V4QI 2 "register_operand" "r")
	      (parallel [(const_int 1)
			 (const_int 0)
			 (const_int 3)
			 (const_int 2)])))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "<su>mulx8\\t%0,%1,%2"
)

;;Implement: khm8
(define_insn "riscv_khm8_v4qi"
  [(set (match_operand:V4QI 0 "register_operand" "=r")
	(truncate:V4QI
	  (ashiftrt:V4HI
	    (ss_mult:V4HI
	      (mult:V4HI
		(sign_extend:V4HI
		  (match_operand:V4QI 1 "register_operand" "r"))
		(sign_extend:V4HI		
		  (match_operand:V4QI 2 "register_operand" "r")))
	      (const_int 2))
	  (const_int 8))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "khm8\t%0,%1,%2"
)

(define_insn "riscv_khm8_v8qi"
  [(set (match_operand:V8QI 0 "register_operand" "=r")
	(truncate:V8QI
	  (ashiftrt:V8HI
	    (ss_mult:V8HI
	      (mult:V8HI
		(sign_extend:V8HI
		  (match_operand:V8QI 1 "register_operand" "r"))
		(sign_extend:V8HI		
		  (match_operand:V8QI 2 "register_operand" "r")))
	      (const_int 2))
	  (const_int 8))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "khm8\t%0,%1,%2"
)

;;Implement: khmx8
(define_insn "riscv_khmx8_v4qi"
  [(set (match_operand:V4QI 0 "register_operand" "=r")
	(truncate:V4QI
	  (ashiftrt:V4HI
	    (ss_mult:V4HI
	      (mult:V4HI
		(sign_extend:V4HI
		  (match_operand:V4QI 1 "register_operand" "r"))
		(sign_extend:V4HI
		  (vec_select:V4QI		
		    (match_operand:V4QI 2 "register_operand" "r")
		    (parallel [(const_int 1)
			       (const_int 0)
			       (const_int 3)
			       (const_int 2)]))))
	      (const_int 2))
	  (const_int 8))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "khmx8\t%0,%1,%2"
)

(define_insn "riscv_khmx8_v8qi"
  [(set (match_operand:V8QI 0 "register_operand" "=r")
	(truncate:V8QI
	  (ashiftrt:V8HI
	    (ss_mult:V8HI
	      (mult:V8HI
		(sign_extend:V8HI
		  (match_operand:V8QI 1 "register_operand" "r"))
		(sign_extend:V8HI
		  (vec_select:V8QI		
		    (match_operand:V8QI 2 "register_operand" "r")
		    (parallel [(const_int 1)
			       (const_int 0)
			       (const_int 3)
			       (const_int 2)
			       (const_int 5)
			       (const_int 4)
			       (const_int 7)
			       (const_int 6)]))))
	      (const_int 2))
	  (const_int 8))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "khmx8\t%0,%1,%2"
)

;;------------------------ 16-bit Misc Instructions ---------------------------
;;Already implemented in previous: None

(define_code_iterator min_max [smin smax umin umax])

;;Implement: smin16, umin16, smax16, umax16
;;Extra implemented: smin8, umin8, smax8, umax8, smin32, umin32, smax32, umax32
(define_insn "riscv_<dsp_arith_intruction_name><dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALL 0 "register_operand" "=r")
	(min_max:DSP_ALL
	    (match_operand:DSP_ALL 1 "register_operand" "r")
	    (match_operand:DSP_ALL 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name><dsp_ebits>\\t%0,%1,%2"
)

;;Implement: sclip16
;;Extra implemented: sclip8, sclip32
(define_insn "riscv_sclip<dsp_ebits>_<mode>"
  [(set (match_operand:DSP_QIHISIX 0 "register_operand" "=r")
	(unspec:DSP_QIHISIX
	  [(match_operand:DSP_QIHISIX 1 "register_operand" "r")
	   (match_operand:SI 2 "immediate_operand" "i")]
	   UNSPEC_SCLIP))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & (<dsp_ebits> - 1));
    return "sclip<dsp_ebits>\\t%0,%1,%2";
  }
)

;;Implement: uclip16
;;Extra implemented: uclip8, uclip32
(define_insn "riscv_uclip<dsp_ebits>_<mode>"
  [(set (match_operand:DSP_QIHISIX 0 "register_operand" "=r")
	(unspec:DSP_QIHISIX
	  [(match_operand:DSP_QIHISIX 1 "register_operand" "r")
	   (match_operand:SI 2 "immediate_operand" "i")]
	   UNSPEC_UCLIP))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & (<dsp_ebits> - 1));
    return "uclip<dsp_ebits>\\t%0,%1,%2";
  }
)

;;Implement: kabs16
;;Extra implemented: kabs8, kabs32
(define_insn "riscv_kabs<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_ALLX 0 "register_operand" "=r")
	(ss_abs:DSP_ALLX (match_operand:DSP_ALLX 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "kabs<dsp_ebits>\\t%0,%1"
)

;;Implement: clrs16
;;Extra implemented: clrs8, clrs32
(define_insn "riscv_clrs<dsp_ebits>_<mode>"
  [(set (match_operand:DSP_QIHISI 0 "register_operand" "=r")
	(clrsb:DSP_QIHISI
	  (match_operand:DSP_QIHISI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "clrs<dsp_ebits>\\t%0,%1"
)

;;Implement: clo16
;;Extra implemented: clo8, clo32
(define_insn "riscv_clo<dsp_ebits>_<mode>"
  [(set (match_operand:DSP_QIHISI 0 "register_operand" "=r")
	(unspec:DSP_QIHISI
	  [(match_operand:DSP_QIHISI 1 "register_operand" "r")]
	UNSPEC_CLO))]
  "TARGET_XTHEAD_ZPN"
  "clo<dsp_ebits>\\t%0,%1"
)

;;Implement: clz16
;;Extra implemented: clz8, clz32
(define_insn "riscv_clz<dsp_ebits>_<mode>"
  [(set (match_operand:DSP_QIHISI 0 "register_operand" "=r")
	(clz:DSP_QIHISI (match_operand:DSP_QIHISI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "clz<dsp_ebits>\\t%0,%1"
)

;;Implement: swap16
;;Extra implemented: swap8
(define_insn "riscv_swap<dsp_ebits>_<mode>"
  [(set (match_operand:DSP_QIHI 0 "register_operand" "=r")
     (unspec:DSP_QIHI
      [(match_operand:DSP_QIHI 1 "register_operand" "r")]
       UNSPEC_SWAP))]
  "TARGET_XTHEAD_ZPN"
  "swap<dsp_ebits>\\t%0,%1"
)

;;------------------------ 8-bit Misc Instructions ----------------------------
;;Already implemented in previous: smin8, umin8, smax8, umax8, sclip8, uclip8
;;                                 kabs8, clrs8, clz8 clo8, swap8

;;--------------------- 8-bit Unpacking Instructions --------------------------
;;Already implemented in previous: None

;;Implement: sunpkd810, zunpkd810
(define_insn "riscv_<zs>unpkd810_v4qi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd810\t%0,%1"
)

(define_insn "*riscv_<zs>unpkd810_v4qi_inv"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd810\t%0,%1"
)

(define_insn "riscv_<zs>unpkd810_v8qi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 0) (const_int 1)
		     (const_int 4) (const_int 5)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<zs>unpkd810\t%0,%1"
)

;;Implement: sunpkd820, zunpkd820
(define_insn "riscv_<zs>unpkd820_v4qi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 2)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd820\t%0,%1"
)

(define_insn "*riscv_<zs>unpkd820_v4qi_inv"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 2)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd820\t%0,%1"
)

(define_insn "riscv_<zs>unpkd820_v8qi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 0) (const_int 2)
		     (const_int 4) (const_int 6)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<zs>unpkd820\t%0,%1"
)

;;Implement: sunpkd830, zunpkd830
(define_insn "riscv_<zs>unpkd830_v4qi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 3)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd830\t%0,%1"
)

(define_insn "*riscv_<zs>unpkd830_v4qi_inv"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 3)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd830\t%0,%1"
)

(define_insn "riscv_<zs>unpkd830_v8qi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 0) (const_int 3)
		     (const_int 4) (const_int 7)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<zs>unpkd830\t%0,%1"
)

;;Implement: sunpkd831, zunpkd831
(define_insn "riscv_<zs>unpkd831_v4qi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 3)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd831\t%0,%1"
)

(define_insn "*riscv_<zs>unpkd831_v4qi_inv"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 3)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd831\t%0,%1"
)

(define_insn "riscv_<zs>unpkd831_v8qi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 1) (const_int 3)
		     (const_int 5) (const_int 7)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<zs>unpkd831\t%0,%1"
)

;;Implement: sunpkd832, zunpkd832
(define_insn "riscv_<zs>unpkd832_v4qi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 3)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 2)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd832\t%0,%1"
)

(define_insn "*riscv_<zs>unpkd832_v4qi_inv"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand" "r")
		(parallel [(const_int 2)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 3)]))))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "<zs>unpkd832\t%0,%1"
)

(define_insn "riscv_<zs>unpkd832_v8qi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 2) (const_int 3)
		     (const_int 6) (const_int 7)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<zs>unpkd832\t%0,%1"
)


;;=============== Partial-SIMD Data Processing Instructions ===================

;;---------------------- 16-bit Packing Instructions --------------------------
;;Already implemented in previous: None

;;Implement: pkbb16
;;Extra implemented: pkbb32
(define_insn "riscv_pkbb<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "pkbb<dsp_ebits>\t%0,%1,%2"
)

(define_insn "*riscv_pkbb<dsp_ebits><dsp_all_mode>_inverse"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "pkbb<dsp_ebits>\t%0,%2,%1"
)

(define_insn "riscv_pkbb16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 4) (const_int 0)
		    (const_int 6) (const_int 2)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "pkbb16\t%0,%1,%2"
)

;;Implement: pkbt16
;;Extra implemented: pkbt32
(define_insn "riscv_pkbt<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "pkbt<dsp_ebits>\t%0,%1,%2"
)

(define_insn "*riscv_pkbt<dsp_ebits><dsp_all_mode>_inverse"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "pkbt<dsp_ebits>\t%0,%2,%1"
)

(define_insn "riscv_pkbt16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 5) (const_int 0)
		    (const_int 7) (const_int 2)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "pkbt16\t%0,%1,%2"
)

;;Implement: pktb16
;;Extra implemented: pktb32
(define_insn "riscv_pktb<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "pktb<dsp_ebits>\t%0,%1,%2"
)

(define_insn "*riscv_pktb<dsp_ebits><dsp_all_mode>_inverse"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "pktb<dsp_ebits>\t%0,%2,%1"
)

(define_insn "riscv_pktb16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 4) (const_int 1)
		    (const_int 6) (const_int 3)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "pktb16\t%0,%1,%2"
)

;;Implement: pktt16
;;Extra implemented: pktt32
(define_insn "riscv_pktt<dsp_ebits><dsp_all_mode>"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "pktt<dsp_ebits>\t%0,%1,%2"
)

(define_insn "*riscv_pktt<dsp_ebits><dsp_all_mode>_inverse"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 1 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (vec_duplicate:DSP_V2E
	    (vec_select:<DSP_EMODE>
	      (match_operand:DSP_V2E 2 "register_operand" "r")
	      (parallel [(const_int 1)])))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN"
  "pktt<dsp_ebits>\t%0,%2,%1"
)

(define_insn "riscv_pktt16_v4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 5) (const_int 1)
		    (const_int 7) (const_int 3)])))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "pktt16\t%0,%1,%2"
)

;;Other uses of pkxy
(define_insn "pkbbsi_1"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int 65535))
		(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pkbb16\t%0,%2,%1"
)

(define_insn "pkbbsi_2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI	(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))
		(and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int 65535))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pkbb16\t%0,%2,%1"
)

(define_insn "pkbbsi_3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (zero_extend:SI	(match_operand:HI 1 "register_operand" "r"))
		(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pkbb16\t%0,%2,%1"
)

(define_insn "pkbbsi_4"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI	(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))
		(zero_extend:SI (match_operand:HI 1 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pkbb16\t%0,%2,%1"
)

;; v0 = (v1 & 0xffff0000) | (v2 & 0xffff)
(define_insn "pktbsi_1"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int -65536))
		(zero_extend:SI (match_operand:HI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pktb16\t%0,%1,%2"
)

(define_insn "pktbsi_2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int -65536))
		(and:SI (match_operand:SI 2 "register_operand" "r")
			(const_int 65535))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pktb16\t%0,%1,%2"
)

(define_insn "pktbsi_3"
  [(set (zero_extract:SI (match_operand:SI 0 "register_operand" "+r")
			 (const_int 16 )
			 (const_int 0))
	(match_operand:SI 1 "register_operand" "r"))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pktb16\t%0,%0,%1"
)

(define_insn "pktbsi_4"
  [(set (zero_extract:SI (match_operand:SI 0 "register_operand" "+r")
			 (const_int 16 )
			 (const_int 0))
	(zero_extend:SI (match_operand:HI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pktb16\t%0,%0,%1"
)

(define_insn "pkttsi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int -65536))
		(lshiftrt:SI (match_operand:SI 2 "register_operand" "r")
			     (const_int 16))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pktt16\t%0,%1,%2"
)

(define_insn "vec_set<mode>_internal"
  [(set (match_operand:DSP_V2E 0 "register_operand" "=r")
	(vec_merge:DSP_V2E
	  (vec_duplicate:DSP_V2E
	    (match_operand:<DSP_EMODE> 1 "p_register_operand" "r"))
	  (match_operand:DSP_V2E 2 "register_operand" "r")
	  (match_operand:SI 3 "immediate_operand" "i")))]
  "TARGET_XTHEAD_ZPN"
  {
    HOST_WIDE_INT elem = INTVAL (operands[3]);
    if (elem == 1)
      return "pktb<dsp_ebits>\t%0,%2,%1";
    else if (elem == 2)
      return "pkbb<dsp_ebits>\t%0,%1,%2";
    else
      gcc_unreachable ();
  }
)

;;--------- Most Significant Word “32x32” Multiply & Add Instructions ---------
;;Already implemented in previous: None

;;Implement: smmul
(define_insn "riscv_smmul_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (mult:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	      (sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN"
  "smmul\t%0,%1,%2"
)

(define_insn "riscv_smmul_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (mult:V2DI
	      (sign_extend:V2DI (match_operand:V2SI 1 "register_operand" "r"))
	      (sign_extend:V2DI (match_operand:V2SI 2 "register_operand" "r")))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smmul\t%0,%1,%2"
)

;;Implement: smmul.u
(define_insn "riscv_smmul_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (plus:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	      (ashift: DI (const_int 1) (const_int 31)))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN"
  "smmul.u\t%0,%1,%2"
)

(define_insn "riscv_smmul_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (plus:V2DI
	      (mult:V2DI
		(sign_extend:V2DI
		  (match_operand:V2SI 1 "register_operand" "r"))
		(sign_extend:V2DI
		  (match_operand:V2SI 2 "register_operand" "r")))
	      (vec_duplicate:V2DI
		(ashift: DI (const_int 1) (const_int 31))))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smmul.u\t%0,%1,%2"
)

;;Implement: kmmac
(define_insn "riscv_kmmac_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI (match_operand:SI 1 "register_operand" " 0")
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		(sign_extend:DI (match_operand:SI 3 "register_operand" "r")))
	      (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmac\t%0,%2,%3"
)

(define_insn "riscv_kmmac_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI (match_operand:V2SI 1 "register_operand" " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (mult:V2DI
		(sign_extend:V2DI
		  (match_operand:V2SI 2 "register_operand" "r"))
		(sign_extend:V2DI
		  (match_operand:V2SI 3 "register_operand" "r")))
	      (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmac\t%0,%2,%3"
)

;;Implement: kmmac.u
(define_insn "riscv_kmmac_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI (match_operand:SI 1 "register_operand" " 0")
	  (truncate:SI
	    (lshiftrt:V2DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		  (sign_extend:DI (match_operand:SI 3 "register_operand" "r")))
		(ashift: DI (const_int 1) (const_int 31)))
	    (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmac.u\t%0,%2,%3"
)

(define_insn "riscv_kmmac_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI (match_operand:V2SI 1 "register_operand" " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (plus:V2DI
		(mult:V2DI
		  (sign_extend:V2DI
		    (match_operand:V2SI 2 "register_operand" "r"))
		  (sign_extend:V2DI
		    (match_operand:V2SI 3 "register_operand" "r")))
		(vec_duplicate:V2DI
		  (ashift: DI (const_int 1) (const_int 31))))
	      (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmac.u\t%0,%2,%3"
)

;;Implement: kmmsb
(define_insn "riscv_kmmsb_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_minus:SI (match_operand:SI 1 "register_operand" " 0")
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		(sign_extend:DI (match_operand:SI 3 "register_operand" "r")))
	      (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmsb\t%0,%2,%3"
)

(define_insn "riscv_kmmsb_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_minus:V2SI (match_operand:V2SI 1 "register_operand" " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (mult:V2DI
		(sign_extend:V2DI
		  (match_operand:V2SI 2 "register_operand" "r"))
		(sign_extend:V2DI
		  (match_operand:V2SI 3 "register_operand" "r")))
	      (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmsb\t%0,%2,%3"
)

;;Implement: kmmsb.u
(define_insn "riscv_kmmsb_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_minus:SI (match_operand:SI 1 "register_operand" " 0")
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		  (sign_extend:DI (match_operand:SI 3 "register_operand" "r")))
		(ashift: DI (const_int 1) (const_int 31)))
	      (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmsb.u\t%0,%2,%3"
)

(define_insn "riscv_kmmsb_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_minus:V2SI (match_operand:V2SI 1 "register_operand" " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (plus:V2DI
		(mult:V2DI
		  (sign_extend:V2DI
		    (match_operand:V2SI 2 "register_operand" "r"))
		  (sign_extend:V2DI
		    (match_operand:V2SI 3 "register_operand" "r")))
		(vec_duplicate:V2DI
		  (ashift: DI (const_int 1) (const_int 31))))
	      (const_int 32)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmsb.u\t%0,%2,%3"
)

;;Implement: kwmmul
(define_insn "riscv_kwmmul_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (ss_mult:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	      (const_int 2))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kwmmul\t%0,%1,%2"
)

(define_insn "riscv_kwmmul_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (ss_mult:V2DI
	      (mult:V2DI
		(sign_extend:V2DI
		  (match_operand:V2SI 1 "register_operand" "r"))
		(sign_extend:V2DI
		  (match_operand:V2SI 2 "register_operand" "r")))
	      (const_int 2))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kwmmul\t%0,%1,%2"
)

;;Implement: kwmmu.u
(define_insn "riscv_kwmmul_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (ss_plus:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		  (sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
		(const_int 2))
	      (ashift: DI (const_int 1) (const_int 31)))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kwmmul.u\t%0,%1,%2"
)

(define_insn "riscv_kwmmul_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (ss_plus:V2DI
	      (ss_mult:V2DI
		(mult:V2DI
		  (sign_extend:V2DI
		    (match_operand:V2SI 1 "register_operand" "r"))
		  (sign_extend:V2DI
		    (match_operand:V2SI 2 "register_operand" "r")))
		(const_int 2))
	      (vec_duplicate:V2DI
		(ashift: DI (const_int 1) (const_int 31))))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kwmmul.u\t%0,%1,%2"
)

;;--------- Most Significant Word “32x16” Multiply & Add Instructions ---------
;;Already implemented in previous: None

;;Implement: smmwb
(define_insn "riscv_smmwb_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (mult:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "smmwb\t%0,%1,%2"
)

(define_insn "riscv_smmwb_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_operand:V2SI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_dup 1)
		      (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_dup 2)
		      (parallel [(const_int 2)]))))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smmwb\t%0,%1,%2"
)

;;Implement: smmwt
(define_insn "riscv_smmwt_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (mult:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 1)]))))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "smmwt\t%0,%1,%2"
)

(define_insn "riscv_smmwt_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_operand:V2SI 1 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_dup 1)
		    (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 3)]))))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smmwt\t%0,%1,%2"
)

;;Implement: smmwb.u
(define_insn "riscv_smmwb_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (plus:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V2HI 2 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (ashift:DI (const_int 1) (const_int 15)))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "smmwb.u\t%0,%1,%2"
)

(define_insn "riscv_smmwb_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 2)
		      (parallel [(const_int 2)]))))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smmwb.u\t%0,%1,%2"
)

;;Implement: smmwt.u
(define_insn "riscv_smmwt_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (plus:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V2HI 2 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (ashift:DI (const_int 1) (const_int 15)))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "smmwt.u\t%0,%1,%2"
)

(define_insn "riscv_smmwt_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 2)
		      (parallel [(const_int 3)]))))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smmwt.u\t%0,%1,%2"
)

;;Implement: kmmawb
(define_insn "riscv_kmmawb_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V2HI 3 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawb\t%0,%2,%3"
)

(define_insn "riscv_kmmawb_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 2 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 3 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 2)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 3)
		      (parallel [(const_int 2)]))))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawb\t%0,%2,%3"
)

;;Implement: kmmawt
(define_insn "riscv_kmmawt_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V2HI 3 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawt\t%0,%2,%3"
)

(define_insn "riscv_kmmawt_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 2 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 3 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 2)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 3)
		      (parallel [(const_int 3)]))))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawt\t%0,%2,%3"
)

;;Implement: kmmawb.u
(define_insn "riscv_kmmawb_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V2HI 3 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawb.u\t%0,%2,%3"
)

(define_insn "riscv_kmmawb_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(plus:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_operand:V2SI 2 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V4HI 3 "register_operand" "r")
			(parallel [(const_int 0)]))))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(plus:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_dup 2)
			(parallel [(const_int 1)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_dup 3)
			(parallel [(const_int 2)]))))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawb.u\t%0,%2,%3"
)

;;Implement: kmmawt.u
(define_insn "riscv_kmmawt_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V2HI 3 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawt.u\t%0,%2,%3"
)

(define_insn "riscv_kmmawt_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(plus:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_operand:V2SI 2 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V4HI 3 "register_operand" "r")
			(parallel [(const_int 1)]))))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(plus:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_dup 2)
			(parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 3)
		      (parallel [(const_int 3)]))))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawt.u\t%0,%2,%3"
)

;;Implement: kmmwb2
(define_insn "riscv_kmmwb2_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (ss_mult:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V2HI 2 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (const_int 2))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmwb2\t%0,%1,%2"
)

(define_insn "riscv_kmmwb2_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 2))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 2)
		      (parallel [(const_int 2)]))))
		(const_int 2))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmwb2\t%0,%1,%2"
)

;;Implement: kmmwt2
(define_insn "riscv_kmmwt2_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (ss_mult:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V2HI 2 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (const_int 2))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmwt2\t%0,%1,%2"
)

(define_insn "riscv_kmmwt2_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 1 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(const_int 2))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 2)
		      (parallel [(const_int 3)]))))
		(const_int 2))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmwt2\t%0,%1,%2"
)

;;Implement: kmmwb2.u
(define_insn "riscv_kmmwb2_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (ss_plus:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V2HI 2 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 2))
	      (ashift: DI (const_int 1) (const_int 31)))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmwb2.u\t%0,%1,%2"
)

(define_insn "riscv_kmmwb2_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_plus:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_operand:V2SI 1 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V4HI 2 "register_operand" "r")
			(parallel [(const_int 0)]))))
		  (const_int 2))
		(ashift:DI (const_int 1) (const_int 15)))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_plus:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_dup 1)
			(parallel [(const_int 1)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_dup 2)
			(parallel [(const_int 2)]))))
		  (const_int 2))
		(ashift:DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmwb2.u\t%0,%1,%2"
)

;;Implement: kmmwt2.u
(define_insn "riscv_kmmwt2_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (ss_plus:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V2HI 2 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(const_int 2))
	      (ashift: DI (const_int 1) (const_int 31)))
	    (const_int 32))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmwt2.u\t%0,%1,%2"
)

(define_insn "riscv_kmmwt2_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_plus:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_operand:V2SI 1 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V4HI 2 "register_operand" "r")
			(parallel [(const_int 1)]))))
		  (const_int 2))
		(ashift:DI (const_int 1) (const_int 15)))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_plus:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_dup 1)
			(parallel [(const_int 1)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_dup 2)
			(parallel [(const_int 3)]))))
		  (const_int 2))
		(ashift:DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmwt2.u\t%0,%1,%2"
)

;;Implement: kmmawb2
(define_insn "riscv_kmmawb2_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V2HI 3 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(const_int 2))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawb2\t%0,%2,%3"
)

(define_insn "riscv_kmmawb2_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_operand:V2SI 2 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V4HI 3 "register_operand" "r")
			(parallel [(const_int 0)]))))
		    (const_int 2))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_dup 2)
			(parallel [(const_int 1)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_dup 3)
			(parallel [(const_int 2)]))))
		  (const_int 2))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawb2\t%0,%2,%3"
)

;;Implement: kmmawt2
(define_insn "riscv_kmmawt2_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_mult:DI
		(mult:DI
		  (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V2HI 3 "register_operand" "r")
		      (parallel [(const_int 1)]))))
		(const_int 2))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawt2\t%0,%2,%3"
)

(define_insn "riscv_kmmawt2_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_operand:V2SI 2 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V4HI 3 "register_operand" "r")
			(parallel [(const_int 1)]))))
		    (const_int 2))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (vec_select:SI
			(match_dup 2)
			(parallel [(const_int 1)])))
		    (sign_extend:DI
		      (vec_select:HI
			(match_dup 3)
			(parallel [(const_int 3)]))))
		  (const_int 2))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawt2\t%0,%2,%3"
)

;;Implement: kmmawb2.u
(define_insn "riscv_kmmawb2_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_plus:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (match_operand:SI 2 "register_operand" "r"))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V2HI 3 "register_operand" "r")
			(parallel [(const_int 0)]))))
		  (const_int 2))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawb2.u\t%0,%2,%3"
)

(define_insn "riscv_kmmawb2_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(ss_plus:DI
		  (ss_mult:DI
		    (mult:DI
		      (sign_extend:DI
			(vec_select:SI
			  (match_operand:V2SI 2 "register_operand" "r")
			  (parallel [(const_int 0)])))
		      (sign_extend:DI
			(vec_select:HI
			  (match_operand:V4HI 3 "register_operand" "r")
			  (parallel [(const_int 0)]))))
		    (const_int 2))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(ss_plus:DI
		  (ss_mult:DI
		    (mult:DI
		      (sign_extend:DI
			(vec_select:SI
			  (match_dup 2)
			  (parallel [(const_int 1)])))
		      (sign_extend:DI
			(vec_select:HI
			  (match_dup 3)
			  (parallel [(const_int 2)]))))
		    (const_int 2))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawb2.u\t%0,%2,%3"
)

;;Implement: kmmawt2.u
(define_insn "riscv_kmmawt2_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (truncate:SI
	    (lshiftrt:DI
	      (ss_plus:DI
		(ss_mult:DI
		  (mult:DI
		    (sign_extend:DI
		      (match_operand:SI 2 "register_operand" "r"))
		    (sign_extend:DI
		      (vec_select:HI
			(match_operand:V2HI 3 "register_operand" "r")
			(parallel [(const_int 1)]))))
		  (const_int 2))
		(ashift: DI (const_int 1) (const_int 15)))
	      (const_int 16)))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmmawt2.u\t%0,%2,%3"
)

(define_insn "riscv_kmmawt2_u_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (vec_concat:V2SI
	    (truncate:SI
	      (lshiftrt:DI
		(ss_plus:DI
		  (ss_mult:DI
		    (mult:DI
		      (sign_extend:DI
			(vec_select:SI
			  (match_operand:V2SI 2 "register_operand" "r")
			  (parallel [(const_int 0)])))
		      (sign_extend:DI
			(vec_select:HI
			  (match_operand:V4HI 3 "register_operand" "r")
			  (parallel [(const_int 1)]))))
		    (const_int 2))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16)))
	    (truncate:SI
	      (lshiftrt:DI
		(ss_plus:DI
		  (ss_mult:DI
		    (mult:DI
		      (sign_extend:DI
			(vec_select:SI
			  (match_dup 2)
			  (parallel [(const_int 1)])))
		      (sign_extend:DI
			(vec_select:HI
			  (match_dup 3)
			  (parallel [(const_int 3)]))))
		    (const_int 2))
		  (ashift: DI (const_int 1) (const_int 15)))
		(const_int 16))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmmawt2.u\t%0,%2,%3"
)

;;-------- Signed 16-bit Multiply with 32-bit Add/Subtract Instructions -------
;;Already implemented in previous: None

;;Implement: smbb16
(define_insn "riscv_smbb16_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mult:SI
	  (sign_extend:SI
	     (vec_select:HI
	       (match_operand:V2HI 1 "register_operand" "r")
	       (parallel [(const_int 0)])))
	  (sign_extend:SI (vec_select:HI
	       (match_operand:V2HI 2 "register_operand" "r")
	       (parallel [(const_int 0)])))))]
  "TARGET_XTHEAD_ZPN"
  "smbb16\t%0,%1,%2"
)

(define_insn "riscv_smbb16_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(mult:V2SI
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 1 "register_operand" "r")
	   (parallel [(const_int 0)
		      (const_int 2)])))
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 2 "register_operand" "r")
	   (parallel [(const_int 0)
		      (const_int 2)])))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smbb16\t%0,%1,%2"
)

;;Implement: smbt16
(define_insn "riscv_smbt16_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mult:SI
	  (sign_extend:SI
	     (vec_select:HI
	       (match_operand:V2HI 1 "register_operand" "r")
	       (parallel [(const_int 0)])))
	  (sign_extend:SI (vec_select:HI
	       (match_operand:V2HI 2 "register_operand" "r")
	       (parallel [(const_int 1)])))))]
  "TARGET_XTHEAD_ZPN"
  "smbt16\t%0,%1,%2"
)

(define_insn "*riscv_smtb16_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mult:SI
	  (sign_extend:SI
	     (vec_select:HI
	       (match_operand:V2HI 1 "register_operand" "r")
	       (parallel [(const_int 1)])))
	  (sign_extend:SI (vec_select:HI
	       (match_operand:V2HI 2 "register_operand" "r")
	       (parallel [(const_int 0)])))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "smbt16\t%0,%2,%1"
)

(define_insn "riscv_smbt16_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(mult:V2SI
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 1 "register_operand" "r")
	   (parallel [(const_int 0)
		      (const_int 2)])))
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 2 "register_operand" "r")
	   (parallel [(const_int 1)
		      (const_int 3)])))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smbt16\t%0,%1,%2"
)

;;Implement: smtt16
(define_insn "riscv_smtt16_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mult:SI
	  (sign_extend:SI
	     (vec_select:HI
	       (match_operand:V2HI 1 "register_operand" "r")
	       (parallel [(const_int 1)])))
	  (sign_extend:SI (vec_select:HI
	       (match_operand:V2HI 2 "register_operand" "r")
	       (parallel [(const_int 1)])))))]
  "TARGET_XTHEAD_ZPN"
  "smtt16\t%0,%1,%2"
)

(define_insn "riscv_smtt16_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(mult:V2SI
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 1 "register_operand" "r")
	   (parallel [(const_int 1)
		      (const_int 3)])))
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 2 "register_operand" "r")
	   (parallel [(const_int 1)
		      (const_int 3)])))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smtt16\t%0,%1,%2"
)

;;Implement: kmda
(define_insn "riscv_kmda_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(const_int 1)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmda\t%0,%1,%2"
)

(define_insn "riscv_kmda_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)]))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmda\t%0,%1,%2"
)

;;Implement: kmxda
(define_insn "riscv_kmxda_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmxda\t%0,%1,%2"
)

(define_insn "riscv_kmxda_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 1) (const_int 3)]))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmxda\t%0,%1,%2"
)

;;Implement: smds
(define_insn "riscv_smds_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(const_int 1)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_XTHEAD_ZPN"
  "smds\t%0,%1,%2"
)

(define_insn "riscv_smds_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(minus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)]))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smds\t%0,%1,%2"
)

;;Implement: smdrs
(define_insn "riscv_smdrs_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPN"
  "smdrs\t%0,%1,%2"
)

(define_insn "riscv_smdrs_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(minus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 1) (const_int 3)]))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smdrs\t%0,%1,%2"
)

;;Implement: smxds
(define_insn "riscv_smxds_v2hi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPN"
  "smxds\t%0,%1,%2"
)

(define_insn "riscv_smxds_v4hi"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(minus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 1) (const_int 3)]))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smxds\t%0,%1,%2"
)

;;Implement: kmabb
(define_insn "riscv_kmabb_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 0)])))
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmabb\t%0,%2,%3"
)

(define_insn "riscv_kmabb_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 2 "register_operand" "r")
		(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 3 "register_operand" "r")
		(parallel [(const_int 0) (const_int 2)]))))
	  (match_operand:V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmabb\t%0,%2,%3"
)

;;Implement: kmabt
(define_insn "riscv_kmabt_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 0)])))
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmabt\t%0,%2,%3"
)

(define_insn "*riscv_kmatb_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 1)])))
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmabt\t%0,%2,%3"
)

(define_insn "riscv_kmabt_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 2 "register_operand" "r")
		(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 3 "register_operand" "r")
		(parallel [(const_int 1) (const_int 3)]))))
	  (match_operand:V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmabt\t%0,%2,%3"
)

(define_insn "*riscv_kmatb_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 2 "register_operand" "r")
		(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 3 "register_operand" "r")
		(parallel [(const_int 0) (const_int 2)]))))
	  (match_operand:V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmabt\t%0,%2,%3"
)

;;Implement: kmatt
(define_insn "riscv_kmatt_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 1)])))
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmatt\t%0,%2,%3"
)

(define_insn "riscv_kmatt_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 2 "register_operand" "r")
		(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 3 "register_operand" "r")
		(parallel [(const_int 1) (const_int 3)]))))
	  (match_operand:V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmatt\t%0,%2,%3"
)

;;Implement: kmada
(define_insn "riscv_kmada_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (ss_plus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" "r")
				(parallel [(const_int 1)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmada\t%0,%2,%3"
)

(define_insn "riscv_kmada_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (ss_plus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)]))))
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 2)
		  (parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 3)
		  (parallel [(const_int 0) (const_int 2)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmada\t%0,%2,%3"
)

;;Implement: kmxada
(define_insn "riscv_kmaxda_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (ss_plus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" "r")
				(parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmaxda\t%0,%2,%3"
)

(define_insn "riscv_kmaxda_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (ss_plus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 2)
		  (parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 3)
		  (parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmaxda\t%0,%2,%3"
)

;;Implement: kmads
(define_insn "riscv_kmads_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 3 "register_operand" "r")
		  (parallel [(const_int 1)]))))
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 0)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmads\t%0,%2,%3"
)

(define_insn "riscv_kmads_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)]))))
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 2)
		  (parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 3)
		  (parallel [(const_int 0) (const_int 2)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmads\t%0,%2,%3"
)

;;Implement: kmadrs
(define_insn "riscv_kmadrs_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmadrs\t%0,%2,%3"
)

(define_insn "riscv_kmadrs_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 2)
		  (parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 3)
		  (parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmadrs\t%0,%2,%3"
)

;;Implement: kmaxds
(define_insn "riscv_kmaxds_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmaxds\t%0,%2,%3"
)

(define_insn "riscv_kmaxds_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 2)
		  (parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 3)
		  (parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmaxds\t%0,%2,%3"
)

;;Implement: kmsda
(define_insn "riscv_kmsda_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_minus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 3 "register_operand" "r")
		  (parallel [(const_int 1)]))))
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 0)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmsda\t%0,%2,%3"
)

(define_insn "riscv_kmsda_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_minus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)]))))
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 2)
		  (parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 3)
		  (parallel [(const_int 0) (const_int 2)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmsda\t%0,%2,%3"
)

;;Implement: kmsxda
(define_insn "riscv_kmsxda_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_minus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "kmsxda\t%0,%2,%3"
)

(define_insn "riscv_kmsxda_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_minus:V2SI
	  (match_operand:V2SI 1 "register_operand" "0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 2)
		  (parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI
		(vec_select:V2HI
		  (match_dup 3)
		  (parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kmsxda\t%0,%2,%3"
)

;;-------- Signed 16-bit Multiply with 64-bit Add/Subtract Instructions -------
;;Already implemented in previous: None

;;Implement: smal
(define_insn "riscv_smal_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI (match_operand:DI 1 "p_register_operand" "r")
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 0)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_dup 2)
		(parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smal\t%0,%1,%2"
)

(define_insn "riscv_smal_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI (match_operand:DI 1 "p_register_operand" "r")
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 2) (parallel [(const_int 3)])))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smal\t%0,%1,%2"
)

;;---------------------- Miscellaneous Instructions ---------------------------
;;Already implemented in previous: sclip32, uclip32, clrs32, clz32, clo32

;;Implement: pbsad
(define_insn "riscv_pbsad_v4qi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (plus:SI (plus:SI
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI
		  (match_operand:V4QI 1 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:QI
		  (match_operand:V4QI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))))
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI (match_dup 1) (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:QI (match_dup 2) (parallel [(const_int 1)]))))))
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI (match_dup 1) (parallel [(const_int 2)])))
	      (sign_extend:SI
		(vec_select:QI (match_dup 2) (parallel [(const_int 2)]))))))
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI (match_dup 1) (parallel [(const_int 3)])))
	      (sign_extend:SI
		(vec_select:QI (match_dup 2) (parallel [(const_int 3)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pbsad\\t%0,%1,%2"
)

(define_insn "riscv_pbsad_v8qi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI (plus:DI (plus:DI (plus:DI (plus:DI (plus:DI (plus:DI
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI
		  (match_operand:V8QI 1 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:QI
		  (match_operand:V8QI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 1) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 1)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 1) (parallel [(const_int 2)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 2)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 1) (parallel [(const_int 3)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 3)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 1) (parallel [(const_int 4)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 4)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 1) (parallel [(const_int 5)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 5)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 1) (parallel [(const_int 6)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 6)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 1) (parallel [(const_int 7)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 7)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "pbsad\\t%0,%1,%2"
)

;;Implement: pbsada
(define_insn "riscv_pbsada_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (plus:SI (plus:SI (plus:SI
	  (match_operand:SI 1 "register_operand" "0")
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI
		  (match_operand:V4QI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:QI
		  (match_operand:V4QI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))))
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:QI (match_dup 3) (parallel [(const_int 1)]))))))
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
	      (sign_extend:SI
		(vec_select:QI (match_dup 3) (parallel [(const_int 2)]))))))
	  (abs:SI
	    (minus:SI
	      (sign_extend:SI
		(vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
	      (sign_extend:SI
		(vec_select:QI (match_dup 3) (parallel [(const_int 3)])))))))]
  "TARGET_XTHEAD_ZPN && !TARGET_64BIT"
  "pbsada\\t%0,%2,%3"
)

(define_insn "riscv_pbsada_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI (plus:DI (plus:DI (plus:DI (plus:DI (plus:DI (plus:DI (plus:SI
	  (match_operand:DI 1 "p_register_operand" "0")
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI
		  (match_operand:V8QI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:QI
		  (match_operand:V8QI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 3) (parallel [(const_int 1)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 3) (parallel [(const_int 2)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 3) (parallel [(const_int 3)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 4)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 3) (parallel [(const_int 4)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 5)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 3) (parallel [(const_int 5)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 6)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 3) (parallel [(const_int 6)]))))))
	  (abs:DI
	    (minus:DI
	      (sign_extend:DI
		(vec_select:QI (match_dup 2) (parallel [(const_int 7)])))
	      (sign_extend:DI
		(vec_select:QI (match_dup 3) (parallel [(const_int 7)])))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "pbsada\\t%0,%2,%3"
)

;;---------------- 8-bit Multiply with 32-bit Add Instructions ----------------
;;Already implemented in previous: None

;;Implement: smaqa, umaqa
(define_insn "riscv_<su>maqa_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (match_operand:SI 1 "register_operand"    " 0")
	  (plus:SI
	    (plus:SI
	      (mult:SI
		(any_extend:SI
		  (vec_select:QI
		    (match_operand:V4QI 2 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(any_extend:SI
		  (vec_select:QI
		    (match_operand:V4QI 3 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (mult:SI
		(any_extend:SI
		  (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		(any_extend:SI
		  (vec_select:QI (match_dup 3) (parallel [(const_int 1)])))))
	  (plus:SI
	      (mult:SI
		(any_extend:SI
		  (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		(any_extend:SI
		  (vec_select:QI (match_dup 3) (parallel [(const_int 2)]))))
	      (mult:SI
		(any_extend:SI
		  (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		(any_extend:SI
		  (vec_select:QI (match_dup 3)
				 (parallel [(const_int 3)]))))))))]
  "TARGET_XTHEAD_ZPN"
  "<su>maqa\\t%0,%2,%3"
)

(define_insn "riscv_<su>maqa_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(plus:V2SI (match_operand:V2SI 1 "register_operand"  " 0")
	  (vec_concat:V2SI
	    (plus:SI
	      (plus:SI
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI
		      (match_operand:V8QI 2 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (any_extend:SI
		    (vec_select:QI
		      (match_operand:V8QI 3 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		  (any_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 1)])))))
	    (plus:SI
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		  (any_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 2)]))))
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		  (any_extend:SI
		    (vec_select:QI (match_dup 3)
				   (parallel [(const_int 3)]))))))
	    (plus:SI
	      (plus:SI
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 4)])))
		  (any_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 4)]))))
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 5)])))
		  (any_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 5)])))))
	    (plus:SI
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 6)])))
		  (any_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 6)]))))
		(mult:SI
		  (any_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 7)])))
		  (any_extend:SI
		    (vec_select:QI (match_dup 3)
				   (parallel [(const_int 7)])))))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<su>maqa\\t%0,%2,%3"
)

;;Implement: smaqa.su
(define_insn "riscv_smaqa_su_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (match_operand:SI 1 "register_operand"    " 0")
	  (plus:SI
	    (plus:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:QI
		    (match_operand:V4QI 2 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(zero_extend:SI
		  (vec_select:QI
		    (match_operand:V4QI 3 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (mult:SI
		(sign_extend:SI
		  (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		(zero_extend:SI
		  (vec_select:QI (match_dup 3) (parallel [(const_int 1)])))))
	  (plus:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		(zero_extend:SI
		  (vec_select:QI (match_dup 3) (parallel [(const_int 2)]))))
	      (mult:SI
		(sign_extend:SI
		  (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		(zero_extend:SI
		  (vec_select:QI (match_dup 3)
				 (parallel [(const_int 3)]))))))))]
  "TARGET_XTHEAD_ZPN"
  "smaqa.su\\t%0,%2,%3"
)

(define_insn "riscv_smaqa_su_v2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(plus:V2SI (match_operand:V2SI 1 "register_operand"  " 0")
	  (vec_concat:V2SI
	    (plus:SI
	      (plus:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI
		      (match_operand:V8QI 2 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (zero_extend:SI
		    (vec_select:QI
		      (match_operand:V8QI 3 "register_operand" "r")
		      (parallel [(const_int 0)]))))
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		  (zero_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 1)])))))
	    (plus:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		  (zero_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 2)]))))
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		  (zero_extend:SI
		    (vec_select:QI (match_dup 3)
				   (parallel [(const_int 3)]))))))
	    (plus:SI
	      (plus:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 4)])))
		  (zero_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 4)]))))
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 5)])))
		  (zero_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 5)])))))
	    (plus:SI
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 6)])))
		  (zero_extend:SI
		    (vec_select:QI (match_dup 3) (parallel [(const_int 6)]))))
		(mult:SI
		  (sign_extend:SI
		    (vec_select:QI (match_dup 2) (parallel [(const_int 7)])))
		  (zero_extend:SI
		    (vec_select:QI (match_dup 3)
				   (parallel [(const_int 7)])))))))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "smaqa.su\\t%0,%2,%3"
)


;;===================== 64-bit Profile Instructions ===========================

;;---------------- 64-bit Addition & Subtraction Instructions -----------------
;;Already implemented in previous: None

;;Implement: add64, sub64, kadd64, ksub64, ukadd64, uksub64
(define_insn "riscv_<dsp_arith_intruction_name>64"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(dsp_add_sub_code:DI
	  (match_operand:DI 1 "p_register_operand" "r")
	  (match_operand:DI 2 "p_register_operand" "r")))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "<dsp_arith_intruction_name>64\\t%0,%1,%2"
)

;;Implement: radd64, rsub64
(define_insn "riscv_r<dsp_arith_intruction_name>64"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(truncate:DI
	  (ashiftrt:TI
	    (dsp_integer_add_sub_code:TI
	      (sign_extend:TI (match_operand:DI 1 "p_register_operand" "r"))
	      (sign_extend:TI (match_operand:DI 2 "p_register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "r<dsp_arith_intruction_name>64\\t%0,%1,%2"
)

;;Implement: uradd64, ursub64
(define_insn "riscv_ur<dsp_arith_intruction_name>64"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(truncate:DI
	  (lshiftrt:TI
	    (dsp_integer_add_sub_code:TI
	      (zero_extend:TI (match_operand:DI 1 "p_register_operand" "r"))
	      (zero_extend:TI (match_operand:DI 2 "p_register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "ur<dsp_arith_intruction_name>64\\t%0,%1,%2"
)

;;---------- 32-bit Multiply with 64-bit Add/Subtract Instructions ------------
;;Already implemented in previous: None

;;Implement: smar64, umar64
(define_insn "riscv_<su>mar64_si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (mult:DI
	    (any_extend:DI (match_operand:SI 2 "register_operand" "r"))
	    (any_extend:DI (match_operand:SI 3 "register_operand" "r")))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "<su>mar64\t%0,%2,%3"
)

(define_insn "riscv_<su>mar64_v2si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (mult:DI
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (any_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (any_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)])))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "<su>mar64\t%0,%2,%3"
)

;;Implement: smsr64, umsr64
(define_insn "riscv_<su>msr64_si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI (match_operand:DI 1 "p_register_operand" "0")
	  (mult:DI
	    (any_extend:DI (match_operand:SI 2 "register_operand" "r"))
	    (any_extend:DI (match_operand:SI 3 "register_operand" "r")))))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "<su>msr64\\t%0,%2,%3"
)

(define_insn "riscv_<su>msr64_v2si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (minus:DI
	    (match_operand:DI 1 "p_register_operand" " 0")
	    (mult:DI
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" "r")
		  (parallel [(const_int 0)])))))
	  (mult:DI
	    (any_extend:DI
	      (vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	    (any_extend:DI
	      (vec_select:SI (match_dup 3) (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "<su>msr64\t%0,%2,%3"
)

;;Implement: kmar64
(define_insn "riscv_kmar64_si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))
	    (sign_extend:DI (match_operand:SI 3 "register_operand" "r")))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "kmar64\t%0,%2,%3"
)

(define_insn "riscv_kmar64_v2si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI (match_operand:DI 1 "p_register_operand" " 0")
	  (ss_plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "kmar64\t%0,%2,%3"
)

;;Implement: kmsr64
(define_insn "riscv_kmsr64_si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_minus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (mult:DI
	    (sign_extend:DI
	      (match_operand:SI 2 "register_operand" "r"))
	    (sign_extend:DI
	      (match_operand:SI 3 "register_operand" "r")))))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "kmsr64\t%0,%2,%3"
)

(define_insn "riscv_kmsr64_v2si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_minus:DI
	  (ss_minus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" "r")
		  (parallel [(const_int 0)])))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "kmsr64\t%0,%2,%3"
)

;;Implement: ukmar64
(define_insn "riscv_ukmar64_si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(us_plus:DI
	  (mult:DI
	    (zero_extend:DI
	      (match_operand:SI 2 "register_operand" "r"))
	    (zero_extend:DI
	      (match_operand:SI 3 "register_operand" "r")))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "ukmar64\t%0,%2,%3"
)

(define_insn "riscv_ukmar64_v2si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(us_plus:DI (match_operand:DI 1 "p_register_operand" " 0")
	  (us_plus:DI
	    (mult:DI
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "ukmar64\t%0,%2,%3"
)

;;Implement: ukmsr64
(define_insn "riscv_ukmsr64_si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(us_minus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (mult:DI
	    (zero_extend:DI
	      (match_operand:SI 2 "register_operand" "r"))
	    (zero_extend:DI
	      (match_operand:SI 3 "register_operand" "r")))))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "ukmsr64\t%0,%2,%3"
)

(define_insn "riscv_ukmsr64_v2si"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(us_minus:DI
	  (us_minus:DI
	    (match_operand:DI 1 "p_register_operand" " 0")
	    (mult:DI
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" "r")
		  (parallel [(const_int 0)])))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "ukmsr64\t%0,%2,%3"
)

;;-------- Signed 16-bit Multiply with 64-bit Add/Subtract Instructions--------
;;Already implemented in previous: None

;;Implement: smalbb
(define_insn "riscv_smalbb_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 0)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalbb\t%0,%2,%3"
)

(define_insn "*riscv_smalbb_hi"
  [(set (match_operand:DI 0                     "p_register_operand" "=r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI (match_operand:HI 2 "register_operand" "r"))
	    (sign_extend:DI (match_operand:HI 3 "register_operand" "r")))
	  (match_operand:DI 1                   "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalbb\t%0,%2,%3"
)

(define_insn "riscv_smalbb_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 2)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 2)])))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smalbb\t%0,%2,%3"
)

;;Implement: smalbt
(define_insn "riscv_smalbt_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 0)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalbt\t%0,%2,%3"
)

(define_insn "*riscv_smaltb_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalbt\t%0,%3,%2"
)

(define_insn "riscv_smalbt_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 2)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 3)])))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smalbt\t%0,%2,%3"
)

(define_insn "*riscv_smaltt_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 3)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 2)])))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smalbt\t%0,%2,%3"
)

;;Implement: smaltt
(define_insn "riscv_smaltt_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" "r")
		(parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 3 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smaltt\t%0,%2,%3"
)

(define_insn "riscv_smaltt_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 3 "register_operand" "r")
		  (parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 3)])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 3)
		  (parallel [(const_int 3)])))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smaltt\t%0,%2,%3"
)

;;Implement: smalda
(define_insn "riscv_smalda_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2)
			       (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 3)
			       (parallel [(const_int 1)])))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalda\t%0,%2,%3"
)

(define_insn "riscv_smalda_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (plus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 2 "register_operand" "r")
				 (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 3 "register_operand" "r")
				 (parallel [(const_int 0)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 1)])))))
	    (plus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smalda\t%0,%2,%3"
)

;;Implement: smalxda
(define_insn "riscv_smalxda_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2)
			       (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 3)
			       (parallel [(const_int 0)])))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalxda\t%0,%2,%3"
)

(define_insn "riscv_smalxda_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (plus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 2 "register_operand" "r")
				 (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 3 "register_operand" "r")
				 (parallel [(const_int 1)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 0)])))))
	    (plus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smalxda\t%0,%2,%3"
)

;;Implement: smalds
(define_insn "riscv_smalds_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (minus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2)
			       (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 3)
			       (parallel [(const_int 0)])))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalds\t%0,%2,%3"
)

(define_insn "riscv_smalds_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (minus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 2 "register_operand" "r")
				 (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 3 "register_operand" "r")
				 (parallel [(const_int 1)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 0)])))))
	    (minus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smalds\t%0,%2,%3"
)

;;Implement: smaldrs
(define_insn "riscv_smaldrs_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (minus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2)
			       (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 3)
			       (parallel [(const_int 1)])))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smaldrs\t%0,%2,%3"
)

(define_insn "riscv_smaldrs_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (minus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 2 "register_operand" "r")
				 (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 3 "register_operand" "r")
				 (parallel [(const_int 0)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 1)])))))
	    (minus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smaldrs\t%0,%2,%3"
)

;;Implement: smalxds
(define_insn "riscv_smalxds_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (minus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2)
			       (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 3)
			       (parallel [(const_int 1)])))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smalxds\t%0,%2,%3"
)

(define_insn "riscv_smalxds_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(plus:DI
	  (plus:DI
	    (minus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 2 "register_operand" "r")
				 (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI (match_operand:V4HI 3 "register_operand" "r")
				 (parallel [(const_int 0)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 1)])))))
	    (minus:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))))
	  (match_operand:DI 1 "p_register_operand" " 0")))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smalxds\t%0,%2,%3"
)

;;Implement: smslda
(define_insn "riscv_smslda_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (minus:DI
	    (match_operand:DI 1 "p_register_operand" " 0")
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 0)])))))
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:HI (match_dup 3) (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smslda\t%0,%2,%3"
)

(define_insn "riscv_smslda_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (minus:DI
	    (minus:DI
	      (minus:DI
		(match_operand:DI 1 "p_register_operand" " 0")
		(mult:DI
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		     (match_operand:V4HI 3 "register_operand" "r")
		     (parallel [(const_int 0)])))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 1)])))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 3) (parallel [(const_int 2)])))))
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (sign_extend:DI
	      (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smslda\t%0,%2,%3"
)

;;Implement: smslxda
(define_insn "riscv_smslxda_v2hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (minus:DI
	    (match_operand:DI 1 "p_register_operand" " 0")
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 1)])))))
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:HI (match_dup 3) (parallel [(const_int 0)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT"
  "smslxda\t%0,%2,%3"
)

(define_insn "riscv_smslxda_v4hi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (minus:DI
	    (minus:DI
	      (minus:DI
		(match_operand:DI 1 "p_register_operand" " 0")
		(mult:DI
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand" "r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		     (match_operand:V4HI 3 "register_operand" "r")
		     (parallel [(const_int 1)])))))
	      (mult:DI
		(sign_extend:DI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI (match_dup 3) (parallel [(const_int 0)])))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	      (sign_extend:DI
		(vec_select:HI (match_dup 3) (parallel [(const_int 3)])))))
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (sign_extend:DI
	      (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "smslxda\t%0,%2,%3"
)


;;========================= Non-SIMD Instructions =============================

;;----------------------- Q15 saturation instructions -------------------------
;;Already implemented in previous: None

(define_code_iterator dsp_fixp_sadd_ssub_code [
   ss_plus ss_minus
])

;;Implement: kaddh, ksubh
(define_insn "riscv_<dsp_arith_intruction_name>h"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(ss_truncate:HI
	  (dsp_fixp_sadd_ssub_code:SI
	    (match_operand:SI 1 "register_operand" "r")
	    (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name>h\\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_intruction_name>h_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(sign_extend:GPR
	  (ss_truncate:HI
	    (dsp_fixp_sadd_ssub_code:SI
	      (match_operand:SI 1 "register_operand" "r")
	      (match_operand:SI 2 "register_operand" "r")))))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name>h\\t%0,%1,%2"
)

(define_code_iterator dsp_fixp_uadd_usub_code [
   us_plus us_minus
])

;;Implement: ukaddh, uksubh
(define_insn "riscv_<dsp_arith_intruction_name>h"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(ss_truncate:HI
	  (dsp_fixp_uadd_usub_code:SI
	    (match_operand:SI 1 "register_operand" "r")
	    (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name>h\\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_intruction_name>h_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(sign_extend:GPR
	  (us_truncate:HI
	    (dsp_fixp_uadd_usub_code:SI
	      (match_operand:SI 1 "register_operand" "r")
	      (match_operand:SI 2 "register_operand" "r")))))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name>h\\t%0,%1,%2"
)

;;Implement: khmbb
(define_insn "*riscv_khmbb_hihi"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(truncate:HI
	  (ashiftrt:GPR
	    (ss_mult:GPR
	      (mult:GPR
		(sign_extend:GPR (match_operand:HI 1 "register_operand" "r"))
		(sign_extend:GPR (match_operand:HI 2 "register_operand" "r")))
	      (const_int 2))
	  (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "khmbb\\t%0,%1,%2"
)

(define_insn "*riscv_khmbb_hi<mode>"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(truncate:HI
	  (ashiftrt:GPR
	    (ss_mult:GPR
	      (mult:GPR
		(sign_extend:GPR
		  (vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
				 (parallel [(const_int 0)])))
		(sign_extend:GPR
		  (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
				 (parallel [(const_int 0)]))))
	      (const_int 2))
	  (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "khmbb\\t%0,%1,%2"
)

(define_insn "*riscv_khmbb_<mode>hi"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(ashiftrt:GPR
	  (ss_mult:GPR
	    (mult:GPR
	      (sign_extend:GPR (match_operand:HI 1 "register_operand" "r"))
	      (sign_extend:GPR (match_operand:HI 2 "register_operand" "r")))
	    (const_int 2))
	  (const_int 16)))]
  "TARGET_XTHEAD_ZPN"
  "khmbb\\t%0,%1,%2"
)

(define_insn "riscv_khmbb_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(ashiftrt:GPR
	  (ss_mult:GPR
	    (mult:GPR
	      (sign_extend:GPR
		(vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:GPR
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)]))))
	    (const_int 2))
	  (const_int 16)))]
  "TARGET_XTHEAD_ZPN"
  "khmbb\\t%0,%1,%2"
)


;;Implement: khmbt
(define_insn "*riscv_khmbt_hi<mode>"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(truncate:HI
	  (ashiftrt:GPR
	    (ss_mult:GPR
	      (mult:GPR
		(sign_extend:GPR
		  (vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
				 (parallel [(const_int 0)])))
		(sign_extend:GPR
		  (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
				 (parallel [(const_int 1)]))))
	      (const_int 2))
	  (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "khmbt\\t%0,%1,%2"
)

(define_insn "riscv_khmbt_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(ashiftrt:GPR
	  (ss_mult:GPR
	    (mult:GPR
	     (sign_extend:GPR
		(vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			       (parallel [(const_int 0)])))
	     (sign_extend:GPR
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	      (const_int 2))
	  (const_int 16)))]
  "TARGET_XTHEAD_ZPN"
  "khmbt\\t%0,%1,%2"
)

;;Implement: khmtt
(define_insn "*riscv_khmtt_hi<mode>"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(truncate:HI
	  (ashiftrt:GPR
	    (ss_mult:GPR
	      (mult:GPR
		(sign_extend:GPR
		  (vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
				 (parallel [(const_int 1)])))
		(sign_extend:GPR
		  (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
				 (parallel [(const_int 1)]))))
	      (const_int 2))
	  (const_int 16))))]
  "TARGET_XTHEAD_ZPN"
  "khmtt\\t%0,%1,%2"
)

(define_insn "riscv_khmtt_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(ashiftrt:GPR
	  (ss_mult:GPR
	    (mult:GPR
	     (sign_extend:GPR
		(vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			       (parallel [(const_int 1)])))
	     (sign_extend:GPR
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	      (const_int 2))
	  (const_int 16)))]
  "TARGET_XTHEAD_ZPN"
  "khmtt\\t%0,%1,%2"
)

;;----------------------- Q31 saturation instructions -------------------------
;;Already implemented in previous: None

;;Implement: kaddw, ksubw, ukaddw, uksubw
(define_insn "riscv_<dsp_arith_intruction_name>w"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(dsp_fixp_add_sub_code:SI
	  (match_operand:SI 1 "register_operand" "r")
	  (match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name>w\\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_intruction_name>w_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(dsp_fixp_add_sub_code:SI
	  (match_operand:SI 1 "register_operand" "r")
	  (match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "<dsp_arith_intruction_name>w\\t%0,%1,%2"
)

(define_insn "riscv_<dsp_arith_intruction_name>w_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (dsp_fixp_add_sub_code:SI
	    (match_operand:SI 1 "register_operand" "r")
	    (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "<dsp_arith_intruction_name>w\\t%0,%1,%2"
)

;;Implement: kdmbb
(define_insn "*riscv_kdmbb_scalar"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_mult:SI
	  (mult:SI
	    (sign_extend:SI (match_operand:HI 1 "register_operand" "r"))
	    (sign_extend:SI (match_operand:HI 2 "register_operand" "r")))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "kdmbb\\t%0,%1,%2"
)

(define_insn "riscv_kdmbb"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_mult:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			     (parallel [(const_int 0)])))
	    (sign_extend:SI
	      (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			     (parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "kdmbb\\t%0,%1,%2"
)

(define_insn "*riscv_kdmbb_scalardi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI (match_operand:HI 1 "register_operand" "r"))
	      (sign_extend:SI (match_operand:HI 2 "register_operand" "r")))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmbb\\t%0,%1,%2"
)

(define_insn "*riscv_kdmbb_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)]))))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmbb\\t%0,%1,%2"
)

;;Implement: kdmbt
(define_insn "riscv_kdmbt"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_mult:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			     (parallel [(const_int 0)])))
	    (sign_extend:SI
	      (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			     (parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "kdmbt\\t%0,%1,%2"
)

(define_insn "*riscv_kdmbt_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmbt\\t%0,%1,%2"
)

;;Implement: kdmtt
(define_insn "riscv_kdmtt"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_mult:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			     (parallel [(const_int 1)])))
	    (sign_extend:SI
	      (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			     (parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_XTHEAD_ZPN"
  "kdmtt\\t%0,%1,%2"
)

(define_insn "*riscv_kdmtt_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 1 "register_operand" "r")
			       (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmtt\\t%0,%1,%2"
)

;;Implement: kslraw
(define_insn "riscv_kslraw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(if_then_else:SI
	  (lt:SI (match_operand:SI 2 "register_operand" "r")
		 (const_int 0))
	  (ashiftrt:SI (match_operand:SI 1 "register_operand" "r")
			 (neg:SI (match_dup 2)))
	  (ss_ashift:SI (match_dup 1)
			  (match_dup 2))))]
  "TARGET_XTHEAD_ZPN"
  "kslraw\t%0,%1,%2"
)

(define_insn "riscv_kslraw_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(if_then_else:SI
	  (lt:SI (match_operand:SI 2 "register_operand" "r")
		 (const_int 0))
	  (ashiftrt:SI (match_operand:SI 1 "register_operand" "r")
			 (neg:SI (match_dup 2)))
	  (ss_ashift:SI (match_dup 1)
			  (match_dup 2))))]
  "TARGET_XTHEAD_ZPN"
  "kslraw\t%0,%1,%2"
)

(define_insn "riscv_kslraw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (if_then_else:SI
	    (lt:SI (match_operand:SI 2 "register_operand" "r")
		   (const_int 0))
	    (ashiftrt:SI (match_operand:SI 1 "register_operand" "r")
			 (neg:SI (match_dup 2)))
	    (ss_ashift:SI (match_dup 1)
			  (match_dup 2)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kslraw\t%0,%1,%2"
)

;;Implement: kslraw.u
(define_insn "riscv_kslraw_u"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(if_then_else:SI
	  (lt:SI (match_operand:SI 2 "register_operand" "r")
		 (const_int 0))
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (neg:SI (match_dup 2))] UNSPEC_SRA_U)
	  (ss_ashift:SI (match_dup 1)
			(match_dup 2))))]
  "TARGET_XTHEAD_ZPN"
  "kslraw.u\t%0,%1,%2"
)

(define_insn "riscv_kslraw_u_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(if_then_else:SI
	  (lt:SI (match_operand:SI 2 "register_operand" "r")
		 (const_int 0))
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (neg:SI (match_dup 2))] UNSPEC_SRA_U)
	  (ss_ashift:SI (match_dup 1)
			(match_dup 2))))]
  "TARGET_XTHEAD_ZPN"
  "kslraw.u\t%0,%1,%2"
)

(define_insn "riscv_kslraw_u_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (if_then_else:SI
	    (lt:SI (match_operand:SI 2 "register_operand" "r")
		   (const_int 0))
	    (unspec:SI [(match_operand:SI 1 "register_operand" "r")
			(neg:SI (match_dup 2))] UNSPEC_SRA_U)
	    (ss_ashift:SI (match_dup 1)
			  (match_dup 2)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kslraw.u\t%0,%1,%2"
)

;;Implement: ksllw, kslliw
(define_insn "riscv_ksllw"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
     (ss_ashift:SI
	(match_operand:SI 1 "register_operand" "r,r")
	(match_operand:SI 2 "const_int_or_reg_operand" "i,r")))]
  "TARGET_XTHEAD_ZPN"
  {
    if (which_alternative == 0)
      {
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
	return "kslliw\\t%0,%1,%2";
      }
    else
      return "ksllw\\t%0,%1,%2";
  }
)

(define_insn "riscv_ksllw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r,r")
	(sign_extend:DI (ss_ashift:SI
	    (match_operand:SI 1 "register_operand" "r,r")
	    (match_operand:SI 2 "const_int_or_reg_operand" "i,r"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  {
    if (which_alternative == 0)
      {
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
	return "kslliw\\t%0,%1,%2";
      }
    else
      return "ksllw\\t%0,%1,%2";
  }
)

;;Implement: kdmabb
(define_insn "*riscv_kdmabb_scalar"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI (match_operand:HI 2 "register_operand" "r"))
	      (sign_extend:SI (match_operand:HI 3 "register_operand" "r")))
	    (const_int 2))
	(match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN"
  "kdmabb\\t%0,%2,%3"
)

(define_insn "riscv_kdmabb"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 0)]))))
	    (const_int 2))
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN"
  "kdmabb\\t%0,%2,%3"
)

(define_insn "*riscv_kdmabb_scalardi"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_plus:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI (match_operand:HI 2 "register_operand" "r"))
		(sign_extend:SI (match_operand:HI 3 "register_operand" "r")))
	      (const_int 2))
	    (match_operand:SI 1 "register_operand" "0"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmabb\\t%0,%2,%3"
)

(define_insn "*riscv_kdmabb_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_plus:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
				 (parallel [(const_int 0)])))
		(sign_extend:SI
		  (vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
				 (parallel [(const_int 0)]))))
	      (const_int 2))
	    (match_operand:SI 1 "register_operand" "0"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmabb\\t%0,%2,%3"
)

;;Implement: kdmabt
(define_insn "riscv_kdmabt"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	    (const_int 2))
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN"
  "kdmabt\\t%0,%2,%3"
)

(define_insn "*riscv_kdmabt_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_plus:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
				 (parallel [(const_int 0)])))
		(sign_extend:SI
		  (vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
				 (parallel [(const_int 1)]))))
	      (const_int 2))
	    (match_operand:SI 1 "register_operand" "0"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmabt\\t%0,%2,%3"
)

;;Implement: kdmatt
(define_insn "riscv_kdmatt"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_plus:SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
			       (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
			       (parallel [(const_int 1)]))))
	    (const_int 2))
	  (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN"
  "kdmatt\\t%0,%2,%3"
)

(define_insn "*riscv_kdmatt_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_plus:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI (match_operand:V2HI 2 "register_operand" "r")
				 (parallel [(const_int 1)])))
		(sign_extend:SI
		  (vec_select:HI (match_operand:V2HI 3 "register_operand" "r")
				 (parallel [(const_int 1)]))))
	      (const_int 2))
	    (match_operand:SI 1 "register_operand" "0"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kdmatt\\t%0,%2,%3"
)

;;Implement: kabsw
(define_insn "riscv_kabsw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ss_abs:SI
	  (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "kabsw\\t%0,%1"
)

(define_insn "*riscv_kabsw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (ss_abs:SI
	    (match_operand:SI 1 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "kabsw\\t%0,%1"
)

;;--------------------- 32-bit Computation Instructions -----------------------
;;Already implemented in previous: None

;;Implement: raddw
(define_insn "riscv_raddw_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (plus:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	      (sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPN"
  "raddw\\t%0,%1,%2"
)

(define_insn "riscv_raddw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ashiftrt:DI
	  (plus:DI
	    (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	    (sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "raddw\\t%0,%1,%2"
)

(define_insn "*riscv_raddw_di2"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend: DI
	  (truncate:SI
	    (ashiftrt:DI
	      (plus:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	      (const_int 1)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "raddw\\t%0,%1,%2"
)

;;Implement:rsubw
(define_insn "riscv_rsubw_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (minus:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	      (sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPN"
  "rsubw\\t%0,%1,%2"
)

(define_insn "riscv_rsubw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ashiftrt:DI
	  (minus:DI
	    (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	    (sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "rsubw\\t%0,%1,%2"
)

(define_insn "*riscv_rsubw_di2"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (truncate:SI
	    (ashiftrt:DI
	      (minus:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	      (const_int 1)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "rsubw\\t%0,%1,%2"
)

;;Implement: uraddw
(define_insn "riscv_uraddw_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (plus:DI
	      (zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
	      (zero_extend:DI (match_operand:SI 2 "register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPN"
  "uraddw\\t%0,%1,%2"
)

(define_insn "riscv_uraddw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(lshiftrt:DI
	  (plus:DI
	    (zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
	    (zero_extend:DI (match_operand:SI 2 "register_operand" "r")))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "uraddw\\t%0,%1,%2"
)

(define_insn "*riscv_uraddw_di2"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend: DI
	  (truncate:SI
	    (lshiftrt:DI
	      (plus:DI
		(zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(zero_extend:DI (match_operand:SI 2 "register_operand" "r")))
	      (const_int 1)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "uraddw\\t%0,%1,%2"
)

;;Implement: ursubw
(define_insn "riscv_ursubw_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (minus:DI
	      (zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
	      (zero_extend:DI (match_operand:SI 2 "register_operand" "r")))
	    (const_int 1))))]
  "TARGET_XTHEAD_ZPN"
  "ursubw\\t%0,%1,%2"
)

(define_insn "riscv_ursubw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ashiftrt:DI
	  (minus:DI
	    (zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
	    (zero_extend:DI (match_operand:SI 2 "register_operand" "r")))
	  (const_int 1)))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "ursubw\\t%0,%1,%2"
)

(define_insn "*riscv_ursubw_di2"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (truncate:SI
	    (ashiftrt:DI
	      (minus:DI
		(zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(zero_extend:DI (match_operand:SI 2 "register_operand" "r")))
	      (const_int 1)))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "ursubw\\t%0,%1,%2"
)

;;Implement: maxw
(define_insn "riscv_maxw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(smax:SI (match_operand:SI 1 "register_operand" "r")
		 (match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "maxw\\t%0,%1,%2"
)

(define_insn "riscv_maxw_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(smax:SI (match_operand:SI 1 "register_operand" "r")
		 (match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "maxw\\t%0,%1,%2"
)

(define_insn "riscv_maxw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (smax:SI (match_operand:SI 1 "register_operand" "r")
		   (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "maxw\\t%0,%1,%2"
)

;;Implement: minw
(define_insn "riscv_minw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(smin:SI (match_operand:SI 1 "register_operand" "r")
		 (match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "minw\\t%0,%1,%2"
)

(define_insn "riscv_minw_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(smin:SI (match_operand:SI 1 "register_operand" "r")
		 (match_operand:SI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  "minw\\t%0,%1,%2"
)

(define_insn "riscv_minw_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (smin:SI (match_operand:SI 1 "register_operand" "r")
		   (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "minw\\t%0,%1,%2"
)

;;Implement: mulr64
(define_insn "riscv_mulr64"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(mult:DI
	  (zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
	  (zero_extend:DI (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "mulr64\\t%0,%1,%2"
)

;;Implement: mulsr64
(define_insn "riscv_mulsr64"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(mult:DI
	  (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	  (sign_extend:DI (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "mulsr64\\t%0,%1,%2"
)

;;Implement: maddr32
(define_insn "riscv_maddr32"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (mult:SI (match_operand:SI 2 "register_operand" "r")
			  (match_operand:SI 3 "register_operand" "r"))
		 (match_operand:SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPN"
  "maddr32\\t%0,%2,%3"
)

(define_insn "riscv_maddr32_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (plus:SI
	    (mult:SI (match_operand:SI 2 "register_operand" "r")
		     (match_operand:SI 3 "register_operand" "r"))
	  (match_operand:SI 1 "register_operand" "0"))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "maddr32\\t%0,%2,%3"
)

;;Implement: msubr32
(define_insn "riscv_msubr32"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI (match_operand:SI 1 "register_operand" "0")
		  (mult:SI (match_operand:SI 2 "register_operand" "r")
			   (match_operand:SI 3 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPN"
  "msubr32\\t%0,%2,%3"
)

(define_insn "riscv_msubr32_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (minus:SI (match_operand:SI 1 "register_operand" "0")
		    (mult:SI (match_operand:SI 2 "register_operand" "r")
			     (match_operand:SI 3 "register_operand" "r")))))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  "msubr32\\t%0,%2,%3"
)

;;------------- Overflow/Saturation status manipulation instructions ----------
;;Already implemented in previous: None

;;Implement: rdov
(define_insn "riscv_rdov_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec_volatile:GPR [(const_int 0)] UNSPECV_RDOV))]
  "TARGET_XTHEAD_ZPN || TARGET_XTHEAD_ZPSFOPERAND"
  "rdov\\t%0"
)

;;Implement: clrov
(define_insn "riscv_clrov"
  [(unspec_volatile [(const_int 0)] UNSPECV_CLROV)]
  "TARGET_XTHEAD_ZPN || TARGET_XTHEAD_ZPSFOPERAND"
  "clrov"
)

;;------------------------ Miscellaneous Instructions -------------------------
;;Already implemented in previous: None
;;wext, wexti, bpick, insb

;;Implement: ave
(define_insn "riscv_ave_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(truncate:X
	  (ashiftrt:<DSP_WIDEN>
	    (plus:<DSP_WIDEN>
	      (plus:<DSP_WIDEN>
		(sign_extend:<DSP_WIDEN>
		  (match_operand:X 1 "register_operand" "r"))
		(sign_extend:<DSP_WIDEN>
		  (match_operand:X 2 "register_operand" "r")))
	      (const_int 1))
	  (const_int 1))))]
  "TARGET_XTHEAD_ZPN"
  "ave\\t%0,%1,%2"
)

;;Implement: sra.u
(define_insn "riscv_sra_u_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(unspec:X
	  [(match_operand:X 1 "register_operand" "r")
	   (match_operand:SI 2 "register_operand" "r")]
	UNSPEC_SRA_U))]
  "TARGET_XTHEAD_ZPN"
  "sra.u\\t%0,%1,%2"
)

;;Implement: srai.u
(define_insn "*riscv_srai_u_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(unspec:X
	  [(match_operand:X 1 "register_operand" "r")
	   (match_operand:SI 2 "immediate_operand" "i")]
	UNSPEC_SRA_U))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & (BITS_PER_WORD - 1));
    return "srai.u\\t%0,%1,%2";
  }
)

;;Implement: bitrev
(define_insn "riscv_bitrev_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
     (unspec:X
      [(match_operand:X 1 "register_operand" "r")
       (match_operand:SI 2 "register_operand" "r")]
       UNSPEC_BITREV))]
  "TARGET_XTHEAD_ZPN"
  "bitrev\\t%0,%1,%2"
)

;;Implement: bitrevi
(define_insn "*riscv_bitrevi_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
     (unspec:X
      [(match_operand:X 1 "register_operand" "r")
       (match_operand:SI 2 "immediate_operand" "i")]
       UNSPEC_BITREV))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & (BITS_PER_WORD - 1));
    return "bitrevi\\t%0,%1,%2";
  }
)

;;Implement: wext, wexti
(define_insn "riscv_<u>wext_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (any_shiftrt:DI
	    (match_operand:DI 1 "p_register_operand" "r")
	    (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "wext\t%0,%1,%2"
)

(define_insn "riscv_wext_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (truncate:SI
	    (ashiftrt:DI
	      (match_operand:DI 1 "p_register_operand" "r")
	      (match_operand:SI 2 "register_operand" "r")))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  "wext\t%0,%1,%2"
)

(define_insn_and_split "*wext_<code>di3"
  [(set (match_operand:DI 0 "p_register_operand" "")
	(any_shift:DI (match_operand:DI 1 "p_register_operand" "")
		      (match_operand:QI 2 "imm6u_operand" "")))]
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT && !reload_completed"
  "#"
  "TARGET_XTHEAD_ZPSFOPERAND && !TARGET_64BIT && !reload_completed"
  [(const_int 0)]
  {
    riscv_split_<code>di3 (operands[0], operands[1], operands[2]);
    DONE;
  }
)

;;Implement: wexti
(define_insn "riscv_<u>wexti_si"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (any_shiftrt:DI
	    (match_operand:DI 1 "p_register_operand" "r")
	    (match_operand:SI 2 "immediate_operand" "i"))))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
    return "wexti\\t%0,%1,%2";
  }
)

(define_insn "riscv_wexti_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (truncate:SI
	    (ashiftrt:DI
	      (match_operand:DI 1 "p_register_operand" "r")
	      (match_operand:SI 2 "immediate_operand" "i")))))]
  "TARGET_XTHEAD_ZPSFOPERAND && TARGET_64BIT"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
    return "wexti\\t%0,%1,%2";
  }
)

(define_insn "*riscv_wexti_<mode>2"
  [(set (match_operand:GPR 0 "register_operand" "=r, r")
	(sign_extract:GPR (match_operand:DI 1 "p_register_operand" "r, r")
			  (const_int 32)
			  (match_operand 2 "reg_or_uimm5_operand" "r, i")))]
  "TARGET_XTHEAD_ZPSFOPERAND"
  "@
   wext\t%0,%1,%2
   wexti\t%0,%1,%2"
)

;;Implement: bpick
(define_insn "riscv_bpick_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	  (ior:GPR
	    (and:GPR
	      (match_operand:GPR 1 "register_operand" "r")
	      (match_operand:GPR 3 "register_operand" "r"))
	    (and:GPR
	      (match_operand:GPR 2 "register_operand" "r")
	      (not:GPR (match_dup 3)))))]
  "TARGET_XTHEAD_ZPN"
  "bpick\t%0,%1,%2,%3"
)

;;Implement: insb
(define_expand "riscv_insb_<mode>"
  [(match_operand:GPR 0 "register_operand" "")
   (match_operand:GPR 1 "register_operand" "0")
   (match_operand:GPR 2 "register_operand" "")
   (match_operand:SI 3 "const_int_operand" "")]
  "TARGET_XTHEAD_ZPN"
  {
    operands[3] = GEN_INT (INTVAL (operands[3]) & (<dsp_ebits>/8 - 1));

    HOST_WIDE_INT selector_index;
    selector_index = INTVAL (operands[3]);
    rtx selector = gen_int_mode (selector_index * 8, SImode);

    emit_insn (gen_insv<mode>_internal (operands[1], selector, operands[2]));
    emit_move_insn (operands[0], operands[1]);
    DONE;
  }
)

(define_insn "insv<mode>_internal"
  [(set (zero_extract:GPR (match_operand:GPR 0 "register_operand" "+r")
			  (const_int 8)
			  (match_operand:SI 1 "insv<dsp_ebits>_operand" "i"))
	(match_operand:GPR 2 "register_operand" "r"))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[1] = GEN_INT (INTVAL (operands[1]) / 8);
    return "insb\t%0,%2,%1";
  }
)

(define_insn "*insv<mode>_internal2"
  [(set (zero_extract:GPR (match_operand:GPR 0 "register_operand" "+r")
			  (const_int 8)
			  (match_operand:SI 1 "insv<dsp_ebits>_operand" "i"))
	(zero_extend:GPR (match_operand:QI 2 "register_operand" "r")))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[1] = GEN_INT (INTVAL (operands[1]) / 8);
    return "insb\t%0,%2,%1";
  }
)

(define_insn "vec_setv4qi_internal"
  [(set (match_operand:V4QI 0 "register_operand" "=r")
	(vec_merge:V4QI
	  (vec_duplicate:V4QI
	    (match_operand:QI 1 "register_operand" "r"))
	  (match_operand:V4QI 2 "register_operand" "0" )
	  (match_operand:SI 3 "pwr_3_operand" "i")))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[3] = GEN_INT (exact_log2 (INTVAL (operands[3])));
    return "insb\t%0,%1,%3";
  }
)

(define_insn "vec_setv4qi_internal_vec"
  [(set (match_operand:V4QI 0 "register_operand" "=r")
	(vec_merge:V4QI
	  (vec_duplicate:V4QI
	    (vec_select:QI
	      (match_operand:V4QI 1 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (match_operand:V4QI 2 "register_operand" "0")
	  (match_operand:SI 3 "pwr_3_operand" "i")))]
  "TARGET_XTHEAD_ZPN"
  {
    operands[3] = GEN_INT (exact_log2 (INTVAL (operands[3])));
    return "insb\t%0,%1,%3";
  }
)

(define_insn "vec_setv8qi_internal"
  [(set (match_operand:V8QI 0 "register_operand" "=r")
	(vec_merge:V8QI
	  (vec_duplicate:V8QI
	    (match_operand:QI 1 "register_operand" "r"))
	  (match_operand:V8QI 2 "register_operand" "0" )
	  (match_operand:SI 3 "pwr_7_operand" "i")))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  {
    operands[3] = GEN_INT (exact_log2 (INTVAL (operands[3])));
    return "insb\t%0,%1,%3";
  }
)

(define_insn "vec_setv8qi_internal_vec"
  [(set (match_operand:V8QI 0 "register_operand" "=r")
	(vec_merge:V8QI
	  (vec_duplicate:V8QI
	    (vec_select:QI
	      (match_operand:V8QI 1 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (match_operand:V8QI 2 "register_operand" "0")
	  (match_operand:SI 3 "pwr_7_operand" "i")))]
  "TARGET_XTHEAD_ZPN && TARGET_64BIT"
  {
    operands[3] = GEN_INT (exact_log2 (INTVAL (operands[3])));
    return "insb\t%0,%1,%3";
  }
)


;;========================= RV64 Only Instructions ============================

;;---------------- 32-bit Addition & Subtraction Instructions -----------------
;;Already implemented in previous: add32, radd32, uradd32, kadd32, ukadd32,
;;                                 sub32, rsub32, ursub32, ksub32, uksub32,
;;                                 cras32, rcras32, ucras32, kcras32, ukcras32,
;;                                 crsa23, rcrsa32, urcrsa32, kcrsa32,
;;                                 ukcrsa32, stas32, rstas32, urstas32,
;;                                 kstas32, ukstas32,  stsa32, rstsa32,
;;                                 urstsa32, kstsa32, ukstsa32

;;----------------------- 32-bit shift Instructions ---------------------------
;;Already implemented in previous: sra32, srai32, sra32.u, srai32.u, srl32,
;;                                 srli32, srl32.u, srli32.u, sll32, slli32,
;;                                 ksll32, kslli32, kslra32, ksra32.u

;;-------------------- 32-bit Miscellaneous Instructions ----------------------
;;Already implemented in previous: smin32, umin32, smax32, umax32, kabs32

;;------------------- Q15 saturating Multiply Instructions --------------------
;;Already implemented in previous: None

;;Implement: khmbb16
(define_insn "riscv_khmbb16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 1 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (const_int 2))
	    (const_int 16))
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 1)
		    (parallel [(const_int 2)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 2)]))))
	      (const_int 2))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "khmbb16\t%0,%1,%2"
)

;;Implement: khmbt16
(define_insn "riscv_khmbt16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 1 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (const_int 2))
	    (const_int 16))
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 1)
		    (parallel [(const_int 2)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 3)]))))
	      (const_int 2))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "khmbt16\t%0,%1,%2"
)

(define_insn "*riscv_khmtb16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 1 "register_operand" "r")
		    (parallel [(const_int 1)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (const_int 2))
	    (const_int 16))
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 1)
		    (parallel [(const_int 3)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 2)]))))
	      (const_int 2))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "khmbt16\t%0,%2,%1"
)

;;Implement: khmtt16
(define_insn "riscv_khmtt16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 1 "register_operand" "r")
		    (parallel [(const_int 1)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (const_int 2))
	    (const_int 16))
	  (ashiftrt:SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 1)
		    (parallel [(const_int 2)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 2)]))))
	      (const_int 2))
	    (const_int 16))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "khmtt16\t%0,%1,%2"
)

;;Implement: kdmbb16
(define_insn "riscv_kdmbb16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 1 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (const_int 2))
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 1)
		  (parallel [(const_int 2)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 2)]))))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmbb16\t%0,%1,%2"
)

;;Implement: kdmbt16
(define_insn "riscv_kdmbt16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 1 "register_operand" "r")
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1)]))))
	    (const_int 2))
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 1)
		  (parallel [(const_int 2)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 3)]))))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmbt16\t%0,%1,%2"
)

(define_insn "*riscv_kdmtb16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 1 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 0)]))))
	    (const_int 2))
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 1)
		  (parallel [(const_int 3)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 2)]))))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmbt16\t%0,%2,%1"
)

;;Implement: kdmtt16
(define_insn "riscv_kdmtt16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_concat:V2SI
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 1 "register_operand" "r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand" "r")
		  (parallel [(const_int 1)]))))
	    (const_int 2))
	  (ss_mult:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 1)
		  (parallel [(const_int 3)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 3)]))))
	    (const_int 2))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmtt16\t%0,%1,%2"
)

;;Implement: kdmabb16
(define_insn "riscv_kdmabb16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (vec_concat:V2SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 3 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (const_int 2))
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 2)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 3)
		    (parallel [(const_int 2)]))))
	      (const_int 2)))
	  (match_operand: V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmabb16\t%0,%2,%3"
)

;;Implement: kdmabt16
(define_insn "riscv_kdmabt16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (vec_concat:V2SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 3 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (const_int 2))
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 2)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 3)
		    (parallel [(const_int 3)]))))
	      (const_int 2)))
	  (match_operand: V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmabt16\t%0,%2,%3"
)

(define_insn "*riscv_kdmatb16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (vec_concat:V2SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 1)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 3 "register_operand" "r")
		    (parallel [(const_int 0)]))))
	      (const_int 2))
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 3)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 3)
		    (parallel [(const_int 2)]))))
	      (const_int 2)))
	  (match_operand: V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmabt16\t%0,%2,%3"
)

;;Implement: kdmatt16
(define_insn "riscv_kdmatt16"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(ss_plus:V2SI
	  (vec_concat:V2SI
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" "r")
		    (parallel [(const_int 1)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 3 "register_operand" "r")
		    (parallel [(const_int 1)]))))
	      (const_int 2))
	    (ss_mult:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(const_int 3)])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 3)
		    (parallel [(const_int 3)]))))
	      (const_int 2)))
	  (match_operand: V2SI 1 "register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kdmatt16\t%0,%2,%3"
)

;;---------------------- 32-bit Multiply Instructions -------------------------
;;Already implemented in previous: None

;;Implemented: smbb32, smbt32, smtt32
(define_insn "riscv_smbb32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(mult:DI
	  (sign_extend:DI
	     (vec_select:SI
	       (match_operand:V2SI 1 "register_operand" "r")
	       (parallel [(const_int 0)])))
	  (sign_extend:DI (vec_select:SI
	       (match_operand:V2SI 2 "register_operand" "r")
	       (parallel [(const_int 0)])))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "smbb32\t%0,%1,%2"
)

(define_insn "riscv_smbt32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(mult:DI
	  (sign_extend:DI
	     (vec_select:SI
	       (match_operand:V2SI 1 "register_operand" "r")
	       (parallel [(const_int 0)])))
	  (sign_extend:DI (vec_select:SI
	       (match_operand:V2SI 2 "register_operand" "r")
	       (parallel [(const_int 1)])))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "smbt32\t%0,%1,%2"
)

(define_insn "*riscv_smtb32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(mult:DI
	  (sign_extend:DI
	     (vec_select:SI
	       (match_operand:V2SI 1 "register_operand" "r")
	       (parallel [(const_int 1)])))
	  (sign_extend:DI (vec_select:SI
	       (match_operand:V2SI 2 "register_operand" "r")
	       (parallel [(const_int 0)])))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "smbt32\t%0,%2,%1"
)

(define_insn "riscv_smtt32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(mult:DI
	  (sign_extend:DI
	     (vec_select:SI
	       (match_operand:V2SI 1 "register_operand" "r")
	       (parallel [(const_int 1)])))
	  (sign_extend:DI (vec_select:SI
	       (match_operand:V2SI 2 "register_operand" "r")
	       (parallel [(const_int 1)])))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "smtt32\t%0,%1,%2"
)

;;------------------- 32-bit Multiply & Add Instructions ----------------------
;;Already implemented in previous: None

;;Implemented: kmabb32
(define_insn "riscv_kmabb32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 2 "register_operand" "r")
		(parallel [(const_int 0)])))
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 3 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmabb32\t%0,%2,%3"
)

;;Implemented: kmabt32
(define_insn "riscv_kmabt32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 2 "register_operand" "r")
		(parallel [(const_int 0)])))
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 3 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmabt32\t%0,%2,%3"
)

(define_insn "*riscv_kmatb32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 2 "register_operand" "r")
		(parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 3 "register_operand" "r")
		(parallel [(const_int 0)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmabt32\t%0,%2,%3"
)

;;Implemented: kmatt32
(define_insn "riscv_kmatt32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 2 "register_operand" "r")
		(parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 3 "register_operand" "r")
		(parallel [(const_int 1)]))))
	  (match_operand:DI 1 "p_register_operand" "0")))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmatt32\t%0,%2,%3"
)

;;--------------- 32-bit Parallel Multiply & Add Instructions -----------------
;;Already implemented in previous: None

;;Impement: kmda32
(define_insn "riscv_kmda32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" "r")
			      (parallel [(const_int 1)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmda32\t%0,%1,%2"
)

;;Impement: kmxda32
(define_insn "riscv_kmxda32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmxda32\t%0,%1,%2"
)

;;Impement: kmada32
(define_insn "riscv_kmada32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "p_register_operand" "0")
	  (ss_plus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" "r")
				(parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmada32\t%0,%2,%3"
)

;;Impement: kmaxda32
(define_insn "riscv_kmaxda32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (ss_plus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" "r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmaxda32\t%0,%2,%3"
)

;;Impement: kmads32
(define_insn "riscv_kmads32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" "r")
				(parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmads32\t%0,%2,%3"
)

;;Impement: kmadrs32
(define_insn "riscv_kmadrs32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" "r")
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" "r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmadrs32\t%0,%2,%3"
)

;;Impement: kmaxds32
(define_insn "riscv_kmaxds32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" "r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmaxds32\t%0,%2,%3"
)

;;Impement: kmsda32
(define_insn "riscv_kmsda32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_minus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" "r")
				(parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmsda32\t%0,%2,%3"
)

;;Impement: kmsxda32
(define_insn "riscv_kmsxda32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(ss_minus:DI
	  (match_operand:DI 1 "p_register_operand" " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" "r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" "r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "kmsxda32\t%0,%2,%3"
)

;;Impement: smds32
(define_insn "riscv_smds32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" "r")
			      (parallel [(const_int 1)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "smds32\t%0,%1,%2"
)

;;Impement: smdrs32
(define_insn "riscv_smdrs32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" "r")
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "smdrs32\t%0,%1,%2"
)

;;Impement: smxds32
(define_insn "riscv_smxds32"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(minus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  "smxds32\t%0,%1,%2"
)

;;--------------------- Non-SIMD 32-bit Shift Instructions --------------------
;;Already implemented in previous: None

;;Implemented: sraiw.u
(define_insn "riscv_sraw_u"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI
	  [(match_operand:SI 1 "register_operand" "r")
	   (match_operand:SI 2 "immediate_operand" "i")]
	  UNSPEC_SRA_U))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
    return "sraiw.u\\t%0,%1,%2";
  }
)

(define_insn "riscv_sraw_u_di"
  [(set (match_operand:DI 0 "p_register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI
	    [(match_operand:SI 1 "register_operand" "r")
	     (match_operand:SI 2 "immediate_operand" "i")]
	    UNSPEC_SRA_U)))]
  "TARGET_XTHEAD_ZPRVSFEXTRA"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
    return "sraiw.u\\t%0,%1,%2";
  }
)

;;----------------------- 32-bit Packing Instructions -------------------------
;;Already implemented in previous: pkbb32, pkbt32, pktb32, pktt32

;;################################### END #####################################
