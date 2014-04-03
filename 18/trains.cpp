#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>

#ifdef OFFLINE
#  define LLD "%lld"
#  define LLU "%llu"
#  define LLX "%llx"
#else
#  define LLD "%I64d"
#  define LLU "%I64u"
#  define LLX "%I64x"
#endif

#define scan(n, ...) assert(fscanf(in, __VA_ARGS__) == n)
#define print(...) fprintf(out, __VA_ARGS__)
#define debug(...) fprintf(stderr, __VA_ARGS__)

using namespace std;

int const max_len = 100;
int const max_trains = 1000;
int const max_swaps = 100*1000;
int numof_trains;
int length;

struct train {
  short curmax;
  char colors[max_len + 1];
};

train trains[max_trains + 1];

struct by_colors {
  bool operator ()(train const *lhs, train const *rhs) const {
    return strcmp(lhs->colors, rhs->colors) < 0;
  }
};

struct by_curmax {
  bool operator ()(train const *lhs, train const *rhs) const {
    return lhs->curmax < rhs->curmax || (lhs->curmax == rhs->curmax && lhs < rhs);
  }
};

typedef set<train *, by_curmax> same_set;
typedef map<train *, same_set, by_colors> same_map;
same_map same_sets;

inline void insert(int t) {
  static train *buf[max_trains];
  same_set &s = same_sets[trains + t];
  s.insert(trains + t);
  short const size = s.size();
  same_set::iterator it = s.begin();
  int buffered = 0;
  for (same_set::iterator end = s.end(); it != end && (*it)->curmax < size; ++it)
    buf[buffered++] = *it;
  s.erase(s.begin(), it);
  for (int i = 0; i < buffered; ++i)
    buf[i]->curmax = size;
  s.insert(buf, buf + buffered);
}

inline void remove(int t) {
  same_map::iterator it = same_sets.find(trains + t);
  if (it->second.size() == 1)
    same_sets.erase(it);
  else {
    it->second.erase(trains + t);
    if (it->first == trains + t) {
      same_set s;
      s.swap(it->second);
      same_sets.erase(it);
      same_sets[*s.begin()].swap(s);
    }
  }
}

inline void swap_cars(int t1, int c1, int t2, int c2) {
  remove(t1);
  if (t1 != t2)
    remove(t2);
  swap(trains[t1].colors[c1 - 1], trains[t2].colors[c2 - 1]);
  insert(t1);
  if (t1 != t2)
    insert(t2);
}

void solve(FILE *in, FILE *out) {
  int numof_swaps;
  scan(3, "%d%d%d", &numof_trains, &length, &numof_swaps);
  for (int i = 1; i <= numof_trains; ++i) {
    scan(1, "%s", trains[i].colors);
    insert(i);
  }
  for (int i = 1, t1, c1, t2, c2; i <= numof_swaps; ++i) {
    scan(4, "%d%d%d%d", &t1, &c1, &t2, &c2);
    swap_cars(t1, c1, t2, c2);
  }
  for (int i = 1; i <= numof_trains; ++i)
    print("%d\n", trains[i].curmax);
}

int main() {
  FILE *in  = fopen("trains.in",  "r");
  FILE *out = fopen("trains.out", "w");
  assert(in);
  assert(out);
  solve(in, out);
  fclose(out);
  fclose(in);
  return 0;
}
