# Benchmark suite for NGSFeatures
cmake_minimum_required(VERSION 3.15)

# Benchmark executable
add_executable(benchmark_em benchmark_em_algorithm.cc)

# Enable aggressive optimizations for benchmarking
target_compile_options(
    benchmark_em
    PRIVATE -O3
            -march=native
            -mtune=native
            -flto
            -ffast-math
            $<$<CONFIG:Release>:-DNDEBUG>)

# Link-time optimization
set_target_properties(benchmark_em PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

# Installation
install(TARGETS benchmark_em RUNTIME DESTINATION bin/benchmarks)

# Custom target to run all benchmarks
add_custom_target(
    run_benchmarks
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/benchmark_em
    DEPENDS benchmark_em
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running performance benchmarks...")

# Target to run benchmarks with profiling
add_custom_target(
    profile_benchmarks
    COMMAND perf record -g ${CMAKE_CURRENT_BINARY_DIR}/benchmark_em
    DEPENDS benchmark_em
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running benchmarks with perf profiling...")

# Target to generate flamegraph
add_custom_target(
    flamegraph
    COMMAND perf script | c++filt | flamegraph.pl > flamegraph.svg
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating flamegraph from perf data...")
