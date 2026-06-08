#include "analysis/AnalysisEngine.h"
#include "scoring/RiskScorer.h"
#include "alerting/AlertingSystem.h"
#include "app/ConfigManager.h"
#include "app/Reporter.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

namespace quanta {
const std::string VERSION = "1.1.0-alpha";
}

using namespace quanta;

void printUsage() {
    std::cout << "QuantaAlarma CLI " << VERSION << "\n"
              << "Usage: quanta_alarma [options] <input_text>\n\n"
              << "Options:\n"
              << "  --config <path>         Path to policy configuration file\n"
              << "  --format json|csv|md    Output format (default: text, summary, findings)\n"
              << "  --list-rules            List all active rules and exit\n"
              << "  --explain-rule <id>     Explain a specific rule and exit\n"
              << "  --list-profiles         List built-in configuration profiles and exit\n"
              << "  --version               Print version information and exit\n"
              << "  --quiet                 Reduce output to findings only\n"
              << "  --verbose               Show detailed analysis reasoning\n"
              << "  --dry-run               Validate configuration and exit\n"
              << "  --stdin                 Read input text from standard input\n"
              << "  --threshold <n>         Set failure threshold (0-100)\n"
              << "  --min-severity <n>      Filter findings by minimum severity\n"
              << "  --filter-category <cat> Filter findings by category\n"
              << "  --filter-path <path>    Filter findings by source path\n"
              << "  --redact                Redact matched text in reports\n"
              << "\nExit Codes:\n"
              << "  0: Success (Risk below threshold)\n"
              << "  1: Risk at or above threshold\n"
              << "  2: Configuration or environmental error\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::vector<std::string> inputs;
    std::string config_path;
    std::string format = "text";
    bool read_stdin = false;
    bool list_rules = false;
    std::string explain_rule_id;
    bool list_profiles = false;
    bool redact = false;
    int min_severity = 0;
    std::string filter_category;
    std::string filter_path;

    AnalysisEngine engine;
    RiskScorer scorer;
    AlertingSystem alerter;
    ConfigManager configManager;

    // Pass 1: Configuration and Profiles
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--config" && i + 1 < argc) {
            config_path = argv[++i];
        } else if (arg == "--profile" && i + 1 < argc) {
            configManager.mutableConfig().profile = argv[++i];
        }
    }

    if (!config_path.empty()) {
        if (!configManager.loadFromFile(config_path)) return 2;
    }
    configManager.applyTo(engine, scorer);

    // Pass 2: Overrides and Commands
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        try {
            if (arg == "--config" || arg == "--profile") {
                i++; // Skip already processed
            } else if (arg == "--format" && i + 1 < argc) {
                format = argv[++i];
            } else if (arg == "--threshold" && i + 1 < argc) {
                configManager.mutableConfig().failure_threshold = std::stoi(argv[++i]);
            } else if (arg == "--min-severity" && i + 1 < argc) {
                min_severity = std::stoi(argv[++i]);
            } else if (arg == "--filter-category" && i + 1 < argc) {
                filter_category = argv[++i];
            } else if (arg == "--filter-path" && i + 1 < argc) {
                filter_path = argv[++i];
            } else if (arg == "--list-rules") {
                list_rules = true;
            } else if (arg == "--explain-rule" && i + 1 < argc) {
                explain_rule_id = argv[++i];
            } else if (arg == "--list-profiles") {
                list_profiles = true;
            } else if (arg == "--redact") {
                redact = true;
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
            } else if (arg.substr(0, 2) != "--") {
                inputs.push_back(arg);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid argument value for '" << arg << "' - " << e.what() << std::endl;
            return 2;
        }
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

    if (!explain_rule_id.empty()) { // Item 95
        for (const auto& r : engine.getRules()) {
            if (r.id == explain_rule_id) {
                std::cout << "Rule: " << r.id << "\n"
                          << "Category: " << r.category << "\n"
                          << "Rationale: " << r.rationale << "\n"
                          << "Remediation: " << r.remediation << "\n"
                          << "Default Severity: " << r.default_severity << "\n"
                          << "Owner: " << r.owner << "\n" // Item 6
                          << "Deprecated: " << (r.deprecated ? "Yes" : "No") << "\n" // Item 7
                          << "False Positives: " << r.false_positives << "\n" // Item 10
                          << "False Negatives: " << r.false_negatives << "\n"; // Item 11
                if (!r.changelog.empty()) { // Item 8
                    std::cout << "Changelog:\n";
                    for (const auto& entry : r.changelog) std::cout << "  - " << entry << "\n";
                }
                return 0;
            }
        }
        std::cerr << "Error: Rule " << explain_rule_id << " not found." << std::endl;
        return 2;
    }

    if (list_profiles) { // Item 96
        std::cout << "Built-in Profiles:\n"
                  << " - default: Standard detection settings\n"
                  << " - strict:  Conservative settings for high-assurance\n"
                  << " - safe:    Permissive settings favoring safety\n"
                  << " - quiet:   Minimal output for automation\n"
                  << " - verbose: Detailed reasoning for debugging\n"
                  << " - baseline: Gradual introduction of checks\n";
        return 0;
    }

    if (read_stdin) {
        std::string full_input;
        std::string line;
        while (std::getline(std::cin, line)) {
            full_input += line + "\n";
        }
        if (!full_input.empty()) {
            inputs.push_back(full_input);
        }
    }

    if (inputs.empty()) {
        std::cerr << "Error: No input text provided." << std::endl;
        return 1;
    }

    int files_scanned = static_cast<int>(inputs.size()); // Item 88

    auto start_time = std::chrono::high_resolution_clock::now(); // Item 87
    std::vector<Finding> findings;
    for (size_t i = 0; i < inputs.size(); ++i) {
        auto sub_findings = engine.analyze(inputs[i], read_stdin ? "stdin" : "arg-" + std::to_string(i));
        findings.insert(findings.end(), sub_findings.begin(), sub_findings.end());
    }

    // Item 139: Deterministic ordering
    Reporter::sortFindings(findings);

    // Item 136, 137: Filtering
    findings = Reporter::filterFindings(findings, min_severity, filter_category, filter_path);

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    int riskScore = scorer.calculateRisk(findings);

    if (!configManager.getConfig().quiet_mode) {
        if (format == "json") std::cout << Reporter::toJSON(findings, redact) << std::endl;
        else if (format == "csv") std::cout << Reporter::toCSV(findings, redact) << std::endl;
        else if (format == "md") std::cout << Reporter::toMarkdown(findings, redact) << std::endl;
        else if (format == "summary") std::cout << Reporter::toSummaryOnly(findings) << std::endl;
        else if (format == "findings") std::cout << Reporter::toFindingsOnly(findings, "text") << std::endl;
        else std::cout << Reporter::toPlainText(findings, redact) << std::endl;

        std::cout << "\n--- Scan Summary ---\n"; // Item 81
        std::cout << "Risk Score: " << riskScore << " / 100\n";
        std::cout << "Findings: " << findings.size() << "\n"; // Item 89
        std::cout << "Scan Time: " << elapsed.count() << " ms\n"; // Item 87
        std::cout << "Files Scanned: " << files_scanned << "\n"; // Item 88
        configManager.printSummary(); // Item 66
    } else {
        std::cout << riskScore << std::endl;
    }

    alerter.triggerAlert(riskScore);

    return (riskScore >= configManager.getConfig().failure_threshold) ? 1 : 0;
}
