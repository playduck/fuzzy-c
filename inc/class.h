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
    MembershipFunction_t *membershipFunctions;
    int length;
} FuzzySet_t;

void FuzzySetInit(FuzzySet_t *set,
                  const MembershipFunction_t *membershipFunctions, int length);
void FuzzySetFree(FuzzySet_t *set);

void normalizeClass(FuzzySet_t *set);

void printClassifier(FuzzySet_t *set, const char **labels);

#endif
