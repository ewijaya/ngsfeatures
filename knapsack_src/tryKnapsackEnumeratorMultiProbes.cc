/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul B. Horton, All rights reserved.
 *  Creation Date: 2009.2.4
 *  Last Modified: $Date: 2009/07/06 08:25:19 $
 *
 *  Purpose: try code involving KnapsackEnumerator
 */
#include "./KnapsackEnumerator.hh"
#include "./KnapsackObjectVector.hh"
#include "utils/argvParsing/ArgvParser.hh"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <cmath>


typedef cbrc::KnapsackEnumerator::weightT weightT;
weightT arg_capacity;
std::istream* arg_objectStreamPtr;
std::vector<std::string> GlobalVec;

namespace cbrc {

std::string GetBaseNameFromFilename(const std::string& fileName) {
    std::string fbaseNameWithExt = fileName.substr((fileName.find_last_of("/\\")) + 1);
    int fperiodIn = fbaseNameWithExt.find(".");
    std::string fbaseName = fbaseNameWithExt.substr(0, fperiodIn);

    return fbaseName;
}

std::string GetPathNameFromFilename(const std::string& filename) {
    std::string foundPath = filename.substr(0, filename.rfind("/") + 1);

    if (foundPath == "") {
        foundPath = "./";
    }
    return foundPath;
}


double Solexa2Phred(double sQ) {
    // Convert Solexa into Phred Quality
    double pQ = 10.00 * log(1.00 + pow(10.00, (sQ / 10.0))) / log(10.00);
    return pQ;
}

double Phred2ErrProb(double pQ) {
    double errprob = (pow(10, -(pQ) / 10.00)) / 3;
    return errprob;
}


std::string Int2String(int IntVal) {
    std::string S;
    std::stringstream out;
    out << IntVal;
    S = out.str();

    return S;
}

std::string Double2String(double IntVal) {
    std::string S;
    std::ostringstream out;
    out << IntVal;
    S = out.str();

    return S;
}

std::string DNA2Num(std::string DNAStr) {
    std::map<char, int> lookup;
    lookup['A'] = 0;
    lookup['C'] = 1;
    lookup['G'] = 2;
    lookup['T'] = 3;


    std::string FinalNumTag = "";
    for (unsigned i = 0; i < DNAStr.size(); i++) {
        int cb = lookup[DNAStr[i]];
        std::string cbstr = Int2String(cb);
        FinalNumTag.append(cbstr);
    }


    return FinalNumTag;
}

std::vector<std::string> getRemainingBase(char Base) {
    std::vector<std::string> RemBases;

    std::vector<char> AllBases;
    AllBases.push_back('A');
    AllBases.push_back('C');
    AllBases.push_back('G');
    AllBases.push_back('T');


    for (unsigned i = 0; i < AllBases.size(); i++) {
        if (Base != AllBases[i]) {
            char rB = AllBases[i];
            std::stringstream ss;
            std::string tempStr;
            ss << rB;
            ss >> tempStr;

            RemBases.push_back(tempStr);
        }
    }


    return RemBases;
}

std::string CreateNeighborTag(std::string NBases, std::vector<size_t> Pos2Mut, std::string OTag) {
    std::string NewString = OTag;

    for (unsigned i = 0; i < Pos2Mut.size(); i++) {
        char Base = NBases[i];

        std::stringstream ss;
        std::string tempBase;
        ss << Base;
        ss >> tempBase;

        NewString.replace(Pos2Mut[i], 1, tempBase);
    }


    return NewString;
}


void EnumAll(const std::vector<std::vector<std::string>>& allVecs, size_t vecIndex,
             std::string strSoFar) {
    if (vecIndex >= allVecs.size()) {
        // cout << strSoFar << endl;
        GlobalVec.push_back(strSoFar);

        return;
    }
    for (size_t i = 0; i < allVecs[vecIndex].size(); i++) {
        EnumAll(allVecs, vecIndex + 1, strSoFar + allVecs[vecIndex][i]);
    }
}


void GetNbBaseForEachPos(std::vector<size_t> Positions2Mutate, std::string OrgTag,
                         double FEprobProd, std::ostream& NBF, std::ostream& NBQF) {
    std::vector<std::vector<std::string>> RemBasesAllPosVec;

    for (unsigned i = 0; i < Positions2Mutate.size(); i++) {
        int p2m = Positions2Mutate[i];
        char Base2Mutate = OrgTag[p2m];
        // std::cout << "\t\t" << p2m << "," << Base2Mutate << "\tRB: ";
        std::vector<std::string> remBases = getRemainingBase(Base2Mutate);

        RemBasesAllPosVec.push_back(remBases);


        // for (unsigned j=0; j<remBases.size(); j++) {
        // std::cout << remBases[j] << ",";
        //}
        // std::cout << std::endl;
    }

    EnumAll(RemBasesAllPosVec, 0, "");

    for (unsigned i = 0; i < GlobalVec.size(); i++) {
        // std::cout << "\t\t\t" << GlobalVec[i] << "->";
        std::string NeighborBases = GlobalVec[i];

        if (NeighborBases.size() != Positions2Mutate.size()) {
            std::cerr << "NeighborBases Not Equal to Position2Mutate" << std::endl;
            break;
        }

        std::string NeighborTag = CreateNeighborTag(NeighborBases, Positions2Mutate, OrgTag);
        std::string NeighborTagNum = DNA2Num(NeighborTag);


        // double logFEprobProd = -log(FEprobProd);
        // std::cout <<  NeighborTagNum << "\t";
        NBF << NeighborTagNum << "\t";
        NBQF << FEprobProd << "\t";
        // std::cout << "\t\tnb_str: " <<  NeighborTag << "\t"  << NeighborTagNum << "\t" <<
        // FEprobProd << "\t" << logFEprobProd <<std::endl;
    }

    // std::cout << std::endl;


    GlobalVec.clear();
}

std::vector<double> ConvErrProbVec2LogProbVec(std::vector<double> ProbVec) {
    std::vector<double> LogProbVec;

    for (unsigned i = 0; i < ProbVec.size(); i++) {
        double logProb = -log(ProbVec[i]);
        LogProbVec.push_back(logProb);
    }

    return LogProbVec;
}

void tryKnapsackEnumeratorVectOfPair(double capacity, std::vector<double>& VOProb,
                                     std::string InitTag, std::ostream& nbf, std::ostream& nbqf) {
    KnapsackEnumerator sack(capacity);
    // std::cout << "capacity is: " << sack.capacity() << std::endl;

    std::vector<double> VOlogProb = ConvErrProbVec2LogProbVec(VOProb);

    KnapsackObjectVector objects(VOlogProb);
    sack.computeCombinations(objects);

    /*--------------------------------------------------
     *   std::cout  <<  "Sorted objects are:\n";
     *   std::cout << "Array sack.objectUniverse():" << std::endl;
     *   for( size_t i = 0; i < sack.objectUniverse().size(); ++i ){
     *     if( i ) std::cout << " ";
     *     std::cout << sack.objectUniverse()(i);
     *   }
     *   std::cout << std::endl;
     *
     *--------------------------------------------------*/

    for (size_t i = 0; i < sack.combinations().size(); ++i) {
        const KnapsackEnumerator::idVecT& curCombination = sack.combinations()[i];
        std::vector<size_t> AllPos2Mutate;
        double FinalErrProb = 1;
        for (size_t j = 0; j < curCombination.size(); ++j) {
            size_t Pos2Mutate = sack.objectUniverse()(curCombination[j]).id();
            // std::cout << Pos2Mutate << ",";
            AllPos2Mutate.push_back(Pos2Mutate);
            FinalErrProb *= VOProb[Pos2Mutate];
        }

        GetNbBaseForEachPos(AllPos2Mutate, InitTag, FinalErrProb, nbf, nbqf);

        // std::cout  <<  std::endl;
    }
}

void tryKnapsackEnumerator() {
    KnapsackEnumerator sack(arg_capacity);
    // std::cout << "capacity is: " << sack.capacity() << std::endl;
    KnapsackObjectVector objects(*arg_objectStreamPtr);

    // std::cout  <<  "objects are:  "   <<  objects  <<  std::endl;
    sack.computeCombinations(objects);

    // std::cout  <<  "Sorted objects are:\n";
    // std::cout << "Array sack.objectUniverse():" << std::endl;
    // for( size_t i = 0; i < sack.objectUniverse().size(); ++i ){
    //   if( i ) std::cout << " ";
    //   std::cout << sack.objectUniverse()(i);
    // }
    // std::cout << std::endl;


    // std::cout  <<  "Possible combinations are:\n";

    for (size_t i = 0; i < sack.combinations().size(); ++i) {
        const KnapsackEnumerator::idVecT& curCombination = sack.combinations()[i];

        for (size_t j = 0; j < curCombination.size(); ++j) {
            if (j)
                std::cout << ",";
            std::cout << sack.objectUniverse()(curCombination[j]).id();
        }

        std::cout << std::endl;
    }
}
}  // namespace cbrc


