# -- project setup -------------------------------------------------------------

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(lib_jstring CXX)

include(FetchContent)
include(GoogleTest)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# -- set useful CMake options --------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_compile_options(-Wall -Wextra -pedantic)

message (STATUS "Build type ${CMAKE_BUILD_TYPE}")

# -- find or fetch dependencies ------------------------------------------------

# Threads
find_package(Threads REQUIRED)

# Googletest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

# -- project files -------------------------------------------------------------

# Find the packet-routing headers
file(GLOB_RECURSE LIB_JSTRING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/libjstring/*.hpp")

# New source files have to be added here
set(LIB_JSTRING_SOURCES
  src/jstring/jstring.cpp
)

# -- Object for different targets ----------------------------------------------

add_library(jstring STATIC ${LIB_JSTRING_HEADERS} ${LIB_JSTRING_SOURCES})
set_property(TARGET jstring PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(jstring PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libjstring")

# -- main for playing with things ----------------------------------------------

macro(add_target name)
  add_executable(${name} "src/${name}.cpp")
  target_include_directories(${name} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/libjstring")
  target_link_libraries(${name} PRIVATE jstring)
endmacro()

add_target(main)

# -- test setup ----------------------------------------------------------------

enable_testing()

add_executable(
  lib_jstring_test
  test/jstring.cpp 
)

target_include_directories(lib_jstring_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libjstring")
target_include_directories(lib_jstring_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test")

target_link_libraries(
  lib_jstring_test
  jstring
  gtest
  gtest_main
)

gtest_discover_tests(lib_jstring_test)
