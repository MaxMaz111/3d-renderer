cmake_minimum_required(VERSION 3.16)

project(renderer VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

######################################################

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Eigen3 3.3 REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(renderer ${SOURCES})

set_target_properties(renderer PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)

target_include_directories(renderer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/controller
        ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel
        ${CMAKE_CURRENT_SOURCE_DIR}/src/view
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(renderer PRIVATE -Wall -Wextra -Wpedantic)
elseif (MSVC)
    target_compile_options(renderer PRIVATE /W4)
endif ()

target_link_libraries(renderer PUBLIC
        Qt6::Widgets
        Qt6::Core
        Qt6::Gui
        Eigen3::Eigen
)

######################################################

add_executable(renderer_app app/main.cpp)
target_link_libraries(renderer_app PRIVATE renderer)

######################################################

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

if (TEST_SOURCES)
    add_executable(renderer_tests ${TEST_SOURCES})
    target_link_libraries(renderer_tests PRIVATE GTest::gtest_main renderer)
    include(GoogleTest)
    gtest_discover_tests(renderer_tests)
endif ()