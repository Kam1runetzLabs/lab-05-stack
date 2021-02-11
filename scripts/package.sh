#!/usr/bin/env bash

set -e
# shellcheck disable=SC2089
CMAKE_LINKER_OPTS="-DCMAKE_EXE_LINKER='-fuse-ld=gold'"
CPACK_CONFIG_OPTS="-DCPACK_OUTPUT_FILE_PREFIX=$(pwd)/_artifacts -DCPACK_GENERATOR=DEB"
CMAKE_CONFIG_OPTS="-DHUNTER_CONFIGURATION_TYPES=Release -DCMAKE_BUILD_TYPE=Release -DBUILD_DOCS=ON"
CMAKE_TOOLCHAIN_OPTS="-DCMAKE_TOOLCHAIN_FILE='$(pwd)/tools/polly/gcc-pic-cxx17.cmake'"
# shellcheck disable=SC2086
CMAKE_OPTS="$CMAKE_LINKER_OPTS $CMAKE_CONFIG_OPTS $CMAKE_TOOLCHAIN_OPTS $CPACK_CONFIG_OPTS"
cmake -H. -B_builds "$CMAKE_OPTS"
cmake --build _builds --target docs
cmake --build _builds --target package

CPACK_CONFIG_OPTS="-DCPACK_OUTPUT_FILE_PREFIX=$(pwd)/_artifacts -DCPACK_GENERATOR=RPM"
# shellcheck disable=SC2086
CMAKE_OPTS="$CMAKE_LINKER_OPTS $CMAKE_CONFIG_OPTS $CMAKE_TOOLCHAIN_OPTS $CPACK_CONFIG_OPTS"
cmake -H. -B_builds "$CMAKE_OPTS"
cmake --build _builds --target package
