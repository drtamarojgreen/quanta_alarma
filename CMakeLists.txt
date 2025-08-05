cmake_minimum_required(VERSION 3.10)
project(QuantaAlarma)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files for the main library
set(QUANTA_ALARMA_SOURCES
    src/AnalysisEngine.cpp
    src/RiskScorer.cpp
    src/AlertingSystem.cpp
)

# Add executable
add_executable(QuantaAlarma src/main.cpp ${QUANTA_ALARMA_SOURCES})
target_include_directories(QuantaAlarma PUBLIC include)


# Add tests
enable_testing()

# Include directories for tests
include_directories(lib)

# Test source files
set(QUANTA_ALARMA_TEST_SOURCES
    tests/TestRunner.cpp
    tests/AnalysisEngineTests.cpp
)

# Add test executable
add_executable(QuantaAlarmaTests ${QUANTA_ALARMA_TEST_SOURCES} ${QUANTA_ALARMA_SOURCES})
target_include_directories(QuantaAlarmaTests PUBLIC include)


# Add the test to CTest
add_test(NAME QuantaAlarmaTests COMMAND QuantaAlarmaTests)
