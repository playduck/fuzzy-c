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
    // Initialize the output memberships of the consequent to 0
    for (int i = 0; i < numRules; i++) {
        rules[i].consequent.variable->memberships[rules[i].consequent.value] =
            0.0;
    }

    // Iterate over each rule
    for (int i = 0; i < numRules; i++) {
        const FuzzyRule *rule = &rules[i];

        // Calculate the membership of the inputs
        double membership = 1.0;
        double orMembership = 0.0;
        bool isOrOperator = false;

        for (int j = 0; j < rule->num_antecedents; j++) {
            printf("Processing antecedent %d of rule %d...\n", j, i);
            FuzzyAntecedent *antecedent = &rule->antecedents[j];

            if (j % 2 == 0) {
                // This is a variable
                printf("Variable: %p, value: %d\n",
                       antecedent->variable.variable,
                       antecedent->variable.value);
                double inputMembership =
                    antecedent->variable.variable
                        ->memberships[antecedent->variable.value];

                printf("Membership: %f\n", inputMembership);
                if (isOrOperator) {
                    orMembership = fmax(orMembership, inputMembership);
                } else {
                    membership = fmin(membership, inputMembership);
                }
            } else {
                // This is an operator
                printf("Operator: %d\n", antecedent->operator);
                if (antecedent->operator== FUZZY_OR) {
                    // Use OR operation
                    isOrOperator = true;
                } else {
                    isOrOperator = false;
                    if (orMembership > 0.0) {
                        membership = fmax(membership, orMembership);
                    }
                    orMembership = 0.0;
                }
            }
        }

        // Apply the final membership to the output
        if (orMembership > 0.0) {
            membership = fmax(membership, orMembership);
        }

        // add the membership to the output
        printf("Output variable: %p, value: %d\n", rule->consequent.variable,
               rule->consequent.value);
        rule->consequent.variable->memberships[rule->consequent.value] =
            fmax(rule->consequent.variable->memberships[rule->consequent.value],
                 membership);
        printf("Membership: %f\n",
               rule->consequent.variable->memberships[rule->consequent.value]);
    }

    // Normalize the output membership
    for (int i = 0; i < numRules; i++) {
        printf("Normalizing rule %d...\n", i);
        normalizeClass(rules[i].consequent.variable);
    }
}
