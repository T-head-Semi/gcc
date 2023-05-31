
(define_constants
  [(T0_REGNUM	5)
   (T1_REGNUM	6)
   (T2_REGNUM	7)
   (A0_REGNUM	10)
   (A1_REGNUM	11)
   (A2_REGNUM	12)
   (A3_REGNUM	13)
   (A4_REGNUM	14)
   (A5_REGNUM	15)
   (A6_REGNUM	16)
   (A7_REGNUM	17)
   (T3_REGNUM	28)
   (T4_REGNUM	29)
   (T5_REGNUM	30)
   (T6_REGNUM	31)
])

;; CONSTRAINT
;; ==========

(define_register_constraint "Qrf" "TARGET_XTHEAD_FMVHW32 ? FP_REGS : NO_REGS"
  "A floating-point register (if available).")

(define_register_constraint "Qrx" "TARGET_XTHEAD_FMVHW32 ? GR_REGS : NO_REGS"
  "A integer register (if available).")

(define_constraint "QcL"
  "A 6-bit unsigned immediate for zext instructions."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 63)")))

(define_constraint "Pi"
  "A unsigned offset for second mem which for ldd/sdd instructions."
  (and (match_code "const_int")
       (match_test "ival >= 8 && ival <= 56")))

(define_constraint "Pj"
  "A unsigned offset for second mem which for lwd/swd instructions."
  (and (match_code "const_int")
       (match_test "ival >= 4 && ival <= 20")))

(define_memory_constraint "Qmx"
  "@internal
   An address valid for GPR."
  (and (match_code "mem")
       (match_test "!riscv_legitimize_address_index_p (XEXP (op, 0), GET_MODE (op), false)")))

(define_memory_constraint "Qmr"
  "@internal
   An address valid for LDR and STR instructions."
  (and (match_code "mem")
       (match_test "riscv_legitimize_address_index_p (XEXP (op, 0), GET_MODE (op), false)")))

(define_memory_constraint "Qmu"
  "@internal
   An address valid for LDUR and STUR instructions."
  (and (match_code "mem")
       (match_test "riscv_legitimize_address_index_p (XEXP (op, 0), GET_MODE (op), true)")))

(define_memory_constraint "Qmb"
  "@internal
   An address valid for LDIB/LDIA and STIB/STIA instructions."
  (and (match_code "mem")
       (match_test "riscv_legitimize_address_modify_p (XEXP (op, 0), GET_MODE (op), false)")))

(define_memory_constraint "Qma"
  "@internal
   An address valid for LDIA and STIA instructions."
  (and (match_code "mem")
       (match_test "riscv_legitimize_address_modify_p (XEXP (op, 0), GET_MODE (op), true)")))

(define_memory_constraint "Qmp"
  "@internal
   An address valid for LDD/SDD instructions."
  (and (match_code "mem")
       (match_test "riscv_legitimize_address_pair_p (XEXP (op, 0), GET_MODE (op), true)")))

;; PREDICATE
;; =========

(define_predicate "const_int_or_reg_operand"
  (ior (match_code "const_int")
       (match_operand 0 "register_operand")))

(define_predicate "const_uns16_arith_operand"
  (and (match_code "const_int")
       (match_test "SMALL_OPERAND_UNSIGNED16 (INTVAL (op))")))

(define_predicate "uns16_arith_operand"
  (ior (match_operand 0 "const_uns16_arith_operand")
       (match_operand 0 "register_operand")))

(define_predicate "const_twobit_operand"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), 0, 3)")))

(define_predicate "const_6bit_operand"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), 0, 63)")))

;; True if the operand is memory reference suitable for a load/store exclusive.
(define_predicate "riscv_sync_memory_operand"
  (and (match_operand 0 "memory_operand")
       (match_code "reg" "0")))

(define_predicate "riscv_mem_pair_operand"
  (and (match_code "mem")
       (match_test "riscv_legitimize_address_pair_p (XEXP (op, 0), GET_MODE (op), true)")))

(define_predicate "const_Pi_operand"
  (and (match_code "const_int")
       (match_test "INTVAL (op) >= 8 && INTVAL (op) <= 56")))

(define_predicate "const_Pj_operand"
  (and (match_code "const_int")
       (match_test "INTVAL (op) >= 4 && INTVAL (op) <= 20")))

(define_predicate "const_K_operand"
  (and (match_code "const_int")
       (match_test "INTVAL (op) >= 0 && INTVAL (op) <= 31")))

(define_predicate "const_M_operand"
  (and (match_code "const_int")
       (match_test "INTVAL (op) >= -16 && INTVAL (op) <= 15")))

(define_predicate "nonimmediate_or_0_operand"
  (ior (match_operand 0 "const_0_operand")
       (match_operand 0 "nonimmediate_operand")))

(define_predicate "riscv_mem_classic_operand"
  (and (match_code "mem")
       (match_test "riscv_legitimize_address_classic_p (XEXP (op, 0), GET_MODE (op))")))

;; MISC
;; ====

(define_code_iterator any_bitwise_butand [ior xor])

;; Main element type used by the insn
(define_attr "emode" "unknown,QI,HI,SI,DI,HF,SF,DF"
  (const_string "unknown"))

;; ADDITION
;; ========

