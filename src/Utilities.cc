#include "Utilities.hh"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
using std::string;
using std::vector;
using std::cout;

std::string GetBaseNameFromFilename(const std::string &fileName) {

    string fbaseNameWithExt = fileName.substr((fileName.find_last_of("/\\")) + 1);
    int    fperiodIn = fbaseNameWithExt.find(".");
    string fbaseName = fbaseNameWithExt.substr(0,fperiodIn);

    return fbaseName;
}



std::string GetPathNameFromFilename( const std::string &filename )
{
    string foundPath = filename.substr( 0, filename.rfind("/")+1 ); 

    if (foundPath == "") { 
        foundPath = "./";
    }
    return foundPath; 
}


/*template <typename T> void  prn_vec(std::vector < T >&arg)
{
	for (int n = 0; n < arg.size(); n++) {
		cout << arg[n] << " ";
	}
}
*/

vector <char> tagnum2acgt(vector<int>arg ) {

    vector<char> stringACGT;
    stringACGT.reserve(arg.size());  // Pre-allocate memory

    // Static const lookup table (created once, reused)
    static const char lookup[4] = {'A', 'C', 'G', 'T'};

    for ( unsigned i=0 ;i<arg.size() ; i++ ) {
        stringACGT.push_back(lookup[arg[i]]);
    }

    return stringACGT;

}

vector <int> acgt2tagnum(string arg ) {

    vector<int> numAll;
    numAll.reserve(arg.size());  // Pre-allocate memory

    // Static const lookup table - faster than map lookup
    // Using switch for maximum compiler optimization
    for ( unsigned i=0 ;i<arg.size() ; i++ ) {
        switch(arg[i]) {
            case 'A': numAll.push_back(0); break;
            case 'C': numAll.push_back(1); break;
            case 'G': numAll.push_back(2); break;
            case 'T': numAll.push_back(3); break;
            default:  numAll.push_back(0); break;
        }
    }

    return numAll;

}

vector <int> id2tagnum(int id, int tgl ) {

    // Optimized version using bit operations instead of pow()
    // Base-4 can be extracted using shifts (4 = 2^2, so 2 bits per digit)
    vector <int> nuMTag;
    nuMTag.reserve(tgl);  // Pre-allocate memory

    int val = id - 1;

    // Extract base-4 digits from most significant to least significant
    for ( int i = tgl - 1; i >= 0; i-- ) {
        int shift = i * 2;  // Each base-4 digit needs 2 bits
        int digit = (val >> shift) & 3;  // Extract 2 bits
        nuMTag.push_back(digit);
    }

    return nuMTag;

}

/*double get_prop( map<string,double>&m, string queryNumTag) {

    // test how to pass map in function

    map<string,double>::iterator iter;
    for ( iter = m.begin(); iter != m.end(); ++iter) {
        cout << "Key " << iter->first << ", Value = " << iter->second << endl;
    }
    return 0;
}
*/


