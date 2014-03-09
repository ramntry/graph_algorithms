#include <cstdio>
#include <vector>
#include <deque>

std::vector<int> result;
std::vector<std::vector<int> > graph;
std::vector<int> vis;

void bfs(int v) {
  result.push_back(v);
  vis[v] = 1;
  size_t curr_i = 0;
  while (curr_i != result.size()) {
    int const curr_v = result[curr_i++];
    int const numof_nbs = graph[curr_v].size();
    for (int i = 0; i < numof_nbs; ++i) {
      int const nb = graph[curr_v][i];
      if (!vis[nb])
        result.push_back(nb);
        vis[nb] = 1;
    }
  }
}

int main() {
  freopen("wave.in", "r", stdin);
  freopen("wave.out", "w", stdout);
  int n, m, v;
  scanf("%d%d%d", &n, &m, &v);
  graph.resize(n + 1);
  vis.resize(n + 1, 0);
  result.reserve(n);
  for (int i = 0; i < m; ++i) {
    int f, t;
    scanf("%d%d", &f, &t);
    graph[f].push_back(t);
    graph[t].push_back(f);
  }
  bfs(v);
  printf("%d\n", n);
  for (int i = 0; i < n; ++i)
    printf("%d ", result[i]);
  printf("\n");
  fclose(stdin);
  fclose(stdout);
}
