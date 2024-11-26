/* { dg-do run { target { riscv_rvv } } }
/* { dg-skip-if "test rvv32" { *-*-* } { "*" } { "-march=rv32*v*" } } */
/* { dg-options " -O2 " } */

#include <stdio.h>
#include <riscv_vector.h>

const int64_t sample_int64[16] = {
	 0x1fffffffffffff, -8394, 50602,
	   2065413, 26383, -77868,
	  -12408, -171873, -92074,
	   25666, 83340, 334081,
	   56700, 728277, 5169449,
	   18251};
int64_t result_int64[16] = {};

void vmvvxi64m1_s_nomask_builtin_test (int64_t *x, int64_t s, size_t vl)
{
    printf("testcase""vmv_v_x_i64m1");
    printf("\n");
    vint64m1_t vx;
    vx = vle64_v_i64m1 (x, vl);
    vx = vmv_v_x_i64m1(s, vl);
    vse64_v_i64m1 (x, vx, vl);
}
void rvvmvvxi64m1_s_nomask_builtin_test_arg()
{
    printf("start test: %s\n", __FUNCTION__);
    int bytes = sizeof(sample_int64 );
    int64_t a;
    union comp
    {
        int64_t origin;
        uint64_t memory;
    };
    union comp present;
    present.memory = 0;
    int len = bytes/sizeof(a);
    size_t vl = vsetvl_e64m1 (len);
    vmvvxi64m1_s_nomask_builtin_test (result_int64, (int64_t)0x1fffffffffffff, vl);
    present.origin = result_int64 [1];
    printf("%llx, \n", present.memory);
    printf("end test.\n");
}
int main()
{
  vwrite_csr(RVV_VXRM, 2);
  rvvmvvxi64m1_s_nomask_builtin_test_arg();
  return 0;
}
/* { dg-output "testcasevmv_v_x_i64m1\r\n" } */
/* { dg-output "1fffffffffffff, \r\n" } */
