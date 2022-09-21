/* RISC-V-specific code for C family languages.
   Copyright (C) 2011-2020 Free Software Foundation, Inc.
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

#define IN_TARGET_CODE 1

#define INCLUDE_STRING
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "c-family/c-common.h"
#include "cpplib.h"
#include "riscv-subset.h"

#define builtin_define(TXT) cpp_define (pfile, TXT)

/* Implement TARGET_CPU_CPP_BUILTINS.  */

void
riscv_cpu_cpp_builtins (cpp_reader *pfile)
{
  builtin_define ("__riscv");

  if (TARGET_XTHEAD)
    builtin_define ("__riscv_xthead");
#ifdef THEAD_VERSION_NUMBER
#define STR(a) #a
#define XSTR(a) STR(a)
  cpp_define_formatted (pfile, "__THEAD_VERSION__=\"%s\"", XSTR (THEAD_VERSION_NUMBER));
#else
  builtin_define ("__THEAD_VERSION__=\"undefined\"");
#endif

#ifndef TARGET_LINUX
  builtin_define_with_int_value ("__THEAD_SIZEOF_PTHREAD_MUTEXATTR_T", 20);
  builtin_define_with_int_value ("__THEAD_SIZEOF_PTHREAD_COND_T", 64);
  builtin_define_with_int_value ("__THEAD_SIZEOF_PTHREAD_CONDATTR_T", 24);
  builtin_define_with_int_value ("__THEAD_SIZEOF_PTHREAD_ATTR_T", 64);
  builtin_define_with_int_value ("__THEAD_SIZEOF_PTHREAD_MUTEX_T", 40);
#endif

  if (TARGET_VECTOR)
    builtin_define ("__riscv_vector");

  if (TARGET_MATRIX)
    builtin_define ("__riscv_matrix");

  if (TARGET_RVC)
    builtin_define ("__riscv_compressed");

  if (TARGET_RVE)
    builtin_define ("__riscv_32e");

  if (TARGET_ATOMIC)
    builtin_define ("__riscv_atomic");

  if (TARGET_MUL)
    builtin_define ("__riscv_mul");
  if (TARGET_DIV)
    builtin_define ("__riscv_div");
  if (TARGET_DIV && TARGET_MUL)
    builtin_define ("__riscv_muldiv");

  builtin_define_with_int_value ("__riscv_xlen", UNITS_PER_WORD * 8);
  if (TARGET_HARD_FLOAT)
    builtin_define_with_int_value ("__riscv_flen", UNITS_PER_FP_REG * 8);

  if (TARGET_HARD_FLOAT && TARGET_FDIV)
    {
      builtin_define ("__riscv_fdiv");
      builtin_define ("__riscv_fsqrt");
    }

  if (TARGET_XTHEAD_ZPN)
    {
      builtin_define ("__riscv_dsp");
    }

  switch (riscv_abi)
    {
    case ABI_ILP32E:
      builtin_define ("__riscv_abi_rve");
      gcc_fallthrough ();

    case ABI_ILP32:
    case ABI_LP64:
    case ABI_LP64V:
      builtin_define ("__riscv_float_abi_soft");
      if (riscv_abi == ABI_LP64V)
	builtin_define ("__riscv_vector_abi_v");
      break;

    case ABI_ILP32F:
    case ABI_LP64F:
      builtin_define ("__riscv_float_abi_single");
      break;

    case ABI_ILP32D:
    case ABI_LP64D:
    case ABI_LP64DV:
      builtin_define ("__riscv_float_abi_double");
      if (riscv_abi == ABI_LP64DV)
	builtin_define ("__riscv_vector_abi_v");
      break;
    }

  switch (riscv_cmodel)
    {
    case CM_MEDLOW:
      builtin_define ("__riscv_cmodel_medlow");
      break;

    case CM_PIC:
      /* __riscv_cmodel_pic is deprecated, and will removed in next GCC release.
	 see https://github.com/riscv/riscv-c-api-doc/pull/11  */
      builtin_define ("__riscv_cmodel_pic");
      /* FALLTHROUGH. */

    case CM_MEDANY:
      builtin_define ("__riscv_cmodel_medany");
      break;

    }

  const riscv_subset_list *subset_list = riscv_current_subset_list ();
  size_t max_ext_len = 0;

  /* Figure out the max length of extension name for reserving buffer.   */
  for (const riscv_subset_t *subset = subset_list->begin ();
       subset != subset_list->end ();
       subset = subset->next)
    max_ext_len = MAX (max_ext_len, subset->name.length ());

  char *buf = (char *)alloca (max_ext_len + 10 /* For __riscv_ and '\0'.  */);

  for (const riscv_subset_t *subset = subset_list->begin ();
       subset != subset_list->end ();
       subset = subset->next)
    {
      int version_value = (subset->major_version * 1000000)
			   + (subset->minor_version * 1000);

      sprintf (buf, "__riscv_%s", subset->name.c_str ());
      builtin_define_with_int_value (buf, version_value);
    }
}
