#!/usr/bin/env bash

set -e

declare -r FILTER=-build/c++11,-runtime/references,-whitespace/braces,-whitespace/indent,-whitespace/comments,-build/include_order

find ./include/ ./scripts/ ./sources/ -name "*.cpp" -or -name "*.hpp" -or -name ".h" | xargs -0 cpplint --filter=$FILTER

export CTEST_OUTPUT_ON_FAILURE=true
# address sanitizer
#CMAKE_LINKER_OPTS="-DCMAKE_EXE_LINKER='-fuse-ld=gold'"
CMAKE_CONFIG_OPTS="-DHUNTER_CONFIGURATION_TYPES=Debug -DCMAKE_BUILD_TYPE=Debug"
# shellcheck disable=SC2089
CMAKE_TOOLCHAIN_OPTS="-DCMAKE_TOOLCHAIN_FILE='$(pwd)/tools/polly/sanitize-address-cxx17-pic.cmake'"
CMAKE_OPTS="$CMAKE_LINKER_OPTS $CMAKE_CONFIG_OPTS $CMAKE_TOOLCHAIN_OPTS"
cmake -H. -B_builds/sanitize-address-cxx17 "$CMAKE_OPTS"
cmake --build _builds/sanitize-address-cxx17
./_builds/sanitize-address-cxx17/stack_tests
./_builds/sanitize-address-cxx17/list_tests
# thread sanitizer
# shellcheck disable=SC2089
CMAKE_TOOLCHAIN_OPTS="-DCMAKE_TOOLCHAIN_FILE='$(pwd)/tools/polly/sanitize-thread-cxx17-pic.cmake'"
CMAKE_OPTS="$CMAKE_LINKER_OPTS $CMAKE_CONFIG_OPTS $CMAKE_TOOLCHAIN_OPTS"
# shellcheck disable=SC2086
# shellcheck disable=SC2090
cmake -H. -B_builds/sanitize-thread-cxx17 $CMAKE_OPTS
cmake --build _builds/sanitize-thread-cxx17
./_builds/sanitize-address-cxx17/stack_tests
./_builds/sanitize-address-cxx17/list_tests
