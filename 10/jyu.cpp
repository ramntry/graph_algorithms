#include <algorithm>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

static size_t const max_n = 500001;
static vector<pair<int, int> > graph[max_n];
static long long root_length[max_n];
static set<long long> subtree_lengths[max_n];
typedef set<long long>::const_iterator lengths_it;
static long long result;
static long long k;

static inline void update_res(set<long long> const &lengths, long long k_val, ostream &out) {
  lengths_it it = lengths.lower_bound(k_val);
  if (it == lengths.end())
    result = min(result, k_val - *(--it));
  else {
    long long const finded_k = *it;
    if (finded_k == k_val) {
      out << 0 << endl;
      exit(0);
    }
    result = min(result, finded_k - k_val);
    if (it != lengths.begin())
      result = min(result, k_val - *(--it));
  }
}

struct is_vertex : unary_function<pair<int, int>, bool> {
  is_vertex(int const v) : v_(v) {}
  bool operator ()(pair<int, int> e) const {
    return e.first == v_;
  }
private:
  int const v_;
};

static inline void dfs(int v, int p, ostream &out) {
  long long const rlen = root_length[v];
  long long const vert_k = k + rlen;
  long long const horz_k = k + 2 * rlen;
  vector<pair<int, int> > &nbs = graph[v];
  nbs.erase(find_if(nbs.begin(), nbs.end(), is_vertex(p)));
  size_t const numof_nbs = nbs.size();
  for (size_t i = 0; i < numof_nbs; ++i) {
    int const nb = nbs[i].first;
    root_length[nb] = rlen + nbs[i].second;
    dfs(nb, v, out);
  }
  for (size_t step = 1; step < numof_nbs; step *= 2)
    for (size_t i = step; i < numof_nbs; i += 2 * step) {
      set<long long> &target = subtree_lengths[nbs[i - step].first];
      set<long long> &source = subtree_lengths[nbs[i].first];
      if (target.size() < source.size())
        target.swap(source);
      lengths_it const end = source.end();
      for (lengths_it it = source.begin(); it != end; ++it) {
        long long rem = horz_k - *it;
        if (rem <= 0)
          break;
        update_res(target, rem, out);
      }
      target.insert(source.begin(), end);
      source.clear();
    }
  if (numof_nbs) {
    subtree_lengths[v].swap(subtree_lengths[nbs[0].first]);
    update_res(subtree_lengths[v], vert_k, out);
  }
  subtree_lengths[v].insert(rlen);
}

static void solve(istream &in, ostream &out) {
  size_t n;
  in >> n >> k;
  int const start_v = 1;
  graph[start_v].push_back(make_pair(0, 0));
  for (size_t e = 1; e < n; ++e) {
    int i, j, w;
    in >> i >> j >> w;
    graph[i].push_back(make_pair(j, w));
    graph[j].push_back(make_pair(i, w));
  }
  result = k;
  dfs(start_v, 0, out);
  out << result << endl;
}

int main() {
  ifstream in("jyu.in");
  ofstream out("jyu.out");
  solve(in, out);
  return 0;
}
