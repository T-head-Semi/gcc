#!/usr/bin/env python3

import os
import sys

import re
import copy

from operator import itemgetter, methodcaller
from itertools import groupby

import math

scritp_version = "2.0"
scritp_dirname, script_filename = os.path.split(os.path.abspath(sys.argv[0]))

simdctypes = {
    "int8xN": {"type32": "int8x4_t", "mode32": "v4qi",
               "type64": "int8x8_t", "mode64": "v8qi",
               "ulong": "uintXLEN_t"},
    "int16xN": {"type32": "int16x2_t", "mode32": "v2hi",
                "type64": "int16x4_t", "mode64": "v4hi",
                "ulong": "uintXLEN_t"},
    "int32xN": {"type32": "int32_t", "mode32": "si",
                "type64": "int32x2_t", "mode64": "v2si",
                "ulong": "intXLEN_t"},
    "uint8xN": {"type32": "uint8x4_t", "mode32": "v4qi",
                "type64": "uint8x8_t", "mode64": "v8qi",
                "ulong": "uintXLEN_t"},
    "uint16xN": {"type32": "uint16x2_t", "mode32": "v2hi",
                 "type64": "uint16x4_t", "mode64": "v4hi",
                 "ulong": "uintXLEN_t"},
    "uint32xN": {"type32": "uint32_t", "mode32": "si",
                 "type64": "uint32x2_t", "mode64": "v2si",
                 "ulong": "uintXLEN_t"},
    "int64xN": {"type": "int64_t", "mode": "di",
                "ulong": "intXLEN_t"},
    "uint64xN": {"type": "uint64_t", "mode": "di",
                "ulong": "uintXLEN_t"},
    "int32x2": {"type": "int32x2_t", "mode": "v2si", "ulong": "uint64_t"},
    "uint32x2": {"type": "uint32x2_t", "mode": "v2si", "ulong": "uint64_t"},
    "int16x2": {"type": "int16x2_t", "mode": "v2hi", "ulong": "uint32_t"},
    "uint16x2": {"type": "uint16x2_t", "mode": "v2hi", "ulong": "uint32_t"},
    "int16x4": {"type": "int16x4_t", "mode": "v4hi", "ulong": "uint64_t"},
    "uint16x4": {"type": "uint16x4_t", "mode": "v4hi", "ulong": "uint64_t"},
    "int8x4": {"type": "int8x4_t", "mode": "v4qi", "ulong": "uint32_t"},
    "uint8x4": {"type": "uint8x4_t", "mode": "v4qi", "ulong": "uint32_t"},
    "int64": {"type": "int64_t", "mode": "di", "ulong": "int64_t"},
    "uint64": {"type": "uint64_t", "mode": "di", "ulong": "uint64_t"},
    "int32": {"type": "int32_t", "mode": "si", "ulong": "int32_t"},
    "uint32": {"type": "uint32_t", "mode": "si", "ulong": "uint32_t"},
    "integer": {"type32": "int32_t", "mode32": "si",
              "type64": "int64_t", "mode64": "di",
              "ulong": "intXLEN_t"},
    "uinteger": {"type32": "uint32_t", "mode32": "si",
               "type64": "uint64_t", "mode64": "di",
               "ulong": "uintXLEN_t"},
    "bitv": {"type": "int32_t", "mode": "si", "ulong": "int32_t" },
    "ubitv": {"type": "uint32_t", "mode": "si", "ulong": "uint32_t" },
    "intXLEN": {"type32": "intXLEN_t", "mode32": "si",
              "type64": "intXLEN_t", "mode64": "di",
              "ulong": "intXLEN_t"},
    "uintXLEN": {"type32": "uintXLEN_t", "mode32": "si",
               "type64": "uintXLEN_t", "mode64": "di",
               "ulong": "uintXLEN_t"},
}

isubc_tables = [
# index 0~10
        "16-bit Addition & Subtraction",
        "8-bit Addition & Subtraction",
        "16-bit Shift",
        "8-bit Shift",
        "16-bit Compare",
        "8-bit Compare",
        "16-bit Multiply",
        "8-bit Multiply",
        "16-bit Misc",
        "8-bit Misc",
        "8-bit Unpacking",
# index 11~17
        "16-bit Packing",
        "Most Significant Word 32x32 Multiply and Add",
        "Most Significant Word 32x16 Multiply and Add",
        "Signed 16-bit Multiply with 32-bit Add/Subtract",
        "Signed 16-bit Multiply with 64-bit Add/Subtract",
        "Miscellaneous",
        "8-bit Multiply with 32-bit Add",
# index 18~20
        "64-bit Addition & Subtraction",
        "32-bit Multiply with 64-bit Add/Subtract",
        "Signed 16-bit Multiply with 64-bit ADD//Subtract",
# index 21~25
        "16-bit Signed Saturation",
        "32-bit Signed Saturation",
        "32-bit Computation",
        "Overflow/Saturation status manipulation instructions",
        "Miscellaneous",
# index 26~34
        "SIMD 32-bit Addition & Subtraction",
        "SIMD 32-bit Shift",
        "SIMD 32-bit Miscellaneous",
        "SIMD 16-bit Signed Saturating Multiply",
        "32-bit Multiply",
        "32-bit Multiply and Add",
        "32-bit Parallel Multiply and Add",
        "Non-SIMD 32-bit Shift",
        "32-bit Packing"
]

