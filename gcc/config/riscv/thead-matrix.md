
;; Machine description for Thead Matrix Extension for GNU compiler.

;; Copyright (C) 2022-2024 Free Software Foundation, Inc.
;; Contributed by Jin Ma <jinma@linux.alibaba.com>,
;; Alibaba Damo Academy Xuantie Team.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_c_enum "unspec" [
  UNSPEC_SET_MSIZE
  UNSPEC_USE_MSIZE
  UNSPEC_MUNDEF
  UNSPEC_MRELEASE
  UNSPEC_MZERO
  UNSPEC_MREAD_XMLENB
  UNSPEC_MREAD_XRLENB
  UNSPEC_MREAD_XMSIZE
  UNSPEC_MLD_MST
  UNSPEC_MSLD_MSST
  UNSPEC_MMOVE
  UNSPEC_MDUP
  UNSPEC_MADD
  UNSPEC_MSUB
  UNSPEC_MSRA
  UNSPEC_MN4CLIP
  UNSPEC_MN4CLIPU
  UNSPEC_MMUL
  UNSPEC_MMULH
  UNSPEC_FMMACC
  UNSPEC_FWMMACC
  UNSPEC_MMAQASS
  UNSPEC_MMAQAUU
  UNSPEC_MMAQAUS
  UNSPEC_MMAQASU
  UNSPEC_PMMAQASS
  UNSPEC_PMMAQAUU
  UNSPEC_PMMAQAUS
  UNSPEC_PMMAQASU
  UNSPEC_MREINTERPRET
  UNSPEC_MSET_TUPLE
  UNSPEC_MGET_TUPLE
])

(define_constants [
   (INVALID_ATTRIBUTE            255)
   (X0_REGNUM                      0)
])

(define_mode_iterator MMODES [
  RVMM1QI RVMM2QI RVMM4QI RVMM8QI
  RVMM1HI RVMM2HI RVMM4HI RVMM8HI
  RVMM1SI RVMM2SI RVMM4SI RVMM8SI
  RVMM1DI RVMM2DI RVMM4DI RVMM8DI

  RVMM1HF RVMM2HF RVMM4HF RVMM8HF
  RVMM1SF RVMM2SF RVMM4SF RVMM8SF
  RVMM1DF RVMM2DF RVMM4DF RVMM8DF
])

(define_mode_iterator M1MODES [
  RVMM1QI RVMM1HI RVMM1SI RVMM1DI
  RVMM1HF RVMM1SF RVMM1DF
])

(define_mode_iterator M1MODES_ALIAS [
  RVMM1QI RVMM1HI RVMM1SI RVMM1DI
  RVMM1HF RVMM1SF RVMM1DF
])

(define_mode_iterator M2MODES [
  RVMM2QI RVMM2HI RVMM2SI RVMM2DI
  RVMM2HF RVMM2SF RVMM2DF
])

(define_mode_iterator M2MODES_ALIAS [
  RVMM2QI RVMM2HI RVMM2SI RVMM2DI
  RVMM2HF RVMM2SF RVMM2DF
])

(define_mode_iterator I_M1MODES [
  RVMM1QI RVMM1HI RVMM1SI RVMM1DI
])

(define_mode_iterator F_M1MODES [
  RVMM1HF RVMM1SF RVMM1DF
])

(define_mode_iterator F_M2MODES [
  RVMM2HF RVMM2SF RVMM2DF
])

(define_mode_attr MEL [
  (RVMM1QI "QI") (RVMM2QI "QI") (RVMM4QI "QI") (RVMM8QI "QI")
  (RVMM1HI "HI") (RVMM2HI "HI") (RVMM4HI "HI") (RVMM8HI "HI")
  (RVMM1SI "SI") (RVMM2SI "SI") (RVMM4SI "SI") (RVMM8SI "SI")
  (RVMM1DI "DI") (RVMM2DI "DI") (RVMM4DI "DI") (RVMM8DI "DI")

  (RVMM1HF "HF") (RVMM2HF "HF") (RVMM4HF "HF") (RVMM8HF "HF")
  (RVMM1SF "SF") (RVMM2SF "SF") (RVMM4SF "SF") (RVMM8SF "SF")
  (RVMM1DF "DF") (RVMM2DF "DF") (RVMM4DF "DF") (RVMM8DF "DF")
])

(define_mode_attr MM1 [
  (RVMM1QI "RVMM1QI") (RVMM2QI "RVMM1QI") (RVMM4QI "RVMM1QI") (RVMM8QI "RVMM1QI")
  (RVMM1HI "RVMM1HI") (RVMM2HI "RVMM1HI") (RVMM4HI "RVMM1HI") (RVMM8HI "RVMM1HI")
  (RVMM1SI "RVMM1SI") (RVMM2SI "RVMM1SI") (RVMM4SI "RVMM1SI") (RVMM8SI "RVMM1SI")
  (RVMM1DI "RVMM1DI") (RVMM2DI "RVMM1DI") (RVMM4DI "RVMM1DI") (RVMM8DI "RVMM1DI")

  (RVMM1HF "RVMM1HF") (RVMM2HF "RVMM1HF") (RVMM4HF "RVMM1HF") (RVMM8HF "RVMM1HF")
  (RVMM1SF "RVMM1SF") (RVMM2SF "RVMM1SF") (RVMM4SF "RVMM1SF") (RVMM8SF "RVMM1SF")
  (RVMM1DF "RVMM1DF") (RVMM2DF "RVMM1DF") (RVMM4DF "RVMM1DF") (RVMM8DF "RVMM1DF")
])

