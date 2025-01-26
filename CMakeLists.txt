cmake_minimum_required(VERSION 3.16)

project(renderer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Eigen3 3.3 REQUIRED)

add_executable(renderer src/main.cpp)

target_link_libraries(renderer PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIRS})
