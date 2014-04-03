#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

typedef signed char val;

static val const max_n = 42;
static val num[max_n];
static short n;

typedef bitset<max_n + 1> bits;
static bits target;

struct action {
  char const *name;
  val lhs;
  val rhs;
  val res;
};

static char const *const shift = "<<";
static char const *const add   =  "+";
static char const *const sub   =  "-";

static action deferred[max_n + 1][max_n];
static val defrd_size[max_n + 1];
static action result[max_n];
static val start_d;
static size_t const mem_size = 128*1000*1000;
static bits mem[mem_size];

static inline bool iddfs(bits state, val const i, val const rem, val const d);

static inline bool process(bits &was_processed, bits state, val const j,
                           val const rem, val const d,
                           char const *const actname, val const lhs, val const rhs) {
  if (was_processed.test(j))
    return false;
  was_processed.set(j);
  if (target.test(j)) {
    state.set(j);
    if (iddfs(state, j, rem - 1, d - 1)) {
      result[d].name = actname;
      result[d].lhs = lhs;
      result[d].rhs = rhs;
      return true;
    }
    return false;
  }
  action &defrd = deferred[d][defrd_size[d]];
  defrd.name = actname;
  defrd.lhs = lhs;
  defrd.rhs = rhs;
  defrd.res = j;
  ++defrd_size[d];
  return false;
}

static inline bool memed(bits const state) {
  unsigned long long const i = (state.to_ullong() * 2038074743) % mem_size;
  bool const already_in = mem[i] == state;
  mem[i] = state;
  return already_in;
}

static inline void mem_clear() {
  memset(reinterpret_cast<void *>(&mem[0]), 0, sizeof(mem[0]) * mem_size);
}

static inline bool iddfs(bits state, val const res, val const rem, val const d) {
  if (rem == 0)
    return true;
  if (memed(state))
    return false;
  if (rem > d)
    return false;
  num[d] = res;
  bits was_processed = state;
  defrd_size[d] = 0;
  for (val k = d; k <= start_d; ++k) {
    val const i = num[k];
    for (val j = start_d; j > k; --j) {
      val const rhs = num[j];
      val const sum = i + rhs;
      if (sum <= max_n && process(was_processed, state, sum, rem, d, add, i, rhs))
        return true;
      val const diff = abs(i - rhs);
      if (process(was_processed, state, diff, rem, d, sub, max(i, rhs), min(i, rhs)))
        return true;
    }
    for (val j = 2*i, p = 1; j <= max_n; j *= 2, ++p)
      if (process(was_processed, state, j, rem, d, shift, i, p))
        return true;
  }
  for (val l = 0; l < defrd_size[d]; ++l) {
    action const &defrd = deferred[d][l];
    state.set(defrd.res);
    if(iddfs(state, defrd.res, rem, d - 1)) {
      result[d] = defrd;
      return true;
    }
    state.reset(defrd.res);
  }
  return false;
}

static void solve(istream &in, ostream &out) {
  in >> n;
  for (short i = 0, a; i < n; ++i) {
    in >> a;
    target.set(a);
  }
  for (start_d = n; start_d < max_n; ++start_d) {
    if (!iddfs(bits(2), 1, n, start_d)) {
      mem_clear();
      continue;
    }
    out << static_cast<int>(start_d) << endl;
    for (val i = start_d; i > 0; --i) {
      action const &act = result[i];
      out << static_cast<int>(act.lhs)
          << act.name
          << static_cast<int>(act.rhs)
          << "\n";
    }
    break;
  }
}

int main() {
  ifstream in("computer.in");
  ofstream out("computer.out");
  solve(in, out);
  return 0;
}
