#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

static int const max_w = 1000*1000*1000;
static long long const inf = static_cast<long long>(max_w) * max_w;

struct edge {
  int w;
  short u;
  short v;
};

static vector<edge> capital;
static vector<edge> country;
static vector<edge> fullset;
static vector<short> parent;
static vector<short> ssize;
static short n, d;
static int over;

static inline bool operator <(edge const lhs, edge const rhs) {
  return lhs.w + over < rhs.w;
}


static inline void sinit() {
  for (short i = 1; i <= n; ++i) {
    parent[i] = i;
    ssize[i] = 1;
  }
}

static inline short sfind(short const v) {
  if (parent[v] == v)
    return v;
  return parent[v] = sfind(parent[v]);
}

static inline void sunion(short s1, short s2) {
  s1 = sfind(s1);
  s2 = sfind(s2);
  if (ssize[s1] < ssize[s2]) {
    parent[s1] = s2;
    ssize[s2] += ssize[s1];
  } else {
    parent[s2] = s1;
    ssize[s1] += ssize[s2];
  }
}

long long kruskal() {
  merge(country.begin(), country.end(),
        capital.begin(), capital.end(),
        fullset.begin());
  sinit();
  short k = n;
  short degree = 0;
  long long weight = 0;
  for (int i = 0; k > 1; ++i) {
    edge const e = fullset[i];
    if (sfind(e.u) == sfind(e.v))
      continue;
    if (e.u == 1)
      if (++degree > d)
        return inf;
    sunion(e.u, e.v);
    weight += e.w;
    --k;
  }
  return weight;
}

long long bs() {
  int first = 0;
  int last = max_w;
  long long result = 0;
  while (last - first > 0) {
    over = first + (last - first) / 2;
    long long const weight = kruskal();
    if (weight < inf) {
      result = weight;
      last = over;
    } else
      first = over + 1;
  }
  return result;
}

void solve(FILE *in, FILE *out) {
  int m;
  fscanf(in, "%hd%d%hd", &n, &m, &d);
  fullset.resize(m);
  parent.resize(n + 1);
  ssize.resize(n + 1);
  for (int i = 0; i < m; ++i) {
    edge e;
    fscanf(in, "%hd%hd%d", &e.u, &e.v, &e.w);
    if (e.u == 1 || e.v == 1) {
      if (e.v == 1)
        swap(e.u, e.v);
      capital.push_back(e);
    } else
      country.push_back(e);
  }
  sort(capital.begin(), capital.end());
  sort(country.begin(), country.end());
  fprintf(out, "%I64d\n", bs());
}

int main() {
  FILE *in = fopen("highway.in", "r");
  FILE *out = fopen("highway.out", "w");
  solve(in, out);
  return 0;
}
