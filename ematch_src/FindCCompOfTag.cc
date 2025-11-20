// =====================================================================================
//
//
// Copyright 2010, Edward Wijaya
// =====================================================================================

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <cstdlib>  // EXIT_FAILURE, atoi()

using namespace std;

// Command
// ./ConfInterval ccoc1.txt TagID_to_check_the_CC_member

int main(int arg_count, char* arg_vec[]) {
    if (arg_count != 3) {
        cerr << "expected one argument" << endl;
        return EXIT_FAILURE;
    }

    string line;
    ifstream myfile(arg_vec[1]);
    int inputTagId = static_cast<int>(atoi(arg_vec[2]));

    cout << "The input : " << inputTagId << endl;

    map<int, vector<int>> CCTagMap;
    map<int, int> TagId2CCId_Map;

    if (myfile.is_open()) {
        // Skip First Line
        getline(myfile, line);

        while (getline(myfile, line)) {
            stringstream ss(line);
            int CcId;
            int TagId = -1;

            while (ss >> CcId) {
                TagId++;
                TagId2CCId_Map.insert(make_pair(TagId, CcId));
                // cout << CcId << "-" << TagId <<  endl;

                if (CCTagMap.find(CcId) == CCTagMap.end()) {
                    CCTagMap.insert(make_pair(CcId, vector<int>()));
                }
                CCTagMap[CcId].push_back(TagId);
            }
        }
        myfile.close();
    } else {
        cout << "Unable to open file\n";
    }


    map<int, int>::iterator it = TagId2CCId_Map.find(inputTagId);

    if (it != TagId2CCId_Map.end()) {
        int CCIdOfTheInput = TagId2CCId_Map[inputTagId];
        cout << "CCID_OF_INPUT = " << CCIdOfTheInput << endl;


        // Get the Members of the Connected Component

        map<int, vector<int>>::iterator it2 = CCTagMap.find(CCIdOfTheInput);

        cout << "Member of this CComp are " << endl;


        if (it2 != CCTagMap.end()) {
            vector<int> TagIdList = CCTagMap[CCIdOfTheInput];

            for (unsigned i = 0; i < TagIdList.size(); i++) {
                cout << TagIdList[i] << endl;
            }
        }
    }

    /*--------------------------------------------------
     *
     *     for (map <int, vector <int> >::iterator iter = CCTagMap.begin(); iter != CCTagMap.end();
     *iter++) {
     *
     *         vector<int> TagIdList = (*iter).second;
     *         cout <<(*iter).first << ": " ;
     *         for (unsigned i=0; i<TagIdList.size(); i++) {
     *              cout << TagIdList[i] << ",";
     *         }
     *         cout << endl;
     *     }
     *
     *--------------------------------------------------*/


    return 0;
}
