/* Definition of RISC-V target for GNU compiler.
   Copyright (C) 2016-2020 Free Software Foundation, Inc.
   Contributed by Andrew Waterman (andrew@sifive.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_RISCV_OPTS_H
#define GCC_RISCV_OPTS_H

enum riscv_abi_type {
  ABI_ILP32,
  ABI_ILP32E,
  ABI_ILP32F,
  ABI_ILP32D,
  ABI_LP64,
  ABI_LP64F,
  ABI_LP64D,
  ABI_LP64DV,
  ABI_LP64V
};
extern enum riscv_abi_type riscv_abi;

enum riscv_code_model {
  CM_MEDLOW,
  CM_MEDANY,
  CM_PIC
};
extern enum riscv_code_model riscv_cmodel;

enum riscv_isa_spec_class {
  ISA_SPEC_CLASS_NONE,

  ISA_SPEC_CLASS_2P2,
  ISA_SPEC_CLASS_20190608,
  ISA_SPEC_CLASS_20191213
};

extern enum riscv_isa_spec_class riscv_isa_spec;

/* Keep this list in sync with define_attr "tune" in riscv.md.  */
enum riscv_microarchitecture_type {
  generic,
  sifive_7,
  c910,
  c906v,
  c908,
  c907
};
extern enum riscv_microarchitecture_type riscv_microarchitecture;

enum riscv_align_data {
  riscv_align_data_type_xlen,
  riscv_align_data_type_natural
};

/* RVV vector register sizes.  */
enum riscv_rvv_vector_bits_enum {
  RVV_SCALABLE,
  RVV_NOT_IMPLEMENTED = RVV_SCALABLE,
  RVV_64 = 64,
  RVV_128 = 128,
  RVV_256 = 256,
  RVV_512 = 512,
  RVV_1024 = 1024
};

/* Enumerates the possible extraction_insn operations.  */
enum riscv_extraction_pattern { RISCV_EP_INSV, RISCV_EP_EXTV, RISCV_EP_EXTZV };

#define MASK_ZBA (1 << 0)
#define MASK_ZBB (1 << 1)
#define MASK_ZBC (1 << 6)
#define MASK_ZBS (1 << 2)

#define TARGET_ZBA ((riscv_bitmanip_subext & MASK_ZBA) != 0)
#define TARGET_ZBB ((riscv_bitmanip_subext & MASK_ZBB) != 0)
#define TARGET_ZBC ((riscv_bitmanip_subext & MASK_ZBC) != 0)
#define TARGET_ZBS ((riscv_bitmanip_subext & MASK_ZBS) != 0)

#define MASK_ZICSR    (1 << 0)
#define MASK_ZIFENCEI (1 << 1)

#define TARGET_ZICSR    ((riscv_zi_subext & MASK_ZICSR) != 0)
#define TARGET_ZIFENCEI ((riscv_zi_subext & MASK_ZIFENCEI) != 0)

#define MASK_ZCA      (1 << 0)
#define MASK_ZCB      (1 << 1)
#define MASK_ZCF      (1 << 3)
#define MASK_ZCD      (1 << 4)

#define TARGET_ZCA    ((riscv_zc_subext & MASK_ZCA) != 0)
#define TARGET_ZCB    ((riscv_zc_subext & MASK_ZCB) != 0)
#define TARGET_ZCF    ((riscv_zc_subext & MASK_ZCF) != 0)
#define TARGET_ZCD    ((riscv_zc_subext & MASK_ZCD) != 0)

#endif /* ! GCC_RISCV_OPTS_H */
