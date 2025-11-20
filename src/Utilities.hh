 //=====================================================================================
 // Filename:  Utilities.hh
 //
 // IMPORTANT: This is a header file. Never use 'using namespace std;' in headers!
 // It pollutes the namespace of all files that include this header.
 // =====================================================================================
#ifndef UTILITIES_HH
#define UTILITIES_HH

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdlib>

// Note: Removed 'using namespace std;' to prevent namespace pollution
// All standard library types now use explicit std:: prefix

std::string GetBaseNameFromFilename(const std::string& fileName);
std::string GetPathNameFromFilename(const std::string& filename);
std::vector<int> id2tagnum(int id, int tgl);
std::vector<int> acgt2tagnum(const std::string& arg);
std::vector<char> tagnum2acgt(const std::vector<int>& arg); 


template <typename T>
void prn_vec_oneval(const std::vector<T>& arg, const std::string& sep = "", unsigned lim = 0)
{
	for (unsigned n = 0; n < arg.size(); n++) {
        if (n >= lim && n <= (lim + 1)) {
			std::cout << arg[n] << sep;
        }
	}
}

template <typename T>
void prn_vec(const std::vector<T>& arg, const std::string& sep = "")
{
	for (unsigned n = 0; n < arg.size(); n++) {
		std::cout << arg[n] << sep;
	}
}

template <typename T>
void prn_vec_binos(const std::vector<T>& arg, std::ostream& fn, const std::string& sep = "")
{
	for (unsigned n = 0; n < arg.size(); n++) {
		fn << arg[n] << sep;
	}
}


template <typename T>
void prn_vec_err(const std::vector<T>& arg, const std::string& sep = "")
{
	for (size_t n = 0; n < arg.size(); n++) {
		std::cerr << arg[n] << sep;
	}
}


template <typename T1, typename T2>
void prn_map(const std::map<T1, T2>& arg, const std::string& sep = "")
{
    for (typename std::map<T1, T2>::const_iterator iter = arg.begin(); iter != arg.end(); ++iter) {
        std::cout << iter->first << "->" << iter->second << sep;
    }
}

template <typename T>
void prnf_vec(const std::vector<T>& arg)
{
	for (size_t n = 0; n < arg.size(); n++) {
		std::printf("%.7f\t", arg[n]);
	}
}

#endif // UTILITIES_HH 
