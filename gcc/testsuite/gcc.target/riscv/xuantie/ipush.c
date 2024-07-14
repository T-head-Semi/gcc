/* { dg-do compile } */
/* { dg-options "-march=rv32gcxtheade -mabi=ilp32d -O0" } */

extern void f(void);

__attribute__((interrupt))
void func(void)
{
  f ();
}

void __attribute__ ((interrupt))
func_leaf1 (void)
{
  register int interrupt_t1 asm ("t1");
  register int interrupt_t2 asm ("t2");
  register int interrupt_a0 asm ("a0");
  register int interrupt_a1 asm ("a1");
  register int interrupt_a2 asm ("a2");
  register int interrupt_a3 asm ("a3");
  register int interrupt_a4 asm ("a4");
  register int interrupt_a5 asm ("a5");
  register int interrupt_a6 asm ("a6");
  register int interrupt_a7 asm ("a7");
  register int interrupt_t3 asm ("t3");
  register int interrupt_t4 asm ("t4");

  interrupt_t1++;
  interrupt_t2++;
  interrupt_a0++;
  interrupt_a1++;
  interrupt_a2++;
  interrupt_a3++;
  interrupt_a4++;
  interrupt_a5++;
  interrupt_a6++;
  interrupt_a7++;
  interrupt_t3++;
  interrupt_t4++;
}

void __attribute__ ((interrupt))
func_leaf2 (void)
{
  register int interrupt_t1 asm ("t1");
  register int interrupt_t2 asm ("t2");
  register int interrupt_a0 asm ("a0");
  register int interrupt_a1 asm ("a1");
  register int interrupt_a2 asm ("a2");
  register int interrupt_a3 asm ("a3");
  register int interrupt_a4 asm ("a4");
  register int interrupt_a5 asm ("a5");
  register int interrupt_a6 asm ("a6");
  register int interrupt_a7 asm ("a7");
  register int interrupt_t3 asm ("t3");

  interrupt_t1++;
  interrupt_t2++;
  interrupt_a0++;
  interrupt_a1++;
  interrupt_a2++;
  interrupt_a3++;
  interrupt_a4++;
  interrupt_a5++;
  interrupt_a6++;
  interrupt_a7++;
  interrupt_t3++;
}

/* { dg-final { scan-assembler-times "ipush\n" 2 } }*/
/* { dg-final { scan-assembler-times "ipop\n" 2 } }*/

/* { dg-final { scan-assembler-times "csrr\t" 3 } } */
/* { dg-final { scan-assembler-times "csrw\t" 3 } } */
/* { dg-final { scan-assembler-times "csrsi\t" 1 } } */

/* { dg-final { scan-assembler-times "mret\n" 1 } }*/
