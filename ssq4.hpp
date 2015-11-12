#pragma once
#include "defines.hpp"

using std::make_pair;
using std::tie;
using std::set_intersection;
using std::back_inserter;

// same as ssq2 but using vector instead of unordered set

namespace ssq4
{
  static vi workspace_vec;

  vvi
  initialize(const vvi& P, int m)
  {
    vvi T;
    T.resize(m);

    for ( auto i = 0; i < P.size(); ++i )
    {
      if ( P[i].empty() ) continue;
      const auto& L = P[i];
      for ( auto j = 0; j < L.size(); ++j )
        T[ L[j] ].push_back(i);
    }

    return T;
  }

  bool query(const vvi& T, const vi& Q)
  {
    if ( Q.empty() ) return true;
    vi S = T[ Q[0] ];
    if ( Q.size() == 1 ) return !S.empty();

    for ( int i = 1; i < Q.size(); ++i )
    {
      const auto& TQi = T[ Q[i] ];
      workspace_vec.clear();
      set_intersection( S.begin(), S.end(), TQi.begin(), TQi.end(),
          back_inserter(workspace_vec));
      S = workspace_vec;
      if ( S.empty() )
        return false;
    }
    return !S.empty();
  }
};
