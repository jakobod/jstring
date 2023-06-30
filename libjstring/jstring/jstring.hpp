/**
 *  @author    Jakob Otto
 *  @file      jstring.hpp
 *  @copyright Copyright 2023 Jakob Otto. All rights reserved.
 *             This file is part of the jstring project, released under
 *             the GNU GPL3 License.
 */

#pragma once

#include <array>
#include <bit>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <iterator>
#include <memory>
#include <string_view>

namespace jstring {

class jstring {
  using short_size_type = std::uint8_t;

  using value_type = char;

  using size_type = std::size_t;

  using difference_type = std::ptrdiff_t;

  using reference = value_type&;

  using const_reference = const value_type&;

  using pointer = value_type*;

  using const_pointer = const value_type*;

  using iterator = pointer;

  using const_iterator = const_pointer;

  using reverse_iterator = std::reverse_iterator<value_type>;

  using const_reverse_iterator = std::reverse_iterator<const value_type>;

  // -- Data types for holding strings memory ----------------------------------

  /// @brief  Represents the string in long mode
  struct long_data {
    pointer data_;
    size_type capacity_;
    size_type size_;
    bool is_long_;
  };

  static constexpr const auto short_capacity
    = (sizeof(long_data) - sizeof(short_size_type) - sizeof(bool));

  /// @brief Represents the string object in short mode
  struct short_data {
    std::array<value_type, short_capacity> data_;
    short_size_type size_;
    bool is_long_;
  };

  /// @brief Represents the string as raw bytes
  struct raw_data {
    std::array<std::byte, sizeof(long_data) - sizeof(bool)> raw_;
    bool is_long_;
  };

  /// @brief Sanity check that the defined types have equal size
  static_assert((sizeof(short_data) == sizeof(long_data))
                && (sizeof(long_data) == sizeof(raw_data)));

public:
  // -- Constructors Destructors -----------------------------------------------

  jstring();

  jstring(const char* data, size_type size);

  jstring(const char* data);

  ~jstring();

  // -- Assignment Constructors/Operators --------------------------------------

  jstring(const jstring& other) = default;
  jstring(jstring&& other) = default;
  jstring& operator=(const jstring& other) = default;
  jstring& operator=(jstring&& other) = default;

  // -- Assignment -------------------------------------------------------------

  jstring& assign(size_type count, value_type value);

  jstring& assign(const_pointer ptr, size_type size);

  // -- Element access ---------------------------------------------------------

  reference at(size_type pos);

  const_reference at(size_type pos) const;

  reference operator[](size_type pos);

  const_reference operator[](size_type pos) const;

  reference front();

  const_reference front() const;

  reference back();

  const_reference back() const;

  pointer data();

  const_pointer data() const;

  const_pointer c_str() const noexcept;

  constexpr operator std::string_view() const noexcept;

  // -- Capacity ---------------------------------------------------------------

  bool empty() const;

  size_type size() const;

  size_type max_size() const;

  void reserve(size_type size);

  size_type capacity() const;

  void shrink_to_fit();

  // -- input/output -----------------------------------------------------------

  friend std::ostream& operator<<(std::ostream& os, const jstring& str);

  // -- internal things --------------------------------------------------------

  bool is_long_mode() const;

private:
  void reallocate(size_type size, bool retain_content = false);

  void clear() { std::memset(raw_.raw_.data(), 0, sizeof(raw_data)); }

  constexpr long_data& as_long_data() { return long_; }

  constexpr short_data& as_short_data() { return short_; }

  union {
    short_data short_;
    long_data long_;
    raw_data raw_ = {};
  };
};

} // namespace jstring
