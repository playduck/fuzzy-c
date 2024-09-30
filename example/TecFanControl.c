/**
 * @file TecFanControl.c
 * @brief Fuzzy Logic implemntation example to control the Fan PWM Duty Cycle
 * for a Thermoelectric (TEC) Cooler based on the Temperature, Rate of Change,
 * TEC Power and the current fan speed
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

const char *lmhLabels[] = {"Low", "Medium", "High"};
const char *changeLabels[] = {"Dec", "Stable", "Inc"};
const char *fanLabels[] = {"Off", "On"};
const char *fanSpeedLabels[] = {"Off", "Slow", "Medium", "Fast"};

// inputs
FuzzySet TemperatureState;
FuzzySet TempChangeState;
FuzzySet TECPowerState;
FuzzySet FanState;

// output
FuzzySet FanSpeed;

// define membership functions
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

FuzzyRule rules[] = {
    // If fan state is "off" and hot-side temperature is "medium" or "high" or
    // TEC heat load is "high", then fan speed is "high"
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_OFF)),
                     ANY_OF(VAR(TemperatureState, TEMPERATURE_MEDIUM),
                            VAR(TemperatureState, TEMPERATURE_HIGH),
                            VAR(TECPowerState, TEC_POWER_HIGH))),
                THEN(FanSpeed, FAN_SPEED_FAST)),

    // if the fan is off and temperature is low and stable or decreasing then
    // keep the fan off
    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_OFF),
                            VAR(TemperatureState, TEMPERATURE_LOW)),
                     ANY_OF(VAR(TempChangeState, TEMP_CHANGE_STABLE),
                            VAR(TempChangeState, TEMP_CHANGE_DECREASING))),
                THEN(FanSpeed, FAN_SPEED_OFF)),

    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_LOW)),
                     ANY_OF(VAR(TempChangeState, TEMP_CHANGE_STABLE),
                            VAR(TempChangeState, TEMP_CHANGE_DECREASING))),
                THEN(FanSpeed, FAN_SPEED_OFF)),

    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TemperatureState, TEMPERATURE_MEDIUM),
                            NOT(TECPowerState, TEC_POWER_HIGH))),
                THEN(FanSpeed, FAN_SPEED_MEDIUM)),

    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TemperatureState, TEMPERATURE_HIGH)),
                     ANY_OF(VAR(TECPowerState, TEC_POWER_MEDIUM),
                            VAR(TECPowerState, TEC_POWER_LOW))),
                THEN(FanSpeed, FAN_SPEED_FAST)),

    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_LOW),
                            VAR(TemperatureState, TEMPERATURE_LOW))),
                THEN(FanSpeed, FAN_SPEED_OFF)),

    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_MEDIUM))),
                THEN(FanSpeed, FAN_SPEED_MEDIUM)),

    PROPOSITION(WHEN(ALL_OF(VAR(FanState, FAN_STATE_ON),
                            VAR(TECPowerState, TEC_POWER_HIGH))),
                THEN(FanSpeed, FAN_SPEED_FAST)),
};

void createClassifiers() {
    // initilize the input classifiers
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

void destroyClassifiers() {
    // free the classifiers
    FuzzySetFree(&TemperatureState);
    FuzzySetFree(&TempChangeState);
    FuzzySetFree(&TECPowerState);
    FuzzySetFree(&FanState);
    FuzzySetFree(&FanSpeed);
}

double map_range(double value, double in_min, double in_max, double out_min,
                 double out_max) {
    // linear map helper function
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
