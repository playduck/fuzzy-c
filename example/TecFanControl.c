/**
 * @file TecFanControl.c
 * @brief This C program implements a fuzzy logic controller that determines the
 * desired fan speed based on four input parameters: current temperature,
 * current temperature change, current TEC power, and current fan state.
 * The program uses fuzzy logic rules to evaluate the inputs and produce a fuzzy
 * output, which is then defuzzified to produce a crisp fan speed value.
 * The program takes the input values from the command line and prints the
 * output fan speed as a percentage.
 * @author Robin prillwitz
 * @date 2024
 *
 *    Copyright 2024 Robin Prillwitz
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *      or view LICENSE.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 +  See the License for the specific language governing permissions and
 *  limitations under the License.
*/

#include "fuzzyc.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Define the labels for the fuzzy sets (only used for debugging)
const char *lmhLabels[] = {"Low", "Medium", "High"};
const char *changeLabels[] = {"Dec", "Stable", "Inc"};
const char *fanLabels[] = {"Off", "On"};
const char *fanSpeedLabels[] = {"Off", "Slow", "Medium", "Fast"};

// Define the input fuzzy sets
FuzzySet TemperatureState;
FuzzySet TempChangeState;
FuzzySet TECPowerState;
FuzzySet FanState;

// Define the output fuzzy set
FuzzySet FanSpeed;

// Define the membership functions for the fuzzy sets
#define TemperatureMembershipFunctions(X)                                      \
    X(TEMPERATURE_LOW, -20.0, -20.0, 18.0, 25.0, TRAPEZOIDAL)                  \
    X(TEMPERATURE_MEDIUM, 18.0, 23.0, 35.0, 0.0, TRIANGULAR)                   \
    X(TEMPERATURE_HIGH, 23.0, 35.0, 100.0, 100.0, TRAPEZOIDAL)
DEFINE_FUZZY_MEMBERSHIP(TemperatureMembershipFunctions)

#define TempChangeMembershipFunctions(X)                                       \
    X(TEMP_CHANGE_DECREASING, -20.0, -20.0, -2.0, 0.0, TRAPEZOIDAL)            \
    X(TEMP_CHANGE_STABLE, -2.0, 0.0, 2.0, 0.0, TRIANGULAR)                     \
    X(TEMP_CHANGE_INCREASING, 0.0, 2.0, 20.0, 20.0, TRAPEZOIDAL)
DEFINE_FUZZY_MEMBERSHIP(TempChangeMembershipFunctions)

#define TECPowerMembershipFunctions(X)                                         \
    X(TEC_POWER_LOW, -5.0, -5.0, 3.0, 15.0, TRAPEZOIDAL)                       \
    X(TEC_POWER_MEDIUM, 3.0, 10.0, 25.0, 25.0, TRIANGULAR)                     \
    X(TEC_POWER_HIGH, 15.0, 25.0, 100.0, 100.0, TRAPEZOIDAL)
DEFINE_FUZZY_MEMBERSHIP(TECPowerMembershipFunctions)

#define FanStateMembershipFunctions(X)                                         \
    X(FAN_STATE_OFF, 0.0, 20.0, 0.0, 0.0, RECTANGULAR)                         \
    X(FAN_STATE_ON, 20.0, 101.0, 0.0, 0.0, RECTANGULAR)
DEFINE_FUZZY_MEMBERSHIP(FanStateMembershipFunctions)

#define FanSpeedMembershipFunctions(X)                                         \
    X(FAN_SPEED_OFF, -20.0, 20.0, 0.0, 0.0, RECTANGULAR)                       \
    X(FAN_SPEED_SLOW, 20.0, 20.0, 40.0, 60.0, TRAPEZOIDAL)                     \
    X(FAN_SPEED_MEDIUM, 30.0, 60.0, 60.0, 65.0, TRAPEZOIDAL)                   \
    X(FAN_SPEED_FAST, 60.0, 65.0, 100.0, 100.0, TRAPEZOIDAL)
DEFINE_FUZZY_MEMBERSHIP(FanSpeedMembershipFunctions)

