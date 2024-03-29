cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

set(
        HUNTER_CACHE_SERVERS
        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
        CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
        SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)

project(stack VERSION 0.1.0)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
#        ${CMAKE_CURRENT_SOURCE_DIR}/sources/source.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/stack.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/list.hpp)

add_executable(demo
        ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
        )

target_include_directories(${PROJECT_NAME} INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
        )

target_link_libraries(demo ${PROJECT_NAME})

if (BUILD_TESTS)
    add_executable(list_tests
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/list_tests.cpp)
    add_executable(stack_tests
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/stack_tests.cpp)
    target_link_libraries(list_tests ${PROJECT_NAME} GTest::gtest_main)
    target_link_libraries(stack_tests ${PROJECT_NAME} GTest::gtest_main)
    enable_testing()
    add_test(NAME unit_stack_tests COMMAND stack_tests)
    add_test(NAME unit_list_tests COMMAND list_tests)
endif ()

if (BUILD_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage/cmake")
    find_package(codecov)
    #    add_coverage(${PROJECT_NAME})
    add_coverage(stack_tests)
    add_coverage(list_tests)
    coverage_evaluate()
endif ()

if (BUILD_DOCS)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/_docs)
    set(DOXYGEN_EXAMPLE_PATH examples)
    set(DOXYGEN_GENERATE_HTML NO)
    set(DOXYGEN_GENERATE_MAN YES)
    find_package(Doxygen)
    doxygen_add_docs(docs include)
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/man DESTINATION share)
endif ()

include(CPackConfig.cmake)

install(TARGETS ${PROJECT_NAME} demo
        EXPORT "${PROJECT_NAME}-targets"
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
        )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(EXPORT "${PROJECT_NAME}-targets"
        NAMESPACE "${PROJECT_NAME}::"
        DESTINATION "lib/cmake"
        )
