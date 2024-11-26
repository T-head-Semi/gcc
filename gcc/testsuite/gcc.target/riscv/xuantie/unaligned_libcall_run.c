/* { dg-do run } */
/* { dg-options "-mstrict-align -munaligned-libcall -Os" } */ 

#include <stdio.h>

struct __attribute__((packed)) u4 { int data; };
struct __attribute__((packed)) u8 { long long data; };

int __attribute__ ((noinline))
unaligned_access_uread4 (void *ptr)
{
  return ((struct u4 *) ptr)->data;
}

void __attribute__ ((noinline))
unaligned_access_uwrite4 (int data, void *ptr)
{
  ((struct u4 *) ptr)->data = data;
}

long long __attribute__ ((noinline))
unaligned_access_uread8 (void *ptr)
{
  return ((struct u8 *) ptr)->data;
}

void __attribute__ ((noinline))
unaligned_access_uwrite8 (long long data, void *ptr)
{
  ((struct u8 *) ptr)->data = data;
}

int main()
{
  int date = 0;
  int *ptr = &date;
  unaligned_access_uwrite4 (0x7fff, ptr);
  printf ("%x", unaligned_access_uread4 (ptr));

  long long long_date = 0;
  long long *long_ptr = &long_date;
  unaligned_access_uwrite8 (0x7fffffff, long_ptr);
  printf ("%llx", unaligned_access_uread8 (long_ptr));
  printf ("\n");
  return 0;
}

/* { dg-output "7fff7fffffff\r\n" } */
