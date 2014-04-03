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

int const max_size = 250*1000;
char buf[max_size + 4];
int last[max_size + 1];
int p[max_size + 1];
int c[max_size + 1];
int size;

void prefix_function() {
  for (int i = 1; i < size; ++i)
    for (int v = i; v > 0;) {
      v = p[v];
      if (buf[v] == buf[i]) {
        p[i + 1] = v + 1;
        break;
      }
    }
}

void solve(FILE *in, FILE *out) {
  scan(1, "%s", buf);
  size = strlen(buf);
  prefix_function();
  for (int i = 1; i <= size; ++i) {
    c[i] = i;
    last[i] = i;
    if (last[c[p[i]]] >= i - c[p[i]]) {
      c[i] = c[p[i]];
      last[c[i]] = i;
    }
    print("%d ", c[i]);
  }
  print("\n");
}

int main() {
  FILE *in  = fopen("cover.in",  "r");
  FILE *out = fopen("cover.out", "w");
  assert(in);
  assert(out);
  solve(in, out);
  fclose(out);
  fclose(in);
  return 0;
}
