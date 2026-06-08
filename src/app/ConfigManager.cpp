#include "app/ConfigManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace quanta {

ConfigManager::ConfigManager() {
    config.checksum = calculateChecksum();
}

bool ConfigManager::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open configuration file: " << filepath << " (Actionable: Check if the file path is correct and accessible)" << std::endl; // Item 46
        return false;
    }

    std::string line;
    int line_num = 0;
    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos) {
            std::cerr << "Warning: Malformed configuration at " << filepath << ":" << line_num << " - Missing '='" << std::endl;
            continue;
        }

        std::string key = line.substr(0, eq_pos);
        std::string value = line.substr(eq_pos + 1);

        try {
            if (key == "failure_threshold") {
                config.failure_threshold = std::stoi(value);
                config.source_trace[key] = filepath; // Item 77
            } else if (key == "quiet_mode") {
                config.quiet_mode = (value == "true");
                config.source_trace[key] = filepath;
            } else if (key == "verbose_mode") {
                config.verbose_mode = (value == "true");
                config.source_trace[key] = filepath;
            } else if (key == "profile") {
                config.profile = value;
                config.source_trace[key] = filepath;
            } else if (key == "legacy_mode") {
                std::cerr << "Warning: Deprecated configuration key at " << filepath << ":" << line_num << " - '" << key << "'. Please use specific profiles instead." << std::endl; // Item 48
            } else {
                std::cerr << "Warning: Unknown configuration key at " << filepath << ":" << line_num << " - '" << key << "' (Suggestion: Check spelling or refer to documentation)" << std::endl; // Item 47
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid value for '" << key << "' at " << filepath << ":" << line_num << " - " << e.what() << " (Expected numeric for threshold or true/false for modes)" << std::endl; // Item 46
        }
    }

    config.checksum = calculateChecksum(); // Item 76
    return true;
}

void ConfigManager::applyTo(AnalysisEngine& engine, RiskScorer& scorer) {
    if (config.profile == "strict") { // High-assurance (Item 67)
        config.failure_threshold = 40;
        scorer.setSeverityWeight("security", 2.0);
        scorer.setSeverityWeight("privacy", 2.0);
        engine.setRuleSeverityOverride("R021", 10); // Prompt injection is critical
    } else if (config.profile == "safe") { // Conservative (Item 67)
        config.failure_threshold = 30;
        scorer.setSeverityWeight("safety", 1.5);
    } else if (config.profile == "quiet") { // Item 68
        config.quiet_mode = true;
    } else if (config.profile == "verbose") { // Item 69
        config.verbose_mode = true;
    } else if (config.profile == "baseline") { // Item 70
        config.failure_threshold = 95; // Permissive for gradual intro
        scorer.setSeverityWeight("security", 0.5);
    }

    for (const auto& rule : config.custom_rules) {
        engine.addRule(rule);
    }
    for (const auto& [cat, weight] : config.category_weights) {
        scorer.setSeverityWeight(cat, weight);
    }
}

void ConfigManager::printSummary() const { // Item 66
    std::string effective_checksum = calculateChecksum();
    std::cout << "--- Effective Configuration Summary ---\n";
    std::cout << "Profile: " << config.profile << "\n";
    std::cout << "Threshold: " << config.failure_threshold << "\n";
    std::cout << "Quiet: " << (config.quiet_mode ? "on" : "off") << "\n";
    std::cout << "Verbose: " << (config.verbose_mode ? "on" : "off") << "\n";
    std::cout << "Checksum: " << effective_checksum << "\n";
    std::cout << "---------------------------------------\n";
}

std::string ConfigManager::calculateChecksum() const { // Item 76: Deterministic FNV-1a style hash
    uint64_t hash = 0xcbf29ce484222325;
    const uint64_t prime = 0x100000001b3;

    auto hash_str = [&](const std::string& s) {
        for (char c : s) {
            hash ^= static_cast<uint64_t>(c);
            hash *= prime;
        }
    };

    auto hash_int = [&](int i) {
        hash ^= static_cast<uint64_t>(i);
        hash *= prime;
    };

    hash_str(config.profile);
    hash_int(config.failure_threshold);
    hash_int(config.quiet_mode ? 1 : 0);
    hash_int(config.verbose_mode ? 1 : 0);
    hash_int(static_cast<int>(config.custom_rules.size()));

    for (const auto& [cat, weight] : config.category_weights) {
        hash_str(cat);
        hash_int(static_cast<int>(weight * 100)); // Fixed precision for stability
    }

    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

} // namespace quanta
