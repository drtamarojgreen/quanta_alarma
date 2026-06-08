# Rule Authoring Guide

## How to add a new rule

1. Open `src/analysis/AnalysisEngine.cpp`.
2. Locate the `loadDefaultRules()` method.
3. Add a new `addRule` call with the following structure:
   ```cpp
   addRule({
       "RULE_ID",       // Unique identifier
       "category",      // safety, security, privacy, reliability, policy
       "regex_pattern", // Case-insensitive regex
       severity_int,    // 1-10
       confidence_dbl,  // 0.0-1.0 base confidence
       "rationale",     // Why this rule exists
       "remediation",   // How to fix the finding
       "owner-team",    // Responsible team
       false,           // deprecated (bool)
       "false_pos",     // Known false positive patterns
       "false_neg",     // Known detection gaps
       {"changelog"}    // History of changes
   });
   ```

## Best Practices
- Keep regex patterns simple to avoid performance degradation.
- Use specific categories to allow for accurate risk scoring and filtering.
- Provide actionable remediation hints.