builtins_only64 = [
    {"name": "$insn",
     "prototype": ["?int32xN", "?int32xN a", "?int32xN b"],
     "insn": ["add32", "sub32", "cras32", "crsa32", "stas32", "stsa32"],
     "available": "64",
     "subclass": 26,
     "idoc": ["ADD32 (SIMD 32-bit Addition)@+",
              "SUB32 (SIMD 32-bit Subtraction)@-",
              "CRAS32 (SIMD 32-bit Cross Addition & Subtraction)",
              "CRSA32 (SIMD 32-bit Cross Subtraction & Addition)",
              "STAS32 (SIMD 32-bit signed Straight Addition & Subtraction)",
              "STSA32 (SIMD 32-bit signed Straight Subtraction & Addition)"],
     "operation":
         "RV32: Unreachable();\n"
         "res[0] = a[0] $idoc b[0];\n"
         "res[1] = a[1] $idoc b[1];\n"
         "return res;\n",
     "operation@cras32":
         "RV32: Unreachable();\n"
         "res[1] = a[1] + b[0];\n"
         "res[0] = a[0] - b[1];\n"
         "return res;\n",
     "operation@crsa32":
         "RV32: Unreachable();\n"
         "res[1] = a[1] - b[0];\n"
         "res[0] = a[0] + b[1];\n"
         "return res;\n",
     "operation@stas32":
         "RV32: Unreachable();\n"
         "res[1] = a[1] + b[1];\n"
         "res[0] = a[0] - b[0];\n"
         "return res;\n",
     "operation@stsa32":
         "RV32: Unreachable();\n"
         "res[1] = a[1] - b[1];\n"
         "res[0] = a[0] + b[0];\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN a", "uint32xN b"],
     "insn": ["ukadd32", "ukcras32", "ukcrsa32", "ukstas32", "ukstsa32", "uksub32",
              "umax32", "umin32", "uradd32", "urcras32", "urcrsa32", "urstas32", "urstsa32", "ursub32"],
     "available": "64",
     "isubclass": [26, 26, 26, 26, 26, 26, 28, 28, 26, 26, 26, 26, 26, 26],
     "idoc": ["UKADD32 (SIMD 32-bit Unsigned Saturating Addition)",
              "UKCRAS32 (SIMD 32-bit Unsigned Saturating Cross Addition & Subtraction)",
              "UKCRSA32 (SIMD 32-bit Unsigned Saturating Cross Subtraction & Addition)",
              "UKSTAS32 (SIMD 32-bit Unsigned Saturating Straight Addition & Subtraction)",
              "UKSTSA32 (SIMD 32-bit Unsigned Saturating Straight Subtraction & Addition)",
              "UKSUB32 (SIMD 32-bit Unsigned Saturating Subtraction)",
              "UMAX32 (SIMD 32-bit Unsigned Maximum)",
              "UMIN32 (SIMD 32-bit Unsigned Minimum)",
              "URADD32 (SIMD 32-bit Unsigned Halving Addition)",
              "URCRAS32 (SIMD 32-bit Unsigned Halving Cross Addition & Subtraction)",
              "URCRSA32 (SIMD 32-bit Unsigned Halving Cross Subtraction & Addition)",
              "URSTAS32 (SIMD 32-bit Unsigned Halving Straight Addition & Subtraction)",
              "URSTSA32 (SIMD 32-bit Unsigned Halving Straightv Subtraction & Addition)",
              "URSUB32 (SIMD 32-bit Unsigned Halving Subtraction)"],
     "operation":
         "RV32: Unreachable();\n"
         "res[0] = Sat_u((uint64_t)a[0] + b[0], 32);\n"
         "res[1] = Sat_u((uint64_t)a[1] + b[1], 32);\n"
         "return res;\n",
     "operation@ukcras32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_u((uint64_t)a[1] + b[0], 32);\n"
         "res[0] = Sat_u((uint64_t)a[0] - b[1], 32);\n"
         "return res;\n",
     "operation@ukcrsa32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_u((uint64_t)a[1] - b[0], 32);\n"
         "res[0] = Sat_u((uint64_t)a[0] + b[1], 32);\n"
         "return res;\n",
     "operation@ukstas32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_u((uint64_t)a[1] + b[1], 32);\n"
         "res[0] = Sat_u((uint64_t)a[0] - b[0], 32);\n"
         "return res;\n",
     "operation@ukstsa32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_u((uint64_t)a[1] - b[1], 32);\n"
         "res[0] = Sat_u((uint64_t)a[0] + b[0], 32);\n"
         "return res;\n",
     "operation@uksub32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_u((uint64_t)a[1] - b[1], 32);\n"
         "res[0] = Sat_u((uint64_t)a[0] - b[0], 32);\n"
         "return res;\n",
     "operation@umax32":
         "RV32: Unreachable();\n"
         "res[1] = (a[1] > b[1]) ? a[1] : b[1];\n"
         "res[0] = (a[0] > b[0]) ? a[0] : b[0];\n"
         "return res;\n",
     "operation@umin32":
         "RV32: Unreachable();\n"
         "res[1] = (a[1] < b[1]) ? a[1] : b[1];\n"
         "res[0] = (a[0] < b[0]) ? a[0] : b[0];\n"
         "return res;\n",
     "operation@uradd32":
         "RV32: Unreachable();\n"
         "res[0] = ((uint64_t)a[0] + b[0]) >> 1;\n"
         "res[1] = ((uint64_t)a[1] + b[1]) >> 1;\n"
         "return res;\n",
     "operation@urcras32":
         "RV32: Unreachable();\n"
         "res[1] = ((uint64_t)a[1] + b[0]) >> 1;\n"
         "res[0] = ((uint64_t)a[0] - b[1]) >> 1;\n"
         "return res;\n",
     "operation@urcrsa32":
         "RV32: Unreachable();\n"
         "res[1] = ((uint64_t)a[1] - b[0]) >> 1;\n"
         "res[0] = ((uint64_t)a[0] + b[1]) >> 1;\n"
         "return res;\n",
     "operation@urstas32":
         "RV32: Unreachable();\n"
         "res[1] = ((uint64_t)a[1] + b[1]) >> 1;\n"
         "res[0] = ((uint64_t)a[0] - b[0]) >> 1;\n"
         "return res;\n",
     "operation@urstsa32":
         "RV32: Unreachable();\n"
         "res[1] = ((uint64_t)a[1] - b[1]) >> 1;\n"
         "res[0] = ((uint64_t)a[0] + b[0]) >> 1;\n"
         "return res;\n",
     "operation@ursub32":
         "RV32: Unreachable();\n"
         "res[1] = ((uint64_t)a[1] - b[1]) >> 1;\n"
         "res[0] = ((uint64_t)a[0] - b[0]) >> 1;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "int32xN b"],
     "insn": ["kadd32", "kcras32", "kcrsa32"],
     "available": "64",
     "subclass": 26,
     "idoc": ["KADD32 (SIMD 32-bit Signed Saturating Addition)",
              "KCRAS32 (SIMD 32-bit Signed Saturating Cross Addition & Subtraction)",
              "KCRSA32 (SIMD 32-bit Signed Saturating Cross Subtraction & Addition)"],
     "operation" :
         "RV32: Unreachable();\n"
         "res[0] = Sat_s((int64_t)a[0] + b[0], 32);\n"
         "res[1] = Sat_s((int64_t)a[1] + b[1], 32);\n"
         "return res;\n",
     "operation@kcras32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_s((int64_t)a[1] + b[0], 32);\n"
         "res[0] = Sat_s((int64_t)a[0] - b[1], 32);\n"
         "return res;\n",
     "operation@kcrsa32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_s((int64_t)a[1] - b[0], 32);\n"
         "res[0] = Sat_s((int64_t)a[0] + b[1], 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "int32xN b"],
     "insn": ["smax32", "smin32", "kstas32", "kstsa32", "ksub32",
              "radd32", "rcras32", "rcrsa32", "rstas32", "rstsa32", "rsub32"],
     "available": "64",
     "isubclass": [28, 28, 26, 26, 26, 26, 26, 26, 26, 26, 26],
     "idoc": ["SMAX32 (SIMD 32-bit Signed Maximum)",
              "SMIN32 (SIMD 32-bit Signed Minimum)",
              "KSTAS32 (SIMD 32-bit Signed Saturating Straight Addition & Subtraction)",
              "KSTSA32 (SIMD 32-bit Signed Saturating Straight Subtraction & Addition)",
              "KSUB32 (SIMD 32-bit Signed Saturating Subtraction)",
              "RADD32 (SIMD 32-bit Signed Halving Addition)",
              "RCRAS32 (SIMD 32-bit Signed Halving Cross Addition & Subtraction)",
              "RCRSA32 (SIMD 32-bit Signed Halving Cross Subtraction & Addition)",
              "RSTAS32 (SIMD 32-bit Signed Halving Straight Addition & Subtraction)",
              "RSTSA32 (SIMD 32-bit Signed Halving Straight Subtraction & Addition)",
              "RSUB32 (SIMD 32-bit Signed Halving Subtraction)"],
     "operation":
         "RV32: Unreachable();\n"
         "res[1] = (a[1] > b[1]) ? a[1] : b[1];\n"
         "res[0] = (a[0] > b[0]) ? a[0] : b[0];\n"
         "return res;\n",
     "operation@smin32":
         "RV32: Unreachable();\n"
         "res[1] = (a[1] < b[1]) ? a[1] : b[1];\n"
         "res[0] = (a[0] < b[0]) ? a[0] : b[0];\n"
         "return res;\n",
     "operation@kstas32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_s((int64_t)a[1] + b[1], 32);\n"
         "res[0] = Sat_s((int64_t)a[0] - b[0], 32);\n"
         "return res;\n",
     "operation@kstsa32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_s((int64_t)a[1] - b[1], 32);\n"
         "res[0] = Sat_s((int64_t)a[0] + b[0], 32);\n"
         "return res;\n",
     "operation@ksub32":
         "RV32: Unreachable();\n"
         "res[1] = Sat_s((int64_t)a[1] - b[1], 32);\n"
         "res[0] = Sat_s((int64_t)a[0] - b[0], 32);\n"
         "return res;\n",
     "operation@radd32":
         "RV32: Unreachable();\n"
         "res[1] = ((int64_t)a[1] + b[1]) >> 1;\n"
         "res[0] = ((int64_t)a[0] + b[0]) >> 1;\n"
         "return res;\n",
     "operation@rcras32":
         "RV32: Unreachable();\n"
         "res[1] = ((int64_t)a[1] + b[0]) >> 1;\n"
         "res[0] = ((int64_t)a[0] - b[1]) >> 1;\n"
         "return res;\n",
     "operation@rcrsa32":
         "RV32: Unreachable();\n"
         "res[1] = ((int64_t)a[1] - b[0]) >> 1;\n"
         "res[0] = ((int64_t)a[0] + b[1]) >> 1;\n"
         "return res;\n",
     "operation@rstas32":
         "RV32: Unreachable();\n"
         "res[1] = ((int64_t)a[1] + b[1]) >> 1;\n"
         "res[0] = ((int64_t)a[0] - b[0]) >> 1;\n"
         "return res;\n",
     "operation@rstsa32":
         "RV32: Unreachable();\n"
         "res[1] = ((int64_t)a[1] - b[1]) >> 1;\n"
         "res[0] = ((int64_t)a[0] + b[0]) >> 1;\n"
         "return res;\n",
     "operation@rsub32":
         "RV32: Unreachable();\n"
         "res[1] = ((int64_t)a[1] - b[1]) >> 1;\n"
         "res[0] = ((int64_t)a[0] - b[0]) >> 1;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN a", "uimm5 b"],
     "insn": ["sll32"],
     "available": "64",
     "subclass": 27,
     "idoc": ["SLL32 (SIMD 32-bit Shift Left Logical)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = a[0] << sa;\n"
         "res[1] = a[1] << sa;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN a", "const uimm5 b"],
     "insn": ["slli32"],
     "available": "64",
     "subclass": 27,
     "idoc": ["SLLI32 (SIMD 32-bit Shift Left Logical Immediate)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = a[0] << sa;\n"
         "res[1] = a[1] << sa;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "uimm5 b"],
     "insn": ["sra32", "sra32.u", "ksll32"],
     "available": "64",
     "subclass": 27,
     "idoc": ["SRA32 (SIMD 32-bit Shift Right Arithmetic)",
              "SRA32.u (SIMD 32-bit Rounding Shift Right Arithmetic)",
              "KSLL32 (SIMD 32-bit Saturating Shift Left Logical)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = a[0] >> sa;\n"
         "res[1] = a[1] >> sa;\n"
         "return res;\n",
     "operation@sra32.u":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = Round(a[0], sa);\n"
         "res[1] = Round(a[1], sa);\n"
         "return res;\n",
     "operation@ksll32":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = Sat_s((int64_t)a[0] << sa, 32);\n"
         "res[1] = Sat_s((int64_t)a[1] << sa, 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "bitv b"],
     "insn": ["kslra32", "kslra32.u"],
     "available": "64",
     "subclass": 27,
     "idoc": ["KSLRA32 (SIMD 32-bit Shift Left Logical with Saturation or Shift Right Arithmetic)",
              "KSLRA32.u (SIMD 32-bit Shift Left Logical with Saturation or Rounding Shift Right Arithmetic)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "if ((b & 0x20) != 0) {\n"
         "  sa = (sa == 0) ? 31 : (sa ^ 0x1f) + 1;\n"
         "  res[0] = a[0] >> sa;\n"
         "  res[1] = a[1] >> sa;\n"
         "}\n"
         "else {\n"
         "  res[0] = Sat_s((int64_t)a[0] << sa, 32);\n"
         "  res[1] = Sat_s((int64_t)a[1] << sa, 32);\n"
         "}\n"
         "return res;\n",
     "operation@kslra32.u":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "if ((b & 0x20) != 0) {\n"
         "  sa = (sa == 0) ? 31 : (sa ^ 0x1f) + 1;\n"
         "  res[0] = Round(a[0], sa);\n"
         "  res[1] = Round(a[1], sa);\n"
         "}\n"
         "else {\n"
         "  res[0] = Sat_s((int64_t)a[0] << sa, 32);\n"
         "  res[1] = Sat_s((int64_t)a[1] << sa, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "const uimm5 b"],
     "insn": ["srai32", "srai32.u", "kslli32"],
     "available": "64",
     "subclass": 27,
     "idoc": ["SRAI32 (SIMD 32-bit Shift Right Arithmetic Immediate)",
              "SRAI32.u (SIMD 32-bit Rounding Shift Right Arithmetic Immediate)",
              "KSLLI32 (SIMD 32-bit Saturating Shift Left Logical Immediate)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = a[0] >> sa;\n"
         "res[1] = a[1] >> sa;\n"
         "return res;\n",
     "operation@srai32.u":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = Round(a[0], sa);\n"
         "res[1] = Round(a[1], sa);\n"
         "return res;\n",
     "operation@kslli32":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = Sat_s((int64_t)a[0] << sa, 32);\n"
         "res[1] = Sat_s((int64_t)a[1] << sa, 32);\n"
         "return res;\n",
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN a", "uimm5 b"],
     "insn": ["srl32", "srl32.u"],
     "available": "64",
     "subclass": 27,
     "idoc": ["SRL32 (SIMD 32-bit Shift Right Logical)",
              "SRL32.u (SIMD 32-bit Rounding Shift Right Logical)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = a[0] >> sa;\n"
         "res[1] = a[1] >> sa;\n"
         "return res;\n",
     "operation@srl32.u":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = Round(a[0], sa);\n"
         "res[1] = Round(a[1], sa);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN a", "const uimm5 b"],
     "insn": ["srli32", "srli32.u"],
     "available": "64",
     "subclass": 27,
     "idoc": ["SRLI32 (SIMD 32-bit Shift Right Logical Immediate)",
               "SRLI32.u (SIMD 32-bit Rounding Shift Right Logical Immediate)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = a[0] >> sa;\n"
         "res[1] = a[1] >> sa;\n"
         "return res;",
     "operation@srli32.u":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res[0] = Round(a[0], sa);\n"
         "res[1] = Round(a[1], sa);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a"],
     "insn": ["kabs32"],
     "available": "64",
     "subclass": 28,
     "idoc": ["KABS32 (Scalar 32-bit Absolute Value with Saturation)"],
     "operation":
         "RV32: Unreachable();\n"
         "res[0] = Sat_s(Abs((int64_t)a[0]), 32);\n"
         "res[1] = Sat_s(Abs((int64_t)a[1]), 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int16xN a", "int16xN b"],
     "insn": ["kdmbb16", "kdmbt16", "kdmtt16"],
     "available": "64",
     "subclass": 29,
     "idoc": ["KDMBB16 (SIMD Signed Saturating Double Multiply B16 x B16)",
              "KDMBT16 (SIMD Signed Saturating Double Multiply B16 x T16)",
              "KDMTT16 (SIMD Signed Saturating Double Multiply T16 x T16)"],
     "operation":
         "RV32: Unreachable();\n"
         "res[0] = Sat_s((int64_t)a[0] * b[0] << 1, 32);\n"
         "res[1] = Sat_s((int64_t)a[2] * b[2] << 1, 32);\n"
         "return res;\n",
     "operation@kdmbt16":
         "RV32: Unreachable();\n"
         "res[0] = Sat_s((int64_t)a[0] * b[1] << 1, 32);\n"
         "res[1] = Sat_s((int64_t)a[2] * b[3] << 1, 32);\n"
         "return res;\n",
     "operation@kdmtt16":
         "RV32: Unreachable();\n"
         "res[0] = Sat_s((int64_t)a[1] * b[1] << 1, 32);\n"
         "res[1] = Sat_s((int64_t)a[3] * b[3] << 1, 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int16xN a", "int16xN b"],
     "insn": ["kdmabb16", "kdmabt16", "kdmatt16"],
     "available": "64",
     "subclass": 29,
     "idoc": ["KDMABB16 (SIMD Signed Saturating Double Multiply Addition B16 x B16)",
              "KDMABT16 (SIMD Signed Saturating Double Multiply Addition B16 x T16)",
              "KDMATT16 (SIMD Signed Saturating Double Multiply Addition T16 x T16)"],
     "operation":
         "RV32: Unreachable();\n"
          "int64_t tmp[2];\n"
          "tmp[0] = Sat_s((int64_t)a[0] * b[0] << 1, 32);\n"
          "tmp[1] = Sat_s((int64_t)a[2] * b[2] << 1, 32);\n"
          "tmp[0] = t[0] + tmp[0];\n"
          "tmp[1] = t[1] + tmp[1];\n"
          "res[0] = Sat_s(tmp[0], 32);\n"
          "res[1] = Sat_s(tmp[1], 32);\n"
          "return res;\n",
    "operation@kdmabt16":
         "RV32: Unreachable();\n"
         "int64_t tmp[2];\n"
         "tmp[0] = Sat_s((int64_t)a[0] * b[1] << 1, 32);\n"
         "tmp[1] = Sat_s((int64_t)a[2] * b[3] << 1, 32);\n"
         "tmp[0] = t[0] + tmp[0];\n"
         "tmp[1] = t[1] + tmp[1];\n"
         "res[0] = Sat_s(tmp[0], 32);\n"
         "res[1] = Sat_s(tmp[1], 32);\n"
         "return res;\n",
    "operation@kdmatt16":
         "RV32: Unreachable();\n"
         "int64_t tmp[2];\n"
         "tmp[0] = Sat_s((int64_t)a[1] * b[1] << 1, 32);\n"
         "tmp[1] = Sat_s((int64_t)a[3] * b[3] << 1, 32);\n"
         "tmp[0] = t[0] + tmp[0];\n"
         "tmp[1] = t[1] + tmp[1];\n"
         "res[0] = Sat_s(tmp[0], 32);\n"
         "res[1] = Sat_s(tmp[1], 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int16xN a", "int16xN b"],
     "insn": ["khmbb16", "khmbt16", "khmtt16"],
     "available": "64",
     "subclass": 29,
     "idoc": ["KHMBB16 (SIMD Signed Saturating Half Multiply B16 x B16)",
              "KHMBT16 (SIMD Signed Saturating Half Multiply B16 x T16)",
              "KHMTT16 (SIMD Signed Saturating Half Multiply T16 x T16)"],
     "operation":
         "RV32: Unreachable();\n"
         "res[0] = Sat_s(a[0] * b[0] >> 15, 16);\n"
         "res[1] = Sat_s(a[2] * b[2] >> 15, 16);\n"
         "return res;\n",
    "operation@khmbt16":
         "RV32: Unreachable();\n"
         "res[0] = Sat_s(a[0] * b[1] >> 15, 16);\n"
         "res[1] = Sat_s(a[2] * b[3] >> 15, 16);\n"
         "return res;\n",
    "operation@khmtt16":
         "RV32: Unreachable();\n"
         "res[0] = Sat_s(a[1] * b[1] >> 15, 16);\n"
         "res[1] = Sat_s(a[3] * b[3] >> 15, 16);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64xN", "int64xN t", "int32xN a", "int32xN b"],
     "insn": ["kmabb32", "kmabt32", "kmatt32", "kmada32", "kmaxda32", "kmads32", "kmadrs32", "kmaxds32", "kmsda32",
              "kmsxda32"],
     "available": "64",
     "isubclass": [30, 30, 30, 31, 31, 31, 31, 31, 31, 31],
     "idoc": ["KMABB32 (Saturating Signed Multiply Bottom Words & Add)",
              "KMABT32 (Saturating Signed Multiply Bottom & Top Words & Add)",
              "KMATT32 (Saturating Signed Multiply Top Words & Add)",
              "KMADA32 (Saturating Signed Multiply Two Words and Two Adds)",
              "KMAXDA32 (Saturating Signed Crossed Multiply Two Words and Two Adds)",
              "KMADS32 (Saturating Signed Multiply Two Words & Subtract & Add)",
              "KMADRS32 (Saturating Signed Multiply Two Words & Reverse Subtract & Add)",
              "KMAXDS32 (Saturating Signed Crossed Multiply Two Words & Subtract & Add)",
              "KMSDA32 (Saturating Signed Multiply Two Words & Add & Subtract)",
              "KMSXDA32 (Saturating Signed Crossed Multiply Two Words & Add & Subtract)"],
     "operation":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[0] * b[0];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmabt32":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[0] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmatt32":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[1] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmada32":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[0] * b[0] + (int128)a[1] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmaxda32":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[1] * b[0] + (int128)a[0] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmads32":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[1] * b[1] - (int128)a[0] * b[0];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmadrs32":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[0] * b[0] - (int128)a[1] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmaxds32":
         "RV32: Unreachable();\n"
         "res = t + (int128)a[1] * b[0] - (int128)a[0] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmsda32":
         "RV32: Unreachable();\n"
         "res = t - (int128)a[0] * b[0] - (int128)a[1] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n",
     "operation@kmsxda32":
         "RV32: Unreachable();\n"
         "res = t - (int128)a[1] * b[0] - (int128)a[0] * b[1];\n"
         "res = Sat_s(res, 64);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64xN", "int32xN a", "int32xN b"],
     "insn": ["kmda32", "kmxda32"],
     "available": "64",
     "subclass": 32,
     "idoc": ["KMDA32 (Signed Multiply Two Words and Add)",
              "KMXDA32 (Signed Crossed Multiply Two Words and Add)"],
     "operation":
         "RV32: Unreachable();\n"
         "res = Sat_s((int28)a[0] * b[0] + (int128)a[1] * b[1], 64);\n"
         "return res;\n",
    "operation@kmxda32":
         "RV32: Unreachable();\n"
         "res = Sat_s((int28)a[0] * b[1] + (int128)a[1] * b[0], 64);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64xN", "int32xN a", "int32xN b"],
     "insn": ["smbb32", "smbt32", "smtt32", "smds32", "smdrs32", "smxds32"],
     "available": "64",
     "isubclass": [29, 29, 29, 31, 31, 31],
     "idoc": ["SMBB32 (Signed Multiply Bottom Word & Bottom Word)",
              "SMBT32 (Signed Multiply Bottom Word & Top Word)",
              "SMTT32 (Signed Multiply Top Word & Top Word)",
              "SMDS32 (Signed Multiply Two Words and Subtract)",
              "SMDRS32 (Signed Multiply Two Words and Reverse Subtract)",
              "SMXDS32 (Signed Crossed Multiply Two Words and Subtract)"],
     "operation":
         "RV32: Unreachable();\n"
         "res = a[0] * b[0];\n"
         "return res;\n",
     "operation@smbt32":
         "RV32: Unreachable();\n"
         "res = a[0] * b[1];\n"
         "return res;\n",
     "operation@smtt32":
         "RV32: Unreachable();\n"
         "res = a[1] * b[1];\n"
         "return res;\n",
     "operation@smds32":
         "RV32: Unreachable();\n"
         "res = a[1] * b[1] - a[0] * b[0];\n"
         "return res;\n",
     "operation@smdrs32":
         "RV32: Unreachable();\n"
         "res = a[0] * b[0] - a[1] * b[1];\n"
         "return res;\n",
     "operation@smxds32":
         "RV32: Unreachable();\n"
         "res = a[1] * b[0] - a[0] * b[1];\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32", "int32 a", "const uimm5 b"],
     "insn": ["sraiw.u"],
     "available": "64",
     "subclass": 34,
     "idoc": ["SRAIW.u (Rounding Shift Right Arithmetic Immediate Word)"],
     "operation":
         "RV32: Unreachable();\n"
         "sa = b & 0x1f;\n"
         "res = Round(a, sa);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN a", "uint32xN b"],
     "insn": ["pkbb32", "pkbt32", "pktb32", "pktt32"],
     "available": "64",
     "subclass": 34,
     "idoc": ["PKBB32 (Pack Two 32-bit Data from Both Bottom Half)@a[0],b[0]",
              "PKBT32 (Pack Two 32-bit Data from Bottom and Top Half)@a[0],b[1]",
              "PKTB32 (Pack Two 32-bit Data from Top and Bottom Half)@a[1],b[0]",
              "PKTT32 (Pack Two 32-bit Data from Both Top Half)@a[1],b[1]"],
     "operation":
         "RV32: Unreachable();\n"
         "res = Concat($idoc);\n"
         "return res;\n"
     }
]

