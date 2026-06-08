# QuantaAlarma Architecture Overview

## Module Responsibilities (Item 162)
- **AnalysisEngine**: Pattern matching and initial finding generation.
- **RiskScorer**: Scoring findings based on severity and weights.
- **AlertingSystem**: Integration with external notification services.
- **ConfigManager**: Loading and validating policy files.
- **Reporter**: Formatting findings for output.

## Data Flow (Item 163)
1. **Input**: Text received via CLI or stdin.
2. **Analysis**: `AnalysisEngine` scans text against regex rules.
3. **Scoring**: `RiskScorer` calculates a normalized score (0-100).
4. **Output**: `Reporter` generates reports in JSON, CSV, MD, or text.
