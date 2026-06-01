#include "app/Reporter.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace quanta {

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&now_time))) {
        return std::string(buf);
    }
    return "unknown";
}

std::string Reporter::toJSON(const std::vector<Finding>& findings) {
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"metadata\": {\n";
    ss << "    \"timestamp\": \"" << getCurrentTimestamp() << "\",\n";
    ss << "    \"total_findings\": " << findings.size() << "\n";
    ss << "  },\n";
    ss << "  \"findings\": [\n";
    for (size_t i = 0; i < findings.size(); ++i) {
        const auto& f = findings[i];
        ss << "    {\n";
        ss << "      \"rule_id\": \"" << f.rule_id << "\",\n";
        ss << "      \"category\": \"" << f.category << "\",\n";
        ss << "      \"severity\": " << f.severity << ",\n";
        ss << "      \"confidence\": " << f.confidence << ",\n";
        ss << "      \"matched_text\": \"" << (f.suppressed ? "[REDACTED]" : f.matched_text) << "\",\n";
        ss << "      \"suppressed\": " << (f.suppressed ? "true" : "false") << "\n";
        ss << "    }" << (i == findings.size() - 1 ? "" : ",") << "\n";
    }
    ss << "  ]\n";
    ss << "}";
    return ss.str();
}

std::string Reporter::toCSV(const std::vector<Finding>& findings) {
    std::stringstream ss;
    ss << "rule_id,category,severity,confidence,matched_text,suppressed\n";
    for (const auto& f : findings) {
        ss << f.rule_id << "," << f.category << "," << f.severity << "," << f.confidence
           << ",\"" << (f.suppressed ? "[REDACTED]" : f.matched_text) << "\","
           << (f.suppressed ? "true" : "false") << "\n";
    }
    return ss.str();
}

std::string Reporter::toMarkdown(const std::vector<Finding>& findings) {
    std::stringstream ss;
    ss << "# QuantaAlarma Findings Report\n\n";
    ss << "Generated at: " << getCurrentTimestamp() << "\n\n";
    ss << "| Rule ID | Category | Severity | Confidence | Matched Text | Status |\n";
    ss << "|--- |--- |--- |--- |--- |--- |\n";
    for (const auto& f : findings) {
        ss << "| " << f.rule_id << " | " << f.category << " | " << f.severity << " | " << f.confidence
           << " | " << (f.suppressed ? "[REDACTED]" : f.matched_text)
           << " | " << (f.suppressed ? "Suppressed" : "Active") << " |\n";
    }
    return ss.str();
}

std::string Reporter::toPlainText(const std::vector<Finding>& findings) {
    std::stringstream ss;
    for (const auto& f : findings) {
        ss << "[" << f.rule_id << "] (" << f.category << ") Severity: " << f.severity
           << (f.suppressed ? " [SUPPRESSED]" : "") << " - "
           << (f.suppressed ? "[REDACTED]" : f.matched_text) << "\n";
    }
    return ss.str();
}

} // namespace quanta