builtins_non_simd = [
    {"name": "$insn",
     "prototype": ["integer", "int32 a", "int32 b"],
     "insn": ["kaddh", "ksubh"],
     "available": "both",
     "subclass": 21,
     "idoc": ["KADDH (Signed Addition with 16-bit Saturation)@+",
              "KSUBH (Signed Subtraction with 16-bit Saturation)@-"],
     "operation":
         "int64_t tmp = (int64_t)a $idoc b;\n"
         "res = Sat_s(tmp, 16);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "int32 a", "int32 b"],
     "insn": ["kaddw", "ksubw", "raddw", "rsubw", "maxw", "minw"],
     "available": "both",
     "isubclass": [22, 22, 23, 23, 23, 23],
     "idoc": ["KADDW (Signed Addition with 32-bit Saturation)",
              "KSUBW (Signed Subtraction with 32-bit Saturation)",
              "RADDW (32-bit Signed Halving Addition)",
              "RSUBW (32-bit Signed Halving Subtraction)",
              "MAXW (32-bit Signed Word Maximum)",
              "MINW (32-bit Signed Word Minimum)"],
     "operation":
         "int64_t tmp = (int64_t)a + b;\n"
         "res = Sat_s(tmp, 32);\n"
         "return res;\n",
     "operation@ksubw":
         "int64_t tmp = (int64_t)a - b;\n"
         "res = Sat_s(tmp, 32);\n"
         "return res;\n",
     "operation@raddw":
         "int64_t tmp = (int64_t)a + b;\n"
         "res = tmp >> 1;\n"
         "return res;\n",
     "operation@rsubw":
         "int64_t tmp = (int64_t)a - b;\n"
         "res = tmp >> 1;\n"
         "return res;\n",
     "operation@maxw":
         "res = (a > b) ? a : b;\n"
         "return res;\n",
     "operation@minw":
         "res = (a < b) ? a : b;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "int16x2 a", "int16x2 b"],
     "insn": ["khmbb", "khmbt", "khmtt"],
     "available": "both",
     "subclass": 21,
     "idoc": ["KHMBB (Signed Saturating Half Multiply B16 x B16)@a[0] * b[0]",
              "KHMBT (Signed Saturating Half Multiply B16 x T16)@a[0] * b[1]",
              "KHMTT (Signed Saturating Half Multiply T16 x T16)@a[1] * b[1]"],
     "operation":
         "res = Sat_s($idoc >> 15, 16);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uint32 a", "uint32 b"],
     "insn": ["ukaddh", "uksubh"],
     "available": "both",
     "subclass": 21,
     "idoc": ["UKADDH (Unsigned Addition with 16-bit Saturation)@+",
              "UKSUBH (Unsigned Subtraction with 16-bit Saturation)@-"],
     "operation":
         "int64_t tmp = (int64_t)a $idoc b;\n"
         "res = Sat_u(tmp, 16);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uint32 a", "uint32 b"],
     "insn": ["ukaddw", "uksubw", "uraddw", "ursubw"],
     "available": "both",
     "isubclass": [22, 22, 23, 23],
     "idoc": ["UKADDW (Unsigned Addition with 32-bit Saturation)",
              "UKSUBW (Unigned Subtraction with 32-bit Saturation)",
              "URADDW (32-bit Unsigned Halving Addition)",
              "URSUBW (32-bit Unsigned Halving Subtraction)"],
     "operation":
         "int64_t tmp = (int64_t)a + b;\n"
         "res = Sat_u(tmp, 32);\n"
         "return res;\n",
     "operation@uksubw":
         "int64_t tmp = (int64_t)a - b;\n"
         "res = Sat_u(tmp, 32);\n"
         "return res;\n",
     "operation@uraddw":
         "int64_t tmp = (int64_t)a + b;\n"
         "res = tmp >> 1;\n"
         "return res;\n",
     "operation@ursubw":
         "int64_t tmp = (int64_t)a - b;\n"
         "res = tmp >> 1;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32", "int16x2 a", "int16x2 b"],
     "insn": ["kdmbb", "kdmbt", "kdmtt"],
     "available": "both",
     "subclass": 22,
     "idoc": ["KDMBB (Signed Saturating Double Multiply B16 x B16)@(int64_t)a[0] * b[0]",
              "KDMBT (Signed Saturating Double Multiply B16 x T16)@(int64_t)a[0] * b[1]",
              "KDMTT (Signed Saturating Double Multiply T16 x T16)@(int64_t)a[1] * b[1]"],
     "operation":
         "res = Sat_s($idoc << 1, 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32", "int32 t", "int16x2 a", "int16x2 b"],
     "insn": ["kdmabb", "kdmabt", "kdmatt"],
     "available": "both",
     "subclass": 22,
     "idoc": ["KDMABB (Signed Saturating Double Multiply Addition B16 x B16)@(int64_t)a[0] * b[0]",
              "KDMABT (Signed Saturating Double Multiply Addition B16 x T16)@(int64_t)a[0] * b[1]",
              "KDMATT (Signed Saturating Double Multiply Addition T16 x T16)@(int64_t)a[1] * b[1]"],
     "operation":
         "int64_t tmp = Sat_s($idoc << 1, 32);\n"
         "res = Sat_s((int32_t)t + tmp, 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "int32 a", "bitv b"],
     "insn": ["kslraw", "kslraw.u"],
     "available": "both",
     "subclass": 22,
     "idoc": ["KSLRAW (Shift Left Logical with 32-bit Saturation or Shift Right Arithmetic)",
              "KSLRAW.u (Shift Left Logical with 32-bit Saturation or Rounding Shift Right Arithmetic)"],
     "operation":
         "sa = b & 0x1f;\n"
         "if ((b & 0x20) != 0) {\n"
         "  sa = (sa == 0) ? 31 : ((sa ^ 0x1f) + 1);\n"
         "  res = (int64_t)a >> sa;\n"
         "}\n"
         "else {\n"
         "  res = Sat_s((int64_t)a << sa, 32);\n"
         "}\n"
         "return res;\n",
     "operation@kslraw.u":
         "sa = b & 0x1f;\n"
         "if ((b & 0x20) != 0) {\n"
         "  sa = (sa == 0) ? 31 : ((sa ^ 0x1f) + 1);\n"
         "  res = Round((int64_t)a, sa);\n"
         "}\n"
         "else {\n"
         "  res = Sat_s((int64_t)a << sa, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32", "int32 a", "uimm5 b"],
     "insn": ["ksllw"],
     "available": "both",
     "subclass": 22,
     "idoc": ["KSLLW (Saturating Shift Left Logical for Word)"],
     "operation":
         "sa = b & 0x1f;\n"
         "  res = Sat_s((int64_t)(int32_t)a << sa, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32", "int32 a", "const uimm5 b"],
     "insn": ["kslliw"],
     "available": "both",
     "subclass": 22,
     "idoc": ["KSLLIW (Saturating Shift Left Logical Immediate for Word)"],
     "operation":
         "sa = b & 0x1f;\n"
         "  res = Sat_s((int64_t)(int32_t)a << sa, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32", "int32 a"],
     "insn": ["kabsw"],
     "available": "both",
     "subclass": 22,
     "idoc": ["KABSW (Scalar 32-bit Absolute Value with Saturation)"],
     "operation":
         "res = Abs((int64_t)a);\n"
         "res = Sat_s(res, 32);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint64", "uint32 a", "uint32 b"],
     "insn": ["mulr64"],
     "available": "both",
     "subclass": 23,
     "Zpsf": True,
     "idoc": ["MULR64 (Multiply Word Unsigned to 64-bit Data)"],
     "operation":
         "res = (uint64_t)a * b;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int32 a", "int32 b"],
     "insn": ["mulsr64"],
     "available": "both",
     "subclass": 23,
     "Zpsf": True,
     "idoc": ["MULSR64 (Multiply Word Signed to 64-bit Data)"],
     "operation":
         "res = (int64_t)a * b;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "integer a", "integer b"],
     "insn": ["ave"],
     "available": "both",
     "subclass": 25,
     "idoc": ["AVE (Average with Rounding)"],
     "operation":
         "res = (int128)a + b + 1;\n"
         "res = res >> 1;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "integer a", "uimmw b"],
     "insn": ["sra.u"],
     "available": "both",
     "subclass": 25,
     "idoc": ["SRA.u (Rounding Shift Right Arithmetic)@"],
     "operation":
         "RV32: sa = b & 0x1f;\n"
         "RV64: sa = b & 0x3f;\n"
         "res = Round(a, sa);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "integer a", "const uimmw b"],
     "insn": ["srai.u"],
     "available": "both",
     "subclass": 25,
     "idoc": ["SRAI.u (Rounding Shift Right Arithmetic Immediate)@"],
     "operation":
         "RV32: sa = b & 0x1f;\n"
         "RV64: sa = b & 0x3f;\n"
         "res = Round(a, sa);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uinteger a", "uimmw msb"],
     "insn": ["bitrev"],
     "available": "both",
     "subclass": 25,
     "idoc": ["BITREV (Bit Reverse)"],
     "operation":
         "RV32: msb &= 0x1f;\n"
         "RV64: msb &= 0x3f;\n"
         "res = 0;\n"
         "for (x = 0; x <= msb; x++)\n"
         "  res |= ((a >> (msb-x)) & 1) << x;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uinteger a", "const uimmw msb"],
     "insn": ["bitrevi"],
     "available": "both",
     "subclass": 25,
     "idoc": ["BITREVI (Bit Reverse Immediate)"],
     "operation":
         "RV32: msb &= 0x1f;\n"
         "RV64: msb &= 0x3f;\n"
         "res = 0;\n"
         "for (x = 0; x <= msb; x++)\n"
         "  res &= ((a >> (msb-x)) & 1) << x;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uinteger t", "uinteger a", "const uimmwbyte bpos"],
     "insn": ["insb"],
     "available": "both",
     "subclass": 25,
     "idoc": ["INSB (Insert Byte)"],
     "operation":
         "RV32: bpos &= 0x3;\n"
         "RV64: bpos &= 0x7;\n"
         "res = t & ~(0xff << 8*bpos) | ((uint8_t)a << 8*bpos)\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uinteger a", "uinteger b", "uinteger c"],
     "insn": ["bpick"],
     "available": "both",
     "subclass": 25,
     "idoc": ["BPICK (Bit-wise Pick)"],
     "operation":
         "res = (~c | a) & (c | b);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32", "int32 t", "int32 a", "int32 b"],
     "insn": ["maddr32", "msubr32"],
     "available": "both",
     "subclass": 25,
     ""    "idoc": ["MADDR32 (Multiply and Add to 32-Bit Word)@+",
                    "MSUBR32 (Multiply and Subtract from 32-Bit Word)@-"],
     "operation":
         "t = t $idoc a * b;\n"
         "res = t;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "uint64 a", "uimm5 b"],
     "insn": ["wext"],
     "subclass": 25,
     "available": "both",
     "Zpsf" : True,
     "idoc": ["WEXT (Extract Word from 64-bit)"],
     "operation":
         "lsb = b & 0x1f;\n"
         "a = a << (32 - lsb);\n"
         "res = (int64_t)a >> 32;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["integer", "uint64 a", "const uimm5 b"],
     "insn": ["wexti"],
     "available": "both",
     "Zpsf" : True,
     "subclass": 25,
     "idoc": ["WEXTI (Extract Word from 64-bit Immediate)"],
     "operation":
         "lsb = b & 0x1f;\n"
         "a = a << (32 - lsb);\n"
         "res = (int64_t)a >> 32;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "void"],
     "insn": ["rdov"],
     "available": "both",
     "subclass": 24,
     "idoc": ["RDOV (Read OV flag)"],
     "operation":
         "return vxsat.OV;\n"
     },

    {"name": "$insn",
     "prototype": ["void", "void"],
     "insn": ["clrov"],
     "available": "both",
     "subclass": 24,
     "idoc": ["CLROV (Clear vsat.OV flag)"],
     "operation":
         "vxsat.OV = 0;\n"
     }
]

