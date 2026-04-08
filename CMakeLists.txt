##
## EPITECH PROJECT, 2026
## Arcade
## File description:
## CMakeLists
##

cmake_minimum_required(VERSION 3.20)

project(arcade VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

# Output binaries and shared libraries to the repo root
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Core / arcade binary

file(GLOB_RECURSE SRC_CORE
    src/main.cpp
    src/Core/Core.cpp
    src/Core/DLLoader.cpp
    src/Core/ScoreManager.cpp
)

add_executable(${PROJECT_NAME} ${SRC_CORE})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE dl)

# NcursesDisplay plugin

find_package(Curses REQUIRED)

add_library(arcade_ncurses SHARED
    plugins/NcursesDisplay/NcursesDisplay.cpp
)
set_target_properties(arcade_ncurses PROPERTIES PREFIX "")
target_include_directories(arcade_ncurses PRIVATE include plugins/NcursesDisplay ${CURSES_INCLUDE_DIR})
target_link_libraries(arcade_ncurses PRIVATE ${CURSES_LIBRARIES})
target_compile_options(arcade_ncurses PRIVATE)

# SfmlDisplay plugin

find_package(SFML 2 REQUIRED COMPONENTS graphics window system audio)

add_library(arcade_sfml SHARED
    plugins/SfmlDisplay/SfmlDisplay.cpp
)
set_target_properties(arcade_sfml PROPERTIES PREFIX "")
target_include_directories(arcade_sfml PRIVATE include plugins/SfmlDisplay)
target_link_libraries(arcade_sfml PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
target_compile_options(arcade_sfml PRIVATE)

# Snake plugin

add_library(arcade_snake SHARED
    plugins/Snake/Snake.cpp
    plugins/Snake/SnakeEntity.cpp
)
set_target_properties(arcade_snake PROPERTIES PREFIX "")
target_include_directories(arcade_snake PRIVATE include plugins/Snake)
target_compile_options(arcade_snake PRIVATE)

# Centipede plugin

add_library(arcade_centipede SHARED
    plugins/Centipede/Centipede.cpp
    plugins/Centipede/CentipedeEntity.cpp
)
set_target_properties(arcade_centipede PROPERTIES PREFIX "")
target_include_directories(arcade_centipede PRIVATE include plugins/Centipede)
target_compile_options(arcade_centipede PRIVATE)

# Menu plugin

add_library(arcade_menu SHARED
    plugins/Menu/Menu.cpp
    plugins/Menu/MenuEntity.cpp
    src/Core/DLLoader.cpp
)
set_target_properties(arcade_menu PROPERTIES PREFIX "")
target_include_directories(arcade_menu PRIVATE include plugins/Menu)
target_link_libraries(arcade_menu PRIVATE dl)
target_compile_options(arcade_menu PRIVATE)

# Tests

option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()

    find_library(CRITERION_LIB criterion REQUIRED)

    file(GLOB_RECURSE TEST_FILES tests/*.cpp)

    file(GLOB_RECURSE SRC_NO_MAIN
        src/Core/Core.cpp
        src/Core/DLLoader.cpp
    )

    add_executable(unit_tests ${TEST_FILES} ${SRC_NO_MAIN})
    target_include_directories(unit_tests PRIVATE include)
    target_link_libraries(unit_tests PRIVATE ${CRITERION_LIB} dl)
    target_link_options(unit_tests PRIVATE --coverage)
    target_compile_options(unit_tests PRIVATE --coverage)

    add_test(NAME unit_tests COMMAND unit_tests)
endif()
