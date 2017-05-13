/* Check for false positives */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

/* returns: bitset of quadratic residues */
static int64_t
qresall (int N)
{
  int64_t qres = 0;
  for (int i = 0; i < N; ++i)
    qres |= 1<<(i*i % N);
  return qres;
}

/* Ns: zero terminated list of moduli */
/* returns: true probable square, false not square */
static bool
qrestest (int *Ns, int x)
{
  for (; *Ns; ++Ns)
    if (! (qresall (*Ns) & (1<<(x % *Ns))))
      return 0;
  return 1;
}

/* returns: the number of non-squares in the 
   range `fst <= i <= lst' with moduli `Ns' */
static int
coverage (int fst, int lst, int *Ns)
{
  int c = 0;
  for (int i = fst; i <= lst; ++i)
    if (! qrestest (Ns, i))
      ++c;
  return c;
}

static int
gcd2 (int a, int b)
{
  assert (a);
  assert (b);
    
  while (a != b)
    if (a > b)
      a -= b;
    else
      b -= a;

  return a;
}

static int
gcd (int *Ns)
{
  assert (Ns[0]);
  int g = Ns[0];
  for (int i = 1; Ns[i]; ++i)
    g = gcd2 (g, Ns[i]);
  return g;
}

static int
lcm (int *Ns)
{
  int m = 1;
  int *p = Ns;
  while (*p)
    m *= *p++;
  return m / gcd (Ns);
}

/* returns: percentage of coverage */
static float
coverageper (int *Ns)
{
  return (float) coverage (1, lcm (Ns), Ns) / lcm (Ns) * 100;
}

static void
printNs (int *Ns)
{
  for (int i = 0; Ns[i]; ++i)
    printf ("%d ", Ns[i]);
  printf ("\n");
}


int
main (void)
{
  int *Ns = (int[]){3,5,9,64,63,65,11,0};
  printf ("%d\n", lcm(Ns));
  printf ("%f\n", coverageper (Ns));
  // printf ("%f\n", coverage (1, INT_MAX, (int []) {64,63,65,11,0}));
}
