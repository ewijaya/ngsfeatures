// =====================================================================================
//
// Copyright 2009, Edward Wijaya
// =====================================================================================

#include "Utilities.hh"
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <numeric>
#include <iomanip>

using namespace std;

double computeLogLikdebug(std::vector <double> &m, std::vector <double> &p, double &lmbd) {

        double Result = 0;
        double pjtot = 0;
        for (unsigned i = 0; i < p.size(); i++) {

            if (p[i] != 0.00) {
                double temp = m[i] * (log(p[i]*lmbd));
                cout << i << "\t"  <<   m[i] << "\t"  << fixed << setprecision(5)  << p[i] << "\t" << lmbd <<  "\t" << temp << endl;
                Result += temp; 
                pjtot += p[i];

            }

        }
        cout <<"Result " << Result << " ---- "  << endl;
        cout << "PJTOT = " <<  pjtot << endl;

        return (-lmbd + Result);

}

// Optimized: pass by const reference
double computeLogLik(const std::vector <double> &m, const std::vector <double> &p, const double &lmbd) {

        double Result = 0.0;
        double pjtotfree=0.0;
        for (unsigned i = 0; i < p.size(); i++) {

            if (p[i] != 0.00) {
                double temp = m[i] * (log(p[i]*lmbd));
                pjtotfree += p[i];
                //cout << "Free " << i << "\t"  <<   m[i] << "\t"  << fixed << setprecision(5)  << p[i] << "\t" << lmbd <<  "\t" << temp << endl;
                Result += temp;
            }

        }

        //cout << "PJTOTFREE = " <<  pjtotfree << endl;


        return (-lmbd + Result);

}

double relative_diff_loglik (double &x, double &y) {
    
    double res = 0;
    res = abs(x-y) / max(abs(x),abs(y));

    return res;
    
}

// Optimized: pass by const reference, reserve space, use multiplication
vector <double> normalizeP(const std::vector <double> &pn) {
    double tot=0.0;
    for (unsigned i = 0; i < pn.size(); i++) {
         tot += pn[i];
    }

    vector <double> nVec;
    nVec.reserve(pn.size());  // Pre-allocate

    const double inv_tot = 1.0 / tot;  // Multiply is faster than divide
    for (unsigned j = 0; j < pn.size(); j++) {
        nVec.push_back(pn[j] * inv_tot);
    }

    return nVec;
}

/*--------------------------------------------------
* double computeLogLik(std::vector <double> &m, std::vector <double> &p, double &lmbd) {
* 
*         double Result = 0;
*         double pjtotfree=0;
*         for (unsigned i = 0; i < p.size(); i++) {
* 
*             if (p[i] != 0.00) {
*                 pjtotfree += p[i];
*                 Result += m[i] * (log(p[i]*lmbd));
*             }
* 
*         }
* 
*         //cout << "PJTOTFREE = " <<  pjtotfree << endl;
* 
* 
*         return (-lmbd + Result);
* 
* }
* 
*--------------------------------------------------*/
vector <double> multiplyVecWithVecCorsp(std::vector <double> &Vec1,  std::vector <double> &Vec2) {

    vector<double>  Result;

    for (unsigned i=0; i<Vec1.size(); i++) {
        double Res = Vec1[i]*Vec2[i];
        Result.push_back(Res);
    }

    return Result;
}

vector <double> multiplyVecWithVecCorspDebug(std::vector <double> &Vec1,  std::vector <double> &Vec2) {

    vector<double>  Result;

    for (unsigned i=0; i<Vec1.size(); i++) {
        double Res = Vec1[i]*Vec2[i];
        cout << "TheM In\t" <<  i << "\t" << Vec1[i] << "\t" << Vec2[i] << endl;
        Result.push_back(Res);
    }

    return Result;
}