builtins_64bit_profile = [
    {"name": "$insn",
     "prototype": ["?int64", "?int64 a", "?int64 b"],
     "insn": ["add64", "sub64"],
     "available": "both",
     "subclass": 18,
     "Zpsf": True,
     "idoc": ["ADD64 (64-bit Addition)@+",
              "SUB64 (64-bit Subtraction)@-"],
     "operation":
         "res = a $idoc b;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int64 a", "int64 b"],
     "insn": ["kadd64", "ksub64", "radd64", "rsub64"],
     "available": "both",
     "subclass": 18,
     "Zpsf": True,
     "idoc": ["KADD64 (64-bit Signed Saturating Addition)@Sat_s((int128)a + b, 64)",
              "KSUB64 (64-bit Signed Saturating Subtraction)@Sat_s((int128)a - b, 64)",
              "RADD64 (64-bit Signed Halving Addition)@((int128)a + b) >> 1",
              "RSUB64 (64-bit Signed Halving Subtraction)@((int128)a - b) >> 1"],
     "operation":
         "res = $idoc;\n"
         "return res;\n"

     },

    {"name": "$insn",
     "prototype": ["uint64", "uint64 a", "uint64 b"],
     "insn": ["ukadd64", "uksub64", "uradd64", "ursub64"],
     "available": "both",
     "subclass": 18,
     "Zpsf": True,
     "idoc": ["UKADD64 (64-bit Unsigned Saturating Addition)@Sat_s((uint128)a + b, 64)",
              "UKSUB64 (64-bit Unsigned Saturating Subtraction)@Sat_s((uint128)a - b, 64)",
              "URADD64 (64-bit Unsigned Halving Addition)@((uint128)a + b) >> 1",
              "URSUB64 (64-bit Unsigned Halving Subtraction)@((uint128)a - b) >> 1"],
     "operation":
         "res = $idoc;\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int64 t", "int32xN a", "int32xN b"],
     "insn": ["smar64", "smsr64", "kmar64", "kmsr64"],
     "available": "both",
     "subclass": 19,
     "Zpsf": True,
     "idoc": ["SMAR64 (Signed Multiply and Add to 64-Bit Data)",
              "SMSR64 (Signed Multiply and Subtract from 64-Bit Data)",
              "KMAR64 (Signed Multiply and Saturating Add to 64-Bit Data)",
              "KMSR64 (Signed Multiply and Saturating Subtract from 64-Bit Data)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += (int64_t)a[i] * b[i];\n"
         "}\n"
         "return res;\n",
     "operation@smsr64":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res -= (int64_t)a[i] * b[i];\n"
         "}\n"
         "return res;\n",
     "operation@kmar64":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "int128 tmp = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  tmp += (int64_t)a[i] * b[i];\n"
         "}\n"
         "res = Sat_s(tmp, 64)"
         "return res;\n",
     "operation@kmsr64":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "int128 tmp = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  tmp -= (int64_t)a[i] * b[i];\n"
         "}\n"
         "res = Sat_s(tmp, 64)"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint64", "uint64 t", "uint32xN a", "uint32xN b"],
     "insn": ["ukmar64", "ukmsr64", "umar64", "umsr64"],
     "available": "both",
     "subclass": 19,
     "Zpsf": True,
     "idoc": ["UKMAR64 (Unsigned Multiply and Saturating Add to 64-Bit Data)",
               "UKMSR64 (Unsigned Multiply and Saturating Subtract from 64-Bit Data)",
               "UMAR64 (Unsigned Multiply and Add to 64-Bit Data)",
               "UMSR64 (Unsigned Multiply and Subtract from 64-Bit Data)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "int128 tmp = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  tmp += (uint64_t)a[i] * b[i];\n"
         "}\n"
         "res = Sat_u(tmp, 64);\n"
         "return res;\n",
     "operation@ukmsr64":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "int128 tmp = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res -= (uint64_t)a[i] * b[i];\n"
         "}\n"
         "res = Sat_u(tmp, 64);\n"
         "return res;\n",
     "operation@umar64":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += (uint64_t)a[i] * b[i];\n"
         "}\n"
         "return res;\n",
     "operation@umsr64":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res -= (uint64_t)a[i] * b[i];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int64 t", "int16xN a", "int16xN b"],
     "insn": ["smalbb", "smalbt", "smaltt"],
     "available": "both",
     "subclass": 20,
     "Zpsf": True,
     "idoc": ["SMALBB (Signed Multiply Bottom Halfs & Add 64-bit)@a[2*x] * b[2*x]",
              "SMALBT (Signed Multiply Bottom Half & Top Half & Add 64-bit)@a[2*x] * b[2*x+1]",
              "SMALTT (Signed Multiply Top Halfs & Add 64-bit)@a[2*x+1] * b[2*x+1]"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += $idoc;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int64 t", "int16xN a", "int16xN b"],
     "insn": ["smalda", "smalxda"],
     "available": "both",
     "subclass": 20,
     "Zpsf": True,
     "idoc": ["SMALDA (Signed Multiply Two Halfs and Two Adds 64-bit)",
               "SMALXDA (Signed Crossed Multiply Two Halfs and Two Adds 64-bit)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += a[x] * b[x];\n"
         "}\n"
         "return res;\n",
     "operation@smalxda":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += a[2*x] * b[2*x+1];\n"
         "  res += a[2*x+1] * b[2*x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int64 t", "int16xN a", "int16xN b"],
     "insn": ["smalds", "smaldrs", "smalxds"],
     "available": "both",
     "subclass": 20,
     "Zpsf": True,
     "idoc": ["SMALDS (Signed Multiply Two Halfs & Subtract & Add 64-bit)",
              "SMALDRS (Signed Multiply Two Halfs & Reverse Subtract & Add 64-bit)",
              "SMALXDS (Signed Crossed Multiply Two Halfs & Subtract & Add 64-bit)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += a[2*x+1] * b[2*x+1];\n"
         "  res -= a[2*x] * b[2*x];\n"
         "}\n"
         "return res;\n",
     "operation@smaldrs":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res -= a[2*x+1] * b[2*x+1];\n"
         "  res += a[2*x] * b[2*x];\n"
         "}\n"
         "return res;\n",
     "operation@smalxds":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += a[2*x+1] * b[2*x];\n"
         "  res -= a[2*x] * b[2*x+1];\n"
         "}\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int64 t", "int16xN a", "int16xN b"],
     "insn": ["smslda", "smslxda"],
     "available": "both",
     "subclass": 20,
     "Zpsf": True,
     "idoc": ["SMSLDA (Signed Multiply Two Halfs and ubtract 64-bit)",
              "SMSLXDA (Signed Crossed Multiply Two Halfs and Subtract 64-bit)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res -= a[x] * b[x];\n"
         "}\n"
         "return res;\n",
     "operation@smslxda":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res -= a[2*x] * b[2*x+1];\n"
         "  res -= a[2*x+1] * b[2*x];\n"
         "}\n"
         "return res;\n"
     }
]

buitins_partial_simd_data = [
    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a", "uint16xN b"],
     "insn": ["pkbb16", "pkbt16", "pktb16", "pktt16"],
     "available": "both",
     "subclass": 11,
     "idoc": ["PKBB16 (Pack Two 16-bit Data from Both Bottom Half)@a[2*x], b[2*x]",
              "PKBT16 (Pack Two 16-bit Data from Bottom and Top Half)@a[2*x], b[2*x+1]",
              "PKTB16 (Pack Two 16-bit Data from Top and Bottom Half)@a[2*x+1], b[2*x]",
              "PKTT16 (Pack Two 16-bit Data from Both Top Half)@a[2*x+1], b[2*x+1]"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++)\n"
         "  res[x] = Concat($idoc);\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "int32xN b"],
     "insn": ["smmul", "smmul.u"],
     "available": "both",
     "subclass": 12,
     "idoc": ["SMMUL (SIMD MSW Signed Multiply Word)",
              "SMMUL.u (SIMD MSW Signed Multiply Word with Rounding)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++)\n"
         "  res[x] = ((int64_t)a[x] * b[x]) >> 32;\n"
         "return res;\n",
     "operation@smmul.u":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++)\n"
         "  res[x] = Round((a[x] * b[x]), 32);\n"
         "return res;\n",
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "int32xN b"],
     "insn": ["kwmmul", "kwmmul.u"],
     "available": "both",
     "subclass": 12,
     "idoc": ["KWMMUL (SIMD Saturating MSW Signed Multiply Word & Double)@tmp >> 31",
              "KWMMUL.u (SIMD Saturating MSW Signed Multiply Word & Double with Rounding)@Round(tmp, 31)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[x];\n"
         "  tmp = $idoc;\n"
         "  res[x] = Sat_s(tmp, 32);\n"
         "}\n"
         "return res;\n",
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int32xN a", "int32xN b"],
     "insn": ["kmmac", "kmmac.u", "kmmsb", "kmmsb.u"],
     "available": "both",
     "subclass": 12,
     "idoc": ["KMMAC (SIMD Saturating MSW Signed Multiply Word and Add)@t[x] + (tmp >> 32)",
              "KMMAC.u (SIMD Saturating MSW Signed Multiply Word and Add with Rounding)@t[x] + Round(tmp, 32)",
              "KMMSB (SIMD Saturating MSW Signed Multiply Word and Subtract)@t[x] - (tmp >> 32)",
              "KMMSB.u (SIMD Saturating MSW Signed Multiply Word and Subtraction with Rounding)@t[x] - Round(tmp, 32)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[x];\n"
         "  tmp = $idoc;\n"
         "  res[x] = Sat_s(tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "int16xN b"],
     "insn": ["smmwb", "smmwb.u", "smmwt", "smmwt.u"],
     "available": "both",
     "subclass": 13,
     "idoc": ["SMMWB (SIMD MSW Signed Multiply Word and Bottom Half)",
              "SMMWB.u (SIMD MSW Signed Multiply Word and Bottom Half with Rounding)",
              "SMMWT (SIMD MSW Signed Multiply Word and Top Half)",
              "SMMWT.u (SIMD MSW Signed Multiply Word and Top Half with Rounding)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x];\n"
         "  res[x] = tmp >> 16;\n"
         "}\n"
         "return res;\n",
     "operation@smmwb.u":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x];\n"
         "  res[x] = Round(tmp, 16);\n"
         "}\n"
         "return res;\n",
     "operation@smmwt":
         "RV32 : Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x+1];\n"
         "  res[x] = tmp >> 16;\n"
         "}\n"
         "return res;\n",
     "operation@smmwt.u":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x+1];\n"
         "  res[x] = Round(tmp, 16);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "int16xN b"],
     "insn": ["kmmwb2", "kmmwb2.u"],
     "available": "both",
     "subclass": 13,
     "idoc": ["KMMWB2 (SIMD Saturating MSW Signed Multiply Word and Bottom Half & 2)@tmp >> 15",
              "KMMWB2.u (SIMD Saturating MSW Signed Multiply Word and Bottom Half & 2 with Rounding)@Round(tmp, 15)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x];\n"
         "  tmp = $idoc;\n"
         "  res[x] = Sat_s(tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "int16xN b"],
     "insn": ["kmmwt2", "kmmwt2.u"],
     "available": "both",
     "subclass": 13,
     "idoc": ["KMMWT2 (SIMD Saturating MSW Signed Multiply Word and Top Half & 2)@tmp >> 15",
              "KMMWT2.u (SIMD Saturating MSW Signed Multiply Word and Top Half & 2 with Rounding)@Round(tmp, 15)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x+1];\n"
         "  tmp = $idoc;\n"
         "  res[x] = Sat_s(tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int32xN a", "int16xN b"],
     "insn": ["kmmawb", "kmmawb.u", "kmmawb2", "kmmawb2.u"],
     "available": "both",
     "subclass": 13,
     "idoc": ["KMMAWB (SIMD Saturating MSW Signed Multiply Word and Bottom Half and Add)@tmp >> 16",
              "KMMAWB.u (SIMD Saturating MSW Signed Multiply Word and Bottom Half and Add with Rounding)@Round(tmp, 16)",
              "KMMAWB2 (SIMD Saturating MSW Signed Multiply Word and Bottom Half & 2 and Add)@tmp >> 15",
              "KMMAWB2.u (SIMD Saturating MSW Signed Multiply Word and Bottom Half & 2 and Add with Rounding)@Round(tmp, 15)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x];\n"
         "  tmp = t[x] + $idoc;\n"
         "  res[x] = Sat_s(tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int32xN a", "int16xN b"],
     "insn": ["kmmawt", "kmmawt.u", "kmmawt2", "kmmawt2.u"],
     "available": "both",
     "subclass": 13,
     "idoc": ["KMMAWT (SIMD Saturating MSW Signed Multiply Word and Top Half and Add)@tmp >> 16",
              "KMMAWT.u (SIMD Saturating MSW Signed Multiply Word and Top Half and Add with Rounding)@Round(tmp, 16)",
              "KMMAWT2 (SIMD Saturating MSW Signed Multiply Word and TopHalf & 2 and Add)@tmp >> 15",
              "KMMAWT2.u (SIMD Saturating MSW Signed Multiply Word and Top Half & 2 and Add with Rounding)@Round(tmp, 15)"],
     "operation":
         "RV32 : Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = (int64_t)a[x] * b[2*x+1];\n"
         "  tmp = t[x] + $idoc;\n"
         "  res[x] = Sat_s(tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int16xN a", "int16xN b"],
     "insn": ["smbb16", "smbt16", "smtt16", "smds", "smdrs", "smxds"],
     "available": "both",
     "subclass": 14,
     "idoc": ["SMBB16 (SIMD Signed Multiply Bottom Half & Bottom Half)@a[2*x] * b[2*x]",
              "SMBT16 (SIMD Signed Multiply Bottom Half & Top Half)@a[2*x] * b[2*x+1]",
              "SMTT16 (SIMD Signed Multiply Top Half & Top Half)@a[2*x+1] * b[2*x+1]",
              "SMDS (SIMD Signed Multiply Two Halfs and Subtract)@(a[2*x+1] * b[2*x+1] - a[2*x] * b[2*x])",
              "SMDRS (SIMD Signed Multiply Two Halfs and Reverse Subtract)@(a[2*x] * b[2*x] - a[2*x+1] * b[2*x+1])",
              "SMXDS (SIMD Signed Crossed Multiply Two Halfs and Subtract)@(a[2*x+1] * b[2*x] - a[2*x] * b[2*x+1])"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = $idoc;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int16xN a", "int16xN b"],
     "insn": ["kmda", "kmxda"],
     "available": "both",
     "subclass": 14,
     "idoc": ["KMDA (SIMD Signed Multiply Two Halfs and Add)@(int64_t)a[2*x+1] * b[2*x+1] + (int64_t)a[2*x] * b[2*x]",
              "KMXDA (SIMD Signed Crossed Multiply Two Halfs and Add)@(int64_t)a[2*x+1] * b[2*x] + (int64_t)a[2*x] * b[2*x+1]"],
     "operation":
         "RV32 : Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = $idoc\n"
         "  res[x] = Sat_s(tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int16xN a", "int16xN b"],
     "insn": ["kmabb", "kmabt", "kmatt", "kmada", "kmaxda", "kmads", "kmadrs", "kmaxds"],
     "available": "both",
     "subclass": 14,
     "idoc": ["KMABB (SIMD Saturating Signed Multiply Bottom Halfs & Add)@(int64_t)a[2*x] * b[2*x]",
              "KMABT (SIMD Saturating Signed Multiply Bottom & Top Halfs & Add)@(int64_t)a[2*x] * b[2*x+1]",
              "KMATT (SIMD Saturating Signed Multiply Top Halfs & Add)@(int64_t)a[2*x+1] * b[2*x+1]",
              "KMADA (SIMD Saturating Signed Multiply Two Halfs and Two Adds)@(int64_t)a[2*x+1] * b[2*x+1] + (int64_t)a[2*x] * b[2*x]",
              "KMAXDA (SIMD Saturating Signed Crossed Multiply Two Halfs and Two Adds)@(int64_t)a[2*x+1] * b[2*x] + (int64_t)a[2*x] * b[2*x+1]",
              "KMADS (SIMD Saturating Signed Multiply Two Halfs & Subtract & Add)@(int64_t)a[2*x+1] * b[2*x+1] - (int64_t)a[2*x] * b[2*x]",
              "KMADRS (SIMD Saturating Signed Multiply Two Halfs & Reverse Subtract & Add)@(int64_t)a[2*x] * b[2*x] - (int64_t)a[2*x+1] * b[2*x+1]",
              "KMAXDS (SIMD Saturating Signed Crossed Multiply Two Halfs & Subtract & Add)@(int64_t)a[2*x+1] * b[2*x] - (int64_t)a[2*x] * b[2*x+1]"],
     "operation":
         "RV32 : Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = $idoc;\n"
         "  res[x] = Sat_s(t[x] + tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int16xN a", "int16xN b"],
     "insn": ["kmsda", "kmsxda"],
     "available": "both",
     "subclass": 14,
     "idoc": ["KMSDA (SIMD Saturating Signed Multiply Two Halfs & Add & Subtract)@(int64_t)a[2*x+1] * b[2*x+1] + (int64_t)a[2*x] * b[2*x]",
              "KMSXDA (SIMD Saturating Signed Crossed Multiply Two Halfs & Add & Subtract)@(int64_t)a[2*x+1] * b[2*x] + (int64_t)a[2*x] * b[2*x+1]"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  int64_t tmp = $idoc\n"
         "  res[x] = Sat_s(t[x] - tmp, 32);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int64", "int64 a", "int16xN b"],
     "insn": ["smal"],
     "available": "both",
     "subclass": 15,
     "Zpsf": True,
     "idoc": ["SMAL (Signed Multiply Halfs & Add 64-bit)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "res = a;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += b[2*x+1] * b[2*x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN a", "const uimm5 b"],
     "insn": ["sclip32"],
     "available": "both",
     "subclass": 16,
     "idoc": ["SCLIP32 (SIMD 32-bit Signed Clip Value)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "imm5u = b & 0x1f;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x], imm5u + 1);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "int32xN a", "const uimm5 b"],
     "insn": ["uclip32"],
     "available": "both",
     "subclass": 16,
     "idoc": ["UCLIP32 (SIMD 32-bit Unsigned Clip Signed Value)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "imm5u = b & 0x1f;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_u(a[x], imm5u);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "int32xN a"],
     "insn": ["clrs32"],
     "available": "both",
     "subclass": 16,
     "idoc": ["CLRS32 (SIMD 32-bit Count Leading Redundant Sign)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  sign = (uint32_t)a[x] >> 31;\n"
         "  res[x] = 0;\n"
         "  for (i = 30; i>= 0; i--) {\n"
         "    if ((((uint32_t)a[x] >> i) & 1) == sign)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n"
     },

     {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN a"],
     "insn": ["clo32", "clz32"],
     "available": "both",
     "subclass": 16,
     "idoc": ["CLO32 (SIMD 32-bit Count Leading One)",
              "CLZ32 (SIMD 32-bit Count Leading Zero)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = 0;\n"
         "  for (i = 31; i>= 0; i--) {\n"
         "    if ((((uint32_t)a[x] >> i) & 1) == 1)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n",
     "operation@clz32":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = 0;\n"
         "  for (i = 31; i>= 0; i--) {\n"
         "    if ((((uint32_t)a[x] >> i) & 1) == 0)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uint8xN a", "uint8xN b"],
     "insn": ["pbsad"],
     "available": "both",
     "subclass": 16,
     "idoc": ["PBSAD (Parallel Byte Sum of Absolute Difference)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "res = 0;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += Abs(a[x] - b[x]);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uinteger", "uinteger t", "uint8xN a", "uint8xN b"],
     "insn": ["pbsada"],
     "available": "both",
     "subclass": 16,
     "idoc": ["PBSADA (Parallel Byte Sum of Absolute Difference Accum)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "res = t;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res += Abs(a[x] - b[x]);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int8xN a", "int8xN b"],
     "insn": ["smaqa"],
     "available": "both",
     "subclass": 17,
     "idoc": ["SMAQA (Signed Multiply Four Bytes with 32-bit Adds)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = t[x] + a[4*x] * b[4*x] + a[4*x+1] * b[4*x+1] +\n"
         "           a[4*x+2] * b[4*x+2] + a[4*x+3] * b[4x+3];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32xN", "int32xN t", "int8xN a", "uint8xN b"],
     "insn": ["smaqa.su"],
     "available": "both",
     "subclass": 17,
     "idoc": ["SMAQA.SU (Signed and Unsigned Multiply Four Bytes with 32-bit Adds)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = t[x] + a[4*x] * b[4*x] + a[4*x+1] * b[4*x+1] +\n"
         "           a[4*x+2] * b[4x+2] + a[4*x+3] * b[4*x+3];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32xN", "uint32xN t", "uint8xN a", "uint8xN b"],
     "insn": ["umaqa"],
     "available": "both",
     "subclass": 17,
     "idoc": ["UMAQA (Unsigned Multiply Four Bytes with 32-bit Adds)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = t[x] + a[4*x] * b[4*x] + a[4*x+1] * b[4*x+1] +\n"
         "           a[4*x+2] * b[4x+2] + a[4*x+3] * b[4*x+3];\n"
         "}\n"
         "return res;\n"
     },
]

buitins_simd_data = [
    {"name": "$insn",
     "prototype": ["?int8xN", "?int8xN a", "?int8xN b"],
     "insn": ["add8", "sub8"],
     "available": "both",
     "subclass": 1,
     "idoc": ["ADD8 (SIMD 8-bit Addition)@+",
              "SUB8 (SIMD 8-bit Subtraction)@-",
              ],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] $idoc b[x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "?int8xN a", "?int8xN b"],
     "insn": ["cmpeq8"],
     "available": "both",
     "subclass": 5,
     "idoc": [
         "CMPEQ8 (SIMD 8-bit Integer Compare Equal)",
     ],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] == b[x]) ? 0xff : 0;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["?int16xN", "?int16xN a", "?int16xN b"],
     "insn": ["add16", "sub16", "cras16", "crsa16", "stas16", "stsa16"],
     "available": "both",
     "subclass": 0,
     "idoc": ["ADD16 (SIMD 16-bit Addition)@+",
               "SUB16 (SIMD 16-bit Subtraction)@-",
              "CRAS16 (SIMD 16-bit Cross Addition & Subtraction)",
              "CRSA16 (SIMD 16-bit Cross Subtraction & Addition)",
              "STAS16 (SIMD 16-bit Straight Addition & Subtraction)",
              "STSA16 (SIMD 16-bit Straight Subtraction & Addition)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] $idoc b[x];\n"
         "}\n"
         "return res;\n",
     "operation@cras16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[2*x+1] + b[2*x];\n"
         "  res[2*x] = a[2*x] - b[2*x+1];\n"
         "}\n"
         "return res;\n",
     "operation@crsa16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[2*x+1] - b[2*x];\n"
         "  res[2*x] = a[2*x] + b[2*x+1];\n"
         "}\n"
         "return res;\n",
     "operation@stas16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[2*x+1] + b[2*x+1];\n"
         "  res[2*x] = a[2*x] - b[2*x];\n"
         "}\n"
         "return res;\n",
     "operation@stsa16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[2*x+1] - b[2*x+1];\n"
         "  res[2*x] = a[2*x] + b[2*x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "?int16xN a", "?int16xN b"],
     "insn": ["cmpeq16"],
     "available": "both",
     "subclass": 4,
     "idoc": ["CMPEQ16 (SIMD 16-bit Integer Compare Equal)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] == b[x]) ? 0xffff : 0;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a", "int16xN b"],
     "insn": ["radd16", "rcras16", "rcrsa16", "rstas16", "rstsa16", "rsub16", "kadd16",
              "kcras16", "kcrsa16", "ksub16", "kstas16", "kstsa16"],
     "available": "both",
     "subclass": 0,
     "idoc": ["RADD16 (SIMD 16-bit Signed Halving Addition)@",
              "RCRAS16 (SIMD 16-bit Signed Halving Cross Addition & Subtraction)@",
              "RCRSA16 (SIMD 16-bit Signed Halving Cross Subtraction & Addition)@",
              "RSTAS16 (SIMD 16-bit Signed Halving Straight Addition & Subtraction)@",
              "RSTSA16 (SIMD 16-bit Signed Halving Straight Subtraction & Addition)@",
              "RSUB16 (SIMD 16-bit Signed Halving Subtraction)@",
              "KADD16 (SIMD 16-bit Signed Saturating Addition)",
              "KCRAS16 (SIMD 16-bit Signed Saturating Cross Addition & Subtraction)",
              "KCRSA16 (SIMD 16-bit Signed Saturating Cross Subtraction & Addition)",
              "KSUB16 (SIMD 16-bit Signed Saturating Subtraction)@",
              "KSTAS16 (SIMD 16-bit Signed Saturating Straight Addition & Subtraction)@",
              "KSTSA16 (SIMD 16-bit Signed Saturating Straight Subtraction & Addition)@"
              ],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] + b[x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@rcras16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] + b[2*x]) >> 1;\n"
         "  res[2*x] = (a[2*x] - b[2*x+1]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@rcrsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] - b[2*x]) >> 1;\n"
         "  res[2*x] = (a[2*x] + b[2*x+1]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@rstas16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] + b[2*x+1]) >> 1;\n"
         "  res[2*x] = (a[2*x] - b[2*x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@rstsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] - b[2*x+1]) >> 1;\n"
         "  res[2*x] = (a[2*x] - b[2*x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@rsub16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] - b[x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@kadd16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x] + b[x], 16);\n"
         "}\n"
         "return res;\n",
     "operation@kcras16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_s(a[2*x+1] + b[2*x], 16);\n"
         "  res[2*x] = Sat_s(a[2*x] - b[2*x+1], 16);\n"
         "}\n"
         "return res;\n",
     "operation@kcrsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_s(a[2*x+1] - b[2*x], 16);\n"
         "  res[2*x] = Sat_s(a[2*x] + b[2*x+1], 16);\n"
         "}\n"
         "return res;\n",
     "operation@ksub16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x] - b[x], 16);\n"
         "}\n"
         "return res;\n",
     "operation@kstas16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_s(a[2*x+1] + b[2*x+1], 16);\n"
         "  res[2*x] = Sat_s(a[2*x] - b[2*x], 16);\n"
         "}\n"
         "return res;\n",
     "operation@kstsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_s(a[2*x+1] - b[2*x+1], 16);\n"
         "  res[2*x] = Sat_s(a[2*x] + b[2*x], 16);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a", "int8xN b"],
     "insn": ["radd8", "rsub8", "kadd8", "ksub8"],
     "available": "both",
     "subclass":1,
     "idoc": ["RADD8 (SIMD 8-bit Signed Halving Addition)@a[x] + b[x]",
              "RSUB8 (SIMD 8-bit Signed Halving Subtraction)@a[x] - b[x]",
              "KADD8 (SIMD 8-bit Signed Saturating Addition)@Sat_s(a[x] + b[x], 8)",
              "KSUB8 (SIMD 8-bit Signed Saturating Subtraction)@Sat_s(a[x] - b[x], 8)",
              ],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = $idoc;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "int16xN a", "int16xN b"],
     "insn": ["scmplt16", "scmple16"],
     "available": "both",
     "subclass": 4,
     "idoc": [
         "SCMPLT16 (SIMD 16-bit Signed Compare Less Than)@<",
         "SCMPLE16 (SIMD 16-bit Signed Compare Less Than & Equal)@<="
     ],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] $idoc b[x]) ? 0xffff : 0;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "int8xN a", "int8xN b"],
     "insn": ["scmplt8", "scmple8"],
     "available": "both",
     "subclass": 5,
     "idoc": [
         "SCMPLT8 (SIMD 8-bit Signed Compare Less Than)@<",
         "SCMPLE8 (SIMD 8-bit Signed Compare Less Than & Equal)@<="
     ],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] $idoc b[x]) ? 0xffff : 0;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a", "int16xN b"],
     "insn": ["khm16", "khmx16"],
     "subclass": 6,
     "available": "both",
     "idoc": ["KHM16 (SIMD Signed Saturating 16-bit Multiply)",
              "KHMX16 (SIMD Signed Saturating Crossed 16-bit Multiply)",
              ],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s((a[x] * b[x]) >> 15, 16);\n"
         "}\n"
         "return res;\n",
     "operation@khmx16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x] = Sat_s((a[2*x] * b[2*x+1]) >> 15, 16);\n"
         "  res[2*x+1] = Sat_s((a[2*x+1] * b[2*x]) >> 15, 16);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a", "int8xN b"],
     "insn": ["khm8", "khmx8"],
     "subclass": 7,
     "available": "both",
     "idoc": ["KHM8 (SIMD Signed Saturating 8-bit Multiply)",
              "KHMX8 (SIMD Signed Saturating Crossed 8-bit Multiply)",
              ],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s((a[x] * b[x]) >> 7, 8);\n"
         "}\n"
         "return res;\n",
     "operation@khmx8":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x] = Sat_s((a[2*x] * b[2*x+1]) >> 7, 8);\n"
         "  res[2*x+1] = Sat_s((a[2*x+1] * b[2*x]) >> 7, 8);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a", "int16xN b"],
     "insn": ["smax16", "smin16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["SMAX16 (SIMD 8-bit Signed Minimum)@>",
              "SMIN16 (SIMD 16-bit Signed Minimum)@<"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] $idoc b[x]) ? a[x] : b[x];\n"
         "}\n"
         "return res;\n",
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a", "int8xN b"],
     "insn": ["smin8", "smax8"],
     "available": "both",
     "subclass": 9,
     "idoc": ["SMIN8 (SIMD 8-bit Signed Minimum)@<",
              "SMAX8 (SIMD 16-bit Signed Minimum)@>"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] $idoc b[x]) ? a[x] : b[x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a", "uint16xN b"],
     "insn": ["umax16", "umin16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["UMAX16 (SIMD 8-bit Unsigned Minimum)@>",
              "UMIN16 (SIMD 16-bit Unsigned Minimum)@<"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] $idoc b[x]) ? a[x] : b[x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a", "uint8xN b"],
     "insn": ["umin8", "umax8"],
     "subclass": 9,
     "available": "both",
     "idoc": ["UMIN8 (SIMD 8-bit Unsigned Minimum)@<",
              "UMAX8 (SIMD 16-bit Unsigned Minimum)@>"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] $idoc b[x]) ? a[x] : b[x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a", "uint16xN b"],
     "insn": ["uradd16", "ucmple16", "ucmplt16", "ukadd16", "ursub16", "urcras16", "urcrsa16", "urstas16",
              "urstsa16", "uksub16", "ukcras16", "ukcrsa16", "ukstas16", "ukstsa16"],
     "available": "both",
     "isubclass": [0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
     "idoc": ["URADD16 (SIMD 16-bit Unsigned Halving Addition)",
              "UCMPLE16 (SIMD 16-bit Unsigned Compare Less Than & Equal)",
              "UCMPLT16 (SIMD 16-bit Unsigned Compare Less Than)",
              "UKADD16 (SIMD 16-bit Unsigned Saturating Addition)",
              "URSUB16 (SIMD 16-bit Unsigned Halving Subtraction)",
              "URCRAS16 (SIMD 16-bit Unsigned Halving Cross Addition & Subtraction)",
              "URCRSA16 (SIMD 16-bit Unsigned Halving Cross Subtraction & Addition)",
              "URSTAS16 (SIMD 16-bit Unsigned Halving Straight Addition & Subtraction)",
              "URSTSA16 (SIMD 16-bit Unsigned Halving Straight Subtraction & Addition)",
              "UKSUB16 (SIMD 16-bit Unsigned Saturating Subtraction)",
              "UKCRAS16 (SIMD 16-bit Unsigned Saturating Cross Addition & Subtraction)",
              "UKCRSA16 (SIMD 16-bit Unsigned Saturating Cross Subtraction & Addition)",
              "UKSTAS16 (SIMD 16-bit Unsigned Saturating Straight Addition & Subtraction)",
              "UKSTSA16 (SIMD 16-bit Unsigned Saturating Straight Subtraction & Addition)"
              ],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] + b[x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@ucmple16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] <= b[x]) ? 0xffff : 0;\n"
         "}\n"
         "return res;\n",
     "operation@ucmplt16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] < b[x]) ? 0xffff : 0;\n"
         "}\n"
         "return res;\n",
     "operation@ukadd16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_u(a[x] + b[x], 16);\n"
         "}\n"
         "return res;\n",
     "operation@ursub16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] - b[x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@urcras16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] + b[2*x]) >> 1;\n"
         "  res[2*x] = (a[2*x] - b[2*x+1]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@urcrsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] - b[2*x]) >> 1;\n"
         "  res[2*x] = (a[2*x] + b[2*x+1]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@urstas16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] + b[2*x+1]) >> 1;\n"
         "  res[2*x] = (a[2*x] - b[2*x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@urstsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = (a[2*x+1] - b[2*x+1]) >> 1;\n"
         "  res[2*x] = (a[2*x] + b[2*x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@uksub16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_u(a[x] - b[x], 16);\n"
         "}\n"
         "return res;\n",
     "operation@ukcras16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_u(a[2*x+1] + b[2*x], 16);\n"
         "  res[2*x] = Sat_u(a[2*x] - b[2*x+1], 16);\n"
         "}\n"
         "return res;\n",
     "operation@ukcrsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_u(a[2*x+1] - b[2*x], 16);\n"
         "  res[2*x] = Sat_u(a[2*x] + b[2*x+1], 16);\n"
         "}\n"
         "return res;\n",
     "operation@ukstas16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_u(a[2*x+1] + b[2*x+1], 16);\n"
         "  res[2*x] = Sat_u(a[2*x] - b[2*x], 16);\n"
         "}\n"
         "return res;\n",
     "operation@ukstsa16":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = Sat_u(a[2*x+1] - b[2*x+1], 16);\n"
         "  res[2*x] = Sat_u(a[2*x] + b[2*x], 16);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a", "uint8xN b"],
     "insn": ["uradd8", "ukadd8", "ucmple8", "ursub8", "uksub8", "ucmplt8"],
     "available": "both",
     "isubclass": [1, 1, 5, 1, 1, 5],
     "idoc": ["URADD8 (SIMD 8-bit Unsigned Halving Addition)",
              "UKADD8 (SIMD 8-bit Unsigned Saturating Addition)",
              "UCMPLE8 (SIMD 8-bit Unsigned Compare Less Than & Equal)",
              "URSUB8 (SIMD 8-bit Unsigned Halving Subtraction)",
              "UKSUB8 (SIMD 8-bit Unsigned Saturating Subtraction)",
              "UCMPLT8 (SIMD 8-bit Unsigned Compare Less Than)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] + b[x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@ukadd8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_u(a[x] + b[x], 8);\n"
         "}\n"
         "return res;\n",
     "operation@ucmple8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] <= b[x]) > 0xff : 0;\n"
         "}\n"
         "return res;\n",
     "operation@ursub8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] - b[x]) >> 1;\n"
         "}\n"
         "return res;\n",
     "operation@uksub8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_u(a[x] - b[x], 8);\n"
         "}\n"
         "return res;\n"
        ,
     "operation@ucmplt8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = (a[x] < b[x]) > 0xff : 0;\n"
         "}\n"
         "return res;\n",
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a", "uimm4 b"],
     "insn": ["sra16", "sra16.u", "ksll16"],
     "available": "both",
     "subclass": 2,
     "idoc": ["SRA16 (SIMD 16-bit Shift Right Arithmetic)",
              "SRA16.u (SIMD 16-bit Rounding Shift Right Arithmetic)",
              "KSLL16 (SIMD 16-bit Saturating Shift Left Logical)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
     "operation@sra16.u":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n",
     "operation@ksll16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x] << sa, 16);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a", "bitv b"],
     "insn": ["kslra16", "kslra16.u"],
     "available": "both",
     "subclass": 2,
     "idoc": ["KSLRA16 (SIMD 16-bit Shift Left Logical with Saturation or Shift Right Arithmetic)",
              "KSLRA16.u (SIMD 16-bit Shift Left Logical with Saturation or Rounding Shift Right Arithmetic)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "if ((b & 0x10) != 0) {\n"
         "  sa = (sa == 0) ? 15 : (sa ^ 0xf) + 1;\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = a[x] >> sa;\n"
         "  }\n"
         "}\n"
         "else {\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = Sat_s(a[x] << sa, 16);\n"
         "  }\n"
         "}\n"
         "return res;\n",
     "operation@kslra16.u":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "if ((b & 0x10) != 0) {\n"
         "  sa = (sa == 0) ? 15 : (sa ^ 0xf) + 1;\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = Round(a[x], sa);\n"
         "  }\n"
         "}\n"
         "else {\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = Sat_s(a[x] << sa, 16);\n"
         "  }\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a", "const uimm4 b"],
     "insn": ["srai16", "srai16.u", "kslli16"],
     "available": "both",
     "subclass": 2,
     "idoc": ["SRAI16 (SIMD 16-bit Shift Right Arithmetic Immediate)",
              "SRA16.u (SIMD 16-bit Rounding Shift Right Arithmetic Immediate)",
              "KSLLI16 (SIMD 16-bit Saturating Shift Left Logical Immediate)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
     "operation@srai16.u":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n",
     "operation@kslli16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x] << sa, 16);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a", "uimm4 b"],
     "insn": ["srl16", "srl16.u"],
     "available": "both",
     "subclass": 2,
     "idoc": [
         "SRL16 (SIMD 16-bit Shift Right Logical)",
         "SRL16.u (SIMD 16-bit Rounding Shift Right Logical)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
     "operation@srl16.u":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a", "const uimm4 b"],
     "insn": ["srli16", "srli16.u"],
     "available": "both",
     "subclass": 2,
     "idoc": [
         "SRLI16 (SIMD 16-bit Shift Right Logical Immediate)",
         "SRLI16.u (SIMD 16-bit Rounding Shift Right Logical Immediate)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
    "operation@srli16.u":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a", "uimm4 b"],
     "insn": ["sll16"],
     "available": "both",
     "subclass": 2,
     "idoc": ["SLL16 (SIMD 16-bit Shift Left Logical)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] << sa;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a", "const uimm4 b"],
     "insn": ["slli16"],
     "available": "both",
     "subclass": 2,
     "idoc": ["SLLI16 (SIMD 16-bit Shift Left Logical Immediate)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "sa = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] << sa;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a", "uimm3 b"],
     "insn": ["sra8", "sra8.u", "ksll8"],
     "available": "both",
     "subclass": 3,
     "idoc": ["SRA8 (SIMD 8-bit Shift Right Arithmetic)",
              "SRA8.u (SIMD 8-bit Rounding Shift Right Arithmetic)",
              "KSLL8 (SIMD 8-bit Saturating Shift Left Logical)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
     "operation@sra8.u":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 7;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n",
     "operation@ksll8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x] << sa, 8);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a", "bitv b"],
     "insn": ["kslra8", "kslra8.u"],
     "available": "both",
     "subclass": 3,
     "idoc": ["KSLRA8 (SIMD 8-bit Shift Left Logical with Saturation or Shift Right Arithmetic)",
              "KSLRA8.u (SIMD 8-bit Shift Left Logical with Saturation or Rounding Shift Right Arithmetic)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "if ((b & 0x8) != 0) {\n"
         "  sa = (sa == 0) ? 7 : (sa ^ 0x7) + 1;\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = a[x] >> sa;\n"
         "  }\n"
         "}\n"
         "else {\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = Sat_s(a[x] << sa, 8)"
         "  }\n"
         "}\n"
         "return res;\n",
     "operation@kslra8.u":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "if ((b & 0x8) != 0) {\n"
         "  sa = (sa == 0) ? 7 : (sa ^ 0x7) + 1;\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = Round(a[x], sa);\n"
         "  }\n"
         "}\n"
         "else {\n"
         "  for (x = 0; x < Loop; x++) {\n"
         "    res[x] = Sat_s(a[x] << sa, 8)"
         "  }\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a", "const uimm3 b"],
     "insn": ["srai8", "srai8.u", "kslli8"],
     "available": "both",
     "subclass": 3,
     "idoc": ["SRAI8 (SIMD 8-bit Shift Right Arithmetic Immediate)",
              "SRAI8.u (SIMD 8-bit Rounding Shift Right Arithmetic Immediate)",
              "KSLLI8 (SIMD 8-bit Saturating Shift Left Logical Immediate)"],
     "operation@srai8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
     "operation@srai8.u":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n",
     "operation@kslli8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x] << sa, 8);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a", "uimm3 b"],
     "insn": ["srl8", "srl8.u"],
     "available": "both",
     "subclass": 3,
     "idoc": ["SRL8 (SIMD 8-bit Shift Right Logical)",
              "SRL8.u (SIMD 8-bit Rounding Shift Right Logical)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
     "operation@srl8.u":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a", "const uimm3 b"],
     "insn": ["srli8", "srli8.u"],
     "available": "both",
     "subclass": 3,
     "idoc": ["SRLI8 (SIMD 8-bit Shift Right Logical Immediate)",
              "SRLI8.u (SIMD 8-bit Rounding Shift Right Logical Immediate)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] >> sa;\n"
         "}\n"
         "return res;\n",
     "operation@srli8.u":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Round(a[x], sa);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a", "uimm3 b"],
     "insn": ["sll8"],
     "available": "both",
     "subclass": 3,
     "idoc": ["SLLI8 (SIMD 8-bit Shift Left Logical)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] << sa;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a", "const uimm3 b"],
     "insn": ["slli8"],
     "available": "both",
     "subclass": 3,
     "idoc": ["SLLI8 (SIMD 8-bit Shift Left Logical Immediate)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "sa = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = a[x] << sa;\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int32x2", "int16x2 a", "int16x2 b"],
     "insn": ["smul16", "smulx16"],
     "available": "both",
     "Zpsf" : True,
     "subclass": 6,
     "idoc": ["SMUL16 (SIMD Signed 16-bit Multiply)",
              "SMULX16 (SIMD Signed Crossed 16-bit Multiply)"],
     "operation":
         "res[0] = a[0] * b[0];\n"
         "res[1] = a[1] * b[1];\n"
         "return res;\n",
     "operation@smulx16":
         "res[0] = a[0] * b[1];\n"
         "res[1] = a[1] * b[0];\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16x4", "int8x4 a", "int8x4 b"],
     "insn": ["smul8", "smulx8"],
     "available": "both",
     "Zpsf" : True,
     "subclass": 7,
     "idoc": ["SMUL8 (SIMD Signed 8-bit Multiply)",
              "SMULX8 (SIMD Signed Crossed 8-bit Multiply)"],
     "operation":
         "for (x = 0; x < 4; x++) {\n"
         "  res[x] = a[x] * b[x];\n"
         "}\n"
         "return res;\n",
     "operation@smulx8":
         "for (x = 0; x < 2; x++) {\n"
         "  res[2*x] = a[2*x] * b[2*x+1];\n"
         "  res[2*x+1] = a[2*x+1] * b[2*x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "int8xN a"],
     "insn": ["clrs8"],
     "available": "both",
     "subclass": 9,
     "idoc": ["CLRS8 (SIMD 8-bit Count Leading Redundant Sign)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  sign = (uint8_t)a[x] >> 7;\n"
         "  res[x] = 0;\n"
         "  for (i = 6; i>= 0; i--) {\n"
         "    if ((((uint8_t)a[x] >> i) & 1)== sign)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a"],
     "insn": ["clo8", "clz8"],
     "available": "both",
     "subclass": 9,
     "idoc": ["CLO8 (SIMD 8-bit Count Leading One)",
              "CLZ8 (SIMD 8-bit Count Leading Zero)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = 0;\n"
         "  for (i = 7; i>= 0; i--) {\n"
         "    if ((((uint8_t)a[x] >> i) & 1) == 1)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n",
     "operation@clz8":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = 0;\n"
         "  for (i = 7; i>= 0; i--) {\n"
         "    if (((uint8_t)a[x] >> i) & 1) == 0)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a"],
     "insn": ["kabs8"],
     "available": "both",
     "subclass": 9,
     "idoc": ["KABS8 (SIMD 8-bit Saturating Absolute)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(Abs(a[x]), 8);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "int16xN a"],
     "insn": ["clrs16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["CLRS16 (SIMD 16-bit Count Leading Redundant Sign)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  sign = (uint16_t)a[x] >> 15;\n"
         "  res[x] = 0;\n"
         "  for (i = 14; i>= 0; i--) {\n"
         "    if ((((uint16_t)a[x] >> i) & 1)== sign)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a"],
     "insn": ["clo16", "clz16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["CLO16 (SIMD 16-bit Count Leading One)",
              "CLZ16 (SIMD 16-bit Count Leading Zero)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = 0;\n"
         "  for (i = 15; i>= 0; i--) {\n"
         "    if ((((uint16_t)a[x] >> i) & 1) == 1)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n",
     "operation@clz16":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = 0;\n"
         "  for (i = 15; i>= 0; i--) {\n"
         "    if (((uint16_t)a[x] >> i) & 1) == 0)\n"
         "      res[x] ++;\n"
         "    else\n"
         "      break;\n"
         "  }\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a"],
     "insn": ["kabs16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["KABS16 (SIMD 16-bit Saturating Absolute)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(Abs(a[x]), 16);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int8xN a"],
     "insn": ["sunpkd810", "sunpkd820", "sunpkd830", "sunpkd831", "sunpkd832"],
     "available": "both",
     "subclass": 10,
     "idoc": ["SUNPKD810 (Signed Unpacking Bytes 1 & 0)",
              "SUNPKD820 (Signed Unpacking Bytes 2 & 0)",
              "SUNPKD830 (Signed Unpacking Bytes 3 & 0)",
              "SUNPKD831 (Signed Unpacking Bytes 3 & 1)",
              "SUNPKD832 (Signed Unpacking Bytes 3 & 2)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+1];\n"
         "  res[2*x] = a[4*x];\n"
         "}\n"
         "return res;\n",
     "operation@sunpkd820":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+2];\n"
         "  res[2*x] = a[4*x];\n"
         "}\n"
         "return res;\n",
     "operation@sunpkd830":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+3];\n"
         "  res[2*x] = a[4*x];\n"
         "}\n"
         "return res;\n",
     "operation@sunpkd831":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+3];\n"
         "  res[2*x] = a[4*x+1];\n"
         "}\n"
         "return res;\n",
     "operation@sunpkd832":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+3];\n"
         "  res[2*x] = a[4*x+2];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint8xN a"],
     "insn": ["zunpkd810", "zunpkd820", "zunpkd830", "zunpkd831", "zunpkd832"],
     "available": "both",
     "subclass": 10,
     "idoc": ["ZUNPKD810 (Unsigned Unpacking Bytes 1 & 0)",
              "ZUNPKD820 (Unsigned Unpacking Bytes 2 & 0)",
              "ZUNPKD830 (Unsigned Unpacking Bytes 3 & 0)",
              "ZUNPKD831 (Unsigned Unpacking Bytes 3 & 1)",
              "ZUNPKD832 (Unsigned Unpacking Bytes 3 & 2)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+1];\n"
         "  res[2*x] = a[4*x];\n"
         "}\n"
         "return res;\n",
     "operation@zunpkd820":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+2];\n"
         "  res[2*x] = a[4*x];\n"
         "}\n"
         "return res;\n",
     "operation@zunpkd830":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+3];\n"
         "  res[2*x] = a[4*x];\n"
         "}\n"
         "return res;\n",
     "operation@zunpkd831":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+3];\n"
         "  res[2*x] = a[4*x+1];\n"
         "}\n"
         "return res;\n",
     "operation@zunpkd832":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x+1] = a[4*x+3];\n"
         "  res[2*x] = a[4*x+2];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int8xN", "int8xN a", "const uimm3 b"],
     "insn": ["sclip8"],
     "available": "both",
     "subclass": 9,
     "idoc": ["SCLIP8 (SIMD 8-bit Signed Clip Value)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "imm3u = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x], imm3u + 1);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["int16xN", "int16xN a", "const uimm4 b"],
     "insn": ["sclip16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["SCLIP16 (SIMD 8-bit Signed Clip Value)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "imm4u = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_s(a[x], imm4u + 1);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "int8xN a", "const uimm3 b"],
     "insn": ["uclip8"],
     "available": "both",
     "subclass": 9,
     "idoc": ["UCLIP8 (SIMD 8-bit Unsigned Clip Signed Value)"],
     "operation":
         "RV32: Loop = 4;\n"
         "RV64: Loop = 8;\n"
         "imm3u = b & 0x7;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_u(a[x], imm3u);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "int16xN a", "const uimm4 b"],
     "insn": ["uclip16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["UCLIP16 (SIMD 16-bit Unsigned Clip Signed Value)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "imm4u = b & 0xf;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[x] = Sat_u(a[x], imm4u);\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint8xN", "uint8xN a"],
     "insn": ["swap8"],
     "available": "both",
     "subclass": 9,
     "idoc": ["SWAP8 (Swap Byte within Halfword)"],
     "operation":
         "RV32: Loop = 2;\n"
         "RV64: Loop = 4;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x] = a[2*x+1];\n"
         "  res[2*x+1] = a[2*x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint16xN", "uint16xN a"],
     "insn": ["swap16"],
     "available": "both",
     "subclass": 8,
     "idoc": ["SWAP16 (Swap Halfword within Word)"],
     "operation":
         "RV32: Loop = 1;\n"
         "RV64: Loop = 2;\n"
         "for (x = 0; x < Loop; x++) {\n"
         "  res[2*x] = a[2*x+1];\n"
         "  res[2*x+1] = a[2*x];\n"
         "}\n"
         "return res;\n"
     },

    {"name": "$insn",
     "prototype": ["uint32x2", "uint16x2 a", "uint16x2 b"],
     "insn": ["umul16", "umulx16"],
     "available": "both",
     "Zpsf" : True,
     "subclass": 6,
     "idoc": ["UMUL16 (SIMD Unsigned 16-bit Multiply)",
              "UMULX16 (SIMD Unsigned Crossed 16-bit Multiply)"],
     "operation":
         "for (x = 0; x < 2; x++) {\n"
         "  res[x] = a[x] * b[x];\n"
         "}\n"
         "return res;\n",
     "operation@umulx16":
         "res[0] = a[0] * b[1];\n"
         "res[1] = a[1] * b[0];\n"
         "return res;\n",
     },

    {"name": "$insn",
     "prototype": ["uint16x4", "uint8x4 a", "uint8x4 b"],
     "insn": ["umul8", "umulx8"],
     "available": "both",
     "Zpsf" : True,
     "subclass": 7,
     "idoc": ["UMUL8 (SIMD Unsigned 8-bit Multiply)",
              "UMULX8 (SIMD Unsigned Crossed 8-bit Multiply)"],
     "operation":
         "for (x = 0; x < 4; x++) {\n"
         "  res[x] = a[x] * b[x];\n"
         "}\n"
         "return res;\n",
     "operation@umulx16":
         "for (x = 0; x < 2; x++) {\n"
         "  res[2*x] = a[2*x] * b[2*+1];\n"
         "  res[2*x+1] = a[2*x+1] * b[2*x];\n"
         "}\n"
         "return res;\n"
     },
]

