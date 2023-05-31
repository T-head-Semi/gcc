/* { dg-do compile } */
/* { dg-options "-march=rv64gc_xtheadc -mabi=lp64d -O3" } */

class a {
public:
  void b();
};

template <int c> long d(unsigned long h) { return h << c; }

using e = a;
bool f;

void g(long h, e &i) {
  bool j;
  f = int(h);
  if (f)
    return;
  unsigned long k = h;
  j = k && k & 1;
  if (j)
    return;
  long l = d<2>(h);
  g(0, i);
  if (l)
    i.b();
}

/* { dg-final { scan-assembler-times "sext.w" 1 } } */
