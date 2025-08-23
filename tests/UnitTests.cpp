#include <iostream>
#include <cassert>
#include <sstream>
#include <streambuf>

// Include the headers of the classes to be tested
#include "../include/RiskScorer.h"
#include "../include/AlertingSystem.h"

// Forward declarations for test functions
void testRiskScorer();
void testAlertingSystem();

int main() {
    std::cout << "Running Unit Tests..." << std::endl;

    testRiskScorer();
    testAlertingSystem();

    std::cout << "All Unit Tests Passed!" << std::endl;

    return 0;
}

// Implementations for test functions will be added in the next steps.
void testRiskScorer() {
    std::cout << "  Testing RiskScorer..." << std::endl;
    RiskScorer scorer;

    // Test case 1: Text contains "danger"
    std::cout << "    - Test case: Text contains 'danger'..." << std::endl;
    int risk1 = scorer.calculateRisk("this is a dangerous message");
    assert(risk1 == 100);
    std::cout << "      PASS" << std::endl;

    // Test case 2: Text does not contain "danger"
    std::cout << "    - Test case: Text does not contain 'danger'..." << std::endl;
    int risk2 = scorer.calculateRisk("this is a safe message");
    assert(risk2 == 0);
    std::cout << "      PASS" << std::endl;

    // Test case 3: Empty string
    std::cout << "    - Test case: Empty string..." << std::endl;
    int risk3 = scorer.calculateRisk("");
    assert(risk3 == 0);
    std::cout << "      PASS" << std::endl;

    // Test case 4: "danger" at the beginning
    std::cout << "    - Test case: 'danger' at the beginning..." << std::endl;
    int risk4 = scorer.calculateRisk("danger ahead");
    assert(risk4 == 100);
    std::cout << "      PASS" << std::endl;

    // Test case 5: "danger" at the end
    std::cout << "    - Test case: 'danger' at the end..." << std::endl;
    int risk5 = scorer.calculateRisk("be aware of danger");
    assert(risk5 == 100);
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

    // Test case 2: Low risk score should not trigger an alert
    std::cout << "    - Test case: Low risk score..." << std::endl;
    strCout.str(""); // Clear the stream
    strCout.clear();

    std::cout.rdbuf(strCout.rdbuf()); // Redirect cout
    alerter.triggerAlert(50);
    std::cout.rdbuf(oldCoutStreamBuf); // Restore cout

    output = strCout.str();
    assert(output.empty());
    std::cout << "      PASS" << std::endl;

    // Test case 3: Boundary high risk score
    std::cout << "    - Test case: Boundary high risk score..." << std::endl;
    strCout.str("");
    strCout.clear();

    std::cout.rdbuf(strCout.rdbuf());
    alerter.triggerAlert(51);
    std::cout.rdbuf(oldCoutStreamBuf);

    output = strCout.str();
    assert(output.find("ALERT!") != std::string::npos);
    assert(output.find("51") != std::string::npos);
    std::cout << "      PASS" << std::endl;
}
