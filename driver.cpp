#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <string>
#include <ctime>
#include <GetOpt.h>
#include "defines.hpp"

#if defined(METHOD1)
#include "ssq1.hpp"
using namespace ssq1;
#elif defined(METHOD2)
#include "ssq2.hpp"
using namespace ssq2;
#elif defined(METHOD3)
#include "ssq3.hpp"
using namespace ssq3;
#elif defined(METHOD4)
#include "ssq4.hpp"
using namespace ssq4;
#elif defined(METHOD5)
#include "ssq5.hpp"
using namespace ssq5;
#else
#include "ssq6.hpp"
using namespace ssq6;
#endif

using namespace std;

inline int get_max_subset_size(const vvi& P)
{
  int M = numeric_limits<int>::min();
  for ( auto v : P )
    M = max(M, int(v.size()));
  return M;
}

inline int get_overlap(const vvi& P, int m)
{
  assert(m > 0);
  vi Ns(m, 0);
  for ( auto v : P )
    for ( auto a : v )
      Ns[a] += 1;
  return *max_element(Ns.begin(), Ns.end());
}

inline int get_max(const vvi& P)
{
  int m = numeric_limits<int>::min();
  for ( auto v : P )
    for ( auto a : v )
      m = max(m, a);
  return m;
}

inline void printpowerset(const vvi& P)
{
  cerr << "P = ";
  for ( auto v : P )
  {
    cerr << " { ";
    for ( auto a : v )
      cerr << a << " ";
    cerr << "} ";
  }

  cerr << endl;
}

void printtable(const vvpi& T)
{
  cerr << "T1 =\n"; 
  for ( int i = 0; i < T.size(); ++i )
  {
    const auto& v = T[i];
    cerr << i << " = [ "; 
    for ( auto a : v )
      cerr << "(" << a.first << ", " << a.second << ") "; 
    cerr << "]\n"; 
  }

  cerr << endl;
}
void printtable(const vsi& T)
{
  cerr << "T2 = "; 
  for ( auto s : T )
  {
    cerr << " [ "; 
    for ( auto a : s )
      cerr << a << " "; 
    cerr << "] "; 
  }

  cerr << endl;
}

void printtable(const vvi& T)
{
  cerr << "T4 = "; 
  for ( auto s : T )
  {
    cerr << " [ "; 
    for ( auto a : s )
      cerr << a << " "; 
    cerr << "] "; 
  }

  cerr << endl;
}

inline void printset(const seti& L, string name)
{
  cerr << name << " = { "; 
  for ( auto l : L )
    cerr << l << " "; 
  cerr << "} " << endl; 
}
inline void printvec(const vi& V, string name)
{
  cerr << name << " = [ "; 
  for ( auto v : V )
    cerr << v << " "; 
  cerr << "] " << endl; 
}

inline void remove_duplicates(vi& v)
{
  sort(v.begin(), v.end());
  v.erase( unique(v.begin(), v.end()), v.end() );
  random_shuffle(v.begin(), v.end());
}

inline vvi load_sets_from_file(const string& filename)
{
  vvi F; // faces
  ifstream fin;
  fin.open(filename);

  if ( !fin.good() )
    exit(1);

  char buf[1024];

  int i = 0;
  while ( !fin.eof() )
  {
    fin.getline(buf,256);
    stringstream ss;
    ss << buf;
    string name;
    ss >> name;
    if ( name.compare("f") )
      continue;
    F.push_back(vi());
    while (ss.good())
    {
      char c = ss.peek();
      if ( c != '/' )
      {
        unsigned int x;
        ss >> x;
        F[i].push_back(x);
      }
      else
        ss.ignore(256, ' ');
    }

    i += 1;
  }

  fin.close();

  // convert vector of faces F to vector of points P that represent all the
  // adjacent faces
  vvi P; // points
  for (int i = 0; i < F.size(); ++i)
  {
    for ( auto j : F[i] )
    {
      if ( j >= P.size() )
        P.resize(j+1);
      P[j].push_back(i);
    }
  }
  return P;
}

// n - number of sets
// m - size of the universe
// M - maximum size of sets P_i (must be at least 3)
inline vvi generate_random_sets(int n, int m, int M)
{
  vvi P(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < 3+(rand() % M-3); ++j)
      P[i].push_back(rand() % m);

  // remove duplicates
  for (int i = 0; i < n; ++i)
    remove_duplicates(P[i]);
  return P;
}

int main(int argc, const char* argv[])
{
  bool load_file = false;
  string filename;

  if (argc > 1)
  {
    if (!strcmp(argv[1], "h"))
    {
      std::cout << "usage: " << argv[0];
      std::cout << " [f <filename> | <seed> | h]" << std::endl;
      exit(0);
    }
    
    if (!strcmp(argv[1], "f"))
    {
      load_file = true;
      filename = string(argv[2]);
    }
    else
      srand(atoi(argv[1]));
  }
  else
    srand(time(0));

  int n = 1e6; // number of subsets
  int m = 1e6; // size of universe
  int M = 15; // maximum subset size
  vvi P = load_file 
    ? load_sets_from_file(filename) 
    : generate_random_sets(n, m, M);

  m = get_max(P)+1;
  if ( load_file )
  {
    M = get_max_subset_size(P);
    n = P.size();
  }

  int N = get_overlap(P,m);
  cout << "m = " << m << "; n = " << n << "; N = " << N << "; M = " << M << "; ";

  assert( m > 2 ); // enough data
  auto T = initialize(P, m);

  //printpowerset(P);

  int num_success = 0;
  vvi Qs(1e7);
  for ( auto& Q : Qs )
  {
    Q.clear();
    for (int i = 0; i < (rand()%M); ++i)
      Q.push_back(rand() % m);
    remove_duplicates(Q);
  }

  clock_t begin_t = clock();
  for ( const auto& Q : Qs )
    num_success += query(T, Q);
  clock_t end_t = clock();
  double elapsed_secs = double(end_t - begin_t) / CLOCKS_PER_SEC;
  cout  << "result = " << num_success
    << " in " << elapsed_secs << " seconds!" << endl;
}
