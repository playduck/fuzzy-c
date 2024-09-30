/**
 * @file minimal.c
 * @brief A minimal example of Fuzzy-C
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
#include <time.h>

// Define the fuzzy sets for the input and output
FuzzySet_t Input;
FuzzySet_t Output;

// Define labels for printing the fuzzy sets (only for debugging)
const char *labels[] = {"Low", "Mid", "High"};

// Define the membership functions for the input fuzzy set
#define InputMembershipFunctions(X)                                            \
    X(INPUT_LOW, 0.0, 0.0, 15.0, 40.0, TRAPEZOIDAL)                            \
    X(INPUT_MEDIUM, 15.0, 40.0, 60.0, 80.0, TRAPEZOIDAL)                       \
    X(INPUT_HIGH, 60.0, 80.0, 100.0, 100.0, TRAPEZOIDAL)
DEFINE_FUZZY_MEMBERSHIP(InputMembershipFunctions)

// Define the membership functions for the output fuzzy set
#define OutputMembershipFunctions(X)                                           \
    X(OUTPUT_LOW, 00.0, 0.0, 30.0, 50.0, TRAPEZOIDAL)                          \
    X(OUTPUT_MEDIUM, 30.0, 50.0, 70.0, 0.0, TRIANGULAR)                        \
    X(OUTPUT_HIGH, 50.0, 70.0, 100.0, 100.0, TRAPEZOIDAL)
DEFINE_FUZZY_MEMBERSHIP(OutputMembershipFunctions)

// Define the fuzzy rules
FuzzyRule_t rules[] = {
    // If the input is low, then the output is high
    PROPOSITION(WHEN(ALL_OF(VAR(Input, INPUT_LOW))), THEN(Output, OUTPUT_HIGH)),

    // If the input is not low, then the output is low
    PROPOSITION(WHEN(ALL_OF(NOT(Input, INPUT_LOW))), THEN(Output, OUTPUT_LOW)),
};

int main(int argc, char *argv[]) {
    double input_x;

    // Get an input value from the command line arguments, or use a random value
    if (argc <= 1) {
        // Seed the random number generator
        srand(time(NULL));
        // Generate a random value between 0 and 100
        input_x = (rand() % 10000) / 100.0;
        printf("Usage: %s <value>\n", argv[0]);
    } else {
        // Convert the command line argument to a double
        input_x = atof(argv[1]);
    }

    // Initialize the fuzzy sets
    FuzzySetInit(&Input, InputMembershipFunctions,
                 FUZZY_LENGTH(InputMembershipFunctions));
    FuzzySetInit(&Output, OutputMembershipFunctions,
                 FUZZY_LENGTH(OutputMembershipFunctions));

    // Classify the input into a fuzzy state
    FuzzyClassifier(input_x, &Input);

    // Print the fuzzified input
    printf("Input %.04f:\n", input_x);
    printClassifier(&Input, labels);

    // Run the fuzzy inference
    fuzzyInference(rules, FUZZY_LENGTH(rules));

    // Defuzzify the output
    double output_y = defuzzification(&Output);

    // Print the output
    printf("Output %.04f:\n", output_y);
    printClassifier(&Output, labels);

    // Free the fuzzy sets
    FuzzySetFree(&Input);
    FuzzySetFree(&Output);

    return 0;
}
