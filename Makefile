CXX      := g++
CXXFLAGS := -Wall -std=c++17 -g -Iinclude

SRC_FILES := \
	src/analysis/AnalysisEngine.cpp \
	src/scoring/RiskScorer.cpp \
	src/alerting/AlertingSystem.cpp \
	src/app/ConfigManager.cpp \
	src/app/Reporter.cpp

TEST_FILES := \
	tests/UnitTests.cpp \
	tests/BddTests.cpp \
	tests/TestRunner.cpp \
	tests/AnalysisEngineTests.cpp

APP_MAIN := src/app/main.cpp
APP_BIN := quanta_alarma

.PHONY: all app test sdd clean

all: app test sdd

app: $(APP_BIN)

$(APP_BIN): $(SRC_FILES) $(APP_MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: unit_test bdd_test runner_test engine_test
	./unit_test
	./bdd_test
	./runner_test
	./engine_test

unit_test: $(SRC_FILES) tests/UnitTests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

bdd_test: $(SRC_FILES) tests/BddTests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

runner_test: $(SRC_FILES) tests/TestRunner.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

engine_test: $(SRC_FILES) tests/AnalysisEngineTests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

sdd:
	./tests/sdd/run_sdd_suite.sh

# Item 117: Convenience targets
check_rules: app
	./quanta_alarma --list-rules

check_profiles: app
	./quanta_alarma --list-profiles

validate_config: app
	./quanta_alarma --dry-run "test"

clean:
	rm -f unit_test bdd_test runner_test engine_test $(APP_BIN)
	rm -rf build
