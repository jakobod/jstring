/**
 *  @author    Jakob Otto
 *  @file      jstring.cpp
 *  @copyright Copyright 2023 Jakob Otto. All rights reserved.
 *             This file is part of the jstring project, released under
 *             the GNU GPL3 License.
 */

#include "jstring/jstring.hpp"

namespace jstring {

// -- Constructors Destructors -------------------------------------------------

jstring::jstring() {
  // nop
}

jstring::jstring(const_pointer data, size_type size) {
  assign(data, size);
}

jstring::jstring(const_pointer data) : jstring{data, std::strlen(data) + 1} {
  // nop
}

jstring::~jstring() {
  if (is_long_mode())
    delete[] long_.data_;
}

// -- Assignment ---------------------------------------------------------------

jstring& jstring::assign(const_pointer ptr, size_type size) {
  const auto is_nullterminated = (ptr[size - 1] == '\0');
  raw_.is_long_ = (size > short_capacity);

  if (is_long_mode()) {
    reallocate(size);
    long_.size_ = size;
    std::memcpy(long_.data_, ptr, size);
  } else {
    short_.size_ = size;
    std::memcpy(short_.data_.data(), ptr, size);
  }
  return *this;
}

// -- Element access -----------------------------------------------------------

jstring::reference jstring::at(size_type pos) {
  if (!is_long_mode())
    return short_.data_.at(pos);

  if (pos >= (long_.size_ >> 1))
    throw std::out_of_range{"index out of range"};
  return long_.data_[pos];
}

jstring::const_reference jstring::at(size_type pos) const {
  if (!is_long_mode())
    return short_.data_.at(pos);

  if (pos >= (long_.size_ >> 1))
    throw std::out_of_range{"index out of range"};
  return long_.data_[pos];
}

jstring::reference jstring::operator[](size_type pos) {
  return is_long_mode() ? long_.data_[pos] : short_.data_[pos];
}

jstring::const_reference jstring::operator[](size_type pos) const {
  return is_long_mode() ? long_.data_[pos] : short_.data_[pos];
}

jstring::reference jstring::front() {
  return (*this)[0];
}

jstring::const_reference jstring::front() const {
  return (*this)[0];
}

jstring::reference jstring::back() {
  return is_long_mode() ? long_.data_[size() - 1] : short_.data_[size() - 1];
}

jstring::const_reference jstring::back() const {
  return is_long_mode() ? long_.data_[size() - 1] : short_.data_[size() - 1];
}

jstring::pointer jstring::data() {
  return is_long_mode() ? long_.data_ : short_.data_.data();
}

jstring::const_pointer jstring::data() const {
  return is_long_mode() ? long_.data_ : short_.data_.data();
}

jstring::const_pointer jstring::c_str() const noexcept {
  return is_long_mode() ? long_.data_ : short_.data_.data();
}

constexpr jstring::operator std::string_view() const noexcept {
  return is_long_mode() ? long_.data_ : short_.data_.data();
}

// -- Iterators ----------------------------------------------------------------

jstring::iterator jstring::begin() {
  return is_long_mode() ? long_.data_ : short_.data_.data();
}

jstring::const_iterator jstring::begin() const {
  return is_long_mode() ? long_.data_ : short_.data_.data();
}

jstring::iterator jstring::end() {
  return is_long_mode() ? long_.data_ + long_.size_
                        : short_.data_.data() + short_.size_;
}

jstring::const_iterator jstring::end() const {
  return is_long_mode() ? long_.data_ + long_.size_
                        : short_.data_.data() + short_.size_;
}

jstring::const_iterator jstring::cbegin() const {
  return is_long_mode() ? long_.data_ : short_.data_.data();
}

jstring::const_iterator jstring::cend() const {
  return is_long_mode() ? long_.data_ + long_.size_
                        : short_.data_.data() + short_.size_;
}

jstring::iterator jstring::rbegin() {
  return end();
}

jstring::const_iterator jstring::rbegin() const {
  return end();
}

jstring::iterator jstring::rend() {
  return begin();
}

jstring::const_iterator jstring::rend() const {
  return begin();
}

jstring::const_iterator jstring::crbegin() const {
  return end();
}

jstring::const_iterator jstring::crend() const {
  return begin();
}

// -- Capacity -----------------------------------------------------------------

bool jstring::empty() const {
  return size() == 0;
}

jstring::size_type jstring::size() const {
  // Cut the zero terminator
  return (is_long_mode() ? long_.size_ : short_.size_) - 1;
}

jstring::size_type jstring::max_size() const {
  return capacity();
}

void jstring::reserve(size_type size) {
  reallocate(size, true);
}

jstring::size_type jstring::capacity() const {
  return (is_long_mode() ? long_.capacity_ : short_capacity);
}

void jstring::shrink_to_fit() {
  // nop - Currently not supported
}

// -- Operations ---------------------------------------------------------------

void jstring::clear() {
  // Free the potentially allocated memory
  if (is_long_mode() && long_.data_)
    delete[] long_.data_;
  // Then zero the objects memory itself
  std::memset(raw_.raw_.data(), 0, sizeof(raw_data));
}

// -- input/output -------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const jstring& str) {
  os.write(str.data(), str.size());
  return os;
}

// -- internal things ----------------------------------------------------------

bool jstring::is_long_mode() const {
  return raw_.is_long_;
}

// -- private member functions -------------------------------------------------

void jstring::reallocate(size_type requested_capacity, bool retain_content) {
  if (!is_long_mode())
    return;
  const auto to_allocate = std::bit_ceil(requested_capacity);
  if (long_.capacity_ >= to_allocate)
    return;
  auto new_ptr = new value_type[to_allocate];
  if (!new_ptr)
    throw std::runtime_error("Could not allocate the requested capacity");
  long_.capacity_ = to_allocate;
  if (retain_content)
    std::memcpy(new_ptr, long_.data_, long_.size_);
  if (long_.data_)
    delete[] long_.data_;
  long_.data_ = new_ptr;
}

} // namespace jstring
