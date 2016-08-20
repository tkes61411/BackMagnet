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
#include <map>
using namespace std;

class StockSimulator {
    public :
        enum {
            GoldenCross = 1, 
            PriceLargerThanAverage5,
            PriceLargerThanAverage10,
            PriceLargerThanAverage20,
            DeadCross,
            PriceLowerThanAverage5,
            PriceLowerThanAverage10,
            PriceLowerThanAverage20
        };
        StockSimulator();
        void setMap(map< int, vector< vector<float> > >);
        void set(string); // parse script
        void printInfo();
        void printGain();
        void printMap();
        void run();
        bool checkBuyCondition(vector< float>);
        bool checkSellCondition(vector< float>);
        int stockID_;
        string buyCommand_, sellCommand_;
        vector< vector<int> > buyCondition_; 
        vector< vector<int> > sellCondition_;
        bool hold_; // already have this stock
        float buyPrice_;
        float lastGain_;
        vector<float> gain_;
        map < int, vector < vector<float> > > stockMap_;


};
#endif
