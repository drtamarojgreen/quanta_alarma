#include "app/Reporter.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <map>

namespace quanta {

const std::string APP_VERSION = "1.1.0-alpha"; // Item 93

std::string Reporter::escapeJSON(const std::string& s) {
    std::stringstream ss;
    for (char c : s) {
        switch (c) {
            case '"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    ss << c;
                }
        }
    }
    return ss.str();
}

std::string Reporter::escapeCSV(const std::string& s) {
    std::string res = s;
    size_t pos = 0;
    while ((pos = res.find('"', pos)) != std::string::npos) {
        res.replace(pos, 1, "\"\"");
        pos += 2;
    }
    return "\"" + res + "\"";
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&now_time))) {
        return std::string(buf);
    }
    return "unknown";
}

std::string Reporter::toJSON(const std::vector<Finding>& findings, bool redact) { // Item 128
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"audit\": {\n"; // Item 127
    ss << "    \"timestamp\": \"" << getCurrentTimestamp() << "\",\n";
    ss << "    \"version\": \"" << APP_VERSION << "\",\n";
    ss << "    \"total_findings\": " << findings.size() << "\n";
    ss << "  },\n";
    ss << "  \"findings\": [\n";
    for (size_t i = 0; i < findings.size(); ++i) {
        const auto& f = findings[i];
        ss << "    {\n";
        ss << "      \"rule_id\": \"" << escapeJSON(f.rule_id) << "\",\n";
        ss << "      \"category\": \"" << escapeJSON(f.category) << "\",\n";
        ss << "      \"severity\": " << f.severity << ",\n";
        ss << "      \"confidence\": " << f.confidence << ",\n";
        ss << "      \"matched_text\": \"" << (redact || f.suppressed ? "[REDACTED]" : escapeJSON(f.matched_text)) << "\",\n";
        ss << "      \"fingerprint\": \"" << escapeJSON(f.fingerprint) << "\",\n"; // Item 142
        ss << "      \"status\": \"" << escapeJSON(f.status) << "\",\n"; // Item 144
        ss << "      \"suppressed\": " << (f.suppressed ? "true" : "false") << "\n";
        ss << "    }" << (i == findings.size() - 1 ? "" : ",") << "\n";
    }
    ss << "  ]\n";
    ss << "}";
    return ss.str();
}

std::string Reporter::toCSV(const std::vector<Finding>& findings, bool redact) {
    std::stringstream ss;
    ss << "rule_id,category,severity,confidence,matched_text,fingerprint,status,suppressed\n";
    for (const auto& f : findings) {
        ss << escapeCSV(f.rule_id) << "," << escapeCSV(f.category) << "," << f.severity << "," << f.confidence
           << "," << (redact || f.suppressed ? "\"[REDACTED]\"" : escapeCSV(f.matched_text)) << ","
           << escapeCSV(f.fingerprint) << "," << escapeCSV(f.status) << ","
           << (f.suppressed ? "true" : "false") << "\n";
    }
    return ss.str();
}

std::string Reporter::toMarkdown(const std::vector<Finding>& findings, bool redact) {
    std::stringstream ss;
    ss << "# QuantaAlarma Findings Report\n\n";
    ss << "Generated at: " << getCurrentTimestamp() << "\n\n";
    ss << "| Rule ID | Category | Severity | Confidence | Fingerprint | Status |\n"; // Trimming (Item 129)
    ss << "|--- |--- |--- |--- |--- |--- |\n";
    for (const auto& f : findings) {
        ss << "| " << f.rule_id << " | " << f.category << " | " << f.severity << " | " << f.confidence
           << " | " << f.fingerprint << " | " << (f.suppressed ? "Suppressed" : f.status) << " |\n";
    }
    return ss.str();
}

std::string Reporter::toPlainText(const std::vector<Finding>& findings, bool redact) {
    std::stringstream ss;
    for (const auto& f : findings) {
        ss << "[" << f.rule_id << "] (" << f.category << ") Severity: " << f.severity
           << (f.suppressed ? " [SUPPRESSED]" : "") << " Status: " << f.status << " - "
           << (redact || f.suppressed ? "[REDACTED]" : f.matched_text) << "\n";
    }
    return ss.str();
}

std::string Reporter::toSummaryOnly(const std::vector<Finding>& findings) { // Item 125
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"summary\": {\n";
    ss << "    \"total_findings\": " << findings.size() << ",\n";
    std::map<std::string, int> cat_counts;
    for (const auto& f : findings) cat_counts[f.category]++;
    ss << "    \"categories\": {\n";
    size_t j = 0;
    for (const auto& [cat, count] : cat_counts) {
        ss << "      \"" << escapeJSON(cat) << "\": " << count << (j == cat_counts.size() - 1 ? "" : ",") << "\n";
        j++;
    }
    ss << "    }\n";
    ss << "  }\n";
    ss << "}";
    return ss.str();
}

std::string Reporter::toFindingsOnly(const std::vector<Finding>& findings, const std::string& format) { // Item 126
    if (format == "json") {
         std::stringstream ss;
         ss << "[\n";
         for (size_t i = 0; i < findings.size(); ++i) {
             const auto& f = findings[i];
             ss << "  {\n";
             ss << "    \"rule_id\": \"" << escapeJSON(f.rule_id) << "\",\n";
             ss << "    \"severity\": " << f.severity << ",\n";
             ss << "    \"fingerprint\": \"" << escapeJSON(f.fingerprint) << "\"\n";
             ss << "  }" << (i == findings.size() - 1 ? "" : ",") << "\n";
         }
         ss << "]";
         return ss.str();
    }
    std::stringstream ss;
    for (const auto& f : findings) {
        ss << f.rule_id << ":" << f.fingerprint << " (" << f.severity << ")\n";
    }
    return ss.str();
}

std::vector<Finding> Reporter::filterFindings(const std::vector<Finding>& findings, int min_severity, const std::string& category, const std::string& path) { // Items 136-138
    std::vector<Finding> result;
    for (const auto& f : findings) {
        if (f.severity < min_severity) continue;
        if (!category.empty() && f.category != category) continue;
        if (!path.empty() && f.source_file.find(path) == std::string::npos) continue;
        result.push_back(f);
    }
    return result;
}

void Reporter::sortFindings(std::vector<Finding>& findings) { // Item 139
    std::sort(findings.begin(), findings.end(), [](const Finding& a, const Finding& b) {
        if (a.rule_id != b.rule_id) return a.rule_id < b.rule_id;
        return a.start_pos < b.start_pos;
    });
}

} // namespace quanta
