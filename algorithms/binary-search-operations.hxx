
#pragma once

// ------- Binary search operations
// lower-bound, upper-bound
// binary-search
// equal-range

#include <utility>

#include "modifying-sequence-operations.hxx"
#include "non-modifying-sequence-operations.hxx"
#include "partitioning-operations.hxx"

namespace learn_std
{
// ----------------------------------------------------------------- lower-bound
// Find iterater that is NOT less than 'value'
template<class ForwardIt, class T, class Compare>
constexpr ForwardIt
lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
   if(first == last) return last;
   while(first != last) {
      auto mid = std::next(first, std::distance(first, last) / 2);
      if(comp(*mid, value))      // mid < 'value'
         first = std::next(mid); // search up, skipping 'mid': not valid result
      else
         last = mid;
   }
   return first;
}

template<class ForwardIt, class T>
constexpr ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value)
{
   return learn_std::lower_bound(
       first, last, value, [](auto& a, auto& b) { return a < b; });
}

// ----------------------------------------------------------------- upper-bound
// Find iterator that is GREATER than 'value'
template<class ForwardIt, class T, class Compare>
constexpr ForwardIt
upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
   while(first != last) {
      auto mid = std::next(first, std::distance(first, last) / 2);
      if(!comp(value, *mid))     // mid  <= 'value'
         first = std::next(mid); // search up, skipping 'mid': not valid result
      else
         last = mid; // search down
   }
   return first;
}

template<class ForwardIt, class T>
constexpr ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
   return learn_std::upper_bound(
       first, last, value, [](auto& a, auto& b) { return a < b; });
}

// --------------------------------------------------------------- binary-search
template<class ForwardIt, class T, class Compare>
constexpr bool
binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
   auto ii = learn_std::lower_bound(first, last, value, comp);
   // if (ii == value), then comp(value, *ii) will be false;
   return (ii == last) ? false : !comp(value, *ii);
}

template<class ForwardIt, class T>
constexpr bool binary_search(ForwardIt first, ForwardIt last, const T& value)
{
   return learn_std::binary_search(
       first, last, value, [](auto& a, auto& b) { return a < b; });
}

// ----------------------------------------------------------------- equal-range
template<class ForwardIt, class T, class Compare>
constexpr std::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
   return std::make_pair(std::lower_bound(first, last, value, comp),
                         std::upper_bound(first, last, value, comp));
}

template<class ForwardIt, class T>
constexpr std::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first, ForwardIt last, const T& value)
{
   return learn_std::equal_range(
       first, last, value, [](auto& a, auto& b) { return a < b; });
}

} // namespace learn_std
