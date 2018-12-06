
#pragma once

#include <iterator>
#include <random>
#include <utility>

// ------- Non-modifying sequence operations
// copy, copy-if, copy-n, copy-backward
// move, move-backward
// fill, fill-n
// transform
// generate, generate-n
// remove, remove-if, remove-copy, remove-copy-if
// replace, replace-if, replace-copy, replace-copy-if
// iter-swap
// swap-ranges
// reverse, reverse-copy
// rotate, rotate-copy
// shift-left, shift-right
// shuffle
// SKIPPED <sample>
// unique

namespace learn_std
{
// --------------------------------------------------------------------- copy-if

template<class InputIt, class OutputIt, class UnaryPredicate>
constexpr OutputIt
copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred)
{
   for(auto ii = first; ii != last; ++ii)
      if(pred(*ii)) *d_first++ = *ii;
   return d_first;
}

// ------------------------------------------------------------------------ copy
template<class InputIt, class OutputIt>
constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
   return learn_std::copy_if(first, last, d_first, [](auto) { return true; });
}

// ---------------------------------------------------------------------- copy-n
template<class InputIt, class Size, class OutputIt>
constexpr OutputIt copy_n(InputIt first, Size count, OutputIt result)
{
   for(Size i = 0; i < count; ++i) *result++ = *first++;
   return result;
}

// --------------------------------------------------------------- copy-backward
template<class BidirIt1, class BidirIt2>
constexpr BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
   while(last != first) *--d_last = *--last;
   return d_last;
}

// ------------------------------------------------------------------------ move
template<class InputIt, class OutputIt>
constexpr OutputIt move(InputIt first, InputIt last, OutputIt d_first)
{
   if(first == d_first) return last;
   while(first != last) *d_first++ = std::move(*first++);
   return d_first;
}

// --------------------------------------------------------------- move-backward
template<class BidirIt1, class BidirIt2>
constexpr BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
   while(last != first) *--d_last = std::move(*--last);
   return d_last;
}

// ------------------------------------------------------------------------ fill
template<class ForwardIt, class T>
constexpr void fill(ForwardIt first, ForwardIt last, const T& value)
{
   while(first != last) *first++ = value;
}

// ---------------------------------------------------------------------- fill-n
template<class OutputIt, class Size, class T>
constexpr OutputIt fill_n(OutputIt first, Size count, const T& value)
{
   for(Size i = 0; i < count; ++i) *first++ = value;
   return first;
}

// ------------------------------------------------------------------- transform
template<class InputIt, class OutputIt, class UnaryOperation>
constexpr OutputIt transform(InputIt first1,
                             InputIt last1,
                             OutputIt d_first,
                             UnaryOperation unary_op)
{
   while(first1 != last1) *d_first++ = unary_op(*first1++);
   return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt, class BinaryOperation>
constexpr OutputIt transform(InputIt1 first1,
                             InputIt1 last1,
                             InputIt2 first2,
                             OutputIt d_first,
                             BinaryOperation binary_op)
{
   while(first1 != last1) *d_first++ = binary_op(*first1++, *first2++);
   return d_first;
}

// -------------------------------------------------------------------- generate
template<class ForwardIt, class Generator>
constexpr void generate(ForwardIt first, ForwardIt last, Generator g)
{
   while(first != last) *first++ = g();
}

// ------------------------------------------------------------------ generate-n
template<class OutputIt, class Size, class Generator>
constexpr OutputIt generate_n(OutputIt first, Size count, Generator g)
{
   for(Size i = 0; i < count; ++i) *first++ = g();
   return first;
}

// ------------------------------------------------------------------- remove-if
template<class ForwardIt, class UnaryPredicate>
constexpr ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
   auto write = first;
   while(first != last) {
      if(!p(*first)) {
         if(write != first) *write = std::move(*first);
         ++write;
      }
      ++first;
   }
   return write;
}

