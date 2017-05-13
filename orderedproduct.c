#include <stdio.h>
#include <stdbool.h>

typedef unsigned int uint;

bool
isproduct (uint p, uint x)
{
  if (x == 0)
    return p == 0;
  
  uint a = (x-1)/2 + 1;
  uint b = x - a;

  do {
    if (a*b == p)
      return true;
    ++a;
    --b;
  } while (b >= 0);
  
  return false;
}

int
list()
{
  uint n = 6;
  uint x, a, b;

  for (x = 0; x <= n; ++x) {
    uint aPb = n*n - (n*n - n*x);
    do {
      if (isproduct (aPb, x))
        printf ("%d\n", n*n - n*x + aPb);
      --aPb;
    } while (aPb >= 0);
  }
}

int
main ()
{
  for (int i = 0; i; --i)
    printf ("%d\n");
  list ();
}
