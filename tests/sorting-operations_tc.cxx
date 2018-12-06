
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "algorithms/sorting-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

CATCH_TEST_CASE("SortingOperations_", "[sorting-operations]")
{
   std::mt19937 g;
   g.seed(1);
   std::uniform_int_distribution<int> uniform;
   using pt = decltype(uniform)::param_type;

   auto rand = [&](int low, int high) { return uniform(g, pt(low, high)); };

   //
   // ------------------------------------------------- is-sorted-is-sorted-util
   //
   CATCH_SECTION("is-sorted-is-sorted-util")
   {
      g.seed(1);

      auto build_u = [&](unsigned len, unsigned n) {
         std::vector<int> u(len);
         std::generate(begin(u), end(u), [&]() { return rand(10, 20); });
         std::sort(begin(u), end(u));
         if(n < len) *std::next(begin(u), n) = -1; // breaks sorting
         return u;
      };

      auto test_it = [&](const std::vector<int>& u, unsigned n) {
         auto ii = learn_std::is_sorted_until(begin(u), end(u));
         auto jj = std::is_sorted_until(begin(u), end(u));
         CATCH_REQUIRE(ii == jj);
         if(n == 0 or u.size() < 2)
            CATCH_REQUIRE(learn_std::is_sorted(begin(u), end(u)));
         else
            CATCH_REQUIRE(!learn_std::is_sorted(begin(u), end(u)));
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = 0u; n < l; ++n) test_it(build_u(l, n), n);
   }

   //
   // ------------------------------------------------------------ inplace-merge
   //
   CATCH_SECTION("inplace-merge")
   {
      g.seed(1);

      auto op = [&](auto& a, auto& b) {
         if(std::isupper(a) and !std::isupper(b)) return false;
         if(!std::isupper(a) and std::isupper(b)) return true;
         return false;
      };

      auto build_u = [&](unsigned len, unsigned n) {
         std::vector<int> u(len);
         std::generate(begin(u), end(u), [&]() { return rand(10, 20); });
         auto mid = begin(u) + n;
         std::sort(begin(u), mid);
         std::sort(mid, end(u));
         return u;
      };

      auto build_v = [&](unsigned len, unsigned n) {
         std::vector<char> v(len);
         auto mid = begin(v) + n;
         iota(begin(v), mid, 'a');
         iota(mid, end(v), 'A' + n);
         shuffle(begin(v), end(v), g);
         std::sort(begin(v), mid, op);
         std::sort(mid, end(v), op);
         return v;
      };

      auto test_it = [](auto u, auto n, auto op) {
         auto v = u;
         learn_std::inplace_merge(begin(u), begin(u) + n, end(u), op);
         std::stable_sort(begin(v), end(v), op);
         CATCH_REQUIRE(u == v);
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto r = 0u; r < 1000; ++r)
            for(auto i = 0u; i < l; ++i) {
               test_it(
                   build_u(l, i), i, [](auto& a, auto& b) { return a < b; });
               test_it(build_v(l, i), i, op);
            }
   }

   //
   // --------------------------------------------------------------------- sort
   //
   CATCH_SECTION("sort")
   {
      g.seed(1);

      auto build_u = [&](unsigned len) {
         std::vector<char> u(len);
         iota(begin(u), end(u), 'a');
         shuffle(begin(u), end(u), g);
         return u;
      };

      auto test_it = [&](auto u) {
         learn_std::sort(begin(u), end(u));
         CATCH_REQUIRE(std::is_sorted(begin(u), end(u)));
      };

      for(auto l = 0u; l <= 26; ++l)
         for(auto n = 0u; n < 100; ++n) test_it(build_u(l));
   }

   //
   // -------------------------------------------------------------- stable-sort
   //
   CATCH_SECTION("stable-sort")
   {
      g.seed(1);

      auto op
          = [&](auto& a, auto& b) { return std::isupper(a) < std::isupper(b); };

      auto build_u = [&](unsigned len, unsigned n) {
         std::vector<char> u(len);
         iota(begin(u), begin(u) + n, 'a');
         iota(begin(u) + n, end(u), 'A' + n);
         shuffle(begin(u), end(u), g);
         return u;
      };

      auto test_it = [&](auto u) {
         auto v = u;
         std::stable_sort(begin(v), end(v), op);
         learn_std::stable_sort(begin(u), end(u), op);
         CATCH_REQUIRE(u == v);
      };

      for(auto l = 0u; l <= 10; ++l)
         for(auto n = 0u; n < l; ++n) test_it(build_u(l, n));
   }

   //
   // -------------------------------------------------------------- nth-element
   //
   CATCH_SECTION("nth-element")
   {
      g.seed(1);

      auto build_u = [&](auto len) {
         std::vector<char> u(len);
         iota(begin(u), end(u), 'a');
         shuffle(begin(u), end(u), g);
         return u;
      };

      auto test_it = [&](auto u, auto n) {
         auto nth = begin(u) + n;
         learn_std::nth_element(begin(u), nth, end(u));
         CATCH_REQUIRE(*nth == 'a' + char(n));
         CATCH_REQUIRE(
             std::all_of(begin(u), nth, [&](auto& a) { return a < *nth; }));
         CATCH_REQUIRE(
             std::all_of(nth, end(u), [&](auto& a) { return a >= *nth; }));
      };

      for(auto l = 0u; l <= 10; ++l)
         for(auto n = 0u; n < l; ++n)
            for(auto r = 0; r < 1000; ++r) test_it(build_u(l), n);
   }

   //
   // ------------------------------------------------------------- partial-sort
   //
   CATCH_SECTION("partial-sort")
   {
      g.seed(1);

      auto build_u = [&](auto len) {
         std::vector<char> u(len);
         iota(begin(u), end(u), 'a');
         shuffle(begin(u), end(u), g);
         return u;
      };

      auto test_it = [&](auto u, auto n) {
         auto mid = begin(u) + n;
         learn_std::partial_sort(begin(u), mid, end(u));
         CATCH_REQUIRE(std::is_sorted(begin(u), mid));
         if(mid != begin(u))
            CATCH_REQUIRE(std::all_of(
                mid, end(u), [&](auto& a) { return a >= *(mid - 1); }));
      };

      for(auto l = 0u; l <= 10; ++l)
         for(auto n = 0u; n < l; ++n)
            for(auto r = 0; r < 1000; ++r) test_it(build_u(l), n);
   }

   //
   // -------------------------------------------------------- partial-sort-copy
   //
   CATCH_SECTION("partial-sort-copy")
   {
      g.seed(1);

      auto build_u = [&](auto len) {
         std::vector<int> u(len);
         iota(begin(u), end(u), 0);
         shuffle(begin(u), end(u), g);
         return u;
      };

      auto test_it = [&](auto u, auto n) {
         auto v = u;
         v.resize(n);
         fill(begin(v), end(v), -1);
         auto t = v;

         auto r1
             = learn_std::partial_sort_copy(begin(u), end(u), begin(v), end(v));
         auto r2 = std::partial_sort_copy(begin(u), end(u), begin(t), end(t));

         CATCH_REQUIRE(v == t);
         CATCH_REQUIRE(distance(begin(v), r1) == distance(begin(t), r2));
      };

      for(auto l = 0u; l <= 10; ++l)
         for(auto n = 0u; n < l + 2; ++n)
            for(auto r = 0; r < 1000; ++r) test_it(build_u(l), n);
   }
}
