#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <glib.h>

#define USED(x)((void)x)

enum {
  eps = -1,
};

typedef struct s s;
struct s {
  s *l, *r;
  char c;
};
typedef struct dfas dfas;
struct dfas {
  //dfas *l, *r;
  int as;
  dfas *c[127];
};
static s *
news (char c)
{
  s *s = calloc (1, sizeof (struct s));
  assert (s);
  s->c = c;
  return s;
}
static s *
buildlet (char c)
{
  assert (c != eps);
  assert (isalpha (c) || c == '\0');
  s *s0 = news (c);
  s0->l = news (eps);
  return s0;
}
static s *
lasts (s *x)
{
  GSList *visited = NULL;
  s * _lasts (s *s) {
    visited = g_slist_append (visited, s);
    if (!s->l && !s->r)
      return s;
    if (s->l && (! g_slist_find (visited, s->l)))
      return _lasts (s->l);
    if (s->r && (! g_slist_find (visited, s->r)))
      return _lasts (s->r);
    assert (! "lasts");
  }
  return _lasts (x);
}
static s *
concats (s *s0, s *s1)
{
  s *t;
  t = lasts (s0);
  t->c = eps;
  t->l = s1;
  lasts (s1)->c = eps;
  return s0;
}
static s *
alts (s *s0, s *s1)
{
  s *s, *e, *l0, *l1;
  s = news (eps);
  s->l = s0;
  s->r = s1;

  e = news (eps);

  l0 = lasts (s0);
  l1 = lasts (s1);
  l0->l = l1->l = e;
  assert (!l0->r && !l1->r);
  
  return s;
}
static s *
closures (s *s0)
{
  s *s, *e, *l;
  
  e = news (eps);
  
  s = news (eps);
  s->l = s0;
  s->r = e;

  l = lasts (s0);
  l->l = s0;
  l->r = e;

  return s;
}
/* parse regexp */
static s *
pr (char *str)
{  
  s *
    _pr (char *str, int op, s *s)
  {
    if (*str == '|')
      return alts (s, _pr (str+1, op, news (eps)));
    else if (*str == '*')
      return _pr (str+1, op, closures (s));
    else if (*str == '(')
      return concats (s, _pr (str+1, op+1, news (eps)));
    else if (*str == ')')
      return _pr (str+1, op-1, s);
    else if (*str == '\0') {
      if (op)
        printf ("parenthesis mismatch: %d\n", op);
      return s;
    }
    else
      return _pr (str+1, op, concats (s, buildlet (*str)));
  }
  int op = 0;
  while (*str == '(') {
    ++str;
    ++op;
  }
  assert (isalpha (*str) || *str == '\0');
  return _pr (str, op, buildlet (*str++));
}
static void
traversenfa (s *x)
{
  GSList *visited = NULL;
  void _t (s *s) {
    visited = g_slist_append (visited, s);

    if (!s->l && !s->r)
      printf ("last\n");
    else if (s->c == eps)
      printf ("eps ");
    else
      printf ("%c(%d) ", s->c, s->c);
  
    if (s->l && (! g_slist_find (visited, s->l)))
      _t (s->l);
    if (s->r && (! g_slist_find (visited, s->r))) {
      _t (s->r);
    }
  }
  _t (x);
  // free (s);
}

static GSList *
epsclosure (s *s0)
{
  GSList *q = NULL;
  void _xxx (s *s) {
    q = g_slist_append (q, s);
    if (s->c == eps) {
      if (s->l)
        _xxx (s->l);
      if (s->r)
        _xxx (s->r);
    }
  }

  _xxx (s0);
  return q;
}

static GSList *
delta (GSList *q, char c)
{
  GSList *d = NULL;
  void _xxx (void *_x, void *data) {
    USED (data);
    s *x = _x;
    assert (x);
    if (g_slist_find (d, x))
      return;
    
    d = g_slist_append (d, x);
    
    if (x->c == c || x->c == eps) {
      if (x->l) {
        _xxx (x->l, NULL);
      }
      if (x->r) {
        _xxx (x->r, NULL);
      }
    }
  }
  g_slist_foreach (q, _xxx, NULL);

  return d;
}

struct t {
  GSList *q;
  char c;
  GSList *t;
};

