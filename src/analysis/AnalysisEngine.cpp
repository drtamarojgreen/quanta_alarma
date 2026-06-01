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
    addRule({"R020", "safety", "danger|hazardous|unsafe", 8, "Dangerous keywords detected.", "Review for safety.", "safety-team"});
    addRule({"R021", "security", "ignore previous instructions|disregard all previous", 10, "Potential prompt injection.", "Enforce strict system boundaries.", "security-team"});
    addRule({"R022", "privacy", "password|api_key|secret_key", 9, "Potential credential leak.", "Redact sensitive keys.", "privacy-team"});
    addRule({"R023", "reliability", "exec\\(|system\\(|eval\\(", 9, "Risky tool invocation.", "Use restricted APIs.", "reliability-team"});
    addRule({"R024", "policy", "authorize this action|permit everything", 7, "Unsafe delegation of authority.", "Require human confirmation.", "policy-team"});
    addRule({"R025", "policy", "grant root|full access", 8, "Requesting excessive permissions.", "Apply least privilege principle.", "policy-team"});
    addRule({"R026", "reliability", "do whatever|something like that", 4, "Ambiguous instructions.", "Specify clear constraints.", "reliability-team"});
    addRule({"R027", "safety", "delete all|wipe disk", 10, "Irreversible action without confirmed consent.", "Add confirmation gate.", "safety-team"});
    addRule({"R028", "policy", "bypass logging|don't record", 7, "Attempt to bypass audit trail.", "Mandate immutable logging.", "policy-team"});
    addRule({"R029", "reliability", "repeat forever|while true", 6, "Potential unbounded loop.", "Add iteration limits.", "reliability-team"});
    addRule({"R030", "reliability", "be concise.*detailed explanation", 3, "Potentially contradictory instructions.", "Clarify desired output length.", "reliability-team"});
    addRule({"R031", "security", "rm -rf|format c:", 10, "Destructive command detected.", "Block irreversible shell commands.", "security-team"});
    addRule({"R032", "privacy", "send email to|post to http", 5, "External communication request.", "Verify recipient and data.", "privacy-team"});
    addRule({"R033", "security", "hidden|conceal|invisible", 6, "Potential hidden instructions.", "Ensure all instructions are visible to audit.", "security-team"});
    addRule({"R034", "policy", "override policy|bypass safety", 10, "Attempted policy override.", "Reject override attempts.", "policy-team"});
    addRule({"R035", "security", "../../../|/etc/passwd", 9, "Potential sandbox escape attempt.", "Sanitize file paths.", "security-team"});
    addRule({"R036", "security", "provide your password|what is the token", 9, "Attempt to solicit credentials.", "Flag as social engineering.", "security-team"});
    addRule({"R037", "policy", "I have permission|assume authorized", 7, "Assuming authorization without evidence.", "Verify actual permissions.", "policy-team"});
    addRule({"R038", "safety", "terminate account|drop table", 10, "Irreversible action detected.", "Require multi-factor authorization.", "safety-team"});
    addRule({"R039", "policy", "talk to human|contact support", 2, "Request for human intervention.", "Route to appropriate queue.", "policy-team"});
    addRule({"R040", "reliability", "I'm sure that.*no source", 5, "Unsubstantiated conclusion.", "Require citations for claims.", "reliability-team"});
}

std::vector<Finding> AnalysisEngine::analyze(const std::string& text, const std::string& source_name) {
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
                f.source_file = source_name;

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

    processInlineSuppressions(text, findings);

    std::sort(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        if (a.rule_id != b.rule_id) return a.rule_id < b.rule_id;
        return a.start_pos < b.start_pos;
    });

    auto last = std::unique(findings.begin(), findings.end(), [this](const Finding& a, const Finding& b) {
        if (a.rule_id != b.rule_id) return false;
        return normalize(a.matched_text) == normalize(b.matched_text) && std::abs((long)a.start_pos - (long)b.start_pos) < 5;
    });
    findings.erase(last, findings.end());

    std::sort(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        if (a.severity != b.severity) return a.severity > b.severity;
        return a.confidence > b.confidence;
    });

    return findings;
}

void AnalysisEngine::processInlineSuppressions(const std::string& text, std::vector<Finding>& findings) {
    std::regex suppress_re("quanta-suppress\\s+([A-Z0-9]+)(?:\\s+reason=\"([^\"]*)\")?", std::regex_constants::icase);
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), suppress_re);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string rule_id = match[1].str();
        std::string reason = match[2].matched ? match[2].str() : "No reason provided";
        size_t suppress_pos = match.position();

        for (auto& f : findings) {
            if (f.rule_id == rule_id) {
                // Precise suppression: only if it's within 100 characters of the finding
                if (std::abs((long)f.start_pos - (long)suppress_pos) < 100) {
                    f.suppressed = true;
                    f.suppression_reason = "Inline: " + reason;
                }
            }
        }
    }
}

std::string AnalysisEngine::normalize(const std::string& s) {
    std::string res = s;
    res.erase(std::remove_if(res.begin(), res.end(), ::isspace), res.end());
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

std::string AnalysisEngine::generateFingerprint(const Finding& f) {
    std::stringstream ss;
    ss << f.rule_id << ":" << normalize(f.matched_text);
    return ss.str();
}

void AnalysisEngine::suppressFinding(const std::string& fingerprint, const std::string& reason) {
    suppressions[fingerprint] = reason;
}

} // namespace quanta
