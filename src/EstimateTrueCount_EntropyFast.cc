// =====================================================================================
//
// Copyright 2009, Edward Wijaya
// =====================================================================================

#include "Utilities.hh"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <cmath>
#include <cstdio>  // printf()
#include <cstdlib>
#include <cstring>  // strcmp()
#include <ctime>    // time()

using namespace std;


// BEgin Entropy Code

double h(double x) {
    return 1. / (1. + exp(-x));
}


vector<double> h_alloc(std::vector<double>& x_vect) {
    //
    vector<double> res_vect;

    //
    for (unsigned i = 0; i < x_vect.size(); i++) {
        res_vect.push_back(h(x_vect[i]));
    }
    return res_vect;
}


double der_h(double x) {
    return exp(-x) / pow((1 + exp(-x)), 2);
}


// ######
// #
// # UTIL
// #
// ######


// sum

double compute_sum(std::vector<double>& x_vect) {
    double res = 0;

    // sum
    for (unsigned i = 0; i < x_vect.size(); i++) {
        res += x_vect[i];
    }

    return res;
}


// sum

double compute_sum_hx_vect(std::vector<double>& x_vect) {
    double res = 0;

    // sum
    for (unsigned i = 0; i < x_vect.size(); i++) {
        res += h(x_vect[i]);
    }

    return res;
}


// sum

double compute_dot(std::vector<double>& x_vect, std::vector<double>& y_vect) {
    double res = 0;

    // sum
    for (unsigned i = 0; i < x_vect.size(); i++) {
        res += x_vect[i] * y_vect[i];
    }

    return res;
}


// turn a positive vector into probabilities

int make_proba(std::vector<double>& x_vect, std::vector<double>& res_vect) {
    // precal
    double tmp_sum = compute_sum(x_vect);


    for (unsigned i = 0; i < x_vect.size(); i++) {
        res_vect[i] = x_vect[i] / tmp_sum;
    }

    return 0;
}


// turn a positive vector into probabilities

vector<double> make_proba_alloc(std::vector<double>& x_vect) {
    vector<double> res_vect;

    // precal
    double tmp_sum = compute_sum(x_vect);


    for (unsigned i = 0; i < x_vect.size(); i++) {
        res_vect.push_back(x_vect[i] / tmp_sum);
    }

    return res_vect;
}


// sum

int copy_vect(std::vector<double>& x_vect, std::vector<double>& res_vect) {
    // sum
    for (unsigned i = 0; i < x_vect.size(); i++) {
        res_vect[i] = x_vect[i];
    }

    return 0;
}


// alloc

vector<double> alloc_vect(int nx) {
    //
    vector<double> res_vect;

    // sum
    for (int i = 0; i < nx; i++) {
        res_vect.push_back(0);
    }

    return res_vect;
}


// ######
// #
// # FUNC
// #
// ######


// likelihood

double f(std::vector<double>& x_vect, std::vector<double>& m_vect) {
    //
    double res = 0;

    // precal
    double sum_hx = compute_sum_hx_vect(x_vect);

    double pi;

    for (unsigned i = 0; i < x_vect.size(); i++) {
        pi = h(x_vect[i]) / sum_hx;
        if (pi > 0) {
            res += m_vect[i] * log(pi);
        }
    }


    return res;
}


// entropy

double ent(std::vector<double>& x_vect) {
    //
    double res = 0;

    // precal
    double sum_hx = compute_sum_hx_vect(x_vect);

    double pi;

    for (unsigned i = 0; i < x_vect.size(); i++) {
        pi = h(x_vect[i]) / sum_hx;
        if (pi > 0) {
            res -= pi * log(pi);
        }
    }
    return res;
}


// ######
// #
// # DERIVATIVES
// #
// ######


// likelihood


