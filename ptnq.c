/* Polynomial Time N-Queen search algorithm. */
/* See: http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=4DC9292839FE7B1AFABA1EDB8183242C?doi=10.1.1.57.4685&rep=rep1&type=pdf */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>

typedef uint32_t qint;
typedef qint dint;
typedef unsigned char uchar;

static void *
pgalloc (size_t sz)
{
  void *p = mmap (NULL, sz, PROT_READ|PROT_WRITE,
                  MAP_PRIVATE | MAP_ANON,
                  -1, 0);
  assert (p != (void *)-1);
  return p;
}

static uchar *
bitnew (int n)
{
  return pgalloc (n/CHAR_BIT + 1);
}
static void
bitset (uchar *a, int i)
{
  a[i/CHAR_BIT] |= i%CHAR_BIT;
}
static void
bitunset (uchar *a, int i)
{
  a[i/CHAR_BIT] &= ~(i%CHAR_BIT);
}
static bool
bitget (uchar *a, int i)
{
  return a[i/CHAR_BIT] & (i%CHAR_BIT);
}

static qint
uniform (qint m)
{
  return rand ()%(m+1);
}
/* Knuth shuffles */
static void
permcols (qint *q, qint n)
{
  for (qint i = 0; i < n; ++i) {
    qint j = uniform (i);
    q[i] = q[j];
    q[j] = i;
  }
}

static qint
d2k (qint r, qint c, qint n)
{
  return r - c + (n-1);
}
static void
calccls (qint *q, qint *d1, qint *d2, qint n)
{
  for (qint i = 0; i < 2*n-1; ++i) {
    d1[i] = 0;
    d2[i] = 0;
  }
    
  for (qint i = 0; i < n; ++i) {
    ++d1[i+q[i]];
    ++d2[d2k(i,q[i],n)];
  }
}
static int
cntcls (qint *d1, qint *d2, qint n)
{
  qint c = 0;
  for (qint i = 0; i < 2*n-1; ++i) {
    c += d1[i] ? d1[i] - 1 : 0;
    c += d2[i] ? d2[i] - 1 : 0;
  }
  return c;
}

qint *
queenfind (qint n)
{
  size_t qsz = n*sizeof(qint);
  size_t dsz = (2*n-1)*sizeof(qint)*2;
  /* uchar *qr, *qc; */
  qint *q, *qr, *qc;
  qint *d1, *d2;
  /* qr = bitnew (qsz); */
  /* bc = bitnew (qsz); */
  q = pgalloc (qsz);
  /* qc = qr + qsz/sizeof(qint)/2; */
  d1 = pgalloc (dsz);
  d2 = d1 + dsz/sizeof(qint)/2;

  qint getr (qint j)
  {
    for (qint i = 0; i < n; ++i)
      if (q[i] == j)
        return i;
    abort ();
  }

  do {
    permcols (q, n);
    calccls (q, d1, d2, n);
    bool swaped;
    do {
      swaped = 0;
      for (qint i = 0; i < n-1; ++i) {
        for (qint j = i+1; j < n; ++j) {
          qint r = getr (j);
#define c1(i)(d1[i] ? d1[i]-1 : 0)
#define c2(i)(d2[i]+1)
#define dec1(i)(if (d1[i]) {--d1[i]})          
          /* if ((d1[i+q[i]] + d1[r+j] + d2[d2k(i,q[i],n)] + d2[d2k(r,j,n)]) > */
          /*     (d1[i+j] + d1[r+q[i]] + d2[d2k(i,j,n)] + d2[d2k(r,q[i],n)] + 4)) */
          if ((c1(i+q[i]) + c1(r+j) + c2(d2k(i,q[i],n)) + c2(d2k(r,j,n))) >
              (c1(i+j) + c1(r+q[i]) + c2(d2k(i,j,n)) + c2(d2k(r,q[i],n))))
            {
              /* --d1[i+q[i]]; */
              /* --d1[r+j]; */
              
              --d2[d2k(i,q[i],n)];
              --d2[d2k(r,j,n)];
              ++d1[i+j];
              ++d1[r+q[i]];
              ++d2[d2k(i,j,n)];
              ++d2[d2k(r,q[i],n)];
              
              qint t = q[i];
              q[i] = q[r];
              q[r] = t;
              swaped = 1;
            }
        }
      }
    } while (swaped);
    printf ("cntcls: %d\n", cntcls (d1, d2, n));
  } while (cntcls (d1, d2, n));
  printf ("cntcls: %d\n", cntcls (d1, d2, n));
  munmap (d1, dsz);
  return q;
}

static void
printq (qint *q, qint n)
{
  qint r,c;
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
int
main (int argc, char *argv[])
{
  int n = 10;
  qint *q = queenfind (n);
  printq (q, n);
}
