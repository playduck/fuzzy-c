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

// Define the states and classifiers
FuzzySet Input;
FuzzySet Output;

// define labels for printing
const char *labels[] = {"Low", "Mid", "High"};

// Define the membership functions
#define INPUT_LOW 0
#define INPUT_MEDIUM 1
#define INPUT_HIGH 2
const MembershipFunction InputMembershipFunctions[] = {
    {0.0, 0.0, 15.0, 40.0, TRAPEZOIDAL},    // input low
    {15.0, 40.0, 60.0, 80.0, TRAPEZOIDAL},  // input medium
    {60.0, 80.0, 100.0, 100.0, TRAPEZOIDAL} // input high
};

#define OUTPUT_LOW 0
#define OUTPUT_MEDIUM 1
#define OUTPUT_HIGH 2
const MembershipFunction OutputMembershipFunctions[] = {
    {0.0, 0.0, 30.0, 50.0, TRAPEZOIDAL},    // output low
    {30.0, 50.0, 70.0, 0.0, TRIANGULAR},    // output medium
    {50.0, 70.0, 100.0, 100.0, TRAPEZOIDAL} // output high
};

FuzzyRule rules[] = {
    // if input is low then output is high
    PROPOSITION(WHEN(ALL_OF(VAR(Input, INPUT_LOW))), THEN(Output, OUTPUT_HIGH)),

    // if input is not low then output is low
    PROPOSITION(WHEN(ALL_OF(NOT(Input, INPUT_LOW))), THEN(Output, OUTPUT_LOW)),
};

int main(int argc, char *argv[]) {
    double input_x;

    // get an input value from args, or use a random value
    if (argc <= 1) {
        srand(time(NULL));
        // random value between 0..100
        input_x = (rand() % 10000) / 100.0;
        printf("Usage: %s <value>\n", argv[0]);
    } else {
        input_x = atof(argv[1]);
    }

    // instantiate classifiers
    FuzzySetInit(&Input, InputMembershipFunctions,
                 FUZZY_LENGTH(InputMembershipFunctions));
    FuzzySetInit(&Output, OutputMembershipFunctions,
                 FUZZY_LENGTH(OutputMembershipFunctions));

    // classifiy the input into a fuzzy state
    FuzzyClassifier(input_x, &Input);

    // print the fuzzified input
    printf("Input %.04f:\n", input_x);
    printClassifier(&Input, labels);

    // run inference
    fuzzyInference(rules, FUZZY_LENGTH(rules));

    // Defuzzify the output
    double output_y = defuzzification(&Output);

    // print the output
    printf("Output %.04f:\n", output_y);
    printClassifier(&Output, labels);

    FuzzySetFree(&Input);
    FuzzySetFree(&Output);

    return 0;
}
