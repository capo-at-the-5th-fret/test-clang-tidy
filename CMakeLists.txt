cmake_minimum_required(VERSION 3.28.0)

project(test_clang_tidy
    VERSION 0.1.0
    LANGUAGES CXX
)

# https://stackoverflow.com/questions/48026483/setting-cmake-cxx-standard-to-various-values
set(CMAKE_CXX_STANDARD 23 CACHE STRING "C++ standard to be used")

include(FetchContent)

# Tags: https://github.com/fmtlib/fmt/tags
# Linkage: target_link_libraries(fmt::fmt)
set(NEEDED_fmt_VERSION "11.0.2")

FetchContent_Declare(
    fmt
    GIT_REPOSITORY       https://github.com/fmtlib/fmt
    GIT_TAG              ${NEEDED_fmt_VERSION}
    GIT_SHALLOW          TRUE
    FIND_PACKAGE_ARGS    ${NEEDED_fmt_VERSION}
)

FetchContent_MakeAvailable(fmt)

# Tags: https://github.com/catchorg/Catch2/tags
# Linkage: target_link_libraries(Catch2::Catch2WithMain or Catch2::Catch2)
set(NEEDED_Catch2_VERSION "3.7.0")

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY       https://github.com/catchorg/Catch2.git
    GIT_TAG              v${NEEDED_Catch2_VERSION}
    GIT_SHALLOW          TRUE
    FIND_PACKAGE_ARGS    ${NEEDED_Catch2_VERSION}
)

FetchContent_MakeAvailable(Catch2)

# this needs to be called for the catch_discover_tests command to work
include(Catch)

add_executable(test_clang_tidy

    cmath_tests.cpp
    sample_tests.cpp

)

target_include_directories(test_clang_tidy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_clang_tidy PRIVATE fmt::fmt Catch2::Catch2WithMain)

catch_discover_tests(test_clang_tidy)