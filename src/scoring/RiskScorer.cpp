#include "scoring/RiskScorer.h"
#include <cmath>
#include <algorithm>

namespace quanta {

RiskScorer::RiskScorer() {}

int RiskScorer::calculateRisk(const std::vector<Finding>& findings) {
    if (findings.empty()) return 0;

    double total_risk = 0.0;
    int active_findings = 0;

    for (const auto& f : findings) {
        if (f.suppressed) continue;

        double weight = 1.0;
        if (category_weights.count(f.category)) {
            weight = category_weights.at(f.category);
        }

        // Basic score: severity * confidence * category_weight
        total_risk += f.severity * f.confidence * weight;
        active_findings++;
    }

    if (active_findings == 0) return 0;

    // Normalize to 0-100 range. Assuming max risk per finding is 10.
    // Use a non-linear scaling so multiple high-severity findings ramp up quickly.
    double score = (total_risk / (active_findings * 10.0)) * 100.0;

    return std::min(100, static_cast<int>(std::round(score)));
}

void RiskScorer::setSeverityWeight(const std::string& category, double weight) {
    category_weights[category] = weight;
}

} // namespace quanta
