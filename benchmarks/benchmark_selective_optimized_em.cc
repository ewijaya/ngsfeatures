// =====================================================================================
// Benchmark for Selective Optimized EM Algorithm Performance
// Only parallelizes expensive operations (log computations)
// Keeps vector operations serial to avoid overhead
// Copyright 2025, NGSFeatures Project
// =====================================================================================

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include <cmath>

#ifdef _OPENMP
#include <omp.h>
#endif

// Threshold for when to parallelize logarithm computations
const size_t LOG_PARALLEL_THRESHOLD = 1000;

// ============================================================================
// ORIGINAL IMPLEMENTATIONS (non-optimized)
// ============================================================================

double computeLogLik_original(const std::vector<double>& m, const std::vector<double>& p,
                              const double& lmbd) {
    double Result = 0.0;
    for (unsigned i = 0; i < p.size(); i++) {
        Result += m[i] * log(p[i] * lmbd);
    }
    return (-lmbd + Result);
}

std::vector<double> divideVecWithScalar_original(const std::vector<double>& Vec,
                                                 const double& val) {
    std::vector<double> Result;
    Result.reserve(Vec.size());
    const double inverse = 1.0 / val;
    for (unsigned i = 0; i < Vec.size(); i++) {
        Result.push_back(Vec[i] * inverse);
    }
    return Result;
}

std::vector<double> divideVecWithVecCorsp_original(const std::vector<double>& Vec1,
                                                   const std::vector<double>& Vec2) {
    std::vector<double> Result;
    Result.reserve(Vec1.size());
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result.push_back(Vec1[i] / Vec2[i]);
    }
    return Result;
}

std::vector<double> multiplyVecWithVecCorsp_original(const std::vector<double>& Vec1,
                                                     const std::vector<double>& Vec2) {
    std::vector<double> Result;
    Result.reserve(Vec1.size());
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result.push_back(Vec1[i] * Vec2[i]);
    }
    return Result;
}

std::vector<double> sparseM_vec_prod_original(const std::vector<double>& p,
                                              const std::vector<int>& rowId,
                                              const std::vector<int>& colId,
                                              const std::vector<double>& realVal) {
    std::vector<double> Result(p.size(), 0.0);
    for (unsigned i = 0; i < rowId.size(); i++) {
        Result[rowId[i]] += realVal[i] * p[colId[i]];
    }
    return Result;
}

// ============================================================================
// SELECTIVE OPTIMIZED IMPLEMENTATIONS
// Only parallelize expensive log() computations
// Keep vector operations serial for cache efficiency
// ============================================================================

double computeLogLik_selective(const std::vector<double>& m, const std::vector<double>& p,
                               const double& lmbd) {
    double Result = 0.0;

    // Log is expensive - parallelize this
    if (p.size() >= LOG_PARALLEL_THRESHOLD) {
#pragma omp parallel for reduction(+ : Result) schedule(static, 64)
        for (size_t i = 0; i < p.size(); i++) {
            Result += m[i] * log(p[i] * lmbd);
        }
    } else {
        for (size_t i = 0; i < p.size(); i++) {
            Result += m[i] * log(p[i] * lmbd);
        }
    }

    return (-lmbd + Result);
}

// Vector operations kept serial - they're already fast and cache-friendly
std::vector<double> divideVecWithScalar_selective(const std::vector<double>& Vec,
                                                  const double& val) {
    std::vector<double> Result(Vec.size());
    const double inverse = 1.0 / val;

    // Simple operations - keep serial for cache efficiency
    for (size_t i = 0; i < Vec.size(); i++) {
        Result[i] = Vec[i] * inverse;
    }

    return Result;
}

std::vector<double> divideVecWithVecCorsp_selective(const std::vector<double>& Vec1,
                                                    const std::vector<double>& Vec2) {
    std::vector<double> Result(Vec1.size());

    // Simple operations - keep serial
    for (size_t i = 0; i < Vec1.size(); i++) {
        Result[i] = Vec1[i] / Vec2[i];
    }

    return Result;
}

std::vector<double> multiplyVecWithVecCorsp_selective(const std::vector<double>& Vec1,
                                                      const std::vector<double>& Vec2) {
    std::vector<double> Result(Vec1.size());

    // Simple operations - keep serial
    for (size_t i = 0; i < Vec1.size(); i++) {
        Result[i] = Vec1[i] * Vec2[i];
    }

    return Result;
}

// Sparse matrix operations - keep serial due to poor cache locality when parallel
std::vector<double> sparseM_vec_prod_selective(const std::vector<double>& p,
                                               const std::vector<int>& rowId,
                                               const std::vector<int>& colId,
                                               const std::vector<double>& realVal) {
    std::vector<double> Result(p.size(), 0.0);

    // Random access pattern - keep serial for cache efficiency
    for (size_t i = 0; i < rowId.size(); i++) {
        Result[rowId[i]] += realVal[i] * p[colId[i]];
    }

    return Result;
}

