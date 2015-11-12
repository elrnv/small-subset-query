#pragma once
#include "fast_set_intersection.hpp"

// same as ssq2 but using vector instead of unordered set

namespace util
{
  // NOTE:
  // This code was taken from https://github.com/erikfrey/themas.

  using std::swap;

  /**
   * interpolation probe: pick the next element to probe by making a jump informed by the difference
   * between the low value, the high value, and the value you are searching
   */
  template< class RandomAccessIterator, class T >
  struct interpolation_probe
  {
    RandomAccessIterator operator()(RandomAccessIterator begin, RandomAccessIterator end, const T & value)
    {
      float a = *begin, b = *(end - 1), x = value;
      return begin + (end - begin - 1) * std::min(1.0f, std::max(0.0f, (x - a) / (b - a) ));
    }
  };

  /**
   * binary probe: pick the next element by choosing the halfway point between low and high
   */
  template< class RandomAccessIterator, class T >
  struct binary_probe
  {
    RandomAccessIterator operator()(RandomAccessIterator begin, RandomAccessIterator end, const T & value)
    {
      return begin + ( (end - begin) >> 1);
    }
  };

  /**
   * lower_bound: like stl's lower_bound but with different kinds of probing
   * note the appearance of the rare template parameter template!
   */
  template< template <class, class> class Probe, class RandomAccessIterator, class T >
  RandomAccessIterator lower_bound(RandomAccessIterator begin, RandomAccessIterator end, const T & value)
  {
    RandomAccessIterator pit;
    Probe< RandomAccessIterator, T > pfunc; // probe-functor (wants to get func'd up)

    while ( begin < end )
    {
      pit = pfunc(begin, end, value);
      if ( *pit < value )
        begin = pit + 1;
      else
        end = pit;
    }
    return begin;
  }

  /*
   * this time with a comparator!
   */
  template< template <class, class> class Probe, class RandomAccessIterator, class T, class Comparator >
  RandomAccessIterator lower_bound(RandomAccessIterator begin, RandomAccessIterator end, const T & value, Comparator cmp)
  {
    RandomAccessIterator pit;
    Probe< RandomAccessIterator, T > pfunc;

    while ( begin < end )
    {
      pit = pfunc(begin, end, value);
      if ( cmp(*pit, value) )
        begin = pit + 1;
      else
        end = pit;
    }
    return begin;
  }

  /*
   * like stl's set_intersect
   */
  template<class InputIterator, class OutputIterator>
  void linear_intersect(InputIterator begin1, InputIterator end1,
                        InputIterator begin2, InputIterator end2,
                        OutputIterator out)
  {
    if ( (end2 - begin2) > (end1 - begin1) )
    {
      // why in the world would i do this?
      // hmmmmmmm.......... !
      swap(begin1, begin2);
      swap(end1, end2);
    }
    while (begin1 != end1 && begin2 != end2)
    {
      if (*begin1 < *begin2)
        ++begin1;
      else if (*begin2 < *begin1)
        ++begin2;
      else
      {
        *out++ = *begin1;
        ++begin1;
        ++begin2;
      }
    }
  }

  /*
   * this time with a comparator!
   */
  template<class InputIterator, class OutputIterator, class Comparator >
  void linear_intersect(InputIterator begin1, InputIterator end1,
                        InputIterator begin2, InputIterator end2,
                        OutputIterator out, Comparator cmp)
  {
    if ( (end2 - begin2) > (end1 - begin1) )
    {
      // why in the world would i do this?
      // hmmmmmmm.......... !
      swap(begin1, begin2);
      swap(end1, end2);
    }
    while (begin1 != end1 && begin2 != end2)
    {
      if (cmp( *begin1, *begin2 ) )
        ++begin1;
      else if ( cmp(*begin2, *begin1) )
        ++begin2;
      else
      {
        *out++ = *begin1;
        ++begin1;
        ++begin2;
      }
    }
  }

  /*
   * baeza_intersect
   */
  template< template <class, class> class Probe,
    class RandomAccessIterator, class OutputIterator>
  void baeza_intersect(RandomAccessIterator begin1, RandomAccessIterator end1,
                       RandomAccessIterator begin2, RandomAccessIterator end2,
                       OutputIterator out)
  {
    RandomAccessIterator probe1, probe2;

    if ( (end1 - begin1) < ( end2 - begin2 ) )
    {
      if ( begin1 == end1 )
        return;
      probe1 = begin1 + ( ( end1 - begin1 ) >> 1 );
      probe2 = lower_bound< Probe >( begin2, end2, *probe1 );
      baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out); // intersect left
      if (! (probe2 == end2 || *probe1 < *probe2 ))
        *out++ = *probe2++;
      baeza_intersect< Probe >(++probe1, end1, probe2, end2, out); // intersect right
    }
    else
    {
      if ( begin2 == end2 )
        return;
      probe2 = begin2 + ( ( end2 - begin2 ) >> 1 );
      probe1 = lower_bound< Probe >( begin1, end1, *probe2 );
      baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out); // intersect left
      if (! (probe1 == end1 || *probe2 < *probe1 ))
        *out++ = *probe1++;
      baeza_intersect< Probe >(probe1, end1, ++probe2, end2, out); // intersect right
    }
  }

  /*
   * with a comparator
   */
  template< template <class, class> class Probe,
    class RandomAccessIterator, class OutputIterator, class Comparator >
  void baeza_intersect(RandomAccessIterator begin1, RandomAccessIterator end1,
                       RandomAccessIterator begin2, RandomAccessIterator end2,
                       OutputIterator out, Comparator cmp)
  {
    RandomAccessIterator probe1, probe2;

    if ( (end1 - begin1) < ( end2 - begin2 ) )
    {
      if ( begin1 == end1 )
        return;
      probe1 = begin1 + ( ( end1 - begin1 ) >> 1 );
      probe2 = lower_bound< Probe >( begin2, end2, *probe1, cmp );
      baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out, cmp); // intersect left
      if (! (probe2 == end2 || cmp( *probe1, *probe2 ) ))
        *out++ = *probe2++;
      baeza_intersect< Probe >(++probe1, end1, probe2, end2, out, cmp); // intersect right
    }
    else
    {
      if ( begin2 == end2 )
        return;
      probe2 = begin2 + ( ( end2 - begin2 ) >> 1 );
      probe1 = lower_bound< Probe >( begin1, end1, *probe2, cmp );
      baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out, cmp); // intersect left
      if (! (probe1 == end1 || cmp( *probe2, *probe1 ) ))
        *out++ = *probe1++;
      baeza_intersect< Probe >(probe1, end1, ++probe2, end2, out, cmp); // intersect right
    }
  }
}; // namespace util
