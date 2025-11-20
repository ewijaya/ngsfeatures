/**
 * @file Utilities.hh
 * @brief Utility functions for NGS sequence processing and DNA manipulation
 *
 * This header provides core utility functions for:
 * - File path operations
 * - DNA sequence conversions (ACGT <-> numeric)
 * - Vector and map printing utilities
 * - Template-based generic printing functions
 *
 * @note IMPORTANT: This is a header file. Never use 'using namespace std;' in headers!
 *       It pollutes the namespace of all files that include this header.
 *       All standard library types use explicit std:: prefix.
 *
 * @author Edward Wijaya
 * @date 2009-2025
 * @copyright Copyright 2009-2025, NGSFeatures Project
 */

#ifndef UTILITIES_HH
#define UTILITIES_HH

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <cstdlib>

/**
 * @brief Extract base filename from a full file path
 *
 * Removes the directory path and file extension, returning only the base name.
 *
 * @param fileName Full file path (e.g., "/path/to/file.txt")
 * @return Base filename without path or extension (e.g., "file")
 *
 * @par Example:
 * @code
 * std::string path = "/home/user/data/sequences.fastq";
 * std::string base = GetBaseNameFromFilename(path);
 * // base == "sequences"
 * @endcode
 */
std::string GetBaseNameFromFilename(const std::string& fileName);

/**
 * @brief Extract directory path from a full file path
 *
 * Returns the directory portion of a file path, excluding the filename.
 *
 * @param filename Full file path (e.g., "/path/to/file.txt")
 * @return Directory path with trailing slash (e.g., "/path/to/")
 *
 * @par Example:
 * @code
 * std::string path = "/home/user/data/sequences.fastq";
 * std::string dir = GetPathNameFromFilename(path);
 * // dir == "/home/user/data/"
 * @endcode
 */
std::string GetPathNameFromFilename(const std::string& filename);

/**
 * @brief Convert numeric ID to DNA tag representation
 *
 * Converts a numeric identifier to its corresponding DNA sequence representation
 * as a vector of integers (A=0, C=1, G=2, T=3).
 *
 * @param id Numeric identifier to convert
 * @param tgl Tag length (sequence length)
 * @return Vector of integers representing DNA bases
 *
 * @par Example:
 * @code
 * std::vector<int> tag = id2tagnum(5, 3);
 * // Returns numeric representation of DNA sequence
 * @endcode
 */
std::vector<int> id2tagnum(int id, int tgl);

/**
 * @brief Convert ACGT string to numeric representation
 *
 * Converts DNA sequence string (ACGT) to vector of integers.
 * Mapping: A=0, C=1, G=2, T=3
 *
 * @param arg DNA sequence string (e.g., "ACGT")
 * @return Vector of integers representing each base
 *
 * @par Example:
 * @code
 * std::string seq = "ACGT";
 * std::vector<int> nums = acgt2tagnum(seq);
 * // nums == {0, 1, 2, 3}
 * @endcode
 */
std::vector<int> acgt2tagnum(const std::string& arg);

/**
 * @brief Convert numeric representation back to ACGT characters
 *
 * Converts vector of integers back to DNA sequence characters.
 * Mapping: 0=A, 1=C, 2=G, 3=T
 *
 * @param arg Vector of integers representing DNA bases
 * @return Vector of characters (ACGT)
 *
 * @par Example:
 * @code
 * std::vector<int> nums = {0, 1, 2, 3};
 * std::vector<char> seq = tagnum2acgt(nums);
 * // seq == {'A', 'C', 'G', 'T'}
 * @endcode
 */
std::vector<char> tagnum2acgt(const std::vector<int>& arg);

/**
 * @brief Print single value from vector at specified index
 *
 * Prints one or two elements from a vector at the specified limit index.
 *
 * @tparam T Element type (must support operator<<)
 * @param arg Vector to print from
 * @param sep Separator string (default: "")
 * @param lim Index to print (prints elements at lim and lim+1)
 *
 * @par Example:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * prn_vec_oneval(vec, " ", 2);  // Prints: 3 4
 * @endcode
 */
