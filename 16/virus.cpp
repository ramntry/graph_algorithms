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
#  define LLX "%I64x"automaton[v].next[*s - '0'];
#endif

#define scan(n, ...) assert(fscanf(in, __VA_ARGS__) == n)
#define print(...) fprintf(out, __VA_ARGS__)
#define debug(...) fprintf(stderr, __VA_ARGS__)

using namespace std;

short const maxn = 30*1000;

struct vertex {
  short link;
  short parent;
  short next[2];
  char incoming;
  char isleaf;
  char known;
};

char buf[maxn + 1];
char color[maxn + 1];
vertex automaton[maxn + 1];
short numof_vs = 1;

inline void add_string(char const *s) {
  short v = 1;
  while (*s) {
    short &next = automaton[v].next[*s - '0'];
    if (!next) {
      next = ++numof_vs;
      automaton[numof_vs].parent = v;
      automaton[numof_vs].incoming = *s - '0';
    }
    v = next;
    ++s;
  }
  automaton[v].isleaf = true;
  automaton[v].known = true;
}

inline short link(short v);

inline short next(short v, short sym) {
  vertex &node = automaton[v];
  if (node.next[sym])
    return node.next[sym];
  return node.next[sym] = next(link(v), sym);
}

inline short link(short v) {
  vertex &node = automaton[v];
  if (node.link)
    return node.link;
  return node.link = next(link(node.parent), node.incoming);
}

inline bool isstrike(short v) {
  vertex &node = automaton[v];
  if (node.known)
    return node.isleaf;
  node.known = true;
  return (node.isleaf = isstrike(link(v)));
}

inline short child(short v, short sym) {
  short const n = next(v, sym);
  return isstrike(n) ? 0 : n;
}

void dump(short v, int indent = 0) {
  static char dump_color[maxn + 1];
  if (indent == 0)
    for (short i = 1; i <= maxn; ++i)
      dump_color[i] = 0;
  debug("%*s(%hd)", indent, "", v);
  if (dump_color[v]) {
    debug(" <backref>\n");
    return;
  }
  if (!v) {
    debug("\n");
    return;
  }
  dump_color[v] = 1;
  vertex const &node = automaton[v];
  debug(" isleaf = %s, link = %hd, parent = %hd, incoming = %hhd\n",
      node.isleaf ? "true" : "false", node.link, node.parent, node.incoming);
  dump(node.next[0], indent + 2);
  dump(node.next[1], indent + 2);
}

void dfs(int v, FILE *out) {
  if (color[v] == 2)
    return;
  if (color[v] == 1) {
    print("TAK\n");
    exit(0);
  }
  color[v] = 1;
  dfs(child(v, 0), out);
  dfs(child(v, 1), out);
  color[v] = 2;
}

void solve(FILE *in, FILE *out) {
  short k = 0;
  scan(1, "%hd", &k);
  for (short i = 0; i < k; ++i) {
    scan(1, "%s", buf);
    add_string(buf);
  }
  automaton[1].link = 1;
  short *rootnext = automaton[1].next;
  if (!rootnext[0])
    rootnext[0] = 1;
  if (!rootnext[1])
    rootnext[1] = 1;
  automaton[rootnext[0]].link = 1;
  automaton[rootnext[1]].link = 1;
  color[0] = 2;
  dfs(1, out);
  print("NIE\n");
}

int main() {
  FILE *in  = fopen("virus.in",  "r");
  FILE *out = fopen("virus.out", "w");
  assert(in);
  assert(out);
  solve(in, out);
  fclose(out);
  fclose(in);
  return 0;
}
