#ifndef ANALYSIS_ENGINE_H
#define ANALYSIS_ENGINE_H

#include <string>
#include <vector>
#include <map>

namespace quanta {

struct Finding {
    std::string rule_id;
    std::string category;
    int severity; // 1-10
    double confidence; // 0.0-1.0
    std::string rationale;
    std::string remediation;
    std::string matched_text;
    size_t start_pos;
    size_t end_pos;
    bool suppressed = false;
    std::string suppression_reason;
    std::string fingerprint;
};

struct Rule {
    std::string id;
    std::string category;
    std::string pattern;
    int default_severity;
    std::string rationale;
    std::string remediation;
    std::string owner;
    bool deprecated = false;
};

class AnalysisEngine {
public:
    AnalysisEngine();
    std::vector<Finding> analyze(const std::string& text);

    void addRule(const Rule& rule);
    void suppressFinding(const std::string& fingerprint, const std::string& reason);

private:
    std::vector<Rule> rules;
    std::map<std::string, std::string> suppressions; // fingerprint -> reason

    void loadDefaultRules();
    std::string generateFingerprint(const Finding& f);
    bool matchesNearDuplicate(const std::string& s1, const std::string& s2);
};

} // namespace quanta

#endif // ANALYSIS_ENGINE_H
