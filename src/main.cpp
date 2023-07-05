/**
 *  @author    Jakob Otto
 *  @file      main.cpp
 *  @copyright Copyright 2023 Jakob Otto. All rights reserved.
 *             This file is part of the jstring project, released under
 *             the GNU GPL3 License.
 */

#include <concepts>
#include <cstdlib>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace {

struct none {
  friend std::ostream& operator<<(std::ostream& os, const none&) {
    return os << "none";
  }
};

struct Apple {};

struct Peach {};

struct Cucumber {};

struct Orange {};

struct Tomato {
  std::string operator()() { return "tomato"; }
};

using Vegetable = std::variant<Apple, Peach, Cucumber, Orange, Tomato>;

// helper type for the visitor #4
template <class... Ts>
struct behavior : Ts... {
  using Ts::operator()...;
};

// explicit deduction guide (not needed as of C++20)
template <class... Ts>
behavior(Ts...) -> behavior<Ts...>;

int visit(const Apple&) {
  std::cout << "This is an apple" << std::endl;
  return 42;
}

std::string visit(const Peach&) {
  std::cout << "This is a peach" << std::endl;
  return "yolo man";
}

double visit(const Orange&) {
  std::cout << "This is a orange" << std::endl;
  return 69.420;
}

none visit(const auto&) {
  std::cout << "This is some other type" << std::endl;
  return {};
}

} // namespace

int main() {
  static constexpr const std::array<Vegetable, 5> vegetables{
    Apple{}, Cucumber{}, Peach{}, Orange{}, Tomato{}};
  for (const auto& veg : vegetables)
    std::visit(
      behavior{
        [](const Apple&) { std::cout << "This is an apple" << std::endl; },
        [](const Peach&) { std::cout << "This is a peach" << std::endl; },
        [](const Cucumber&) { std::cout << "this is a cucumber" << std::endl; },
        [](const auto& obj) {
          if constexpr (std::is_invocable_r_v<
                          std::string, std::remove_reference<decltype(obj)>>) {
            std::cout << "this is invocable: " << obj();
          } else {
            std::cout << "this is something else" << std::endl;
          }
        },
      },
      veg);

  // for (const auto& veg : vegetables) {
  //   auto res = std::visit([](const auto& veg) { return visit(veg); }, veg);
  //   std::cout << "res = " << res << std::endl;
  // }

  return EXIT_SUCCESS;
}
