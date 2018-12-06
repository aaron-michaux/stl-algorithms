
#pragma once

#include <iterator>

// ------- Non-modifying sequence operations
// all_of, any_of, none_of
// for_each, for_each_n
// count, count_if
// mismatch
// find, find_if, find_if_not
// search, find_end,
// find_first_of
// adjacent_find
// search_n

namespace learn_std
{
// ---------------------------------------------------------------------- All of
template<class InputIt, class UnaryPredicate>
constexpr bool all_of(InputIt first, InputIt last, UnaryPredicate p)
{
   while(first != last)
      if(!p(*first++)) return false;
   return true; // true if range is empty
}

// ---------------------------------------------------------------------- Any of
template<class InputIt, class UnaryPredicate>
constexpr bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
   while(first != last)
      if(p(*first++)) return true;
   return false; // false if range is empty
}

// --------------------------------------------------------------------- None of
template<class InputIt, class UnaryPredicate>
constexpr bool none_of(InputIt first, InputIt last, UnaryPredicate p)
{
   return !learn_std::any_of(first, last, p); // true if range empty
}

// -------------------------------------------------------------------- For Each
template<class InputIt, class UnaryFunction>
constexpr UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
{
   while(first != last) f(*first++);
   return f;
}

// ------------------------------------------------------------------ For Each N
template<class InputIt, class Size, class UnaryFunction>
constexpr InputIt for_each_n(InputIt first, Size n, UnaryFunction f)
{
   for(Size i = 0; i < n; ++i) f(*first++);
   return first;
}

// ----------------------------------------------------------------------- Count
template<class InputIt, class T>
constexpr typename std::iterator_traits<InputIt>::difference_type
count(InputIt first, InputIt last, const T& value)
{
   typename std::iterator_traits<InputIt>::difference_type counter = 0;
   while(first != last)
      if(*first++ == value) counter++;
   return counter;
}

// -------------------------------------------------------------------- Count If
template<class InputIt, class UnaryPredicate>
constexpr typename std::iterator_traits<InputIt>::difference_type
count_if(InputIt first, InputIt last, UnaryPredicate p)
{
   typename std::iterator_traits<InputIt>::difference_type counter = 0;
   while(first != last)
      if(p(*first++)) counter++;
   return counter;
}

// -------------------------------------------------------------------- Mismatch

template<class InputIt1, class InputIt2, class BinaryPredicate>
constexpr std::pair<InputIt1, InputIt2>
mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
   while(first1 != last1 and p(*first1, *first2)) {
      ++first1;
      ++first2;
   }
   return std::make_pair(first1, first2);
}

template<class InputIt1, class InputIt2>
constexpr std::pair<InputIt1, InputIt2>
mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
   return learn_std::mismatch(
       first1, last1, first2, [](auto& a, auto& b) { return a == b; });
}

template<class InputIt1, class InputIt2, class BinaryPredicate>
constexpr std::pair<InputIt1, InputIt2> mismatch(InputIt1 first1,
                                                 InputIt1 last1,
                                                 InputIt2 first2,
                                                 InputIt2 last2,
                                                 BinaryPredicate p)
{
   while(first1 != last1 and first2 != last2 and p(*first1, *first2)) {
      ++first1;
      ++first2;
   }
   return std::make_pair(first1, first2);
}

template<class InputIt1, class InputIt2>
constexpr std::pair<InputIt1, InputIt2>
mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
   return learn_std::mismatch(
       first1, last1, first2, last2, [](auto& a, auto& b) { return a == b; });
}

// ------------------------------------------------------------------------ Find
template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value)
{
   while(first != last and !(*first == value)) ++first;
   return first;
}

// --------------------------------------------------------------------- Find If
template<class InputIt, class UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
{
   while(first != last and !p(*first)) ++first;
   return first;
}

// ----------------------------------------------------------------- Find If Not
template<class InputIt, class UnaryPredicate>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate p)
{
   while(first != last and p(*first)) ++first;
   return first;
}

// ---------------------------------------------------------------------- Search
template<class ForwardIt1, class ForwardIt2, class BinaryPredicate>

