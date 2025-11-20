cmake_minimum_required(VERSION 3.15)

# Project definition
project(NGSFeatures
    VERSION 2.0.0
    DESCRIPTION "Next Generation Sequencing Feature Generation Tool"
    LANGUAGES CXX
)

# ============================================================================
# Project-wide settings
# ============================================================================

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration and clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Release build if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# ============================================================================
# Build options
# ============================================================================

option(BUILD_TESTS "Build test programs" ON)
option(BUILD_DOCS "Build documentation with Doxygen" OFF)
option(ENABLE_LTO "Enable Link-Time Optimization" ON)
option(ENABLE_NATIVE_ARCH "Enable -march=native optimizations" ON)
option(ENABLE_FAST_MATH "Enable fast math optimizations" ON)

# ============================================================================
# Compiler flags
# ============================================================================

# Common flags for all configurations
set(COMMON_FLAGS
    -Wall
    -Wextra
    -Wpedantic
)

# Release-specific flags (matching the original makefiles)
set(RELEASE_FLAGS
    -O3
    -funroll-loops
    -finline-functions
)

# Add native architecture optimizations if enabled
if(ENABLE_NATIVE_ARCH)
    list(APPEND RELEASE_FLAGS -march=native -mtune=native)
    message(STATUS "Native architecture optimizations: ENABLED")
else()
    message(STATUS "Native architecture optimizations: DISABLED")
endif()

# Add fast-math optimizations if enabled
if(ENABLE_FAST_MATH)
    list(APPEND RELEASE_FLAGS -ffast-math)
    message(STATUS "Fast math optimizations: ENABLED")
else()
    message(STATUS "Fast math optimizations: DISABLED")
endif()

# Debug-specific flags
set(DEBUG_FLAGS
    -g
    -O0
    -DGDB_DEBUG
)

# Apply flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(${COMMON_FLAGS} ${RELEASE_FLAGS})
else()
    add_compile_options(${COMMON_FLAGS} ${DEBUG_FLAGS})
endif()

# Enable Link-Time Optimization for Release builds
if(ENABLE_LTO AND (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
        message(STATUS "Link-Time Optimization: ENABLED")
    else()
        message(WARNING "LTO not supported: ${ipo_error}")
    endif()
endif()

# ============================================================================
# Dependencies
# ============================================================================

# Find Boost (required for regex)
find_package(Boost 1.65 REQUIRED COMPONENTS regex)
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")

# ============================================================================
# Subdirectories
# ============================================================================

# Main source directory (core binaries)
add_subdirectory(src)

# Expectation-Matching module
add_subdirectory(ematch_src)

# Knapsack module
add_subdirectory(knapsack_src)

# Tests (if enabled)
if(BUILD_TESTS)
    enable_testing()
    message(STATUS "Building tests: ENABLED")
    add_subdirectory(tests)
endif()

# ============================================================================
# Documentation
# ============================================================================

if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        # Configure Doxygen
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_LATEX NO)

        doxygen_add_docs(docs
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/ematch_src
            ${CMAKE_CURRENT_SOURCE_DIR}/knapsack_src
            ${CMAKE_CURRENT_SOURCE_DIR}/README.md
            COMMENT "Generating API documentation with Doxygen"
        )
        message(STATUS "Doxygen documentation: ENABLED")
    else()
        message(WARNING "Doxygen not found, cannot build documentation")
    endif()
endif()

# ============================================================================
# Installation
# ============================================================================

# Install Python scripts
install(PROGRAMS
    ngsfeatgen.py
    recount_capacity.py
    DESTINATION bin
)

# Install example data
install(FILES
    small-len10-50.txt
    DESTINATION share/ngsfeatures/examples
)

# Install documentation
install(FILES
    README.md
    PYTHON_PORT_README.md
    COPYING
    DESTINATION share/doc/ngsfeatures
)

# ============================================================================
# Summary
# ============================================================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "  NGSFeatures ${PROJECT_VERSION} Configuration")
message(STATUS "========================================")
message(STATUS "  Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard:         C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler:             ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Native optimizations: ${ENABLE_NATIVE_ARCH}")
message(STATUS "  Fast math:            ${ENABLE_FAST_MATH}")
message(STATUS "  LTO:                  ${ENABLE_LTO}")
message(STATUS "  Build tests:          ${BUILD_TESTS}")
message(STATUS "  Build docs:           ${BUILD_DOCS}")
message(STATUS "  Install prefix:       ${CMAKE_INSTALL_PREFIX}")
message(STATUS "========================================")
message(STATUS "")
