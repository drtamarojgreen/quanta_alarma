#include "analysis/AnalysisEngine.h"
#include "scoring/RiskScorer.h"
#include "alerting/AlertingSystem.h"
#include "app/ConfigManager.h"
#include "app/Reporter.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace quanta;

const std::string VERSION = "1.1.0-alpha";

void printUsage() {
    std::cout << "QuantaAlarma CLI " << VERSION << "\n"
              << "Usage: quanta_alarma [options] <input_text>\n\n"
              << "Options:\n"
              << "  --config <path>         Path to policy configuration file\n"
              << "  --format json|csv|md    Output format (default: text)\n"
              << "  --list-rules            List all active rules and exit\n"
              << "  --version               Print version information and exit\n"
              << "  --quiet                 Reduce output to findings only\n"
              << "  --verbose               Show detailed analysis reasoning\n"
              << "  --dry-run               Validate configuration and exit\n"
              << "  --stdin                 Read input text from standard input\n"
              << "  --threshold <n>         Set failure threshold (0-100)\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string input;
    std::string config_path;
    std::string format = "text";
    bool read_stdin = false;
    bool list_rules = false;

    AnalysisEngine engine;
    RiskScorer scorer;
    AlertingSystem alerter;
    ConfigManager configManager;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--config" && i + 1 < argc) {
            config_path = argv[++i];
        } else if (arg == "--format" && i + 1 < argc) {
            format = argv[++i];
        } else if (arg == "--threshold" && i + 1 < argc) {
            configManager.mutableConfig().failure_threshold = std::stoi(argv[++i]);
        } else if (arg == "--list-rules") {
            list_rules = true;
        } else if (arg == "--version") {
            std::cout << "QuantaAlarma version " << VERSION << std::endl;
            return 0;
        } else if (arg == "--quiet") {
            configManager.mutableConfig().quiet_mode = true;
        } else if (arg == "--verbose") {
            configManager.mutableConfig().verbose_mode = true;
        } else if (arg == "--dry-run") {
            configManager.mutableConfig().dry_run = true;
        } else if (arg == "--stdin") {
            read_stdin = true;
        } else if (arg == "--help" || arg == "-h") {
            printUsage();
            return 0;
        } else {
            input = arg;
        }
    }

    if (!config_path.empty()) {
        if (!configManager.loadFromFile(config_path)) {
            return 2; // Config error
        }
        configManager.applyTo(engine, scorer);
    }

    if (configManager.getConfig().dry_run) {
        std::cout << "Configuration validated successfully." << std::endl;
        return 0;
    }

    if (list_rules) {
        std::cout << "Active Rules:\n";
        for (const auto& r : engine.getRules()) {
            std::cout << " - [" << r.id << "] " << r.category << ": " << r.rationale << " (Severity: " << r.default_severity << ")\n";
        }
        return 0;
    }

    if (read_stdin) {
        std::string line;
        while (std::getline(std::cin, line)) {
            input += line + "\n";
        }
    }

    if (input.empty()) {
        std::cerr << "Error: No input text provided." << std::endl;
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto findings = engine.analyze(input, read_stdin ? "stdin" : "arg");
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    int riskScore = scorer.calculateRisk(findings);

    if (!configManager.getConfig().quiet_mode) {
        if (format == "json") std::cout << Reporter::toJSON(findings) << std::endl;
        else if (format == "csv") std::cout << Reporter::toCSV(findings) << std::endl;
        else if (format == "md") std::cout << Reporter::toMarkdown(findings) << std::endl;
        else std::cout << Reporter::toPlainText(findings) << std::endl;

        std::cout << "\n--- Scan Summary ---\n";
        std::cout << "Risk Score: " << riskScore << " / 100\n";
        std::cout << "Findings: " << findings.size() << "\n";
        std::cout << "Scan Time: " << elapsed.count() << " ms\n";
    } else {
        std::cout << riskScore << std::endl;
    }

    alerter.triggerAlert(riskScore);

    return (riskScore >= configManager.getConfig().failure_threshold) ? 1 : 0;
}
