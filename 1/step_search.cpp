#include <cstdio>
#include <vector>

std::vector<std::vector<int> > graph;
std::vector<int> results;
std::vector<int> vis;

void dfs(int v) {
  vis[v] = 1;
  results.push_back(v);
  int const numof_nbs = graph[v].size();
  for (int i = 0; i < numof_nbs; ++i) {
    int const nb = graph[v][i];
    if (!vis[nb]) {
      dfs(nb);
      results.push_back(v);
    }
  }
}

int main() {
  freopen("step.in", "r", stdin);
  freopen("step.out", "w", stdout);
  int n, m, v;
  scanf("%d%d%d", &n, &m, &v);
  graph.resize(n + 1);
  vis.resize(n + 1, 0);
  for (int i = 0; i < m; ++i) {
    int f, t;
    scanf("%d%d", &f, &t);
    graph[f].push_back(t);
    graph[t].push_back(f);
  }
  dfs(v);
  int results_size = results.size();
  printf("%d\n", results_size);
  for (int i = 0; i < results_size; ++i)
    printf("%d ", results[i]);
  printf("\n");
}
