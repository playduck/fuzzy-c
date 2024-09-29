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

#include <stdint.h>
#include <stdlib.h>

#include "membership_function.h"

typedef struct {
    MembershipFunction* inputs;
    uint8_t length;
} FuzzyClassifier;

typedef struct {
    double* memberships;
    uint8_t length;
} FuzzyClass;

void fuzzyClassifier(double x, FuzzyClassifier* input, FuzzyClass* output);

void fuzzyClassifierInit(FuzzyClassifier* input, uint8_t length);
void FuzzyClassInit(FuzzyClass* output, uint8_t length);
void fuzzyClassifierFree(FuzzyClassifier* input);
void FuzzyClassFree(FuzzyClass* output);

void initClassifier(FuzzyClassifier* inputClassifier, FuzzyClass* outputClassifier, const MembershipFunction* membershipFunctions, uint8_t length);
void normalizeClass(FuzzyClass* class);
void printClassifier(FuzzyClass* output, const char** labels);

#endif
