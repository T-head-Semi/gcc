/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xtheadc -mabi=ilp32d -O2" } */

void foo (void *p, unsigned long off, unsigned long val)
{
  unsigned long *tmp = (unsigned long*)(p + off);
  asm volatile ("sw	%1,%0"
                : "=m"(*tmp)
                : "r"(val));
}

/* { dg-final { scan-assembler-not "sw\t\[a-z\]\[0-9\]+,\[a-z\]\[0-9\]+,\[a-z\]\[0-9\]+,0" } }*/