// ---------------------------------------------------------------------- remove
template<class ForwardIt, class T>
constexpr ForwardIt remove(ForwardIt first, ForwardIt last, const T& value)
{
   return learn_std::remove_if(
       first, last, [&](auto& a) { return a == value; });
}

// -------------------------------------------------------------- remove-copy-if
template<class InputIt, class OutputIt, class UnaryPredicate>
constexpr OutputIt
remove_copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate p)
{
   for(auto ii = first; ii != last; ++ii)
      if(!p(*ii)) *d_first++ = *ii;
   return d_first;
}

// ----------------------------------------------------------------- remove-copy
template<class InputIt, class OutputIt, class T>
constexpr OutputIt
remove_copy(InputIt first, InputIt last, OutputIt d_first, const T& value)
{
   return learn_std::remove_copy_if(
       first, last, d_first, [&](auto& a) { return a == value; });
}

// ------------------------------------------------------------------ replace-if
template<class ForwardIt, class UnaryPredicate, class T>
constexpr void replace_if(ForwardIt first,
                          ForwardIt last,
                          UnaryPredicate p,
                          const T& new_value)
{
   for(auto ii = first; ii != last; ++ii)
      if(p(*ii)) *ii = new_value;
}

// --------------------------------------------------------------------- replace
template<class ForwardIt, class T>
constexpr void
replace(ForwardIt first, ForwardIt last, const T& old_value, const T& new_value)
{
   return learn_std::replace_if(
       first, last, [&](auto& a) { return a == old_value; }, new_value);
}

// ------------------------------------------------------------- replace-copy-if
template<class InputIt, class OutputIt, class UnaryPredicate, class T>
constexpr OutputIt replace_copy_if(InputIt first,
                                   InputIt last,
                                   OutputIt d_first,
                                   UnaryPredicate p,
                                   const T& new_value)
{
   for(auto ii = first; ii != last; ++ii) *d_first++ = p(*ii) ? new_value : *ii;
   return d_first;
}

// ---------------------------------------------------------------- replace-copy
template<class InputIt, class OutputIt, class T>
constexpr OutputIt replace_copy(InputIt first,
                                InputIt last,
                                OutputIt d_first,
                                const T& old_value,
                                const T& new_value)
{
   return learn_std::replace_copy_if(first,
                                     last,
                                     d_first,
                                     [&](auto& a) { return a == old_value; },
                                     new_value);
}

// ------------------------------------------------------------------- iter-swap
template<class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
{
   using std::swap;
   if(a != b) swap(*a, *b);
}

// ----------------------------------------------------------------- swap-ranges
template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt2
swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
{
   while(first1 != last1) learn_std::iter_swap(first1++, first2++);
   return first2;
}

// ---------------------------------------------------------------- reverse-copy
template<class BidirIt> constexpr void reverse(BidirIt first, BidirIt last)
{
   while(first != last) {
      learn_std::iter_swap(first, --last);
      if(first++ == last) break;
   }
}

// ---------------------------------------------------------------- reverse-copy
template<class BidirIt, class OutputIt>
constexpr OutputIt reverse_copy(BidirIt first, BidirIt last, OutputIt d_first)
{
   while(first != last) *d_first++ = *--last;
   return d_first;
}

// ---------------------------------------------------------------------- rotate
// return The iterator equal to first + (last - n_first)
template<class ForwardIt>
constexpr ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
{
   if(first == n_first) return last;
   if(n_first == last) return first;

   auto read      = n_first;
   auto write     = first;
   auto next_read = first; // read position for when "read" hits "last"

   while(read != last) {
      if(write == next_read) next_read = read; // track where "first" went
      learn_std::iter_swap(write++, read++);
   }
   learn_std::rotate(write, next_read, last);
   return write;
}

