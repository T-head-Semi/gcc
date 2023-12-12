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

(define_mode_iterator VALL [
  RVVM8QI RVVM4QI RVVM2QI RVVM1QI RVVMF2QI RVVMF4QI (RVVMF8QI "TARGET_MIN_VLEN > 32")

  RVVM8HI RVVM4HI RVVM2HI RVVM1HI RVVMF2HI (RVVMF4HI "TARGET_MIN_VLEN > 32")

  (RVVM8HF "TARGET_VECTOR_ELEN_FP_16") (RVVM4HF "TARGET_VECTOR_ELEN_FP_16") (RVVM2HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVM1HF "TARGET_VECTOR_ELEN_FP_16") (RVVMF2HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")

  RVVM8SI RVVM4SI RVVM2SI RVVM1SI (RVVMF2SI "TARGET_MIN_VLEN > 32")

  (RVVM8SF "TARGET_VECTOR_ELEN_FP_32") (RVVM4SF "TARGET_VECTOR_ELEN_FP_32") (RVVM2SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVM1SF "TARGET_VECTOR_ELEN_FP_32") (RVVMF2SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")

  (RVVM8DI "TARGET_VECTOR_ELEN_64") (RVVM4DI "TARGET_VECTOR_ELEN_64")
  (RVVM2DI "TARGET_VECTOR_ELEN_64") (RVVM1DI "TARGET_VECTOR_ELEN_64")

  (RVVM8DF "TARGET_VECTOR_ELEN_FP_64") (RVVM4DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM2DF "TARGET_VECTOR_ELEN_FP_64") (RVVM1DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVMF64BI "TARGET_MIN_VLEN > 32") RVVMF32BI RVVMF16BI RVVMF8BI RVVMF4BI RVVMF2BI RVVM1BI
  RVVM1x8QI RVVMF2x8QI RVVMF4x8QI (RVVMF8x8QI "TARGET_MIN_VLEN > 32")
  RVVM1x7QI RVVMF2x7QI RVVMF4x7QI (RVVMF8x7QI "TARGET_MIN_VLEN > 32")
  RVVM1x6QI RVVMF2x6QI RVVMF4x6QI (RVVMF8x6QI "TARGET_MIN_VLEN > 32")
  RVVM1x5QI RVVMF2x5QI RVVMF4x5QI (RVVMF8x5QI "TARGET_MIN_VLEN > 32")
  RVVM2x4QI RVVM1x4QI RVVMF2x4QI RVVMF4x4QI (RVVMF8x4QI "TARGET_MIN_VLEN > 32")
  RVVM2x3QI RVVM1x3QI RVVMF2x3QI RVVMF4x3QI (RVVMF8x3QI "TARGET_MIN_VLEN > 32")
  RVVM4x2QI RVVM2x2QI RVVM1x2QI RVVMF2x2QI RVVMF4x2QI (RVVMF8x2QI "TARGET_MIN_VLEN > 32")

  RVVM1x8HI RVVMF2x8HI (RVVMF4x8HI "TARGET_MIN_VLEN > 32")
  RVVM1x7HI RVVMF2x7HI (RVVMF4x7HI "TARGET_MIN_VLEN > 32")
  RVVM1x6HI RVVMF2x6HI (RVVMF4x6HI "TARGET_MIN_VLEN > 32")
  RVVM1x5HI RVVMF2x5HI (RVVMF4x5HI "TARGET_MIN_VLEN > 32")
  RVVM2x4HI RVVM1x4HI RVVMF2x4HI (RVVMF4x4HI "TARGET_MIN_VLEN > 32")
  RVVM2x3HI RVVM1x3HI RVVMF2x3HI (RVVMF4x3HI "TARGET_MIN_VLEN > 32")
  RVVM4x2HI RVVM2x2HI RVVM1x2HI RVVMF2x2HI (RVVMF4x2HI "TARGET_MIN_VLEN > 32")

  (RVVM1x8HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF2x8HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4x8HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")
  (RVVM1x7HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF2x7HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4x7HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")
  (RVVM1x6HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF2x6HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4x6HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")
  (RVVM1x5HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF2x5HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4x5HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")
  (RVVM2x4HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVM1x4HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF2x4HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4x4HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")
  (RVVM2x3HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVM1x3HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF2x3HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4x3HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")
  (RVVM4x2HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVM2x2HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVM1x2HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF2x2HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVMF4x2HF "TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN > 32")

  RVVM1x8SI (RVVMF2x8SI "TARGET_MIN_VLEN > 32")
  RVVM1x7SI (RVVMF2x7SI "TARGET_MIN_VLEN > 32")
  RVVM1x6SI (RVVMF2x6SI "TARGET_MIN_VLEN > 32")
  RVVM1x5SI (RVVMF2x5SI "TARGET_MIN_VLEN > 32")
  RVVM2x4SI RVVM1x4SI (RVVMF2x4SI "TARGET_MIN_VLEN > 32")
  RVVM2x3SI RVVM1x3SI (RVVMF2x3SI "TARGET_MIN_VLEN > 32")
  RVVM4x2SI RVVM2x2SI RVVM1x2SI (RVVMF2x2SI "TARGET_MIN_VLEN > 32")

  (RVVM1x8SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2x8SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (RVVM1x7SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2x7SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (RVVM1x6SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2x6SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (RVVM1x5SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2x5SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (RVVM2x4SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVM1x4SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2x4SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (RVVM2x3SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVM1x3SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2x3SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (RVVM4x2SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVM2x2SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVM1x2SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2x2SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")

  (RVVM1x8DI "TARGET_VECTOR_ELEN_64")
  (RVVM1x7DI "TARGET_VECTOR_ELEN_64")
  (RVVM1x6DI "TARGET_VECTOR_ELEN_64")
  (RVVM1x5DI "TARGET_VECTOR_ELEN_64")
  (RVVM2x4DI "TARGET_VECTOR_ELEN_64")
  (RVVM1x4DI "TARGET_VECTOR_ELEN_64")
  (RVVM2x3DI "TARGET_VECTOR_ELEN_64")
  (RVVM1x3DI "TARGET_VECTOR_ELEN_64")
  (RVVM4x2DI "TARGET_VECTOR_ELEN_64")
  (RVVM2x2DI "TARGET_VECTOR_ELEN_64")
  (RVVM1x2DI "TARGET_VECTOR_ELEN_64")

  (RVVM1x8DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM1x7DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM1x6DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM1x5DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM2x4DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM1x4DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM2x3DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM1x3DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM4x2DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM2x2DF "TARGET_VECTOR_ELEN_FP_64")
  (RVVM1x2DF "TARGET_VECTOR_ELEN_FP_64")

  (V1QI "riscv_vector::vls_mode_valid_p (V1QImode)")
  (V2QI "riscv_vector::vls_mode_valid_p (V2QImode)")
  (V4QI "riscv_vector::vls_mode_valid_p (V4QImode)")
  (V8QI "riscv_vector::vls_mode_valid_p (V8QImode)")
  (V16QI "riscv_vector::vls_mode_valid_p (V16QImode)")
  (V1HI "riscv_vector::vls_mode_valid_p (V1HImode)")
  (V2HI "riscv_vector::vls_mode_valid_p (V2HImode)")
  (V4HI "riscv_vector::vls_mode_valid_p (V4HImode)")
  (V8HI "riscv_vector::vls_mode_valid_p (V8HImode)")
  (V16HI "riscv_vector::vls_mode_valid_p (V16HImode)")
  (V1SI "riscv_vector::vls_mode_valid_p (V1SImode)")
  (V2SI "riscv_vector::vls_mode_valid_p (V2SImode)")
  (V4SI "riscv_vector::vls_mode_valid_p (V4SImode)")
  (V8SI "riscv_vector::vls_mode_valid_p (V8SImode)")
  (V16SI "riscv_vector::vls_mode_valid_p (V16SImode) && TARGET_MIN_VLEN >= 64")
  (V1DI "riscv_vector::vls_mode_valid_p (V1DImode) && TARGET_VECTOR_ELEN_64")
  (V2DI "riscv_vector::vls_mode_valid_p (V2DImode) && TARGET_VECTOR_ELEN_64")
  (V4DI "riscv_vector::vls_mode_valid_p (V4DImode) && TARGET_VECTOR_ELEN_64")
  (V8DI "riscv_vector::vls_mode_valid_p (V8DImode) && TARGET_VECTOR_ELEN_64 && TARGET_MIN_VLEN >= 64")
  (V16DI "riscv_vector::vls_mode_valid_p (V16DImode) && TARGET_VECTOR_ELEN_64 && TARGET_MIN_VLEN >= 128")
  (V1HF "riscv_vector::vls_mode_valid_p (V1HFmode) && TARGET_VECTOR_ELEN_FP_16")
  (V2HF "riscv_vector::vls_mode_valid_p (V2HFmode) && TARGET_VECTOR_ELEN_FP_16")
  (V4HF "riscv_vector::vls_mode_valid_p (V4HFmode) && TARGET_VECTOR_ELEN_FP_16")
  (V8HF "riscv_vector::vls_mode_valid_p (V8HFmode) && TARGET_VECTOR_ELEN_FP_16")
  (V16HF "riscv_vector::vls_mode_valid_p (V16HFmode) && TARGET_VECTOR_ELEN_FP_16")
  (V1SF "riscv_vector::vls_mode_valid_p (V1SFmode) && TARGET_VECTOR_ELEN_FP_32")
  (V2SF "riscv_vector::vls_mode_valid_p (V2SFmode) && TARGET_VECTOR_ELEN_FP_32")
  (V4SF "riscv_vector::vls_mode_valid_p (V4SFmode) && TARGET_VECTOR_ELEN_FP_32")
  (V8SF "riscv_vector::vls_mode_valid_p (V8SFmode) && TARGET_VECTOR_ELEN_FP_32")
  (V16SF "riscv_vector::vls_mode_valid_p (V16SFmode) && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN >= 64")
  (V1DF "riscv_vector::vls_mode_valid_p (V1DFmode) && TARGET_VECTOR_ELEN_FP_64")
  (V2DF "riscv_vector::vls_mode_valid_p (V2DFmode) && TARGET_VECTOR_ELEN_FP_64")
  (V4DF "riscv_vector::vls_mode_valid_p (V4DFmode) && TARGET_VECTOR_ELEN_FP_64")
  (V8DF "riscv_vector::vls_mode_valid_p (V8DFmode) && TARGET_VECTOR_ELEN_FP_64 && TARGET_MIN_VLEN >= 64")
  (V16DF "riscv_vector::vls_mode_valid_p (V16DFmode) && TARGET_VECTOR_ELEN_FP_64 && TARGET_MIN_VLEN >= 128")

  (V1BI "riscv_vector::vls_mode_valid_p (V1BImode)")
  (V2BI "riscv_vector::vls_mode_valid_p (V2BImode)")
  (V4BI "riscv_vector::vls_mode_valid_p (V4BImode)")
  (V8BI "riscv_vector::vls_mode_valid_p (V8BImode)")
  (V16BI "riscv_vector::vls_mode_valid_p (V16BImode)")

  (V32QI "riscv_vector::vls_mode_valid_p (V32QImode)")
  (V64QI "riscv_vector::vls_mode_valid_p (V64QImode) && TARGET_MIN_VLEN >= 64")
  (V128QI "riscv_vector::vls_mode_valid_p (V128QImode) && TARGET_MIN_VLEN >= 128")
  (V256QI "riscv_vector::vls_mode_valid_p (V256QImode) && TARGET_MIN_VLEN >= 256")
  (V512QI "riscv_vector::vls_mode_valid_p (V512QImode) && TARGET_MIN_VLEN >= 512")
  (V1024QI "riscv_vector::vls_mode_valid_p (V1024QImode) && TARGET_MIN_VLEN >= 1024")
  (V2048QI "riscv_vector::vls_mode_valid_p (V2048QImode) && TARGET_MIN_VLEN >= 2048")
  (V4096QI "riscv_vector::vls_mode_valid_p (V4096QImode) && TARGET_MIN_VLEN >= 4096")
  (V32HI "riscv_vector::vls_mode_valid_p (V32HImode) && TARGET_MIN_VLEN >= 64")
  (V64HI "riscv_vector::vls_mode_valid_p (V64HImode) && TARGET_MIN_VLEN >= 128")
  (V128HI "riscv_vector::vls_mode_valid_p (V128HImode) && TARGET_MIN_VLEN >= 256")
  (V256HI "riscv_vector::vls_mode_valid_p (V256HImode) && TARGET_MIN_VLEN >= 512")
  (V512HI "riscv_vector::vls_mode_valid_p (V512HImode) && TARGET_MIN_VLEN >= 1024")
  (V1024HI "riscv_vector::vls_mode_valid_p (V1024HImode) && TARGET_MIN_VLEN >= 2048")
  (V2048HI "riscv_vector::vls_mode_valid_p (V2048HImode) && TARGET_MIN_VLEN >= 4096")
  (V32SI "riscv_vector::vls_mode_valid_p (V32SImode) && TARGET_MIN_VLEN >= 128")
  (V64SI "riscv_vector::vls_mode_valid_p (V64SImode) && TARGET_MIN_VLEN >= 256")
  (V128SI "riscv_vector::vls_mode_valid_p (V128SImode) && TARGET_MIN_VLEN >= 512")
  (V256SI "riscv_vector::vls_mode_valid_p (V256SImode) && TARGET_MIN_VLEN >= 1024")
  (V512SI "riscv_vector::vls_mode_valid_p (V512SImode) && TARGET_MIN_VLEN >= 2048")
  (V1024SI "riscv_vector::vls_mode_valid_p (V1024SImode) && TARGET_MIN_VLEN >= 4096")
  (V32DI "riscv_vector::vls_mode_valid_p (V32DImode) && TARGET_VECTOR_ELEN_64 && TARGET_MIN_VLEN >= 256")
  (V64DI "riscv_vector::vls_mode_valid_p (V64DImode) && TARGET_VECTOR_ELEN_64 && TARGET_MIN_VLEN >= 512")
  (V128DI "riscv_vector::vls_mode_valid_p (V128DImode) && TARGET_VECTOR_ELEN_64 && TARGET_MIN_VLEN >= 1024")
  (V256DI "riscv_vector::vls_mode_valid_p (V256DImode) && TARGET_VECTOR_ELEN_64 && TARGET_MIN_VLEN >= 2048")
  (V512DI "riscv_vector::vls_mode_valid_p (V512DImode) && TARGET_VECTOR_ELEN_64 && TARGET_MIN_VLEN >= 4096")
  (V32HF "riscv_vector::vls_mode_valid_p (V32HFmode) && TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN >= 64")
  (V64HF "riscv_vector::vls_mode_valid_p (V64HFmode) && TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN >= 128")
  (V128HF "riscv_vector::vls_mode_valid_p (V128HFmode) && TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN >= 256")
  (V256HF "riscv_vector::vls_mode_valid_p (V256HFmode) && TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN >= 512")
  (V512HF "riscv_vector::vls_mode_valid_p (V512HFmode) && TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN >= 1024")
  (V1024HF "riscv_vector::vls_mode_valid_p (V1024HFmode) && TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN >= 2048")
  (V2048HF "riscv_vector::vls_mode_valid_p (V2048HFmode) && TARGET_VECTOR_ELEN_FP_16 && TARGET_MIN_VLEN >= 4096")
  (V32SF "riscv_vector::vls_mode_valid_p (V32SFmode) && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN >= 128")
  (V64SF "riscv_vector::vls_mode_valid_p (V64SFmode) && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN >= 256")
  (V128SF "riscv_vector::vls_mode_valid_p (V128SFmode) && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN >= 512")
  (V256SF "riscv_vector::vls_mode_valid_p (V256SFmode) && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN >= 1024")
  (V512SF "riscv_vector::vls_mode_valid_p (V512SFmode) && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN >= 2048")
  (V1024SF "riscv_vector::vls_mode_valid_p (V1024SFmode) && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN >= 4096")
  (V32DF "riscv_vector::vls_mode_valid_p (V32DFmode) && TARGET_VECTOR_ELEN_FP_64 && TARGET_MIN_VLEN >= 256")
  (V64DF "riscv_vector::vls_mode_valid_p (V64DFmode) && TARGET_VECTOR_ELEN_FP_64 && TARGET_MIN_VLEN >= 512")
  (V128DF "riscv_vector::vls_mode_valid_p (V128DFmode) && TARGET_VECTOR_ELEN_FP_64 && TARGET_MIN_VLEN >= 1024")
  (V256DF "riscv_vector::vls_mode_valid_p (V256DFmode) && TARGET_VECTOR_ELEN_FP_64 && TARGET_MIN_VLEN >= 2048")
  (V512DF "riscv_vector::vls_mode_valid_p (V512DFmode) && TARGET_VECTOR_ELEN_FP_64 && TARGET_MIN_VLEN >= 4096")

  (V32BI "riscv_vector::vls_mode_valid_p (V32BImode)")
  (V64BI "riscv_vector::vls_mode_valid_p (V64BImode) && TARGET_MIN_VLEN >= 64")
  (V128BI "riscv_vector::vls_mode_valid_p (V128BImode) && TARGET_MIN_VLEN >= 128")
  (V256BI "riscv_vector::vls_mode_valid_p (V256BImode) && TARGET_MIN_VLEN >= 256")
  (V512BI "riscv_vector::vls_mode_valid_p (V512BImode) && TARGET_MIN_VLEN >= 512")
  (V1024BI "riscv_vector::vls_mode_valid_p (V1024BImode) && TARGET_MIN_VLEN >= 1024")
  (V2048BI "riscv_vector::vls_mode_valid_p (V2048BImode) && TARGET_MIN_VLEN >= 2048")
  (V4096BI "riscv_vector::vls_mode_valid_p (V4096BImode) && TARGET_MIN_VLEN >= 4096")
])

(define_insn "@th_vmov<mode>"
  [(set (match_operand:VALL 0 "reg_or_mem_operand" "=vr,vr,m")
	(unspec:VALL
	  [(match_operand:VALL 1 "reg_or_mem_operand" "vr,m,vr")
	   (reg:SI VL_REGNUM)]
	 UNSPEC_TH_VWLDST))
   (use (reg:SI VTYPE_REGNUM))]
  "TARGET_XTHEADVECTOR"
  "@
   vmv.v.v\t%0,%1
   vle.v\t%0,%1
   vse.v\t%1,%0"
  [(set_attr "type" "vector")])

(define_expand "@th_whole_mov<mode>"
  [(set (match_operand:VALL 0 "reg_or_mem_operand")
	(unspec:VALL
          [(match_operand:VALL 1 "reg_or_mem_operand")]
        UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
{})

(define_insn_and_split "*th_whole_mov<mode>"
  [(set (match_operand:VB_VLS 0 "reg_or_mem_operand"    "=vr, m,vr")
	(unspec:VB_VLS
          [(match_operand:VB_VLS 1 "reg_or_mem_operand" "  m,vr,vr")]
        UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  {
    if (!(REG_P (operands[0]) && REG_P (operands[1])
	&& (REGNO (operands[0]) == REGNO (operands[1]))))
      {
	riscv_vector::emit_hard_vlmax_vsetvl (RVVM1QImode, RVV_VLMAX);
	emit_insn (gen_th_vmov (<MODE>mode, operands[0], operands[1]));
      }
    else
      emit_note (NOTE_INSN_DELETED);
    DONE;
  }
  [(set_attr "type" "vector")])

(define_insn_and_split "*th_whole_mov<mode>"
  [(set (match_operand:VALL 0 "reg_or_mem_operand"    "=vr, m,vr")
	(unspec:VALL
          [(match_operand:VALL 1 "reg_or_mem_operand" "  m,vr,vr")]
        UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  {
    if (!(REG_P (operands[0]) && REG_P (operands[1])
	&& (REGNO (operands[0]) == REGNO (operands[1]))))
      {
	riscv_vector::emit_hard_vlmax_vsetvl (<MODE>mode, RVV_VLMAX);
	emit_insn (gen_th_vmov (<MODE>mode, operands[0], operands[1]));
      }
    else
      emit_note (NOTE_INSN_DELETED);
    DONE;
  }
  [(set_attr "type" "vector")])

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

(define_insn "@th_vsetvl<mode>"
  [(set (match_operand:P 0 "register_operand" "=r")
	(unspec:P [(match_operand:P 1 "csr_operand" "rK")
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
(define_insn "th_vsetvl_vtype_change_only"
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
(define_insn "@th_vsetvl_discard_result<mode>"
  [(set (reg:SI VL_REGNUM)
	(unspec:SI [(match_operand:P 0 "csr_operand" "rK")
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
	(unspec:P [(match_operand:P 1 "csr_operand" "rK")
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
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
  "TARGET_VECTOR"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_le_one (<MODE>mode)"
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
  "TARGET_VECTOR && riscv_vector::cmp_lmul_gt_one (<MODE>mode)"
  "vmf%B3.vf\t%0,%4,%5%p1"
  [(set_attr "type" "vfcmp")
   (set_attr "mode" "<MODE>")])