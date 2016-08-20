#include "simulator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

using namespace std;

StockSimulator::StockSimulator() {
    hold_ = false;
    gain_.clear();
}

bool StockSimulator::checkBuyCondition(vector<float> data, int idx, vector< vector<float> > weekData, vector< vector<float> > monthData) {
    bool result = false;
    int wIdx = idx / 5; // which week?
    int mIdx = idx / 22;
    for(unsigned int i = 0; i < buyCondition_.size(); ++i) { // for every OR condition 
        bool check = false;
        //if(buyCondition_.size() > 1 || buyCondition_[i].size() > 1) cout << "?" << endl;
        // close, high, low, open, vol, avaerage5, average10, average20, K9, D9
        for (unsigned int j = 0; j < buyCondition_[i].size(); ++j) { // for every AND condition
            switch(buyCondition_[i][j]) {
                case GoldenCross:
                    if (data[8] > data[9]) // K > D
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekGoldenCross:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][8] > weekData[wIdx - 1][9]) // K > D
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthGoldenCross:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][8] > monthData[mIdx - 1][9]) // K > D
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case PriceLargerThanAverage5:
                    if (data[0] > data[5]) // Close > Average5
                        check = true;
                    else 
                        check = false; 
                    break;
                case PriceLargerThanAverage10:
                    if (data[0] > data[6]) // Close > Average10
                        check = true;
                    else 
                        check = false;
                    break;
                case PriceLargerThanAverage20:
                    if (data[0] > data[7]) // Close > Average20
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekPriceLargerThanAverage5:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] > weekData[wIdx - 1][5]) // Close > Average5
                            check = true;
                        else 
                            check = false; 
                    }
                    break;
                case WeekPriceLargerThanAverage10:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] > weekData[wIdx - 1][6]) // Close > Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case WeekPriceLargerThanAverage20:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] > weekData[wIdx - 1][7]) // Close > Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLargerThanAverage5:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] > monthData[mIdx - 1][5]) // Close > Average5
                            check = true;
                        else 
                            check = false; 
                    }
                    break;
                case MonthPriceLargerThanAverage10:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] > monthData[mIdx - 1][6]) // Close > Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLargerThanAverage20:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] > monthData[mIdx - 1][7]) // Close > Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case DeadCross:
                    if (data[9] > data[8]) // D > K
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekDeadCross:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][9] > weekData[wIdx - 1][8]) // D > K
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthDeadCross:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][9] > monthData[mIdx - 1][8]) // D > K
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case PriceLowerThanAverage5:
                    if (data[0] < data[5]) // Close < Average5
                        check = true;
                    else 
                        check = false;
                    break;
                case PriceLowerThanAverage10:
                    if (data[0] < data[6]) // Close < Average10
                        check = true;
                    else 
                        check = false;
                    break;
                case PriceLowerThanAverage20:
                    if (data[0] < data[7]) // Close < Average20
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekPriceLowerThanAverage5:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] < weekData[wIdx - 1][5]) // Close < Average5
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case WeekPriceLowerThanAverage10:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] < weekData[wIdx - 1][6]) // Close < Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case WeekPriceLowerThanAverage20:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] < weekData[wIdx - 1][7]) // Close < Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLowerThanAverage5:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] < monthData[mIdx - 1][5]) // Close < Average5
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLowerThanAverage10:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] < monthData[mIdx - 1][6]) // Close < Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLowerThanAverage20:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] < monthData[mIdx - 1][7]) // Close < Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                default :
                    cout << "Cannot recognize the condition";
                    check = false;
            }
            if (!check) break;
        }
        result |= check;
    }
    return result;
}

