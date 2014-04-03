#include <algorithm>
#include <fstream>
#include <vector>

#define MAX_M 300000
#define MAX_N 7000

using namespace std;

struct Edge {
  int w;
  short u;
  short v;
  int i;
};

static Edge edges[MAX_M];
static bool results[MAX_M];
static short parent[MAX_N + 1];

struct EdgeCmp {
  bool operator ()(Edge const &e1, Edge const &e2) const {
    return e1.w < e2.w;
  }
};

static inline short find(int el) {
  if (parent[el] == el)
    return el;
  return parent[el] = find(parent[el]);
}

static inline void union_set(int el1, int el2) {
  parent[find(el1)] = find(el2);
}

void solve(istream &in, ostream &out) {
  int n, m;
  in >> n >> m;
  for (int i = 0; i < m; ++i) {
    in >> edges[i].u >> edges[i].v >> edges[i].w;
    edges[i].i = i;
  }
  sort(edges, edges + m, EdgeCmp());
  for (int i = 1; i <= n; ++i)
    parent[i] = i;
  for (int i = 0, j; i < m;) {
    for (j = i; j < m && edges[j].w == edges[i].w; ++j)
      if (find(edges[j].u) != find(edges[j].v))
        results[edges[j].i] = true;
    for (; i < j; ++i)
      if (find(edges[i].u) != find(edges[i].v))
        union_set(edges[i].u, edges[i].v);
  }
  for (int i = 0; i < m; ++i)
    if (results[i])
      out << "YES\n";
    else
      out << "NO\n";
}

int main() {
  ifstream in("byteland.in");
  ofstream out("byteland.out");
  solve(in, out);
  return 0;
}
