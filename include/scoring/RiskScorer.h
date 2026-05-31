#ifndef RISK_SCORER_H
#define RISK_SCORER_H

#include "analysis/AnalysisEngine.h"
#include <vector>

namespace quanta {

class RiskScorer {
public:
    RiskScorer();
    int calculateRisk(const std::vector<Finding>& findings);

    // Tuning
    void setSeverityWeight(const std::string& category, double weight);

private:
    std::map<std::string, double> category_weights;
};

} // namespace quanta

#endif // RISK_SCORER_H
