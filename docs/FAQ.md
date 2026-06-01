# QuantaAlarma FAQ

## Q: How do I suppress a finding?
A: You can use an inline comment: `quanta-suppress R021 reason="Safe usage"`.

## Q: What are the severity levels?
A: Severity ranges from 1 (lowest) to 10 (highest). High severity findings significantly impact the risk score.

## Q: How can I use this in CI?
A: Use the `--format json` flag and check the exit code. A non-zero exit code indicates the risk score exceeded the threshold.

## Q: How do I list all available rules?
A: Run `quanta_alarma --list-rules`.

## Q: How do I load a custom policy?
A: Use the `--config <path>` option. See `docs/example_strict_policy.conf` for a template.
