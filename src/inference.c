/**
 * @file inference.c
 * @brief Fuzzy Logic inference engine implementation.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#include "inference.h"

#include "classifier.h"
#include "membership_function.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Performs fuzzy inference on a set of fuzzy rules.
 *
 * This function takes a set of fuzzy rules and calculates the output
 * memberships for each rule. It iterates over each rule, calculates the
 * minimum membership of the inputs, and updates the output memberships
 * accordingly.
 *
 * @param rules An array of fuzzy rules.
 * @param numRules The number of fuzzy rules in the array.
 */
void fuzzyInference(const FuzzyRule *rules, int numRules) {
    // Initialize the output memberships to 0 for each rule
    for (int i = 0; i < numRules; i++) {
        const FuzzyRule *rule = &rules[i];
        rule->output->memberships[rule->outputIndex] = 0;
    }

    // Iterate over each rule
    for (int i = 0; i < numRules; i++) {
        const FuzzyRule *rule = &rules[i];

        // Calculate the minimum membership of the inputs
        double minMembership = 1.0;

        for (int j = 0; j < rule->numInputs; j++) {
            double membership =
                rule->inputs[j]->memberships[rule->inputIndices[j]];
            minMembership = minMembership * membership;
        }

        rule->output->memberships[rule->outputIndex] += minMembership;
    }

    // Normalize the output memberships
    for (int i = 0; i < numRules; i++) {
        const FuzzyRule *rule = &rules[i];
        normalizeClass(rule->output);
    }
}