bool StockSimulator::checkSellCondition(vector<float> data, int idx, vector< vector<float> > weekData, vector< vector<float> > monthData) {
    bool result = false;
    int wIdx = idx / 5; // which week?
    int mIdx = idx / 22;
    for(unsigned int i = 0; i < sellCondition_.size(); ++i) { // for every OR condition 
        bool check = false;
        //if(sellCondition_.size() > 1 || sellCondition_[i].size() > 1) cout << "?" << endl;
        // close, high, low, open, vol, avaerage5, average10, average20, K9, D9
        for (unsigned int j = 0; j < sellCondition_[i].size(); ++j) { // for every AND condition
            switch(sellCondition_[i][j]) {
                case GoldenCross:
                    if (data[8] > data[9]) // K > D
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekGoldenCross:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][8] > weekData[wIdx - 1][9]) // K > D
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthGoldenCross:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][8] > monthData[mIdx - 1][9]) // K > D
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case PriceLargerThanAverage5:
                    if (data[0] > data[5]) // Close > Average5
                        check = true;
                    else 
                        check = false; 
                    break;
                case PriceLargerThanAverage10:
                    if (data[0] > data[6]) // Close > Average10
                        check = true;
                    else 
                        check = false;
                    break;
                case PriceLargerThanAverage20:
                    if (data[0] > data[7]) // Close > Average20
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekPriceLargerThanAverage5:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] > weekData[wIdx - 1][5]) // Close > Average5
                            check = true;
                        else 
                            check = false; 
                    }
                    break;
                case WeekPriceLargerThanAverage10:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] > weekData[wIdx - 1][6]) // Close > Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case WeekPriceLargerThanAverage20:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] > weekData[wIdx - 1][7]) // Close > Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLargerThanAverage5:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] > monthData[mIdx - 1][5]) // Close > Average5
                            check = true;
                        else 
                            check = false; 
                    }
                    break;
                case MonthPriceLargerThanAverage10:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] > monthData[mIdx - 1][6]) // Close > Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLargerThanAverage20:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] > monthData[mIdx - 1][7]) // Close > Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case DeadCross:
                    if (data[9] > data[8]) // D > K
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekDeadCross:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][9] > weekData[wIdx - 1][8]) // D > K
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthDeadCross:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][9] > monthData[mIdx - 1][8]) // D > K
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case PriceLowerThanAverage5:
                    if (data[0] < data[5]) // Close < Average5
                        check = true;
                    else 
                        check = false;
                    break;
                case PriceLowerThanAverage10:
                    if (data[0] < data[6]) // Close < Average10
                        check = true;
                    else 
                        check = false;
                    break;
                case PriceLowerThanAverage20:
                    if (data[0] < data[7]) // Close < Average20
                        check = true;
                    else 
                        check = false;
                    break;
                case WeekPriceLowerThanAverage5:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] < weekData[wIdx - 1][5]) // Close < Average5
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case WeekPriceLowerThanAverage10:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] < weekData[wIdx - 1][6]) // Close < Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case WeekPriceLowerThanAverage20:
                    if (wIdx == 0) check = false;
                    else {
                        if (weekData[wIdx - 1][0] < weekData[wIdx - 1][7]) // Close < Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLowerThanAverage5:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] < monthData[mIdx - 1][5]) // Close < Average5
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLowerThanAverage10:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] < monthData[mIdx - 1][6]) // Close < Average10
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                case MonthPriceLowerThanAverage20:
                    if (mIdx == 0) check = false;
                    else {
                        if (monthData[mIdx - 1][0] < monthData[mIdx - 1][7]) // Close < Average20
                            check = true;
                        else 
                            check = false;
                    }
                    break;
                default :
                    cout << "Cannot recognize the condition";
                    check = false;
            }
            if (!check) break;
        }
        result |= check;
    }
    return result;
}

