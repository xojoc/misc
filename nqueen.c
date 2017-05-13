#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static int n;
static int ns = 0;
static int i;

static void
printq (int *q)
{
  int r,c;
  for (r = 0; r < n; ++r) {
    for (c = 0; c < n; ++c) {
      if (q[r] == c)
        printf ("q ");
      else
        printf ("0 ");
    }
    printf ("\n");
  }
  printf ("\n");
}
static void
printacc (int *q, int last)
{
  static int c = 0, cnt = 0;
  if (last) {
    printf ("%4d ", cnt);
    ++c;
    for (; c < n; ++c)
      printf ("   0 ");
    printf ("\n");
  } else {
    if (q[0] == c)
      ++cnt;
    else {
      printf ("%4d ", cnt);
      ++c;
      while (c < q[0]) {
	printf ("   0 ");
	++c;
      }
      cnt = 1;
    }
  }
}
static void
printseq (int *q)
{
#define x(i) ((q[i]+1)+(i)*n)
  for (int i = 1; i < n; ++i)
    printf ("%2d { %2d } ", x(i-1), x(i)-x(i-1));
  
  printf ("%2d\n", x(i-1));
#undef x
}

static void
s (int *q)
{
  if (i == n) {
    ++ns;
    /* printseq (q); */
    /* printacc (q, 0); */
    printq (q);
    return;
  }
  int c;
  for (c = 0; c < n; ++c) {
    bool ok = 1;
    for (int j = 0; j < i; ++j) {
      if ((q[j] == c) ||
	  (i - j) == abs (c - q[j]))
	{
	  ok = 0;
	  break;
	}
    }
    if (ok) {
      q[i] = c;
      ++i;
      s (q);
      --i;
    }
  }
}

int 
main (int argc, char *argv[])
{
  ((void)argc);
  ((void)printq);
  ((void)printseq);
  ((void)printacc);
  n = 6;
  if (argv[1])
    n = strtol (argv[1], NULL, 10);
  int *q = malloc (n * sizeof (*q));
  s (q);
  /* printacc (q, 1); */
  printf ("%d\n", ns);
  return 0;
}