double der_f_k(std::vector<double>& x_vect, std::vector<double>& m_vect, int kk) {
    double res = 0;


    // precal
    double tmp_sum = compute_sum_hx_vect(x_vect);


    for (int jj = 0; jj < static_cast<int>(x_vect.size()); jj++) {
        int tmp_bool = (kk == jj);
        double hx_j = h(x_vect[jj]);
        double der_hx_k = der_h(x_vect[kk]);
        double ckj = der_hx_k * (tmp_sum * tmp_bool - hx_j) / pow(tmp_sum, 2);
        res += m_vect[jj] * tmp_sum * ckj / hx_j;
    }

    return res;
}


// entropy

double der_ent_k(std::vector<double>& x_vect, int kk) {
    double res = 0;

    // precal
    double tmp_sum = compute_sum_hx_vect(x_vect);


    for (int jj = 0; jj < static_cast<int>(x_vect.size()); jj++) {
        int tmp_bool = (kk == jj);
        double hx_j = h(x_vect[jj]);
        double der_hx_k = der_h(x_vect[kk]);
        double ckj = der_hx_k * (tmp_sum * tmp_bool - hx_j) / pow(tmp_sum, 2);
        if (hx_j > 0) {
            res += -ckj * (1 + log(hx_j / tmp_sum));
        }
    }

    return res;
}


// ######
// #
// # - f() + gamma * ent()
// #
// ######


//

double final_f(std::vector<double>& x_vect, std::vector<double>& m_vect, double gamma) {
    return -f(x_vect, m_vect) + gamma * ent(x_vect);
}


//

int final_der(std::vector<double>& x_vect, std::vector<double>& m_vect, double gamma,
              std::vector<double>& res_vect) {
    //
    for (unsigned jj = 0; jj < res_vect.size(); jj++) {
        res_vect[jj] = -der_f_k(x_vect, m_vect, jj) + gamma * der_ent_k(x_vect, jj);
    }


    return 0;
}


// ######
// #
// # GRADIENT
// #
// ######


//

double backtrackingLineSearch(std::vector<double>& x_vect, std::vector<double>& m_vect,
                              std::vector<double>& der_vect, double new_fx, double gamma) {
    //
    double alpha = 0.1;
    double beta = 0.5;

    //
    double t = 1;

    //
    vector<double> tmp_x_vect = alloc_vect(der_vect.size());

    double res_a;
    double res_b;

    //
    while (1) {
        // x + t * ( - derX )
        for (unsigned jj = 0; jj < tmp_x_vect.size(); jj++) {
            tmp_x_vect[jj] = x_vect[jj] - t * der_vect[jj];
        }

        //
        res_a = final_f(tmp_x_vect, m_vect, gamma);
        res_b = new_fx + alpha * t * (-compute_dot(der_vect, der_vect));

        // sortie normale
        if (res_a <= res_b) {
            break;
        }

        // maj
        t *= beta;

        // sortie anormale
        if (t == 0) {
            break;
        }
    }


    return t;
}


//

vector<double> gradient_descent(std::vector<double>& m_vect, double gamma) {
    //
    // vector <double> x_vect = alloc_vect( m_vect.size() );
    vector<double> new_x_vect = alloc_vect(m_vect.size());
    vector<double> der_vect = alloc_vect(m_vect.size());


    // Generate Random Number for x_vect
    vector<double> x_vect;
    for (int i = 0; i < static_cast<int>(m_vect.size()); i++) {
        x_vect.push_back(sin(rand()) * 5);
    }

    //
    int n_iter = 0;
    double new_fx = final_f(x_vect, m_vect, gamma);
    double old_fx = 0;
    double epsilon = 0;

    // param
    int n_iter_max = 1000;
    double nu = pow(10, -4);


    //
    while (1) {
        // gradient
        final_der(x_vect, m_vect, gamma, der_vect);

        // epsilon
        epsilon = backtrackingLineSearch(x_vect, m_vect, der_vect, new_fx, gamma);

        // maj x
        for (unsigned jj = 0; jj < x_vect.size(); jj++) {
            new_x_vect[jj] = x_vect[jj] - epsilon * der_vect[jj];
        }
        // maj fx
        new_fx = final_f(new_x_vect, m_vect, gamma);

        // sortie anormale
        if (n_iter > 0 && new_fx > old_fx) {
            break;
        }

        // sortie normale
        if ((n_iter > 0 && abs((new_fx - old_fx) / old_fx) < nu * x_vect.size()) ||
            (n_iter == n_iter_max)) {
            break;
        }

        // maj
        copy_vect(new_x_vect, x_vect);
        n_iter += 1;
        old_fx = new_fx;
    }

    //
    // cout << "gradient_descent() : " << n_iter << " iterations" << endl;

    //
    return new_x_vect;
}


