#include <cassert>
#include <iostream>

#include "scoring/RiskScorer.h"
#include "analysis/AnalysisEngine.h"

using namespace quanta;

int main() {
    std::cout << "Running TestRunner tests..." << std::endl;

    AnalysisEngine engine;
    RiskScorer scorer;

    auto findingsWithDanger = engine.analyze("this is a dangerous message");
    int riskWithDanger = scorer.calculateRisk(findingsWithDanger);
    assert(riskWithDanger > 0);

    auto findingsWithoutDanger = engine.analyze("this is a safe message");
    int riskWithoutDanger = scorer.calculateRisk(findingsWithoutDanger);
    assert(riskWithoutDanger == 0);

    std::cout << "All TestRunner tests passed." << std::endl;
    return 0;
}
