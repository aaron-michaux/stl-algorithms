
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "algorithms/min-max-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

CATCH_TEST_CASE("MinMaxOperations_", "[min-max-operations]")
{
   //
   // --------------------------------------------------- min-max-minmax-element
   //
   CATCH_SECTION("min-max-minmax-element")
   {
      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         auto counter = 0;
         std::generate(begin(u), end(u), [&]() { return counter++ / 2; });
         return u;
      };

      auto test_it = [&](auto u) {
         { // max-element
            auto ii = learn_std::max_element(begin(u), end(u));
            auto jj = std::max_element(begin(u), end(u));
            CATCH_REQUIRE(ii == jj);
         }

         { // min-element
            auto ii = learn_std::min_element(begin(u), end(u));
            auto jj = std::min_element(begin(u), end(u));
            CATCH_REQUIRE(ii == jj);
         }

         { // minmax-element
            auto ii = learn_std::minmax_element(begin(u), end(u));
            auto jj = std::minmax_element(begin(u), end(u));
            CATCH_REQUIRE(ii == jj);
         }
      };

      for(auto i = 0u; i < 20; ++i) test_it(build_u(i));
   }

   //
   // ----------------------------------------------------------- min-max-minmax
   //
   CATCH_SECTION("min-max-minmax")
   {
#define TESTIT(x) CATCH_REQUIRE(learn_std::x == std::x);
      TESTIT(min(1, 2));
      TESTIT(min(1, 1));
      TESTIT(min(2, 1));

      TESTIT(max(1, 2));
      TESTIT(max(1, 1));
      TESTIT(max(2, 1));

      TESTIT(minmax({1, 2, 3, 4, 5}));
      TESTIT(minmax({2}));
      TESTIT(minmax({1, 1, 2, 2, 3, 3}));
#undef TESTIT
   }

   //
   // -------------------------------------------------------------------- clamp
   //
   CATCH_SECTION("clamp")
   {
#define TESTIT(x) CATCH_REQUIRE(learn_std::x == std::x);
      TESTIT(clamp(1, 0, 9));
      TESTIT(clamp(0, 0, 9));
      TESTIT(clamp(9, 0, 9));
      TESTIT(clamp(-1, 0, 9));
      TESTIT(clamp(10, 0, 9));
#undef TESTIT
   }
}
