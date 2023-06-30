/**
 *  @author    Jakob Otto
 *  @file      test/jstring.cpp
 *  @copyright Copyright 2023 Jakob Otto. All rights reserved.
 *             This file is part of the jstring project, released under
 *             the GNU GPL3 License.
 */

#include "gtest/gtest.h"

#include "jstring/jstring.hpp"

namespace {

constexpr const std::array<char, 26> lowercase_alphabet{
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

constexpr const std::array<char, 26> uppercase_alphabet{
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

} // namespace

TEST(jstring, construct) {
  {
    jstring::jstring str;
    EXPECT_FALSE(str.is_long_mode());
  }
  {
    jstring::jstring str{"Hello World"};
    EXPECT_FALSE(str.is_long_mode());
  }
  {
    jstring::jstring str{"Hello World, this is a test string"};
    EXPECT_TRUE(str.is_long_mode());
  }
  {
    jstring::jstring str{"abcdefghijklmnopqrstuvw"};
    EXPECT_FALSE(str.is_long_mode());
  }
  {
    jstring::jstring str{
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    EXPECT_TRUE(str.is_long_mode());
  }
}

TEST(jstring, access) {
  const jstring::jstring str{"abcdefghijklmnopqrstuvwxyz"};
  ASSERT_EQ(str.size(), lowercase_alphabet.size());
  for (std::size_t i{0}; i < str.size(); ++i)
    EXPECT_EQ(str.at(i), lowercase_alphabet.at(i));
}
