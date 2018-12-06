
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "algorithms/partitioning-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

#define TEST(f) CATCH_REQUIRE(learn_std::f == std::f);

CATCH_TEST_CASE("PartitioningOperations_", "[partitioning-operations]")
{
   //
   // ----------------------------------------------------------- is-partitioned
   //
   CATCH_SECTION("is-partitioned")
   {
      auto test_it = [&](unsigned len, unsigned n) {
         auto p = [](auto a) { return a == 'a'; };
         std::vector<char> u(len); // is partitioned
         std::fill(begin(u), begin(u) + n, 'a');
         std::fill(begin(u) + n, end(u), 'X');
         CATCH_REQUIRE(learn_std::is_partitioned(begin(u), end(u), p));
         if(len > 2) {
            std::vector<char> v(len);
            std::fill(begin(v), end(v), 'a');
            v[std::min(len - 2, n)] = 'X';
            CATCH_REQUIRE(!learn_std::is_partitioned(begin(v), end(v), p));
         }
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = 0u; n <= l; ++n) test_it(l, n);
   }

   //
   // ------------------------------------------------- partition-partition-copy
   //
   CATCH_SECTION("partition-partition-copy")
   {
      std::random_device rd;
      std::mt19937 g(rd());

      auto test_it = [&](unsigned len, unsigned n) {
         auto p = [](auto a) { return a == 'a'; };
         std::vector<char> u(len); // is partitioned
         std::vector<char> s(len);
         std::vector<char> t(len);
         std::fill(begin(u), begin(u) + n, 'a');
         std::fill(begin(u) + n, end(u), 'X');
         std::shuffle(begin(u), end(u), g);
         auto [ss, tt] = learn_std::partition_copy(
             begin(u), end(u), begin(s), begin(t), p);
         s.erase(ss, end(s));
         t.erase(tt, end(t));
         auto ii = learn_std::partition(begin(u), end(u), p);
         CATCH_REQUIRE(std::distance(begin(u), ii) == n);
         CATCH_REQUIRE(std::is_partitioned(begin(u), end(u), p));
         CATCH_REQUIRE(s.size() == n);
         CATCH_REQUIRE(t.size() == len - n);
         CATCH_REQUIRE(std::all_of(begin(s), end(s), p));
         CATCH_REQUIRE(std::none_of(begin(t), end(t), p));
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = 0u; n <= l; ++n)
            for(auto k = 0; k < 10; ++k) // 10 random retries
               test_it(l, n);
   }

   //
   // --------------------------------------------------------- stable-partition
   //
   CATCH_SECTION("stable-partition")
   {
      std::mt19937 g;
      g.seed(1);

      auto my_toupper = [](char ch) {
         return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
      };

      auto test_it = [&](unsigned len, unsigned n) {
         auto p = [](auto a) { return std::isupper(a); };
         std::vector<char> u(len); // is partitioned
         std::iota(begin(u), end(u), 'a');

         // Make a random set uppercase
         std::vector<int> indices(len);
         std::iota(begin(indices), end(indices), 0);
         std::shuffle(begin(indices), end(indices), g);
         learn_std::for_each_n(
             begin(indices), n, [&](auto ind) { u[ind] = my_toupper(u[ind]); });

         auto ii = learn_std::stable_partition(begin(u), end(u), p);
         auto jj = std::adjacent_find(
             begin(u), end(u), [&](auto a, auto b) { return !(a < b); });

         CATCH_REQUIRE(std::distance(begin(u), ii) == n);
         CATCH_REQUIRE(std::all_of(begin(u), ii, p));
         CATCH_REQUIRE(std::none_of(ii, end(u), p));
         CATCH_REQUIRE(jj == end(u));
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = 0u; n < l; ++n)
            for(auto k = 0; k < 10; ++k) // 10 random retries
               test_it(l, n);
   }

   //
   // ---------------------------------------------------------- partition-point
   //
   CATCH_SECTION("partition-point")
   {
      auto test_it = [&](unsigned len, unsigned n) {
         std::vector<char> u(len);
         std::fill(begin(u), begin(u) + n, 'a');
         std::fill(begin(u) + n, end(u), 'X');
         auto p  = [](auto& x) { return x == 'a'; };
         auto ii = learn_std::partition_point(begin(u), end(u), p);
         if(ii != end(u)) CATCH_REQUIRE(!p(*ii));
         if(ii != begin(u)) CATCH_REQUIRE(p(*(ii - 1)));
      };

      for(auto l = 0u; l < 10; ++l)
         for(auto n = 0u; n <= l; ++n) test_it(l, n);
   }
}
