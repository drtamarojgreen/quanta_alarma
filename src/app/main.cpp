#include "analysis/AnalysisEngine.h"
#include "scoring/RiskScorer.h"
#include "alerting/AlertingSystem.h"
#include "app/ConfigManager.h"
#include "app/Reporter.h"
#include <iostream>
#include <vector>
#include <string>

using namespace quanta;

void printUsage() {
    std::cout << "Usage: quanta_alarma [--config <path>] [--format json|csv|md|text] <input_text>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string input;
    std::string config_path;
    std::string format = "text";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--config" && i + 1 < argc) {
            config_path = argv[++i];
        } else if (arg == "--format" && i + 1 < argc) {
            format = argv[++i];
        } else {
            input = arg;
        }
    }

    AnalysisEngine engine;
    RiskScorer scorer;
    AlertingSystem alerter;
    ConfigManager configManager;

    if (!config_path.empty()) {
        configManager.loadFromFile(config_path);
        configManager.applyTo(engine, scorer);
    }

    auto findings = engine.analyze(input);
    int riskScore = scorer.calculateRisk(findings);

    if (format == "json") std::cout << Reporter::toJSON(findings) << std::endl;
    else if (format == "csv") std::cout << Reporter::toCSV(findings) << std::endl;
    else if (format == "md") std::cout << Reporter::toMarkdown(findings) << std::endl;
    else std::cout << Reporter::toPlainText(findings) << std::endl;

    std::cout << "Final Risk Score: " << riskScore << std::endl;
    alerter.triggerAlert(riskScore);

    return (riskScore >= configManager.getConfig().failure_threshold) ? 1 : 0;
}
