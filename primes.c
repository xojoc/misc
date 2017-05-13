#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

// #define PRINT 0
#define USED(v) ((void)v)

typedef unsigned int uint;

static char *primes;

static void
setbit (char *a, uint n)
{
	a[n/8] |= 1 << (n%8);
}

static bool
getbit (char *a, uint n)
{
	return a[n/8] & (1 << (n%8));
}

/* It's wrong with N=1.000.000.000 */
static void
mairson (uint N)
{
  primes = calloc (N / 8 + 1, 1);
	if (! primes)
		assert (! "out of memory");
  uint p = 2;

  while (p * p <= N)
    {
      uint f = p;
      do
	{
	  uint c = p * f;
	  do
	    {
	      setbit (primes, c);
	      c *= p;
	    } while (c <= N);

	  ++f;
	  while (getbit (primes, f))
	    ++f;
	} while (p * f <= N);

      ++p;
      while (getbit (primes, p))
	++p;
    }
#ifdef PRINT
  for (p = 2; p < N; ++p)
    {
      if (!getbit (primes, p))
	printf ("%u\n", p);
    }
#endif
}
/* Algorithm 3.3. */
static void
dual (uint N)
{
	primes = calloc (N / 8 + 1, 1);
	uint f = 2;
	while (f * 2 <= N) {
		uint p = 2;
		int pOK = 1;
		while (pOK) {
			setbit (primes, p*f);
			uint t = p;
			++p;
			while (getbit (primes, p))
				++p;
			pOK = p * f <= N && (f % t != 0);
		}
		++f;
	}
}
/* static int */
/* isp ( */
static bool
isprime (uint n)
{ return ! getbit (primes, n); }
static void
partition_them (uint n)
{
  char *a = calloc (n+1, 1);
  for (uint i = 1; i <= n; ++i) {
    printf ("i: %u\n", i);
    while (getbit (a, i) && i <= n)
      ++i;
    if (i > n)
      break;
    printf ("after i: %u\n", i);
    for (uint j = i + 1; j <= n; ++j) {
      printf ("j: %u\n", j);
      while (getbit (a, j) && j <= n)
	++j;
      if (j > n)
	goto out;
      if (isprime (i + j)) {
	printf ("pair (%u, %u): %u\n", i, j, i + j);
	setbit (a, i);
	setbit (a, j);
	break;
      }
    }
  }
 out:;
}
static void
comb (uint n)
{
  uint c = 0;
  for (uint i = 1; i <= n; ++i)
    for (uint j = i; j <= n; ++j) {
      ++c;
      if (isprime (i + j))
	printf ("comb (%u, %u): %u\n", i, j, i + j);
    }
}
int
main ()
{
	uint N = 1000*1000*1000;
	//dual (N);
	mairson (N);
	for (uint i = 2; i <= N; ++i)
		if (isprime (i))
			printf ("%u\n", i);

	/* partition_them (8); */
	/* comb (100); */
}
