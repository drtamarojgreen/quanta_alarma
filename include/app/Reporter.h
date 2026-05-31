#ifndef REPORTER_H
#define REPORTER_H

#include "analysis/AnalysisEngine.h"
#include <string>
#include <vector>

namespace quanta {

class Reporter {
public:
    static std::string toJSON(const std::vector<Finding>& findings);
    static std::string toCSV(const std::vector<Finding>& findings);
    static std::string toMarkdown(const std::vector<Finding>& findings);
    static std::string toPlainText(const std::vector<Finding>& findings);
};

} // namespace quanta

#endif // REPORTER_H
