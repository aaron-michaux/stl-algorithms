
#pragma once

// ------- Permutation Operations
// is_permutation
// next_permutation, prev_permutation

#include <iterator>
#include <tuple>
#include <utility>

#include "heap-operations.hxx"
#include "modifying-sequence-operations.hxx"
#include "non-modifying-sequence-operations.hxx"
#include "partitioning-operations.hxx"

namespace learn_std
{
// -------------------------------------------------------------- is permutation
template<class ForwardIt1, class ForwardIt2, class BinaryPredicate>
constexpr bool is_permutation(ForwardIt1 first1,
                              ForwardIt1 last1,
                              ForwardIt2 first2,
                              ForwardIt2 last2,
                              BinaryPredicate p)
{
   // Skip common elements at beginning of both sequences
   std::tie(first1, first2) = learn_std::mismatch(first1, last1, first2, last2);
   for(auto ii = first1; ii != last1; ++ii) {
      auto u = [&](auto& a) { return p(*ii, a); };     // curried predicate.
      if(ii != std::find_if(first1, ii, u)) continue;  // already counted.
      auto c1 = learn_std::count_if(ii, last1, u);     // Count in [ii, l1)
      auto c2 = learn_std::count_if(first2, last2, u); //   and in [f2, l2)
      if(c1 != c2) return false;                       // should match.
   }
   return true;
}

template<class ForwardIt1, class ForwardIt2>
constexpr bool is_permutation(ForwardIt1 first1,
                              ForwardIt1 last1,
                              ForwardIt2 first2,
                              ForwardIt2 last2)
{
   return learn_std::is_permutation(
       first1, last1, first2, last2, [](auto& a, auto& b) { return a == b; });
}

template<class ForwardIt1, class ForwardIt2, class BinaryPredicate>
constexpr bool is_permutation(ForwardIt1 first1,
                              ForwardIt1 last1,
                              ForwardIt2 first2,
                              BinaryPredicate p)
{
   return learn_std::is_permutation(
       first1,
       last1,
       first2,
       std::next(first2, std::distance(first1, last1)),
       p);
}

template<class ForwardIt1, class ForwardIt2>
constexpr bool
is_permutation(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
{
   return learn_std::is_permutation(
       first1, last1, first2, [](auto& a, auto& b) { return a == b; });
}

// ------------------------------------------------------------ next permutation
template<class BidirIt, class Compare>
constexpr bool next_permutation_(BidirIt first, BidirIt last, Compare comp)
{
   if(first == last || std::next(first) == last) return false;

   auto ii = std::prev(std::prev(last));
   do {
      auto pp = std::next(ii);
      printf("=>");
      print3(first, last, ii, pp);
      if(comp(*ii, *pp)) {
         printf(". ");
         print2(first, last, ii);
         auto jj = last;
         while(!comp(*ii, *--jj))
            ;
         learn_std::iter_swap(ii, jj);
         learn_std::reverse(pp, last);
         return true;
      }
   } while(ii-- != first);

   learn_std::reverse(first, last);
   return false;
}

template<class BidirIt, class Compare>
bool next_permutation(BidirIt first, BidirIt last, Compare comp)
{
   if(first == last || std::next(first) == last) return false;

   BidirIt ii = std::prev(last);
   while(ii-- != first) {
      auto pp = std::next(ii);
      if(comp(*ii, *pp)) {
         auto jj = last;
         while(!comp(*ii, *--jj))
            ;
         std::iter_swap(ii, jj);
         std::reverse(pp, last);
         return true;
      }
   }

   std::reverse(first, last);
   return false;
}

template<class BidirIt>
constexpr bool next_permutation(BidirIt first, BidirIt last)
{
   return learn_std::next_permutation(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------ prev permutation
template<class BidirIt, class Compare>
constexpr bool prev_permutation(BidirIt first, BidirIt last, Compare comp)
{
   if(first == last || std::next(first) == last) return false;

   auto ii = std::prev(last);
   while(ii-- != first) {
      auto pp = std::next(ii);
      if(comp(*pp, *ii)) {
         auto jj = last;
         while(!comp(*--jj, *ii))
            ;
         learn_std::iter_swap(ii, jj);
         learn_std::reverse(pp, last);
         return true;
      }
   }

   learn_std::reverse(first, last);
   return false;
}

template<class BidirIt>
constexpr bool prev_permutation(BidirIt first, BidirIt last)
{
   return learn_std::prev_permutation(
       first, last, [](auto& a, auto& b) { return a < b; });
}

} // namespace learn_std
