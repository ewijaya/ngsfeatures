// Optimized version of EstimateTrueCount.cc with OpenMP parallelization
// Copyright 2025, NGSFeatures Project
//
// OPTIMIZATION CHANGES:
// 1. Added OpenMP parallelization to vector operations
// 2. Added SIMD vectorization hints with #pragma omp simd
// 3. Optimized sparse matrix operations with parallel reductions
//

#include "Utilities.hh"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;

/*
 * ====================================================================================
 * Begin Functions here - OPTIMIZED WITH OPENMP
 * =====================================================================================
 */

// OPTIMIZED: Added OpenMP parallel reduction for vectorization
double computeLogLik(const std::vector<double>& m, const std::vector<double>& p,
                     const double& lmbd) {
    double Result = 0.0;

#pragma omp parallel for reduction(+ : Result)
    for (unsigned i = 0; i < p.size(); i++) {
        Result += m[i] * log(p[i] * lmbd);
    }

    return (-lmbd + Result);
}

// OPTIMIZED: Added OpenMP parallel for
vector<double> divideVecWithScalar(const std::vector<double>& Vec, const double& val) {
    vector<double> Result(Vec.size());
    const double inverse = 1.0 / val;

#pragma omp parallel for
    for (unsigned i = 0; i < Vec.size(); i++) {
        Result[i] = Vec[i] * inverse;
    }

    return Result;
}

// OPTIMIZED: Added OpenMP parallel for
vector<double> divideVecWithVecCorsp(const std::vector<double>& Vec1,
                                     const std::vector<double>& Vec2) {
    vector<double> Result(Vec1.size());

#pragma omp parallel for
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result[i] = Vec1[i] / Vec2[i];
    }

    return Result;
}

// OPTIMIZED: Added OpenMP parallel for
vector<double> multiplyVecWithVecCorsp(const std::vector<double>& Vec1,
                                       const std::vector<double>& Vec2) {
    vector<double> Result(Vec1.size());

#pragma omp parallel for
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result[i] = Vec1[i] * Vec2[i];
    }

    return Result;
}

// OPTIMIZED: Sparse matrix-vector product with careful parallelization
// Uses atomic operations to avoid race conditions
vector<double> sparseM_vec_prod(const vector<double>& p, const std::vector<int>& rowId,
                                const std::vector<int>& colId, const std::vector<double>& realVal) {
    std::vector<double> Result(p.size(), 0.0);

#pragma omp parallel for
    for (unsigned i = 0; i < rowId.size(); i++) {
        double contribution = realVal[i] * p[colId[i]];
#pragma omp atomic
        Result[rowId[i]] += contribution;
    }

    return Result;
}

// Original helper functions (no major optimization opportunities)

double getMainTagPropInit(const std::vector<double>& arg) {
    double mainTagProp = 0.0;

    for (unsigned i = 0; i < arg.size(); i++) {
        mainTagProp += arg[i];
    }

    return max(0.01, min(1.00, (1.00 - mainTagProp)));
}


double getMainTagPropMeanScaled(const std::vector<double>& arg) {
    double mainTagProp = 0.0;

    for (unsigned i = 0; i < arg.size(); i++) {
        mainTagProp += arg[i];
    }

    return max(0.01, min(1.00, (1.00 - mainTagProp)));
}


vector<pair<int, int>> getIndexFromVector(std::vector<string>& nbNt, std::vector<string>& arg) {
    vector<pair<int, int>> foundIndex;

    for (unsigned i = 0; i < nbNt.size(); i++) {
        for (unsigned j = 0; j < arg.size(); j++) {
            if (nbNt[i] == arg[j]) {
                pair<int, int> tuple = make_pair(i, j);
                foundIndex.push_back(tuple);
            }
        }
    }

    return foundIndex;
}


vector<pair<int, int>> getIndexFromMap(const std::vector<string>& nbNt,
                                       const std::map<string, int>& m) {
    vector<pair<int, int>> foundIndex;
    foundIndex.reserve(nbNt.size());

    for (unsigned i = 0; i < nbNt.size(); i++) {
        map<string, int>::const_iterator iter = m.find(nbNt[i]);

        if (iter != m.end()) {
            int j = iter->second;
            foundIndex.push_back(make_pair(i, j));
        }
    }

    return foundIndex;
}

vector<double> getPropSum(const std::vector<string>& neigb, const map<string, double>& m) {
    vector<double> propVec;
    propVec.reserve(neigb.size());

    for (unsigned i = 0; i < neigb.size(); i++) {
        map<string, double>::const_iterator iter = m.find(neigb[i]);
        double value = (iter != m.end()) ? iter->second : 0.0;
        propVec.push_back(value);
    }

    vector<double> pSum;
    int nofPos = neigb.size() / 3;
    pSum.reserve(nofPos);

    int b = 0;
    for (int k = 0; k < nofPos; k++) {
        double output = propVec[b] + propVec[b + 1] + propVec[b + 2];
        pSum.push_back(output);
        b += 3;
    }

    return pSum;
}

