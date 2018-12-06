
#pragma once

// ------- Set operations
// includes
// set_difference
// set_intersection
// set_symmetric_difference
// set_union

#include <utility>

#include "heap-operations.hxx"
#include "modifying-sequence-operations.hxx"
#include "non-modifying-sequence-operations.hxx"
#include "partitioning-operations.hxx"

namespace learn_std
{
// -------------------------------------------------------------------- includes
template<class InputIt1, class InputIt2, class Compare>
constexpr bool includes(InputIt1 first1,
                        InputIt1 last1,
                        InputIt2 first2,
                        InputIt2 last2,
                        Compare comp)
{
   while(first2 != last2) {
      if(first1 == last1) return false;        // ran out of values
      if(comp(*first2, *first1)) return false; // first2 < first1
      if(!comp(*first1, *first2))
         ++first2; // first1 >= first2  implies first1 == first2
      first1++;
   }
   return true;
}

template<class InputIt1, class InputIt2>
constexpr bool
includes(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
   return learn_std::includes(
       first1, last1, first2, last2, [](auto& a, auto& b) { return a < b; });
}

// -------------------------------------------------------------- set-difference
// Copies the elements from the sorted range [first1, last1)
// which are not found in the sorted range [first2, last2)
// to the range beginning at d_first.
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
constexpr OutputIt set_difference(InputIt1 first1,
                                  InputIt1 last1,
                                  InputIt2 first2,
                                  InputIt2 last2,
                                  OutputIt d_first,
                                  Compare comp)
{
   while(first1 != last1) {
      if(first2 == last2) // [first2,last2) exhausted
         return learn_std::copy(first1, last1, d_first);
      if(comp(*first1, *first2)) {
         *d_first++ = *first1++; // first1 not in [first2, last2)
      } else if(!comp(*first2, *first1)) {
         ++first1; // first1 == first2, so omit
         ++first2;
      } else {
         ++first2; // first2 not in [first1, last1)
      }
   }
   return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt>
constexpr OutputIt set_difference(InputIt1 first1,
                                  InputIt1 last1,
                                  InputIt2 first2,
                                  InputIt2 last2,
                                  OutputIt d_first)
{
   return learn_std::set_difference(
       first1, last1, first2, last2, d_first, [](auto& a, auto& b) {
          return a < b;
       });
}

// ------------------------------------------------------------ set-intersection
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
constexpr OutputIt set_intersection(InputIt1 first1,
                                    InputIt1 last1,
                                    InputIt2 first2,
                                    InputIt2 last2,
                                    OutputIt d_first,
                                    Compare comp)
{
   while(first1 != last1 and first2 != last2) {
      if(comp(*first1, *first2)) {
         ++first1; // first1 not in [first2, last2), so omit
      } else if(!comp(*first2, *first1)) { // first1 == first2
         *d_first++ = *first1++;
         ++first2;
      } else { // first2 not in [first1, last1), so omit
         ++first2;
      }
   }
   return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt>
constexpr OutputIt set_intersection(InputIt1 first1,
                                    InputIt1 last1,
                                    InputIt2 first2,
                                    InputIt2 last2,
                                    OutputIt d_first)
{
   return learn_std::set_intersection(
       first1, last1, first2, last2, d_first, [](auto& a, auto& b) {
          return a < b;
       });
}

// ---------------------------------------------------- set-symmetric-difference
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
constexpr OutputIt set_symmetric_difference(InputIt1 first1,
                                            InputIt1 last1,
                                            InputIt2 first2,
                                            InputIt2 last2,
                                            OutputIt d_first,
                                            Compare comp)
{
   while(first1 != last1) {
      if(first2 == last2) // set2 exhausted, so copy remainder of set1
         return learn_std::copy(first1, last1, d_first);
      if(comp(*first1, *first2)) {
         *d_first++ = *first1++; // first1 not in [first2, last2)
      } else if(!comp(*first2, *first1)) {
         ++first1; // first1 == first2, so omit
         ++first2;
      } else {
         *d_first++ = *first2++; // first2 not in [first1, last1)
      }
   }
   // set1 exhausted, so copy remainder of set2
   return learn_std::copy(first2, last2, d_first);
}

template<class InputIt1, class InputIt2, class OutputIt>
constexpr OutputIt set_symmetric_difference(InputIt1 first1,
                                            InputIt1 last1,
                                            InputIt2 first2,
                                            InputIt2 last2,
                                            OutputIt d_first)
{
   return learn_std::set_symmetric_difference(
       first1, last1, first2, last2, d_first, [](auto& a, auto& b) {
          return a < b;
       });
}

// ------------------------------------------------------------------- set-union
template<class InputIt1, class InputIt2, class OutputIt, class Compare>
constexpr OutputIt set_union(InputIt1 first1,
                             InputIt1 last1,
                             InputIt2 first2,
                             InputIt2 last2,
                             OutputIt d_first,
                             Compare comp)
{
   while(first1 != last1) {
      if(first2 == last2) // set2 exhausted, so copy remainder of set1
         return learn_std::copy(first1, last1, d_first);
      if(comp(*first1, *first2)) {
         *d_first++ = *first1++; // first1 not in [first2, last2)
      } else if(!comp(*first2, *first1)) {
         *d_first++ = *first1++; // first1 == first2, so copy once
         ++first2;               // skip
      } else {
         *d_first++ = *first2++; // first2 not in [first1, last1)
      }
   }
   // set1 exhausted, so copy remainder of set2
   return learn_std::copy(first2, last2, d_first);
}

template<class InputIt1, class InputIt2, class OutputIt>
constexpr OutputIt set_union(InputIt1 first1,
                             InputIt1 last1,
                             InputIt2 first2,
                             InputIt2 last2,
                             OutputIt d_first)
{
   return learn_std::set_union(
       first1, last1, first2, last2, d_first, [](auto& a, auto& b) {
          return a < b;
       });
}

} // namespace learn_std
