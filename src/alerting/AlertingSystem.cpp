#include "alerting/AlertingSystem.h"

#include <iostream>

AlertingSystem::AlertingSystem() {}

void AlertingSystem::triggerAlert(int riskScore) {
    if (riskScore > 50) {
        std::cout << "ALERT! High risk score detected: " << riskScore << std::endl;
    }
}
