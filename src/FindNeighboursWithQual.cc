// =====================================================================================
// Finds Neighbors of a Tag within 1 Hamming Distance
// Copyright 2009, Edward Wijaya
// =====================================================================================

#include "Utilities.hh"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <cmath>
#include <cstdlib>

using namespace std;

// Optimized: inline for frequent calls, use log10 instead of log(x)/log(10)
inline double Solexa2Phred(double sQ) {
    // Convert Solexa into Phred Quality
    // Optimized: log10 is faster than log(x)/log(10)
    double pQ = 10.00 * log10(1.00 + pow(10.00, (sQ / 10.0)));
    return pQ;
}

// Optimized: use std::to_string instead of stringstream
inline string ConvertInt2String(int IntVal) {
    return std::to_string(IntVal);
}

// Optimized: pass by const reference, reserve space, avoid string conversions
vector<int> MismatchPos(const std::vector<int>& STag, const string& Nb) {
    vector<int> MMPos;
    MMPos.reserve(STag.size());  // Pre-allocate worst case

    // Optimized: avoid string conversions by comparing directly
    for (unsigned i = 0; i < STag.size(); i++) {
        int tagDigit = STag[i];
        int nbDigit = Nb[i] - '0';  // Convert char to int directly

        if (nbDigit != tagDigit) {
            MMPos.push_back(i);
        }
    }

    return MMPos;
}

// Optimized: pass by const reference, cache constant division
double MultiplyError(const std::vector<double>& Qual, const std::vector<int>& Pos) {
    double Result = 1.0;
    static const double one_third = 1.0 / 3.0;  // Cache constant

    for (unsigned i = 0; i < Pos.size(); i++) {
        double phred = Solexa2Phred(Qual[Pos[i]]);
        // Optimized: use exp10 if available, or precalculate pow(10, -x/10)
        double err = pow(10.0, -phred / 10.0) * one_third;
        Result *= err;
    }

    return Result;
}

// Optimized: pass by const reference, reserve space
vector<int> MismatchPosByVec(const std::vector<int>& STag, const vector<int>& Nb) {
    vector<int> MMPos;
    MMPos.reserve(STag.size());  // Pre-allocate

    for (unsigned i = 0; i < STag.size(); i++) {
        if (Nb[i] != STag[i]) {
            MMPos.push_back(i);
        }
    }

    return MMPos;
}

// Optimized: pass by const reference where possible
double normalizeQualByMismatches1Tag(const std::vector<int>& SeedTag,
                                     const std::vector<int>& NbTags,
                                     const std::vector<double>& qual) {
    vector<int> mmPos = MismatchPosByVec(SeedTag, NbTags);
    double ErrorProb = MultiplyError(qual, mmPos);
    return ErrorProb;
}


// Optimized: pass by const reference and return modified copy
vector<int> neighbors(const vector<int>& arg, int posNo, int baseNo) {
    // pass base position and return neighbors
    vector<int> transfVec = arg;  // Copy constructor
    transfVec[posNo] = baseNo;
    return transfVec;
}


// Optimized: pass by const reference, reserve space, avoid string conversions
string Vec2Str(const vector<int>& NTg) {
    string StTg;
    StTg.reserve(NTg.size());  // Pre-allocate

    // Optimized: convert int to char directly (0-3 -> '0'-'3')
    for (unsigned i = 0; i < NTg.size(); i++) {
        StTg += ('0' + NTg[i]);  // Much faster than ConvertInt2String
    }
    return StTg;
}


