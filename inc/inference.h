/**
 * @file membership_function.h
 * @brief Fuzzy Logic inference header.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#ifndef FUZZY_INFERENCE_H
#define FUZZY_INFERENCE_H
#pragma once

#include "class.h"
#include "classifier.h"
#include "membership_function.h"

// Define a type for a fuzzy variable
typedef struct {
    FuzzyClass *variable;
    int value;
} FuzzyVariable;

// Define a type for a fuzzy antecedent
typedef enum { FUZZY_AND, FUZZY_OR } FuzzyOperator;

typedef union {
    FuzzyVariable variable;
    FuzzyOperator operator;
} FuzzyAntecedent;

// Define a type for a fuzzy rule
typedef struct {
    FuzzyAntecedent *antecedents;
    int num_antecedents;
    FuzzyVariable consequent;
} FuzzyRule;

// Define macros to create fuzzy variables and antecedents
#define VAR(_variable, _value)                                                 \
    {                                                                          \
        .variable = {.variable = _variable, .value = _value }                  \
    }
#define AND {.operator= FUZZY_AND}
#define OR {.operator= FUZZY_OR}
#define ANTECEDENTS(...)                                                       \
    .antecedents = (FuzzyAntecedent[]){__VA_ARGS__},                           \
    .num_antecedents =                                                         \
        sizeof((FuzzyAntecedent[]){__VA_ARGS__}) / sizeof(FuzzyAntecedent)

#define THEN(_variable, _value)                                                \
    .consequent = (FuzzyVariable) { .variable = _variable, .value = _value }

// Define a macro to create a fuzzy rule
#define IF(A, B) {A, B}

void fuzzyInference(const FuzzyRule *rules, int numRules);

#endif
