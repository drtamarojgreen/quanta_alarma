#ifndef RISK_SCORER_H
#define RISK_SCORER_H

#include <string>

class RiskScorer {
public:
    RiskScorer();
    int calculateRisk(const std::string& text);
};

#endif // RISK_SCORER_H
