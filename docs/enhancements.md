# Future Enhancements for QuantaAlarma

This document outlines potential future enhancements for QuantaAlarma that can be implemented without adding any new third-party libraries or frameworks. Each item should rely on the existing runtime, standard library features, current project conventions, configuration files, documentation, tests, and lightweight scripts already available in the repository.

## Core Detection and Analysis

1. **Rule severity tuning:** Add configuration fields that let users adjust severity levels for existing rules.
2. **Rule confidence scoring:** Include a simple numeric confidence score in each finding based on existing rule matches.
3. **Rule rationale messages:** Expand rule output with concise explanations that describe why a finding was raised.
4. **Rule remediation hints:** Add short remediation suggestions for common findings using static text.
5. **Rule category labels:** Tag findings with categories such as safety, reliability, privacy, or policy.
6. **Rule owner metadata:** Add optional owner fields to rules so teams know who maintains each rule.
7. **Rule deprecation markers:** Let maintainers mark rules as deprecated while keeping backward compatibility.
8. **Rule changelog entries:** Document notable rule behavior changes alongside rule definitions.
9. **Rule test fixtures:** Add more fixture files that exercise expected rule matches and non-matches.
10. **Rule false-positive notes:** Document known false-positive patterns and recommended mitigations.
11. **Rule false-negative notes:** Document known detection gaps so users understand current limitations.
12. **Rule grouping:** Group related findings in output when they share the same source and category.
13. **Rule ordering:** Sort findings by severity, confidence, and source position for easier review.
14. **Rule suppression comments:** Support inline suppression markers using existing parser logic.
15. **Rule suppression reasons:** Require optional human-readable reasons for suppression comments.
16. **Suppression expiration dates:** Allow suppressions to include an expiration date checked with standard date parsing.
17. **Suppression audit output:** Emit a summary of active suppressions during scans.
18. **Duplicate finding collapse:** Collapse identical findings detected in the same scan run.
19. **Near-duplicate finding notes:** Add simple normalization to identify findings that only differ by whitespace.
20. **Risk keyword expansion:** Broaden existing keyword lists for common risky agent behaviors.
21. **Prompt-injection pattern list:** Add plain-text patterns for prompt-injection indicators.
22. **Data-leakage pattern list:** Add static patterns for common accidental secret or PII disclosure signals.
23. **Tool-misuse pattern list:** Add patterns that flag risky tool invocation text.
24. **Unsafe delegation pattern list:** Add patterns that detect agents delegating restricted actions.
25. **Over-permission pattern list:** Flag text suggesting broader permissions than the task needs.
26. **Ambiguous instruction detector:** Detect language that may create unclear agent behavior.
27. **Missing consent detector:** Flag workflows that imply user-impacting actions without confirmation.
28. **Missing audit detector:** Flag high-risk actions that lack logging or traceability notes.
29. **Unbounded loop detector:** Identify repeated agent steps that indicate possible runaway behavior.
30. **Contradictory instruction detector:** Flag nearby instructions that appear to conflict.
31. **Sensitive action detector:** Identify file deletion, credential use, payment, or deployment language.
32. **External communication detector:** Flag prompts that ask agents to contact external parties.
33. **Hidden instruction detector:** Detect phrases that imply concealed behavior or invisible rules.
34. **Policy override detector:** Flag attempts to ignore, bypass, or rewrite safety policies.
35. **Sandbox escape detector:** Detect text suggesting attempts to bypass runtime boundaries.
36. **Credential request detector:** Flag requests for passwords, tokens, private keys, or session cookies.
37. **Authorization assumption detector:** Flag text that assumes permission without evidence.
38. **Irreversible action detector:** Highlight destructive or hard-to-reverse operations.
39. **Human handoff detector:** Identify cases where policy recommends review by a human.
40. **Low-evidence conclusion detector:** Flag confident conclusions that lack cited evidence.

## Configuration and Policy Management

