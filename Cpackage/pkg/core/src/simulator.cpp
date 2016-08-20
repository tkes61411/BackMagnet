#include "simulator.h"

StockSimulator::StockSimulator() {
    buyConstraint1 = buyConstraint2 = buyConstraint3 = false;
    sellConstraint1 = sellConstraint2 = sellConstraint3 = false;
    hold = false;
    gain.clear();
}

void StockSimulator::run() {
    
}
