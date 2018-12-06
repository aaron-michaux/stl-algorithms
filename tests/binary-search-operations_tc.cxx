
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "algorithms/binary-search-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

CATCH_TEST_CASE("BinarySearchOperations_", "[binary-search-operations]")
{
   std::mt19937 g;
   g.seed(1);

   //
   // -------------------------------------------- lower-upper-bound-equal-range
   //
   CATCH_SECTION("lower-upper-bound-equal-range")
   {
      g.seed(1);

      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         auto counter = 0;
         std::generate(begin(u), end(u), [&]() { return ++counter / 2; });
         return u;
      };

      auto test_it = [&](auto u, int n) {
         auto ii = std::lower_bound(begin(u), end(u), n);
         auto jj = std::upper_bound(begin(u), end(u), n);
         auto kk = learn_std::lower_bound(begin(u), end(u), n);
         auto ll = learn_std::upper_bound(begin(u), end(u), n);
         CATCH_REQUIRE(ii == kk);
         CATCH_REQUIRE(jj == ll);
         CATCH_REQUIRE(learn_std::equal_range(begin(u), end(u), n)
                       == std::equal_range(begin(u), end(u), n));
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = -2; n < int(l) + 2; ++n) test_it(build_u(l), n);
   }

   //
   // ------------------------------------------------------------ binary-search
   //
   CATCH_SECTION("binary-search")
   {
      auto build_u = [&](unsigned len) {
         std::vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         return u;
      };

      auto test_it = [&](auto u, int n) {
         auto has   = (n >= 0 and n < int(u.size()));
         auto found = learn_std::binary_search(begin(u), end(u), n);
         CATCH_REQUIRE(has == found);
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = -2; n < int(l) + 2; ++n) test_it(build_u(l), n);
   }
}