vector <double> divideVecWithVecCorsp(std::vector <double> &Vec1,  std::vector <double> &Vec2) {

    vector<double>  Result;

    for (unsigned i=0; i<Vec1.size(); i++) {
        double Res = Vec1[i]/Vec2[i];
        //cout << Vec1[i] << " - " << Vec2[i] << "\tRes : " << Res << endl;
        Result.push_back(Res);
    }

    return Result;
}
vector <double> divideVecWithVecCorspDebug(std::vector <double> &Vec1,  std::vector <double> &Vec2) {

    vector<double>  Result;

    for (unsigned i=0; i<Vec1.size(); i++) {
        double Res = Vec1[i]/Vec2[i];
        cout << "rSums\t" << i << "\t" <<  Vec1[i] << "\t" << Vec2[i] << "\t=" << Res << endl;
        Result.push_back(Res);
    }

    return Result;
}

vector <double> divideVecWithScalar(std::vector <double> &Vec,  double &theC) {

    vector<double>  Result;

    for (unsigned i=0; i<Vec.size(); i++) {
        double Res = Vec[i]/theC;
        Result.push_back(Res);
    }

    return Result;
}
vector <double> divideVecWithScalarDebug(std::vector <double> &Vec,  double &theC) {

    vector<double>  Result;

    for (unsigned i=0; i<Vec.size(); i++) {
        double Res = Vec[i]/theC;
        cout <<  "M\t" << i <<"\t"  << Vec[i] << "\tthePInit\t" << Res << endl;
        Result.push_back(Res);
    }

    return Result;
}

vector <double> sparseM_vec_prodDebug(vector <double> &p, 
                        std::vector<int> &rowId, 
                        std::vector<int> &colId, 
                        std::vector<double> &realVal) {

     std::vector <double> Result;
     Result.assign(p.size(), 0.00);

     for (unsigned i= 0; i < rowId.size(); i++) {
        cout << "Tspsm\t" << i << "\t" << realVal[i] << " " << p[colId[i]] << endl;
         
        Result[rowId[i]] += realVal[i] * p[colId[i]];
     }

     return Result;
}

vector <double> sparseM_vec_prodDebugSPSM(vector <double> &p, 
                        std::vector<int> &rowId, 
                        std::vector<int> &colId, 
                        std::vector<double> &realVal) {

     std::vector <double> Result;
     Result.assign(p.size(), 0.00);

     for (unsigned i= 0; i < rowId.size(); i++) {
        cout << "Sspsm\t" << i << "\t" << realVal[i] << " " << p[colId[i]] << endl;
         
        Result[rowId[i]] += realVal[i] * p[colId[i]];
     }

     return Result;
}

vector <double> sparseM_vec_prod(vector <double> &p, 
                        std::vector<int> &rowId, 
                        std::vector<int> &colId, 
                        std::vector<double> &realVal) {

     std::vector <double> Result;
     Result.assign(p.size(), 0.00);

     for (unsigned i= 0; i < rowId.size(); i++) {
        //cout << realVal[i] << " " << p(colId[i]) << endl;
         
        Result[rowId[i]] += realVal[i] * p[colId[i]];
     }

     return Result;
}


double getMainTagPropInit(std::vector < double >&arg) {

	double mainTagProp = 0;

	for (unsigned i = 0; i < arg.size(); i++) {
		mainTagProp += arg[i];
	}

	return max(0.01,(1 - mainTagProp));
}

double getMainTagPropMeanScaled(std::vector < double >&arg) {

	double mainTagProp = 0;

	for (unsigned i = 0; i < arg.size(); i++) {
		mainTagProp += arg[i];
	}

	return max(0.01,min(1.00,(1.00 - mainTagProp)));
}


vector <pair <int,int> > getIndexFromVector(std::vector <string> &nbNt, std::vector<string> &arg) {
    vector <pair< int, int> > foundIndex;

    for (unsigned i=0; i < nbNt.size() ; i++) {
        //cout << i ;
        for (unsigned j=0; j <arg.size() ; j++) {
           //cout << "\t"  << nbNt[i] << "," << arg[j] << " " << j <<  endl;
           
           if ( nbNt[i] == arg[j]) {
                 //cout << "\t"  << nbNt[i] << "," << arg[j] << " " << j <<  endl;
                 pair <int,int> tuple = make_pair(i,j);
                 foundIndex.push_back(tuple); 
           }

        }

    }

    return foundIndex;
}


