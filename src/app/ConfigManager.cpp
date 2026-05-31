#include "app/ConfigManager.h"
#include <fstream>
#include <sstream>

namespace quanta {

ConfigManager::ConfigManager() {}

bool ConfigManager::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        // Simple key=value parser for now
        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos) continue;

        std::string key = line.substr(0, eq_pos);
        std::string value = line.substr(eq_pos + 1);

        if (key == "failure_threshold") config.failure_threshold = std::stoi(value);
        else if (key == "quiet_mode") config.quiet_mode = (value == "true");
        else if (key == "verbose_mode") config.verbose_mode = (value == "true");
        else if (key == "profile") config.profile = value;
    }
    return true;
}

void ConfigManager::applyTo(AnalysisEngine& engine, RiskScorer& scorer) {
    for (const auto& rule : config.custom_rules) {
        engine.addRule(rule);
    }
    for (const auto& [cat, weight] : config.category_weights) {
        scorer.setSeverityWeight(cat, weight);
    }
}

} // namespace quanta
