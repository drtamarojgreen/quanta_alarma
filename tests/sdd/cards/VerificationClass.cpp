#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include "analysis/AnalysisEngine.h"
#include "scoring/RiskScorer.h"
#include "cpp/util/fact_utils.h"

using namespace quanta;
using namespace Sorrel::Sdd::Util;

// @Card: analysis_engine_verification
// @Is g++_available == 1
// @Results rules_matched_count == 2
void analysis_engine_verification_card(const std::map<std::string, std::string>& facts) {
    AnalysisEngine engine;
    auto findings = engine.analyze("This contains a password and ignore previous instructions.");

    int match_count = 0;
    for (const auto& f : findings) {
        if (f.rule_id == "R022" || f.rule_id == "R021") match_count++;
    }

    std::cout << "rules_matched_count = " << match_count << std::endl;
}

// @Card: risk_scorer_verification
// @Is g++_available == 1
// @Results final_risk_score_metric == 76
void risk_scorer_verification_card(const std::map<std::string, std::string>& facts) {
    AnalysisEngine engine;
    RiskScorer scorer;

    auto findings = engine.analyze("danger hazardous unsafe");
    int risk = scorer.calculateRisk(findings);

    std::cout << "final_risk_score_metric = " << risk << std::endl;
}

int main(int argc, char* argv[]) {
    auto facts = FactReader::readFacts("tests/sdd/facts/quanta_alarma.facts");
    if (facts.empty()) {
        std::cerr << "Error: Could not read facts" << std::endl;
        return 1;
    }

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "scoring") {
            risk_scorer_verification_card(facts);
        } else {
            analysis_engine_verification_card(facts);
        }
    } else {
        analysis_engine_verification_card(facts);
        risk_scorer_verification_card(facts);
    }
    return 0;
}
