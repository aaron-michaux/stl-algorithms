
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "algorithms/modifying-sequence-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

#define TEST(f) CATCH_REQUIRE(learn_std::f == std::f);

CATCH_TEST_CASE("ModifyingSequenceOperations_",
                "[modifying-sequence-operations]")
{
   //
   // ------------------------------------------------------------------- copy-n
   //
   CATCH_SECTION("copy-n")
   {
      auto test_it = [&](unsigned len, unsigned n) {
         std::vector<int> u(len);
         std::vector<int> v(len);
         std::iota(begin(u), end(u), 1);
         std::fill(begin(v), end(v), 0);
         auto r1 = learn_std::copy_n(begin(u), n, begin(v));
         CATCH_REQUIRE(r1 == begin(v) + n);
         for(auto i = 0u; i < n; ++i) CATCH_REQUIRE(v[i] == u[i]);
      };

      for(auto i = 0u; i < 10; ++i)
         for(auto n = 0u; n < i; ++n) test_it(i, n);
   }

   //
   // ---------------------------------------------------------------- move-copy
   //
   CATCH_SECTION("move-copy")
   {
      auto test_it = [&](unsigned len, int offset) {
         std::vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         auto v  = u;
         auto t  = u;
         auto r1 = learn_std::move(begin(u) + offset, end(u), begin(u));
         auto r2 = learn_std::copy(begin(t) + offset, end(t), begin(t));
         CATCH_REQUIRE(r1 == end(u) - offset);
         CATCH_REQUIRE(r2 == end(t) - offset);
         for(auto i = 0u; i < len - unsigned(offset); ++i) {
            CATCH_REQUIRE(v[i + unsigned(offset)] == u[i]);
            CATCH_REQUIRE(v[i + unsigned(offset)] == t[i]);
         }
      };

      for(auto i = 0u; i < 10; ++i)
         for(auto o = 0u; o < i; ++o) test_it(i, int(o));
   }

   //
   // ------------------------------------------------------- move-copy-backward
   //
   CATCH_SECTION("move-copy-backward")
   {
      auto test_it = [&](unsigned len, unsigned offset) {
         std::vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         auto v  = u;
         auto t  = u;
         auto r1 = learn_std::move_backward(begin(u), end(u) - offset, end(u));
         auto r2 = learn_std::copy_backward(begin(t), end(t) - offset, end(t));
         CATCH_REQUIRE(r1 == begin(u) + int(offset));
         CATCH_REQUIRE(r2 == begin(t) + int(offset));
         for(auto i = 0u; i < len - unsigned(offset); ++i) {
            CATCH_REQUIRE(u[i + unsigned(offset)] == v[i]);
            CATCH_REQUIRE(t[i + unsigned(offset)] == v[i]);
         }
      };

      for(auto i = 0u; i < 10; ++i)
         for(auto o = 0u; o < i; ++o) test_it(i, o);
   }

   //
   // --------------------------------------------------------------------- fill
   //
   CATCH_SECTION("fill")
   {
      auto test_it = [&](unsigned len) {
         auto x = len;
         std::vector<int> u(len);
         learn_std::fill(begin(u), end(u), int(x));
         for(auto& x : u) CATCH_REQUIRE(x == x);
      };

      for(auto i = 0u; i < 10; ++i) test_it(i);
   }

   //
   // ------------------------------------------------------------------- fill-n
   //
   CATCH_SECTION("fill-n")
   {
      auto test_it = [&](unsigned len, unsigned n) {
         auto x = len;
         std::vector<int> u(len);
         auto r1 = learn_std::fill_n(begin(u), n, int(x));
         CATCH_REQUIRE(r1 == begin(u) + int(n));
         for(auto i = 0u; i < n; ++i) CATCH_REQUIRE(u[i] == x);
      };

      for(auto i = 0u; i < 10; ++i)
         for(auto n = 0u; n < i; ++n) test_it(i, n);
   }

   //
   // ---------------------------------------------------------------- transform
   //
   CATCH_SECTION("transform")
   {
      auto op1 = [](int a) { return -a; };
      auto op2 = [](int a, int b) { return a * b; };

      auto test_it = [&](unsigned len) {
         std::vector<int> u(len);
         std::vector<int> v(len);
         std::iota(begin(u), end(u), 0);
         std::iota(begin(v), end(v), 0);
         std::vector<int> out1(len);
         std::vector<int> out2(len);

         auto r1 = learn_std::transform(begin(u), end(u), begin(out1), op1);
         auto r2 = learn_std::transform(
             begin(u), end(u), begin(v), begin(out2), op2);

         CATCH_REQUIRE(r1 == std::next(begin(out1), int(u.size())));
         CATCH_REQUIRE(r2 == std::next(begin(out2), int(u.size())));

         for(auto i = 0u; i < u.size(); ++i) CATCH_REQUIRE(u[i] == -out1[i]);
         for(auto i = 0u; i < u.size(); ++i)
            CATCH_REQUIRE(u[i] * v[i] == out2[i]);
      };

      for(auto i = 0u; i < 10; ++i) test_it(i);
   }

   //
   // ------------------------------------------------------------------- remove
   //
   CATCH_SECTION("remove")
   {
      auto test_it = [](const unsigned len) {
         auto test_one = [](auto& u, auto value) {
            auto v    = u;
            auto ret1 = learn_std::remove(begin(u), end(u), value);
            auto ret2 = std::remove(begin(v), end(v), value);
            CATCH_REQUIRE(std::distance(begin(u), ret1)
                          == std::distance(begin(v), ret2));
            CATCH_REQUIRE(u == v);
         };

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            test_one(u, 'x');
            test_one(u, '_');
         }

         {
            std::vector<char> u(len);
            std::iota(begin(u), end(u), 'a');
            for(auto i = 0u; i < len; ++i) test_one(u, char('a' + i));
            test_one(u, '_');
         }
      };

      for(auto len = 0u; len < 5; ++len) test_it(len);
   }

   //
   // -------------------------------------------------------------- remove-copy
   //
   CATCH_SECTION("remove-copy")
   {
      auto test_it = [](const unsigned len) {
         auto test_one = [](auto& u, auto value) {
            auto p = u;
            auto q = u;
            auto ret1
                = learn_std::remove_copy(begin(u), end(u), begin(p), value);
            auto ret2 = std::remove_copy(begin(u), end(u), begin(q), value);
            CATCH_REQUIRE(std::distance(begin(p), ret1)
                          == std::distance(begin(q), ret2));
            CATCH_REQUIRE(p == q);
         };

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            test_one(u, 'x');
            test_one(u, '_');
         }

         {
            std::vector<char> u(len);
            std::iota(begin(u), end(u), 'a');
            test_one(u, 'a');
            test_one(u, '_');
         }
      };

      for(auto len = 0u; len < 5; ++len) test_it(len);
   }

   //
   // ------------------------------------------------------------------ replace
   //
   CATCH_SECTION("replace")
   {
      auto test_it = [](const unsigned len) {
         auto test_one = [](auto& u, auto old_value, auto new_value) {
            auto v = u;
            learn_std::replace(begin(u), end(u), old_value, new_value);
            std::replace(begin(v), end(v), old_value, new_value);
            CATCH_REQUIRE(u == v);
         };

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            test_one(u, 'x', '_');
            test_one(u, '_', 'x');
         }

         {
            std::vector<char> u(len);
            std::iota(begin(u), end(u), 'a');
            test_one(u, 'a', '_');
            test_one(u, '_', 'x');
         }
      };

      for(auto len = 0u; len < 5; ++len) test_it(len);
   }

   //
   // ------------------------------------------------------------- replace-copy
   //
   CATCH_SECTION("replace-copy")
   {
      auto test_it = [](const unsigned len) {
         auto test_one = [](auto& u, auto old_value, auto new_value) {
            std::vector<char> p(u.size());
            std::vector<char> q(u.size());
            auto ret1 = learn_std::replace_copy(
                begin(u), end(u), begin(p), old_value, new_value);
            auto ret2 = std::replace_copy(
                begin(u), end(u), begin(q), old_value, new_value);
            CATCH_REQUIRE(std::distance(begin(p), ret1)
                          == std::distance(begin(q), ret2));
            CATCH_REQUIRE(p == q);
         };

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            test_one(u, 'x', '_');
            test_one(u, '_', 'x');
         }

         {
            std::vector<char> u(len);
            std::iota(begin(u), end(u), 'a');
            test_one(u, 'a', '_');
            test_one(u, '_', 'x');
         }
      };

      for(auto len = 0u; len < 5; ++len) test_it(len);
   }

   //
   // -------------------------------------------------------------- swap-ranges
   //
   CATCH_SECTION("swap-ranges")
   {
      auto test_it = [](const unsigned len) {
         std::vector<char> v(len);
         auto u = v;
         std::iota(begin(u), end(u), 'A');
         std::iota(begin(v), end(v), 'a');
         auto p    = u;
         auto q    = v;
         auto ret1 = learn_std::swap_ranges(begin(v), end(v), begin(u));
         auto ret2 = learn_std::swap_ranges(begin(q), end(q), begin(p));
         CATCH_REQUIRE(std::distance(begin(u), ret1)
                       == std::distance(begin(p), ret2));
         CATCH_REQUIRE(u == p);
      };

      for(auto len = 0u; len < 10; ++len) test_it(len);
   }

   //
   // ------------------------------------------------------------------ reverse
   //
   CATCH_SECTION("reverse")
   {
      auto test_it = [](const unsigned len) {
         std::vector<char> v(len);
         std::iota(begin(v), end(v), 'a');
         auto u = v;
         learn_std::reverse(begin(v), end(v));
         std::reverse(begin(u), end(u));
         CATCH_REQUIRE(u == v);
      };

      for(auto len = 0u; len < 5; ++len) test_it(len);
   }

   //
   // ------------------------------------------------------------- reverse-copy
   //
   CATCH_SECTION("reverse-copy")
   {
      auto test_it = [](const unsigned len) {
         std::vector<char> v(len);
         std::iota(begin(v), end(v), 'a');
         auto u    = v;
         auto t    = v;
         auto ret1 = learn_std::reverse_copy(begin(v), end(v), begin(u));
         auto ret2 = std::reverse_copy(begin(v), end(v), begin(t));

         CATCH_REQUIRE(std::distance(begin(u), ret1)
                       == std::distance(begin(t), ret2));
         CATCH_REQUIRE(u == t);
      };

      for(auto len = 0u; len < 5; ++len) test_it(len);
   }

   //
   // ------------------------------------------------------------------- rotate
   //
   CATCH_SECTION("rotate")
   {
      auto test_it = [](const unsigned len, const unsigned pos) {
         std::vector<char> v(len);
         std::iota(begin(v), end(v), 'a');
         auto u   = v;
         auto ret = learn_std::rotate(begin(v), begin(v) + pos, end(v));

         for(auto i = 0u; i < len; ++i)
            CATCH_REQUIRE(u[i] == v[(i + (len - pos)) % len]);

         auto ret2 = std::rotate(begin(u), begin(u) + pos, end(u));
         CATCH_REQUIRE(u == v);
         CATCH_REQUIRE(std::distance(begin(u), ret2)
                       == std::distance(begin(v), ret));
      };

      for(auto len = 0u; len < 20; ++len)
         for(auto pos = 0u; pos < len; ++pos) { test_it(len, pos); }

      // for(auto i = 0u; i <= 10; ++i) {
      //    std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      //    std::rotate(begin(v), std::next(begin(v), i), end(v));
      //    for(auto x : v) std::cout << x << " ";
      //    std::cout << std::endl;
      // }
   }

   //
   // -------------------------------------------------------------- rotate-copy
   //
   CATCH_SECTION("rotate-copy")
   {
      auto test_it = [](const unsigned len, const unsigned pos) {
         std::vector<char> v(len);
         std::iota(begin(v), end(v), 'a');
         auto u   = v;
         auto ret = learn_std::rotate_copy(
             begin(v), begin(v) + pos, end(v), begin(u));

         for(auto i = 0u; i < len; ++i)
            CATCH_REQUIRE(v[i] == u[(i + (len - pos)) % len]);

         auto t = v;
         auto ret2
             = std::rotate_copy(begin(v), begin(v) + pos, end(v), begin(t));
         CATCH_REQUIRE(u == t);
         CATCH_REQUIRE(std::distance(begin(t), ret2)
                       == std::distance(begin(u), ret));
      };

      for(auto len = 0u; len < 5; ++len)
         for(auto pos = 0u; pos < len; ++pos) { test_it(len, pos); }
   }

   //
   // --------------------------------------------------------------- shift-left
   //
   CATCH_SECTION("shift-left")
   {
      auto test_it = [](const unsigned len, const int shift) {
         vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         auto v  = u;
         auto ii = learn_std::shift_left(begin(u), end(u), shift);
         unsigned u_shift
             = (shift < 0) ? 0u : (shift >= int(len)) ? len : unsigned(shift);
         CATCH_REQUIRE(ii == end(u) - u_shift);
         for(auto i = 0u; i < unsigned(ii - begin(u)); ++i)
            CATCH_REQUIRE(u[i] == v[i + u_shift]);
      };

      for(auto len = 0u; len < 5; ++len)
         for(auto shift = -1; shift <= int(len); ++shift) test_it(len, shift);
   }

   //
   // -------------------------------------------------------------- shift-right
   //
   CATCH_SECTION("shift-right")
   {
      auto test_it = [](const unsigned len, const int shift) {
         vector<int> u(len);
         std::iota(begin(u), end(u), 0);
         auto v  = u;
         auto ii = learn_std::shift_right(begin(u), end(u), shift);
         unsigned u_shift
             = (shift < 0) ? 0u : (shift >= int(len)) ? len : unsigned(shift);
         CATCH_REQUIRE(ii == begin(u) + u_shift);
         for(auto i = 0u; i < unsigned(len - u_shift); ++i)
            CATCH_REQUIRE(u[i + u_shift] == v[i]);
      };

      for(auto len = 0u; len < 5; ++len)
         for(auto shift = -1; shift <= int(len); ++shift) test_it(len, shift);
   }

   //
   // ------------------------------------------------------------------ shuffle
   //
   CATCH_SECTION("shuffle")
   {
      std::random_device rd;
      std::mt19937 g(rd());
      std::vector<int> u = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> v = u;
      learn_std::shuffle(begin(u), end(u), g);
      std::sort(begin(u), end(u));
      CATCH_REQUIRE(u == v);
   }

   //
   // ------------------------------------------------------------------- unique
   //
   CATCH_SECTION("unique")
   {
      auto test_it = [](unsigned len) {
         auto test_one = [](auto& u) {
            auto v    = u;
            auto ret1 = learn_std::unique(begin(u), end(u));
            auto ret2 = std::unique(begin(v), end(v));
            u.erase(ret1, end(u));
            v.erase(ret2, end(v));
            CATCH_REQUIRE(std::distance(begin(u), ret1)
                          == std::distance(begin(v), ret2));
            CATCH_REQUIRE(u == v);
         };

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            test_one(u);
         }

         {
            std::vector<char> u(len);
            std::iota(begin(u), end(u), 'a');
            test_one(u);
         }

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            std::fill(begin(u) + len / 2, end(u), 'y');
            test_one(u);
         }
      };

      for(auto len = 0u; len < 20; ++len) test_it(len);
   }

   //
   // -------------------------------------------------------------- unique-copy
   //
   CATCH_SECTION("unique-copy")
   {
      auto test_it = [](unsigned len) {
         auto test_one = [](auto& u) {
            auto p    = u;
            auto q    = u;
            auto ret1 = learn_std::unique_copy(begin(u), end(u), begin(p));
            auto ret2 = std::unique_copy(begin(u), end(u), begin(q));
            p.erase(ret1, end(p));
            q.erase(ret2, end(q));
            CATCH_REQUIRE(std::distance(begin(p), ret1)
                          == std::distance(begin(q), ret2));
            CATCH_REQUIRE(p == q);
         };

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            test_one(u);
         }

         {
            std::vector<char> u(len);
            std::iota(begin(u), end(u), 'a');
            test_one(u);
         }

         {
            std::vector<char> u(len);
            std::fill(begin(u), end(u), 'x');
            std::fill(begin(u) + len / 2, end(u), 'y');
            test_one(u);
         }
      };

      for(auto len = 0u; len < 20; ++len) test_it(len);
   }
}
