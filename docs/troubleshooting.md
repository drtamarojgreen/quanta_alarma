# Troubleshooting Guide

## Common Issues (Item 169)
- **Regex Error**: Ensure patterns follow C++ std::regex (ECMAScript) syntax.
- **Config Not Loading**: Check file paths and ensure the `=` separator is used.
- **High Memory Usage**: Reduce the number of rules or text size in a single scan.

# FAQ (Item 170)
**Q: How do I add a new rule?**
A: Use `addRule` in C++ or define it in a custom policy file (future enhancement).

**Q: What does risk score mean?**
A: A value from 0-100 indicating the severity of the findings.
