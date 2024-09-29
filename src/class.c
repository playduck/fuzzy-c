/**
 * @file class.c
 * @brief Fuzzy Logic inference engine implementation.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#include "class.h"

#include <stdint.h>
#include <stdlib.h>

/**
 * Initializes a FuzzyClass struct.
 *
 * This function allocates memory for the membership values in the FuzzyClass
 * struct.
 *
 * @param output The FuzzyClass struct to initialize.
 * @param length The number of membership values in the FuzzyClass struct.
 */
void FuzzyClassInit(FuzzyClass *output, uint8_t length) {
    output->memberships = (double *)malloc(length * sizeof(double));
    output->length = length;
}

/**
 * Frees the memory allocated for a FuzzyClass struct.
 *
 * This function should be called when the FuzzyClass struct is no longer
 * needed.
 *
 * @param output The FuzzyClass struct to free.
 */
void FuzzyClassFree(FuzzyClass *output) { free(output->memberships); }

/**
 * Normalizes the membership values in a FuzzyClass struct.
 *
 * This function calculates the sum of all membership values and divides each
 * membership value by the sum.
 *
 * @param class The FuzzyClass struct to normalize.
 */
void normalizeClass(FuzzyClass *class) {
    // Calculate the sum of all membership values
    double sum = 0.0;
    for (uint8_t i = 0; i < class->length; i++) {
        sum += class->memberships[i];
    }

    // Check for division by zero
    if (sum == 0.0) {
        // Handle the case where the sum is zero
        // For example, set all membership values to 0.0
        for (uint8_t i = 0; i < class->length; i++) {
            class->memberships[i] = 0.0;
        }
    } else {
        // Normalize the membership values
        for (uint8_t i = 0; i < class->length; i++) {
            class->memberships[i] /= sum;
        }
    }
}
