#include "app/ConfigManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace quanta {

ConfigManager::ConfigManager() {}

bool ConfigManager::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open configuration file: " << filepath << std::endl;
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
            if (key == "failure_threshold") config.failure_threshold = std::stoi(value);
            else if (key == "quiet_mode") config.quiet_mode = (value == "true");
            else if (key == "verbose_mode") config.verbose_mode = (value == "true");
            else if (key == "profile") config.profile = value;
            else {
                std::cerr << "Warning: Unknown configuration key at " << filepath << ":" << line_num << " - '" << key << "'" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid value for '" << key << "' at " << filepath << ":" << line_num << " - " << e.what() << std::endl;
        }
    }
    return true;
}

void ConfigManager::applyTo(AnalysisEngine& engine, RiskScorer& scorer) {
    if (config.profile == "strict") {
        config.failure_threshold = 50;
        scorer.setSeverityWeight("security", 2.0);
    } else if (config.profile == "safe") {
        config.failure_threshold = 30;
    } else if (config.profile == "quiet") {
        config.quiet_mode = true;
    }

    for (const auto& rule : config.custom_rules) {
        engine.addRule(rule);
    }
    for (const auto& [cat, weight] : config.category_weights) {
        scorer.setSeverityWeight(cat, weight);
    }
}

} // namespace quanta
