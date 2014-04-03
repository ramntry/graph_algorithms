#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

#ifdef OFFLINE
#  define LLD "%lld"
#else
#  define LLD "%I64d"
#endif

#define scan(n, ...) assert(fscanf(in, __VA_ARGS__) == n)
#define print(...) fprintf(out, __VA_ARGS__)
#define debug(...) fprintf(stderr, __VA_ARGS__)

using namespace std;

typedef signed char val;
typedef unsigned long long state;

static size_t const table_size = 8*1024*1024;
static size_t const table_mask = 0x7FFFFF;

struct table {
  state keys[table_size];
  val values[table_size];
};

static inline size_t hash(state const key) {
  return (key + (key >> 23) + (key >> 41)) * 2147483647;
}

static inline void update(table *const t, state const key, val const value) {
  size_t h = hash(key);
  for (;; ++h) {
    size_t const i = h & table_mask;
    state const k = t->keys[i];
    if (k == 0) {
      t->keys[i] = key;
      t->values[i] = value;
      return;
    }
    if (k == key) {
      t->values[i] = min(t->values[i], value);
      return;
    }
  }
}

static inline val get(table *const t, state const key) {
  size_t h = hash(key);
  for (;; ++h) {
    size_t const i = h & table_mask;
    state const k = t->keys[i];
    if (k == 0)
      return -1;
    if (k == key)
      return t->values[i];
  }
}

static table m1;
static table m2;

static val num;
static state const xF = 0xF;
static state const x1 = 0x1;

static inline state step_into(state s, val const to, val const from) {
  val const sht = (num - to)   * 4;
  val const shf = (num - from) * 4;
  state const vt = (s >> sht) & xF;
  s &= ~(xF << sht);
  s &= ~(xF << shf);
  s |= vt << shf;
  s |= x1 << sht;
  return s;
}

static inline void dfs(state const s, val const pos, val const from,
                       val const d, val const bound, table *const m) {
  static val const tos[4][16] = {
    { 16,  3,  5,  6,  8, 9, 10, 12, 13, 14, 15, 16, 16, 16,  16, 16 },
    { 16,  2,  4,  5,  7, 8,  9, 11, 12, 13, 14, 16, 16, 16,  16, 16 },
    { 16, 16, 16,  1, 16, 2,  3, 16,  4,  5,  6, 16,  7,  8,   9, 10 },
    { 16, 16,  1, 16,  2, 3, 16,  4,  5,  6, 16,  7,  8,  9,  10, 16 } };
  update(m, s, d);
  if (d == bound)
    return;
  for (val i = 0; i < 4; ++i) {
    val const to = tos[i][pos];
    if (to > num || to == from)
      continue;
    dfs(step_into(s, to, pos), to, pos, d + 1, bound, m);
  }
}

void solve(FILE *in, FILE *out) {
  val n;
  scan(1, "%hhd", &n);
  num = n * (n + 1) / 2;
  state s = 0;
  state init = 0;
  val pos = 0;
  for (val i = 1, lbl; i <= num; ++i) {
    s <<= 4;
    init <<= 4;
    scan(1, "%hhd", &lbl);
    s |= lbl;
    init |= i;
    if (lbl == 1)
      pos = i;
  }
  dfs(s, pos, pos, 0, 22, &m1);
  dfs(init, 1, 1, 0, 23, &m2);
  val res = 100;
  for (size_t i = 0; i < table_size; ++i) {
    state const k = m1.keys[i];
    if (k) {
      val const v2 = get(&m2, k);
      if (v2 != -1)
        res = min(res, static_cast<val>(m1.values[i] + v2));
    }
  }
  print("%hhd\n", res);
}

int main() {
  FILE *in  = fopen("billiards.in",  "r");
  FILE *out = fopen("billiards.out", "w");
  assert(in);
  assert(out);
  solve(in, out);
  fclose(out);
  fclose(in);
  return 0;
}