(define_expand "adddi3"
  [(set (match_operand:DI          0 "register_operand" "=r,r")
	(plus:DI (match_operand:DI 1 "register_operand" " r,r")
		 (match_operand:DI 2 "arith_operand"    " r,I")))]
  "TARGET_64BIT || TARGET_XTHEAD_ZPSFOPERAND"
  {
    if (!TARGET_64BIT)
      operands[2] = force_reg (DImode, operands[2]);
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")])

;; SUBTRACTION
;; ===========

(define_expand "subdi3"
  [(set (match_operand:DI           0 "register_operand" "= r")
	(minus:DI (match_operand:DI 1 "reg_or_0_operand" " rj")
		  (match_operand:DI 2 "arith_operand"    "  r")))]
  "TARGET_64BIT || TARGET_XTHEAD_ZPSFOPERAND"
  {
    if (!TARGET_64BIT)
      {
	operands[1] = force_reg (DImode, operands[1]);
	operands[2] = force_reg (DImode, operands[2]);
      }
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")])

;; BIT-FIELD
;; =========

;; TARGET_XTHEAD_EXT

(define_insn "extvdi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	    (sign_extract:DI (match_operand:DI 1 "register_operand" "r")
			     (match_operand 2 "const_6bit_operand" "QcL")
			     (match_operand 3 "const_6bit_operand" "QcL")))]
  "TARGET_XTHEAD_EXT && TARGET_64BIT"
  "ext\t%0,%1,%2+%3-1,%3"
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")])

(define_insn "extzvdi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	    (zero_extract:DI (match_operand:DI 1 "register_operand" "r")
			     (match_operand 2 "const_6bit_operand" "QcL")
			     (match_operand 3 "const_6bit_operand" "QcL")))]
  "TARGET_XTHEAD_EXT && TARGET_64BIT"
  {
    if (INTVAL (operands[3]) == 0 && INTVAL (operands[2]) <= 6)
      {
	operands[4] = GEN_INT ((1 << INTVAL (operands[2])) - 1);
	return "andi\t%0,%1,%4";
      }
    return "extu\t%0,%1,%3+%2-1,%3";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")])

(define_insn "extvsi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(sign_extract:SI (match_operand:SI 1 "register_operand" "r")
			 (match_operand 2 "const_int_operand" "K")
			 (match_operand 3 "const_int_operand" "K")))]
  "TARGET_XTHEAD_EXT && !TARGET_64BIT"
  "ext\t%0,%1,%2+%3-1,%3"
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

(define_insn "extzvsi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(zero_extract:SI (match_operand:SI 1 "register_operand" "r")
			 (match_operand 2 "const_int_operand" "K")
			 (match_operand 3 "const_int_operand" "K")))]
  "TARGET_XTHEAD_EXT && !TARGET_64BIT"
  {
    if (INTVAL (operands[3]) == 0 && INTVAL (operands[2]) <= 6)
      {
	operands[4] = GEN_INT ((1 << INTVAL (operands[2])) - 1);
	return "andi\t%0,%1,%4";
      }
    return "extu\t%0,%1,%3+%2-1,%3";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

(define_insn "*xthead_extu<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(and:X (ashift:X (match_operand:X 1 "register_operand" "r")
			 (match_operand 2 "const_int_operand" "i"))
			 (match_operand 3 "const_int_operand" "i")))]
  "TARGET_XTHEAD_EXT
   && (exact_log2 ((INTVAL (operands[3]) >> INTVAL (operands[2])) + 1) != -1)
   && ((((1 << INTVAL (operands[2])) - 1) & INTVAL (operands[3])) == 0)
   && 0"
  {
    operands[3] = GEN_INT (exact_log2 ((INTVAL (operands[3]) >> INTVAL (operands[2])) + 1)
		  - INTVAL (operands[2]));
    return "extu\t%0,%1,%3,0";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "<MODE>")]
)

;; LOGICAL
;; =======

;; TARGET_XTHEAD_EXT

(define_expand "anddi3"
  [(set (match_operand:DI         0 "register_operand" "")
	(and:DI (match_operand:DI 1 "register_operand" "")
		(match_operand:DI 2 "const_int_or_reg_operand" "")))]
  "TARGET_64BIT"
  "
  {
    if (CONST_INT_P (operands[2]))
      {
	if (!SMALL_OPERAND (INTVAL (operands[2])))
	  {
	    if (TARGET_64BIT && TARGET_XTHEAD_EXT)
	      {
		HOST_WIDE_INT value = INTVAL (operands[2]);
		int i;

		for (i = IMM_BITS; i <= 63; i++)
		  {
		    if (value == (((HOST_WIDE_INT)1 << i) - 1))
		      {
			emit_insn (gen_extzvdi (operands[0], operands[1],
						GEN_INT (i),
						GEN_INT (0)));
			DONE;
		      }
		  }
	      }
	    operands[2] = force_reg (DImode, operands[2]);
	  }
      }
  }
  "
)

(define_insn "*xthead_anddi3"
  [(set (match_operand:DI         0 "register_operand" "=r,r")
	(and:DI (match_operand:DI 1 "register_operand" "%r,r")
		(match_operand:DI 2 "arith_operand"    " r,I")))]
  "TARGET_64BIT"
  "and%i2\t%0,%1,%2"
  [(set_attr "type" "logical")
   (set_attr "mode" "DI")])

(define_insn "andsi3"
  [(set (match_operand:SI         0 "register_operand" "=r,r")
	(and:SI (match_operand:SI 1 "register_operand" "%r,r")
			(match_operand:SI 2 "arith_operand"    " r,I")))]
  "!TARGET_64BIT"
  "and%i2\t%0,%1,%2"
  [(set_attr "type" "logical")
   (set_attr "mode" "SI")])

;; TARGET_XTHEAD_XOR_COMBINE

(define_insn "*xthead_xor<mode>3"
  [(set (match_operand:X        0 "register_operand" "=r,r")
	(xor:X (match_operand:X 1 "register_operand" "%r,r")
	       (match_operand:X 2 "uns16_arith_operand" "r,I")))]
  "TARGET_XTHEAD_XOR_COMBINE"
  "xor%i2\t%0,%1,%2"
  [(set_attr "type" "logical")
   (set_attr "mode" "<MODE>")])

;; ZERO EXTENSION
;; ==============

;; TARGET_XTHEAD_EXT

