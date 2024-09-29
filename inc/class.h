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

#include <stdint.h>

typedef struct {
    double *memberships;
    uint8_t length;
} FuzzyClass;

void FuzzyClassInit(FuzzyClass *output, uint8_t length);
void FuzzyClassFree(FuzzyClass *output);
void normalizeClass(FuzzyClass *class);

#endif
