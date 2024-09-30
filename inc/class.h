/**
 * @file class.h
 * @brief Fuzzy Logic class header.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#ifndef FUZZY_CLASS_H
#define FUZZY_CLASS_H
#pragma once

#include "membership_function.h"

typedef struct {
    double *membershipValues;
    MembershipFunction *membershipFunctions;
    int length;
} FuzzySet;

void FuzzySetInit(FuzzySet *set, const MembershipFunction *membershipFunctions,
                  int length);
void FuzzySetFree(FuzzySet *set);

void normalizeClass(FuzzySet *set);

void printClassifier(FuzzySet *set, const char **labels);

#endif
