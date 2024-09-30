/**
 * @file classifier.c
 * @brief Fuzzy Logic classification implementation.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#include "classifier.h"

#include "membership_function.h"

#include <math.h>
#include <stdio.h>

/**
 * Performs fuzzy classification on an input value.
 *
 * This function takes an input value x and a FuzzySet_t struct as
 * arguments. It calculates the membership degree of the input value for each
 * membership function in the FuzzySet_t struct and stores the resulting values.
 *
 * @param x The input value to classify.
 * @param input The FuzzySet_t
 */
void FuzzyClassifier(double x, FuzzySet_t *set) {
    for (int i = 0; i < set->length; i++) {
        set->membershipValues[i] =
            membershipFunction(x, set->membershipFunctions[i]);
    }
}
