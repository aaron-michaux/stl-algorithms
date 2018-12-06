
#pragma once

#include <iterator>
#include <random>
#include <utility>

#include "modifying-sequence-operations.hxx"
#include "non-modifying-sequence-operations.hxx"

// ------- Partitioning operations
// is-partitioned
// partition, partition-copy
// stable-partition
// partition-point

namespace learn_std
{
// -------------------------------------------------------------- is-partitioned
template<class InputIt, class UnaryPredicate>
constexpr bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p)
{
   auto mid = learn_std::find_if_not(first, last, p); // first false
   return learn_std::none_of(mid, last, p);
}

// ------------------------------------------------------------------- partition
template<class ForwardIt, class UnaryPredicate>
constexpr ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
   auto write = learn_std::find_if_not(first, last, p);
   if(write == last) return last;
   for(auto read = std::next(write); read != last; ++read)
      if(p(*read)) learn_std::iter_swap(read, write++);
   return write;
}

// -------------------------------------------------------------- partition-copy
template<class InputIt, class OutputIt1, class OutputIt2, class UnaryPredicate>
constexpr std::pair<OutputIt1, OutputIt2>
partition_copy(InputIt first,
               InputIt last,
               OutputIt1 d_first_true,
               OutputIt2 d_first_false,
               UnaryPredicate p)
{
   for(auto ii = first; ii != last; ++ii)
      if(p(*ii))
         *d_first_true++ = *ii;
      else
         *d_first_false++ = *ii;
   return std::make_pair(d_first_true, d_first_false);
}

// ------------------------------------------------------------ stable-partition
// O(n) applications of p
// O(n log n) swaps
// Returns Iterator to the first element of the second group
template<class BidirIt, class UnaryPredicate>
BidirIt stable_partition(BidirIt first, BidirIt last, UnaryPredicate p)
{
   auto len = std::distance(first, last); // okay asymptotically.

   // Base cases
   if(len == 0) return last;
   if(len == 1) return p(*first) ? last : first; // apply predicate once

   // Partition the smaller ranges
   auto mid = std::next(first, len / 2);
   auto r1  = learn_std::stable_partition(first, mid, p);
   auto r2  = learn_std::stable_partition(mid, last, p);

   // Merge the two ranges with a rotate
   return learn_std::rotate(r1, mid, r2);
}

// ------------------------------------------------------------- partition-point

template<class ForwardIt, class UnaryPredicate>
constexpr ForwardIt
partition_point(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
   using category = typename std::iterator_traits<ForwardIt>::iterator_category;
   if constexpr(std::is_same_v<category, std::random_access_iterator_tag>) {
      while(first != last) { // do a binary search
         auto mid = std::next(first, (last - first) / 2);
         if(p(*mid))
            first = std::next(mid);
         else
            last = mid;
      }
      return first;
   }
   return learn_std::find_if_not(first, last, p);
}

} // namespace learn_std
