# Unit tests for NGSFeatures

# Test for utilities
add_executable(test_utilities test_utilities.cc)
target_link_libraries(test_utilities
    PRIVATE
    GTest::gtest_main
    GTest::gmock
)
target_include_directories(test_utilities PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Register with CTest
include(GoogleTest)
gtest_discover_tests(test_utilities)

# Add more test executables here as they are created
# Example:
# add_executable(test_em_algorithm test_em_algorithm.cc)
# target_link_libraries(test_em_algorithm PRIVATE GTest::gtest_main)
# gtest_discover_tests(test_em_algorithm)
