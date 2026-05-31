#include "app/Reporter.h"
#include <sstream>
#include <iomanip>

namespace quanta {

std::string Reporter::toJSON(const std::vector<Finding>& findings) {
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < findings.size(); ++i) {
        const auto& f = findings[i];
        ss << "  {\n";
        ss << "    \"rule_id\": \"" << f.rule_id << "\",\n";
        ss << "    \"category\": \"" << f.category << "\",\n";
        ss << "    \"severity\": " << f.severity << ",\n";
        ss << "    \"confidence\": " << f.confidence << ",\n";
        ss << "    \"matched_text\": \"" << f.matched_text << "\"\n";
        ss << "  }" << (i == findings.size() - 1 ? "" : ",") << "\n";
    }
    ss << "]";
    return ss.str();
}

std::string Reporter::toCSV(const std::vector<Finding>& findings) {
    std::stringstream ss;
    ss << "rule_id,category,severity,confidence,matched_text\n";
    for (const auto& f : findings) {
        ss << f.rule_id << "," << f.category << "," << f.severity << "," << f.confidence << ",\"" << f.matched_text << "\"\n";
    }
    return ss.str();
}

std::string Reporter::toMarkdown(const std::vector<Finding>& findings) {
    std::stringstream ss;
    ss << "# QuantaAlarma Findings Report\n\n";
    ss << "| Rule ID | Category | Severity | Confidence | Matched Text |\n";
    ss << "|--- |--- |--- |--- |--- |\n";
    for (const auto& f : findings) {
        ss << "| " << f.rule_id << " | " << f.category << " | " << f.severity << " | " << f.confidence << " | " << f.matched_text << " |\n";
    }
    return ss.str();
}

std::string Reporter::toPlainText(const std::vector<Finding>& findings) {
    std::stringstream ss;
    for (const auto& f : findings) {
        ss << "[" << f.rule_id << "] (" << f.category << ") Severity: " << f.severity << " - " << f.matched_text << "\n";
    }
    return ss.str();
}

} // namespace quanta
