#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <bitset>
#include <iostream>
#include <algorithm>

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

typedef unsigned long long state;

int const max_num  = 64;
int const max_len  = 9;
int const abc_size = 'Z' - 'A' + 1;
size_t const max_res  = 1000;
state const x1 = static_cast<state>(1);
state const x0 = static_cast<state>(0);

int num;
int len;
int half;
char words[max_num][max_len + 1];
char results[max_res][max_len + 1];
int numof_results;

state tables[max_len][abc_size + 1];

int const table_size = abc_size * abc_size * abc_size * abc_size * 4 - 3;
pair<int, vector<int> > values[table_size];
state keys[table_size];

static inline size_t hash_it(state const key) {
  return (key >> 32) * 241 + key;
}

inline bool has(state const key) {
  for (size_t h = hash_it(key);; ++h) {
    size_t const i = h % table_size;
    if (!values[i].first)
      return false;
    if (keys[i] == key)
      return true;
  }
}

inline pair<int, vector<int> > const &get(state const key) {
  for (size_t h = hash_it(key);; ++h) {
    size_t const i = h % table_size;
    if (keys[i] == key)
      return values[i];
  }
}

inline void add(state const key, int const new_value) {
  for (size_t h = hash_it(key);; ++h) {
    size_t const i = h % table_size;
    pair<int, vector<int> > &value = values[i];
    if (!value.first) {
      keys[i] = key;
      value.first = new_value;
      return;
    }
    if (keys[i] == key) {
      if (value.second.size() < max_res)
        value.second.push_back(new_value);
      return;
    }
  }
}

void init_tables() {
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < num; ++j) {
      int const letter = words[j][i] - 'A';
      tables[i][letter] |= (x1 << j);
    }
    state const a_state = tables[i][0];
    for (int k = 1; k < abc_size; ++k)
      tables[i][k - 1] ^= tables[i][k];
    tables[i][abc_size - 1] ^= a_state;
    tables[i][abc_size] = a_state;
  }
}

inline void add_result(int lhs, int rhs) {
  for (int i = half - 1; i >= 0; --i) {
    results[numof_results][i] = (lhs % 26) + 'A';
    lhs /= 26;
  }
  for (int i = len - 1; i >= half ; --i) {
    results[numof_results][i] = (rhs % 26) + 'A';
    rhs /= 26;
  }
  ++numof_results;
}

inline void gen_mitm(int const i, int const to, int const n, state s) {
  if (i == to) {
    add(s, n);
    return;
  }
  s ^= tables[i][abc_size];
  for (int j = 0; j < abc_size; ++j) {
    gen_mitm(i + 1, to, n * abc_size + j, s);
    s ^= tables[i][j];
  }
}

inline void gen_results(int const i, int const to, int const n, state s) {
  if (i == to) {
    if (!has(s))
      return;
    pair<int, vector<int> > const &lhs = get(s);
    add_result(n, lhs.first);
    size_t const lhs_size = lhs.second.size();
    for (size_t i = 0; i < lhs_size; ++i)
      add_result(n, lhs.second[i]);
    return;
  }
  s ^= tables[i][abc_size];
  for (int j = 0; j < abc_size; ++j) {
    gen_results(i + 1, to, n * abc_size + j, s);
    s ^= tables[i][j];
  }
}

void solve(FILE *in, FILE *out) {
  int even;
  scan(2, "%d%d", &even, &num);
  state const target_mask = ~(~x0 << num) << even;
  num += even;
  for (int i = 0; i < num; ++i)
    scan(1, "%s", words[i]);
  len = strlen(words[0]);
  half = (len + 1) / 2;
  init_tables();
  gen_mitm(half, len, 1, 0);
  gen_results(0, half, 1, target_mask);
  print("%d\n", numof_results);
  for (int i = 0; i < numof_results - 1; ++i)
    print("%s ", results[i]);
  if (numof_results)
    print("%s", results[numof_results - 1]);
  print("\n");
}

int main() {
  FILE *in  = fopen("wordgame.in",  "r");
  FILE *out = fopen("wordgame.out", "w");
  assert(in);
  assert(out);
  solve(in, out);
  fclose(out);
  fclose(in);
  return 0;
}
