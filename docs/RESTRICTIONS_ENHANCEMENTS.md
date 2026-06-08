# Enhancement Implementation Restrictions

## General
- FORBID "minimal" or "demo" implementations for any enhancement item (e.g., checksums, metrics).
- FORBID unused data members in `Finding` or `Rule` structs; every field must be populated and used.
- FORBID arbitrary numeric constants in logic (e.g., hardcoded confidence scores).

## Verification
- REQUIRE all numeric evidence in `sorrel_checkouts.md` to be derived from real execution output.
- REQUIRE all filtering logic to be exercisable via CLI flags.

## Documentation
- FORBID placeholder text in new documentation files; all guides must reflect actual system behavior.
