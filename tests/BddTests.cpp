#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <functional>

// For redirecting cout
#include <sstream>
#include <streambuf>

// Include the headers of the classes to be tested
#include "../include/RiskScorer.h"
#include "../include/AlertingSystem.h"

// --- Simple BDD Framework ---

#define SCENARIO(description) std::cout << std::endl << "SCENARIO: " << description << std::endl;
#define GIVEN(description) std::cout << "  GIVEN: " << description << std::endl;
#define WHEN(description) std::cout << "  WHEN: " << description << std::endl;
#define THEN(description) std::cout << "  THEN: " << description << std::endl;
#define AND(description) std::cout << "  AND: " << description << std::endl;

// --- Test Scenarios ---

void BddRiskScorerScenario();
void BddAlertingSystemScenario();

int main() {
    std::cout << "Running BDD Tests..." << std::endl;

    BddRiskScorerScenario();
    BddAlertingSystemScenario();

    std::cout << "All BDD Scenarios Passed!" << std::endl;

    return 0;
}


// --- Scenario Implementations ---
void BddRiskScorerScenario() {
    SCENARIO("Risk scores are calculated based on text content");

    GIVEN("A RiskScorer");
    RiskScorer scorer;
    int risk;

    WHEN("the text contains the word 'danger'");
    risk = scorer.calculateRisk("this is a dangerous message");
    THEN("the risk score should be 100");
    assert(risk == 100);
    std::cout << "    -> PASSED" << std::endl;

    WHEN("the text does not contain the word 'danger'");
    risk = scorer.calculateRisk("this is a safe message");
    THEN("the risk score should be 0");
    assert(risk == 0);
    std::cout << "    -> PASSED" << std::endl;
}

void BddAlertingSystemScenario() {
    SCENARIO("Alerting system notifies on high risk scores");

    GIVEN("an AlertingSystem");
    AlertingSystem alerter;

    // Backup original cout buffer
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strCout;

    WHEN("a high risk score of 100 is received");
    std::cout.rdbuf(strCout.rdbuf()); // Redirect cout
    alerter.triggerAlert(100);
    std::cout.rdbuf(oldCoutStreamBuf); // Restore cout

    THEN("an alert containing 'ALERT!' is triggered");
    std::string output = strCout.str();
    assert(output.find("ALERT!") != std::string::npos);
    std::cout << "    -> PASSED" << std::endl;

    AND("the alert contains the score '100'");
    assert(output.find("100") != std::string::npos);
    std::cout << "    -> PASSED" << std::endl;

    // --- New WHEN/THEN block for low risk ---
    WHEN("a low risk score of 50 is received");
    strCout.str(""); // Clear stream
    strCout.clear();
    std::cout.rdbuf(strCout.rdbuf()); // Redirect cout
    alerter.triggerAlert(50);
    std::cout.rdbuf(oldCoutStreamBuf); // Restore cout

    THEN("no alert is triggered");
    output = strCout.str();
    assert(output.empty());
    std::cout << "    -> PASSED" << std::endl;
}