(define_insn "*xthead_zero_extend<mode>di2"
  [(set (match_operand:DI     0 "register_operand"    "=r")
	(zero_extend:DI
	  (match_operand:HISI 1 "register_operand"    "r")))]
  "TARGET_64BIT && TARGET_XTHEAD_EXT"
  {
    operands[2] = GEN_INT (GET_MODE_BITSIZE (<MODE>mode) - 1);
    return "extu\t%0,%1,%2,0";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")])

(define_insn "*xthead_zero_extendhisi2"
  [(set (match_operand:SI     0 "register_operand"    "=r")
	(zero_extend:SI
	  (match_operand:HI 1 "register_operand"    "r")))]
  "TARGET_XTHEAD_EXT"
  {
    operands[2] = GEN_INT (GET_MODE_BITSIZE (HImode) - 1);
    return "extu\t%0,%1,%2,0";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")])

;; TARGET_XTHEAD_ADDSL

(define_insn "*xthead_indexqi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(plus:DI (zero_extend:DI (match_operand:SI 2 "register_operand" "r"))
		 (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ADDSLUW"
  "addsluw.b\t%0, %1, %2"
  [(set_attr "type" "arith")]
)

(define_insn "*xthead_indexhi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(plus:DI (and:DI (ashift:DI
		   (match_operand:DI 2 "register_operand" "r")
		   (const_int 1)) (const_int 8589934590))
		 (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ADDSLUW"
  "addsluw.h\t%0, %1, %2"
  [(set_attr "type" "arith")]
)

(define_insn "*xthead_indexsi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(plus:DI (and:DI (ashift:DI
		   (match_operand:DI 2 "register_operand" "r")
		   (const_int 2)) (const_int 17179869180))
		 (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ADDSLUW"
  "addsluw.w\t%0, %1, %2"
  [(set_attr "type" "arith")]
)

(define_insn "*xthead_indexdi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(plus:DI (and:DI (ashift:DI
		   (match_operand:DI 2 "register_operand" "r")
		   (const_int 3)) (const_int 34359738360))
		 (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_ADDSLUW"
  "addsluw.d\t%0, %1, %2"
  [(set_attr "type" "arith")]
)

(define_insn "*xthead_addsl<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(plus:X (ashift:X (match_operand:X 2 "register_operand" "r")
			  (match_operand:QI 3 "const_twobit_operand" "i"))
		(match_operand:X 1 "register_operand" "r")))]
   "TARGET_XTHEAD_ADDSL"
   "addsl\t%0, %1, %2, %3"
   [(set_attr "type" "arith")]
)

(define_insn "*xthead_addsl_mult<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(plus:X (mult:X (match_operand:X 2 "register_operand" "r")
			(match_operand:X 3 "const_int_operand" "i"))
		(match_operand:X 1 "register_operand" "r")))]
   "TARGET_XTHEAD_ADDSL
    && CONST_INT_P (operands[3])
    && IN_RANGE (exact_log2 (INTVAL (operands[3])), 0, 3)"
   {
    operands[3] = gen_rtx_CONST_INT (<MODE>mode,
				     exact_log2 (INTVAL (operands[3])));
    return "addsl\t%0, %1, %2, %3";
   }
  [(set_attr "type" "arith")]
)

;; SIGN EXTENSION
;; ==============

;; TARGET_XTHEAD_EXT

(define_insn "*xthead_extend<SHORT:mode><X:mode>2"
  [(set (match_operand:X       0 "register_operand"    "=r")
	(sign_extend:X
	  (match_operand:SHORT  1 "register_operand"    "r")))]
  "TARGET_XTHEAD_EXT"
  {
    operands[2] = GEN_INT (GET_MODE_BITSIZE (<SHORT:MODE>mode) - 1);
    return "ext\t%0,%1,%2,0";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "<X:MODE>")])

;; CONDITIONAL BRANCHES
;; ====================

;; TARGET_XTHEAD_EXT

(define_insn_and_split "*xthead_branch_extend_zerocmphi"
  [(set (pc)
	(if_then_else
	 (match_operator 2 "signed_order_operator"
			 [(lshiftrt:DI (match_operand:DI 0 "register_operand" "r")
				       (match_operand:DI 1 "const_int_operand" "i"))
			  (const_int 0)])
	 (label_ref (match_operand 3 "" ""))
	 (pc)))
   (clobber (match_scratch:DI 4 "=&r"))]
  "TARGET_64BIT && TARGET_XTHEAD_EXT"
  "#"
  "reload_completed"
  [(set (match_dup 4)
	(zero_extract:DI (match_dup 0)
			 (match_operand:DI 5 "const_int_operand" "i")
			 (match_dup 1)))
   (set (pc)
	 (if_then_else
	  (match_op_dup 2 [(match_dup 4) (const_int 0)])
	  (label_ref (match_operand 3 "" ""))
	  (pc)))]
  "operands[5] = (GEN_INT (64 - INTVAL (operands[1])));"
)

;; MULTIPLY ACCUMULATION
;; =====================

;; TARGET_XTHEAD_MULA

(define_insn "*xthead_madd<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	      (plus:X (mult:X (match_operand:X 1 "register_operand" "r")
			      (match_operand:X 2 "register_operand" "r"))
		      (match_operand:X 3 "register_operand" "0")))]
  "TARGET_MUL && TARGET_XTHEAD_MULA"
  "mula\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "<MODE>")]
)

(define_insn "*xthead_msub<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	      (minus:X (match_operand:X 3 "register_operand" "0")
		       (mult:X (match_operand:X 1 "register_operand" "r")
			       (match_operand:X 2 "register_operand" "r"))))]
  "TARGET_MUL && TARGET_XTHEAD_MULA"
  "muls\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "<MODE>")]
)

(define_insn "*xthead_maddsi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (plus:SI (mult:SI (match_operand:SI 1 "register_operand" "r")
			    (match_operand:SI 2 "register_operand" "r"))
		   (match_operand:SI 3 "register_operand" "0"))))]
  "TARGET_MUL && TARGET_XTHEAD_MULA && TARGET_64BIT"
  "mulaw\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

(define_insn "*xthead_maddsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	      (plus:SI (mult:SI (match_operand:SI 1 "register_operand" "r")
				(match_operand:SI 2 "register_operand" "r"))
		       (match_operand:SI 3 "register_operand" "0")))]
  "TARGET_MUL && TARGET_XTHEAD_MULA && TARGET_64BIT"
  "mulaw\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

(define_insn "*xthead_msubsi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (minus:SI (match_operand:SI 3 "register_operand" "0")
		    (mult:SI (match_operand:SI 1 "register_operand" "r")
			     (match_operand:SI 2 "register_operand" "r")))))]
  "TARGET_MUL && TARGET_XTHEAD_MULA && TARGET_64BIT"
  "mulsw\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

(define_insn "*xthead_msubsi"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI (match_operand:SI 3 "register_operand" "0")
		  (mult:SI (match_operand:SI 1 "register_operand" "r")
			   (match_operand:SI 2 "register_operand" "r"))))]
  "TARGET_MUL && TARGET_XTHEAD_MULA && TARGET_64BIT"
  "mulsw\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

