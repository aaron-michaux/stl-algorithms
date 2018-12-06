
#pragma once

// ------- Min/max operations
// max_element, min_element, minmax_element
// max, min, minmax
// clamp

#include <iterator>
#include <utility>

namespace learn_std
{
// ----------------------------------------------------------------- max-element
template<class ForwardIt, class Compare>
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp)
{
   if(first == last) return last;
   auto max_itr = first;
   auto read    = std::next(first);
   while(read != last) {
      if(comp(*max_itr, *read)) max_itr = read;
      ++read;
   }
   return max_itr;
}

template<class ForwardIt>
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last)
{
   return learn_std::max_element(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// ----------------------------------------------------------------- min-element
template<class ForwardIt, class Compare>
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp)
{
   if(first == last) return last;
   auto min_itr = first;
   auto read    = std::next(first);
   while(read != last) {
      if(comp(*read, *min_itr)) min_itr = read;
      ++read;
   }
   return min_itr;
}

template<class ForwardIt>
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last)
{
   return learn_std::min_element(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// -------------------------------------------------------------- minmax-element
template<class ForwardIt, class Compare>
constexpr std::pair<ForwardIt, ForwardIt>
minmax_element(ForwardIt first, ForwardIt last, Compare comp)
{
   if(first == last) return std::make_pair(last, last);
   auto pq = std::make_pair(first, first);
   ++first;

   while(first != last) {
      if(comp(*first, *pq.first)) // first < pq.first, new min
         pq.first = first;
      else if(!comp(*first, *pq.second)) // pq.second >= first, new max
         pq.second = first;
      ++first;
   }

   return pq;
}

template<class ForwardIt>
constexpr std::pair<ForwardIt, ForwardIt> minmax_element(ForwardIt first,
                                                         ForwardIt last)
{
   return learn_std::minmax_element(
       first, last, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------------------- max
template<class T, class Compare>
constexpr const T& max(const T& a, const T& b, Compare comp)
{
   return (comp(a, b)) ? b : a;
}

template<class T> constexpr const T& max(const T& a, const T& b)
{
   return learn_std::max(a, b, [](auto& a, auto& b) { return a < b; });
}

template<class T, class Compare>
constexpr T max(std::initializer_list<T> ilist, Compare comp)
{
   return *std::max_element(beign(ilist), end(ilist), comp);
}

template<class T> constexpr T max(std::initializer_list<T> ilist)
{
   return learn_std::max(ilist, [](auto& a, auto& b) { return a < b; });
}

// ------------------------------------------------------------------------- min
template<class T, class Compare>
constexpr const T& min(const T& a, const T& b, Compare comp)
{
   return (comp(a, b)) ? a : b;
}

template<class T> constexpr const T& min(const T& a, const T& b)
{
   return learn_std::min(a, b, [](auto& a, auto& b) { return a < b; });
}

template<class T, class Compare>
constexpr T min(std::initializer_list<T> ilist, Compare comp)
{
   return *std::min_element(beign(ilist), end(ilist), comp);
}

template<class T> constexpr T min(std::initializer_list<T> ilist)
{
   return learn_std::min(ilist, [](auto& a, auto& b) { return a < b; });
}

// ---------------------------------------------------------------------- minmax
template<class T, class Compare>
constexpr std::pair<const T&, const T&>
minmax(const T& a, const T& b, Compare comp)
{
   return (comp(b, a)) ? std::make_pair(b, a) : std::make_pair(a, b);
}

template<class T>
constexpr std::pair<const T&, const T&> minmax(const T& a, const T& b)
{
   return learn_std::minmax(a, b, [](auto& a, auto& b) { return a < b; });
}

template<class T, class Compare>
constexpr std::pair<T, T> minmax(std::initializer_list<T> ilist, Compare comp)
{
   auto pq = learn_std::minmax_element(begin(ilist), end(ilist), comp);
   return std::make_pair(*pq.first, *pq.second);
}

template<class T>
constexpr std::pair<T, T> minmax(std::initializer_list<T> ilist)
{
   return learn_std::minmax(ilist, [](auto& a, auto& b) { return a < b; });
}

// ----------------------------------------------------------------------- clamp
template<class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
{
   return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template<class T> constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
   return learn_std::clamp(v, lo, hi, [](auto& a, auto& b) { return a < b; });
}

} // namespace learn_std
