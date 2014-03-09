#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

vector<vector<int> > graph;
vector<int> cutpoints;
vector<int> uppermost_succtime;
vector<int> time_in;
int timer;

void dfs(int v, int p = 0) {
  time_in[v] = ++timer;
  uppermost_succtime[v] = timer;
  int numof_children = 0;
  bool already_cutpoint = false;
  for (int i = 0, numof_nbs = graph[v].size(); i < numof_nbs; ++i) {
    int const nb = graph[v][i];
    if (nb == p)
      continue;
    if (time_in[nb])
      uppermost_succtime[v] = min(uppermost_succtime[v], time_in[nb]);
    else {
      dfs(nb, v);
      int const child_umsc = uppermost_succtime[nb];
      uppermost_succtime[v] = min(uppermost_succtime[v], child_umsc);
      if (p && !already_cutpoint && child_umsc >= time_in[v]) {
        cutpoints.push_back(v);
        already_cutpoint = true;
      }
      ++numof_children;
    }
  }
  if (!p && numof_children > 1)
    cutpoints.push_back(v);
}

int main() {
  freopen("points.in", "r", stdin);
  freopen("points.out", "w", stdout);
  int n, m;
  scanf("%d%d", &n, &m);
  graph.resize(n + 1);
  time_in.resize(n + 1, 0);
  uppermost_succtime.resize(n + 1, 0);
  for (int i = 0, f, t; i < m; ++i) {
    scanf("%d%d", &f, &t);
    graph[f].push_back(t);
    graph[t].push_back(f);
  }
  for (int v = 1; v <= n; ++v)
    if (!time_in[v])
      dfs(v);
  sort(cutpoints.begin(), cutpoints.end());
  int const numof_cutpoints = cutpoints.size();
  printf("%d\n", numof_cutpoints);
  for (int i = 0; i < numof_cutpoints; ++i)
    printf("%d ", cutpoints[i]);
  printf("\n");
  return 0;
}
