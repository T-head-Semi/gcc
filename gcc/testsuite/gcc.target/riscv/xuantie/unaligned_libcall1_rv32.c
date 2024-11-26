/* { dg-do compile } */
/* { dg-options "-march=rv32imafdc -mabi=ilp32d -mstrict-align -munaligned-libcall -Os" } */

struct __attribute__((packed)) u4 { int data; };
struct __attribute__((packed)) u8 { long long data; };

int
test_unaligned_access_uread4 (void *ptr)
{
  return ((struct u4 *) ptr)->data;
}

void
test_unaligned_access_uwrite4 (int data, void *ptr)
{
  ((struct u4 *) ptr)->data = data;
}

long long
test_unaligned_access_uread8 (void *ptr)
{
  return ((struct u8 *) ptr)->data;
}

void
test_unaligned_access_uwrite8 (long long data, void *ptr)
{
  ((struct u8 *) ptr)->data = data;
}

/* { dg-final { scan-assembler-times "call\t__thead_uread4" 3 } } */
/* { dg-final { scan-assembler-times "call\t__thead_uwrite4" 3 } } */
/* { dg-final { scan-assembler-not "call\t__thead_uread8" } } */
/* { dg-final { scan-assembler-not "call\t__thead_uwrite8" } } */
