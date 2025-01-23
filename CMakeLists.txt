cmake_minimum_required(VERSION 3.16)

project(renderer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  Qt
  GIT_REPOSITORY https://github.com/qt/qtbase.git
  GIT_TAG v6.8.1
)

FetchContent_MakeAvailable(Qt)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
