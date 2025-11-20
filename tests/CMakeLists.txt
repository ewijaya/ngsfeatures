# Google Test integration for NGSFeatures
cmake_minimum_required(VERSION 3.15)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make Google Test available
FetchContent_MakeAvailable(googletest)

# Enable CTest
enable_testing()

# Add unit tests subdirectory
add_subdirectory(unit)

# Add integration tests subdirectory
# (will be populated later)
# add_subdirectory(integration)

# Create a target to run all tests
add_custom_target(
    run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests..."
)