void StockSimulator::run() {
    if (stockID_ == 1) { // all stocks : use 0050 instead
        vector < vector<float> > data = stockMap_[50];
        vector < vector<float> > weekData = weekStockMap_[50];
        vector < vector<float> > monthData = monthStockMap_[50];
        hold_ = false; // true if we buy it
        gain_.clear();
        //gain_.push_back(0);
        for (unsigned int i = 0; i < data.size(); ++i) { // every day
            vector<float> dailyData = data[i];
            //cout << i << endl;
            if (!hold_) { // check buy condition
                bool check = checkBuyCondition(dailyData, i, weekData, monthData);
                if (check) {
                    //cout << "buy at " << i << " at price " << dailyData[0] << endl;
                    hold_ = true;
                    buyPrice_ = dailyData[0];
                    if (gain_.size() == 0) 
                        gain_.push_back(1);
                    else
                        gain_.push_back(gain_[gain_.size() - 1]);
                    //cout << "current gain : " << gain_[gain_.size() - 1] << endl;
                    
                } else {
                    if (gain_.size() == 0) 
                        gain_.push_back(1);
                    else
                        gain_.push_back(gain_[gain_.size() - 1]);
                }
                lastGain_ = gain_[gain_.size() - 1];
            }
            else { // check sell condition
                bool check = checkSellCondition(dailyData, i, weekData, monthData);
                if (check) {
                    //cout << "sell at " << i << "at price " << dailyData[0] << endl;
                    //cout << "gain " << lastGain_ * dailyData[0] / buyPrice_ << endl;
                    hold_ = false;
                }
                gain_.push_back(lastGain_ * dailyData[0] / buyPrice_);
            }
            //cout << i << " d" << endl;
        }
        
    }  
    else if (stockID_ == 2) {/*TODO*/} // 10%
    else { // single stock
        vector < vector<float> > data = stockMap_[stockID_];
        vector < vector<float> > weekData = weekStockMap_[stockID_];
        vector < vector<float> > monthData = monthStockMap_[stockID_];
        hold_ = false; // true if we buy it
        gain_.clear();
        //gain_.push_back(0);
        for (unsigned int i = 0; i < data.size(); ++i) { // every day
            vector<float> dailyData = data[i];
            //cout << i << endl;
            if (!hold_) { // check buy condition
                bool check = checkBuyCondition(dailyData, i, weekData, monthData);
                if (check) {
                    //cout << "buy at " << i << " at price " << dailyData[0] << endl;
                    hold_ = true;
                    buyPrice_ = dailyData[0];
                    if (gain_.size() == 0) 
                        gain_.push_back(1);
                    else
                        gain_.push_back(gain_[gain_.size() - 1]);
                    //cout << "current gain : " << gain_[gain_.size() - 1] << endl;
                    
                } else {
                    if (gain_.size() == 0) 
                        gain_.push_back(1);
                    else
                        gain_.push_back(gain_[gain_.size() - 1]);
                }
                lastGain_ = gain_[gain_.size() - 1];
            }
            else { // check sell condition
                bool check = checkSellCondition(dailyData, i, weekData, monthData);
                if (check) {
                    //cout << "sell at " << i << "at price " << dailyData[0] << endl;
                    //cout << "gain " << lastGain_ * dailyData[0] / buyPrice_ << endl;
                    hold_ = false;
                }
                gain_.push_back(lastGain_ * dailyData[0] / buyPrice_);
            }
            //cout << i << " d" << endl;
        }
    }
}

void StockSimulator::setMap(map < int, vector < vector<float> > > m) {
   stockMap_ = m; 
}
void StockSimulator::setWeekMap(map < int, vector < vector<float> > > m) {
   weekStockMap_ = m; 
}
void StockSimulator::setMonthMap(map < int, vector < vector<float> > > m) {
   monthStockMap_ = m; 
}
void StockSimulator::set(string command) {
    //cout << command << endl; // for debugging
    istringstream ss(command);
    string token;
    
    // get stockID
    getline(ss, token, '_');
    stockID_ = atoi(token.c_str());
    
    // parse buy command
    getline(ss, token, '_');
    buyCommand_ = token;
    istringstream bc(token);
    string andItem;
    while(getline(bc, andItem, '+')) {
        vector<int> andCondition;
        istringstream ac(andItem);
        string aitem;
        while(getline(ac, aitem, '^')) {
            andCondition.push_back(atoi(aitem.c_str()));
        }
        buyCondition_.push_back(andCondition);
    }

    //cout << endl;

    // parse sell command
    getline(ss, token, '_');
    sellCommand_ = token.c_str();
    istringstream sc(token);
    while(getline(sc, andItem, '+')) {
        vector<int> andCondition;
        istringstream ac(andItem);
        string aitem;
        while(getline(ac, aitem, '^')) {
            andCondition.push_back(atoi(aitem.c_str()));
        }
        sellCondition_.push_back(andCondition);
    }
    //cout << endl;
    printInfo();
}

