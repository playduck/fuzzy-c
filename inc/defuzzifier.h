/**
 * @file defuzzifier.h
 * @brief Fuzzy Logic defuzuiffier header.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#ifndef FUZZY_DEFUZZIFIER_H
#define FUZZY_DEFUZZIFIER_H
#pragma once

#include "class.h"
#include "classifier.h"

double defuzzification(FuzzySet_t *set);

#endif