vector <pair <int,int> > getIndexFromMap(std::vector <string> &nbNt, std::map<string,int>&m) {
    vector <pair< int, int> > foundIndex;

    for (unsigned i=0; i < nbNt.size() ; i++) {


        map<string,int>::iterator iter = m.find(nbNt[i]);

        if ( iter != m.end()) {
            int j = iter->second; 
            pair <int,int> tuple = make_pair(i,j);
            foundIndex.push_back(tuple); 
        }

    }

    return foundIndex;
}

vector <double> getPropSum(std::vector < string >&neigb, map<string,double>&m ) {

    vector <double> propVec;

    // Find value of a neigbours
    // from the map, otherwise return zero

    for ( unsigned i= 0; i<neigb.size()  ;i++ ) {
        map<string,double>::iterator iter = m.find(neigb[i]);
            double value = 0;

            if ( iter != m.end()  ){
                value = m[neigb[i]];
            }
            propVec.push_back(value);
            //cout << neigb[i] << " " << value << endl;
    }


    // Sum the vector in three-three
    // Resulting vector = nofpos

    vector <double> pSum;
    int nofPos = neigb.size()/3;
    int b = 0;
    for ( int k=0; k<nofPos ; k++ ) {
       double output = propVec[b] + propVec[b + 1] + propVec[b + 2];
       pSum.push_back(output);
       b += 3;
    }

    
    return pSum;
}

