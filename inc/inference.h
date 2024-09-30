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

#include <stdbool.h>

// Define a type for a fuzzy variable
typedef struct {
    FuzzySet *variable;
    int value;
    bool invert;
} FuzzyVariable;

// Define a type for a fuzzy antecedent
typedef enum { FUZZY_ANY_OF, FUZZY_ALL_OF } FuzzyOperator;

typedef struct {
    FuzzyVariable *variables;
    int num_variables;
    FuzzyOperator operator;
} FuzzyAntecedent;

// Define a type for a fuzzy rule
typedef struct {
    FuzzyAntecedent *antecedent;
    int num_antecedents;
    FuzzyVariable consequent;
} FuzzyRule;

// Define macros to create fuzzy variables and antecedents
#define NOT(_variable, _value)                                                 \
    (FuzzyVariable) { .variable = &_variable, .value = _value, .invert = true }

#define VAR(_variable, _value)                                                 \
    (FuzzyVariable) { .variable = &_variable, .value = _value, .invert = false }

#define THEN(_variable, _value)                                                \
    (FuzzyVariable) { .variable = &_variable, .value = _value }

#define ANY_OF(...)                                                            \
    {.operator= FUZZY_ANY_OF,                                                  \
     .variables = (FuzzyVariable[]){__VA_ARGS__},                              \
     .num_variables =                                                          \
         sizeof((FuzzyVariable[]){__VA_ARGS__}) / sizeof(FuzzyVariable)}

#define ALL_OF(...)                                                            \
    {.operator= FUZZY_ALL_OF,                                                  \
     .variables = (FuzzyVariable[]){__VA_ARGS__},                              \
     .num_variables =                                                          \
         sizeof((FuzzyVariable[]){__VA_ARGS__}) / sizeof(FuzzyVariable)}

#define WHEN(...) {__VA_ARGS__}

// Define a macro to create a fuzzy rule
#define PROPOSITION(_antecedent, _consequent)                                  \
    {.antecedent = (FuzzyAntecedent[])_antecedent,                             \
     .num_antecedents =                                                        \
         sizeof((FuzzyAntecedent[])_antecedent) / sizeof(FuzzyAntecedent),     \
     .consequent = _consequent}

void fuzzyInference(const FuzzyRule *rules, int numRules);

#endif