41. **Example minimal policy:** Add a small sample policy focused on core checks.
42. **Example strict policy:** Add a sample policy for high-assurance environments.
43. **Example development policy:** Add a permissive sample policy for local experimentation.
44. **Configuration schema comments:** Expand comments in sample configuration files.
45. **Configuration defaults table:** Document each default value in a readable table.
46. **Configuration validation messages:** Make validation errors more specific and actionable.
47. **Unknown key warnings:** Warn users when configuration contains unrecognized keys.
48. **Deprecated key warnings:** Warn users before removing support for old configuration keys.
49. **Configuration precedence docs:** Document the order in which defaults, files, and flags are applied.
50. **Configuration dry-run mode:** Add a mode that loads configuration and exits after validation.
51. **Policy naming guidelines:** Document recommended naming conventions for policies.
52. **Policy version field:** Add an optional version field to policy files.
53. **Policy description field:** Add optional descriptions so policies are self-documenting.
54. **Policy owner field:** Add an optional owner or team field for operational accountability.
55. **Policy review date field:** Add an optional review date to remind teams to revisit policies.
56. **Policy migration notes:** Document how to update policies when rule behavior changes.
57. **Policy comments examples:** Add examples that show how to annotate policy intent.
58. **Policy lint command:** Add a built-in check that validates policy formatting and references.
59. **Policy diff guidance:** Document how to review policy changes safely in code review.
60. **Policy template placeholders:** Provide copyable templates with placeholder values.
61. **Environment variable docs:** Document supported environment variables in one section.
62. **Environment variable validation:** Warn when environment variable values are malformed.
63. **Relative path handling docs:** Explain how relative paths are resolved in configuration.
64. **Glob pattern examples:** Add examples for include and exclude patterns already supported.
65. **Configuration troubleshooting section:** Add common configuration mistakes and fixes.
66. **Configuration summary output:** Print effective configuration in a compact form when requested.
67. **Safe default profile:** Add a built-in profile that favors conservative detection settings.
68. **Quiet profile:** Add a profile that reduces non-essential output for automation.
69. **Verbose profile:** Add a profile that exposes detailed reasoning for debugging.
70. **Baseline profile:** Add a profile that helps teams introduce checks gradually.
71. **Policy compatibility check:** Report when a policy references missing or renamed rules.
72. **Policy duplicate detection:** Warn when the same rule or path appears multiple times.
73. **Policy sorting recommendation:** Recommend deterministic ordering for policy entries.
74. **Policy examples index:** Add an index that maps user goals to sample policies.
75. **Policy field glossary:** Define each policy term in plain language.
76. **Configuration checksum output:** Print a simple checksum of the effective configuration for audit logs.
77. **Configuration source tracing:** Show which file or flag provided each effective value.
78. **Policy comment preservation:** Preserve comments when rewriting policy examples, where supported.
79. **Configuration sanity checklist:** Add a checklist for production readiness.
80. **Policy review workflow:** Document a lightweight review workflow for policy updates.

## Command-Line and Automation

