# QuantaAlarma v1.1.0: Enhancement Implementation Restrictions

## Engineering Rigor
- FORBID "chatbot-style" generation; all implementations must be valid engineering systems, not plausible language.
- FORBID "minimal" or "demo" implementations for any enhancement item. Examples: checksums must be deterministic and exhaustive; metrics must be dynamically calculated.
- FORBID unused data members in any struct. If a field exists, it must be integrated into the logic and reporting pipelines.
- FORBID arbitrary numeric constants in logic. Confidence scores and risk multipliers must be derived from structured data or statistical match analysis.
- REQUIRE all rules to have a descriptive, human-readable name; cryptic IDs must not be the primary user-facing identifier.

## Operational Integrity
- REQUIRE robust data escaping (JSON/CSV) for all matched text to ensure report parseability.
- REQUIRE strict CLI flag precedence: specific overrides (e.g., --threshold) MUST take precedence over profile defaults.
- FORBID binary artifacts or empty executable stubs in the repository index.

## SDD Verification
- REQUIRE all numeric evidence in `sorrel_checkouts.md` to be derived from real execution results (e.g., `coverage_percent = 100`).
- REQUIRE all filtering logic (severity, category, path) to be exercisable via CLI flags and validated via SDD cards.

## Documentation
- FORBID placeholder text, TODOs, or implementation templates in documentation.
- REQUIRE documentation to reflect the actual technical state and provide concrete examples of the system's usage.
