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

bool StockSimulator::checkBuyCondition(vector<float> data) {
    bool result = false;
    for(unsigned int i = 0; i < buyCondition_.size(); ++i) { // for every OR condition 
        bool check = false;
        // close, high, low, open, vol, avaerage5, average10, average20, K9, D9
        for (unsigned int j = 0; j < buyCondition_[i].size(); ++j) { // for every AND condition
            switch(buyCondition_[i][j]) {
                case GoldenCross:
                    if (data[8] > data[9]) // K > D
                        check = true;
                    else 
                        check = false;
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
                case DeadCross:
                    if (data[9] > data[8]) // D > K
                        check = true;
                    else 
                        check = false;
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

bool StockSimulator::checkSellCondition(vector<float> data) {
    bool result = false;
    for(unsigned int i = 0; i < sellCondition_.size(); ++i) { // for every OR condition 
        bool check = false;
        // close, high, low, open, vol, avaerage5, average10, average20, K9, D9
        for (unsigned int j = 0; j < sellCondition_[i].size(); ++j) { // for every AND condition
            switch(sellCondition_[i][j]) {
                case GoldenCross:
                    if (data[8] > data[9]) // K > D
                        check = true;
                    else 
                        check = false;
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
                case DeadCross:
                    if (data[9] > data[8]) // D > K
                        check = true;
                    else 
                        check = false;
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
    if (stockID_ == 1) {/*TODO*/} // all stock    
    else if (stockID_ == 2) {/*TODO*/} // 10%
    else { // single stock
        cout << "Single Stock Mode" << endl;
        vector < vector<float> > data = stockMap_[stockID_];
        hold_ = false; // true if we buy it
        gain_.clear();
        //gain_.push_back(0);
        for (unsigned int i = 0; i < data.size(); ++i) { // every day
            vector<float> dailyData = data[i];
            if (!hold_) { // check buy condition
                bool check = checkBuyCondition(dailyData);
                if (check) {
                    hold_ = true;
                    buyPrice_ = dailyData[0];
                    if (gain_.size() == 0) 
                        gain_.push_back(0);
                    else
                        gain_.push_back(gain_[gain_.size() - 1]);
                    
                } else {
                    if (gain_.size() == 0) 
                        gain_.push_back(0);
                    else
                        gain_.push_back(gain_[gain_.size() - 1]);
                }
                lastGain_ = gain_[gain_.size() - 1];
            }
            else if (hold_) { // check sell condition
                bool check = checkSellCondition(dailyData);
                if (check) 
                    hold_ = false;
                gain_.push_back(lastGain_ + (dailyData[0] - buyPrice_) / buyPrice_ * 100);
            }
        }
    }
}

void StockSimulator::setMap(map < int, vector < vector<float> > > m) {
   stockMap_ = m; 
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
        int conditions = atoi(andItem.c_str());
        while(conditions != 0) {
            int singleCondition = conditions % 10;
            conditions = conditions / 10;
            //cout << singleCondition << " ";
            andCondition.push_back(singleCondition);
        }
        //cout << endl;
        buyCondition_.push_back(andCondition);
    }

    //cout << endl;

    // parse sell command
    getline(ss, token, '_');
    sellCommand_ = token.c_str();
    istringstream sc(token);
    while(getline(sc, andItem, '+')) {
        vector<int> andCondition;
        int conditions = atoi(andItem.c_str());
        while(conditions != 0) {
            int singleCondition = conditions % 10;
            conditions = conditions / 10;
            cout << singleCondition << " ";
            andCondition.push_back(singleCondition);
        }
        cout << endl;
        sellCondition_.push_back(andCondition);
    }
    //cout << endl;
    printInfo();
}

void StockSimulator::printGain() {
    fstream fout;
    fout.open("profit.rpt",ios::out);
    fout << gain_[0];
    for (unsigned int i = 1; i < gain_.size(); ++i) {
        fout << ", " << gain_[i];
    }
     int fd;
     char * myfifo = "./myfifo2";
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
               case PriceLargerThanAverage5:
                   cout << "PriceLagerThanAverage5";
                   break;
               case PriceLargerThanAverage10:
                   cout << "PriceLagerThanAverage10";
                   break;
               case PriceLargerThanAverage20:
                   cout << "PriceLagerThanAverage20";
                   break;
               case DeadCross:
                   cout << "DeadCross";
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
               case PriceLargerThanAverage5:
                   cout << "PriceLagerThanAverage5";
                   break;
               case PriceLargerThanAverage10:
                   cout << "PriceLagerThanAverage10";
                   break;
               case PriceLargerThanAverage20:
                   cout << "PriceLagerThanAverage20";
                   break;
               case DeadCross:
                   cout << "DeadCross";
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
               default :
                   cout << "Cannot recognize the condition";
           }
           if (j != sellCondition_[i].size() - 1) cout << " and ";
       }
       if ( i != sellCondition_.size() - 1) cout << " or ";

   }
   cout << endl;
}