//

vector<double> wrap_gradient_descent(std::vector<double>& m_vect, double gamma) {
    //
    vector<double> x0_vect = gradient_descent(m_vect, gamma);
    vector<double> x1_vect = gradient_descent(m_vect, gamma);
    vector<double> best_x_vect;

    //
    double fx0 = final_f(x0_vect, m_vect, gamma);
    double fx1 = final_f(x1_vect, m_vect, gamma);

    //
    if (fx0 < fx1) {
        best_x_vect = x0_vect;
    } else {
        best_x_vect = x1_vect;
    }

    //
    // double best_ent = ent( best_x_vect );
    // cout << "best_ent = "<< best_ent << endl;

    //
    vector<double> hx_vect = h_alloc(best_x_vect);
    vector<double> res_vect = make_proba_alloc(hx_vect);

    // prn_vec <double>( res_vect, " ");
    // cout << endl;
    // exit(0);

    //
    return res_vect;
}


// End Entropy Code

string ConvertInt2String(int IntVal) {
    std::string S;
    std::stringstream out;
    out << IntVal;
    S = out.str();

    return S;
}


string ConvertDouble2String(double DblVal) {
    std::string S;
    std::stringstream out;
    out << DblVal;
    S = out.str();

    return S;
}


double computeLogLik(std::vector<double>& m, std::vector<double>& p, double& lmbd) {
    double Result = 0;
    for (unsigned i = 0; i < p.size(); i++) {
        if (p[i] != 0.00) {
            Result += m[i] * log(p[i] * lmbd);
        }
    }

    return (-lmbd + Result);
}

vector<double> multiplyVecWithVecCorsp(std::vector<double>& Vec1, std::vector<double>& Vec2) {
    vector<double> Result;

    for (unsigned i = 0; i < Vec1.size(); i++) {
        double Res = Vec1[i] * Vec2[i];
        Result.push_back(Res);
    }

    return Result;
}
vector<double> divideVecWithVecCorsp(std::vector<double>& Vec1, std::vector<double>& Vec2) {
    vector<double> Result;

    for (unsigned i = 0; i < Vec1.size(); i++) {
        double Res = Vec1[i] / Vec2[i];
        Result.push_back(Res);
    }

    return Result;
}

vector<double> divideVecWithScalar(std::vector<double>& Vec, double& theC) {
    vector<double> Result;

    for (unsigned i = 0; i < Vec.size(); i++) {
        double Res = Vec[i] / theC;
        Result.push_back(Res);
    }

    return Result;
}

vector<double> sparseM_vec_prod(vector<double>& p, std::vector<int>& rowId, std::vector<int>& colId,
                                std::vector<double>& realVal) {
    std::vector<double> Result;
    Result.assign(p.size(), 0);

    for (unsigned i = 0; i < rowId.size(); i++) {
        // cout << realVal[i] << " " << p(colId[i]) << endl;
        Result[rowId[i]] += realVal[i] * p[colId[i]];
    }

    return Result;
}


double getMainTagPropInit(std::vector<double>& arg) {
    double mainTagProp = 0;

    for (unsigned i = 0; i < arg.size(); i++) {
        mainTagProp += arg[i];
    }

    return max(0.01, (1 - mainTagProp));
}

double getMainTagPropMeanScaled(std::vector<double>& arg) {
    double mainTagProp = 0;

    for (unsigned i = 0; i < arg.size(); i++) {
        mainTagProp += arg[i];
    }

    return max(0.01, min(1.00, (1.00 - mainTagProp)));
}


