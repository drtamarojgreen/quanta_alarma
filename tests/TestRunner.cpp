#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "RiskScorer.h"

SCENARIO("Risk scores are calculated based on text content", "[riskscorer]") {
    GIVEN("A RiskScorer") {
        RiskScorer scorer;

        WHEN("the text contains the word 'danger'") {
            int risk = scorer.calculateRisk("this is a dangerous message");
            THEN("the risk score is 100") {
                REQUIRE(risk == 100);
            }
        }

        WHEN("the text does not contain the word 'danger'") {
            int risk = scorer.calculateRisk("this is a safe message");
            THEN("the risk score is 0") {
                REQUIRE(risk == 0);
            }
        }
    }
}
