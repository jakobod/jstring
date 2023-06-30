/**
 *  @author    Jakob Otto
 *  @file      main.cpp
 *  @copyright Copyright 2023 Jakob Otto. All rights reserved.
 *             This file is part of the jstring project, released under
 *             the GNU GPL3 License.
 */

#include "jstring/jstring.hpp"
#include <cstdlib>
#include <iostream>

int main(int, char**) {
  jstring::jstring str{"Hello World, this is a test string"};
  std::cout << "\"" << str
            << "\" is_long_mode() = " << (str.is_long_mode() ? "true" : "false")
            << std::endl;
  return EXIT_SUCCESS;
}