vector<pair<int, int>> getIndexFromVector(std::vector<string>& nbNt, std::vector<string>& arg) {
    vector<pair<int, int>> foundIndex;

    for (unsigned i = 0; i < nbNt.size(); i++) {
        // cout << i ;
        for (unsigned j = 0; j < arg.size(); j++) {
            // cout << "\t"  << nbNt[i] << "," << arg[j] << " " << j <<  endl;

            if (nbNt[i] == arg[j]) {
                // cout << "\t"  << nbNt[i] << "," << arg[j] << " " << j <<  endl;
                pair<int, int> tuple = make_pair(i, j);
                foundIndex.push_back(tuple);
            }
        }
    }

    return foundIndex;
}


vector<pair<int, int>> getIndexFromMap(std::vector<string>& nbNt, std::map<string, int>& m) {
    vector<pair<int, int>> foundIndex;

    for (unsigned i = 0; i < nbNt.size(); i++) {
        map<string, int>::iterator iter = m.find(nbNt[i]);

        if (iter != m.end()) {
            int j = iter->second;
            pair<int, int> tuple = make_pair(i, j);
            foundIndex.push_back(tuple);
        }
    }

    return foundIndex;
}

vector<double> getPropSum(std::vector<string>& neigb, map<string, double>& m) {
    vector<double> propVec;

    // Find value of a neigbours
    // from the map, otherwise return zero

    for (unsigned i = 0; i < neigb.size(); i++) {
        map<string, double>::iterator iter = m.find(neigb[i]);
        double value = 0;

        if (iter != m.end()) {
            value = m[neigb[i]];
        }
        propVec.push_back(value);
        // cout << neigb[i] << " " << value << endl;
    }


    // Sum the vector in three-three
    // Resulting vector = nofpos

    vector<double> pSum;
    int nofPos = neigb.size() / 3;
    int b = 0;
    for (int k = 0; k < nofPos; k++) {
        double output = propVec[b] + propVec[b + 1] + propVec[b + 2];
        pSum.push_back(output);
        b += 3;
    }


    return pSum;
}

