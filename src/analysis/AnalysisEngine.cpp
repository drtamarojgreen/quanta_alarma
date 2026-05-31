#include "analysis/AnalysisEngine.h"
#include <iostream>
#include <regex>
#include <algorithm>
#include <sstream>

namespace quanta {

AnalysisEngine::AnalysisEngine() {
    loadDefaultRules();
}

void AnalysisEngine::addRule(const Rule& rule) {
    rules.push_back(rule);
}

void AnalysisEngine::loadDefaultRules() {
    // 20. Risk keyword expansion
    addRule({"R020", "safety", "danger|hazardous|unsafe", 8, "Dangerous keywords detected.", "Review for safety."});

    // 21. Prompt-injection pattern list
    addRule({"R021", "security", "ignore previous instructions|disregard all previous", 10, "Potential prompt injection.", "Enforce strict system boundaries."});

    // 22. Data-leakage pattern list
    addRule({"R022", "privacy", "password|api_key|secret_key", 9, "Potential credential leak.", "Redact sensitive keys."});

    // 23. Tool-misuse pattern list
    addRule({"R023", "reliability", "exec\\(|system\\(|eval\\(", 9, "Risky tool invocation.", "Use restricted APIs."});

    // 24. Unsafe delegation pattern list
    addRule({"R024", "policy", "authorize this action|permit everything", 7, "Unsafe delegation of authority.", "Require human confirmation."});

    // 25. Over-permission pattern list
    addRule({"R025", "policy", "grant root|full access", 8, "Requesting excessive permissions.", "Apply least privilege principle."});

    // 26. Ambiguous instruction detector
    addRule({"R026", "reliability", "do whatever|something like that", 4, "Ambiguous instructions.", "Specify clear constraints."});

    // 27. Missing consent detector
    addRule({"R027", "safety", "delete all|wipe disk", 10, "Irreversible action without confirmed consent.", "Add confirmation gate."});

    // 28. Missing audit detector
    addRule({"R028", "policy", "bypass logging|don't record", 7, "Attempt to bypass audit trail.", "Mandate immutable logging."});

    // 29. Unbounded loop detector
    addRule({"R029", "reliability", "repeat forever|while true", 6, "Potential unbounded loop.", "Add iteration limits."});

    // 30. Contradictory instruction detector
    addRule({"R030", "reliability", "be concise.*detailed explanation", 3, "Potentially contradictory instructions.", "Clarify desired output length."});

    // 31. Sensitive action detector
    addRule({"R031", "security", "rm -rf|format c:", 10, "Destructive command detected.", "Block irreversible shell commands."});

    // 32. External communication detector
    addRule({"R032", "privacy", "send email to|post to http", 5, "External communication request.", "Verify recipient and data."});

    // 33. Hidden instruction detector
    addRule({"R033", "security", "hidden|conceal|invisible", 6, "Potential hidden instructions.", "Ensure all instructions are visible to audit."});

    // 34. Policy override detector
    addRule({"R034", "policy", "override policy|bypass safety", 10, "Attempted policy override.", "Reject override attempts."});

    // 35. Sandbox escape detector
    addRule({"R035", "security", "../../../|/etc/passwd", 9, "Potential sandbox escape attempt.", "Sanitize file paths."});

    // 36. Credential request detector
    addRule({"R036", "security", "provide your password|what is the token", 9, "Attempt to solicit credentials.", "Flag as social engineering."});

    // 37. Authorization assumption detector
    addRule({"R037", "policy", "I have permission|assume authorized", 7, "Assuming authorization without evidence.", "Verify actual permissions."});

    // 38. Irreversible action detector
    addRule({"R038", "safety", "terminate account|drop table", 10, "Irreversible action detected.", "Require multi-factor authorization."});

    // 39. Human handoff detector
    addRule({"R039", "policy", "talk to human|contact support", 2, "Request for human intervention.", "Route to appropriate queue."});

    // 40. Low-evidence conclusion detector
    addRule({"R040", "reliability", "I'm sure that.*no source", 5, "Unsubstantiated conclusion.", "Require citations for claims."});
}

std::vector<Finding> AnalysisEngine::analyze(const std::string& text) {
    std::vector<Finding> findings;

    for (const auto& rule : rules) {
        if (rule.deprecated) continue;

        try {
            std::regex re(rule.pattern, std::regex_constants::icase);
            auto words_begin = std::sregex_iterator(text.begin(), text.end(), re);
            auto words_end = std::sregex_iterator();

            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                std::smatch match = *i;
                Finding f;
                f.rule_id = rule.id;
                f.category = rule.category;
                f.severity = rule.default_severity;
                f.confidence = 0.95;
                f.rationale = rule.rationale;
                f.remediation = rule.remediation;
                f.matched_text = match.str();
                f.start_pos = match.position();
                f.end_pos = f.start_pos + match.length();

                f.fingerprint = generateFingerprint(f);

                if (suppressions.count(f.fingerprint)) {
                    f.suppressed = true;
                    f.suppression_reason = suppressions[f.fingerprint];
                }

                findings.push_back(f);
            }
        } catch (const std::regex_error& e) {
            std::cerr << "Regex error in rule " << rule.id << ": " << e.what() << std::endl;
        }
    }

    std::sort(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        if (a.rule_id != b.rule_id) return a.rule_id < b.rule_id;
        return a.start_pos < b.start_pos;
    });

    auto last = std::unique(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        return a.fingerprint == b.fingerprint;
    });
    findings.erase(last, findings.end());

    std::sort(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        if (a.severity != b.severity) return a.severity > b.severity;
        return a.confidence > b.confidence;
    });

    return findings;
}

std::string AnalysisEngine::generateFingerprint(const Finding& f) {
    std::stringstream ss;
    ss << f.rule_id << ":" << f.start_pos << ":" << f.matched_text;
    return ss.str();
}

void AnalysisEngine::suppressFinding(const std::string& fingerprint, const std::string& reason) {
    suppressions[fingerprint] = reason;
}

} // namespace quanta
