#include <iostream>

#include "AnalysisEngine.h"

int main() {
    std::cout << "Running AnalysisEngine tests..." << std::endl;

    AnalysisEngine engine;
    engine.analyze("test payload");

    std::cout << "All AnalysisEngine tests passed." << std::endl;
    return 0;
}
