#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

static long long const wmod = 1000;
static long long const smod = 2147483647;
static long long const coeff = 16807;
static size_t const max_n = 2000;
static short graph[max_n][max_n];
static int condensed[max_n];
static vector<int> components[max_n];
static size_t consize;
static vector<int> zero_org[max_n];
static vector<int> zero_inv[max_n];
enum { White, Black, Red };
static int const color_step = 4;
static int white;
static int black;
static int red;
static int color[max_n];
static bool classified[max_n];

static void floyd() {
  for (size_t k = 0; k < consize; ++k) {
    int const middle = condensed[k];
    for (size_t i = 0; i < consize; ++i) {
      int const from = condensed[i];
      for (size_t j = 0; j < consize; ++j) {
        int const to = condensed[j];
        int const direct = graph[from][to];
        int const indirect = graph[from][middle] + graph[middle][to];
        if (indirect < direct)
          graph[from][to] = indirect;
      }
    }
  }
}

static inline void dfs_org(int v) {
  color[v] = black;
  vector<int> const &nbs = zero_org[v];
  size_t const numof_nbs = nbs.size();
  for (size_t i = 0; i < numof_nbs; ++i) {
    int const nb = nbs[i];
    if (color[nb] <= white)
      dfs_org(nb);
  }
}

static inline void dfs_inv(int v) {
  if (color[v] == black) {
    components[consize].push_back(v);
    classified[v] = true;
  }
  color[v] = red;
  vector<int> const &nbs = zero_inv[v];
  size_t const numof_nbs = nbs.size();
  for (size_t i = 0; i < numof_nbs; ++i) {
    int const nb = nbs[i];
    if (color[nb] <= black)
      dfs_inv(nb);
  }
}

static void condense(size_t n) {
  for (size_t v = 0; v < n; ++v)
    if (!classified[v]) {
      white = consize * color_step;
      black = white + Black;
      red = white + Red;
      dfs_org(v);
      dfs_inv(v);
      condensed[consize++] = v;
    }
  for (size_t i = 0; i < consize; ++i) {
    int const from = condensed[i];
    vector<int> const &from_comp = components[i];
    size_t const from_size = from_comp.size();
    for (size_t j = 0; j < consize; ++j) {
      int const to = condensed[j];
      vector<int> const &to_comp = components[j];
      size_t const to_size = to_comp.size();
      short w = graph[from][to];
      for (size_t i_prime = 0; i_prime < from_size; ++i_prime) {
        int const from_prime = from_comp[i_prime];
        for (size_t j_prime = 0; j_prime < to_size; ++j_prime) {
          int const to_prime = to_comp[j_prime];
          w = min(w, graph[from_prime][to_prime]);
        }
      }
    graph[from][to] = w;
    }
  }
}

static void decondense() {
  for (size_t i = 0; i < consize; ++i) {
    int const from = condensed[i];
    vector<int> const &from_comp = components[i];
    size_t const from_size = from_comp.size();
    for (size_t j = 0; j < consize; ++j) {
      int const to = condensed[j];
      vector<int> const &to_comp = components[j];
      size_t const to_size = to_comp.size();
      short const w = graph[from][to];
      for (size_t i_prime = 0; i_prime < from_size; ++i_prime) {
        int const from_prime = from_comp[i_prime];
        for (size_t j_prime = 0; j_prime < to_size; ++j_prime) {
          int const to_prime = to_comp[j_prime];
          graph[from_prime][to_prime] = w;
        }
      }
    }
  }
}

static void solve(istream &in, ostream &out) {
  size_t n;
  long long s;
  in >> n >> s;
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      if (i != j) {
        short const w = s % wmod;
        s = (s * coeff) % smod;
        graph[i][j] = w;
        if (w == 0) {
          zero_org[i].push_back(j);
          zero_inv[j].push_back(i);
        }
      }
  condense(n);
  floyd();
  decondense();
  long long r = 0;
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      r = (r * coeff + graph[i][j]) % smod;
  out << r << endl;
}

int main() {
  ifstream in("floyd.in");
  ofstream out("floyd.out");
  solve(in, out);
  return 0;
}
