/**
 * @file membership_function.h
 * @brief Fuzzy Logic membership function header.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */

#ifndef FUZZY_MEMBERSHIP_FUNCTION_H
#define FUZZY_MEMBERSHIP_FUNCTION_H
#pragma once

typedef enum { TRIANGULAR, TRAPEZOIDAL, RECTANGULAR } MembershipFunctionType;

typedef struct {
    double a;
    double b;
    double c;
    double d;
    MembershipFunctionType type;
} MembershipFunction;

#define FUZZY_LABEL(a, ...) a,
#define FUZZY_VALUE(a, ...) {__VA_ARGS__},

#define DEFINE_FUZZY_MEMBERSHIP(name)                                          \
    enum { name(FUZZY_LABEL) };                                                \
    MembershipFunction name[] = {name(FUZZY_VALUE)};

double membershipFunction(double x, MembershipFunction mf);

#endif
