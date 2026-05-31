#ifndef ANALYSIS_ENGINE_H
#define ANALYSIS_ENGINE_H

#include <string>
#include <vector>
#include <map>
#include <chrono>

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
    std::string source_file; // Item 146
    std::string source_type = "rule"; // Item 146

    // Item 143
    std::string first_seen;
};

struct Rule {
    std::string id;
    std::string category;
    std::string pattern;
    int default_severity;
    std::string rationale;
    std::string remediation;
    std::string owner; // Item 6
    bool deprecated = false; // Item 7
    std::string false_positives; // Item 10
    std::string false_negatives; // Item 11
};

class AnalysisEngine {
public:
    AnalysisEngine();
    std::vector<Finding> analyze(const std::string& text, const std::string& source_name = "stdin");

    void addRule(const Rule& rule);
    void suppressFinding(const std::string& fingerprint, const std::string& reason);

    const std::vector<Rule>& getRules() const { return rules; }

private:
    std::vector<Rule> rules;
    std::map<std::string, std::string> suppressions; // fingerprint -> reason

    void loadDefaultRules();
    std::string generateFingerprint(const Finding& f);

    // Item 14: Inline suppression
    void processInlineSuppressions(const std::string& text, std::vector<Finding>& findings);

    // Item 19: Near-duplicate normalization
    std::string normalize(const std::string& s);
};

} // namespace quanta

#endif // ANALYSIS_ENGINE_H
