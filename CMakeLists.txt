cmake_minimum_required(VERSION 3.29)

# Add compiler choice variable with default value
set(COMPILER_CHOICE "clang" CACHE STRING "Choose compiler: gcc, clang, or msvc")
# Set allowed values for the compiler choice
set_property(CACHE COMPILER_CHOICE PROPERTY STRINGS gcc clang msvc)

# Use the compiler choice to set the compiler
if (COMPILER_CHOICE STREQUAL "clang")
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)

    # Check if environment variables exist
    if (DEFINED ENV{clang_include} AND DEFINED ENV{clang_lib})
        include_directories($ENV{clang_include})
        link_directories($ENV{clang_lib})
    else ()
        message(FATAL_ERROR "clang_include and/or clang_lib environment variables are not set")
    endif ()

    set(CMAKE_GENERATOR "MSYS Makefiles")

elseif (COMPILER_CHOICE STREQUAL "gcc")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)

    set(CMAKE_GENERATOR "MinGW Makefiles")

elseif (COMPILER_CHOICE STREQUAL "msvc")
    # MSVC is automatically set on Windows when using Visual Studio
    # No need to set compiler explicitly
endif ()

project(repl)

set(CMAKE_CXX_STANDARD 17)

# Debug/Release configuration settings
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Create separate output directories for Debug and Release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# Include FetchContent
include(FetchContent)

# Declare fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
)

# Declare GoogleTest
FetchContent_Declare(
        GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

# Make the libraries available
FetchContent_MakeAvailable(fmt GTest)

############################################################
# Create a library
############################################################
file(GLOB REPL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/repl/*.cpp)
add_library(repl_library STATIC
        ${REPL_SOURCES}
)
add_library(repl::library ALIAS repl_library)

target_include_directories(repl_library
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include
)

target_link_libraries(repl_library
        PRIVATE
        fmt::fmt-header-only
)

############################################################
# Create Main executables (Debug and Release)
############################################################

# Debug executable
add_executable(repl_debug src/main.cpp)
set_target_properties(repl_debug PROPERTIES
        OUTPUT_NAME "repl"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
        COMPILE_FLAGS ${CMAKE_CXX_FLAGS_DEBUG}
)

target_include_directories(repl_debug
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include
)

target_link_libraries(repl_debug
        PRIVATE
        repl::library
        fmt::fmt-header-only
)

# Release executable
add_executable(repl_release src/main.cpp)
set_target_properties(repl_release PROPERTIES
        OUTPUT_NAME "repl"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
        COMPILE_FLAGS ${CMAKE_CXX_FLAGS_RELEASE}
)

target_include_directories(repl_release
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include
)

target_link_libraries(repl_release
        PRIVATE
        repl::library
        fmt::fmt-header-only
)

############################################################
# Create Test executable
############################################################
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
add_executable(unit_tests
        ${TEST_SOURCES}
)

set_target_properties(unit_tests PROPERTIES
        OUTPUT_NAME "unit_tests"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
        COMPILE_FLAGS ${CMAKE_CXX_FLAGS_DEBUG}
)

target_include_directories(unit_tests
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include
)

# Link libraries to test executable
target_link_libraries(unit_tests PRIVATE
        repl::library
        GTest::gtest_main
)

############################################################
# Add LLVM only for clang compiler
############################################################
if (COMPILER_CHOICE STREQUAL "clang")
    target_link_libraries(repl_library PRIVATE LLVM-19)
    target_link_libraries(repl_debug PRIVATE LLVM-19)
    target_link_libraries(repl_release PRIVATE LLVM-19)
    target_link_libraries(unit_tests PRIVATE LLVM-19)
endif ()
