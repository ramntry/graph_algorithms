#include <fstream>
#include <vector>

using namespace std;

typedef long long Weight;
typedef pair<int, Weight> From;
typedef vector<From> RevNbs;
typedef vector<RevNbs> Graph;
typedef vector<Weight> Dists;

Weight const pinf = 8LL*1000*1000*1000*1000*1000*1000;
Weight const ninf = -pinf;

template <typename Updater>
void bf(Graph const &graph, Dists &dists, Updater const &update) {
  size_t const size = graph.size();
  for (size_t i = 1; i < size; ++i)
    for (size_t v = 1; v < size; ++v) {
      Weight d = dists[v];
      RevNbs const &revnbs = graph[v];
      size_t const numof_revnbs = revnbs.size();
      for (size_t j = 0; j < numof_revnbs; ++j) {
        From const &u = revnbs[j];
        Weight const u_dist = dists[u.first];
        if (u_dist < pinf)
          d = min(d, u_dist + u.second);
      }
      update(dists[v], d);
    }
}

struct Normal {
  void operator ()(Weight &curr_dist, Weight new_dist) const {
    curr_dist = new_dist;
}};

struct Contagious {
  void operator ()(Weight &curr_dist, Weight new_dist) const {
    if (new_dist != curr_dist)
      curr_dist = ninf;
}};

void solve(istream &in, ostream &out) {
  int n, m, s;
  in >> n >> m >> s;
  Graph graph(n + 1);
  for (int i = 0, f, t; i < m; ++i) {
    Weight w;
    in >> f >> t >> w;
    graph[t].push_back(make_pair(f, w));
  }

  Dists dists(n + 1, pinf);
  dists[s] = 0;
  bf(graph, dists, Normal());
  bf(graph, dists, Contagious());

  for (int v = 1; v <= n; ++v) {
    Weight const d = dists[v];
    if (d <= ninf)
      out << '-' << endl;
    else if (d >= pinf)
      out << '*' << endl;
    else
      out << d << endl;
  }
}

int main() {
  ifstream in("path.in");
  ofstream out("path.out");
  solve(in, out);
  return 0;
}
