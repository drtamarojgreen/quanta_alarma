# QuantaAlarma System Structural Restrictions

## General
- REQUIRE all Phase III enhancements to be implemented in C++.
- REQUIRE explicit verification of all empirical outputs.
- FORBID the use of raw pointers (`new`/`delete`) and empty catch blocks.

## SDD Standards
- REQUIRE a 10-point penalty for each SDD test card using "lazy" binary logic (1/0) instead of descriptive numeric metrics (e.g., scores 0-100).
- FORBID "fallback logic" in source code, requiring robust error propagation instead.

## Architectural
- REQUIRE modules to be organized into logical packages: `analysis`, `scoring`, `alerting`, `app`.
- LIMIT dependencies to standard library features where possible.