constexpr ForwardIt1 search(ForwardIt1 first,
                            ForwardIt1 last,
                            ForwardIt2 s_first,
                            ForwardIt2 s_last,
                            BinaryPredicate p)
{
   if(s_first == s_last) return first;
   while(first != last) {
      if(learn_std::mismatch(first, last, s_first, s_last, p).second == s_last)
         return first;
      ++first;
   }
   return first;
}

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt1
search(ForwardIt1 first, ForwardIt1 last, ForwardIt2 s_first, ForwardIt2 s_last)
{
   return learn_std::search(
       first, last, s_first, s_last, [](auto& a, auto& b) { return a == b; });
}

template<class ForwardIt, class Searcher>
constexpr ForwardIt
search(ForwardIt first, ForwardIt last, const Searcher& searcher)
{
   return searcher(first, last).first;
}

template<class ForwardIt, class Searcher, class BinaryPredicate>
constexpr ForwardIt search(ForwardIt first,
                           ForwardIt last,
                           const Searcher& searcher,
                           BinaryPredicate p)
{
   return searcher(first, last, p).first;
}

// -------------------------------------------------------------------- Find End
// Searches for the last occurrence of the sequence [s_first, s_last)
// in the range [first, last).
template<class ForwardIt1, class ForwardIt2, class BinaryPredicate>
constexpr ForwardIt1 find_end(ForwardIt1 first,
                              ForwardIt1 last,
                              ForwardIt2 s_first,
                              ForwardIt2 s_last,
                              BinaryPredicate p)
{
   ForwardIt1 ret = last;
   if(s_first != s_last) {
      while(true) {
         ForwardIt1 ii = learn_std::search(first, last, s_first, s_last, p);
         if(ii == last) {
            break;
         } else {
            ret   = ii;
            first = ret;
            ++first;
         }
      }
   }
   return ret;
}

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt1 find_end(ForwardIt1 first,
                              ForwardIt1 last,
                              ForwardIt2 s_first,
                              ForwardIt2 s_last)
{
   return learn_std::find_end(
       first, last, s_first, s_last, [](auto& a, auto& b) { return a == b; });
}

// --------------------------------------------------------------- Find First Of
// Searches the range [first, last) for any of the elements
// in the range [s_first, s_last).
template<class InputIt, class ForwardIt, class BinaryPredicate>
constexpr InputIt find_first_of(InputIt first,
                                InputIt last,
                                ForwardIt s_first,
                                ForwardIt s_last,
                                BinaryPredicate p)
{
   while(first != last) {
      if(learn_std::any_of(
             s_first, s_last, [&](auto& x) { return p(*first, x); }))
         break;
      ++first;
   }
   return first;
}

template<class InputIt, class ForwardIt>
constexpr InputIt
find_first_of(InputIt first, InputIt last, ForwardIt s_first, ForwardIt s_last)
{
   return learn_std::find_first_of(
       first, last, s_first, s_last, [](auto& a, auto& b) { return a == b; });
}

// --------------------------------------------------------------- Adjacent Find
// Searches the range [first, last) for two consecutive identical elements.
template<class ForwardIt, class BinaryPredicate>
constexpr ForwardIt
adjacent_find(ForwardIt first, ForwardIt last, BinaryPredicate p)
{
   auto tail = next(first);
   while(tail != last) {
      if(p(*first, *tail)) return first;
      ++first;
      ++tail;
   }
   return last;
}

template<class ForwardIt>
constexpr ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
{
   return learn_std::adjacent_find(
       first, last, [](auto& a, auto& b) { return a == b; });
}

// -------------------------------------------------------------------- Search N
template<class ForwardIt, class Size, class T, class BinaryPredicate>
constexpr ForwardIt search_n(ForwardIt first,
                             ForwardIt last,
                             Size count,
                             const T& value,
                             BinaryPredicate p)
{
   if(count == 0) return first;
   ForwardIt tail = first;
   for(Size i = 0; i < count and tail != last; ++i) ++tail; // 'clamped' next
   while(tail != last) {
      if(learn_std::all_of(first, tail, [&](auto& x) { return p(value, x); }))
         return first;
      ++first;
      ++tail;
   }
   return last;
}

template<class ForwardIt, class Size, class T>
constexpr ForwardIt
search_n(ForwardIt first, ForwardIt last, Size count, const T& value)
{
   return learn_std::search_n(
       first, last, count, value, [](auto& a, auto& b) { return a == b; });
}

} // namespace learn_std