(define_mode_attr mlum [
  (RVMM1QI "1") (RVMM2QI "2") (RVMM4QI "4") (RVMM8QI "8")
  (RVMM1HI "1") (RVMM2HI "2") (RVMM4HI "4") (RVMM8HI "8")
  (RVMM1SI "1") (RVMM2SI "2") (RVMM4SI "4") (RVMM8SI "8")
  (RVMM1DI "1") (RVMM2DI "2") (RVMM4DI "4") (RVMM8DI "8")

  (RVMM1HF "1") (RVMM2HF "2") (RVMM4HF "4") (RVMM8HF "8")
  (RVMM1SF "1") (RVMM2SF "2") (RVMM4SF "4") (RVMM8SF "8")
  (RVMM1DF "1") (RVMM2DF "2") (RVMM4DF "4") (RVMM8DF "8")
])

(define_mode_attr lsfmt [
  (RVMM1QI "b") (RVMM2QI "b") (RVMM4QI "b") (RVMM8QI "b")
  (RVMM1HI "h") (RVMM2HI "h") (RVMM4HI "h") (RVMM8HI "h")
  (RVMM1SI "w") (RVMM2SI "w") (RVMM4SI "w") (RVMM8SI "w")
  (RVMM1DI "d") (RVMM2DI "d") (RVMM4DI "d") (RVMM8DI "d")

  (RVMM1HF "h") (RVMM2HF "h") (RVMM4HF "h") (RVMM8HF "h")
  (RVMM1SF "w") (RVMM2SF "w") (RVMM4SF "w") (RVMM8SF "w")
  (RVMM1DF "d") (RVMM2DF "d") (RVMM4DF "d") (RVMM8DF "d")
])

(define_mode_attr mfmt [
  (RVMM1QI "b") (RVMM2QI "b") (RVMM4QI "b") (RVMM8QI "b")
  (RVMM1HI "h") (RVMM2HI "h") (RVMM4HI "h") (RVMM8HI "h")
  (RVMM1SI "s") (RVMM2SI "s") (RVMM4SI "s") (RVMM8SI "s")
  (RVMM1DI "d") (RVMM2DI "d") (RVMM4DI "d") (RVMM8DI "d")

  (RVMM1HF "h") (RVMM2HF "h") (RVMM4HF "h") (RVMM8HF "h")
  (RVMM1SF "s") (RVMM2SF "s") (RVMM4SF "s") (RVMM8SF "s")
  (RVMM1DF "d") (RVMM2DF "d") (RVMM4DF "d") (RVMM8DF "d")
])

;; True if the type is RVV instructions that include VTYPE
;; global status register in the use op list.
;; We known VTYPE has 4 fields: SEW, LMUL, TA, MA.
;; The instruction need any of VTYPE field is set as true
;; in this attribute.
(define_attr "th_m_has_mcfg_op" "false,true"
  (cond [(eq_attr "type" "mld,mst,madd,msub,mmul,mmulh,mfmacc,mmaqa,mshift")
	 (const_string "true")]
	(const_string "false")))

(define_attr "matrix_mcfgm_op_idx" "" (const_int INVALID_ATTRIBUTE))

(define_attr "matrix_mcfgn_op_idx" "" (const_int INVALID_ATTRIBUTE))

(define_attr "matrix_mcfgk_op_idx" "" (const_int INVALID_ATTRIBUTE))

(define_attr "matrix_ebytes" ""
  (cond [(eq_attr "emode" "QI") (const_int 1)
	 (eq_attr "emode" "HI,HF") (const_int 2)
	 (eq_attr "emode" "SI,SF") (const_int 4)
	 (eq_attr "emode" "DI,DF") (const_int 8)]
  (const_int 0)))

;; Matrix cfg
(define_insn "matrix_mcfg"
  [(set (match_operand:SI 0 "register_operand"    "=r")
	(match_operand:SI 1 "register_operand" " r"))
   (set (reg:QI MSIZE_M_REGNUM)
	(zero_extract:QI
	  (match_dup 1)
	  (const_int 8)
	  (const_int 0)))
   (set (reg:QI MSIZE_N_REGNUM)
	(zero_extract:QI
	  (match_dup 1)
	  (const_int 8)
	  (const_int 8)))
   (set (reg:HI MSIZE_K_REGNUM)
	(zero_extract:HI
	  (match_dup 1)
	  (const_int 16)
	  (const_int 16)))]
  "TARGET_MATRIX"
  "mcfg\t%0,%1"
  [(set_attr "type" "mcfg")
  (set_attr "mode" "none")])