void StockSimulator::printMap() {
    vector< vector<float> > temp = stockMap_[50];
    for (unsigned int i = 0; i < temp.size(); ++i) {
        for (unsigned int j = 0; j < temp[i].size(); ++j)
            cout << temp[i][j] << ",";
        cout << endl;
    }
}
void StockSimulator::printGain() {
    fstream fout;
    fout.open("Cpackage/profit.rpt",ios::out);
    fout << gain_[0];
    for (unsigned int i = 1; i < gain_.size(); ++i) {
        fout << ", " << gain_[i];
    }
     int fd;
     char * myfifo = "./Cpackage/myfifo2";
     //cout << result << endl;
     /* create the FIFO (named pipe) */
     mkfifo(myfifo, 0666);
     /* write to the FIFO */
     fd = open(myfifo, O_WRONLY);
     write(fd, "done", sizeof("done"));
     close(fd);
     fout.close();
}
void StockSimulator::printInfo() {
   cout << "Stock ID     : " << stockID_ << endl; 
   cout << "Buy Conditions : ";
   for (unsigned int i = 0; i < buyCondition_.size(); ++i) {
       for (unsigned int j = 0; j < buyCondition_[i].size(); ++j) {
           switch(buyCondition_[i][j]) {
               case GoldenCross:
                   cout << "GoldenCross";
                   break;
               case WeekGoldenCross:
                   cout << "WeekGoldenCross";
                   break;
               case MonthGoldenCross:
                   cout << "MonthGoldenCross";
                   break;
               case PriceLargerThanAverage5:
                   cout << "PriceLagerThanAverage5";
                   break;
               case PriceLargerThanAverage10:
                   cout << "PriceLagerThanAverage10";
                   break;
               case PriceLargerThanAverage20:
                   cout << "PriceLagerThanAverage20";
                   break;
               case WeekPriceLargerThanAverage5:
                   cout << "WeekPriceLagerThanAverage5";
                   break;
               case WeekPriceLargerThanAverage10:
                   cout << "WeekPriceLagerThanAverage10";
                   break;
               case WeekPriceLargerThanAverage20:
                   cout << "WeekPriceLagerThanAverage20";
                   break;
               case MonthPriceLargerThanAverage5:
                   cout << "MonthPriceLagerThanAverage5";
                   break;
               case MonthPriceLargerThanAverage10:
                   cout << "MonthPriceLagerThanAverage10";
                   break;
               case MonthPriceLargerThanAverage20:
                   cout << "MonthPriceLagerThanAverage20";
                   break;
               case DeadCross:
                   cout << "DeadCross";
                   break;
               case WeekDeadCross:
                   cout << "WeekDeadCross";
                   break;
               case MonthDeadCross:
                   cout << "MonthDeadCross";
                   break;
               case PriceLowerThanAverage5:
                   cout << "PriceLowerThanAverage5";
                   break;
               case PriceLowerThanAverage10:
                   cout << "PriceLowerThanAverage10";
                   break;
               case PriceLowerThanAverage20:
                   cout << "PriceLowerThanAverage20";
                   break;
               case WeekPriceLowerThanAverage5:
                   cout << "WeekPriceLowerThanAverage5";
                   break;
               case WeekPriceLowerThanAverage10:
                   cout << "WeekPriceLowerThanAverage10";
                   break;
               case WeekPriceLowerThanAverage20:
                   cout << "WeekPriceLowerThanAverage20";
                   break;
               case MonthPriceLowerThanAverage5:
                   cout << "MonthPriceLowerThanAverage5";
                   break;
               case MonthPriceLowerThanAverage10:
                   cout << "MonthPriceLowerThanAverage10";
                   break;
               case MonthPriceLowerThanAverage20:
                   cout << "MonthPriceLowerThanAverage20";
                   break;
               default :
                   cout << "Cannot recognize the condition";
           }
           if (j != buyCondition_[i].size() - 1) cout << " and ";
       }
       if ( i != buyCondition_.size() - 1) cout << " or ";

   }
   cout << endl;
   cout << "Sell Conditions : ";
   for (unsigned int i = 0; i < sellCondition_.size(); ++i) {
       for (unsigned int j = 0; j < sellCondition_[i].size(); ++j) {
           switch(sellCondition_[i][j]) {
               case GoldenCross:
                   cout << "GoldenCross";
                   break;
               case WeekGoldenCross:
                   cout << "WeekGoldenCross";
                   break;
               case MonthGoldenCross:
                   cout << "MonthGoldenCross";
                   break;
               case PriceLargerThanAverage5:
                   cout << "PriceLagerThanAverage5";
                   break;
               case PriceLargerThanAverage10:
                   cout << "PriceLagerThanAverage10";
                   break;
               case PriceLargerThanAverage20:
                   cout << "PriceLagerThanAverage20";
                   break;
               case WeekPriceLargerThanAverage5:
                   cout << "WeekPriceLagerThanAverage5";
                   break;
               case WeekPriceLargerThanAverage10:
                   cout << "WeekPriceLagerThanAverage10";
                   break;
               case WeekPriceLargerThanAverage20:
                   cout << "WeekPriceLagerThanAverage20";
                   break;
               case MonthPriceLargerThanAverage5:
                   cout << "MonthPriceLagerThanAverage5";
                   break;
               case MonthPriceLargerThanAverage10:
                   cout << "MonthPriceLagerThanAverage10";
                   break;
               case MonthPriceLargerThanAverage20:
                   cout << "MonthPriceLagerThanAverage20";
                   break;
               case DeadCross:
                   cout << "DeadCross";
                   break;
               case WeekDeadCross:
                   cout << "WeekDeadCross";
                   break;
               case MonthDeadCross:
                   cout << "MonthDeadCross";
                   break;
               case PriceLowerThanAverage5:
                   cout << "PriceLowerThanAverage5";
                   break;
               case PriceLowerThanAverage10:
                   cout << "PriceLowerThanAverage10";
                   break;
               case PriceLowerThanAverage20:
                   cout << "PriceLowerThanAverage20";
                   break;
               case WeekPriceLowerThanAverage5:
                   cout << "WeekPriceLowerThanAverage5";
                   break;
               case WeekPriceLowerThanAverage10:
                   cout << "WeekPriceLowerThanAverage10";
                   break;
               case WeekPriceLowerThanAverage20:
                   cout << "WeekPriceLowerThanAverage20";
                   break;
               case MonthPriceLowerThanAverage5:
                   cout << "MonthPriceLowerThanAverage5";
                   break;
               case MonthPriceLowerThanAverage10:
                   cout << "MonthPriceLowerThanAverage10";
                   break;
               case MonthPriceLowerThanAverage20:
                   cout << "MonthPriceLowerThanAverage20";
                   break;
               default :
                   cout << "Cannot recognize the condition : " << sellCondition_[i][j];
           }
           if (j != sellCondition_[i].size() - 1) cout << " and ";
       }
       if ( i != sellCondition_.size() - 1) cout << " or ";

   }
   cout << endl;
}