(define_insn "maddhisi4"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand" " r"))
			  (sign_extend:SI (match_operand:HI 2 "register_operand" " r")))
		 (match_operand:SI 3 "register_operand" " 0")))]
  "TARGET_MUL && TARGET_XTHEAD_MULA"
  "mulah\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

(define_insn "*xthead_sextw_maddhisi4"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (plus:SI (mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand" " r"))
			    (sign_extend:SI (match_operand:HI 2 "register_operand" " r")))
		   (match_operand:SI 3 "register_operand" " 0"))))]
  "TARGET_MUL && TARGET_XTHEAD_MULA && TARGET_64BIT"
  "mulah\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

(define_insn "msubhisi4"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI (match_operand:SI 3 "register_operand" " 0")
		  (mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand" " r"))
			   (sign_extend:SI (match_operand:HI 2 "register_operand" " r")))))]
  "TARGET_MUL && TARGET_XTHEAD_MULA"
  "mulsh\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

(define_insn "*xthead_sextw_msubhisi4"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (minus:SI (match_operand:SI 3 "register_operand" " 0")
		    (mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand" " r"))
			     (sign_extend:SI (match_operand:HI 2 "register_operand" " r"))))))]
  "TARGET_MUL && TARGET_XTHEAD_MULA && TARGET_64BIT"
  "mulsh\\t%0,%1,%2"
  [(set_attr "type" "imul")
   (set_attr "mode" "SI")]
)

;; BYTE SWAP
;; =========

;; TARGET_XTHEAD_REV

(define_expand "xthead_bswapsi2"
  [(set (match_operand:SI           0 "register_operand")
	(bswap:SI (match_operand:SI 1 "register_operand")))]
  "TARGET_XTHEAD_REVW || (TARGET_XTHEAD_REV && !TARGET_64BIT)")

(define_insn "*bswapsi2"
  [(set (match_operand:SI           0 "register_operand" "=r")
	(bswap:SI (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_XTHEAD_REVW || (TARGET_XTHEAD_REV && !TARGET_64BIT)"
  {
    return TARGET_XTHEAD_REVW ? "revw\t%0, %1" : "rev\t%0, %1";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "SI")]
)

(define_expand "xthead_bswapdi2"
  [(set (match_operand:DI           0 "register_operand")
	(bswap:DI (match_operand:DI 1 "register_operand")))]
  "TARGET_64BIT && TARGET_XTHEAD_REV")

(define_insn "*bswapdi2"
  [(set (match_operand:DI           0 "register_operand" "=r")
	(bswap:DI (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_64BIT && TARGET_XTHEAD_REV"
  "rev\t%0, %1"
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")]
)

;; SHIFTS
;; ======

;; TARGET_XTHEAD_SRRI

(define_expand "xthead_rotrsi3"
  [(set (match_operand:SI              0 "register_operand")
	(rotatert:SI (match_operand:SI 1 "register_operand")
		     (match_operand:SI 2 "const_int_operand")))]
  "(TARGET_64BIT && TARGET_XTHEAD_SRRIW)
   || (TARGET_XTHEAD_SRRI && !TARGET_64BIT)")

(define_insn "*rotrsi3"
  [(set (match_operand:SI              0 "register_operand"     "=r")
	(rotatert:SI (match_operand:SI 1 "register_operand"     "r")
		     (match_operand:SI 2 "const_int_operand"    "QcL")))]
  "(TARGET_64BIT && TARGET_XTHEAD_SRRIW)
   || (TARGET_XTHEAD_SRRI && !TARGET_64BIT)"
  {
    return TARGET_64BIT ? "srriw\t%0, %1, %2" : "srri\t%0, %1, %2";
  }
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")]
)

(define_expand "xthead_rotrdi3"
  [(set (match_operand:DI              0 "register_operand")
	(rotatert:DI (match_operand:DI 1 "register_operand")
		     (match_operand:DI 2 "const_int_operand")))]
  "TARGET_64BIT && TARGET_XTHEAD_SRRI")

(define_insn "*rotrdi3"
  [(set (match_operand:DI              0 "register_operand"     "=r")
	(rotatert:DI (match_operand:DI 1 "register_operand"     "r")
		     (match_operand:DI 2 "const_int_operand"    "QcL")))]
  "TARGET_64BIT && TARGET_XTHEAD_SRRI"
  "srri\t%0, %1, %2"
  [(set_attr "type" "arith")
   (set_attr "mode" "DI")]
)

;; CONDITIONAL MOVE
;; ================

;; TARGET_XTHEAD_CONDMV

;; TODO This cannot be generated unless the operand 1 has no mode,
;;      but it will increase the dynamic instruction count in case
;;      of GCC-1367
(define_expand "mov<GPR:mode>cc"
  [(set (match_operand:GPR 0 "register_operand" "")
	(if_then_else:GPR (match_operand:DI 1 "order_operator" "")
			  (match_operand:GPR 2 "reg_or_0_operand" "")
			  (match_operand:GPR 3 "register_operand" "")))]
  "TARGET_XTHEAD_CONDMV && !optimize_size"
{
  int result = riscv_expand_movcc (operands);
  switch (result)
    {
    case 0:
      DONE;
      break;
    case -1:
      FAIL;
      break;
    case 1:
      break;
    default:
      gcc_unreachable ();
    }
})

(define_insn_and_split "*xthead_cmov<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(if_then_else:GPR (match_operator 1 "order_operator"
			     [(match_operand:X 2 "register_operand" "r")
			      (match_operand:X 3 "register_operand" "r")])
			  (match_operand:GPR 4 "reg_or_0_operand" "rJ")
			  (match_operand:GPR 5 "register_operand" "r")))]
  "TARGET_XTHEAD_CONDMV
   && !reload_completed"
  "#"
  "&& true"
  [(const_int 0)]
  {
    emit_insn (gen_mov<GPR:mode>cc (operands[0], operands[1],
				    operands[4], operands[5]));
    DONE;
  }
)

(define_insn "*xthead_cmovz<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand"                     "=r, r")
	(if_then_else:GPR (eq (match_operand:X 1 "register_operand" " r, r")
			      (const_int 0))
			  (match_operand:GPR 2 "reg_or_0_operand"    "rJ, 0")
			  (match_operand:GPR 3 "register_operand"    " 0, r")))]
  "TARGET_XTHEAD_CONDMV"
  "@
   mveqz\t%0, %z2, %1
   mvnez\t%0, %3, %1"
  [(set_attr "type" "arith")]
)

(define_insn "*xthead_cmovn<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand"                         "=r, r")
	    (if_then_else:GPR (ne (match_operand:X 1 "register_operand" " r, r")
				  (const_int 0))
			      (match_operand:GPR 2 "reg_or_0_operand"   "rJ, 0")
			      (match_operand:GPR 3 "register_operand"   " 0, r")))]
  "TARGET_XTHEAD_CONDMV"
  "@
   mvnez\t%0, %z2, %1
   mveqz\t%0, %3, %1"
  [(set_attr "type" "arith")]
)

