/* { dg-do compile } */
/* { dg-options "-march=rv64gc_xtheadc -mabi=lp64d -O3 -Wno-shift-count-overflow" } */


class a {
  public:
    int d();
};

long b;
a c;

void e()
{
  b >> 57784725 || c.d();
}

/* { dg-final { scan-assembler "extu\t\[a-z\]\[0-9\]+,\[a-z\]\[0-9\]+,63,21" } } */
