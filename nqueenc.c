#include <stdio.h>

static int count, all;

static void
try (int ld, int cols, int rd)
{
  if (cols == all) {
    ++count;
    return;
  }

  int poss = ~(ld | cols | rd) & all;
  while (poss) {
    int bit = poss & -poss;
    poss -= bit;
    try ( (ld|bit)<<1, cols|bit, (rd|bit)>>1 );
  }
}
int
main (void)
{
  int n;
  all = 1;
  for (n = 1; n <= 31; ++n) {
    count = 0;
    try (0, 0, 0);
    printf ("%2d: %10d\n", n, count);
    all = (all<<1) + 1;
  }
  return 0;
}