int main(int arg_count, char* arg_vec[]) {
    if (arg_count < 3) {
        cerr << "Expected two arguments FileName and Max Hamming Distance" << endl;
        return EXIT_FAILURE;
    }

    string line;

    string filename = arg_vec[1];
    ifstream myfile(filename.c_str());

    // Max Hamming Distance to Generate Neighbours
    int hd = static_cast<int>(atoi(arg_vec[2]));
    double BaseErrProbLim = static_cast<double>(atof(arg_vec[3]));


    // cerr << "HD " <<  hd << endl;

    if (hd > 2) {
        cerr << "Hamming Distance should be <= 2" << endl;
        return EXIT_FAILURE;
    }


    // cerr << BaseErrProbLim << endl;

    string baseName = GetBaseNameFromFilename(filename);
    string pathName = GetPathNameFromFilename(filename);

    string nbFileName = pathName + baseName + ".nb";
    string nbqFileName = pathName + baseName + ".nbq";

    ofstream nbFile;
    nbFile.open(nbFileName.c_str(), ios::out);

    ofstream nbqFile;
    nbqFile.open(nbqFileName.c_str(), ios::out);

    vector<string> DNAStrings;

    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (line.find("#") == 0) {
                continue;
            }

            stringstream ss(line);
            string DNA;
            double qualSc;
            double rawCount;
            vector<double> qualBase;
            qualBase.reserve(50);  // Reserve typical read length

            // ss >> rawCount >> DNA;
            ss >> rawCount >> DNA;

            if (rawCount == 0.00) {
                rawCount = rawCount + 0.00001;
            }

            string dnaTag;
            dnaTag = DNA;

            while (ss >> qualSc) {
                qualBase.push_back(qualSc);
            }

            // we process string line by line here
            // avoiding slurping with push_back

            nbFile << DNA << "\t";
            nbqFile << DNA << "\t";

            // Convert string to numeric using optimized switch (faster than map)
            vector<int> numTag;
            numTag.reserve(DNA.size());  // Pre-allocate

            for (unsigned j = 0; j < DNA.size(); j++) {
                int cb;
                switch (DNA[j]) {
                    case 'A':
                        cb = 0;
                        break;
                    case 'C':
                        cb = 1;
                        break;
                    case 'G':
                        cb = 2;
                        break;
                    case 'T':
                        cb = 3;
                        break;
                    default:
                        cb = 0;
                        break;
                }
                numTag.push_back(cb);
            }

            // prn_vec(numTag);
            prn_vec_binos<int>(numTag, nbFile);
            nbFile << "\t\t";

            prn_vec_binos<int>(numTag, nbqFile);
            nbqFile << "\t\t";

            if (hd == 1) {
                for (unsigned p = 0; p < numTag.size(); p++) {
                    for (int b = 1; b <= 3; b++) {
                        // cerr << " Pos: " << p << ", base= " << b << endl;

                        int bval = b;
                        if (numTag[p] == b) {
                            bval = 0;
                        }

                        vector<int> nbnumTag = neighbors(numTag, p, bval);
                        double nrmQual = normalizeQualByMismatches1Tag(numTag, nbnumTag, qualBase);


                        // prn_vec <int>(nbnumTag);
                        prn_vec_binos<int>(nbnumTag, nbFile);
                        nbFile << "\t";
                        nbqFile << nrmQual << "\t";
                    }
                }

                nbFile << endl;
                nbqFile << endl;
            } else {
                int TagLen = static_cast<int>(numTag.size());

                for (int p = 0; p < TagLen; p++) {
                    // First loop is to generate tags 1 position differ
                    for (int b = 0; b <= 3; b++) {
                        int bval = b;
                        if (numTag[p] == b) {
                            continue;
                        }

                        vector<int> nbnumTag = neighbors(numTag, p, bval);
                        string SnbnumTag = Vec2Str(nbnumTag);
                        double nrmQual = normalizeQualByMismatches1Tag(numTag, nbnumTag, qualBase);


                        // We want to keep all 1 mismatch neighbors
                        nbqFile << nrmQual << "\t";


                        //
                        // Second loop for tags in 2 position differ
                        for (int l = p + 1; l < TagLen; l++) {
                            for (int c = 0; c <= 3; c++) {
                                int cval = c;

                                if (nbnumTag[l] == c) {
                                    continue;
                                }
                                vector<int> nbnumTag2 = neighbors(nbnumTag, l, cval);
                                string SnbnumTag2 = Vec2Str(nbnumTag2);
                                double nrmQual2 =
                                    normalizeQualByMismatches1Tag(numTag, nbnumTag2, qualBase);

                                if (nrmQual >= BaseErrProbLim) {
                                    nbFile << SnbnumTag2;
                                    nbFile << "\t";
                                    nbqFile << nrmQual2 << "\t";
                                }
                            }
                        }
                    }
                }


                nbFile << endl;
                nbqFile << endl;
            }
        }
        myfile.close();
        nbFile.close();
        nbqFile.close();
    }

    else
        nbFile << "Unable to open file\n";
    return 0;
}
