#pragma once
#include "defines.hpp"

using std::make_pair;
using std::tie;

// same as ssq1 but we replace lists with unordered maps
// so binary search becomes a table lookup

namespace ssq3
{
  vmi initialize(const vvi& P, int m)
  {
    vmi T;
    T.resize(m);

    for ( auto i = 0; i < P.size(); ++i )
    {
      if ( P[i].empty() ) continue;
      auto L = P[i];
      sort(L.begin(), L.end());
      for ( auto j = 0; j < L.size()-1; ++j )
        T[ L[j] ].insert(make_pair(i, L[j+1]));
      T[ L.back() ].insert(make_pair(i, -1));
    }

    return T;
  }

  bool query(const vmi& T, const vi& Qin)
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

        auto it = T[k].find(i);
        if (it == T[k].end())
          break;

        k = it->second;
      }
    }

    return false;
  }
};