(define_insn "matrix_mcfgm"
  [(set (reg:QI MSIZE_M_REGNUM)
	(match_operand:QI 1 "reg_or_const_int_operand" "rI"))
    (set (match_operand:SI 0 "register_operand"        "=r")
	(unspec:SI [
	  (reg:QI MSIZE_M_REGNUM)
	  (reg:QI MSIZE_N_REGNUM)
	  (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_USE_MSIZE))]
  "TARGET_MATRIX"
  "mcfgm%i1\t%0,%1"
  [(set_attr "type" "mcfg")
  (set_attr "mode" "none")])

(define_insn "matrix_mcfgn"
  [(set (reg:QI MSIZE_N_REGNUM)
	(match_operand:QI 1 "reg_or_const_int_operand" "rI"))
    (set (match_operand:SI 0 "register_operand"        "=r")
	(unspec:SI [
	  (reg:QI MSIZE_M_REGNUM)
	  (reg:QI MSIZE_N_REGNUM)
	  (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_USE_MSIZE))]
  "TARGET_MATRIX"
  "mcfgn%i1\t%0,%1"
  [(set_attr "type" "mcfg")
  (set_attr "mode" "none")])

(define_insn "matrix_mcfgk"
  [(set (reg:HI MSIZE_K_REGNUM)
	(match_operand:HI 1 "reg_or_const_int_operand" "rI"))
    (set (match_operand:SI 0 "register_operand"        "=r")
	(unspec:SI [
	  (reg:QI MSIZE_M_REGNUM)
	  (reg:QI MSIZE_N_REGNUM)
	  (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_USE_MSIZE))]
  "TARGET_MATRIX"
  "mcfgk%i1\t%0,%1"
  [(set_attr "type" "mcfg")
  (set_attr "mode" "none")])

(define_insn "@matrix_mundefined<mode>"
  [(set (match_operand:MMODES 0 "register_operand" "=xr")
	      (unspec:MMODES
	  [(reg:SI X0_REGNUM)]
	UNSPEC_MUNDEF))]
  "TARGET_MATRIX"
  ""
  [(set_attr "type" "mmov")])

;; Matrix zero
(define_insn "@matrix_mzero<mode>"
  [(set (match_operand:MMODES 0 "register_operand" "=xr")
	(unspec:MMODES
		[(reg:SI X0_REGNUM)]
	UNSPEC_MZERO))]
  "TARGET_MATRIX"
  {
    static char buf[128] = {0};
    size_t bytes = 0;
    unsigned regno = REGNO (operands[0]);

    bytes += snprintf (buf + bytes, sizeof (buf), "mzero\t%s", reg_names[regno]);

    unsigned lmul = 1 << th_m_get_mlmul (<MODE>mode);
    for (unsigned i = 1; i < lmul; i++)
      bytes += snprintf (buf + bytes, sizeof (buf), "\n\tmzero\t%s", reg_names[regno + i]);

    return buf;
  }
  [(set_attr "type" "mzero")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")])

;; Matrix mrelease
(define_insn "matrix_mrelease"
  [(unspec_volatile [(const_int 0)] UNSPEC_MRELEASE)]
  "TARGET_MATRIX"
  "mrelease"
  [(set_attr "type" "mrelease")
  (set_attr "mode" "none")])

;; Matrix read mlenb

(define_expand "matrix_read_xmlenb"
  [(set (match_operand 0 "register_operand")
	(unspec
	  [(const_int 0)]
	UNSPEC_MREAD_XMLENB))]
  "TARGET_MATRIX"
{
  rtx imm = gen_int_mode (UNITS_PER_M_REG, Pmode);
  emit_move_insn (operands[0], imm);
  DONE;
})

(define_insn "matrix_read_xrlenb<mode>"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P
	  [(const_int 0)]
	UNSPEC_MREAD_XRLENB))]
  "TARGET_MATRIX"
  "csrr\t%0,xrlenb"
  [(set_attr "type" "mcsr")
   (set_attr "mode" "none")])

(define_insn "matrix_read_xmsize<mode>"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P
	  [(const_int 0)]
	UNSPEC_MREAD_XMSIZE))]
  "TARGET_MATRIX"
  "csrr\t%0,xmsize"
  [(set_attr "type" "mcsr")
   (set_attr "mode" "none")])

;; Matrix whole register

(define_insn "@mov<mode>"
  [(set (match_operand:MMODES 0 "reg_or_mem_operand" "=xr, m,xr")
	(match_operand:MMODES 1 "reg_or_mem_operand" "  m,xr,xr"))]
  "TARGET_MATRIX"
  {
    switch (which_alternative)
      {
      case 0:
	return "mld<mlum>m<lsfmt>\t%0,%1";
      case 1:
	return "mst<mlum>m<lsfmt>\t%1,%0";
      case 2:
	{
	  static char buf[128] = {0};
	  size_t bytes = 0;
	  unsigned regno0 = REGNO (operands[0]);
	  unsigned regno1 = REGNO (operands[1]);
	  
	  bytes += snprintf (buf + bytes, sizeof (buf), "mmov.mm\t%s,%s", reg_names[regno0], reg_names[regno1]);
	  
	  unsigned lmul = 1 << th_m_get_mlmul (<MODE>mode);
	  for (unsigned i = 1; i < lmul; i++)
	  bytes += snprintf (buf + bytes, sizeof (buf), "\n\tmmov.mm\t%s,%s", reg_names[regno0 + i], reg_names[regno1 + i]);
	  
	  return buf;
	}
      default:
	gcc_unreachable ();
      }
  }
  [(set_attr "type" "mldr,mstr,mmovr")
   (set_attr "mode" "none")])

;; Matrix load
(define_expand "@matrix_mld<M1MODES:mode><P:mode>"
  [(set (match_operand:M1MODES 0 "register_operand")
	(unspec:M1MODES
	  [(mem:M1MODES (match_operand:P 1 "register_operand"))
	   (match_operand:P 2 "register_operand")
	   (match_operand:QI 3 "reg_or_const_int_operand")
	   (match_operand:HI 4 "reg_or_const_int_operand")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MLD_MST))]
  "TARGET_MATRIX"
  {})

(define_insn "*matrix_mld<M1MODES:mode><P:mode>"
  [(set (match_operand:M1MODES 0 "register_operand"       "=xr")
	(unspec:M1MODES
	  [(match_operand:M1MODES 1 "memory_operand"      "  m")
	   (match_operand:P 2 "register_operand"          "  r")
	   (match_operand:QI 3 "reg_or_const_int_operand" " rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" " rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MLD_MST))]
  "TARGET_MATRIX"
  "mld<lsfmt>\t%0,%2,%1"
  [(set_attr "type" "mld")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

(define_expand "@matrix_msld<M1MODES:mode><P:mode>"
  [(set (match_operand:M1MODES 0 "register_operand")
	(unspec:M1MODES
	  [(mem:M1MODES (match_operand:P 1 "register_operand"))
	   (match_operand:P 2 "register_operand")
	   (match_operand:QI 3 "reg_or_const_int_operand")
	   (match_operand:HI 4 "reg_or_const_int_operand")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MSLD_MSST))]
  "TARGET_MATRIX"
  {})

(define_insn "*matrix_msld<M1MODES:mode><P:mode>"
  [(set (match_operand:M1MODES 0 "register_operand"       "=xr")
	(unspec:M1MODES
	  [(match_operand:M1MODES 1 "memory_operand"      "  m")
	   (match_operand:P 2 "register_operand"          "  r")
	   (match_operand:QI 3 "reg_or_const_int_operand" " rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" " rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MSLD_MSST))]
  "TARGET_MATRIX"
  "msld<lsfmt>\t%0,%2,%1"
  [(set_attr "type" "mld")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

;; Matrix store
(define_expand "@matrix_mst<M1MODES:mode><P:mode>"
  [(set (mem:M1MODES (match_operand:P 0 "register_operand"))
	(unspec:M1MODES
	  [(match_operand:P 1 "register_operand")
	   (match_operand:M1MODES 2 "register_operand")
	   (match_operand:QI 3 "reg_or_const_int_operand")
	   (match_operand:HI 4 "reg_or_const_int_operand")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MLD_MST))]
  "TARGET_MATRIX"
  {})

(define_insn "*matrix_mst<M1MODES:mode><P:mode>"
  [(set (match_operand:M1MODES 0 "memory_operand"         "=m")
	(unspec:M1MODES
	  [(match_operand:P 1 "register_operand"          " r")
	   (match_operand:M1MODES 2 "register_operand"    "xr")
	   (match_operand:QI 3 "reg_or_const_int_operand" "rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" "rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MLD_MST))]
  "TARGET_MATRIX"
  "mst<lsfmt>\t%2,%1,%0"
  [(set_attr "type" "mst")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

(define_expand "@matrix_msst<M1MODES:mode><P:mode>"
  [(set (mem:M1MODES (match_operand:P 0 "register_operand"))
	(unspec:M1MODES
	  [(match_operand:P 1 "register_operand")
	   (match_operand:M1MODES 2 "register_operand")
	   (match_operand:QI 3 "reg_or_const_int_operand")
	   (match_operand:HI 4 "reg_or_const_int_operand")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MSLD_MSST))]
  "TARGET_MATRIX"
  {})

(define_insn "*matrix_msst<M1MODES:mode><P:mode>"
  [(set (match_operand:M1MODES 0 "memory_operand"         "=m")
	(unspec:M1MODES
	  [(match_operand:P 1 "register_operand"          " r")
	   (match_operand:M1MODES 2 "register_operand"    "xr")
	   (match_operand:QI 3 "reg_or_const_int_operand" "rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" "rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MSLD_MSST))]
  "TARGET_MATRIX"
  "msst<lsfmt>\t%2,%1,%0"
  [(set_attr "type" "mst")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])


;; Matrix mov

(define_insn "@matrix_mmov_mv<mode>"
  [(set (match_operand:M1MODES 0 "register_operand"    "=xr,xr")
	(unspec:M1MODES
	  [(match_operand:M1MODES 1 "register_operand" " xr,xr")
	   (match_operand:QI 2 "reg_or_imm_operand"    " xi, I")]
	UNSPEC_MMOVE))]
  "TARGET_MATRIX"
  {
    if (immediate_operand (operands[2], GET_MODE (operands[2]))){
      return "mmov.mv.i\t%0,%1[%2]";
    }

    return "mmov.mv.x\t%0,%1[%2]";
  }
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")])

(define_insn "@matrix_mmov_m_x<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr")
	(unspec:I_M1MODES
	  [(match_operand:I_M1MODES 1 "register_operand"  "  0")
	   (match_operand:<MEL> 2 "register_operand" "  r")
	   (match_operand:QI 3 "register_operand"         "  r")]
	UNSPEC_MMOVE))]
  "TARGET_MATRIX"
  "mmov<lsfmt>.m.x\t%0,%2,%3"
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")])

(define_insn "@matrix_mmov_x_m<mode>"
  [(set (match_operand:<MEL> 0 "register_operand"   "=r")
	(unspec:<MEL>
	  [(match_operand:I_M1MODES 1 "register_operand" "xr")
	   (match_operand:QI 2 "register_operand"        " r")]
	UNSPEC_MMOVE))]
  "TARGET_MATRIX"
  "mmov<lsfmt>.x.m\t%0,%1,%2"
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")])

(define_insn "@matrix_mdup_m_x<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr")
	(unspec:I_M1MODES
	  [(match_operand:<MEL> 1 "register_operand" "  r")]
	UNSPEC_MDUP))]
  "TARGET_MATRIX"
  "mdup<lsfmt>.m.x\t%0,%1"
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")])

;; Matrix integer pointwise arithmetic
(define_int_iterator PW_ITERATOR [UNSPEC_MADD UNSPEC_MSUB UNSPEC_MMUL UNSPEC_MMULH])
(define_int_attr pw_pattern [(UNSPEC_MADD "madd") (UNSPEC_MSUB "msub") (UNSPEC_MMUL "mmul") (UNSPEC_MMULH "mmulh")])

(define_insn "@matrix_<pw_pattern>_mm<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr")
	(unspec:I_M1MODES
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr")
	   (match_operand:I_M1MODES 2 "register_operand"  " xr")
	   (match_operand:QI 3 "reg_or_const_int_operand" " rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" " rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	PW_ITERATOR))]
  "TARGET_MATRIX"
  "<pw_pattern>.<mfmt>.mm\t%0,%1,%2"
  [(set_attr "type" "<pw_pattern>")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

(define_insn "@matrix_<pw_pattern>_mv<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr,xr")
	(unspec:I_M1MODES
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr,xr")
	   (match_operand:I_M1MODES 2 "register_operand"  " xr,xr")
	   (match_operand:QI 3 "reg_or_imm_operand"       " xi, I")
	   (match_operand:QI 4 "reg_or_const_int_operand" " rI, rI")
	   (match_operand:HI 5 "reg_or_const_int_operand" " rI, rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	PW_ITERATOR))]
  "TARGET_MATRIX"
  {
    if (immediate_operand (operands[3], GET_MODE (operands[3]))){
      return "<pw_pattern>.<mfmt>.mv.i\t%0,%1,%2[%3]";
    }

    return "<pw_pattern>.<mfmt>.mv.x\t%0,%1,%2[%3]";
  }
  [(set_attr "type" "<pw_pattern>")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgk_op_idx") (const_int 5))])

(define_insn "@matrix_<pw_pattern>_mx<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr")
	(unspec:I_M1MODES
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr")
	   (match_operand:<MEL> 2 "register_operand" " xi")
	   (match_operand:QI 3 "reg_or_const_int_operand" " rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" " rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	PW_ITERATOR))]
  "TARGET_MATRIX"
  "<pw_pattern>.<mfmt>.mx\t%0,%1,%2"
  [(set_attr "type" "<pw_pattern>")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

;; Matrix sra
(define_insn "@matrix_msra_mm<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr")
	(unspec:I_M1MODES
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr")
	   (match_operand:I_M1MODES 2 "register_operand"  " xr")
	   (match_operand:QI 3 "reg_or_const_int_operand" " rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" " rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MSRA))]
  "TARGET_MATRIX"
  "msra.<mfmt>.mm\t%0,%1,%2"
  [(set_attr "type" "mshift")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

(define_insn "@matrix_msra_mv<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr,xr")
	(unspec:I_M1MODES
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr,xr")
	   (match_operand:I_M1MODES 2 "register_operand"  " xr,xr")
	   (match_operand:QI 3 "reg_or_imm_operand"       " xi,I")
	   (match_operand:QI 4 "reg_or_const_int_operand" " rI,rI")
	   (match_operand:HI 5 "reg_or_const_int_operand" " rI,rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MSRA))]
  "TARGET_MATRIX"
  {
    if (immediate_operand (operands[3], GET_MODE (operands[3]))){
      return "msra.<mfmt>.mv.i\t%0,%1,%2[%3]";
    }

    return "msra.<mfmt>.mv.x\t%0,%1,%2[%3]";
  }
  [(set_attr "type" "mshift")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgk_op_idx") (const_int 5))])

(define_insn "@matrix_msra_mx<mode>"
  [(set (match_operand:I_M1MODES 0 "register_operand"     "=xr")
	(unspec:I_M1MODES
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr")
	   (match_operand:<MEL> 2 "register_operand" " xi")
	   (match_operand:QI 3 "reg_or_const_int_operand" " rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" " rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	UNSPEC_MSRA))]
  "TARGET_MATRIX"
  "msra.<mfmt>.mx\t%0,%1,%2"
  [(set_attr "type" "mshift")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

;; Matrix mn4clip/mn4clipu
(define_int_iterator CLIP_ITERATOR [UNSPEC_MN4CLIP UNSPEC_MN4CLIPU])
(define_int_attr clip_pattern [(UNSPEC_MN4CLIP "mn4clip") (UNSPEC_MN4CLIPU "mn4clipu")])

(define_insn "@matrix_<clip_pattern>_mm<mode>"
  [(set (match_operand:RVMM1QI 0 "register_operand"       "=xr")
	(unspec:RVMM1QI
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr")
	   (match_operand:I_M1MODES 2 "register_operand"  " xr")
	   (match_operand:QI 3 "reg_or_const_int_operand" " rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" " rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	CLIP_ITERATOR))]
  "TARGET_MATRIX"
  "<clip_pattern>.<mfmt>.mm\t%0,%1,%2"
  [(set_attr "type" "mshift")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])

(define_insn "@matrix_<clip_pattern>_mv<mode>"
  [(set (match_operand:RVMM1QI 0 "register_operand"       "=xr,xr")
	(unspec:RVMM1QI
	  [(match_operand:I_M1MODES 1 "register_operand"  " xr,xr")
	   (match_operand:I_M1MODES 2 "register_operand"  " xr,xr")
	   (match_operand:QI 3 "reg_or_imm_operand"       " xi,I")
	   (match_operand:QI 4 "reg_or_const_int_operand" " rI,rI")
	   (match_operand:HI 5 "reg_or_const_int_operand" " rI,rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	CLIP_ITERATOR))]
  "TARGET_MATRIX"
  {
    if (immediate_operand (operands[3], GET_MODE (operands[3]))){
      return "<clip_pattern>.<mfmt>.mv.i\t%0,%1,%2[%3]";
    }

    return "<clip_pattern>.<mfmt>.mv.x\t%0,%1,%2[%3]";
  }
  [(set_attr "type" "mshift")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgk_op_idx") (const_int 5))])

(define_insn "@matrix_<clip_pattern>_mx<mode>"
  [(set (match_operand:RVMM1QI 0 "register_operand"      "=xr")
	(unspec:RVMM1QI
	  [(match_operand:I_M1MODES 1 "register_operand" " xr")
	   (match_operand:<MEL> 2 "register_operand" "xi")
	   (match_operand:QI 3 "reg_or_const_int_operand" "rI")
	   (match_operand:HI 4 "reg_or_const_int_operand" "rI")
	   (reg:QI MSIZE_M_REGNUM)
	   (reg:HI MSIZE_K_REGNUM)]
	CLIP_ITERATOR))]
  "TARGET_MATRIX"
  "<clip_pattern>.<mfmt>.mx\t%0,%1,%2"
  [(set_attr "type" "mshift")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")
  (set (attr "matrix_mcfgm_op_idx") (const_int 3))
  (set (attr "matrix_mcfgk_op_idx") (const_int 4))])


;; Matrix fmmacc

(define_insn "matrix_fmmacc_h"
  [(set (match_operand:RVMM1HF 0 "register_operand"         "=&xr")
	(unspec:RVMM1HF
	  [(plus:RVMM1HF
	     (match_operand:RVMM1HF 1 "register_operand"    "   0")
	     (mult:RVMM1HF
	       (match_operand:RVMM1HF 2 "register_operand"  "  xr")
	       (match_operand:RVMM2HF 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_FMMACC))]
  "TARGET_MATRIX"
  "fmmacc.h\t%0,%3,%2"
  [(set_attr "type" "mfmacc")
  (set_attr "mode" "none")
  (set_attr "emode" "HF")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])

(define_insn "matrix_fmmacc_s"
  [(set (match_operand:RVMM1SF 0 "register_operand"         "=&xr")
	(unspec:RVMM1SF
	  [(plus:RVMM1SF
	     (match_operand:RVMM1SF 1 "register_operand"    "   0")
	     (mult:RVMM1SF
	       (match_operand:RVMM1SF 2 "register_operand"  "  xr")
	       (match_operand:RVMM1SF 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_FMMACC))]
  "TARGET_MATRIX"
  "fmmacc.s\t%0,%3,%2"
  [(set_attr "type" "mfmacc")
  (set_attr "mode" "none")
  (set_attr "emode" "HF")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])

(define_insn "matrix_fmmacc_d"
  [(set (match_operand:RVMM2DF 0 "register_operand"         "=&xr")
	(unspec:RVMM2DF
	  [(plus:RVMM2DF
	     (match_operand:RVMM2DF 1 "register_operand"    "   0")
	     (mult:RVMM2DF
	       (match_operand:RVMM1DF 2 "register_operand"  "  xr")
	       (match_operand:RVMM1DF 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_FMMACC))]
  "TARGET_MATRIX"
  "fmmacc.d\t%0,%3,%2"
  [(set_attr "type" "mfmacc")
  (set_attr "mode" "none")
  (set_attr "emode" "DF")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])


;; Matrix fwmmacc

(define_insn "matrix_fwmmacc_h"
  [(set (match_operand:RVMM1SF 0 "register_operand"         "=&xr")
	(unspec:RVMM1SF
	  [(plus:RVMM1SF
	     (match_operand:RVMM1SF 1 "register_operand"    "   0")
	     (mult:RVMM1SF
	       (match_operand:RVMM1HF 2 "register_operand"  "  xr")
	       (match_operand:RVMM1HF 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_FWMMACC))]
  "TARGET_MATRIX"
  "fwmmacc.h\t%0,%3,%2"
  [(set_attr "type" "mfmacc")
  (set_attr "mode" "none")
  (set_attr "emode" "HF")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])

(define_insn "matrix_fwmmacc_s"
  [(set (match_operand:RVMM2DF 0 "register_operand"         "=&xr")
	(unspec:RVMM2DF
	  [(plus:RVMM2DF
	     (match_operand:RVMM2DF 1 "register_operand"    "   0")
	     (mult:RVMM2DF
	       (match_operand:RVMM1SF 2 "register_operand"  "  xr")
	       (match_operand:RVMM1SF 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_FWMMACC))]
  "TARGET_MATRIX"
  "fwmmacc.s\t%0,%3,%2"
  [(set_attr "type" "mfmacc")
  (set_attr "mode" "none")
  (set_attr "emode" "HF")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])


;; Matrix mmaqa
(define_int_iterator UNSPEC_MMAQA_IT [UNSPEC_MMAQASS UNSPEC_MMAQAUU UNSPEC_MMAQAUS UNSPEC_MMAQASU])
(define_int_attr mmaqa_pattern [(UNSPEC_MMAQASS "") (UNSPEC_MMAQAUU "u") (UNSPEC_MMAQAUS "us") (UNSPEC_MMAQASU "su")])

(define_insn "matrix_mmaqa<mmaqa_pattern>_b"
  [(set (match_operand:RVMM1SI 0 "register_operand"         "=&xr")
	(unspec:RVMM1SI
	  [(plus:RVMM1SI
	     (match_operand:RVMM1SI 1 "register_operand"    "   0")
	     (mult:RVMM1SI
	       (match_operand:RVMM1QI 2 "register_operand"  "  xr")
	       (match_operand:RVMM1QI 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_MMAQA_IT))]
  "TARGET_MATRIX"
  "mmaqa<mmaqa_pattern>.b\t%0,%3,%2"
  [(set_attr "type" "mmaqa")
  (set_attr "mode" "none")
  (set_attr "emode" "QI")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])

(define_insn "matrix_mmaqa<mmaqa_pattern>_h"
  [(set (match_operand:RVMM2DI 0 "register_operand"         "=&xr")
	(unspec:RVMM2DI
	  [(plus:RVMM2DI
	     (match_operand:RVMM2DI 1 "register_operand"    "   0")
	     (mult:RVMM2DI
	       (match_operand:RVMM1HI 2 "register_operand"  "  xr")
	       (match_operand:RVMM1HI 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_MMAQA_IT))]
  "TARGET_MATRIX"
  "mmaqa<mmaqa_pattern>.h\t%0,%3,%2"
  [(set_attr "type" "mmaqa")
  (set_attr "mode" "none")
  (set_attr "emode" "HI")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])

;; Matrix pmmaqa
(define_int_iterator UNSPEC_PMMAQA_IT [UNSPEC_PMMAQASS UNSPEC_PMMAQAUU UNSPEC_PMMAQAUS UNSPEC_PMMAQASU])
(define_int_attr pmmaqa_pattern [(UNSPEC_PMMAQASS "") (UNSPEC_PMMAQAUU "u") (UNSPEC_PMMAQAUS "us") (UNSPEC_PMMAQASU "su")])

(define_insn "matrix_pmmaqa<pmmaqa_pattern>_b"
  [(set (match_operand:RVMM1SI 0 "register_operand"         "=&xr")
	(unspec:RVMM1SI
	  [(plus:RVMM1SI
	     (match_operand:RVMM1SI 1 "register_operand"    "   0")
	     (mult:RVMM1SI
	       (match_operand:RVMM1QI 2 "register_operand"  "  xr")
	       (match_operand:RVMM1QI 3 "register_operand"  "  xr")))
	     (match_operand:QI 4 "reg_or_const_int_operand" "  rI")
	     (match_operand:QI 5 "reg_or_const_int_operand" "  rI")
	     (match_operand:HI 6 "reg_or_const_int_operand" "  rI")
	     (reg:QI MSIZE_M_REGNUM)
	     (reg:HI MSIZE_K_REGNUM)
	     (reg:QI MSIZE_N_REGNUM)]
	  UNSPEC_PMMAQA_IT))]
  "TARGET_MATRIX"
  "pmmaqa<pmmaqa_pattern>.b\t%0,%3,%2"
  [(set_attr "type" "mmaqa")
  (set_attr "mode" "none")
  (set_attr "emode" "QI")
  (set (attr "matrix_mcfgm_op_idx") (const_int 4))
  (set (attr "matrix_mcfgn_op_idx") (const_int 5))
  (set (attr "matrix_mcfgk_op_idx") (const_int 6))])

;; Matrix helper pattern
;; Matrix convert
(define_insn_and_split "@matrix_mreinterpret_m<M1MODES:mode><M1MODES_ALIAS:mode>"
  [(set (match_operand:M1MODES 0 "register_operand"          "=xr")
	(unspec:M1MODES
	  [(match_operand:M1MODES_ALIAS 1 "register_operand" " xr")]
	UNSPEC_MREINTERPRET))]
  "TARGET_MATRIX"
  "mmov.mm\t%0,%1"
  "&& (REGNO (operands[0]) == REGNO (operands[1]))"
  [(const_int 0)]
  {
    emit_note (NOTE_INSN_DELETED);
    DONE;
  }
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<M1MODES:MEL>")])

(define_insn_and_split "@matrix_mreinterpret_m<M2MODES:mode><M2MODES_ALIAS:mode>"
  [(set (match_operand:M2MODES 0 "register_operand"          "=xr")
	(unspec:M2MODES
	  [(match_operand:M2MODES_ALIAS 1 "register_operand" " xr")]
	UNSPEC_MREINTERPRET))]
  "TARGET_MATRIX"
  "mmov.mm\t%0,%1\n\tmmov.mm\t%N0,%N1"
  "&& (REGNO (operands[0]) == REGNO (operands[1]))"
  [(const_int 0)]
  {
    emit_note (NOTE_INSN_DELETED);
    DONE;
  }
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<M2MODES:MEL>")])

;; Matrix mset/mget
(define_insn_and_split "@matrix_mset<mode>"
  [(set (match_operand:M2MODES 0 "register_operand"    "=xr")
	(unspec:M2MODES
	  [(match_operand:M2MODES 1 "register_operand" "  0")
	   (match_operand 2 "const_int_operand"        "  I")
	   (match_operand:<MM1> 3 "register_operand"   " xr")]
	UNSPEC_MSET_TUPLE))]
  "TARGET_MATRIX && INTVAL (operands[2]) < 2"
  {
    if (INTVAL (operands[2]) == 0)
      return "mmov.mm\t%0,%3";

    if (INTVAL (operands[2]) == 1)
      return "mmov.mm\t%N0,%3";

    gcc_unreachable ();
  }
  "&& reload_completed"
  [(const_int 0)]
  {
    if ((INTVAL (operands[2]) == 0 &&  (REGNO (operands[0]) == REGNO (operands[3])))
	|| (INTVAL (operands[2]) == 1 &&  ((REGNO (operands[0]) + 1) == REGNO (operands[3]))))
      {
	emit_note (NOTE_INSN_DELETED);
	DONE;
      }

    FAIL;
  }
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")])

(define_insn_and_split "@matrix_mget<mode>"
  [(set (match_operand:<MM1>  0 "register_operand"     "=xr")
	(unspec:<MM1>
	  [(match_operand:M2MODES 1 "register_operand" " xr")
	   (match_operand 2 "const_int_operand"        "  I")]
	UNSPEC_MGET_TUPLE))]
  "TARGET_MATRIX && INTVAL (operands[2]) < 2"
  {
    if (INTVAL (operands[2]) == 0)
      return "mmov.mm\t%0,%1";

    if (INTVAL (operands[2]) == 1)
      return "mmov.mm\t%0,%N1";

    gcc_unreachable ();
  }
  "&& reload_completed"
  [(const_int 0)]
  {
    if ((INTVAL (operands[2]) == 0 &&  (REGNO (operands[0]) == REGNO (operands[1])))
	|| (INTVAL (operands[2]) == 1 &&  (REGNO (operands[0]) == (REGNO (operands[1]) + 1))))
      {
	emit_note (NOTE_INSN_DELETED);
	DONE;
      }

    FAIL;
  }
  [(set_attr "type" "mmov")
  (set_attr "mode" "none")
  (set_attr "emode" "<MEL>")])