for insns in buitins_simd_data:
    insns["class"] = "SIMD Data Processing"
for insns in buitins_partial_simd_data:
    insns["class"] = "Partial-SIMD Data Processing"
for insns in builtins_64bit_profile:
    insns["class"] = "64-bit Profile"
for insns in builtins_non_simd:
    insns["class"] = "Non-SIMD"
for insns in builtins_only64:
    insns["class"] = "RV64 Only"

builtins = buitins_simd_data + buitins_partial_simd_data + builtins_64bit_profile + builtins_non_simd + builtins_only64

build_builtins_head = '''
#ifndef __GCC_RISCV_DSP_H__
#define __GCC_RISCV_DSP_H__

#include <stdint.h>

typedef signed char int8x4_t __attribute__ ((vector_size (4)));
typedef unsigned char uint8x4_t __attribute__ ((vector_size (4)));
typedef signed char int8x8_t __attribute__ ((vector_size (8)));
typedef unsigned char uint8x8_t __attribute__ ((vector_size (8)));

typedef signed short int16x2_t __attribute__ ((vector_size (4)));
typedef unsigned short uint16x2_t __attribute__ ((vector_size (4)));
typedef signed short int16x4_t __attribute__ ((vector_size (8)));
typedef unsigned short uint16x4_t __attribute__ ((vector_size (8)));

typedef signed int int32x2_t __attribute__ ((vector_size (8)));
typedef unsigned int uint32x2_t __attribute__ ((vector_size (8)));

#if __riscv_xlen == 32
typedef int32_t  intXLEN_t;
typedef uint32_t uintXLEN_t;
#elif __riscv_xlen == 64
typedef int64_t  intXLEN_t;
typedef uint64_t uintXLEN_t;
#endif

#if !__riscv_dsp && !__riscv_zpsfoperand
#error "dsp require -march the 'p' extension or 'zpsfoperand' extension"
#endif

'''
build_builtins_tail = '''
#endif // __GCC_RISCV_DSP_H__
'''
build_builtins_prototype = '''
__extension__ static __inline __attribute__ ((__always_inline__))
%(w)s {
%(i)s
}
'''