81. **Machine-readable summary:** Add a compact summary block that scripts can parse easily.
82. **Exit-code documentation:** Document every exit code and when it is returned.
83. **No-color output flag:** Add an option to disable ANSI color for logs and CI output.
84. **Force-color output flag:** Add an option to force ANSI color in supported terminals.
85. **Output width option:** Let users control wrapping width for terminal output.
86. **Stable sort option:** Add deterministic output sorting for reproducible CI results.
87. **Scan timing summary:** Print elapsed time for major scan phases.
88. **File count summary:** Print counts of scanned, skipped, and excluded files.
89. **Finding count summary:** Print counts grouped by severity and category.
90. **Quiet mode:** Reduce output to only findings and final status.
91. **Verbose mode:** Include rule identifiers, source snippets, and configuration notes.
92. **Debug mode:** Print additional internal state helpful for maintainers.
93. **Version command:** Add a command that prints version and build metadata.
94. **List rules command:** Add a command that lists available rules and descriptions.
95. **Explain rule command:** Add a command that explains one rule by identifier.
96. **List profiles command:** Add a command that lists built-in configuration profiles.
97. **Validate config command:** Add a command dedicated to configuration validation.
98. **Validate policy command:** Add a command dedicated to policy validation.
99. **Baseline creation command:** Add a command that records current findings for later comparison.
100. **Baseline comparison command:** Add a command that reports only new findings.
101. **Changed-files scan mode:** Document and support scanning a provided list of changed files.
102. **Standard input scan mode:** Support reading content from standard input for quick checks.
103. **Path echo option:** Include normalized paths in output to simplify CI annotations.
104. **Relative path option:** Print paths relative to the current working directory.
105. **Absolute path option:** Print absolute paths for editor integrations.
106. **Failure threshold option:** Let users fail only when findings meet or exceed a severity threshold.
107. **Warning threshold option:** Let users classify lower-severity findings as warnings.
108. **Maximum findings option:** Stop after a configurable number of findings to reduce noisy output.
109. **Include hidden files option:** Let users explicitly include hidden files in scans.
110. **Exclude generated files option:** Add documented patterns for common generated output.
111. **Progress output:** Print periodic progress for long scans without adding dependencies.
112. **Progress disable flag:** Let automation disable progress output fully.
113. **CI mode:** Add an option that chooses deterministic, non-interactive output defaults.
114. **Local mode:** Add an option that favors developer-friendly console output.
115. **Command examples:** Add more copyable examples for common terminal workflows.
116. **Shell completion docs:** Document simple completion snippets if the project already exposes commands.
117. **Makefile targets:** Add convenience targets that wrap common validation commands if a Makefile exists.
118. **Script examples:** Add small standard-library scripts for batch scanning examples.
119. **Pre-commit usage docs:** Document how to call the existing CLI from a pre-commit hook without new packages.
120. **CI usage docs:** Add generic CI snippets that call existing commands directly.

## Output, Reporting, and Auditability

121. **Plain-text report:** Add a human-readable report format that requires no rendering tools.
122. **JSON report:** Add a structured report using the language's standard JSON support.
123. **CSV report:** Add a simple CSV report for spreadsheets using standard string formatting.
124. **Markdown report:** Add a Markdown report suitable for issue comments or pull requests.
125. **Summary-only report:** Add an output format that contains only aggregate counts.
126. **Findings-only report:** Add an output format that omits scan metadata.
127. **Audit metadata block:** Include timestamp, command, version, and configuration checksum in reports.
128. **Redacted report mode:** Mask sensitive substrings before writing reports.
129. **Source snippet trimming:** Limit snippets to a safe number of characters around each finding.
130. **Source snippet line numbers:** Include line and column numbers for findings when available.
131. **Report file overwrite warning:** Warn before overwriting an existing report unless forced.
132. **Report directory creation:** Create missing output directories using standard filesystem APIs.
133. **Report filename templates:** Support simple timestamp placeholders in report filenames.
134. **Report footer:** Add footer text with documentation links and support guidance.
135. **Report glossary:** Include definitions for severity, confidence, and category.
136. **Report filter by severity:** Let users generate reports containing selected severities only.
137. **Report filter by category:** Let users generate reports containing selected categories only.
138. **Report filter by path:** Let users generate reports for selected paths only.
139. **Report deterministic ordering:** Ensure reports are stable across repeated runs.
140. **Report comparison notes:** Add guidance for comparing two report files with standard diff tools.
141. **Audit trail docs:** Document how to archive reports for compliance reviews.
142. **Finding fingerprint:** Add a simple deterministic fingerprint for each finding.
143. **Finding first-seen date:** Store first-seen dates in baseline files where applicable.
144. **Finding status field:** Add status values such as new, existing, suppressed, or resolved.
145. **Finding evidence field:** Separate matched evidence from explanatory text in output.
146. **Finding source field:** Identify whether the finding came from a rule, configuration check, or parser check.
147. **Finding stable identifier:** Document a stable identifier format for findings.
148. **Report size summary:** Include report byte size after writing output.
149. **Report path summary:** Print the final report path after successful generation.
150. **Report retention guidance:** Document suggested retention periods for different environments.
151. **Report privacy checklist:** Add a checklist for safely sharing reports externally.
152. **Report redaction examples:** Show before-and-after examples of redacted output.
153. **Report validation command:** Add a command that validates generated JSON reports.
154. **Report schema docs:** Document report fields in a table without introducing schema tooling.
155. **Report changelog:** Track report format changes in documentation.
156. **Report compatibility notes:** Explain how consumers should handle unknown report fields.
157. **Report grouping by file:** Group findings by file for easier manual triage.
158. **Report grouping by rule:** Group findings by rule for rule-tuning reviews.
159. **Report grouping by owner:** Group findings by configured owner metadata.
160. **Report top risks section:** Highlight the highest-severity findings at the top of reports.

