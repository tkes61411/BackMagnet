// **************************************************************************
//   File       [main.cpp]
//   Author     [Yu-Hao Ho]
//   Synopsis   [The main program of 2016 hackNTU]
//   Created    [2016/8/20 Yu-Hao Ho]
// **************************************************************************

#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "simulator.h"

using namespace std;

int main(int argc, char* argv[]) {
    
    /// Parse data ///
    fstream fin, fout, fstockList, fstock;
    map< int, vector< vector<float> > > stockMap;
    clock_t start, finish;
    double duration;
    string fname, line;
    //StockSimulator sim;
    start = clock();
    cout << "Initialize ... ";
    system("ls stocks > stock.list");
    fstockList.open("./stock.list", ios::in);
    while (fstockList >> fname) { // for every stock
        istringstream ss(fname);
        string token;
        float sum5, sum10, sum20;
        float average5, average10, average20;
        float rsv, todayClose, lowest9, highest9, K9, D9;
        sum5 = sum10 = sum20 = 0;
        average5 = average10 = average20 = 0;
        getline(ss, token, '.');
        istringstream ss2(token);
        string stockID;
        getline(ss2, stockID, 's');
        getline(ss2, stockID, 's'); // stockID
        fname = "stocks/" + fname; // where the file is
    
        fstock.open(fname.c_str(), ios::in);  
        fstock >> line; //skip first line
        vector< vector<float> > data;
        while (fstock >> line) { // for every day
            istringstream ss3(line);
            string item;
            vector<float> dailyData;
            getline(ss3, item, ','); // skip date
            while (getline(ss3, item, ',')) { // close, high, low, open, vol, avaerage5, average10, average20, K9, D9
                dailyData.push_back(atof(item.c_str()));
            }
            // calculate average 5
            sum5 += dailyData[0];
            if (data.size() >= 5) {
                sum5 = sum5 - data[data.size() - 5][0];
            }
            average5 = sum5 / 5; 
            dailyData.push_back(average5);
            // calculate average 10
            sum10 += dailyData[0];
            if (data.size() >= 10) {
                sum10 = sum10 - data[data.size() - 10][0];
            }
            average10 = sum10 / 10; 
            dailyData.push_back(average10);
            // calculate average 20
            sum20 += dailyData[0];
            if (data.size() >= 20) {
                sum20 = sum20 - data[data.size() - 20][0];
            }
            average20 = sum20 / 20; 
            dailyData.push_back(average20);
            // K9 D9
            todayClose = dailyData[0];
            highest9 = dailyData[1];
            lowest9 = dailyData[2];
            for (int i = 0; i < 8; ++i) {
                if (data.size() - i <= 0) break;
                if (data[data.size() - 1 - i][1] > highest9) highest9 = data[data.size() - 1 - i][1];
                if (data[data.size() - 1 - i][2] > lowest9) lowest9 = data[data.size() - 1 - i][2];
            }
            rsv = (todayClose - lowest9) / (highest9 - lowest9) * 100; // calculate rsv
            float lastK9, lastD9;
            if (data.size() == 0) lastK9 = lastD9 = 0;
            else {
                lastK9 = data[data.size() - 1][8];
                lastD9 = data[data.size() - 1][9];
            }
            K9 = lastK9 * 2 / 3 + rsv * 1 / 3; // K9
            D9 = lastD9 * 2 / 3 + K9 * 1 / 3;  // D9
            dailyData.push_back(K9);
            dailyData.push_back(D9);

            // push back new data
            data.push_back(dailyData);
        }
        stockMap[atoi(stockID.c_str())] = data;
        fstock.close();
    }
    fstockList.close();
    cout << "Finish" << endl;
   
    StockSimulator sim;
    while (1) {
        cout << "Setting ... ";
        //sim->set(fin);
        // TODO
        cout << "Finish" << endl;

        cout << "Simulation ... ";
        sim.run();
        cout << "Finish" << endl;

        cout << "Writing ... ";
        // TODO
        cout << "Finish" << endl;
        finish = clock();
        duration = double(finish - start) / CLOCKS_PER_SEC;
        cout <<"Run time : " <<  duration << " sec " << endl;
    }
    return 0;
}
