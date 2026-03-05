#include <cassert>
#include <iostream>

#include "RiskScorer.h"

int main() {
    std::cout << "Running TestRunner tests..." << std::endl;

    RiskScorer scorer;

    int riskWithDanger = scorer.calculateRisk("this is a dangerous message");
    assert(riskWithDanger == 100);

    int riskWithoutDanger = scorer.calculateRisk("this is a safe message");
    assert(riskWithoutDanger == 0);

    std::cout << "All TestRunner tests passed." << std::endl;
    return 0;
}
