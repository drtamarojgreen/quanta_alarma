# Rule Authoring Guide

To add a new rule to QuantaAlarma:

1. Identify the pattern you want to detect.
2. Determine the category (e.g., safety, security, privacy, reliability, policy).
3. Assign a default severity (1-10).
4. Write a concise rationale and remediation hint.

## Example

```cpp
addRule({"R041", "security", "eval\\(input\\)", 10, "Direct eval of input is highly dangerous.", "Use a safe parser instead."});
```

New rules can be added in `AnalysisEngine::loadDefaultRules()` or via a configuration file (to be fully implemented).