def build_builtins(ofname="riscv-dsp.h"):
    def isindex_type(itype):
        index_types = {
            "uimm6", "uimm5", "uimm4", "uimm3", "uimm2",
            "uimmw", "uimmwbyte"
        }
        for index_type in index_types:
            if index_type in itype:
                return True;
        return False;

    def isconst_type(itype):
        return itype.startswith("const")

    def turetype(the_type, size):
        if the_type in simdctypes.keys():
            return simdctypes[the_type].get("type"+size) or simdctypes[the_type].get("type")
        elif the_type == "intXLEN_t":
            return simdctypes["integer"].get("type"+size)
        elif the_type == "uintXLEN_t":
            return simdctypes["uinteger"].get("type"+size)
        else:
            return the_type.lstrip("@")

    def build_builtins_type_lower(insn, vdefs, tdefs):

        if insn["expandmode"] == "ulong":
            return
        if "both signed and unsigned" in insn.keys() \
                and insn["both signed and unsigned"] == "u":
            return

        if insn["expandmode"] == "both":
            name32 = insn["builtin name32"]
            insn["available"] = "32"
            build_builtins_type_gcc(name32, insn, vdefs, tdefs)
            name64 = insn["builtin name64"]
            insn["available"] = "64"
            build_builtins_type_gcc(name64, insn, vdefs, tdefs)
            insn["available"] = "both"
        else:
            name = insn["builtin name"]
            build_builtins_type_gcc(name, insn, vdefs, tdefs)

    def build_builtins_insn_expand(insn):

        def expand_init(insn):
            if "class" not in insn.keys():
                insn["class"] = "Unknow"
            if "insn" not in insn.keys():
                insn["insn"] = [insn["name"]]
            return [insn]

        def expand_def(insn):
            if "expand" in insn.keys():
                return insn["expand"](insn)
            return [insn]

        def expand_bothsignedandunsigned(insn):
            rets = [insn]
            prototype = insn["prototype"]
            ret = prototype[0]
            args = prototype[1:]
            insn["ret_type"] = ret
            insn["args"] = [] if ("void" in args) else list(map(lambda m: m.split()[-1], args))
            insn["args_type"] = ["void"] if ("void" in args) else list(map(lambda m: " ".join(m.split()[:-1]), args))

            all_type = [ret] + insn["args_type"]
            hasU = False
            for i in all_type:
                if "?" in i: hasU = True

            if not hasU: return rets

            ou = copy.deepcopy(insn)
            insn["both signed and unsigned"] = "s"
            ou["both signed and unsigned"] = "u"

            if "?" in ret:
                ou["cast_returnu"] = insn["ret_type"] = ret.replace("?", "")
                if insn["ret_type"].lstrip("?") in simdctypes:
                    ou["ret_type"] = ret.replace("?", "u")
                else:
                    ou["ret_type"] = ret.replace("?", "unsigned ")

            args_type = insn["args_type"]
            for index in range(len(args_type)):
                i = args_type[index]
                if "?" in i:
                    args_type[index] = insn["args_type"][index] = i.replace("?", "")
                    if i.lstrip("?") in simdctypes:
                        ou["args_type"][index] = i.replace("?", "u")
                    else:
                        ou["args_type"][index] = i.replace("?", "unsigned ")
            ou["cast_argsu"] = args_type
            rets.append(ou)
            return rets

        def expand_idoc(insn, index):

            def expand_idoc_operation(okeys, insn):
                for key in okeys:
                    insns = key.split("@")[-1].split(",")
                    if insn in insns:
                        return key
                return ""

            def expand_idoc_format(doc, op):
                if doc.find("$idoc") != -1:
                    doc = doc.replace("$idoc", op)
                doc = doc.replace("\n", "\n ")
                doc = " " + doc
                return doc

            if "idoc" not in insn.keys():
                return

            if "subclass" not in insn.keys():
                assert index < len(insn["isubclass"]), insn
                insn["subclass"] = insn["isubclass"][index]

            assert index < len(insn["idoc"]), insn
            idoc = insn["idoc"][index].split("@")[0]
            op = insn["idoc"][index].split("@")[-1]
            iname = insn["insn"][0]

            if "insnu" in insn.keys() and "types" in insn.keys():
                un = insn["types"].startswith("u") and "un" or ""
                idoc = idoc.replace("<$insnu>", un)

            insn["idoc"] = idoc

            okeys = list(filter(lambda m: m.startswith("operation@"), insn.keys()))
            mkeys = list(filter(lambda m: m.startswith("moperation@"), insn.keys()))

            okeys = expand_idoc_operation(okeys, iname)
            if okeys:
                insn["operation"] = insn[okeys]
            mkeys = expand_idoc_operation(mkeys, iname)
            if mkeys:
                insn["moperation"] = insn[mkeys]

            op = op != idoc and op or iname

            if "operation" in insn.keys():
                insn["operation"] = expand_idoc_format(insn["operation"], op)

            if "moperation" in insn.keys():
                insn["moperation"] = expand_idoc_format(insn["moperation"], op)

        def expand_expandmode(insn):
            rets = []
            ret = insn["ret_type"]
            args_type = insn["args_type"]
            all_type = args_type + [ret]
            only32 = False
            only64 = False
            the_class = insn["available"]
            if the_class == "64":
                only64 = True
            if the_class == "32":
                only32 = True

            mode32 = ""
            mode64 = ""
            hasulong = False

            def ulongsame_p(checktype, size):
                if checktype not in simdctypes.keys():
                    return True
                if turetype(checktype, size) == turetype(simdctypes[checktype]["ulong"], size):
                    return True
                return False

            nopr32 = True
            nopr64 = True
            hasboth = False
            if only64:
                for t in all_type:
                    if not ulongsame_p(t, "64"):
                        nopr64 = False
            elif only32:
                for t in all_type:
                    if not ulongsame_p(t, "32"):
                        nopr64 = False
            else:
                for t in all_type:
                    if t not in simdctypes.keys(): continue
                    if "mode32" in simdctypes[t].keys():
                        if not mode32: mode32 = simdctypes[t]["mode32"]
                    if "mode64" in simdctypes[t].keys():
                        if not mode64: mode64 = simdctypes[t]["mode64"]
                    if not ulongsame_p(t, "32"):
                        nopr32 = False
                    if not ulongsame_p(t, "64"):
                        nopr64 = False
                if not nopr32 and not nopr64:
                    hasboth = True
                    for t in all_type:
                        if turetype(t, "32") != turetype(t, "64") and t not in {"integer", "uinteger", "intXLEN", "uintXLEN"}:
                            hasboth = False
                            break

            oulong = copy.deepcopy(insn)
            oulong["expandmode"] = "ulong"
            if mode32:
                    oulong["mode32"] = mode32
            if mode64:
                    oulong["mode64"] = mode64
            if only32:
                    oulong["available"] = "32"
            elif only64:
                    oulong["available"] = "64"
            else:
                    oulong["available"] = "both"
            oulong["nopr32"] = nopr32
            oulong["nopr64"] = nopr64
            rets.append(oulong)

            if hasboth:
                oboth = copy.deepcopy(insn)
                oboth["expandmode"] = "both"
                if mode32:
                    oboth["mode32"] = mode32
                if mode64:
                    oboth["mode64"] = mode64
                oboth["available"] = "both"
                rets.append(oboth)
                return rets

            if not only64:
                o32 = copy.deepcopy(insn)
                o32["expandmode"] = "32"
                o32["mode32"] = mode32
                o32["nopr"] = nopr32
                o32["hasulong"] = hasulong
                o32["available"] = "32"
                rets.append(o32)
            if not only32:
                o64 = copy.deepcopy(insn)
                o64["expandmode"] = "64"
                o64["mode64"] = mode64
                o64["nopr"] = nopr64
                o64["hasulong"] = hasulong
                o64["available"] = "64"
                rets.append(o64)
            return rets

        def expand_type(insn):
            rets = [insn]

            mode = insn["expandmode"]
            ret = insn["ret_type"]
            args_type = insn["args_type"][:]
            args = insn["args"][:]
            cname = "__rv__"

            def hasRedundancyCabi(insn):
                redoundancytype = {"slli": "sll", "srli": "srl", "srai": "sra", "bitrevi": "bitrev", "wexti": "wext"}
                for rname in redoundancytype.keys():
                    if "sraiw" in insn["name"]:
                        insn["name"] = insn["name"].replace("sraiw", "sraw")
                        return False
                    elif rname in insn["name"]:
                        insn["name"] = insn["name"].replace(rname, redoundancytype[rname])
                        return True
                return False

            if hasRedundancyCabi(insn): insn["redundancyCabi"] = True
            name = insn["name"]


            if mode == "ulong":
                if "both signed and unsigned" in insn.keys():
                    if insn["both signed and unsigned"] == "u" \
                            and not (insn["nopr32"] and insn["nopr64"]):
                        return []
                    if insn["nopr32"] and insn["nopr64"]:
                        insn["name"] = insn["both signed and unsigned"] + insn["name"]

                cname = cname + insn["name"]
                if ret in simdctypes.keys():
                    if ("both signed and unsigned" in insn.keys() and insn["nopr32"] or insn["available"] == "64") and ret == "int32xN":
                        insn["ret_type"] = "uintXLEN_t"
                    else:
                        insn["ret_type"] = simdctypes[ret]["ulong"]
                else:
                    insn["ret_type"] = ret.lstrip("@")
                cname = insn["ret_type"] + " " + cname
                cname_arg = []
                for index in range(len(args_type)):
                    i = args_type[index]
                    if i == "void":
                        cname_arg.append(i)
                        break
                    if i in simdctypes.keys():
                        if ("both signed and unsigned" in insn.keys() and insn["nopr32"] or insn["available"] == "64") and i == "int32xN":
                            i = "uintXLEN_t"
                        else:
                            i = simdctypes[i]["ulong"]
                    else:
                        i = i.replace("@", "")
                    insn["args_type"][index] = i
                    if isindex_type(i):
                        if isconst_type(i): i = "const uint32_t"
                        else: i = "uint32_t"
                    cname_arg.append(i + " " + args[index])
                cname = cname + "(" + ", ".join(cname_arg) + ")"
                insn["capi_name"] = cname

                if "cast_returnu" in insn.keys(): ret = insn["cast_returnu"]
                if "cast_argsu" in insn.keys(): args_type = insn["cast_argsu"][:]

                if "mode32" in insn.keys():
                    bname32 = "__builtin_riscv_"
                    bname32 += name + "_" + insn["mode32"]

                    if turetype(insn["ret_type"], "32") != turetype(ret, "32"):
                        bname32 = "(" + turetype(insn["ret_type"], "32") + ") " + bname32

                    args32 = []
                    for index in range(len(args)):
                        i = args_type[index]
                        i = turetype(i, "32")
                        if i != turetype(insn["args_type"][index], "32"):
                            args32.append("(" + i + ")" + insn["args"][index])
                        else:
                            args32.append(insn["args"][index])
                    bname32 += "(" + ", ".join(args32) + ")"
                    insn["builtin32"] = bname32

                if "mode64" in insn.keys():
                    bname64 = "__builtin_riscv_"
                    bname64 += name + "_" + insn["mode64"]

                    if turetype(insn["ret_type"], "64") != turetype(ret, "64"):
                        bname64 = "(" + turetype(insn["ret_type"], "64") + ") " + bname64

                    args64 = []
                    for index in range(len(args)):
                        i = args_type[index]
                        i = turetype(i, "64")
                        if i != turetype(insn["args_type"][index], "64"):
                            args64.append("(" + i + ")" + insn["args"][index])
                        else:
                            args64.append(insn["args"][index])
                    bname64 += "(" + ", ".join(args64) + ")"
                    insn["builtin64"] = bname64

                if "mode32" not in insn.keys() and "mode64" not in insn.keys():
                    bname = "__builtin_riscv_" + name

                    selsize = insn["available"] != "both" and insn["available"] or ""

                    if turetype(insn["ret_type"], selsize) != turetype(ret, selsize):
                        bname = "(" + turetype(insn["ret_type"], selsize) + ") " + bname

                    argsboth = []

                    for index in range(len(args)):
                        i = args_type[index]
                        i = turetype(i, selsize)

                        if i != turetype(insn["args_type"][index], selsize):
                            argsboth.append("(" + i + ")" + insn["args"][index])
                        else:
                            argsboth.append(insn["args"][index])
                    bname += "(" + ", ".join(argsboth) + ")"
                    insn["builtin"] = bname

            elif mode == "both":
                insn["builtin name32"] = insn["name"] + (insn.get("mode32") and "_" + insn["mode32"] or "")
                insn["builtin name64"] = insn["name"] + (insn.get("mode64") and "_" + insn["mode64"] or "")
                if "both signed and unsigned" in insn.keys():
                    insn["name"] = insn["both signed and unsigned"] + insn["name"]
                insn["name"] = "v_" + insn["name"]
                cname = cname + insn["name"]
                if ret in simdctypes.keys():
                    insn["ret_type"] = simdctypes[ret].get("type") or simdctypes[ret]["ulong"]
                else:
                    insn["ret_type"] = ret.lstrip("@")
                cname = insn["ret_type"] + " " + cname
                cname_arg = []
                for index in range(len(args_type)):
                    i = args_type[index]
                    if i == "void":
                        cname_arg.append(i)
                        break
                    if i in simdctypes.keys():
                        i = simdctypes[i].get("type") or simdctypes[i]["ulong"]
                    else:
                        i = i.replace("@", "")
                    insn["args_type"][index] = i
                    if isindex_type(i):
                        if isconst_type(i): i = "const uint32_t"
                        else: i = "uint32_t"
                    cname_arg.append(i + " " + args[index])
                cname = cname + "(" + ", ".join(cname_arg) + ")"
                insn["capi_name"] = cname

                if "cast_returnu" in insn.keys(): ret = insn["cast_returnu"]
                if "cast_argsu" in insn.keys(): args_type = insn["cast_argsu"][:]

                if "mode32" in insn.keys():
                    bname32 = "__builtin_riscv_"
                    bname32 += name + "_" + insn["mode32"]

                    if turetype(insn["ret_type"], "32") != turetype(ret, "32"):
                        bname32 = "(" + turetype(insn["ret_type"], "32") + ") " + bname32

                    args32 = []
                    for index in range(len(args)):
                        i = args_type[index]
                        i = turetype(i, "32")
                        if i != turetype(insn["args_type"][index], "32"):
                            args32.append("(" + i + ")" + insn["args"][index])
                        else:
                            args32.append(insn["args"][index])
                    bname32 += "(" + ", ".join(args32) + ")"
                    insn["builtin32"] = bname32

                if "mode64" in insn.keys():
                    bname64 = "__builtin_riscv_"
                    bname64 += name + "_" + insn["mode64"]

                    if turetype(insn["ret_type"], "64") != turetype(ret, "64"):
                        bname64 = "(" + turetype(insn["ret_type"], "64") + ") " + bname64

                    args64 = []
                    for index in range(len(args)):
                        i = args_type[index]
                        i = turetype(i, "64")
                        if i != turetype(insn["args_type"][index], "64"):
                            args64.append("(" + i + ")" + insn["args"][index])
                        else:
                            args64.append(insn["args"][index])
                    bname64 += "(" + ", ".join(args64) + ")"
                    insn["builtin64"] = bname64

                if "mode32" not in insn.keys() and "mode64" not in insn.keys():
                    bname = "__builtin_riscv_" + name

                    selsize = insn["available"] != "both" and insn["available"] or ""

                    if turetype(insn["ret_type"], selsize) != turetype(ret, selsize):
                        bname = "(" + turetype(insn["ret_type"], selsize) + ") " + bname

                    argsboth = []

                    for index in range(len(args)):
                        i = args_type[index]
                        i = turetype(i, selsize)

                        if i != turetype(insn["args_type"][index], selsize):
                            argsboth.append("(" + i + ")" + insn["args"][index])
                        else:
                            argsboth.append(insn["args"][index])
                    bname += "(" + ", ".join(argsboth) + ")"
                    insn["builtin"] = bname

            elif mode == "32":
                insn["builtin name"] = insn["name"] + (insn["mode32"] and "_" + insn["mode32"] or "")
                if "both signed and unsigned" in insn.keys():
                    insn["name"] = insn["both signed and unsigned"] + insn["name"]
                insn["name"] = "v_" + insn["name"]
                cname = cname + insn["name"]
                if ret in simdctypes.keys():
                    insn["ret_type"] = simdctypes[ret].get("type32") or simdctypes[ret]["type"]
                else:
                    insn["ret_type"] = ret.lstrip("@")
                cname = insn["ret_type"] + " " + cname
                cname_arg = []
                for index in range(len(args_type)):
                    i = args_type[index]
                    if i == "void":
                        cname_arg.append(i)
                        break
                    if i in simdctypes.keys():
                        i = simdctypes[i].get("type32") or simdctypes[i]["type"]
                    else:
                        i = i.replace("@", "")
                    insn["args_type"][index] = i
                    if isindex_type(i):
                        if isconst_type(i): i = "const uint32_t"
                        else: i = "uint32_t"
                    cname_arg.append(i + " " + args[index])
                cname = cname + "(" + ", ".join(cname_arg) + ")"
                insn["capi_name"] = cname

                if "cast_returnu" in insn.keys(): ret = insn["cast_returnu"]
                if "cast_argsu" in insn.keys(): args_type = insn["cast_argsu"][:]

                bname32 = "__builtin_riscv_" + insn["builtin name"]

                if ret in simdctypes.keys():
                    ret = simdctypes[ret].get("type32") or simdctypes[ret]["type"]
                else:
                    ret = ret.lstrip("@")
                if insn["ret_type"] != ret:
                    bname32 = "(" + insn["ret_type"] + ") " + bname32

                args32 = []
                for index in range(len(args)):
                    i = args_type[index]
                    if i in simdctypes.keys():
                        i = simdctypes[i].get("type32") or simdctypes[i]["type"]
                    else:
                        i = i.lstrip("@")
                    if i != insn["args_type"][index]:
                        args32.append("(" + i + ")" + insn["args"][index])
                    else:
                        args32.append(insn["args"][index])
                bname32 += "(" + ", ".join(args32) + ")"
                insn["builtin"] = bname32

            elif mode == "64":
                insn["builtin name"] = insn["name"] + (insn["mode64"] and "_" + insn["mode64"] or "")
                if "both signed and unsigned" in insn.keys():
                    insn["name"] = insn["both signed and unsigned"] + insn["name"]
                insn["name"] = "v_" + insn["name"]
                cname = cname + insn["name"]
                if ret in simdctypes.keys():
                    insn["ret_type"] = simdctypes[ret].get("type64") or simdctypes[ret]["type"]
                else:
                    insn["ret_type"] = ret.lstrip("@")
                cname = insn["ret_type"] + " " + cname
                cname_arg = []
                for index in range(len(args_type)):
                    i = args_type[index]
                    if i == "void":
                        cname_arg.append(i)
                        break
                    if i in simdctypes.keys():
                        i = simdctypes[i].get("type64") or simdctypes[i]["type"]
                    else:
                        i = i.replace("@", "")
                    insn["args_type"][index] = i
                    if isindex_type(i):
                        if isconst_type(i): i = "const uint32_t"
                        else: i = "uint32_t"
                    cname_arg.append(i + " " + args[index])
                cname = cname + "(" + ", ".join(cname_arg) + ")"
                insn["capi_name"] = cname

                if "cast_returnu" in insn.keys(): ret = insn["cast_returnu"]
                if "cast_argsu" in insn.keys(): args_type = insn["cast_argsu"][:]

                bname64 = "__builtin_riscv_" + insn["builtin name"]

                if ret in simdctypes.keys():
                    ret = simdctypes[ret].get("type64") or simdctypes[ret]["type"]
                else:
                    ret = ret.lstrip("@")
                if insn["ret_type"] != ret:
                    bname64 = "(" + insn["ret_type"] + ") " + bname64

                args64 = []
                for index in range(len(args)):
                    i = args_type[index]
                    if i in simdctypes.keys():
                        i = simdctypes[i].get("type64") or simdctypes[i]["type"]
                    else:
                        i = i.lstrip("@")
                    if i != insn["args_type"][index]:
                        args64.append("(" + i + ")" + insn["args"][index])
                    else:
                        args64.append(insn["args"][index])
                bname64 += "(" + ", ".join(args64) + ")"
                insn["builtin"] = bname64

            return rets

        def expand_insn(insn):
            rets = []
            if not re.match(".*\\$insn", insn["name"]):
                expand_idoc(insn, 0)
                return [insn]
            for i in insn["insn"]:
                oi = copy.deepcopy(insn)
                index = oi["insn"].index(i)
                if "insnu" in oi.keys():
                    if len(oi["insnu"]) > index and oi["types"].startswith("u"):
                        i = oi["insnu"][index]
                oi["name"] = oi["name"].replace("$insn", i.replace(".", "_"))
                oi["insn"] = [i]
                expand_idoc(oi, index)
                rets.append(oi)
            return rets

        expands = [expand_init, expand_def, expand_bothsignedandunsigned, expand_expandmode, expand_insn, expand_type]

        def expander(expands, i, insn):
            rets = []
            if not i:
                return expands[i](insn)
            for x in expander(expands, i - 1, insn):
                rets += expands[i](x)
            return rets

        return expander(expands, len(expands) - 1, insn)

    def build_builtins_insn(insns):
        rets = []
        for insn in insns:
            ret = build_builtins_insn_expand(insn)
            for insn in ret:
                rets.append(insn)
        return rets

    build_builtins_type_gcc_ftype = ["DEF_RISCV_FTYPE (0, (USI))\n",
                                     "DEF_RISCV_FTYPE (1, (VOID, USI))\n"]

    def build_builtins_type_gcc(name, insn, vdefs, tdefs):

        def type_gcc_ftype(prototype, tdefs, defs="riscv-ftypes.def"):
            xdefs = build_builtins_type_gcc_ftype
            with open(defs) as ifp:
                for i in ifp.readlines():
                    if not i:
                        continue
                    if not i.startswith("DEF_RISCV_FTYPE"):
                        continue
                    xdefs.append(i)
            patten = "DEF_RISCV_FTYPE (%d, (%s, %s))\n"
            prototype = prototype.replace("RISCV", "").split("FTYPE")
            prototype = list(map(lambda m: m.strip("_"), prototype))
            ret = prototype[0]
            args = prototype[1].split("_")
            if args[0] == "":
                patten = "DEF_RISCV_FTYPE (0, (%s))\n" % (ret)
            else:
                patten = patten % (len(args), ret, ", ".join(args))
            if patten not in tdefs and patten not in xdefs:
                tdefs.append(patten)

        if insn["available"] == "64":
            condition = "dsp64"
        elif insn.get("Zpsf"):
            condition = "zpsfoperand"
        elif insn["available"] == "32":
            condition = "dsp32"
        else:
            condition = "dsp"

        for i in range(len(vdefs)):
            iname = vdefs[i].split("(")[1].split(",")[0]
            if name == iname:
                old_condition = vdefs[i].split("(")[1].split(",")[2].rstrip(")").strip()
                if old_condition == "dsp32" and condition == "dsp64":
                    vdefs[i] = vdefs[i].replace("dsp32", "dsp")
                return

        ictypes = {
            "void": "VOID",
            "int8_t": "QI",
            "uint8_t": "UQI",
            "int8x4_t": "V4QI", "int8x8_t": "V8QI",
            "uint8x4_t": "UV4QI", "uint8x8_t": "UV8QI",
            "int16_t": "HI",
            "uint16_t": "UHI",
            "int16x2_t": "V2HI", "int16x4_t": "V4HI",
            "uint16x2_t": "UV2HI", "uint16x4_t": "UV4HI",
            "int32_t": "SI",
            "uint32_t": "USI",
            "int32x1_t": "SI", "int32x2_t": "V2SI",
            "uint32x2_t": "USI", "uint32x2_t": "UV2SI",

            "int64_t": "DI",
            "uint64_t": "UDI",
            "int64x1_t": "DI",
            "uint64x1_t": "UDI",

            "float16": "HF",
            "float": "SF",
            "double": "DF"
        }

        ret = copy.deepcopy(insn["ret_type"])
        args = copy.deepcopy(insn["args_type"])
        ipatten = "\nDIRECT%s_BUILTIN (%s, %s, %s),"
        iprototype = "RISCV_%s_FTYPE%s"
        ireturn = ret == "void" and "_NO_TARGET" or ""

        if ret == "intXLEN_t":
            if insn["available"] == "32":
                ret = "SI"
            else:
                ret = "DI"
        elif ret == "uintXLEN_t":
            if insn["available"] == "32":
                ret = "USI"
            else:
                ret = "UDI"
        else:
            ret = ictypes[ret]
        for i in range(len(args)):
            if args[i] == "void":
                args.pop(i)
                break
            if args[i] == "intXLEN_t":
                if insn["available"] == "32":
                    args[i] = "_SI"
                else:
                    args[i] = "_DI"
            elif args[i] == "uintXLEN_t":
                if insn["available"] == "32":
                    args[i] = "_USI"
                else:
                    args[i] = "_UDI"
            elif args[i] == "const uimmw":
                if insn["available"] == "32":
                    args[i] = "_CUIMM5"
                else:
                    args[i] = "_CUIMM6"
            elif args[i] == "const uimmwbyte":
                if insn["available"] == "32":
                    args[i] = "_CUIMM2"
                else:
                    args[i] = "_CUIMM3"
            elif args[i] == "uimmw":
                if insn["available"] == "32":
                    args[i] = "_UIMM5"
                else:
                    args[i] = "_UIMM6"
            elif args[i] == "uimmwbyte":
                if insn["available"] == "32":
                    args[i] = "_UIMM2"
                else:
                    args[i] = "_UIMM3"
            elif isconst_type(args[i]):
                args[i] = "_C" + args[i].split()[1].upper()
            elif isindex_type(args[i]):
                args[i] = "_" + args[i].split()[0].upper()
            else:
                args[i] = "_" + ictypes[args[i]]
        iprototype = iprototype % (ret, "".join(args))

        ret = ipatten % (ireturn, name, iprototype, condition)
        if ret not in vdefs:
            vdefs.append(ret)
        type_gcc_ftype(iprototype, tdefs)

    insns = build_builtins_insn(builtins)

    vdefs = []
    tdefs = []

    Zpsf_head = "\n#if __riscv_zpsfoperand\n"
    Zpsf_end = "\n#endif //__riscv_zpsfoperand\n"
    p_head = "\n#if __riscv_dsp\n"
    p_end = "\n#endif //__riscv_dsp\n"

    with open(ofname, "w") as ofp:
        ofp.write(build_builtins_head)
        insns.sort(key=lambda x: [True, None].index(x.get("Zpsf")))
        insnszp_or_p = groupby(insns, key=methodcaller('get', "Zpsf"))

        for zpflag, groupzp_or_p in insnszp_or_p:
            if zpflag == True:
                ofp.write(Zpsf_head)
                endt = Zpsf_end
            else:
                ofp.write(p_head)
                endt = p_end

            insnsg = sorted(groupzp_or_p, key=itemgetter("available"))
            insnsg = groupby(insnsg, itemgetter("available"))

            for available, group in insnsg:
                has_print_availabe = False

                group = list(group)
                group.sort(key=itemgetter('prototype'))
                for i in group:
                    if i.get("redundancyCabi"): continue
                    build_builtins_type_lower(i, vdefs, tdefs)
                    has_return = "" if i["ret_type"] == "void" else "return "
                    if "builtin32" in i.keys() and "builtin64" in i.keys():
                        patten = "#if __riscv_xlen == 64\n" \
                                "  %s%s;\n" \
                                "#elif __riscv_xlen == 32\n" \
                                "  %s%s;\n" \
                                "#endif" %(has_return, i["builtin64"], has_return, i["builtin32"])
                    elif "builtin" in i.keys():
                        patten = "  %s%s;" % (has_return, i["builtin"])
                    else:
                        print("bad builtin")
                        print(i)
                        exit(-1)
                    if available in ["64", "32"]:
                        i["capi_name"] = i["capi_name"].replace("XLEN", available)
                    prototype = {
                        "w": "%s" % (i["capi_name"]),
                        "i": patten,
                    }
                    if "nopr" not in i.keys() or not i["nopr"]:
                        if not has_print_availabe:
                            if available == "64":
                                ofp.write("\n#if __riscv_xlen == 64\n")
                            elif available == "32":
                                ofp.write("\n#if __riscv_xlen == 32\n")
                            has_print_availabe = True
                        ofp.write(build_builtins_prototype % prototype)
                if available in ["64", "32"] and has_print_availabe:
                        ofp.write("\n#endif // __riscv_xlen == %s\n" %(available))

            ofp.write(endt)

        ofp.write(build_builtins_tail)

    files = {
        "riscv-builtins-p.def": sorted(vdefs, key=lambda m: m.split("(")[-1].split(",")[0]),
        "riscv-ftypes-p.def": sorted(tdefs, key=lambda m: m.split("(")[1]),
    }

    for k, v in files.items():
        if not v:
            continue
        with open(k, "w") as ofp:
            for i in v:
                ofp.write(i)
    return insns

