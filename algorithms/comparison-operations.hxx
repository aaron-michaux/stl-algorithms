
#pragma once

// ------- Comparison operations
// equal
// lexicographical_compare

#include <iterator>
#include <utility>

#include "non-modifying-sequence-operations.hxx"

namespace learn_std
{
// ----------------------------------------------------------------------- equal
template<class InputIt1, class InputIt2, class BinaryPredicate>
constexpr bool
equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
   return learn_std::mismatch(first1, last1, first2, p).first == last1;
}

template<class InputIt1, class InputIt2>
constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
   return learn_std::equal(
       first1, last1, first2, [](auto& a, auto& b) { return a == b; });
}

template<class InputIt1, class InputIt2, class BinaryPredicate>
constexpr bool equal(InputIt1 first1,
                     InputIt1 last1,
                     InputIt2 first2,
                     InputIt2 last2,
                     BinaryPredicate p)
{
   return learn_std::mismatch(first1, last1, first2, last2, p)
          == std::make_pair(last1, last2);
}

template<class InputIt1, class InputIt2>
constexpr bool
equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
   return learn_std::equal(
       first1, last1, first2, last2, [](auto& a, auto& b) { return a == b; });
}

// ----------------------------------------------------- lexicographical_compare
template<class InputIt1, class InputIt2, class Compare>
constexpr bool lexicographical_compare(InputIt1 first1,
                                       InputIt1 last1,
                                       InputIt2 first2,
                                       InputIt2 last2,
                                       Compare comp)
{
   while(first1 != last1 and first2 != last2) {
      if(comp(*first1, *first2)) return true;
      if(comp(*first2, *first1)) return false;
      ++first1; // *first1 == *first2
      ++first2;
   }
   return (first1 == last1) && (first2 != last2);
}

template<class InputIt1, class InputIt2>
constexpr bool lexicographical_compare(InputIt1 first1,
                                       InputIt1 last1,
                                       InputIt2 first2,
                                       InputIt2 last2)
{
   return learn_std::lexicographical_compare(
       first1, last1, first2, last2, [](auto& a, auto& b) { return a < b; });
}

} // namespace learn_std
