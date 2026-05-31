#!/bin/bash
set -e
# Navigate to the root of the repository
cd "$(dirname "$0")/../.."

mkdir -p build
g++ -Iinclude -Isrc -Itests/sdd -Itests/sdd/cpp/util tests/sdd/cards/VerificationClass.cpp \
    src/analysis/AnalysisEngine.cpp \
    src/scoring/RiskScorer.cpp \
    -o build/sorrel_test

./build/sorrel_test
echo "SDD SUITE PASSED"