def build_tests(insns, outdir = "../../testsuite/gcc.target/riscv/dsp"):
    build_builtins_head_t = \
'''
/* { dg-do assemble } */
/* { dg-skip-if "test P-ext insns" { *-*-* } { "*" } { %(opt)s } } */
/* { dg-options "-O2 --save-temps" } */

#include <riscv-dsp.h>
'''
    build_builtins_tail_t = \
'''
/* { dg-final { cleanup-saved-temps } } */
'''

    def build_tests_auto(insn, ofp):
        build_builtins_prototype_t = \
'''
%(w)s {
%(i)s
}

'''
        def auto_insn(insn, types):
            if not types:
                return insn
            types = ",e" + re.findall("[1-9]+xm[1-9]+", types)[0].replace("x", ",")
            pattern = "vsetvli\\tzero,x0%s\\n\\t%s"
            return pattern % (types, insn)

        insns = []
        count = 0
        ret, name, args = insn["prototype"]
        types = "types" in insn.keys() and insn["types"] or ""

        prototype = {
            "w"     : "%s %s (%s)" % (ret, "test_" + name, ", ".join(args)),
            "i"     : insn["coperation"],
        }
        ofp.write(build_builtins_prototype_t % prototype)
        insns += map(lambda m: auto_insn(m, types), insn["insn"])
        count += 1

        return insns, count

    def build_tests_builtin(insn, ofp):
        build_builtins_prototype_t = \
