const struct riscv_tune_param thead_e9_tune_info = {
  {COSTS_N_INSNS (4), COSTS_N_INSNS (5)},	/* fp_add */
  {COSTS_N_INSNS (4), COSTS_N_INSNS (5)},	/* fp_mul */
  {COSTS_N_INSNS (20), COSTS_N_INSNS (20)},	/* fp_div */
  {COSTS_N_INSNS (4), COSTS_N_INSNS (4)},	/* int_mul */
  {COSTS_N_INSNS (6), COSTS_N_INSNS (6)},	/* int_div */
  1,						/* issue_rate */
  3,						/* branch_cost */
  5,						/* memory_cost */
  true,						/* slow_unaligned_access */

  NULL,						/* function_align */
  NULL,						/* jump_align */
  "4",						/* loop_align */
};

const struct riscv_tune_param thead_c906_tune_info = {
  {COSTS_N_INSNS (4), COSTS_N_INSNS (5)},	/* fp_add */
  {COSTS_N_INSNS (4), COSTS_N_INSNS (5)},	/* fp_mul */
  {COSTS_N_INSNS (20), COSTS_N_INSNS (20)},	/* fp_div */
  {COSTS_N_INSNS (4), COSTS_N_INSNS (4)},	/* int_mul */
  {COSTS_N_INSNS (6), COSTS_N_INSNS (6)},	/* int_div */
  1,						/* issue_rate */
  3,						/* branch_cost */
  5,						/* memory_cost */
  false,					/* slow_unaligned_access */

  "16",						/* function_align */
  "4",						/* jump_align */
  "8",						/* loop_align */
  COSTS_N_INSNS (1),				/* if_then_else */
  COSTS_N_INSNS (1),				/* zero/signed_extend */
  100,						/* ifcvt_unpred */
};

const struct riscv_tune_param thead_c910_tune_info = {
  {COSTS_N_INSNS (4), COSTS_N_INSNS (5)},	/* fp_add */
  {COSTS_N_INSNS (4), COSTS_N_INSNS (5)},	/* fp_mul */
  {COSTS_N_INSNS (20), COSTS_N_INSNS (20)},	/* fp_div */
  {COSTS_N_INSNS (4), COSTS_N_INSNS (4)},	/* int_mul */
  {COSTS_N_INSNS (6), COSTS_N_INSNS (6)},	/* int_div */
  1,						/* issue_rate */
  3,						/* branch_cost */
  5,						/* memory_cost */
  false,					/* slow_unaligned_access */

  "16",						/* function_align */
  "4",						/* jump_align */
  "8",						/* loop_align */
  COSTS_N_INSNS (1),				/* if_then_else */
  COSTS_N_INSNS (1),				/* zero/signed_extend */
  100,						/* ifcvt_unpred */
};
