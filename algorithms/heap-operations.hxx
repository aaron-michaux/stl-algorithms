
#pragma once

// ------- Heap operations
// is-heap, is-heap-until
// make-heap
// push-heap
// pop-heap
// sort-heap

#include "modifying-sequence-operations.hxx"

namespace learn_std
{
// ---------------------------------------------------------------- is-heap-util
template<class RandomIt, class Compare>
constexpr RandomIt is_heap_until(RandomIt first, RandomIt last, Compare comp)
{
   auto len         = std::distance(first, last);
   decltype(len) i0 = 0;
   decltype(len) i1 = 1;

   while(i1 < len) {
      if(comp(first[i0], first[i1])) return first + i1;
      if(++i1 == len) return last;
      if(comp(first[i0], first[i1])) return first + i1;
      i1 = 2 * ++i0 + 1;
   }

   return first + i1;
}

template<class RandomIt>
constexpr RandomIt is_heap_until(RandomIt first, RandomIt last)
{
   return learn_std::is_heap_until(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// --------------------------------------------------------------------- is-heap
template<class RandomIt, class Compare>
constexpr bool is_heap(RandomIt first, RandomIt last, Compare comp)
{
   return last == learn_std::is_heap_until(first, last, comp);
}

template<class RandomIt> constexpr bool is_heap(RandomIt first, RandomIt last)
{
   return learn_std::is_heap(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------------- push-heap
template<class RandomIt, class Compare>
constexpr void push_heap(RandomIt first, RandomIt last, Compare comp)
{
   if(first == last) return;
   auto child = std::prev(last);
   while(child != first) {
      auto parent = std::next(first, (std::distance(first, child) - 1) / 2);
      if(comp(*parent, *child)) {
         learn_std::iter_swap(parent, child);
         child = parent;
      } else {
         break;
      }
   }
}

template<class RandomIt> constexpr void push_heap(RandomIt first, RandomIt last)
{
   learn_std::push_heap(first, last, [](auto& a, auto& b) { return a < b; });
}

// -------------------------------------------------------------------- pop-heap
namespace detail
{
   template<class RandomIt, class Compare>
   constexpr void heap_sift_down(RandomIt first, RandomIt last, Compare comp)
   {
      auto len    = std::distance(first, last);
      auto parent = first;
      while(parent != last) {
         auto child_idx = std::distance(first, parent) * 2 + 1;
         if(child_idx >= len) break;
         auto child   = std::next(first, child_idx);
         auto r_child = std::next(child);

         // right-child exists and is larger than left child
         if(r_child != last and comp(*child, *r_child)) child = r_child;

         if(comp(*parent, *child)) {
            learn_std::iter_swap(parent, child);
            parent = child;
         } else {
            break; // parent is in heap order
         }
      }
   }
} // namespace detail

template<class RandomIt, class Compare>
constexpr void pop_heap(RandomIt first, RandomIt last, Compare comp)
{
   if(first == last) return;
   learn_std::iter_swap(first, --last);
   detail::heap_sift_down(first, last, comp);
}

template<class RandomIt> constexpr void pop_heap(RandomIt first, RandomIt last)
{
   learn_std::pop_heap(first, last, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------------- make-heap
template<class RandomIt, class Compare>
constexpr void make_heap(RandomIt first, RandomIt last, Compare comp)
{
   if(first == last) return;
   auto ii = first;
   do {
      learn_std::push_heap(first, ++ii, comp);
   } while(ii != last);
}

template<class RandomIt> constexpr void make_heap(RandomIt first, RandomIt last)
{
   learn_std::make_heap(first, last, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------------- sort-heap
template<class RandomIt, class Compare>
constexpr void sort_heap(RandomIt first, RandomIt last, Compare comp)
{
   if(first == last) return;
   while(last != first) { learn_std::pop_heap(first, last--, comp); }
}

template<class RandomIt> constexpr void sort_heap(RandomIt first, RandomIt last)
{
   learn_std::sort_heap(first, last, [](auto& a, auto& b) { return a < b; });
}

} // namespace learn_std