// return The iterator equal to first + (last - n_first)
template<class ForwardIt>
constexpr ForwardIt rotate_(ForwardIt first, ForwardIt n_first, ForwardIt last)
{
   if(first == n_first) return last;
   if(n_first == last) return first;

   auto read      = n_first;
   auto write     = first;
   auto next_read = first; // read position for when "read" hits "last"
   auto ret_value = last;

   while(write != last) {
      if(write == next_read) next_read = read; // track where "first" went
      learn_std::iter_swap(write++, read++);
      if(read == last) {
         if(ret_value == last)
            ret_value = write; // The first time wrapping: save the return value
         // The remaining elements need to be rotated into place.
         // One could recursively call: rotate(write, next_read, last).
         // The while loop acheives the same effect.
         read = next_read;
         if(read == ret_value) {
            // no need to continue. We've wrapped in the special case
            // where the remaining elements are already in order
            break;
         }
      }
   }

   return ret_value;
}

// ----------------------------------------------------------------- rotate-copy
template<class ForwardIt, class OutputIt>
constexpr OutputIt rotate_copy(ForwardIt first,
                               ForwardIt n_first,
                               ForwardIt last,
                               OutputIt d_first)
{
   d_first = learn_std::copy(n_first, last, d_first);
   return learn_std::copy(first, n_first, d_first);
}

// ------------------------------------------------------------------ shift-left
template<class ForwardIt>
constexpr ForwardIt
shift_left(ForwardIt first,
           ForwardIt last,
           typename std::iterator_traits<ForwardIt>::difference_type n)
{
   // advance 'first' n spots
   auto mid = first;
   for(decltype(n) i = 0; i < n and mid != last; ++i) ++mid;
   // Move the range [mid, last) to the beginning
   return learn_std::move(mid, last, first);
}

// ----------------------------------------------------------------- shift-right
template<class ForwardIt>
constexpr ForwardIt
shift_right(ForwardIt first,
            ForwardIt last,
            typename std::iterator_traits<ForwardIt>::difference_type n)
{
   // Use 'rotate', because we've got ForwardIt
   if(n <= 0) return first;
   decltype(n) len = std::distance(first, last);
   if(n >= len) return last;
   auto ii = std::next(first, len - n);
   learn_std::rotate(first, ii, last);
   return std::next(first, n);
}

// --------------------------------------------------------------------- shuffle
template<class RandomIt, class URBG>
void shuffle(RandomIt first, RandomIt last, URBG&& g)
{
   std::uniform_int_distribution<decltype(last - first)> uniform;
   using param_type = typename decltype(uniform)::param_type;

   for(auto i = last - first - 1; i > 0; --i)
      learn_std::iter_swap(first + i, first + uniform(g, param_type(0, i)));
}

// ---------------------------------------------------------------------- unique
template<class ForwardIt, class BinaryPredicate>
constexpr ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p)
{
   if(first == last) return last;
   for(auto read = std::next(first); read != last; ++read)
      if(!p(*first, *read))
         if(++first != read) *first = std::move(*read);
   return ++first;
}

template<class ForwardIt>
constexpr ForwardIt unique(ForwardIt first, ForwardIt last)
{
   return learn_std::unique(
       first, last, [&](auto& a, auto& b) { return a == b; });
}

// ----------------------------------------------------------------- unique-copy
template<class InputIt, class OutputIt, class BinaryPredicate>
constexpr OutputIt
unique_copy(InputIt first, InputIt last, OutputIt d_first, BinaryPredicate p)
{
   if(first == last) return d_first;
   *d_first = std::move(*first++);
   for(auto read = first; read != last; ++read)
      if(!p(*d_first, *read)) *++d_first = std::move(*read);
   return ++d_first;
}

template<class InputIt, class OutputIt>
constexpr OutputIt unique_copy(InputIt first, InputIt last, OutputIt d_first)
{
   return learn_std::unique_copy(
       first, last, d_first, [&](auto& a, auto& b) { return a == b; });
}

} // namespace learn_std