// Define the fuzzy rulesQ
FuzzyRule rules[] = {
    // Rule 1: Turn on the fan at high speed when it's off and the temperature
    // is high or the TEC heat load is high
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_OFF)),
                     ANY_OF(VAR(TemperatureState, TEMPERATURE_MEDIUM),
                            VAR(TemperatureState, TEMPERATURE_HIGH),
                            VAR(TECPowerState, TEC_POWER_HIGH))),
                THEN(FanSpeed, FAN_SPEED_FAST)),

    // Rule 2: Keep the fan off when it's already off and the temperature is low
    // and stable or decreasing
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_OFF),
                            VAR(TemperatureState, TEMPERATURE_LOW)),
                     ANY_OF(VAR(TempChangeState, TEMP_CHANGE_STABLE),
                            VAR(TempChangeState, TEMP_CHANGE_DECREASING))),
                THEN(FanSpeed, FAN_SPEED_OFF)),

    // Rule 3: Turn off the fan when it's on and the TEC power is low, and the
    // temperature is stable or decreasing
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_LOW)),
                     ANY_OF(VAR(TempChangeState, TEMP_CHANGE_STABLE),
                            VAR(TempChangeState, TEMP_CHANGE_DECREASING))),
                THEN(FanSpeed, FAN_SPEED_OFF)),

    // Rule 4: Set the fan speed to medium when it's on and the temperature is
    // medium, but the TEC power is not high
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TemperatureState, TEMPERATURE_MEDIUM),
                            NOT(TECPowerState, TEC_POWER_HIGH))),
                THEN(FanSpeed, FAN_SPEED_MEDIUM)),

    // Rule 5: Turn on the fan at high speed when it's on and the temperature is
    // high, and the TEC power is not low
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TemperatureState, TEMPERATURE_HIGH)),
                     ANY_OF(VAR(TECPowerState, TEC_POWER_MEDIUM),
                            VAR(TECPowerState, TEC_POWER_LOW))),
                THEN(FanSpeed, FAN_SPEED_FAST)),

    // Rule 6: Turn off the fan when it's on, the TEC power is low, and the
    // temperature is low
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_LOW),
                            VAR(TemperatureState, TEMPERATURE_LOW))),
                THEN(FanSpeed, FAN_SPEED_OFF)),

    // Rule 7: Set the fan speed to medium when it's on and the TEC power is
    // medium
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_MEDIUM))),
                THEN(FanSpeed, FAN_SPEED_MEDIUM)),

    // Rule 8: Turn on the fan at high speed when it's on and the TEC power is
    // high
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_HIGH))),
                THEN(FanSpeed, FAN_SPEED_FAST)),
};

// Helper function to create the fuzzy classifiers
void createClassifiers() {
    FuzzySetInit(&TemperatureState, TemperatureMembershipFunctions,
                 FUZZY_LENGTH(TemperatureMembershipFunctions));
    FuzzySetInit(&TempChangeState, TempChangeMembershipFunctions,
                 FUZZY_LENGTH(TempChangeMembershipFunctions));
    FuzzySetInit(&TECPowerState, TECPowerMembershipFunctions,
                 FUZZY_LENGTH(TECPowerMembershipFunctions));
    FuzzySetInit(&FanState, FanStateMembershipFunctions,
                 FUZZY_LENGTH(FanStateMembershipFunctions));
    FuzzySetInit(&FanSpeed, FanSpeedMembershipFunctions,
                 FUZZY_LENGTH(FanSpeedMembershipFunctions));
}

// Helper function to destroy the fuzzy classifiers
void destroyClassifiers() {
    FuzzySetFree(&TemperatureState);
    FuzzySetFree(&TempChangeState);
    FuzzySetFree(&TECPowerState);
    FuzzySetFree(&FanState);
    FuzzySetFree(&FanSpeed);
}

// Helper function to map a value from one range to another
double map_range(double value, double in_min, double in_max, double out_min,
                 double out_max) {
    double mapped =
        (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return fmin(fmax(mapped, out_min), out_max);
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command line arguments are provided
    if (argc != 5) {
        printf("Usage: %s <currentTemperature> <currentTemperatureChange> "
               "<currentTECPower> <currentFan>\n",
               argv[0]);
        return 1;
    }

    // Convert command line arguments to double
    double currentTemperature = atof(argv[1]);
    double currentTemperatureChange = atof(argv[2]);
    double currentTECPower = atof(argv[3]);
    double currentFan = atof(argv[4]);

    // Allocate memory
    createClassifiers();

    // Classify the inputs
    FuzzyClassifier(currentTemperature, &TemperatureState);
    FuzzyClassifier(currentTemperatureChange, &TempChangeState);
    FuzzyClassifier(currentTECPower, &TECPowerState);
    FuzzyClassifier(currentFan, &FanState);

    // Print the input class memberships
    printf("Temperature %.04f degC\n", currentTemperature);
    printClassifier(&TemperatureState, lmhLabels);
    printf("Temp Change %.04f degC/sec\n", currentTemperatureChange);
    printClassifier(&TempChangeState, changeLabels);
    printf("TEC Power %.04f W\n", currentTECPower);
    printClassifier(&TECPowerState, lmhLabels);
    printf("Fan State %.04f %%\n", currentFan);
    printClassifier(&FanState, fanLabels);

    // Perform fuzzy inference
    fuzzyInference(rules, (sizeof(rules) / sizeof(rules[0])));

    // Print the output class membership
    printf("Fan Speed\n");
    printClassifier(&FanSpeed, fanSpeedLabels);

    // Defuzzify the output
    double fanSpeed = defuzzification(&FanSpeed);
    if (fanSpeed <= 20.0) {
        // fan won't run below 20, so it's effectively off
        fanSpeed = 0.0;
    } else {
        // extreme points (0, 100) can't be reached due to the centroid
        // calculation
        fanSpeed = map_range(fanSpeed, 10.0, 80.0, 30.0, 100.0);
    }

    // Print the result
    printf("Fan Speed: %.04f %%\n", fanSpeed);

    // Cleanup memory
    destroyClassifiers();

    return 0;
}