## Developer Experience, Tests, and Documentation

161. **Architecture overview:** Add a concise diagram-free architecture overview using Markdown lists.
162. **Module responsibility docs:** Document what each major module owns.
163. **Data flow walkthrough:** Describe how input becomes findings and reports.
164. **Rule authoring guide:** Add a step-by-step guide for writing a new rule.
165. **Rule review checklist:** Add a checklist for reviewing rule changes.
166. **Test fixture guide:** Document how to add and name fixtures.
167. **Regression test checklist:** Document which tests to update for bug fixes.
168. **Manual test checklist:** Add manual checks maintainers can run before releases.
169. **Troubleshooting guide:** Add common failures, likely causes, and fixes.
170. **FAQ:** Add answers for common setup, configuration, and output questions.
171. **Glossary:** Define project-specific terms used in logs and reports.
172. **Release checklist:** Add a release checklist using existing project commands.
173. **Upgrade guide:** Document how users should move between versions.
174. **Backward compatibility policy:** Document what users can expect to remain stable.
175. **Contribution guide expansion:** Clarify preferred commit, test, and documentation practices.
176. **Issue template text:** Add copyable text for high-quality bug reports.
177. **Security reporting docs:** Document how to report potential vulnerabilities privately.
178. **Maintainer triage guide:** Document labels, priority meanings, and escalation paths.
179. **Code comments for complex logic:** Add focused comments where behavior is non-obvious.
180. **Inline examples in docs:** Add short examples near each documented option.
181. **Examples directory index:** Add an index that describes every example file.
182. **End-to-end example:** Add a small walkthrough from configuration to report review.
183. **Minimal reproduction guide:** Document how to create a small reproduction for a bug.
184. **Performance troubleshooting docs:** Explain how to reduce scan scope and output volume.
185. **Large repository guidance:** Document scanning strategies for large repositories.
186. **Monorepo guidance:** Document path and policy organization for monorepos.
187. **Documentation lint checklist:** Add a checklist for headings, links, examples, and terminology.
188. **Link maintenance checklist:** Add periodic checks for internal documentation links.
189. **Terminology consistency pass:** Standardize terms such as finding, rule, policy, and report.
190. **Error message style guide:** Document conventions for actionable error messages.
191. **Logging style guide:** Document conventions for concise and useful logs.
192. **Testing matrix docs:** Document supported runtime versions and operating systems.
193. **Platform notes:** Add notes for Linux, macOS, and Windows shell differences where relevant.
194. **Editor integration notes:** Document how editors can invoke the existing CLI.
195. **Code review examples:** Add examples of useful review comments for rule changes.
196. **Dependency-free principle:** Document the preference for standard-library solutions when practical.
197. **Risk taxonomy docs:** Define the project's risk categories and severity levels.
198. **Operational readiness checklist:** Add a checklist for teams adopting the tool in production.
199. **Adoption roadmap:** Add a staged plan for enabling checks across a team.
200. **Enhancement tracking table:** Add a lightweight table for status, owner, and target release of accepted enhancements.

### Enhancement Tracking Table (Item 200)

| Item | Status | Owner | Target Release |
| --- | --- | --- | --- |
| 1-40 | Functionally Complete | Precision Power | 1.1.0 |
| 41-80 | Functionally Complete | Precision Power | 1.1.0 |
| 81-120 | Functionally Complete | Precision Power | 1.1.0 |
| 121-160 | Functionally Complete | Precision Power | 1.1.0 |
| 161-200 | Documentation Complete | Precision Power | 1.1.0 |