'''
%(w)s {
%(i)s
}

'''

        insns = []
        count = 0
        args = insn["args"]
        params = [map(lambda m: m.strip("*"), args)]

        if "param" in insn.keys():
            if not insn["name"].endswith("_mask"):
                insn["param"] = map(lambda m: re.sub(",[ ]*mask", "", m), insn["param"])
            insn["param"] = insn["param"].replace(".", "_")
            params = map(lambda m: m.split(","), insn["param"])

        for param in params:
            if insn["insn"][0] == "bitrevi" or insn["insn"][0] == "kslliw" or\
               insn["insn"][0] == "kslli8" or insn["insn"][0] == "kslli16" or\
               insn["insn"][0] == "slli8" or insn["insn"][0] == "slli16" or insn["insn"][0] == "srai.u" or\
               insn["insn"][0] == "srai8" or insn["insn"][0] == "srai8.u" or insn["insn"][0] == "srai16" or\
               insn["insn"][0] == "srai16.u" or insn["insn"][0] == "srli8" or insn["insn"][0] == "srli8.u" or\
               insn["insn"][0] == "srli16" or insn["insn"][0] == "srli16.u" or insn["insn"][0] == "wexti" or\
               insn["insn"][0] == "sclip8" or insn["insn"][0] == "sclip16" or insn["insn"][0] == "sclip32" or\
               insn["insn"][0] == "uclip8" or insn["insn"][0] == "uclip16" or insn["insn"][0] == "uclip32" or\
               insn["insn"][0] == "kslli32" or insn["insn"][0] == "slli32" or insn["insn"][0] == "srai32" or\
               insn["insn"][0] == "srli32" or insn["insn"][0] == "srai32.u" or insn["insn"][0] == "srli32.u" or\
               insn["insn"][0] == "sraiw.u":
                param = ['a', '1']
        if insn["insn"][0] == "insb":
            param = ['t', 'a', '1']
        prototype = {
            "w"     : "%s" % (insn["capi_name"].replace("__rv__", "test_")),
            "i"     : "    return %s (%s);" % ("__rv__" + insn["name"], ", ".join(param)),
        }
        ofp.write(build_builtins_prototype_t % prototype)
        insns += insn["insn"]
        count += 1

        return insns, count



    def build_tests_insns(insns, outdir, callback):
        classg = sorted(insns, key=itemgetter("available"))
        classg = groupby(classg, itemgetter("available"))

        for available, group in classg:
            opt = "\"-march=rv*p*\""
            if available == "64":
                opt = "\"-march=rv64*p*\""
            if available == "32":
                opt = "\"-march=rv32*p*\""

            for insn in group:
                if insn["expandmode"] == "ulong" and insn.get("both signed and unsigned") == "u" \
                    and not (insn["nopr32"] and insn["nopr64"]):
                    continue
                if insn["expandmode"] in ["32","64"] \
                    and insn["nopr"]:
                    continue
                name = insn["name"]
                if insn.get("redundancyCabi"): name += "i"
                if insn["expandmode"] in ["32","64"]: name += "_" + insn["expandmode"]
                os.makedirs(outdir,exist_ok=True)
                with open(os.path.join(outdir, name + ".c"), "w") as ofp:
                    if insn["insn"][0] == "add64" or insn["insn"][0] == "sub64":
                        ofp.write(build_builtins_head_t % { "opt" : "\"-march=rv32*p*\""})
                    else:
                        ofp.write(build_builtins_head_t % { "opt" : opt})
                    scans = []
                    count = 0

                    i, c = callback(insn, ofp)
                    scans += i
                    count += c

                    scans = list(set(scans))
                    if len(scans) != count and len(scans) == 1:
                        scans[0] += "@%d" % count
                    for insn in scans:
                        pattern = "/* { dg-final { scan-assembler \"\\t%s\" } }*/\n" % insn
                        if "@" in insn:
                            pattern = "/* { dg-final { scan-assembler-times \"\\t%s\" %s } }*/\n" % tuple(insn.split("@"))
                        ofp.write(pattern)
                    ofp.write(build_builtins_tail_t)

    insnsZpn = filter(lambda m: "Zpsf" not in m.keys(), insns)
    build_tests_insns(insnsZpn, outdir, build_tests_builtin)
    insnsZpsf = filter(lambda m: "Zpsf" in m.keys(), insns)
    build_tests_insns(insnsZpsf, os.path.join(outdir, "zpsfoperand"), build_tests_builtin)

build_doc_config = r"""
# -*- coding: utf-8 -*-

project = u'DSP Intrinsic Manual'
author = u'CompilerTeam'
release = u'1.2'

master_doc = 'index'

latex_elements = {

    'preamble': r'''
\usepackage{draftwatermark}
\SetWatermarkText{XuanTie}
''',
}

"""
build_doc_index = '''
riscv dsp
=========
..  toctree::
    :maxdepth: 3

    %(names)s
'''
def build_doc(insns, outdir = "riscv-dsp-doc"):
    doc_builtins_index = '''
Reference
=========
..  toctree::

    %(names)s
'''
    doc_builtins_prolog = '''
**%(class)s**
====%(len_of_class)s

.. highlight:: c

'''
    doc_builtins_subclass_prolog = '''
%(subclass)s
%(len_of_sclass)s
'''
    doc_builtins_prototype_prolog = '''
%(doc)s
%(len_of_doc)s

**Instruction**:
%(instr)s
'''
    doc_builtins_prototype = '''
**Prototypes**:

%(prototype)s

**Operation**:

.. code-block:: c

%(operation)s

'''

    build_doc_intro = '''
Introduction
============

Digital Signal Processing (DSP), has emerged as an important technology for modern electronic
systems. A wide range of modern applications employ DSP algorithms to solve problems in their
particular domains, including sensor fusion, servo motor control, audio decode/encode, speech
synthesis and coding, MPEG4 decode, medical imaging, computer vision, embedded control,
robotics, human interface, etc.
The proposed P instruction set extension increases the DSP algorithm processing capabilities of the
RISC-V CPU IP products. With the addition of the RISC-V P instruction set extension, the RISC-V CPUs
can now run these various DSP applications with lower power and higher performance.

This article will show the defined intrinsic interfaces for reference and use.
'''

    version_doc = '''
**About Version**
-----------------

The Intrinsic functions in this article is based on *P-ext-propasl (version 0.9)* and should be used
with the riscv complier built by **XuanTie** which internal version is V1.10.3 or V2.0.0.
'''

    type_doc_def = '''
**Type Definitions**
--------------------

The following defined types may be used in the intrinsic definitions.

int8_t:
 An 8-bit wide type and treated as a signed number.

uint8_t:
 An 8-bit wide type and treated as a unsigned number.

int16_t:
 A 16-bit wide type and treated as a signed number.

uint16_t:
 A 16-bit wide type and treated as a unsigned number.

int32_t:
 A 32-bit wide type and treated as a signed number.

uint32_t:
 A 32-bit wide type and treated as a unsigned number.

int64_t:
 A 64-bit wide type and treated as a signed number.

uint64_t:
 A 64-bit wide type and treated as a unsigned number.

int8x4_t:
 A 32-bit wide type and consisting of 4 int8_t.

uint8x4_t:
 A 32-bit wide type and consisting of 4 uint8_t.

int16x2_t:
 A 32-bit wide type and consisting of 2 int16_t.

uint16x2_t:
 A 32-bit wide type and consisting of 2 uint16_t.

int8x8_t:
 A 32-bit wide type and consisting of 8 int8_t.

uint8x8_t:
 A 32-bit wide type and consisting of 8 uint8_t.

int16x4_t:
 A 64-bit wide type and consisting of 4 int16_t.

uint16x4_t:
 A 64-bit wide type and consisting of 4 uint16_t.

int32x2_t:
 A 64-bit wide type and consisting of 2 int32_t.

uint32x2_t:
 A 64-bit wide type and consisting of 2 uint32_t.

intXLEN_t:
 The actual type is determined by the architecture configuration before compilation.
  * rv32: This type will be treat as int32_t.
  * rv64: This type will be treat as int64_t.

uintXLEN_t:
 The actual type is determined by the architecture configuration before compilation.
  * rv32: This type will be treat as uint32_t.
  * rv64: This type will be treat as uint64_t.

'''

    build_doc_shorthand = '''
**Shorthand Definitions**
-------------------------

The functions and phrases blow may be found at the "**operation**" part of the interfaces. And these definitaions
is helpful to explian how the operation work.


RV32:*...*:
 The code will be reach and available at RV32.

RV64:*...*:
 The code will be reach and available at RV64.

Abs(*x*):
 Calculate the absolute value of "x".

Concat(*x*, *y*):
 Concatenate "x" and "y" form a value. For the resutlt bits, x is the high bits and y is the low bits.

Round(*x*, *n*):
 Indicate "rounding", shift "x" right by "n" bits and round the result according to the last bit moved out.

.. code-block:: c

 Round(x,n) {
   if (n == 0)
     return x;
   x >> n-1;
   res = x + 1;
   res = res >> 1;
   return res;
 }

Sat_s(*x*, *n*):
 Saturate to the range of [-2\ :sup:`n-1`\ , 2\ :sup:`n-1`\ -1], if saturation happens, set PSW.OV.

Sat_u(*x*, *n*):
 Saturate to the range of [0, 2\ :sup:`n`\ -1], if saturation happens, set PSW.OV.

'''

    example_doc = '''
**Simple steps to ues**
-----------------------

 1. Use *'riscv-dsp.h'* to include the intrinsic definitions.\n
    i.e.:

    .. code-block:: c

     #include <riscv-dsp.h>

 #. Write C code as wished.\n
    e.g.:

    .. code-block:: c

     #include <riscv-dsp.h>

     int main () {
         int8x8_t a = {-4,-3,-2,-1,1,2,3,4};
         int8x8_t b = {-40,-30,-20,-10,10,20,30,40};
         int8x8_t result = __rv__v_sadd8 (a, b);

         printf("reuslt is : {%d, %d, %d, %d, %d, %d, %d, %d}\\n",
                 result[0], result[1], result[2], result[3],
                 result[4], result[5], result[6], result[7]);
     }

 #. Compile the file with the *'p'* or *'Zpsf'* extension included in the *'-march'* option.\n
    e.g.:

    .. code-block:: shell

     riscv64-unknown-linux-gnu-gcc test.c -march=rv64gcpxthead -mabi=lp64d

    .. Note::
     The 'rv64', 'rv32', 'p' and 'Zpsf' may have different intrinsics , pay attention to make a good distinction.

'''

    about_Zpsf_doc = '''
**About Zpsf**
--------------

The Zpsf extension is a subset of the P extension. And the Zpsf would be include in
baseline of P when RV64 was given but would be used independently when RV32.

Base on the above understanding, we specify *'-march'* with the following convention:
 * For RV32, 'p' and 'Zpsf' are independent of each other.\n
   For example, rv32gcp, rv32gcZpsf and rv32gcpZpsf are all legal.
 * For RV64, 'Zpsf' should not be present, but these intrinsics are available when 'p' was given.\n
   For example, rv64gcp is leagl, but rv64gcZpsf and rv64gcpZpsf are not legal.

All the Zpsf below:

.. list-table:: Zpsf isa
 :header-rows: 1
 :stub-columns: 1
 :widths: 1 5
 :align: center

 * - No.
   - Instruction
 * - 1
   - SMAL
 * - 2
   - ADD64
 * - 3
   - RADD64
 * - 4
   - URADD64
 * - 5
   - KADD64
 * - 6
   - UKADD64
 * - 7
   - SUB64
 * - 8
   - RSUB64
 * - 9
   - URSUB64
 * - 10
   - KSUB64
 * - 11
   - UKSUB64
 * - 12
   - SMAR64
 * - 13
   - SMSR64
 * - 14
   - UMAR64
 * - 15
   - UMSR64
 * - 16
   - KMAR64
 * - 17
   - KMSR64
 * - 18
   - UKMAR64
 * - 19
   - UKMSR64
 * - 20
   - SMALBB
 * - 21
   - SMALBT
 * - 22
   - SMALTT
 * - 23
   - SMALDA
 * - 24
   - SMALXDA
 * - 25
   - SMALDS
 * - 26
   - SMALDRS
 * - 27
   - SMALXDS
 * - 28
   - SMSLDA
 * - 29
   - SMSLXDA
 * - 30
   - MULR64
 * - 31
   - MULSR64
 * - 32
   - UMUL8
 * - 33
   - UMULX8
 * - 34
   - UMUL16
 * - 35
   - UMULX16
 * - 36
   - SMUL8
 * - 37
   - SMULX8
 * - 38
   - SMUL16
 * - 39
   - SMULX16
 * - 40
   - WEXT
 * - 41
   - *CLROV* [1]_
 * - 42
   - *RDOV*

.. [1] In special, the clrov/rdov are both available for 'p' and 'Zpsf'.

'''

    def build_doc_builtins_patten(name, patten, outdir = outdir, index = ""):
        os.makedirs(outdir, exist_ok=True)

        npatten = ("." in name and name) or ("%s.rst" % name)

        with open(os.path.join(outdir, npatten), "w") as ofp:
            ofp.write(patten)
        if not index:
            index = name
        return index

    def build_doc_builtins(insns, outdir):
        outdir = os.path.join(outdir, 'builtins')
        os.makedirs(outdir, exist_ok=True)

        insn_sorted = []
        for classord in ["SIMD Data Processing",
                         "Partial-SIMD Data Processing",
                         "64-bit Profile",
                         "Non-SIMD",
                         "RV64 Only"]:
            thisclassinsns = list(filter(lambda m:m["class"] == classord, insns))
            insn_sorted += thisclassinsns
        insnsg = groupby(insn_sorted,itemgetter('class'))
        names = []
        idocs = []

        for classname, classgroup in insnsg:
            classname = classname.split("@")[-1]
            iname = classname.replace(" ", "-")
            names.append(iname)
            with open(os.path.join(outdir, iname + ".rst"), "w") as ofp:
                ofp.write(doc_builtins_prolog % { "class" : classname, "len_of_class" : "="*len(classname)})
                classgroup = sorted(classgroup, key=itemgetter('subclass'))
                classgroup = groupby(classgroup,itemgetter('subclass'))

                for sname_index, sgroup in classgroup:
                    sname = isubc_tables[sname_index]
                    ofp.write(doc_builtins_subclass_prolog % { "subclass" : sname, "len_of_sclass" : "-"*len(sname)})
                    sgroup = sorted(sgroup, key=lambda m: m["insn"][0])
                    sgroup = groupby(sgroup, lambda m: m["insn"][0])

                    for name, igroup in sgroup:
                        igroup = list(igroup)
                        insn = igroup[0]

                        if insn.get("redundancyCabi"): continue
                        doc = insn.get("idoc")
                        if not doc:
                            continue
                        assert(doc not in idocs or eval(doc))
                        idocs.append(doc)
                        instr = doc.split()[0].strip()
                        doc = " ".join(doc.split()[1:]).strip("()")
                        if insn.get('Zpsf') == "Zpsf or p": instr += " (Zpsf or p)"
                        elif insn.get('Zpsf'): instr += " (Zpsf)"
                        ofp.write(doc_builtins_prototype_prolog % { "doc": doc, "len_of_doc": ">"*len(doc),
                                                                    "instr": instr.lower()})

                        prototype = {
                            "patten" : doc_builtins_prototype,
                            "prototype" : "",
                            "operation": "operation" in insn.keys() and insn["operation"]
                        }
                        igroup = sorted(igroup, key=itemgetter('available'), reverse=True)
                        igroup = groupby(igroup, itemgetter('available'))

                        for available, ag in igroup:
                            funcs = ""
                            availabe = { "both" : "RV32 & RV64\n\n", "32" : "RV32\n\n", "64": "RV64\n\n"}[available]
                            for i in ag:
                                if i.get("nopr"):
                                    continue
                                funcs += " | %s\n" % (i["capi_name"])
                            if funcs:
                                prototype["prototype"] += "- " + availabe + funcs + "\n"
                        if prototype["prototype"]:
                            ofp.write(prototype["patten"] % prototype)

        return build_doc_builtins_patten("index",
                                         doc_builtins_index % { "names" : "\n    ".join(names) },
                                         outdir,
                                         'builtins/index')

    indexs = [
        build_doc_builtins_patten("intro", build_doc_intro + version_doc + type_doc_def + build_doc_shorthand + example_doc + about_Zpsf_doc),
        build_doc_builtins(insns, outdir),
    ]
    build_doc_builtins_patten("index", build_doc_index % { "names" : "\n    ".join(indexs) })
    build_doc_builtins_patten("conf.py", build_doc_config)


def main():
    os.chdir(scritp_dirname)
    insns = build_builtins()
    build_tests(insns)
    build_doc(insns)

if __name__ == "__main__":
    main()
