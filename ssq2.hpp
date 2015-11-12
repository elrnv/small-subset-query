#pragma once
#include "defines.hpp"

using std::make_pair;
using std::tie;

// set intersection

namespace ssq2
{
  static seti workspace_set;

  inline void
  intersect( const seti& set1, const seti& set2, seti& result )
  {
    if ( set1.size() <= set2.size() )
    {
      result.clear();
      for ( auto const &element : set1 )
      {
        if ( set2.count( element ) ) // set2.find(element) != set2.end()
          result.insert( element );
      }
    }
    else
      intersect( set2, set1, result );
  }

  vsi
  initialize(const vvi& P, int m)
  {
    vsi T;
    T.resize(m);

    for ( auto i = 0; i < P.size(); ++i )
    {
      if ( P[i].empty() ) continue;
      const auto& L = P[i];
      for ( auto j = 0; j < L.size(); ++j )
        T[ L[j] ].insert(i);
    }

    return T;
  }

  bool query(const vsi& T, const vi& Q)
  {
    if ( Q.empty() ) return true;
    seti S = T[ Q[0] ];
    if ( Q.size() == 1 ) return !S.empty();

    int num = Q.size();
    for ( int i = 1; i < num; ++i )
    {
      intersect( S, T[ Q[i] ], workspace_set );
      S = workspace_set;
      if ( S.empty() )
        return false;
    }
    return !S.empty();
  }
};
