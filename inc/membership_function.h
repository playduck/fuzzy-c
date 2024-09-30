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

typedef enum { TRIANGULAR, TRAPEZOIDAL, RECTANGULAR } MembershipFunctionType_e;

typedef struct {
    double a;
    double b;
    double c;
    double d;
    MembershipFunctionType_e type;
} MembershipFunction_t;

#define FUZZY_LABEL(a, ...) a,
#define FUZZY_VALUE(a, ...) {__VA_ARGS__},

// this macro takes a membership function definition list with labels and
// produces an enum and a list of MembershipFunction_t elements. The list MUST
// have the shape of:
// > #define OutputMembershipFunctions(X)
// >  X(OUTPUT_LOW, 00.0, 0.0, 30.0, 50.0, TRAPEZOIDAL)
// >  X(OUTPUT_MEDIUM, 30.0, 50.0, 70.0, 0.0, TRIANGULAR)
// >  X(OUTPUT_HIGH, 50.0, 70.0, 100.0, 100.0, TRAPEZOIDAL)
// > DEFINE_FUZZY_MEMBERSHIP(OutputMembershipFunctions)
// The X parameter and X() function can be any undefined Macro, the order of
// arguments is: ENUM_LABEL, a, b, c, d, MembershipFunctionType_e the ENUM_LABEL
// will be generated to aid with writing rules

#define DEFINE_FUZZY_MEMBERSHIP(name)                                          \
    enum { name(FUZZY_LABEL) };                                                \
    MembershipFunction_t name[] = {name(FUZZY_VALUE)};

double membershipFunction(double x, MembershipFunction_t mf);

#endif
