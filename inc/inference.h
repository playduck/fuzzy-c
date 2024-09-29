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

#define CREATE_FUZZY_RULE_1(input1, input1_index, output_class, output_index) \
    (FuzzyRule) { \
        .inputs = (FuzzyClass*[]) {input1}, \
        .inputIndices = (int[]) {input1_index}, \
        .numInputs = 1, \
        .output = output_class, \
        .outputIndex = output_index \
    }

#define CREATE_FUZZY_RULE_2(input1, input1_index, input2, input2_index, output_class, output_index) \
    (FuzzyRule) { \
        .inputs = (FuzzyClass*[]) {input1, input2}, \
        .inputIndices = (int[]) {input1_index, input2_index}, \
        .numInputs = 2, \
        .output = output_class, \
        .outputIndex = output_index \
    }

#define CREATE_FUZZY_RULE_3(input1, input1_index, input2, input2_index, input3, input3_index, output_class, output_index) \
    (FuzzyRule) { \
        .inputs = (FuzzyClass*[]) {input1, input2, input3}, \
        .inputIndices = (int[]) {input1_index, input2_index, input3_index}, \
        .numInputs = 3, \
        .output = output_class, \
        .outputIndex = output_index \
    }

typedef struct {
    FuzzyClass** inputs;         // Array of input fuzzy classes
    int* inputIndices;           // Array of input indices
    int numInputs;
    FuzzyClass* output;          // Output fuzzy class
    int outputIndex;             // Output index
} FuzzyRule;

void fuzzyInference(const FuzzyRule* rules, int numRules);

#endif
