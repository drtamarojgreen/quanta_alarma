# QuantaAlarma Architecture Overview

QuantaAlarma is designed as a modular pipeline for analyzing LLM agent outputs.

## Modules

- **AnalysisEngine (`quanta::AnalysisEngine`)**: Responsible for scanning text against a set of rules. Uses regex for pattern matching.
- **RiskScorer (`quanta::RiskScorer`)**: Calculates a numeric risk score (0-100) based on findings from the AnalysisEngine.
- **AlertingSystem (`quanta::AlertingSystem`)**: Triggers notifications or alerts based on the calculated risk score.
- **ConfigManager (`quanta::ConfigManager`)**: Loads policies and configuration from files.
- **Reporter (`quanta::Reporter`)**: Formats findings into various machine-readable and human-readable formats (JSON, CSV, Markdown).

## Data Flow

1. Input text is received via CLI.
2. `ConfigManager` loads any specified policy.
3. `AnalysisEngine` scans the text and produces a list of `Finding` objects.
4. `RiskScorer` processes the `Finding` objects to produce a `riskScore`.
5. `Reporter` outputs the findings in the requested format.
6. `AlertingSystem` triggers an alert if the `riskScore` exceeds the threshold.
7. The application exits with a non-zero code if the threshold is met or exceeded.