static struct t *
newT (GSList *q, char c, GSList *t)
{
  struct t *x = malloc (sizeof (*x));
  x->q = q;
  x->c = c;
  x->t = t;
  return x;
}
static dfas *
newdfas (void)
{
  dfas *d = calloc (1, sizeof (*d));
  return d;
}
/* NFA to DFA */
static s *
nfa2dfa (s *s0)
{
  GSList *q0, *Q, *WL, *T, *t, *q;

  q0 = Q = WL = T = t = q = NULL;
  
  q0 = epsclosure (s0);
  printf ("q0: %d\n", g_slist_length (q0));
  Q = g_slist_append (Q, q0);
  WL = g_slist_append (WL, q0);

  while (WL) {
    q = g_slist_nth_data (WL, 0);
    WL = g_slist_next (WL);
    printf ("q: %d\n", g_slist_length (q));
    for (char c = 'a'; c <= 'c'; ++c) {
      t = NULL;
      GSList *d = delta (q, c);
      printf ("delta: %d\n", g_slist_length (d));
      void _tadd (void *x, void *data) {
        USED (data);
        void _app (void *y, void *data) {
          USED (data);
          if (! g_slist_find (t, y))
            t = g_slist_append (t, y);
        }
        GSList *c = epsclosure (x);
        g_slist_foreach (c, _app, NULL);
      }
      g_slist_foreach (d, _tadd, NULL);

      printf ("t: %d\n", g_slist_length (t));
      
      T = g_slist_append (T, newT (q, c, t));
      
      int sameset (const void *_Qdata, const void *_t) {
        GSList *Qdata = _Qdata;
        GSList *t = _t;
        while (t) {
          if (! g_slist_find (Qdata, t->data))
            return 1;
          t = t->next;
        }
        return 0;
      }

      /* if (! g_slist_find (Q, t)) { */
      if (! g_slist_find_custom (Q, t, sameset)) {
        printf ("not equal\n");
        Q = g_slist_append (Q, t);
        WL = g_slist_append (WL, t);
      } else {
        printf ("equal\n");
      }
    }
  }

  void _pr (void *x, void *data) {
    USED (data);
    struct t *t = x;
    printf ("%d\n", g_slist_length (t->t));
    printf ("%c\n", t->c);
  }
  void _Qpr (void *_x, void *data) {
    USED (data);
    GSList *x = _x;
    printf ("%d\n", g_slist_length (x));
  }
                                 
  printf ("Q: %d\n", g_slist_length (Q));
  g_slist_foreach (Q, _Qpr, NULL);
  printf ("T: %d\n", g_slist_length (T));
  g_slist_foreach (T, _pr, NULL);

  /* dfas * QT2dfa (s *qi, char c) { */
  /*   GSList *t; */
  /*   for (t = T; t; t = t->next) { */
  /*     struct t *x = t->data; */
  /*     if (x->q == qi && */
  /*         x->c == c) */
  /*       { */
  /*         dfas *d = newdfas (); */
          
        
  /*     if (t->data */
  /*   dfas *d = newdfas (); */
    
  
  /* dfas *dfa = newdfas (); */
  /* s *xq = g_slist_nth (Q, 0); */
  /* Q = g_slist_next (Q); */
  /* dfa.c[xq.c] */
  /* GSList *q0 = g_slist_nth (Q, 0); */
  
  return NULL;
}

void
str2nfa (char *str)
{
  printf ("%s\n", str);
  traversenfa (pr(str));
  printf ("\n");
}

int
main (void)
{
  str2nfa ("a");
  str2nfa ("(a)");
  str2nfa ("ab");
  str2nfa ("a|b");
  str2nfa ("ab|cd");
  str2nfa ("a*");
  str2nfa ("a(b|c)*");
  /* printf ("a|b\n"); */
  /* r = pr ("a|b"); */
  /* traversenfa (r); */
  /* printf ("\n"); */
  /* printf ("(a)\n"); */
  /* r = pr ("(a)"); */
  /* traversenfa (r); */
  /* printf ("\n"); */
  /* printf ("a(b)\n"); */
  /* r = pr ("a(b)"); */
  /* traversenfa (r); */
  /* printf ("\n"); */
  /* printf ("|a\n"); */
  /* r = pr ("|a"); */
  /* traversenfa (r); */
  /* printf ("\n"); */
  /* printf ("a*\n"); */
  /* r = pr ("a*"); */
  /* traversenfa (r); */
  /* printf ("\n"); */

  /* str = "a(b|c)*"; */
  /* printf ("%s\n", str); */
  /* r = pr (str); */
  /* traversenfa (r); */
  /* printf ("\n"); */
  /* printf ("nfa2dfa\n"); */
  /* nfa2dfa (r->l); */
}
