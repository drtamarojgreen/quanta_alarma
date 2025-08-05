#include "RiskScorer.h"

RiskScorer::RiskScorer() {}

int RiskScorer::calculateRisk(const std::string& text) {
    // Stub implementation
    if (text.find("danger") != std::string::npos) {
        return 100;
    }
    return 0;
}
