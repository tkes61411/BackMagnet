// **************************************************************************
//    File       [simulator.h]
//    Author     [Yu-Hao Ho]
//    Synopsis   [The header file of simulator]
//    Created    [2016/08/20 Yu-Hao Ho]
// **************************************************************************

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <vector>
#include <string>
using namespace std;

class StockSimulator {
    public :
        enum {
            GoldenCross = 0, 
            PriceLargerThanAverage5,
            PriceLargerThanAverage10,
            PriceLargerThanAverage20,
            DeadCross,
            PriceSmallerThanAverage5,
            PriceSmallerThanAverage10,
            PriceSmallerThanAverage20
        };
        StockSimulator();
        void set(string); // parse script
        void printInfo();
        //void run();
        int stockID;
        vector< vector<int> > buyCondition; 
        vector< vector<int> > sellCondition;
        bool hold; // already have this stock
        float buyPrice;
        vector<int> gain;



};
#endif
