#include <cstdlib>
#include <cstdio>
#include <vector>

enum color { White, Gray, Black };
std::vector<std::vector<int > > graph;
std::vector<int> topsort;
std::vector<color> colors;

void bfs(int v) {
  colors[v] = Gray;
  int const numof_nbs = graph[v].size();
  for (int i = 0; i < numof_nbs; ++i) {
    int const nb = graph[v][i];
    if (colors[nb] == Gray) {
      printf("-1\n");
      exit(0);
    }
    if (colors[nb] == White)
      bfs(nb);
  }
  colors[v] = Black;
  topsort.push_back(v);
}

int main() {
  freopen("topsort.in", "r", stdin);
  freopen("topsort.out", "w", stdout);
  int n, m;
  scanf("%d%d", &n, &m);
  graph.resize(n + 1);
  colors.resize(n + 1, White);
  topsort.reserve(n);
  for (int i = 0; i < m; ++i) {
    int f, t;
    scanf("%d%d", &f, &t);
    graph[f].push_back(t);
  }
  for (int i = 1; i <= n; ++i)
    if (colors[i] == White)
      bfs(i);
  for (int i = n - 1; i >= 0; --i)
    printf("%d ", topsort[i]);
  printf("\n");
  return 0;
}
