
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "algorithms/comparison-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

CATCH_TEST_CASE("ComparisonOperations_", "[comparison-operations]")
{
   std::mt19937 g;
   g.seed(1);
   std::uniform_int_distribution<int> uniform;
   using pt = decltype(uniform)::param_type;

   auto rand = [&](int low, int high) { return uniform(g, pt(low, high)); };

   //
   // -------------------------------------------- equal-lexicographical-compare
   //
   CATCH_SECTION("equal-lexicographical-compare")
   {
      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         auto counter = 0;
         std::generate(begin(u), end(u), [&]() { return counter++ / 2; });
         return u;
      };

      auto build_v = [&](unsigned len) {
         std::vector<int> v(len);
         std::generate(begin(v), end(v), [&]() { return rand(11, 99); });
         return v;
      };

      auto test_it = [&](unsigned l1, unsigned l2) {
#define TESTIT(x) CATCH_REQUIRE(learn_std::x == std::x);
         auto u = build_u(l1);
         auto v = u;
         auto r = build_v(l1);
         auto s = build_v(l2);

         TESTIT(equal(begin(u), end(u), begin(v)));
         TESTIT(equal(begin(v), end(v), begin(u)));
         TESTIT(equal(begin(u), end(u), begin(r)));

         TESTIT(equal(begin(u), end(u), begin(v), end(v)));
         TESTIT(equal(begin(v), end(v), begin(u), end(u)));
         TESTIT(equal(begin(u), end(u), begin(r), end(r)));
         TESTIT(equal(begin(u), end(u), begin(s), end(s)));

         TESTIT(lexicographical_compare(begin(u), end(u), begin(v), end(v)));
         TESTIT(lexicographical_compare(begin(v), end(v), begin(u), end(u)));
         TESTIT(lexicographical_compare(begin(u), end(u), begin(r), end(r)));
#undef TESTIT
      };

      for(auto l1 = 0u; l1 < 10; ++l1)
         for(auto l2 = 0u; l2 < 10; ++l2) test_it(l1, l2);
   }
}
