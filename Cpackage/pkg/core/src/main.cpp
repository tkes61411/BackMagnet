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
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF 1024

using namespace std;

int main(int argc, char* argv[]) {
    
    /// Parse data ///
    fstream fstockList, fstock;
    map< int, vector< vector<float> > > stockMap;
    map< int, vector< vector<float> > > weekStockMap;
    map< int, vector< vector<float> > > monthStockMap;
    clock_t start, finish;
    double duration;
    string fname, line;
    //StockSimulator sim;
    start = clock();
    cout << "Initialize ... " << endl;
    system("ls Cpackage/stocks > Cpackage/stock.list");
    fstockList.open("./Cpackage/stock.list", ios::in);
    while (fstockList >> fname) { // for every stock
        istringstream ss(fname);
        string token;
        float sum5, sum10, sum20;
        float average5, average10, average20;
        float rsv, todayClose, lowest9, highest9, K9, D9;
        
        float weekSum5, weekSum10, weekSum20;
        float weekAverage5, weekAverage10, weekAverage20;
        float wRsv, thisWeekClose, wLowest9, wHighest9, wK9, wD9;
        

        float monthSum5, monthSum10, monthSum20;
        float monthAverage5, monthAverage10, monthAverage20;
        float mRsv, thisMonthClose, mLowest9, mHighest9, mK9, mD9;

        sum5 = sum10 = sum20 = 0;
        average5 = average10 = average20 = 0;
        weekSum5 = weekSum10 = weekSum20 = 0;
        weekAverage5 = weekAverage10 = weekAverage20 = 0;
        monthSum5 = monthSum10 = monthSum20 = 0;
        monthAverage5 = monthAverage10 = monthAverage20 = 0;

        getline(ss, token, '.');
        istringstream ss2(token);
        string stockID;
        getline(ss2, stockID, 's');
        getline(ss2, stockID, 's'); // stockID
        fname = "Cpackage/stocks/" + fname; // where the file is
    
        fstock.open(fname.c_str(), ios::in);  
        fstock >> line; //skip first line
        vector< vector<float> > data;
        vector< vector<float> > weekData;
        vector< vector<float> > monthData;
        
        int weekCount = 0;
        int monthCount = 0;
        float weekClose, weekHigh, weekLow, weekOpen;
        float weekVol = 0;
        weekClose = weekHigh = weekLow = weekOpen = 0;
        float monthClose, monthHigh, monthLow, monthOpen;
        float monthVol = 0;
        monthClose = monthHigh = monthLow = monthOpen = 0;

        while (fstock >> line) { // for every day
            
            weekCount++;
            monthCount++;

            istringstream ss3(line);
            string item;
            vector<float> dailyData;
            vector<float> thisWeekData;
            vector<float> thisMonthData;

            getline(ss3, item, ','); // skip date
            while (getline(ss3, item, ',')) { // close, high, low, open, vol, avaerage5, average10, average20, K9, D9
                dailyData.push_back(atof(item.c_str()));
            }

            if (weekCount == 1) {
                weekOpen = dailyData[3];
                weekHigh = dailyData[1];
                weekLow = dailyData[2];
                weekVol = dailyData[4];
            } else if ( weekCount < 5) {
                if (dailyData[1] > weekHigh) weekHigh = dailyData[1];
                if (dailyData[2] < weekLow) weekLow = dailyData[2];
                weekVol += dailyData[4];
            } else {
                weekClose = dailyData[0];
                if (dailyData[1] > weekHigh) weekHigh = dailyData[1];
                if (dailyData[2] < weekLow) weekLow = dailyData[2];
                weekVol += dailyData[4];
                thisWeekData.push_back(weekClose);
                thisWeekData.push_back(weekHigh);
                thisWeekData.push_back(weekLow);
                thisWeekData.push_back(weekOpen);
                thisWeekData.push_back(weekVol);
                weekCount = 0;



                // calculate average 5
                weekSum5 += thisWeekData[0];
                if (weekData.size() >= 5) {
                    weekSum5 = weekSum5 - weekData[weekData.size() - 5][0];
                }
                weekAverage5 = weekSum5 / 5; 
                thisWeekData.push_back(weekAverage5);
                // calculate average 10
                weekSum10 += thisWeekData[0];
                if (weekData.size() >= 10) {
                    weekSum10 = weekSum10 - weekData[weekData.size() - 10][0];
                }
                weekAverage10 = weekSum10 / 10; 
                thisWeekData.push_back(weekAverage10);
                // calculate average 20
                weekSum20 += thisWeekData[0];
                if (weekData.size() >= 20) {
                    weekSum20 = weekSum20 - weekData[weekData.size() - 20][0];
                }
                weekAverage20 = weekSum20 / 20; 
                thisWeekData.push_back(weekAverage20);
                // K9 D9
                thisWeekClose = thisWeekData[0];
                wHighest9 = thisWeekData[1];
                wLowest9 = thisWeekData[2];
                for (int i = 0; i < 8; ++i) {
                    if (weekData.size() - i <= 0) break;
                    if (weekData[weekData.size() - 1 - i][1] > wHighest9) wHighest9 = weekData[weekData.size() - 1 - i][1];
                    if (weekData[weekData.size() - 1 - i][2] < wLowest9) wLowest9 = weekData[weekData.size() - 1 - i][2];
                }
                wRsv = 100 * (thisWeekClose - wLowest9) / (wHighest9 - wLowest9); // calculate rsv
                float wLastK9, wLastD9;
                if (weekData.size() == 0) wLastK9 = wLastD9 = 50;
                else {
                    wLastK9 = weekData[weekData.size() - 1][8];
                    wLastD9 = weekData[weekData.size() - 1][9];
                }
                wK9 = wLastK9 * 2 / 3 + wRsv * 1 / 3; // wK9
                wD9 = wLastD9 * 2 / 3 + wK9 * 1 / 3;  // wD9
                thisWeekData.push_back(wK9);
                thisWeekData.push_back(wD9);
                weekData.push_back(thisWeekData);
            }



            if (monthCount == 1) {
                monthOpen = dailyData[3];
                monthHigh = dailyData[1];
                monthLow = dailyData[2];
                monthVol = dailyData[4];
            } else if ( monthCount < 22) {
                if (dailyData[1] > monthHigh) monthHigh = dailyData[1];
                if (dailyData[2] < monthLow) monthLow = dailyData[2];
                monthVol += dailyData[4];
            } else {
                monthClose = dailyData[0];
                if (dailyData[1] > monthHigh) monthHigh = dailyData[1];
                if (dailyData[2] < monthLow) monthLow = dailyData[2];
                monthVol += dailyData[4];
                thisMonthData.push_back(monthClose);
                thisMonthData.push_back(monthHigh);
                thisMonthData.push_back(monthLow);
                thisMonthData.push_back(monthOpen);
                thisMonthData.push_back(monthVol);
                monthCount = 0;
                // calculate average 5
                monthSum5 += thisMonthData[0];
                if (monthData.size() >= 5) {
                    monthSum5 = monthSum5 - monthData[monthData.size() - 5][0];
                }
                monthAverage5 = monthSum5 / 5; 
                thisMonthData.push_back(monthAverage5);
                // calculate average 10
                monthSum10 += thisMonthData[0];
                if (monthData.size() >= 10) {
                    monthSum10 = monthSum10 - monthData[monthData.size() - 10][0];
                }
                monthAverage10 = monthSum10 / 10; 
                thisMonthData.push_back(monthAverage10);
                // calculate average 20
                monthSum20 += thisMonthData[0];
                if (monthData.size() >= 20) {
                    monthSum20 = monthSum20 - monthData[monthData.size() - 20][0];
                }
                monthAverage20 = monthSum20 / 20; 
                thisMonthData.push_back(monthAverage20);
                // K9 D9
                thisMonthClose = thisMonthData[0];
                mHighest9 = thisMonthData[1];
                mLowest9 = thisMonthData[2];
                for (int i = 0; i < 8; ++i) {
                    if (monthData.size() - i <= 0) break;
                    if (monthData[monthData.size() - 1 - i][1] > mHighest9) mHighest9 = monthData[monthData.size() - 1 - i][1];
                    if (monthData[monthData.size() - 1 - i][2] < mLowest9) mLowest9 = monthData[monthData.size() - 1 - i][2];
                }
                mRsv = 100 * (thisMonthClose - mLowest9) / (mHighest9 - mLowest9); // calculate rsv
                float mLastK9, mLastD9;
                if (monthData.size() == 0) mLastK9 = mLastD9 = 50;
                else {
                    mLastK9 = monthData[monthData.size() - 1][8];
                    mLastD9 = monthData[monthData.size() - 1][9];
                }
                mK9 = mLastK9 * 2 / 3 + mRsv * 1 / 3; // wK9
                mD9 = mLastD9 * 2 / 3 + mK9 * 1 / 3;  // wD9
                thisMonthData.push_back(mK9);
                thisMonthData.push_back(mD9);
                monthData.push_back(thisMonthData);
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
                if (data[data.size() - 1 - i][2] < lowest9) lowest9 = data[data.size() - 1 - i][2];
            }
            rsv = 100 * (todayClose - lowest9) / (highest9 - lowest9); // calculate rsv
            float lastK9, lastD9;
            if (data.size() == 0) lastK9 = lastD9 = 50;
            else {
                lastK9 = data[data.size() - 1][8];
                lastD9 = data[data.size() - 1][9];
            }
            K9 = lastK9 * 2 / 3 + rsv * 1 / 3; // K9
            D9 = lastD9 * 2 / 3 + K9 * 1 / 3;  // D9
            dailyData.push_back(K9);
            dailyData.push_back(D9);

            /*for(unsigned int i = 0; i < dailyData.size(); ++i) {
                cout << dailyData[i] << ",";
            }
            cout << endl;*/
            // push back new data
            data.push_back(dailyData);
        }
        stockMap[atoi(stockID.c_str())] = data;
        weekStockMap[atoi(stockID.c_str())] = weekData;
        monthStockMap[atoi(stockID.c_str())] = monthData;
        fstock.close();
    }
    fstockList.close();
   
    int fd;
    char * myfifo = "./Cpackage/myfifo";
    char buf[MAX_BUF];
    fd = open(myfifo, O_RDONLY);
    cout << "Start ... " << endl;
    while (1) {
        while(read(fd, buf, MAX_BUF) != 0) {
            StockSimulator sim;
            cout << "Setting ... " << endl;
            sim.setMap(stockMap);
            sim.setWeekMap(weekStockMap);
            sim.setMonthMap(monthStockMap);
            //sim.printMap();
            sim.set(buf);
            if (sim.stockMap_.find(sim.stockID_) == sim.stockMap_.end()) {
                int fd;
                char * myfifo = "./Cpackage/myfifo2";
                //cout << result << endl;
                /* create the FIFO (named pipe) */
                mkfifo(myfifo, 0666);
                /* write to the FIFO */
                fd = open(myfifo, O_WRONLY);
                write(fd, "noID", sizeof("noID"));
                close(fd);
            } else {
                cout << "Simulation ... ";
                sim.run();
                cout << "Finish" << endl;

                cout << "Writing ... ";
                sim.printGain();
                cout << "Finish" << endl;
                finish = clock();
                duration = double(finish - start) / CLOCKS_PER_SEC;
                cout <<"Run time : " <<  duration << " sec " << endl;
            }
        }
    }
    close(fd);
    return 0;
}
