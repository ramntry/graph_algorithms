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

int const max_size = 1000*1000;
int z[max_size + 1];
int s[max_size];
int size;

void z_function() {
  for (int i = 1, l = 1, r = 1; i < size; ++i) {
    int z_val = 0;
    if (i < r)
      z_val = min(z[i - l + 1], r - i);
    while (i + z_val < size && s[z_val] == s[i + z_val])
      ++z_val;
    if (i + z_val > r) {
      l = i;
      r = i + z_val;
    }
    z[i + 1] = z_val;
  }
}
void solve(FILE *in, FILE *out) {
  scan(1, "%d", &size);
  for (int i = 0; i < size; ++i)
    s[i] = i;
  for (int i = 0, j = 0, p; i < size; ++i) {
    scan(1, "%d", &p);
    for (j = max(j, i - p + 1); j <= i; ++j)
      s[j] = s[j - i + p - 1];
  }
  z_function();
  for (int i = 1; i <= size; ++i)
    print("%d ", z[i]);
  print("\n");
}

int main() {
  FILE *in  = fopen("ptoz.in",  "r");
  FILE *out = fopen("ptoz.out", "w");
  assert(in);
  assert(out);
  solve(in, out);
  fclose(out);
  fclose(in);
  return 0;
}
