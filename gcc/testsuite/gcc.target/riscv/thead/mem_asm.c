/* { dg-do compile } */
/* { dg-options "-march=rv64gc_xtheadc -mabi=lp64d -O2" } */

void foo (void *p, unsigned long off, unsigned long val)
{
  unsigned long *tmp = (unsigned long*)(p + off);
  asm volatile ("sd	%1,%0"
                : "=m"(*tmp)
                : "r"(val));
}

/* { dg-final { scan-assembler-not "sd\t\[a-z\]\[0-9\]+,\[a-z\]\[0-9\]+,\[a-z\]\[0-9\]+,0" } }*/
