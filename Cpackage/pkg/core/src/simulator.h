// **************************************************************************
//    File       [simulator.h]
//    Author     [Yu-Hao Ho]
//    Synopsis   [The header file of simulator]
//    Created    [2016/08/20 Yu-Hao Ho]
// **************************************************************************

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <vector>
using namespace std;

class StockSimulator {
    public :
        StockSimulator();
        //void set(); // parse script
        void run();
        int stockID;
        bool buyConstraint1; // KD good convergence
        bool buyConstraint2; // 
        bool buyConstraint3; //
        bool sellConstraint1; // KD dead convergence
        bool sellConstraint2; //
        bool sellConstraint3; //
        bool hold; // already have this stock
        float buyPrice;
        vector<int> gain;



};
#endif
