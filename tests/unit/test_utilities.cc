// Unit tests for Utilities module
// Copyright 2025, NGSFeatures Project

#include <string>
#include <vector>

#include <gtest/gtest.h>

// Test fixture for utility functions
class UtilitiesTest : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Sample test: DNA base conversion
TEST_F(UtilitiesTest, DNABaseConversion) {
    // Test basic DNA base to integer conversion
    // A=0, C=1, G=2, T=3
    EXPECT_EQ(0, 0);  // Placeholder - will implement actual conversion tests
}

// Sample test: Vector operations
TEST_F(UtilitiesTest, VectorOperations) {
    std::vector<int> test_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(test_vec.size(), 5);
    EXPECT_EQ(test_vec[0], 1);
    EXPECT_EQ(test_vec[4], 5);
}

// Sample test: String operations
TEST_F(UtilitiesTest, StringOperations) {
    std::string test_str = "ACGT";
    EXPECT_EQ(test_str.length(), 4);
    EXPECT_EQ(test_str[0], 'A');
    EXPECT_EQ(test_str[3], 'T');
}

// Test for file path operations (from Utilities.hh)
TEST_F(UtilitiesTest, FilePathOperations) {
    std::string path = "/path/to/file.txt";
    // Tests will be added once we expose GetBaseNameFromFilename
    // and GetPathNameFromFilename functions
    EXPECT_TRUE(path.find(".txt") != std::string::npos);
}

// Parametrized test example
class QualityScoreTest : public ::testing::TestWithParam<std::pair<double, double>> {};

TEST_P(QualityScoreTest, PhredConversion) {
    auto param = GetParam();
    double input = param.first;
    double expected = param.second;
    // Placeholder for actual Phred conversion tests
    EXPECT_GE(input, 0.0);
}

INSTANTIATE_TEST_SUITE_P(QualityScores, QualityScoreTest,
                         ::testing::Values(std::make_pair(10.0, 0.1), std::make_pair(20.0, 0.01),
                                           std::make_pair(30.0, 0.001)));

// Main function (provided by gtest_main)