int main(int argc, const char* argv[]) {
    // cbrc::ArgvParser argvP( argc, argv, "capacity TagsCountQualFile" );


    // argvP.setOrDie( arg_capacity, 1 );
    // arg_objectStreamPtr  =  argvP.getIstreamPtr( 2 );
    // argvP.dieIfUnusedArgs();

    double Capacity = static_cast<double>(atof(argv[1]));
    std::string TagsCountQualFile = argv[2];
    std::string line;

    std::string capacity = cbrc::Double2String(Capacity);

    std::string baseName = cbrc::GetBaseNameFromFilename(TagsCountQualFile);
    std::string pathName = cbrc::GetPathNameFromFilename(TagsCountQualFile);

    std::string nbFileName = pathName + baseName + "_" + capacity + ".nb";
    std::string nbqFileName = pathName + baseName + "_" + capacity + ".nbq";

    // std::cerr << TagsCountQualFile << " " << Capacity << std::endl;
    // std::cerr << nbFileName << " " << nbqFileName << std::endl;
    std::ifstream TCQFile(TagsCountQualFile.c_str());


    std::ofstream nbFile;
    nbFile.open(nbFileName.c_str(), std::ios::out);

    std::ofstream nbqFile;
    nbqFile.open(nbqFileName.c_str(), std::ios::out);


    if (TCQFile.is_open()) {
        while (getline(TCQFile, line)) {
            std::stringstream ss(line);
            std::string ObservedCount;
            std::string Tag;
            double Quality;

            ss >> ObservedCount >> Tag;

            std::string NumericTag = cbrc::DNA2Num(Tag);

            // std::cout <<  Tag  << "\t" << NumericTag <<"\t\t";
            nbFile << Tag << "\t" << NumericTag << "\t\t";
            nbqFile << Tag << "\t" << NumericTag << "\t\t";


            std::vector<std::pair<int, double>> VecOfIndexProbPair;
            std::vector<double> VecOfProb;

            int mmPos = -1;
            while (ss >> Quality) {
                mmPos++;
                double eP = cbrc::Phred2ErrProb(cbrc::Solexa2Phred(Quality));
                // std::cout << "\t" <<  Quality << "," << eP  << "," << mmPos << std::endl;
                VecOfProb.push_back(eP);
            }

            // Pass VecOfIndexPair and Capacity here to Knapsack function
            cbrc::tryKnapsackEnumeratorVectOfPair(Capacity, VecOfProb, Tag, nbFile, nbqFile);

            // std::cout << std::endl;
            nbFile << std::endl;
            nbqFile << std::endl;
        }

        TCQFile.close();
        nbFile.close();
        nbqFile.close();
    }


    // cbrc::tryKnapsackEnumerator();
    return 1;
}
