#include "simulator.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

StockSimulator::StockSimulator() {
    hold = false;
    gain.clear();
}

void StockSimulator::set(string command) {
    cout << command << endl; // for debugging
    istringstream ss(command);
    string token;
    getline(ss, token, '_');
    stockID = atoi(token.c_str());
    printInfo();
}

void StockSimulator::printInfo() {
   cout << "Stock ID : " << stockID << endl; 
}

