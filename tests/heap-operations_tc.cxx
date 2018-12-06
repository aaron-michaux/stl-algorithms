
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "algorithms/heap-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

CATCH_TEST_CASE("HeapOperations_", "[heap-operations]")
{
   std::mt19937 g;
   g.seed(1);

   //
   // ---------------------------------------------------- is-heap-is-heap-until
   //
   CATCH_SECTION("is-heap-is-heap-until")
   {
      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         std::make_heap(begin(u), end(u));
         return u;
      };

      auto test_it = [&](auto u, unsigned n) {
         CATCH_REQUIRE(learn_std::is_heap(begin(u), end(u)));
         u[n] = 99;
         CATCH_REQUIRE(!learn_std::is_heap(begin(u), end(u)));
         auto pos = learn_std::is_heap_until(begin(u), end(u));
         CATCH_REQUIRE(std::distance(begin(u), pos) == n);
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = 1u; n < l; ++n) test_it(build_u(l), n);
   }

   //
   // ------------------------------------------------------ push-heap-make-heap
   //
   CATCH_SECTION("push-heap-make-heap")
   {
      g.seed(1);

      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         std::shuffle(begin(u), end(u), g);
         return u;
      };

      auto test_it = [&](std::vector<int> u) {
         auto t = u;
         auto v = u;
         for(auto i = 0u; i < u.size(); ++i) {
            auto last = std::next(begin(u), i);
            learn_std::push_heap(begin(u), last);
            CATCH_REQUIRE(std::is_heap(begin(u), last));
         }

         u = t;
         learn_std::make_heap(begin(u), end(u));
         CATCH_REQUIRE(std::is_heap(begin(u), end(u)));
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = 0u; n < 1000; ++n) test_it(build_u(l));
   }

   //
   // ------------------------------------------------------- pop-heap-sort-heap
   //
   CATCH_SECTION("pop-heap-sort-heap")
   {
      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         std::make_heap(begin(u), end(u));
         return u;
      };

      auto test_it = [&](std::vector<int> u) {
         auto v = u;

         for(auto i = 0u; i < u.size(); ++i) {
            learn_std::pop_heap(begin(u), end(u) - i);
            CATCH_REQUIRE(std::is_sorted(end(u) - i, end(u)));
            CATCH_REQUIRE(std::is_heap(begin(u), end(u) - i - 1));
         }

         learn_std::sort_heap(begin(v), end(v));
         CATCH_REQUIRE(std::is_sorted(begin(v), end(v)));
      };

      for(auto l = 0u; l < 100; ++l) test_it(build_u(l));
   }
}
