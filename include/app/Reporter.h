#ifndef REPORTER_H
#define REPORTER_H

#include "analysis/AnalysisEngine.h"
#include <string>
#include <vector>

namespace quanta {

class Reporter {
public:
    static std::string escapeJSON(const std::string& s);
    static std::string escapeCSV(const std::string& s);
    static std::string toJSON(const std::vector<Finding>& findings, bool redact = false); // Item 128
    static std::string toCSV(const std::vector<Finding>& findings, bool redact = false);
    static std::string toMarkdown(const std::vector<Finding>& findings, bool redact = false);
    static std::string toPlainText(const std::vector<Finding>& findings, bool redact = false);

    static std::string toSummaryOnly(const std::vector<Finding>& findings); // Item 125
    static std::string toFindingsOnly(const std::vector<Finding>& findings, const std::string& format); // Item 126

    // Filtering and ordering (Items 136-139)
    static std::vector<Finding> filterFindings(const std::vector<Finding>& findings, int min_severity = 0, const std::string& category = "", const std::string& path = "");
    static void sortFindings(std::vector<Finding>& findings); // Deterministic ordering (Item 139)
};

} // namespace quanta

#endif // REPORTER_H
