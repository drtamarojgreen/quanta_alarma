#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "analysis/AnalysisEngine.h"
#include "scoring/RiskScorer.h"
#include <string>
#include <vector>
#include <map>

namespace quanta {

struct Config {
    std::vector<Rule> custom_rules;
    std::map<std::string, double> category_weights;
    int failure_threshold = 80;
    bool quiet_mode = false;
    bool verbose_mode = false;
    bool dry_run = false; // Item 50
    std::string profile = "default";
};

class ConfigManager {
public:
    ConfigManager();
    bool loadFromFile(const std::string& filepath);
    const Config& getConfig() const { return config; }
    Config& mutableConfig() { return config; }

    void applyTo(AnalysisEngine& engine, RiskScorer& scorer);

private:
    Config config;
};

} // namespace quanta

#endif // CONFIG_MANAGER_H