// ============================================================================
// Timer utility
// ============================================================================

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

// ============================================================================
// Benchmark functions
// ============================================================================

void benchmark_em_iteration(size_t num_sequences, size_t num_iterations, int version) {
    // Generate test data
    std::vector<double> rawCount(num_sequences);
    std::vector<double> theM(num_sequences);
    std::vector<double> theP(num_sequences);

    // Create sparse matrix
    size_t sparse_size = num_sequences * 3;
    std::vector<int> IA(sparse_size);
    std::vector<int> JA(sparse_size);
    std::vector<double> RA(sparse_size);

    std::random_device rd;
    std::mt19937 gen(42);
    std::uniform_real_distribution<> count_dist(1.0, 100.0);
    std::uniform_real_distribution<> prop_dist(0.01, 0.1);

    for (size_t i = 0; i < num_sequences; ++i) {
        rawCount[i] = count_dist(gen);
        theM[i] = rawCount[i];
        theP[i] = 1.0 / num_sequences;
    }

    for (size_t i = 0; i < sparse_size; ++i) {
        IA[i] = i % num_sequences;
        JA[i] = (i + 1) % num_sequences;
        RA[i] = prop_dist(gen);
    }

    double lambda = static_cast<double>(num_sequences);

    Timer timer;
    timer.start();

    // Run EM iterations
    for (size_t iter = 0; iter < num_iterations; ++iter) {
        std::vector<double> sparseM_prod_P;
        std::vector<double> rSums;
        std::vector<double> tsparseM_prod_rSums;
        double logLik;

        if (version == 0) {  // Original
            theP = divideVecWithScalar_original(theM, lambda);
            sparseM_prod_P = sparseM_vec_prod_original(theP, IA, JA, RA);
            rSums = divideVecWithVecCorsp_original(rawCount, sparseM_prod_P);
            tsparseM_prod_rSums = sparseM_vec_prod_original(rSums, JA, IA, RA);
            theM = multiplyVecWithVecCorsp_original(theP, tsparseM_prod_rSums);
            logLik = computeLogLik_original(theM, theP, lambda);
        } else {  // Selective optimized
            theP = divideVecWithScalar_selective(theM, lambda);
            sparseM_prod_P = sparseM_vec_prod_selective(theP, IA, JA, RA);
            rSums = divideVecWithVecCorsp_selective(rawCount, sparseM_prod_P);
            tsparseM_prod_rSums = sparseM_vec_prod_selective(rSums, JA, IA, RA);
            theM = multiplyVecWithVecCorsp_selective(theP, tsparseM_prod_rSums);
            logLik = computeLogLik_selective(theM, theP, lambda);
        }
    }

    double elapsed = timer.elapsed_ms();
    double iter_per_sec = (num_iterations * 1000.0) / elapsed;

    const char* label = (version == 0) ? "Original " : "Optimized";
    std::cout << "  " << label << ": " << std::fixed << std::setprecision(3) << elapsed << " ms ("
              << std::setprecision(0) << iter_per_sec << " iter/sec)";
    if (version == 0) {
        std::cout << std::endl;
    }
}

int main() {
#ifdef _OPENMP
    std::cout << "========================================" << std::endl;
    std::cout << "OpenMP enabled with " << omp_get_max_threads() << " threads" << std::endl;
    std::cout << "Strategy: Selective parallelization" << std::endl;
    std::cout << "  - Only parallelize log() operations" << std::endl;
    std::cout << "  - Keep vector ops serial (cache-friendly)" << std::endl;
    std::cout << "  - Log parallel threshold: " << LOG_PARALLEL_THRESHOLD << std::endl;
    std::cout << "========================================" << std::endl;
#else
    std::cout << "========================================" << std::endl;
    std::cout << "WARNING: OpenMP not enabled!" << std::endl;
    std::cout << "========================================" << std::endl;
#endif
    std::cout << std::endl;

    std::cout << "EM Algorithm Selective Optimization Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    const size_t num_iterations = 50;

    struct TestCase {
        size_t sequences;
        const char* name;
    };

    TestCase test_cases[] = {{100, "Small dataset (100 sequences)"},
                             {1000, "Medium dataset (1,000 sequences)"},
                             {10000, "Large dataset (10,000 sequences)"},
                             {100000, "Very large dataset (100,000 sequences)"}};

    for (const auto& test : test_cases) {
        std::cout << test.name << " - 50 iterations:" << std::endl;

        // Measure original
        Timer timer1;
        timer1.start();
        benchmark_em_iteration(test.sequences, num_iterations, 0);
        double original_time = timer1.elapsed_ms();

        // Measure optimized
        Timer timer2;
        timer2.start();
        benchmark_em_iteration(test.sequences, num_iterations, 1);
        double optimized_time = timer2.elapsed_ms();

        double speedup = original_time / optimized_time;
        std::cout << std::fixed << std::setprecision(2) << " [Speedup: " << speedup << "x]"
                  << std::endl;
        std::cout << std::endl;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "Benchmark complete!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
