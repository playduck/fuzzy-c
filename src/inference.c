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
        rules[i]
            .consequent.variable->membershipValues[rules[i].consequent.value] =
            0.0;
    }

    // Iterate over each rule
    for (int i = 0; i < numRules; i++) {
        const FuzzyRule *rule = &rules[i];

        // Calculate the membership of the inputs
        double membership = 1.0; // Initialize membership to 1.0 (maximum)

        // Iterate over each antecedent in the rule
        for (int j = 0; j < rule->num_antecedents; j++) {
            const FuzzyAntecedent *antecedent = &rule->antecedent[j];

            // Check if the antecedent is an ANY_OF operator
            if (antecedent->operator== FUZZY_ANY_OF) {
                // Calculate the maximum membership of the variables in the
                // ANY_OF operator
                double orMembership = 0.0;
                for (int k = 0; k < antecedent->num_variables; k++) {
                    double inputMembership;

                    // Check if the variable is inverted (i.e., NOT() macro is
                    // used)
                    if (antecedent->variables[k].invert) {
                        // If the variable is inverted, calculate the membership
                        // of the complement (i.e., 1 - membership) This is
                        // because the NOT() macro inverts the membership of the
                        // variable
                        inputMembership =
                            1.0 -
                            antecedent->variables[k].variable->membershipValues
                                [antecedent->variables[k].value];
                    } else {
                        // If the variable is not inverted, calculate the
                        // membership as usual
                        inputMembership =
                            antecedent->variables[k].variable->membershipValues
                                [antecedent->variables[k].value];
                    }

                    // Update the maximum membership of the variables in the
                    // ANY_OF operator
                    orMembership = fmax(orMembership, inputMembership);
                }

                // Update the membership with the minimum of the current
                // membership and the ANY_OF membership
                membership = fmin(membership, orMembership);
            } else if (antecedent->operator== FUZZY_ALL_OF) {
                // Calculate the minimum membership of the variables in the
                // ALL_OF operator
                double andMembership = 1.0;
                for (int k = 0; k < antecedent->num_variables; k++) {
                    double inputMembership;

                    // Check if the variable is inverted (i.e., NOT() macro is
                    // used)
                    if (antecedent->variables[k].invert) {
                        // If the variable is inverted, calculate the membership
                        // of the complement (i.e., 1 - membership) This is
                        // because the NOT() macro inverts the membership of the
                        // variable
                        inputMembership =
                            1.0 -
                            antecedent->variables[k].variable->membershipValues
                                [antecedent->variables[k].value];
                    } else {
                        // If the variable is not inverted, calculate the
                        // membership as usual
                        inputMembership =
                            antecedent->variables[k].variable->membershipValues
                                [antecedent->variables[k].value];
                    }

                    // Update the minimum membership of the variables in the
                    // ALL_OF operator
                    andMembership = fmin(andMembership, inputMembership);
                }

                // Update the membership with the minimum of the current
                // membership and the ALL_OF membership
                membership = fmin(membership, andMembership);
            }
        }

        // Update the output membership with the maximum of the current
        // membership and the calculated membership
        rules[i]
            .consequent.variable->membershipValues[rules[i].consequent.value] =
            fmax(rules[i]
                     .consequent.variable
                     ->membershipValues[rules[i].consequent.value],
                 membership);
    }

    // Normalize the output membership
    for (int i = 0; i < numRules; i++) {
        normalizeClass(rules[i].consequent.variable);
    }
}
