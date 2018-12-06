
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "algorithms/permutation-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

CATCH_TEST_CASE("PermutationOperations_", "[permutation-operations]")
{
   std::mt19937 g;
   g.seed(1);
   std::uniform_int_distribution<int> uniform;
   using pt = decltype(uniform)::param_type;

   auto rand = [&](int low, int high) { return uniform(g, pt(low, high)); };

   //
   // ---------------------------------------------------- next-prev-permutation
   //
   CATCH_SECTION("next-prev-permutation")
   {
      //
      auto make_it = [](unsigned len) {
         std::vector<int> v(len);
         std::iota(begin(v), end(v), 0);
         return v;
      };

      auto test_it = [&](std::vector<int> v) {
         auto u = v;
         auto t = v;

         while(true) {
            auto r1 = learn_std::next_permutation(begin(u), end(u));
            auto r2 = std::next_permutation(begin(v), end(v));
            auto p1 = learn_std::is_permutation(begin(u), end(u), begin(t));
            auto p2 = learn_std::is_permutation(begin(t), end(t), begin(u));
            CATCH_REQUIRE(u == v);
            CATCH_REQUIRE(r1 == r2);
            CATCH_REQUIRE(p1);
            CATCH_REQUIRE(p2);

            auto r3 = learn_std::prev_permutation(begin(u), end(u));
            auto r4 = std::prev_permutation(begin(v), end(v));
            CATCH_REQUIRE(u == v);
            CATCH_REQUIRE(r3 == r4);

            learn_std::next_permutation(begin(u), end(u));
            std::next_permutation(begin(v), end(v));

            if(!r2) break;
         }
      };

      // test_it(make_it(3));
      for(auto len = 0u; len < 8; ++len) test_it(make_it(len));
   }

   //
   // ----------------------------------------------------------- is-permutation
   //
   CATCH_SECTION("is-permutation")
   {
      auto make_it = [&](unsigned len) {
         std::vector<int> v(len);
         std::generate(begin(v), end(v), [&]() { return rand(0, 6); });
         return v;
      };

      auto test_it = [&](std::vector<int> v) {
         auto u = v;
         auto t = v;
         if(t.size() > 0) t[0] = -1;

         while(true) {
            auto r1 = std::next_permutation(begin(v), end(v));
            std::next_permutation(begin(t), end(t));
            CATCH_REQUIRE(
                learn_std::is_permutation(begin(v), end(v), begin(u)));
            CATCH_REQUIRE(
                learn_std::is_permutation(begin(u), end(u), begin(v)));

            CATCH_REQUIRE(
                std::is_permutation(begin(t), end(t), begin(u))
                == learn_std::is_permutation(begin(t), end(t), begin(u)));
            CATCH_REQUIRE(
                std::is_permutation(begin(t), end(t), begin(u))
                == learn_std::is_permutation(begin(u), end(u), begin(t)));
            if(!r1) break;
         }
      };

      for(auto len = 0u; len < 8; ++len) test_it(make_it(len));
   }
}
