/**
 * @file classififer.h
 * @brief Fuzzy Logic classififer header.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#ifndef FUZZY_CLASSIFIER_H
#define FUZZY_CLASSIFIER_H
#pragma once

#include "class.h"
#include "membership_function.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    MembershipFunction *inputs;
    uint8_t length;
} FuzzyClassifier;

void fuzzyClassifier(double x, FuzzyClassifier *input, FuzzyClass *output);

void fuzzyClassifierInit(FuzzyClassifier *input, uint8_t length);
void fuzzyClassifierFree(FuzzyClassifier *input);

void initClassifier(FuzzyClassifier *inputClassifier,
                    FuzzyClass *outputClassifier,
                    const MembershipFunction *membershipFunctions,
                    uint8_t length);
void printClassifier(FuzzyClass *output, const char **labels);

#endif
