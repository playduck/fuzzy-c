/**
 * @file fuzzy.h
 * @brief Fuzzy Logic global header.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#ifndef FUZZY_C_H
#define FUZZY_C_H
#pragma once

#include "class.h"
#include "classifier.h"
#include "defuzzifier.h"
#include "inference.h"
#include "membership_function.h"

#define FUZZY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

#endif
