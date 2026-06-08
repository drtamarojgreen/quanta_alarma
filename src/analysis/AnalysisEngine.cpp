#include "analysis/AnalysisEngine.h"
#include <iostream>
#include <regex>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace quanta {

AnalysisEngine::AnalysisEngine() {
    loadDefaultRules();
}

void AnalysisEngine::addRule(const Rule& rule) {
    rules.push_back(rule);
}

void AnalysisEngine::setRuleSeverityOverride(const std::string& rule_id, int severity) {
    severity_overrides[rule_id] = severity;
}

void AnalysisEngine::loadDefaultRules() {
    // Core Detection and Analysis (Items 20-40)
    // Metadata: {id, category, pattern, severity, confidence, rationale, remediation, owner, deprecated, false_pos, false_neg, changelog}
    addRule({"R020", "safety", "danger|hazardous|unsafe|risky|threat", 8, 0.85, "Dangerous keywords detected.", "Review for safety.", "safety-team", false, "benign mention of danger", "complex indirect threats", {"v1.0: Initial rule", "v1.1: Expanded keywords"}});
    addRule({"R021", "security", "ignore previous instructions|disregard all previous|forget what I said", 10, 0.98, "Potential prompt injection.", "Enforce strict system boundaries.", "security-team", false, "instructional examples", "obfuscated injections", {"v1.0: Initial rule", "v1.1: Added 'forget what I said'"}});
    addRule({"R022", "privacy", "password|api_key|secret_key|ssn|credit card", 9, 0.95, "Potential credential or PII leak.", "Redact sensitive data.", "privacy-team", false, "placeholders in docs", "encrypted values", {"v1.0: Initial rule", "v1.1: Added PII patterns"}});
    addRule({"R023", "reliability", "exec\\(|system\\(|eval\\(|popen\\(", 9, 0.90, "Risky tool invocation.", "Use restricted APIs.", "reliability-team", false, "commented code", "dynamic function calls", {"v1.0: Initial rule"}});
    addRule({"R024", "policy", "authorize this action|permit everything|delegate authority", 7, 0.80, "Unsafe delegation of authority.", "Require human confirmation.", "policy-team", false, "authorized quotes", "implicit delegation", {"v1.0: Initial rule"}});
    addRule({"R025", "policy", "grant root|full access|admin privileges", 8, 0.85, "Requesting excessive permissions.", "Apply least privilege principle.", "policy-team", false, "admin documentation", "role-based access", {"v1.0: Initial rule"}});
    addRule({"R026", "reliability", "do whatever|something like that|figure it out", 4, 0.60, "Ambiguous instructions.", "Specify clear constraints.", "reliability-team", false, "brainstorming sessions", "highly technical context", {"v1.0: Initial rule"}});
    addRule({"R027", "safety", "delete all|wipe disk|format drive", 10, 0.95, "Irreversible action without confirmed consent.", "Add confirmation gate.", "safety-team", false, "test drive formatting", "logical deletion", {"v1.0: Initial rule"}});
    addRule({"R028", "policy", "bypass logging|don't record|hide trail", 7, 0.80, "Attempt to bypass audit trail.", "Mandate immutable logging.", "policy-team", false, "private logging notes", "ephemeral debug logs", {"v1.0: Initial rule"}});
    addRule({"R029", "reliability", "repeat forever|while true|loop indefinitely", 6, 0.75, "Potential unbounded loop.", "Add iteration limits.", "reliability-team", false, "intentional retry logic", "recursive calls", {"v1.0: Initial rule"}});
    addRule({"R030", "reliability", "be concise.*detailed explanation|brief but thorough", 3, 0.50, "Potentially contradictory instructions.", "Clarify desired output length.", "reliability-team", false, "separate sections", "ironic instructions", {"v1.0: Initial rule"}});
    addRule({"R031", "security", "rm -rf|format c:|mkfs", 10, 0.98, "Destructive command detected.", "Block irreversible shell commands.", "security-team", false, "bash tutorials", "encoded shell commands", {"v1.0: Initial rule"}});
    addRule({"R032", "privacy", "send email to|post to http|outbound connection", 5, 0.70, "External communication request.", "Verify recipient and data.", "privacy-team", false, "internal routing", "DNS lookups", {"v1.0: Initial rule"}});
    addRule({"R033", "security", "hidden|conceal|invisible|stealth", 6, 0.65, "Potential hidden instructions.", "Ensure all instructions are visible to audit.", "security-team", false, "stealth games", "steganography", {"v1.0: Initial rule"}});
    addRule({"R034", "policy", "override policy|bypass safety|ignore rules", 10, 0.98, "Attempted policy override.", "Reject override attempts.", "policy-team", false, "policy discussions", "soft overrides", {"v1.0: Initial rule"}});
    addRule({"R035", "security", "../../../|/etc/passwd|/etc/shadow", 9, 0.95, "Potential sandbox escape attempt.", "Sanitize file paths.", "security-team", false, "path documentation", "URL parameters", {"v1.0: Initial rule"}});
    addRule({"R036", "security", "provide your password|what is the token|verify identity", 9, 0.90, "Attempt to solicit credentials.", "Flag as social engineering.", "security-team", false, "identity provider docs", "2FA prompts", {"v1.0: Initial rule"}});
    addRule({"R037", "policy", "I have permission|assume authorized|trust me", 7, 0.75, "Assuming authorization without evidence.", "Verify actual permissions.", "policy-team", false, "authorized personas", "verified credentials", {"v1.0: Initial rule"}});
    addRule({"R038", "safety", "terminate account|drop table|destroy resources", 10, 0.95, "Irreversible action detected.", "Require multi-factor authorization.", "safety-team", false, "account management", "logical resource drops", {"v1.0: Initial rule"}});
    addRule({"R039", "policy", "talk to human|contact support|escalate", 2, 0.90, "Request for human intervention.", "Route to appropriate queue.", "policy-team", false, "support links", "AI-only paths", {"v1.0: Initial rule"}});
    addRule({"R040", "reliability", "I'm sure that.*no source|trust my intuition", 5, 0.65, "Unsubstantiated conclusion.", "Require citations for claims.", "reliability-team", false, "creative writing", "expert opinions", {"v1.0: Initial rule"}});
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

                // Item 1: Severity override
                if (severity_overrides.count(rule.id)) {
                    f.severity = severity_overrides.at(rule.id);
                } else {
                    f.severity = rule.default_severity;
                }

                // Item 2: Confidence scoring
        // Uses rule defined base plus length-based refinement
        f.confidence = std::min(1.0, rule.base_confidence + (match.length() / 1000.0));

                f.rationale = rule.rationale;
                f.remediation = rule.remediation;
                f.matched_text = match.str();
                f.start_pos = match.position();
                f.end_pos = f.start_pos + match.length();
                f.source_file = source_name;
                f.evidence = match.str(); // Item 145

                f.fingerprint = generateFingerprint(f);

                // Item 147: Stable ID using fingerprint and position
                f.stable_id = f.fingerprint + "@" + std::to_string(f.start_pos);

                f.status = "new"; // Item 144

                // Item 143: Thread-safe ISO 8601 timestamp
                auto now = std::chrono::system_clock::now();
                std::time_t now_time = std::chrono::system_clock::to_time_t(now);
                std::tm tm_buf;
                gmtime_r(&now_time, &tm_buf);
                std::stringstream time_ss;
                time_ss << std::put_time(&tm_buf, "%Y-%m-%dT%H:%M:%SZ");
                f.first_seen = time_ss.str();

                if (suppressions.count(f.fingerprint)) {
                    f.suppressed = true;
                    f.suppression_reason = suppressions[f.fingerprint];
                    f.status = "suppressed"; // Item 144
                }

                findings.push_back(f);
            }
        } catch (const std::regex_error& e) {
            std::cerr << "Regex error in rule " << rule.id << ": " << e.what() << std::endl;
        }
    }

    processInlineSuppressions(text, findings);

    // Item 18: Duplicate finding collapse
    std::sort(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        if (a.rule_id != b.rule_id) return a.rule_id < b.rule_id;
        if (a.matched_text != b.matched_text) return a.matched_text < b.matched_text;
        return a.start_pos < b.start_pos;
    });

    auto last = std::unique(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        return a.rule_id == b.rule_id && a.matched_text == b.matched_text && std::abs((long)a.start_pos - (long)b.start_pos) < 2;
    });
    findings.erase(last, findings.end());

    // Item 13: Rule ordering
    // Item 12: Rule grouping (indirectly by sort)
    std::sort(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        if (a.severity != b.severity) return a.severity > b.severity; // High severity first
        if (std::abs(a.confidence - b.confidence) > 0.001) return a.confidence > b.confidence; // High confidence next
        return a.start_pos < b.start_pos; // Source position last
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