;; MISC
;; ====

;; TARGET_XTHEAD_LDD

;; double store
(define_peephole2
  [(set (match_operand:DI 0 "riscv_mem_pair_operand" "")
	(match_operand:DI 1 "register_operand" ""))
   (set (match_operand:DI 2 "memory_operand" "")
	(match_operand:DI 3 "register_operand" ""))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
  && rtx_equal_p (XEXP (operands[2], 0),
      plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (DImode)))
  && GET_CODE (XEXP (operands[2], 0)) == PLUS
  && (INTVAL (XEXP (XEXP (operands[2], 0), 1)) - 8) <= 48
  && (INTVAL (XEXP (XEXP (operands[2], 0), 1)) - 8) >= 0"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)

(define_peephole2
  [(set (match_operand:SI 0 "riscv_mem_pair_operand" "")
	(match_operand:SI 1 "register_operand" ""))
   (set (match_operand:SI 2 "memory_operand" "")
	(match_operand:SI 3 "register_operand" ""))]
  "TARGET_XTHEAD_LDD
  && rtx_equal_p (XEXP (operands[2], 0),
      plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (SImode)))
  && GET_CODE (XEXP (operands[2], 0)) == PLUS
  && (INTVAL (XEXP (XEXP (operands[2], 0), 1)) - 4) <= 16
  && (INTVAL (XEXP (XEXP (operands[2], 0), 1)) - 4) >= 0"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)

;; SDD
(define_insn "store_pairdi"
  [(set (match_operand:DI 0 "riscv_mem_pair_operand" "=Qmp")
	(match_operand:DI 1 "register_operand"       "r"))
   (set (mem:DI (plus:DI (match_operand:DI 2 "register_operand"      "r")
			 (match_operand  3 "const_Pi_operand"     "Pi")))
	(match_operand:DI 4 "register_operand"       "r"))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
   && rtx_equal_p (plus_constant (Pmode, operands[2], INTVAL (operands[3])),
      plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (DImode)))"
  "sdd\t%1,%4,(%2),%j3,4"
  [(set_attr "type" "store")
   (set_attr "mode" "DI")])

;; SWD
(define_insn "*store_pairsi<mode>"
  [(set (match_operand:SI 0 "riscv_mem_pair_operand" "=Qmp")
	(match_operand:SI 1 "register_operand"       "r"))
   (set (mem:SI (plus:X (match_operand:X 2 "register_operand"      "r")
			 (match_operand 3 "const_Pj_operand"     "Pj")))
	(match_operand:SI 4 "register_operand"       "r"))]
  "TARGET_XTHEAD_LDD
  && rtx_equal_p (plus_constant (Pmode, operands[2], INTVAL (operands[3])),
      plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (SImode)))"
  "swd\t%1,%4,(%2),%k3,3"
  [(set_attr "type" "store")
   (set_attr "mode" "SI")])

;; double load
(define_peephole2
  [(set (match_operand:DI 0 "register_operand" "")
	(match_operand:DI 1 "riscv_mem_pair_operand" ""))
   (set (match_operand:DI 2 "register_operand" "")
	(match_operand:DI 3 "memory_operand" ""))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
  && REGNO (operands[0]) != REGNO (operands[2])
  && rtx_equal_p (XEXP (operands[3], 0),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (DImode)))
  && GET_CODE (XEXP (operands[3], 0)) == PLUS
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 8) <= 48
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 8) >= 0
  && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
  && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)

(define_peephole2
  [(set (match_operand:DI 0 "register_operand" "")
	(sign_extend:DI (match_operand:SI 1 "riscv_mem_pair_operand" "")))
   (set (match_operand:DI 2 "register_operand" "")
	(sign_extend:DI (match_operand:SI 3 "memory_operand" "")))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
  && REGNO (operands[0]) != REGNO (operands[2])
  && rtx_equal_p (XEXP (operands[3], 0),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
  && GET_CODE (XEXP (operands[3], 0)) == PLUS
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 4) <= 16
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 4) >= 0
  && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
  && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (sign_extend:DI (match_dup 1)))
	      (set (match_dup 2) (sign_extend:DI (match_dup 3)))])]
)

(define_peephole2
  [(set (match_operand:DI 0 "register_operand" "")
	(zero_extend:DI (match_operand:SI 1 "riscv_mem_pair_operand" "")))
   (set (match_operand:DI 2 "register_operand" "")
	(zero_extend:DI (match_operand:SI 3 "memory_operand" "")))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
  && REGNO (operands[0]) != REGNO (operands[2])
  && rtx_equal_p (XEXP (operands[3], 0),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
  && GET_CODE (XEXP (operands[3], 0)) == PLUS
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 4) <= 16
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 4) >= 0
  && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
  && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (zero_extend:DI (match_dup 1)))
	      (set (match_dup 2) (zero_extend:DI (match_dup 3)))])]
)

(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "riscv_mem_pair_operand" ""))
   (set (match_operand:SI 2 "register_operand" "")
	(match_operand:SI 3 "memory_operand" ""))]
  "!TARGET_64BIT && TARGET_XTHEAD_LDD
  && REGNO (operands[0]) != REGNO (operands[2])
  && rtx_equal_p (XEXP (operands[3], 0),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
  && GET_CODE (XEXP (operands[3], 0)) == PLUS
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 4) <= 16
  && (INTVAL (XEXP (XEXP (operands[3], 0), 1)) - 4) >= 0
  && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
  && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)