vector<double> getNumTagProp(std::vector<string>& neigb, map<string, double>& m) {
    vector<double> propVec;

    // Find value of a neigbours
    // from the map, otherwise return zero

    for (unsigned i = 0; i < neigb.size(); i++) {
        map<string, double>::iterator iter = m.find(neigb[i]);
        double value = 0;

        if (iter != m.end()) {
            value = m[neigb[i]];
        }
        propVec.push_back(value);
        // cout << neigb[i] << " " << value << endl;
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
    // for sorting Vector of Vector. Only  based on first->second ascendingly
    bool operator()(const std::vector<int>& fs, const std::vector<int>& sc) { return fs < sc; }
};

struct sort_pred_double {
    // for sorting Vector of Vector. Only  based on second->first ascendingly
    bool operator()(const std::vector<double>& lhs, const std::vector<double>& rhs) {
        return (lhs[1] < rhs[1] || (rhs[0] == lhs[0] && lhs[1] < rhs[1]));
    }
};


struct strCmp {
    bool operator()(const char* s1, const char* s2) const { return strcmp(s1, s2) < 0; }
};


/*
 * ====================================================================================
 * End Struct here
 *
 * =====================================================================================
 */

int main(int arg_count, char* arg_vec[]) {
    if (arg_count != 3) {
        cerr << "Expected two argument inputfile and beta" << endl;
        return EXIT_FAILURE;
    }
    srand(time(0));
    /*
       Final Output Variables (Sparse)
       RA -> Vector with real value
       JA -> Vector with colum index
       IA -> Vector with row index
    */

    vector<double> RA;
    vector<int> JA;
    vector<int> IA;
    vector<string> ordNumTags;
    map<string, int> ordNumTagsMap;

    vector<vector<int>> accumIndexInOrd;
    vector<vector<int>> accumIndexIA;
    vector<vector<double>> accumIndexRA;
    vector<double> rawCount;  // raw count container
    vector<string> Tags;

    // Store Prop File as Hash
    map<string, double> theMap;
    string line;
    string qualFileName = arg_vec[1];
    string baseName = GetBaseNameFromFilename(qualFileName);
    string pathName = GetPathNameFromFilename(qualFileName);

    string nbFileName = pathName + baseName + ".nb";
    string propFileName = pathName + baseName + ".prop";
    string nbQualFileName = pathName + baseName + ".nbq";


    double beta = static_cast<double>(atof(arg_vec[2]));


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
            // cout << numTag << "\t" << prop << endl;
            theMap.insert(make_pair(numTag, prop));
            JA.push_back(numberOfSeq - 1);
            ordNumTagsMap.insert(make_pair(numTag, ordId++));
        }
        propfile.close();


    } else {
        cout << "Unable to open input file\n";
    };


    // for( map<const string, int, strCmp>::iterator iter = ordNumTagsMap.begin(); iter !=
    // ordNumTagsMap.end(); ++iter) {
    //      cout << (*iter).first << " - " << (*iter).second << endl;
    // }

    // Initialize IA first column
    // it's the same with JA
    IA = JA;

    // Begin iterating the neigbours file and quality file, then find
    // estimated error mean

    ifstream nbfile(nbFileName.c_str());
    ifstream qlfile(qualFileName.c_str());
    ifstream nbqfile(nbQualFileName.c_str());

    string nbline;
    string qlline;
    string nbqline;

    int lineno = 0;
    int lineno_ = 0;

    // std::clock_t startTime = std::clock();

    if (nbfile.is_open() && qlfile.is_open() && nbqfile.is_open()) {
        while (getline(nbfile, nbline) && getline(qlfile, qlline) && getline(nbqfile, nbqline)) {
            stringstream sn(nbline);

            // Parsing Neighbors File
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


            // Parsing Quality File
            stringstream sq(qlline);
            double tableEntry;
            sq >> tableEntry;
            double observedCount = tableEntry;


            // Parsing Neighbourhood Quality file
            stringstream sp(nbqline);
            string Tagsq;
            string numTagq;
            double numTagProp;
            vector<double> nbnumTagsProp;

            sp >> Tagsq >> numTagq;

            while (sp >> numTagProp) {
                nbnumTagsProp.push_back(numTagProp);
            }


            //
            // Begin computing Rtags.A
            //

            vector<double> propSum = getPropSum(nbnumTags, theMap);
            vector<double> propOfNumTag = getNumTagProp(nbnumTags, theMap);
            double MainTagProp = getMainTagPropInit(nbnumTagsProp);

            vector<double> rTagsQual;
            rTagsQual.reserve(nbnumTagsProp.size());

            // Find Error Mean
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
                // cout << tpl.first << "-" << tpl.second << endl;
                vector<int> temp;
                temp.push_back(tpl.first);
                temp.push_back(lineno++);
                temp.push_back(tpl.second);
                indexInOrdLineNo.push_back(temp);
            }

            // printf("%.7f\t", MainTagProp);
            // prn_vec<double>(rTagsQual,"\t");
            // cout << endl;

            accumIndexInOrd.insert(accumIndexInOrd.end(), indexInOrdLineNo.begin(),
                                   indexInOrdLineNo.end());

            int tln = lineno_++;

            RA.push_back(MainTagProp);
            rawCount.push_back(double(observedCount));

            // For IA and RA
            vector<vector<int>> forIA;
            vector<vector<double>> forRA;

            for (unsigned n = 0; n < rTagsQual.size(); n++) {
                vector<int> temp2;
                vector<double> temp3;

                if (rTagsQual[n] > 0) {
                    // cout << rTagsQual[n] << "-" << tln << "-" << n+1 << " ";
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


            // cout << " ------- " <<endl;
        }
        nbfile.close();
        qlfile.close();
        nbqfile.close();
    } else {
        cout << "One of the three files NB,QL, and NBQ cannot be open\n";
    }


    // cerr << "# CPU time for parsing two files: " << (clock() - startTime + 0.0) / CLOCKS_PER_SEC
    //     << " seconds\n";


    /*
     Here we extract show all the sparse matrix param (RA,JA,IA);
     Then convert them into the standard SparseMatrix
    */

    // std::clock_t startSparseTime = std::clock();

    // cout << "DIM " << lineno_ << "x" << lineno_ << endl;
    // cout << "RAW COUNT ";
    // prn_vec_err<double>(rawCount,",");
    // cout << rawCount.size();
    // cout << endl;

    // Print and keep RA
    std::stable_sort(accumIndexRA.begin(), accumIndexRA.end(), sort_pred_double());
    for (unsigned z = 0; z < accumIndexRA.size(); z++) {
        vector<double> dbtriplet = accumIndexRA[z];
        // cout << "\t" << dbtriplet[0] << "\t" << dbtriplet[1] << "\t" << dbtriplet[2] <<  endl;
        RA.push_back(dbtriplet[2]);
    }


    // Print and Keep IA
    std::sort(accumIndexIA.begin(), accumIndexIA.end(), sort_pred());
    for (unsigned y = 0; y < accumIndexIA.size(); y++) {
        vector<int> duplet = accumIndexIA[y];
        IA.push_back(duplet[1]);
    }


    // Print and Keep JA
    std::sort(accumIndexInOrd.begin(), accumIndexInOrd.end(), sort_pred());
    for (unsigned k = 0; k < accumIndexInOrd.size(); k++) {
        vector<int> triplet = accumIndexInOrd[k];
        JA.push_back(triplet[2]);
    }

    if (RA.size() != IA.size() || IA.size() != JA.size()) {
        cerr << "RA : ";
        // prn_vec_err<double>(RA, " ");
        cerr << RA.size() << endl;
        cerr << "IA : ";
        // prn_vec_err<int>(IA, " ");
        cerr << IA.size() << endl;
        cerr << "JA : ";
        // prn_vec_err<int>(JA," ");
        cerr << JA.size() << endl;
        cerr << "Error: unequal dimension sizes in sparse matrix" << endl;
        exit(EXIT_FAILURE);
    }


    // cerr << "# CPU time for storing and sorting SparseM: " << (clock() - startSparseTime + 0.0) /
    // CLOCKS_PER_SEC
    //     << " seconds\n";


    /*
     * *********************
     *
     * BEGIN EM Step
     *
     *********************
     */
    // std::clock_t startEMTime = std::clock();

    double lambda = double(lineno_);
    int maxStep = 50;

    vector<double> nCount = rawCount;
    vector<double> theM = rawCount;
    double temp_loglik = 0;

    for (int m = 0; m < maxStep; m++) {
        vector<double> theP;
        vector<double> theP_temp;
        vector<double> sparseM_prod_P;
        vector<double> rSums;
        vector<double> tsparseM_prod_rSums;
        double logLik;


        // M-Step
        theP = divideVecWithScalar(theM, lambda);

        // Print M to a temporary vector

        vector<double> thePnew = wrap_gradient_descent(theM, beta);

        // Assinged the New Estimated P
        theP = thePnew;

        // E-Step
        sparseM_prod_P = sparseM_vec_prod(theP, IA, JA, RA);
        rSums = divideVecWithVecCorsp(nCount, sparseM_prod_P);
        tsparseM_prod_rSums = sparseM_vec_prod(rSums, JA, IA, RA);
        theM = multiplyVecWithVecCorsp(theP, tsparseM_prod_rSums);


        // Compute Loglik
        logLik = computeLogLik(theM, theP, lambda);
        double diff_loglik = logLik - temp_loglik;
        temp_loglik = logLik;

        // cout << "Step " << m << "\t" << logLik << "\t" << diff_loglik << endl;


        // Show only the last iteration
        if (diff_loglik <= 0.001) {
            // if (m == maxStep-1) {

            // cout << "Final Step : " << m << "LLDIF " << diff_loglik <<  endl;

            for (unsigned i = 0; i < theM.size(); i++) {
                double ExpCount = theM[i];

                cout << Tags[i] << "\t" << theP[i] << "\t";
                printf("%.3f", ExpCount);
                cout << "\t";
                cout << endl;
            }

            // cout << "Final Step " << m << endl;

            break;
        }
    }


    return 0;
}
