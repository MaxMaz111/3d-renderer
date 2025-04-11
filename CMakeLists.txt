cmake_minimum_required(VERSION 3.16)

project(renderer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Eigen3 3.3 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_SIGNALS_SLOTS_KEYWORDS")

set(SOURCES
    src/controller/controller.cpp
    src/kernel/camera.cpp
    src/kernel/color.cpp
    src/kernel/kernel.cpp
    src/kernel/obj_reader.cpp
    src/kernel/plane.cpp
    src/kernel/renderer.cpp
    src/kernel/scene.cpp
    src/kernel/timer.cpp
    src/kernel/triangle.cpp
    src/view/main_window.cpp
    src/view/view.cpp
    src/application.cpp
    src/except.cpp
    src/main.cpp
)

add_executable(renderer ${SOURCES})

target_include_directories(renderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(renderer PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(renderer PRIVATE /W4)
endif()

target_link_libraries(renderer PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    Eigen3::Eigen
)

install(TARGETS renderer
    RUNTIME DESTINATION bin
)