;; LDD
(define_insn "load_pairdi"
  [(set (match_operand:DI 0 "register_operand"        "=r")
	(match_operand:DI 1 "riscv_mem_pair_operand"  "Qmp"))
   (set (match_operand:DI 2 "register_operand"        "=r")
	(mem:DI (plus:DI (match_operand:DI 3 "register_operand"      "r")
			 (match_operand  4 "const_Pi_operand"     "Pi"))))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
  && rtx_equal_p (plus_constant (Pmode, operands[3], INTVAL (operands[4])),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (DImode)))
  && REGNO (operands[0]) != REGNO (operands[3])
  && REGNO (operands[2]) != REGNO (operands[3])"
  "ldd\t%0,%2,(%3),%j4,4"
  [(set_attr "type" "load")
   (set_attr "mode" "DI")])

;; LWD
(define_insn "*load_pair_extendsidi"
  [(set (match_operand:DI 0 "register_operand"        "=r")
	(sign_extend:DI (match_operand:SI 1 "riscv_mem_pair_operand"  "Qmp")))
   (set (match_operand:DI 2 "register_operand"        "=r")
	(sign_extend:DI (mem:SI (plus:DI (match_operand:DI 3 "register_operand"      "r")
					 (match_operand 4 "const_Pj_operand"     "Pj")))))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
   && rtx_equal_p (plus_constant (Pmode, operands[3], INTVAL (operands[4])),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && REGNO (operands[0]) != REGNO (operands[3])
   && REGNO (operands[2]) != REGNO (operands[3])"
  "lwd\t%0,%2,(%3),%k4,3"
  [(set_attr "type" "load")
   (set_attr "mode" "SI")])

(define_insn "load_pairsi"
  [(set (match_operand:SI 0 "register_operand"        "=r")
	(match_operand:SI 1 "riscv_mem_pair_operand"  "Qmp"))
   (set (match_operand:SI 2 "register_operand"        "=r")
	(mem:SI (plus:SI (match_operand:SI 3 "register_operand"      "r")
			 (match_operand 4 "const_Pj_operand"     "Pj"))))]
  "!TARGET_64BIT && TARGET_XTHEAD_LDD
   && rtx_equal_p (plus_constant (Pmode, operands[3], INTVAL (operands[4])),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && REGNO (operands[0]) != REGNO (operands[3])
   && REGNO (operands[2]) != REGNO (operands[3])"
  "lwd\t%0,%2,(%3),%k4,3"
  [(set_attr "type" "load")
   (set_attr "mode" "SI")])

;; LWUD
(define_insn "*load_pair_zero_extendsidi"
  [(set (match_operand:DI 0 "register_operand"        "=r")
	(zero_extend:DI (match_operand:SI 1 "riscv_mem_pair_operand"  "Qmp")))
   (set (match_operand:DI 2 "register_operand"        "=r")
	(zero_extend:DI (mem:SI (plus:DI (match_operand:DI 3 "register_operand"      "r")
					 (match_operand  4 "const_Pj_operand"     "Pj")))))]
  "TARGET_XTHEAD_LDD && TARGET_64BIT
   && rtx_equal_p (plus_constant (Pmode, operands[3], INTVAL (operands[4])),
      plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
  && REGNO (operands[0]) != REGNO (operands[3])
  && REGNO (operands[2]) != REGNO (operands[3])"
  "lwud\t%0,%2,(%3),%k4,3"
  [(set_attr "type" "load")
   (set_attr "mode" "SI")])

;; Builtins
;; ========

(define_c_enum "unspecv" [
  ;; Interrupt handler instructions.
  UNSPECV_IPUSH
  UNSPECV_IPOP
])

;; TARGET_XTHEAD_IPUSH

(define_insn "riscv_ipop"
  [(return)
   (unspec_volatile [(const_int 0)] UNSPECV_IPOP)]
  "TARGET_XTHEAD_IPUSH"
  "ipop")

(define_insn "riscv_ipush"
  [(unspec_volatile [(const_int 0)] UNSPECV_IPUSH)
   (use (reg RETURN_ADDR_REGNUM))
   (use (reg T0_REGNUM))
   (use (reg T1_REGNUM))
   (use (reg T2_REGNUM))
   (use (reg A0_REGNUM))
   (use (reg A1_REGNUM))
   (use (reg A2_REGNUM))
   (use (reg A3_REGNUM))
   (use (reg A4_REGNUM))
   (use (reg A5_REGNUM))
   (use (reg A6_REGNUM))
   (use (reg A7_REGNUM))
   (use (reg T3_REGNUM))
   (use (reg T4_REGNUM))
   (use (reg T5_REGNUM))
   (use (reg T6_REGNUM))]
  "TARGET_XTHEAD_IPUSH"
  "ipush")


;; C908 has a special acceleration for div and mod. When a div/mod instruction
;; is given after another mod/div and they use a common divisor and divisor,
;; only 2 extra cycles are needed for this current div/mod instruction.
;; To implement this features, we treat the div and mod pair as a single
;; instruction and use the SPN "divmodm4" to cover it.

(define_code_iterator any_divmod_div [div udiv])
(define_code_attr any_divmod_mod [(div "mod") (udiv "umod")])
(define_code_attr any_divmod_u [(div "") (udiv "u")])
(define_code_attr any_divmod_extend [(div "sign_extend") (udiv "zero_extend")])

(define_insn "<any_divmod_u>divmodsi4"
  [(set (match_operand:SI                    0 "register_operand" "=&r")
	(any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			   (match_operand:SI 2 "register_operand" " r")))
   (set (match_operand:SI                    3 "register_operand" "=r")
	(<any_divmod_mod>:SI (match_dup 1)
			   (match_dup 2)))]
  "TARGET_DIV && riscv_microarchitecture == c908"
  { return TARGET_64BIT ?
    "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"
    : "div<any_divmod_u>\t%0,%1,%2\n\t" "rem<any_divmod_u>\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "SI")])

(define_insn "<any_divmod_u>divmoddi4"
  [(set (match_operand:DI                    0 "register_operand" "=&r")
	(any_divmod_div:DI (match_operand:DI 1 "register_operand" " r")
			   (match_operand:DI 2 "register_operand" " r")))
   (set (match_operand:DI                    3 "register_operand" "=r")
	(<any_divmod_mod>:DI (match_dup 1)
			   (match_dup 2)))]
  "TARGET_DIV && TARGET_64BIT && riscv_microarchitecture == c908"
  { return "div<any_divmod_u>\t%0,%1,%2\n\t" "rem<any_divmod_u>\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])

(define_insn "*divmodsi4_extend1"
  [(set (match_operand:DI                        0 "register_operand" "=&r")
	(<any_divmod_extend>:DI
	    (any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			       (match_operand:SI 2 "register_operand" " r"))))
   (set (match_operand:DI                        3 "register_operand" "=r")
	(<any_divmod_extend>:DI
	    (<any_divmod_mod>:SI (match_dup 1)
				 (match_dup 2))))]
  "TARGET_DIV && TARGET_64BIT && riscv_microarchitecture == c908"
  { return "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])

(define_insn "*divmodsi4_extend2"
  [(set (match_operand:DI                        0 "register_operand" "=&r")
	(<any_divmod_extend>:DI
	    (any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			       (match_operand:SI 2 "register_operand" " r"))))
   (set (match_operand:SI                        3 "register_operand" "=r")
	(<any_divmod_mod>:SI (match_dup 1)
			     (match_dup 2)))]
  "TARGET_DIV && TARGET_64BIT && riscv_microarchitecture == c908"
  { return "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])

(define_insn "*divmodsi4_extend3"
  [(set (match_operand:SI                    0 "register_operand" "=&r")
	(any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			   (match_operand:SI 2 "register_operand" " r")))
   (set (match_operand:DI                    3 "register_operand" "=r")
	(<any_divmod_extend>:DI
	    (<any_divmod_mod>:SI (match_dup 1)
				 (match_dup 2))))]
  "TARGET_DIV && TARGET_64BIT && riscv_microarchitecture == c908"
  { return "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])


;; C908 fuse load/store pair for si/di mode.

(define_peephole2
  [(set (match_operand:DI 0 "register_operand" "")
	(match_operand:DI 1 "riscv_mem_classic_operand" ""))
   (set (match_operand:DI 2 "register_operand" "")
	(match_operand:DI 3 "riscv_mem_classic_operand" ""))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && REGNO (operands[0]) != REGNO (operands[2])
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (DImode)))
   && GET_CODE (XEXP (operands[3], 0)) == PLUS
   && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
   && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)

(define_peephole2
  [(set (match_operand:DI 0 "register_operand" "")
	(sign_extend:DI (match_operand:SI 1 "riscv_mem_classic_operand" "")))
   (set (match_operand:DI 2 "register_operand" "")
	(sign_extend:DI (match_operand:SI 3 "riscv_mem_classic_operand" "")))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && REGNO (operands[0]) != REGNO (operands[2])
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && GET_CODE (XEXP (operands[3], 0)) == PLUS
   && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
   && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (sign_extend:DI (match_dup 1)))
	      (set (match_dup 2) (sign_extend:DI (match_dup 3)))])]
)

(define_peephole2
  [(set (match_operand:DI 0 "register_operand" "")
	(zero_extend:DI (match_operand:SI 1 "riscv_mem_classic_operand" "")))
   (set (match_operand:DI 2 "register_operand" "")
	(zero_extend:DI (match_operand:SI 3 "riscv_mem_classic_operand" "")))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && REGNO (operands[0]) != REGNO (operands[2])
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && GET_CODE (XEXP (operands[3], 0)) == PLUS
   && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
   && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (zero_extend:DI (match_dup 1)))
	      (set (match_dup 2) (zero_extend:DI (match_dup 3)))])]
)

(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "riscv_mem_classic_operand" ""))
   (set (match_operand:SI 2 "register_operand" "")
	(match_operand:SI 3 "riscv_mem_classic_operand" ""))]
  "riscv_microarchitecture == c908
   && REGNO (operands[0]) != REGNO (operands[2])
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && GET_CODE (XEXP (operands[3], 0)) == PLUS
   && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
   && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)

(define_peephole2
  [(set (match_operand:DI 0 "riscv_mem_classic_operand" "")
	(match_operand:DI 1 "register_operand" ""))
   (set (match_operand:DI 2 "riscv_mem_classic_operand" "")
	(match_operand:DI 3 "register_operand" ""))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && REGNO (operands[1]) != REGNO (operands[3])
   && rtx_equal_p (XEXP (operands[2], 0),
	plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (DImode)))
   && GET_CODE (XEXP (operands[2], 0)) == PLUS
   && REGNO (operands[1]) != REGNO (XEXP (XEXP (operands[2], 0), 0))
   && REGNO (operands[3]) != REGNO (XEXP (XEXP (operands[2], 0), 0))"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)

(define_peephole2
  [(set (match_operand:SI 0 "riscv_mem_classic_operand" "")
	(match_operand:SI 1 "register_operand" ""))
   (set (match_operand:SI 2 "riscv_mem_classic_operand" "")
	(match_operand:SI 3 "register_operand" ""))]
  "riscv_microarchitecture == c908
   && REGNO (operands[1]) != REGNO (operands[3])
   && rtx_equal_p (XEXP (operands[2], 0),
	plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (SImode)))
   && GET_CODE (XEXP (operands[2], 0)) == PLUS
   && REGNO (operands[1]) != REGNO (XEXP (XEXP (operands[2], 0), 0))
   && REGNO (operands[3]) != REGNO (XEXP (XEXP (operands[2], 0), 0))"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (set (match_dup 2) (match_dup 3))])]
)


;; Load di pair
(define_insn "fused_load_pairdi"
  [(set (match_operand:DI 0 "register_operand"           "=r")
	(match_operand:DI 1 "riscv_mem_classic_operand"  "m"))
   (set (match_operand:DI 2 "register_operand"           "=r")
	(match_operand:DI 3 "riscv_mem_classic_operand"  "m"))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (DImode)))
   && GET_CODE (XEXP (operands[3], 0)) == PLUS
   && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
   && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  "ld\t%0,%1\n\tld\t%2,%3"
  [(set_attr "type" "load")
   (set_attr "mode" "DI")
   (set_attr "length" "8")])

;; Load si pair
(define_insn "*fused_load_pair_extendsidi"
  [(set (match_operand:DI                 0 "register_operand"           "=r")
	(sign_extend:DI (match_operand:SI 1 "riscv_mem_classic_operand"  "m")))
   (set (match_operand:DI                 2 "register_operand"           "=r")
	(sign_extend:DI (match_operand:SI 3 "riscv_mem_classic_operand"  "m")))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && GET_CODE (XEXP (operands[3], 0)) == PLUS
   && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
   && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  "lw\t%0,%1\n\tlw\t%2,%3"
  [(set_attr "type" "load")
   (set_attr "mode" "SI")
   (set_attr "length" "8")])

(define_insn "fused_load_pairsi"
  [(set (match_operand:SI 0 "register_operand"           "=r")
	(match_operand:SI 1 "riscv_mem_classic_operand"  "m"))
   (set (match_operand:SI 2 "register_operand"           "=r")
	(match_operand:SI 3 "riscv_mem_classic_operand"  "m"))]
  "riscv_microarchitecture == c908
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
   && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  "lw\t%0,%1\n\tlw\t%2,%3"
  [(set_attr "type" "load")
   (set_attr "mode" "SI")
   (set_attr "length" "8")])

;; Load si pair and zeor extend.
(define_insn "*fused_load_pair_zero_extendsidi"
  [(set (match_operand:DI                 0 "register_operand"           "=r")
	(zero_extend:DI (match_operand:SI 1 "riscv_mem_classic_operand"  "m")))
   (set (match_operand:DI                 2 "register_operand"           "=r")
	(zero_extend:DI (match_operand:SI 3 "riscv_mem_classic_operand"  "m")))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && rtx_equal_p (XEXP (operands[3], 0),
	plus_constant (Pmode, XEXP (operands[1], 0), GET_MODE_SIZE (SImode)))
   && GET_CODE (XEXP (operands[3], 0)) == PLUS
    && REGNO (operands[0]) != REGNO (XEXP (XEXP (operands[3], 0), 0))
    && REGNO (operands[2]) != REGNO (XEXP (XEXP (operands[3], 0), 0))"
  "lwu\t%0,%1\n\tlwu\t%2,%3"
  [(set_attr "type" "load")
   (set_attr "mode" "SI")
   (set_attr "length" "8")])

;; Store di pair
(define_insn "fused_store_pairdi"
  [(set (match_operand:DI 0 "riscv_mem_classic_operand"  "=m")
	(match_operand:DI 1 "register_operand"           "r"))
   (set (match_operand:DI 2 "riscv_mem_classic_operand"  "=m")
	(match_operand:DI 3 "register_operand"           "r"))]
  "TARGET_64BIT && riscv_microarchitecture == c908
   && rtx_equal_p (XEXP (operands[2], 0),
	plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (DImode)))
   && GET_CODE (XEXP (operands[2], 0)) == PLUS
   && REGNO (operands[1]) != REGNO (XEXP (XEXP (operands[2], 0), 0))
   && REGNO (operands[3]) != REGNO (XEXP (XEXP (operands[2], 0), 0))"
  "sd\t%1,%0\n\tsd\t%3,%2"
  [(set_attr "type" "store")
   (set_attr "mode" "DI")
   (set_attr "length" "8")])

;; Store di pair
(define_insn "fused_store_pairsi"
  [(set (match_operand:SI 0 "riscv_mem_classic_operand"  "=m")
	(match_operand:SI 1 "register_operand"           "r"))
   (set (match_operand:SI 2 "riscv_mem_classic_operand"  "=m")
	(match_operand:SI 3 "register_operand"           "r"))]
  "riscv_microarchitecture == c908
   && rtx_equal_p (XEXP (operands[2], 0),
	plus_constant (Pmode, XEXP (operands[0], 0), GET_MODE_SIZE (SImode)))
   && GET_CODE (XEXP (operands[2], 0)) == PLUS
   && REGNO (operands[1]) != REGNO (XEXP (XEXP (operands[2], 0), 0))
   && REGNO (operands[3]) != REGNO (XEXP (XEXP (operands[2], 0), 0))"
  "sw\t%1,%0\n\tsw\t%3,%2"
  [(set_attr "type" "store")
   (set_attr "mode" "SI")
   (set_attr "length" "8")])

;; RV32
;; ====

(define_insn "*movsf_hardfloat"
  [(set (match_operand:SF 0 "nonimmediate_operand" "=f,f,f,m,Qmx,*f,*r,  *r,*r,*Qmx")
	(match_operand:SF 1 "move_operand"         " f,G,m,f,G,*r,*f,*G*r,*Qmx,*r"))]
  "!TARGET_64BIT
   && TARGET_XTHEAD_FLDR
   && (register_operand (operands[0], SFmode)
       || reg_or_0_operand (operands[1], SFmode))"
  { return riscv_output_move (operands[0], operands[1]); }
  [(set_attr "move_type" "fmove,mtc,fpload,fpstore,store,mtc,mfc,move,load,store")
   (set_attr "mode" "SF")])

(define_insn "*movdf_hardfloat_rv32"
  [(set (match_operand:DF 0 "nonimmediate_operand" "=f,f,f,m,Qmx,*Qrf,*Qrx,  *r,*r,*Qmx")
	(match_operand:DF 1 "move_operand"         " f,G,m,f,G,*Qrx,*Qrf,*r*G,*Qmx,*r"))]
  "!TARGET_64BIT && TARGET_DOUBLE_FLOAT
   && TARGET_XTHEAD_FLDR
   && (register_operand (operands[0], DFmode)
       || reg_or_0_operand (operands[1], DFmode))"
  { return riscv_output_move (operands[0], operands[1]); }
  [(set_attr "move_type" "fmove,mtc,fpload,fpstore,store,mtc,mfc,move,load,store")
   (set_attr "mode" "DF")])

(include "c906v.md")
(include "c910.md")
