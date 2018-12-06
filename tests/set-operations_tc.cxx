
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "algorithms/set-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

CATCH_TEST_CASE("SetSearchOperations_", "[set-search-operations]")
{
   std::mt19937 g;
   g.seed(1);
   std::uniform_int_distribution<int> uniform;
   using pt = decltype(uniform)::param_type;

   auto rand = [&](int low, int high) { return uniform(g, pt(low, high)); };

   //
   // ----------------------------------------------------------------- includes
   //
   CATCH_SECTION("includes")
   {
      g.seed(1);

      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         auto counter = 0;
         std::generate(begin(u), end(u), [&]() { return counter++ / 2; });
         return u;
      };

      auto test_it = [&](auto u) {
         vector<int> v;
         for(auto i = 0u; i < u.size(); ++i) {
            v.clear();
            std::sample(begin(u), end(u), std::back_inserter(v), i, g);
            std::sort(begin(v), end(v));
            CATCH_REQUIRE(
                learn_std::includes(begin(u), end(u), begin(v), end(v)));

            // Now add something that shouldn't be there
            if(v.size() == 0)
               v.push_back(-1);
            else
               v[unsigned(rand(0, int(v.size()) - 1))] = -1;

            auto it_is
                = learn_std::includes(begin(u), end(u), begin(v), end(v));

            CATCH_REQUIRE(!it_is);
         }
      };

      test_it(build_u(10));

      for(auto l = 0u; l < 10; ++l)
         for(auto r = 0; r < 100; ++r) test_it(build_u(l));
   }

   //
   // ----------------------------------------------------------- set-operations
   //
   CATCH_SECTION("set-operations")
   {
      g.seed(1);

      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         auto counter = 0;
         std::generate(begin(u), end(u), [&]() { return rand(10, 20); });
         std::sort(begin(u), end(u));
         auto ii = std::unique(begin(u), end(u));
         u.erase(ii, end(u)); // remove duplicate elements
         return u;
      };

      auto test_it = [&](auto u, auto v) {
         { // difference
            vector<int> m(u.size()), n(u.size());
            auto r1 = learn_std::set_difference(
                begin(u), end(u), begin(v), end(v), begin(m));
            auto r2 = std::set_difference(
                begin(u), end(u), begin(v), end(v), begin(n));
            m.erase(r1, end(m));
            n.erase(r2, end(n));
            CATCH_REQUIRE(m == n);
            CATCH_REQUIRE(distance(begin(m), r1) == distance(begin(n), r2));
         }

         { // intersection
            vector<int> m(u.size()), n(u.size());
            auto r1 = learn_std::set_intersection(
                begin(u), end(u), begin(v), end(v), begin(m));
            auto r2 = std::set_intersection(
                begin(u), end(u), begin(v), end(v), begin(n));
            m.erase(r1, end(m));
            n.erase(r2, end(n));
            CATCH_REQUIRE(m == n);
            CATCH_REQUIRE(distance(begin(m), r1) == distance(begin(n), r2));
         }

         { // symmetric-difference
            vector<int> m(u.size() + v.size()), n(u.size() + v.size());
            auto r1 = learn_std::set_symmetric_difference(
                begin(u), end(u), begin(v), end(v), begin(m));
            auto r2 = std::set_symmetric_difference(
                begin(u), end(u), begin(v), end(v), begin(n));
            m.erase(r1, end(m));
            n.erase(r2, end(n));
            CATCH_REQUIRE(m == n);
            CATCH_REQUIRE(distance(begin(m), r1) == distance(begin(n), r2));
         }

         { // union
            vector<int> m(u.size() + v.size()), n(u.size() + v.size());
            auto r1 = learn_std::set_union(
                begin(u), end(u), begin(v), end(v), begin(m));
            auto r2
                = std::set_union(begin(u), end(u), begin(v), end(v), begin(n));
            m.erase(r1, end(m));
            n.erase(r2, end(n));
            CATCH_REQUIRE(m == n);
            CATCH_REQUIRE(distance(begin(m), r1) == distance(begin(n), r2));
         }
      };

      for(auto i = 0u; i < 20; ++i)
         for(auto j = 0u; j < 20; ++j)
            for(auto r = 0; r < 100; ++r) test_it(build_u(i), build_u(j));
   }
}
