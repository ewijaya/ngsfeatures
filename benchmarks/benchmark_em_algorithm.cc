// =====================================================================================
// Benchmark for EM Algorithm Performance
// Copyright 2025, NGSFeatures Project
// =====================================================================================

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Simple timing utilities
class Timer {
   public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }

    double elapsed_ms() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;
    }

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

// Mock EM algorithm for benchmarking
class EMBenchmark {
   public:
    void run(size_t num_sequences, size_t num_iterations) {
        // Generate random test data
        std::vector<double> counts(num_sequences);
        std::vector<double> proportions(num_sequences);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 100.0);

        for (size_t i = 0; i < num_sequences; ++i) {
            counts[i] = dis(gen);
            proportions[i] = 1.0 / num_sequences;
        }

        Timer timer;
        timer.start();

        // Run EM iterations
        for (size_t iter = 0; iter < num_iterations; ++iter) {
            // E-step: compute expected counts
            std::vector<double> expected(num_sequences);
            double lambda = std::accumulate(counts.begin(), counts.end(), 0.0);

            for (size_t i = 0; i < num_sequences; ++i) {
                expected[i] = proportions[i] * lambda;
            }

            // M-step: update proportions
            double total = std::accumulate(expected.begin(), expected.end(), 0.0);
            for (size_t i = 0; i < num_sequences; ++i) {
                proportions[i] = expected[i] / total;
            }
        }

        double elapsed = timer.elapsed_ms();
        double iter_per_sec = (num_iterations * 1000.0) / elapsed;

        std::cout << "Sequences: " << num_sequences << ", Iterations: " << num_iterations
                  << std::endl;
        std::cout << "  Time: " << elapsed << " ms" << std::endl;
        std::cout << "  Throughput: " << iter_per_sec << " iterations/sec" << std::endl;
        std::cout << "  Per-iteration: " << (elapsed / num_iterations) << " ms" << std::endl;
    }
};

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "EM Algorithm Performance Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    EMBenchmark bench;

    // Small dataset
    std::cout << "Small dataset:" << std::endl;
    bench.run(100, 50);
    std::cout << std::endl;

    // Medium dataset
    std::cout << "Medium dataset:" << std::endl;
    bench.run(1000, 50);
    std::cout << std::endl;

    // Large dataset
    std::cout << "Large dataset:" << std::endl;
    bench.run(10000, 50);
    std::cout << std::endl;

    // Very large dataset
    std::cout << "Very large dataset:" << std::endl;
    bench.run(100000, 50);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Benchmark complete!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
