#pragma once
#include "defines.hpp"

using std::make_pair;
using std::tie;

// binary search

namespace ssq1
{
  vvpi initialize(const vvi& P, int m)
  {
    vvpi T;
    T.resize(m);

    for ( auto i = 0; i < P.size(); ++i )
    {
      if ( P[i].empty() ) continue;
      auto L = P[i];
      sort(L.begin(), L.end());
      for ( auto j = 0; j < L.size()-1; ++j )
        T[ L[j] ].push_back(make_pair(i, L[j+1]));
      T[ L.back() ].push_back(make_pair(i, -1));
    }

    return T;
  }

  bool query(const vvpi& T, const vi& Qin)
  {
    if ( Qin.empty() ) return true;
    if ( Qin.size() == 1 ) return !T[ Qin[0] ].empty();
    auto Q = Qin; // copy the query
    sort(Q.begin(), Q.end());

    for ( auto p : T[ Q[0] ] )
    {
      int i, k; tie(i, k) = p;
      int s = 1;

      while ( k <= Q[s] && k > -1 )
      {
        s += k == Q[s];
        if ( s == Q.size() )
          return true;

        auto it = lower_bound( T[k].begin(), T[k].end(), i,
            [](const pi& x, int val) -> bool { return x.first < val; } );
        if (it == T[k].end() || it->first != i)
          break;

        k = it->second;
      }
    }

    return false;
  }
};
