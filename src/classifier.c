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

#include <stdio.h>
#include <math.h>

#include "classifier.h"
#include "membership_function.h"

/**
 * Performs fuzzy classification on an input value.
 *
 * This function takes an input value x and a FuzzyClassifier struct as arguments.
 * It calculates the membership degree of the input value for each membership function
 * in the FuzzyClassifier struct and stores the results in a FuzzyClass struct.
 *
 * @param x The input value to classify.
 * @param input The FuzzyClassifier struct that defines the membership functions.
 * @param output The FuzzyClass struct that will store the classification results.
 */
void fuzzyClassifier(double x, FuzzyClassifier *input, FuzzyClass *output)
{
    for (uint8_t i = 0; i < input->length; i++)
    {
        output->memberships[i] = membershipFunction(x, input->inputs[i]);
    }
    output->length = input->length;
}

/**
 * Initializes a FuzzyClassifier struct.
 *
 * This function allocates memory for the membership functions in the FuzzyClassifier struct.
 *
 * @param input The FuzzyClassifier struct to initialize.
 * @param length The number of membership functions in the FuzzyClassifier struct.
 */
void fuzzyClassifierInit(FuzzyClassifier *input, uint8_t length)
{
    input->inputs = (MembershipFunction *)malloc(length * sizeof(MembershipFunction));
    input->length = length;
}

/**
 * Initializes a FuzzyClass struct.
 *
 * This function allocates memory for the membership values in the FuzzyClass struct.
 *
 * @param output The FuzzyClass struct to initialize.
 * @param length The number of membership values in the FuzzyClass struct.
 */
void FuzzyClassInit(FuzzyClass *output, uint8_t length)
{
    output->memberships = (double *)malloc(length * sizeof(double));
    output->length = length;
}

/**
 * Frees the memory allocated for a FuzzyClassifier struct.
 *
 * This function should be called when the FuzzyClassifier struct is no longer needed.
 *
 * @param input The FuzzyClassifier struct to free.
 */
void fuzzyClassifierFree(FuzzyClassifier *input)
{
    free(input->inputs);
}

/**
 * Frees the memory allocated for a FuzzyClass struct.
 *
 * This function should be called when the FuzzyClass struct is no longer needed.
 *
 * @param output The FuzzyClass struct to free.
 */
void FuzzyClassFree(FuzzyClass *output)
{
    free(output->memberships);
}

/**
 * Initializes a classifier with a set of membership functions.
 *
 * This function initializes a FuzzyClassifier struct and a FuzzyClass struct with a set of membership functions.
 *
 * @param inputClassifier The FuzzyClassifier struct to initialize.
 * @param outputClassifier The FuzzyClass struct to initialize.
 * @param membershipFunctions The array of membership functions to use.
 * @param length The number of membership functions in the array.
 */
void initClassifier(FuzzyClassifier *inputClassifier, FuzzyClass *outputClassifier, const MembershipFunction *membershipFunctions, uint8_t length)
{
    fuzzyClassifierInit((FuzzyClassifier *)inputClassifier, length);
    FuzzyClassInit((FuzzyClass *)outputClassifier, length);
    for (uint8_t i = 0; i < length; i++)
    {
        inputClassifier->inputs[i] = membershipFunctions[i];
    }
}

/**
 * Normalizes the membership values in a FuzzyClass struct.
 *
 * This function calculates the sum of all membership values and divides each membership value by the sum.
 *
 * @param class The FuzzyClass struct to normalize.
 */
void normalizeClass(FuzzyClass *class)
{
    // Calculate the sum of all membership values
    double sum = 0.0;
    for (uint8_t i = 0; i < class->length; i++)
    {
        sum += class->memberships[i];
    }

    // Check for division by zero
    if (sum == 0.0)
    {
        // Handle the case where the sum is zero
        // For example, set all membership values to 0.0
        for (uint8_t i = 0; i < class->length; i++)
        {
            class->memberships[i] = 0.0;
        }
    }
    else
    {
        // Normalize the membership values
        for (uint8_t i = 0; i < class->length; i++)
        {
            class->memberships[i] /= sum;
        }
    }
}

/**
 * Prints the classification results in a FuzzyClass struct.
 *
 * This function prints the membership values in a FuzzyClass struct along with a bar chart representation.
 *
 * @param output The FuzzyClass struct to print.
 * @param labels The array of labels to use for the membership values.
 */
void printClassifier(FuzzyClass *output, const char **labels)
{
    for (uint8_t i = 0; i < output->length; i++)
    {
        printf("%s", labels[i]);

        printf("\t [");
        const uint8_t bar_length = 24;
        int8_t threshold = round(output->memberships[i] * bar_length) - 1;
        for (uint8_t i = 0; i < bar_length; i++)
        {
            if (i < threshold)
            {
                printf("=");
            }
            else if (i == threshold)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }
        }
        printf("] %6.2f %%\n", output->memberships[i] * 100.0);
    }
    printf("\n");
}
