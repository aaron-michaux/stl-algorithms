
#include <algorithm>
#include <iostream>
#include <vector>

#include "algorithms/non-modifying-sequence-operations.hxx"

#define CATCH_CONFIG_PREFIX_ALL
#include "catch.hpp"

using std::cout;
using std::endl;
using std::vector;

#define TEST(f) CATCH_REQUIRE(learn_std::f == std::f);

CATCH_TEST_CASE("NonModifyingSequenceOperations_",
                "[non-modifying-sequence-operations]")
{
   //
   // ------------------------------------------------------------- any-all-none
   //
   CATCH_SECTION("any-all-none")
   {
      auto p = [](int x) { return x == 0; };
      { // Test empty sequences
         std::vector<int> v;
         CATCH_REQUIRE(learn_std::all_of(cbegin(v), cend(v), p) == true);
         CATCH_REQUIRE(learn_std::any_of(cbegin(v), cend(v), p) == false);
         CATCH_REQUIRE(learn_std::none_of(cbegin(v), cend(v), p) == true);
      }

      {
         std::vector<int> a{{1, 0, 3}}; // has_zero
         CATCH_REQUIRE(learn_std::all_of(cbegin(a), cend(a), p) == false);
         CATCH_REQUIRE(learn_std::any_of(cbegin(a), cend(a), p) == true);
         CATCH_REQUIRE(learn_std::none_of(cbegin(a), cend(a), p) == false);

         std::vector<int> b{{1, 2, 3}}; // no_zero
         CATCH_REQUIRE(learn_std::all_of(cbegin(b), cend(b), p) == false);
         CATCH_REQUIRE(learn_std::any_of(cbegin(b), cend(b), p) == false);
         CATCH_REQUIRE(learn_std::none_of(cbegin(b), cend(b), p) == true);

         std::vector<int> c{{0, 0, 0}}; // only_zero
         CATCH_REQUIRE(learn_std::all_of(cbegin(c), cend(c), p) == true);
         CATCH_REQUIRE(learn_std::any_of(cbegin(c), cend(c), p) == true);
         CATCH_REQUIRE(learn_std::none_of(cbegin(c), cend(c), p) == false);
      }
   }

   //
   // ----------------------------------------------------------- count-count-if
   //
   CATCH_SECTION("count-count-if")
   {
      auto p = [](int x) { return x == 0; };
      { // Test empty sequences
         std::vector<int> v;
         CATCH_REQUIRE(learn_std::count(cbegin(v), cend(v), 0) == 0);
         CATCH_REQUIRE(learn_std::count_if(cbegin(v), cend(v), p) == 0);
      }

      {
         std::vector<int> a{{1, 0, 3}}; // has_zero
         CATCH_REQUIRE(learn_std::count(cbegin(a), cend(a), 0) == 1);
         CATCH_REQUIRE(learn_std::count_if(cbegin(a), cend(a), p) == 1);

         std::vector<int> b{{1, 2, 3}}; // no_zero
         CATCH_REQUIRE(learn_std::count(cbegin(b), cend(b), 0) == 0);
         CATCH_REQUIRE(learn_std::count_if(cbegin(b), cend(b), p) == 0);

         std::vector<int> c{{0, 0, 0}}; // only_zero
         CATCH_REQUIRE(learn_std::count(cbegin(c), cend(c), 0) == 3);
         CATCH_REQUIRE(learn_std::count_if(cbegin(c), cend(c), p) == 3);
      }
   }

   //
   // ----------------------------------------------------------------- mismatch
   //
   CATCH_SECTION("mismatch")
   {
      auto mirror_ends = [](const std::string& in) -> std::string {
         return std::string(
             in.begin(),
             learn_std::mismatch(in.begin(), in.end(), in.rbegin()).first);
      };

      CATCH_REQUIRE(mirror_ends("abXYZba") == "ab");
      CATCH_REQUIRE(mirror_ends("abca") == "a");
      CATCH_REQUIRE(mirror_ends("aba") == "aba");

      std::string s1 = "Heavenly STL";
      std::string s2 = "Heavenly Halabaloo";
      std::string s3 = "It's a commotion; a fuss";
      std::string s4 = "Heavy STL";

      TEST(mismatch(cbegin(s1), cend(s1), cbegin(s1)));
      TEST(mismatch(cbegin(s1), cend(s1), cbegin(s2)));
      TEST(mismatch(cbegin(s1), cend(s1), cbegin(s3)));
      TEST(mismatch(cbegin(s1), cend(s1), cbegin(s4), cend(s4)));
   }

   //
   // ------------------------------------------------- find-find-if-find-if-not
   //
   CATCH_SECTION("find-find-if-find-if-not")
   {
      { // Empty sequence
         std::vector<int> v;
         auto p = [&](auto& a) { return a == 0; };
         TEST(find(cbegin(v), cend(v), 0));
         TEST(find_if(cbegin(v), cend(v), p));
         TEST(find_if_not(cbegin(v), cend(v), p));
      }

      { // x in sequence
         std::vector<int> v{0, 1, 2, 3, 4};
         for(auto x : v) {
            auto p = [&](auto& a) { return x == a; };
            TEST(find(cbegin(v), cend(v), x));
            TEST(find_if(cbegin(v), cend(v), p));
            TEST(find_if_not(cbegin(v), cend(v), p));
         }
      }

      { // x not in sequence
         std::vector<int> v{0, 1, 2, 3, 4};
         for(auto x : v) {
            auto p = [&](auto& a) { return a == -1; };
            TEST(find(cbegin(v), cend(v), -1));
            TEST(find_if(cbegin(v), cend(v), p));
            TEST(find_if_not(cbegin(v), cend(v), p));
         }
      }
   }

   //
   // ---------------------------------------------------------- search-find-end
   //
   CATCH_SECTION("search-find-end")
   {
      { // empty
         std::string t1;
         std::string s1 = "hello";
         TEST(search(cbegin(t1), cend(t1), cbegin(t1), cend(t1)));
         TEST(search(cbegin(t1), cend(t1), cbegin(s1), cend(s1)));
         TEST(search(cbegin(s1), cend(s1), cbegin(t1), cend(t1)));
         TEST(find_end(cbegin(t1), cend(t1), cbegin(t1), cend(t1)));
         TEST(find_end(cbegin(t1), cend(t1), cbegin(s1), cend(s1)));
         TEST(find_end(cbegin(s1), cend(s1), cbegin(t1), cend(t1)));
      }

      {
         std::string t1
             = "why waste time learning, when ignorance is instantaneous?";
         std::string s1 = "learning";
         std::string s2 = "lemming";
         std::string s3 = "ta";

         TEST(search(cbegin(t1), cend(t1), cbegin(s1), cend(s1)));
         TEST(search(cbegin(t1), cend(t1), cbegin(s2), cend(s2)));
         TEST(search(cbegin(t1), cend(t1), cbegin(s3), cend(s3)));
         TEST(find_end(cbegin(t1), cend(t1), cbegin(s1), cend(s1)));
         TEST(find_end(cbegin(t1), cend(t1), cbegin(s2), cend(s2)));
         TEST(find_end(cbegin(t1), cend(t1), cbegin(s3), cend(s3)));
      }
   }

   //
   // ------------------------------------------------------------ find-first-of
   //
   CATCH_SECTION("find-first-of")
   {
      { // empty
         std::string t1;
         std::string s1 = "hello";
         TEST(find_first_of(cbegin(t1), cend(t1), cbegin(t1), cend(t1)));
         TEST(find_first_of(cbegin(t1), cend(t1), cbegin(s1), cend(s1)));
         TEST(find_first_of(cbegin(s1), cend(s1), cbegin(t1), cend(t1)));
      }

      {
         std::vector<int> t{3, 19, 10, 2};
         std::vector<int> u{25};
         std::vector<int> v{0, 2, 3, 25, 5};
         TEST(find_first_of(cbegin(t), cend(t), cbegin(t), cend(t)));
         TEST(find_first_of(cbegin(t), cend(t), cbegin(u), cend(u)));
         TEST(find_first_of(cbegin(t), cend(t), cbegin(v), cend(v)));
         TEST(find_first_of(cbegin(u), cend(u), cbegin(t), cend(t)));
         TEST(find_first_of(cbegin(u), cend(u), cbegin(u), cend(u)));
         TEST(find_first_of(cbegin(u), cend(u), cbegin(v), cend(v)));
         TEST(find_first_of(cbegin(v), cend(v), cbegin(t), cend(t)));
         TEST(find_first_of(cbegin(v), cend(v), cbegin(u), cend(u)));
         TEST(find_first_of(cbegin(v), cend(v), cbegin(v), cend(v)));
      }
   }

   //
   // ------------------------------------------------------------ adjacent-find
   //
   CATCH_SECTION("adjacent-find")
   {
      { // empty
         std::string t1;
         TEST(adjacent_find(cbegin(t1), cend(t1)));
      }

      {
         std::string s1 = "hello";
         std::string s2 = "no-result";
         TEST(adjacent_find(cbegin(s1), cend(s1)));
         TEST(adjacent_find(cbegin(s2), cend(s2)));
      }
   }

   //
   // ----------------------------------------------------------------- search-n
   //
   CATCH_SECTION("search-n")
   {
      { // empty
         std::string t1;
         TEST(search_n(cbegin(t1), cend(t1), 0, '\0'));
         TEST(search_n(cbegin(t1), cend(t1), 1, '\0'));
      }

      {
         std::string s1 = "hello";
         std::string s2 = "no-result";
         std::string s3 = "nada";
         for(auto i = 0; i < 4; ++i) {
            TEST(search_n(cbegin(s1), cend(s1), i, 'l'));
            TEST(search_n(cbegin(s2), cend(s2), i, 'l'));
            TEST(search_n(cbegin(s3), cend(s3), i, 'l'));
         }
      }
   }
}
