#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* See: https://en.wikipedia.org/wiki/Binary_exponentiation */

double
naiveexpt (double n, int e)
{
  double r = 1;
  double a = fabs (e);

  while (a--)
    r *= n;

  return e >= 0 ? r : 1/r;
}

double
binexpt (double n, int e)
{
  double _h (double n, unsigned int e) {
    if (e == 0)
      return 1;

    return e%2 == 0 ? _h (n*n, e/2) : n * _h (n*n, (e-1)/2);
  }
  
  if (e < 0)
    return 1 / _h (n, -e);
  else
    return _h (n, e);
}

unsigned int
mylog2 (uint32_t n)
{
  unsigned int r;
  unsigned int shift;
  
  r = (n > 0xFFFF) << 4;
  n >>= r;
  shift = (n > 0xFF) << 3;
  n >>= shift;
  r |= shift;
  shift = (n > 0xF) << 2;
  n >>= shift;
  r |= shift;
  shift = (n > 0x3) << 1;
  n >>= shift;
  r |= shift;
  r |= (n >> 1);

  return r;
}

int
trailing0s (unsigned int n)
{
  int c;
  if (n) {
    n = (n ^ (n-1)) >> 1;
    for (c = 0; n; ++c)
      n >>= 1;
  } else
    c = 8 * sizeof (n);

  return c;
}

/* with k=1 this is equivalent to binexp */
double
twokaryexpt (double n, int32_t e)
{
  int k = 3;
  double y = 1;
  int l = mylog2 (e) / k + 1;
  int i;

  int re = 0;
  
  int e_i ()
  {
    return (e >> (i*k)) & ((1<<k)-1);
  }

  void f (int *u, int *s)
  {
    if (e_i () == 0) {
      *s = k;
      *u = 0;
    } else {
      *s = trailing0s (e_i());
      *u = e_i() >> *s;
    }
  }

  for (i = l-1; i >= 0; --i) {
    int u, s;

    f (&u, &s);

    for (int j = 0; j < k - s; ++j) {
      re *= 2;
      y *= y;
      printf ("re: %d\n", re);
    }
    
    y *= naiveexpt (n, u);
    re += u;
    printf ("re: %d\n", re);

    for (int j = 0; j < s; ++j) {
      re *= 2;
      y *= y;
      printf ("re: %d\n", re);
    }
  }

  return y;
}

int
main (void)
{
/*   printf ("%f\n", twokaryexpt (2.3, 2)); */
/*   printf ("%f\n", twokaryexpt (2.3, 3)); */
/*   printf ("%f\n", twokaryexpt (2.3, 4)); */
/*   printf ("%f\n", twokaryexpt (2.3, 8)); */
/*   printf ("%f\n", twokaryexpt (2.3, 13)); */
  /* printf ("%f\n", twokaryexpt (2.3, -1)); */
/* printf ("%f\n", twokaryexpt (2.3, -5)); */
  printf ("%f\n", twokaryexpt (2, 398));
  
}