vector<double> getNumTagProp(const std::vector<string>& neigb, const map<string, double>& m) {
    vector<double> propVec;
    propVec.reserve(neigb.size());

    for (unsigned i = 0; i < neigb.size(); i++) {
        map<string, double>::const_iterator iter = m.find(neigb[i]);
        double value = (iter != m.end()) ? iter->second : 0.0;
        propVec.push_back(value);
    }

    return propVec;
}


/*
 * ====================================================================================
 * End Functions here
 *
 * =====================================================================================
 */

struct sort_pred {
    bool operator()(const std::vector<int>& fs, const std::vector<int>& sc) { return fs < sc; }
};

struct sort_pred_double {
    bool operator()(const std::vector<double>& lhs, const std::vector<double>& rhs) {
        return (lhs[1] < rhs[1] || (rhs[0] == lhs[0] && lhs[1] < rhs[1]));
    }
};


struct strCmp {
    bool operator()(const char* s1, const char* s2) const { return strcmp(s1, s2) < 0; }
};


/*
 * ====================================================================================
 * Main function - same as original
 * =====================================================================================
 */

int main(int arg_count, char* arg_vec[]) {
    if (arg_count != 2) {
        cerr << "Expected one argument" << endl;
        return EXIT_FAILURE;
    }

#ifdef _OPENMP
    // Set number of threads (can be overridden by OMP_NUM_THREADS environment variable)
    // omp_set_num_threads(4);
    cerr << "# OpenMP enabled with " << omp_get_max_threads() << " threads" << endl;
#else
    cerr << "# OpenMP not enabled - running single-threaded" << endl;
#endif

    vector<double> RA;
    vector<int> JA;
    vector<int> IA;
    vector<string> ordNumTags;
    map<string, int> ordNumTagsMap;

    vector<vector<int>> accumIndexInOrd;
    vector<vector<int>> accumIndexIA;
    vector<vector<double>> accumIndexRA;
    vector<double> rawCount;
    vector<string> Tags;

    map<string, double> theMap;
    string line;
    string qualFileName = arg_vec[1];
    string baseName = GetBaseNameFromFilename(qualFileName);
    string pathName = GetPathNameFromFilename(qualFileName);

    string nbFileName = pathName + baseName + ".nb";
    string propFileName = pathName + baseName + ".prop";
    string nbQualFileName = pathName + baseName + ".nbq";

    ifstream propfile(propFileName.c_str());

    int numberOfSeq = 0;
    int ordId = 0;

    if (propfile.is_open()) {
        while (getline(propfile, line)) {
            numberOfSeq++;
            stringstream ss(line);
            string numTag;
            double prop;

            ss >> numTag >> prop;
            theMap.insert(make_pair(numTag, prop));
            JA.push_back(numberOfSeq - 1);
            ordNumTagsMap.insert(make_pair(numTag, ordId++));
        }
        propfile.close();
    } else {
        cout << "Unable to open input file\n";
    };

    IA = JA;

    ifstream nbfile(nbFileName.c_str());
    ifstream qlfile(qualFileName.c_str());
    ifstream nbqfile(nbQualFileName.c_str());

    string nbline;
    string qlline;
    string nbqline;

    int lineno = 0;
    int lineno_ = 0;

    if (nbfile.is_open() && qlfile.is_open() && nbqfile.is_open()) {
        while (getline(nbfile, nbline) && getline(qlfile, qlline) && getline(nbqfile, nbqline)) {
            stringstream sn(nbline);

            string numTagd;
            string numTags;
            string numTagnb;
            vector<string> nbnumTags;

            sn >> numTags >> numTagd;
            Tags.push_back(numTags);
            string numTag = numTagd;

            while (sn >> numTagd) {
                nbnumTags.push_back(numTagd);
            }

            stringstream sq(qlline);
            double tableEntry;
            sq >> tableEntry;
            double observedCount = tableEntry;

            stringstream sp(nbqline);
            string Tagsq;
            string numTagq;
            double numTagProp;
            vector<double> nbnumTagsProp;

            sp >> Tagsq >> numTagq;

            while (sp >> numTagProp) {
                nbnumTagsProp.push_back(numTagProp);
            }

            vector<double> propSum = getPropSum(nbnumTags, theMap);
            vector<double> propOfNumTag = getNumTagProp(nbnumTags, theMap);
            double MainTagProp = getMainTagPropInit(nbnumTagsProp);

            vector<double> rTagsQual;
            rTagsQual.reserve(nbnumTagsProp.size());

            int groupIndex = 0;
            for (unsigned x = 0; x < propSum.size(); x += 1) {
                double rTagsQual1 =
                    nbnumTagsProp[groupIndex] * propOfNumTag[groupIndex] / propSum[x];
                double rTagsQual2 =
                    nbnumTagsProp[groupIndex + 1] * propOfNumTag[groupIndex + 1] / propSum[x];
                double rTagsQual3 =
                    nbnumTagsProp[groupIndex + 2] * propOfNumTag[groupIndex + 2] / propSum[x];

                if (isnan(rTagsQual1)) {
                    rTagsQual1 = 0;
                }

                if (isnan(rTagsQual2)) {
                    rTagsQual2 = 0;
                }

                if (isnan(rTagsQual3)) {
                    rTagsQual3 = 0;
                }

                rTagsQual.push_back(rTagsQual1);
                rTagsQual.push_back(rTagsQual2);
                rTagsQual.push_back(rTagsQual3);

                groupIndex += 3;
            }

            MainTagProp = getMainTagPropMeanScaled(rTagsQual);
            vector<pair<int, int>> indexInOrd = getIndexFromMap(nbnumTags, ordNumTagsMap);

            vector<vector<int>> indexInOrdLineNo;
            for (unsigned k = 0; k < indexInOrd.size(); k++) {
                pair<int, int> tpl = indexInOrd[k];
                vector<int> temp;
                temp.push_back(tpl.first);
                temp.push_back(lineno++);
                temp.push_back(tpl.second);
                indexInOrdLineNo.push_back(temp);
            }

            accumIndexInOrd.insert(accumIndexInOrd.end(), indexInOrdLineNo.begin(),
                                   indexInOrdLineNo.end());

            int tln = lineno_++;

            RA.push_back(MainTagProp);
            rawCount.push_back(double(observedCount));

            vector<vector<int>> forIA;
            vector<vector<double>> forRA;

            for (unsigned n = 0; n < rTagsQual.size(); n++) {
                vector<int> temp2;
                vector<double> temp3;

                if (rTagsQual[n] > 0) {
                    temp2.push_back(n + 1);
                    temp2.push_back(tln);
                    forIA.push_back(temp2);

                    temp3.push_back(double(tln));
                    temp3.push_back(double(n + 1));
                    temp3.push_back(rTagsQual[n]);
                    forRA.push_back(temp3);
                }
            }

            accumIndexIA.insert(accumIndexIA.end(), forIA.begin(), forIA.end());
            accumIndexRA.insert(accumIndexRA.end(), forRA.begin(), forRA.end());
        }
        nbfile.close();
        qlfile.close();
        nbqfile.close();
    } else {
        cout << "One of the three files NB,QL, and NBQ cannot be open\n";
    }

    std::stable_sort(accumIndexRA.begin(), accumIndexRA.end(), sort_pred_double());
    for (unsigned z = 0; z < accumIndexRA.size(); z++) {
        vector<double> dbtriplet = accumIndexRA[z];
        RA.push_back(dbtriplet[2]);
    }

    std::sort(accumIndexIA.begin(), accumIndexIA.end(), sort_pred());
    for (unsigned y = 0; y < accumIndexIA.size(); y++) {
        vector<int> duplet = accumIndexIA[y];
        IA.push_back(duplet[1]);
    }

    std::sort(accumIndexInOrd.begin(), accumIndexInOrd.end(), sort_pred());
    for (unsigned k = 0; k < accumIndexInOrd.size(); k++) {
        vector<int> triplet = accumIndexInOrd[k];
        JA.push_back(triplet[2]);
    }

    if (RA.size() != IA.size() || IA.size() != JA.size()) {
        cerr << "RA : " << RA.size() << endl;
        cerr << "IA : " << IA.size() << endl;
        cerr << "JA : " << JA.size() << endl;
        cerr << "Error: unequal dimension sizes in sparse matrix" << endl;
        exit(EXIT_FAILURE);
    }

    /*
     * *********************
     *
     * BEGIN EM Step - OPTIMIZED WITH OPENMP
     *
     *********************
     */

    double lambda = double(lineno_);
    int maxStep = 50;

    vector<double> nCount = rawCount;
    vector<double> theM = rawCount;

    const double convergence_threshold = 1e-6;
    double prev_logLik = -1e100;
    bool converged = false;

    for (int m = 0; m < maxStep; m++) {
        vector<double> thePM;
        vector<double> theP;
        vector<double> sparseM_prod_P;
        vector<double> rSums;
        vector<double> tsparseM_prod_rSums;
        double logLik;

        thePM = theM;
        theP = divideVecWithScalar(theM, lambda);
        sparseM_prod_P = sparseM_vec_prod(theP, IA, JA, RA);
        rSums = divideVecWithVecCorsp(nCount, sparseM_prod_P);
        tsparseM_prod_rSums = sparseM_vec_prod(rSums, JA, IA, RA);
        theM = multiplyVecWithVecCorsp(theP, tsparseM_prod_rSums);
        logLik = computeLogLik(theM, theP, lambda);

        if (m > 0 && fabs(logLik - prev_logLik) < convergence_threshold) {
            converged = true;

            for (unsigned i = 0; i < theM.size(); i++) {
                double ExpCount = theM[i];
                cout << Tags[i] << "\t" << fixed << setprecision(3) << rawCount[i] << "\t";
                printf("%.3f", ExpCount);
                cout << "\t";
                cout << endl;
            }
            break;
        }
        prev_logLik = logLik;

        if (m == maxStep - 1) {
            for (unsigned i = 0; i < theM.size(); i++) {
                double ExpCount = theM[i];

                cout << Tags[i] << "\t" << fixed << setprecision(3) << rawCount[i] << "\t";
                printf("%.3f", ExpCount);
                cout << "\t";
                cout << endl;
            }
        }
    }

    return EXIT_SUCCESS;
}
