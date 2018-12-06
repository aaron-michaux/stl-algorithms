
#pragma once

// ------- Sorting operations
// merge, inplace-merge
// is-sorted, is-sorted-until
// sort, stable-sort
// nth-element
// partial-sort, partial-sort-copy

#include <utility>

#include "heap-operations.hxx"
#include "modifying-sequence-operations.hxx"
#include "non-modifying-sequence-operations.hxx"
#include "partitioning-operations.hxx"

namespace learn_std
{
// ----------------------------------------------------------------------- merge
// The standard function should use moves...
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
constexpr OutputIt merge(InputIt1 first1,
                         InputIt1 last1,
                         InputIt2 first2,
                         InputIt2 last2,
                         OutputIt d_first,
                         Compare comp)
{
   while(first1 != last1 and first2 != last2)
      if(comp(*first1, *first2))
         *d_first++ = *first1++;
      else
         *d_first++ = *first2++;

   if(first1 == last1) return learn_std::copy(first2, last2, d_first);
   return learn_std::copy(first1, last1, d_first);
}

template<class InputIt1, class InputIt2, class OutputIt>
constexpr OutputIt merge(InputIt1 first1,
                         InputIt1 last1,
                         InputIt2 first2,
                         InputIt2 last2,
                         OutputIt d_first)
{
   return learn_std::merge(
       first1, last1, first2, last2, d_first, [](auto& a, auto& b) {
          return a < b;
       });
}

// --------------------------------------------------------------- inplace-merge

template<class BidirIt, class Compare>
void inplace_merge(BidirIt first, BidirIt middle, BidirIt last, Compare comp)
{
   // base case
   if(std::distance(first, last) <= 1) return;   // nothing to do
   if(middle == first) return;                   // we're done
   if(middle == last) return;                    // also done
   if(std::is_sorted(first, last, comp)) return; // also done

   // ----
   // Move elements such that every element in [first, middle)
   // is <= to every [middle, last)
   //
   // Compare pairs, moving outwards from middle
   auto s1     = std::prev(middle);
   auto s2     = middle;
   auto start1 = s1;
   auto start2 = s2;

   while(comp(*s2, *s1)) {
      start1 = s1--;
      start2 = s2++;
      if(start1 == first or s2 == last) break;
   }

   // Shift start2 as far right as possible
   while(s2 != last and comp(*s2, *start1)) start2 = s2++;
   s1 = start1; // Shift start1 as far left as possible
   while(s1 != first and comp(*start2, *s1)) start1 = s1--;

   start2++;
   middle = learn_std::rotate(start1, middle, start2);

   // ---- Done

   // ---- Now inplace-merge the sub-ranges
   learn_std::inplace_merge(first, start1, middle, comp);
   learn_std::inplace_merge(middle, start2, last, comp);
}

template<class BidirIt>
void inplace_merge(BidirIt first, BidirIt middle, BidirIt last)
{
   learn_std::inplace_merge(
       first, middle, last, [](auto& a, auto& b) { return a < b; });
}

// -------------------------------------------------------------- is-sorted-util
template<class ForwardIt, class Compare>
constexpr ForwardIt
is_sorted_until(ForwardIt first, ForwardIt last, Compare comp)
{
   auto ii = learn_std::adjacent_find(
       first, last, [&](auto& a, auto& b) { return comp(b, a); });
   return ii == last ? last : std::next(ii);
}

template<class ForwardIt>
constexpr ForwardIt is_sorted_until(ForwardIt first, ForwardIt last)
{
   return learn_std::is_sorted_until(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------------- is-sorted
template<class ForwardIt, class Compare>
constexpr bool is_sorted(ForwardIt first, ForwardIt last, Compare comp)
{
   return learn_std::is_sorted_until(first, last, comp) == last;
}

template<class ForwardIt>
constexpr bool is_sorted(ForwardIt first, ForwardIt last)
{
   return learn_std::is_sorted(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------------------ sort

template<class RandomIt, class Compare>
constexpr void sort(RandomIt first, RandomIt last, Compare comp)
{
   auto len = std::distance(first, last);
   if(len < 2) return; // Base case
   const auto pivot = std::prev(last);
   auto mid         = learn_std::partition(
       first, pivot, [&](auto& a) { return comp(a, *pivot); });
   learn_std::iter_swap(mid, pivot);
   learn_std::sort(first, mid, comp);
   learn_std::sort(++mid, last, comp);
}

template<class RandomIt> constexpr void sort(RandomIt first, RandomIt last)
{
   return learn_std::sort(first, last, [](auto& a, auto& b) { return a < b; });
}

// ----------------------------------------------------------------- stable-sort
template<class RandomIt, class Compare>
void stable_sort(RandomIt first, RandomIt last, Compare comp)
{
   auto len = std::distance(first, last);
   if(len < 2) return;
   auto mid = std::next(first, len / 2);
   learn_std::stable_sort(first, mid, comp);
   learn_std::stable_sort(mid, last, comp);
   learn_std::inplace_merge(first, mid, last, comp);
}

template<class RandomIt> void stable_sort(RandomIt first, RandomIt last)
{
   learn_std::stable_sort(first, last, [](auto& a, auto& b) { return a < b; });
}

// ----------------------------------------------------------------- nth-element
// Quickselect algorithm
template<class RandomIt, class Compare>
constexpr void
nth_element(RandomIt first, RandomIt nth, RandomIt last, Compare comp)
{
   while(first != last) {
      auto pivot = std::prev(last);
      auto mid   = learn_std::partition(
          first, pivot, [&](auto& a) { return comp(a, *pivot); });
      learn_std::iter_swap(mid, pivot);
      if(mid == nth) break; // we're done
      if(mid > nth)
         last = mid;
      else
         first = mid;
   }
}

template<class RandomIt>
constexpr void nth_element(RandomIt first, RandomIt nth, RandomIt last)
{
   learn_std::nth_element(
       first, nth, last, [](auto& a, auto& b) { return a < b; });
}

// ---------------------------------------------------------------- partial-sort
template<class RandomIt, class Compare>
constexpr void
partial_sort(RandomIt first, RandomIt middle, RandomIt last, Compare comp)
{
   if(middle != last) learn_std::nth_element(first, --middle, last, comp);
   learn_std::sort(first, middle);
}

template<class RandomIt>
constexpr void partial_sort(RandomIt first, RandomIt middle, RandomIt last)
{
   learn_std::partial_sort(
       first, middle, last, [](auto& a, auto& b) { return a < b; });
}

// ----------------------------------------------------------- partial-sort-copy
template<class InputIt, class RandomIt, class Compare>
constexpr RandomIt partial_sort_copy(InputIt first,
                                     InputIt last,
                                     RandomIt d_first,
                                     RandomIt d_last,
                                     Compare comp)
{
   if(d_first == d_last) return d_last;

   auto read  = first;
   auto write = d_first;
   while(read != last and write != d_last) { *write++ = *read++; }

   // (1) make a heap from [d_first, d_last)
   // (2) push remaining elements
   // (3) sort the result
   learn_std::make_heap(d_first, write, comp);

   while(read != last) {
      if(comp(*read, *d_first)) {
         *d_first = *read;
         detail::heap_sift_down(d_first, write, comp);
      }
      ++read;
   }

   learn_std::sort_heap(d_first, write, comp);

   // Add elements from first to the heap
   return write;
}

template<class InputIt, class RandomIt>
constexpr RandomIt partial_sort_copy(InputIt first,
                                     InputIt last,
                                     RandomIt d_first,
                                     RandomIt d_last)
{
   return learn_std::partial_sort_copy(
       first, last, d_first, d_last, [](auto& a, auto& b) { return a < b; });
}

} // namespace learn_std