vector <double> getNumTagProp(std::vector < string >&neigb, map<string,double>&m ) {

    vector <double> propVec;

    // Find value of a neigbours
    // from the map, otherwise return zero

    for ( unsigned i= 0; i<neigb.size()  ;i++ ) {
        map<string,double>::iterator iter = m.find(neigb[i]);
            double value = 0;

            if ( iter != m.end()  ){
                value = m[neigb[i]];
            }
            propVec.push_back(value);
            //cout << neigb[i] << " " << value << endl;
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
    bool operator()(const std::vector<int> &fs, const std::vector<int> &sc) {
        return fs < sc;
    }
};

struct sort_pred_double {
    // for sorting Vector of Vector. Only  based on second->first ascendingly
    bool operator()(const std::vector<double> &lhs, const std::vector<double> &rhs) {
        return (lhs[1] < rhs[1] || (rhs[0] == lhs[0] && lhs[1] < rhs[1]));
    }
};



struct strCmp {
    bool operator()( const char* s1, const char* s2 ) const {
        return strcmp( s1, s2 ) < 0;
    }
};






/* 
 * ====================================================================================
 * End Struct here 
 *  
 * =====================================================================================
 */

int main  ( int arg_count, char *arg_vec[] ) {
    if (arg_count !=2 ) {
        cerr << "Expected one argument" << endl;
        return EXIT_FAILURE;
    }

    /* 
       Final Output Variables (Sparse)
       RA -> Vector with real value
       JA -> Vector with colum index
       IA -> Vector with row index
    */
        
    vector <double> RA;
    vector <int> JA;
    vector <int> IA;
    vector <string> ordNumTags;
    map<string,int> ordNumTagsMap;

    vector < vector <int> > accumIndexInOrd;   
    vector < vector <int> > accumIndexIA;   
    vector < vector <double> > accumIndexRA;
    vector <double> rawCount; // raw count container
    vector <string> Tags;

    // Store Prop File as Hash
    map<string,double> theMap;
    string line;
    string qualFileName = arg_vec[1];
    string baseName = GetBaseNameFromFilename(qualFileName);
    string pathName = GetPathNameFromFilename(qualFileName);

    string nbFileName       = pathName + baseName + ".nb";
    string propFileName     = pathName + baseName + ".prop";
    string nbQualFileName   = pathName + baseName + ".nbq";
    //cout << propFileName << endl;
    
    ifstream propfile (propFileName.c_str());

    int numberOfSeq = 0;
    int ordId = 0;
    vector <double> Pinit;

    if (propfile.is_open())
    {
        while (getline(propfile,line) )
        {
                numberOfSeq++;
                stringstream ss(line);
                string numTag;
                double prop;

                ss >> numTag >> prop;
                //cout << numTag << "\t" << prop << endl;
                theMap.insert(make_pair(numTag,prop));
                JA.push_back(numberOfSeq-1);
                Pinit.push_back(prop);
                ordNumTagsMap.insert(make_pair(numTag,ordId++));

        }
        propfile.close();


    }
    else { cout << "Unable to open input file\n"; }; 


   // for( map<const string, int, strCmp>::iterator iter = ordNumTagsMap.begin(); iter != ordNumTagsMap.end(); ++iter) {
   //      cout << (*iter).first << " - " << (*iter).second << endl; 
   // }

    // Initialize IA first column
    // it's the same with JA
    IA = JA;

    // Begin iterating the neigbours file and quality file, then find 
    // estimated error mean

    ifstream nbfile (nbFileName.c_str());
    ifstream qlfile (qualFileName.c_str());
    ifstream nbqfile (nbQualFileName.c_str()); 

    string nbline;
    string qlline;
    string nbqline;

    int lineno = 0;
    int lineno_ = 0;

    //std::clock_t startTime = std::clock();

    if ( nbfile.is_open() && qlfile.is_open() && nbqfile.is_open()) {

        while ( getline(nbfile,nbline) && getline(qlfile,qlline) && getline(nbqfile,nbqline) ){
                stringstream sn(nbline);

                // Parsing Neighbors File
                string numTagd;
                string numTags;
                string numTagnb;
                vector<string> nbnumTags;

                sn >> numTags >> numTagd;
                Tags.push_back(numTags);
                string numTag =  numTagd;
                
                while ( sn >> numTagd  ){
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

                while ( sp >> numTagProp  ){
                    nbnumTagsProp.push_back(numTagProp);
               }


                //
                // Begin computing Rtags.A
                //

                vector < double >propSum      = getPropSum(nbnumTags, theMap);
                vector < double >propOfNumTag = getNumTagProp(nbnumTags, theMap);
                double MainTagProp            = getMainTagPropInit(nbnumTagsProp);

               vector<double> rTagsQual;
               rTagsQual.reserve(nbnumTagsProp.size());

                // Find Error Mean
                int groupIndex = 0;
                for (unsigned x = 0; x < propSum.size(); x += 1 ){
                    double  rTagsQual1 = nbnumTagsProp[groupIndex] * propOfNumTag[groupIndex] / propSum[x];
                    double  rTagsQual2 = nbnumTagsProp[groupIndex + 1] * propOfNumTag[groupIndex + 1] / propSum[x];
                    double  rTagsQual3 = nbnumTagsProp[groupIndex + 2] * propOfNumTag[groupIndex + 2] / propSum[x];

                   if (isnan(rTagsQual1) ) {
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

               MainTagProp =  getMainTagPropMeanScaled(rTagsQual); 


               vector <pair <int,int> > indexInOrd = getIndexFromMap(nbnumTags,ordNumTagsMap); 
               //cout << indexInOrd.size() << "\t" << rTagsQual.size() << endl;
                
                vector < vector <int> > indexInOrdLineNo;
                for (unsigned k = 0 ; k < indexInOrd.size(); k++) {
                    pair <int,int> tpl = indexInOrd[k];
                    //cout << tpl.first << "-" << tpl.second << endl;
                    vector <int> temp;
                    temp.push_back(tpl.first);
                    temp.push_back(lineno++);
                    temp.push_back(tpl.second);
                    indexInOrdLineNo.push_back(temp);
                }

                //printf("%.7f\t", MainTagProp);
                //cout << "RTAGS QUAL " ;;
                //prn_vec<double>(rTagsQual,"\t");
                //cout << endl;
                
                accumIndexInOrd.insert(accumIndexInOrd.end(), indexInOrdLineNo.begin(), indexInOrdLineNo.end());

                int tln = lineno_++;

                RA.push_back(MainTagProp);                
                rawCount.push_back(double(observedCount));

                // For IA and RA
                vector < vector <int> > forIA;
                vector < vector <double> > forRA;

                for (unsigned n=0; n < rTagsQual.size(); n++) {
                      vector <int> temp2;
                      vector <double> temp3;

                    if (rTagsQual[n] > 0) {
                        //cout << rTagsQual[n] << "-" << tln << "-" << n+1 << " ";
                        temp2.push_back(n+1);
                        temp2.push_back(tln);
                        forIA.push_back(temp2);
                        
                        temp3.push_back(double(tln));
                        temp3.push_back(double(n+1));
                        temp3.push_back(rTagsQual[n]);
                        forRA.push_back(temp3);

                    }
                }
            
                accumIndexIA.insert(accumIndexIA.end(), forIA.begin(), forIA.end()); 
                accumIndexRA.insert(accumIndexRA.end(), forRA.begin(), forRA.end()); 
                

           //cout << " ------- " <<endl;

        }
            nbfile.close(); 
            qlfile.close();
            nbqfile.close(); 
    }
    else {
        cout << "One of the three files NB,QL, and NBQ cannot be open\n";
    }


   //cerr << "# CPU time for parsing two files: " << (clock() - startTime + 0.0) / CLOCKS_PER_SEC
   //    << " seconds\n";  


/*
 Here we extract show all the sparse matrix param (RA,JA,IA);
 Then convert them into the standard SparseMatrix
*/

 //std::clock_t startSparseTime = std::clock();

 //cout << "DIM " << lineno_ << "x" << lineno_ << endl;
 //cout << "RAW COUNT ";
 //prn_vec_err<double>(rawCount,",");
 //cout << rawCount.size();
 //cout << endl;

 // Print and keep RA
 std::stable_sort(accumIndexRA.begin(), accumIndexRA.end(), sort_pred_double());
 for (unsigned z=0; z < accumIndexRA.size(); z++) {
     vector <double> dbtriplet = accumIndexRA[z];
     //cout << "\t" << dbtriplet[0] << "\t" << dbtriplet[1] << "\t" << dbtriplet[2] <<  endl;
     RA.push_back(dbtriplet[2]);
 }


 //Print and Keep IA
 std::sort(accumIndexIA.begin(), accumIndexIA.end(), sort_pred());
 for (unsigned y=0; y < accumIndexIA.size(); y++) {
     vector <int> duplet = accumIndexIA[y];
     IA.push_back(duplet[1]);
 }

 
 // Print and Keep JA
 std::sort(accumIndexInOrd.begin(), accumIndexInOrd.end(), sort_pred());
 for (unsigned k = 0 ; k < accumIndexInOrd.size(); k++) {
     vector <int>  triplet = accumIndexInOrd[k];
     JA.push_back(triplet[2]);

 }


 if (RA.size() != IA.size() || IA.size() != JA.size() ) {
    cerr << "RA : ";
    //prn_vec_err<double>(RA, " ");
    cerr <<  RA.size() << endl;
    cerr << "IA : " ;
    //prn_vec_err<int>(IA, " ");
    cerr << IA.size() << endl;
    cerr << "JA : " ;
    //prn_vec_err<int>(JA," ");
    cerr << JA.size() << endl;
    cerr << "Error: unequal dimension sizes in sparse matrix" << endl;
    exit(EXIT_FAILURE);
      
 }



 // Compute Lfree = unclamped likelihood
 double lambda_free = double(lineno_);
 int maxStep_free = 51;
 
 vector <double> nCount_free = rawCount;
 vector <double> theM_free   = rawCount;
  double loglik_free = 0;

  vector <double> ExpCountFreeVec;
  //double m_temp_free = 0;
  double temp_loglik_free = 0;

 for (int m_free = 0; m_free < maxStep_free; m_free++) {
     //cout << "Free Step " << m_free << endl;

     vector <double> theP_free; 
     vector <double> sparseM_prod_P_free; 
     vector <double> rSums_free;
     vector <double> tsparseM_prod_rSums_free;
      

     
     theP_free                = divideVecWithScalar(theM_free,lambda_free); 
     vector <double> theP_freeNrm = normalizeP(theP_free);

     sparseM_prod_P_free      = sparseM_vec_prod(theP_freeNrm,IA,JA,RA);
     rSums_free               = divideVecWithVecCorsp(nCount_free,sparseM_prod_P_free);
     tsparseM_prod_rSums_free = sparseM_vec_prod(rSums_free,JA,IA,RA);
     theM_free                = multiplyVecWithVecCorsp(theP_freeNrm, tsparseM_prod_rSums_free);
     loglik_free              = computeLogLik(theM_free, theP_freeNrm, lambda_free);

     //cout << loglik_free << endl;

     //double diff_mfree = theM_free[0] - m_temp_free;
     //m_temp_free = theM_free[0];
     //double diff_loglik_free = loglik_free - temp_loglik_free;
     double diff_loglik_free = relative_diff_loglik(loglik_free,temp_loglik_free);
     temp_loglik_free = loglik_free;

     /*--------------------------------------------------
     *      cout << "Free Step " << m_free << "\t"  ;
     *      cout << "\tLoglik Free " << loglik_free << "\tll_diff " << diff_loglik_free  << endl;
     *      cout << "theP\t" ;
     *      prn_vec<double>(theP_free,"\t");
     *      cout << endl;
     *      cout << "theM\t" ;
     *      prn_vec<double>(theM_free,"\t");
     *      cout << endl;
     *      cout << "lambda\t" ;
     *      cout << lambda_free << endl;
     *--------------------------------------------------*/

     

     //Show only the last iteration
     if (diff_loglik_free < 0.001) {
     //if ( m_free == maxStep_free-1) {

         for (unsigned i=0; i<theM_free.size(); i++) {
            double ExpCount_free = theM_free[i];

             ExpCountFreeVec.push_back(ExpCount_free);

            //cout << Tags[i] << "\t" << fixed <<  setprecision(3) <<  rawCount[i] << "\t";
            //printf("%.3f", ExpCount);
            //cout << "\t";
            //cout << endl;
         }


         break;
     }




 }

 //cout << "++++++++++ END OF LOGLIK FREE ++++++" << endl;



 // Compute L0 = likelihood of each tag clamped into 0
 // Run this EM inside a loop of tags inside every connected components
 //cout << Tags.size() << endl;

  //cout << "CC Size " << Tags.size() << endl;
  for (unsigned tag_i = 0 ; tag_i < Tags.size(); tag_i++) {

      //cout << "Tag No To Clamp: " << tag_i << " " <<  Tags[tag_i] << "\t" << fixed <<  setprecision(3) <<  rawCount[tag_i] << "\t"  ;
      cout <<  Tags[tag_i] << "\t" << fixed <<  setprecision(5) << rawCount[tag_i] << "\t" << ExpCountFreeVec[tag_i] << "\t"  ;
      //cout <<  Tags[tag_i] << "\t" << fixed <<  setprecision(3) <<  rawCount[tag_i] << "\t"  ;
      //printf("%.3f", ExpCountFreeVec[tag_i]); 
      //cout << "\t";


      // For each of this tag run EM 
      /*
      * *********************
      *
      * BEGIN EM Step
      *
      *********************
      */
      //std::clock_t startEMTime = std::clock();

    double lambda = double(lineno_);
    int maxStep = 51;
    
    double temp_loglik = 0;
    vector <double> nCount = rawCount;
    vector <double> theM   = rawCount;
    
    //cout << endl;

    for (int m = 0; m < maxStep; m++) {

        vector <double> theP; 
        vector <double> sparseM_prod_Pnew; 
        vector <double> rSums;
        vector <double> tsparseM_prod_rSums;
        double loglik = 0.00;
        
         //cout << "\tStep " << m <<  endl;
        //theP                = divideVecWithScalarDebug(theM,lambda); 
        theP                = divideVecWithScalar(theM,lambda); 


        vector <double> thePnew = theP;
        vector <double> nCountnew = nCount;

        thePnew[tag_i] = 0.000;

        vector <double> thePnewNrm = normalizeP(thePnew);
        //double totPnew = 0.00;
        sparseM_prod_Pnew   = sparseM_vec_prod(thePnewNrm,IA,JA,RA);
        //sparseM_prod_Pnew   = sparseM_vec_prodDebugSPSM(thePnewNrm,IA,JA,RA);
        //cout << "sparseM_prod_Pnew" << endl;
        //prn_vec<double>(sparseM_prod_Pnew, "\n");
        //cout << endl;

        rSums               = divideVecWithVecCorsp(nCountnew,sparseM_prod_Pnew);
        //rSums               = divideVecWithVecCorspDebug(nCountnew,sparseM_prod_Pnew);
        
        //cout << "rSums" << endl;
        //prn_vec<double>(rSums, "\n");
        //cout << endl
        
        tsparseM_prod_rSums = sparseM_vec_prod(rSums,JA,IA,RA);
        //tsparseM_prod_rSums = sparseM_vec_prodDebug(rSums,JA,IA,RA);
        //cout << "tsparseM_prod_rSums" << endl;
        //prn_vec<double>(tsparseM_prod_rSums, "\n");
        //cout << endl;


        //theM                = multiplyVecWithVecCorspDebug(thePnewNrm, tsparseM_prod_rSums);
        theM                = multiplyVecWithVecCorsp(thePnewNrm, tsparseM_prod_rSums);
        
        //cout << "theM" << endl;
        //prn_vec<double>(theM, "\n");
        //cout << endl;

        
        //loglik              = computeLogLikdebug(theM, thePnewNrm, lambda);
        loglik              = computeLogLik(theM, thePnewNrm, lambda);

        //cout << "\t" << theM[0] << endl;;
        //prn_vec<double>(theM, "\n");
        //cout << "Pnew" << endl;
        //prn_vec<double>(thePnewNrm, "\n");
        //cout << lambda;
        //cout << endl;

         


        //double diff_loglik = loglik - temp_loglik;
        double diff_loglik = relative_diff_loglik(loglik,temp_loglik);

        temp_loglik = loglik;

        double llfree_ll = loglik_free - loglik;
        //cout << " LL " <<  loglik << " " << llfree_ll <<  endl;

          
          
     //if ( m == maxStep-1) {
     if ( diff_loglik  < 0.01) {

        //cout << "LL_free=" << loglik_free <<", LL_0= " << loglik << "\t" << llfree_ll <<  endl;
        cout   << fixed << setprecision(15)  << "\t" <<   llfree_ll ;
        cout << "\n";

            //for (unsigned i=0; i<theM.size(); i++) {
                //double ExpCount = theM[i];
                //cout << "\t\t" << Tags[i] << "\t" << fixed <<  setprecision(3) <<  rawCount[i] << "\t";
                //printf("%.3f", ExpCount);
                //cout << "\t";
                //cout << endl;
           // }

            //final_step = m;
            break;

        
     }


    }

    //cout << "Final Step " << final_step << endl;
     //cout << endl;

 }

//cerr << "# CPU time for EM: " << (clock() - startEMTime + 0.0) / CLOCKS_PER_SEC
//    << " seconds\n";  


    return 0;
}

