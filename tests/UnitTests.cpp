#include <iostream>
#include <cassert>
#include <sstream>
#include <streambuf>

// Include the headers of the classes to be tested
#include "analysis/AnalysisEngine.h"
#include "scoring/RiskScorer.h"
#include "alerting/AlertingSystem.h"

using namespace quanta;

// Forward declarations for test functions
void testRiskScorer();
void testAlertingSystem();
void testAnalysisEngine();

int main() {
    std::cout << "Running Unit Tests..." << std::endl;

    testAnalysisEngine();
    testRiskScorer();
    testAlertingSystem();

    std::cout << "All Unit Tests Passed!" << std::endl;

    return 0;
}

void testAnalysisEngine() {
    std::cout << "  Testing AnalysisEngine..." << std::endl;
    AnalysisEngine engine;

    std::string text = "This is a dangerous message with a password and ignore previous instructions.";
    auto findings = engine.analyze(text);

    // Should find R020 (danger), R021 (ignore previous), R022 (password)
    assert(findings.size() >= 3);

    bool found_r021 = false;
    for (const auto& f : findings) {
        if (f.rule_id == "R021") found_r021 = true;
    }
    assert(found_r021);

    std::cout << "      PASS" << std::endl;
}

void testRiskScorer() {
    std::cout << "  Testing RiskScorer..." << std::endl;
    AnalysisEngine engine;
    RiskScorer scorer;

    // Test case 1: Text contains "danger"
    std::cout << "    - Test case: Text contains 'danger'..." << std::endl;
    auto findings1 = engine.analyze("this is a dangerous message");
    int risk1 = scorer.calculateRisk(findings1);
    assert(risk1 > 0);
    std::cout << "      PASS (risk score: " << risk1 << ")" << std::endl;

    // Test case 2: Text does not contain "danger"
    std::cout << "    - Test case: Text does not contain 'danger'..." << std::endl;
    auto findings2 = engine.analyze("this is a safe message");
    int risk2 = scorer.calculateRisk(findings2);
    assert(risk2 == 0);
    std::cout << "      PASS" << std::endl;
}

void testAlertingSystem() {
    std::cout << "  Testing AlertingSystem..." << std::endl;
    AlertingSystem alerter;

    // Backup original cout buffer
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strCout;

    // Test case 1: High risk score should trigger an alert
    std::cout << "    - Test case: High risk score..." << std::endl;
    std::cout.rdbuf(strCout.rdbuf()); // Redirect cout
    alerter.triggerAlert(100);
    std::cout.rdbuf(oldCoutStreamBuf); // Restore cout

    std::string output = strCout.str();
    assert(output.find("ALERT!") != std::string::npos);
    assert(output.find("100") != std::string::npos);
    std::cout << "      PASS" << std::endl;
}
