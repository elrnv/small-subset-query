#pragma once
#include <type_traits>
#include "defines.hpp"
#include "fast_set_intersection.hpp"

// same as ssq5 but using a fancy Baeza-Yates set intersection algorithm with
// interpolation search

namespace ssq6
{
  using std::make_pair;
  using std::tie;
  using std::back_inserter;
  using namespace util;

  static vi workspace_vec;

  pvi
  initialize(const vvi& P, int m)
  {
    vvi t;
    t.resize(m);
    int total_size = 0;
    for ( auto i = 0; i < P.size(); ++i )
      total_size += P[i].size();

    int avg_size = total_size / P.size();

    for ( auto i = 0; i < m; ++i )
      t[i].reserve(avg_size);

    for ( auto i = 0; i < P.size(); ++i )
    {
      if ( P[i].empty() ) continue;
      const auto& L = P[i];
      for ( auto j = 0; j < L.size(); ++j )
        t[ L[j] ].push_back(i);
    }

    pvi T;
    T.first.resize(m+1);
    T.second.resize(total_size);
    int k = 0;
    for ( int i = 0; i < m; ++i )
    {
      T.first[i] = k;
      for ( auto& val : t[i] )
        T.second[k++] = val;
    }
    T.first[m] = k; // also add the end

    return T;
  }

  bool query(const pvi& T, const vi& Q)
  {
    if ( Q.empty() ) return true;
    vi S( T.second.begin() + T.first[ Q[0] ], T.second.begin() + T.first[ Q[0]+1 ]);
    if ( Q.size() == 1 ) return !S.empty();

    int num = Q.size();
    for ( int i = 1; i < num; ++i )
    {
      const auto begin = T.second.begin() + T.first[ Q[i] ];
      const auto end = T.second.begin() + T.first[ Q[i]+1 ];
      workspace_vec.clear();

      baeza_intersect<interpolation_probe>(S.cbegin(), S.cend(), begin, end,
          back_inserter(workspace_vec));

      S = workspace_vec;

      if ( S.empty() )
        return false;
    }
    return !S.empty();
  }
};