template <typename T>
void prn_vec_oneval(const std::vector<T>& arg, const std::string& sep = "", unsigned lim = 0) {
    for (unsigned n = 0; n < arg.size(); n++) {
        if (n >= lim && n <= (lim + 1)) {
            std::cout << arg[n] << sep;
        }
    }
}

/**
 * @brief Print all elements of a vector to stdout
 *
 * Generic template function to print vector elements with optional separator.
 *
 * @tparam T Element type (must support operator<<)
 * @param arg Vector to print
 * @param sep Separator string between elements (default: "")
 *
 * @par Example:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * prn_vec(vec, ", ");  // Prints: 1, 2, 3, 4, 5,
 * @endcode
 */
template <typename T>
void prn_vec(const std::vector<T>& arg, const std::string& sep = "") {
    for (unsigned n = 0; n < arg.size(); n++) {
        std::cout << arg[n] << sep;
    }
}

/**
 * @brief Print vector elements to a specified output stream
 *
 * Generic template function to print vector to any output stream (file, cout, etc.).
 *
 * @tparam T Element type (must support operator<<)
 * @param arg Vector to print
 * @param fn Output stream to write to
 * @param sep Separator string between elements (default: "")
 *
 * @par Example:
 * @code
 * std::ofstream file("output.txt");
 * std::vector<double> vec = {1.5, 2.5, 3.5};
 * prn_vec_binos(vec, file, "\n");  // Write to file with newlines
 * @endcode
 */
template <typename T>
void prn_vec_binos(const std::vector<T>& arg, std::ostream& fn, const std::string& sep = "") {
    for (unsigned n = 0; n < arg.size(); n++) {
        fn << arg[n] << sep;
    }
}

/**
 * @brief Print vector elements to stderr
 *
 * Prints vector elements to standard error stream, useful for debugging and logging.
 *
 * @tparam T Element type (must support operator<<)
 * @param arg Vector to print
 * @param sep Separator string between elements (default: "")
 *
 * @par Example:
 * @code
 * std::vector<std::string> errors = {"Error1", "Error2"};
 * prn_vec_err(errors, "\n");  // Print errors to stderr
 * @endcode
 */
template <typename T>
void prn_vec_err(const std::vector<T>& arg, const std::string& sep = "") {
    for (size_t n = 0; n < arg.size(); n++) {
        std::cerr << arg[n] << sep;
    }
}

/**
 * @brief Print all key-value pairs from a map
 *
 * Prints map contents in "key->value" format with optional separator.
 *
 * @tparam T1 Key type (must support operator<<)
 * @tparam T2 Value type (must support operator<<)
 * @param arg Map to print
 * @param sep Separator string between pairs (default: "")
 *
 * @par Example:
 * @code
 * std::map<std::string, int> m = {{"A", 1}, {"C", 2}};
 * prn_map(m, " ");  // Prints: A->1 C->2
 * @endcode
 */
template <typename T1, typename T2>
void prn_map(const std::map<T1, T2>& arg, const std::string& sep = "") {
    for (typename std::map<T1, T2>::const_iterator iter = arg.begin(); iter != arg.end(); ++iter) {
        std::cout << iter->first << "->" << iter->second << sep;
    }
}

/**
 * @brief Print vector of floating-point numbers with fixed precision
 *
 * Prints floating-point vector elements using printf with 7 decimal places.
 *
 * @tparam T Numeric type (typically double or float)
 * @param arg Vector of numeric values to print
 *
 * @par Example:
 * @code
 * std::vector<double> values = {1.234567, 2.345678, 3.456789};
 * prnf_vec(values);  // Prints: 1.2345670 2.3456780 3.4567890
 * @endcode
 */
template <typename T>
void prnf_vec(const std::vector<T>& arg) {
    for (size_t n = 0; n < arg.size(); n++) {
        std::printf("%.7f\t", arg[n]);
    }
}

#endif  // UTILITIES_HH
