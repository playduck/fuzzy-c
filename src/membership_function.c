/**
 * @file membership_functions.c
 * @brief Fuzzy Logic membership function implementation.
 * @author Robin Prilliwtz
 * @date 2024
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * See LICENSE.txt file for details.
 *
 */


#include "membership_function.h"

/**
 * Calculates the membership degree of a triangular membership function.
 *
 * The triangular membership function is defined by three parameters a, b, and c,
 * which represent the start, peak, and end points of the triangle, respectively.
 *
 * @param x The input value to calculate the membership degree for.
 * @param a The start point of the triangle.
 * @param b The peak point of the triangle.
 * @param c The end point of the triangle.
 * @return The membership degree of the input value.
 */
double triangularMembershipFunction(double x, double a, double b, double c)
{
    // If x is outside the triangle, return 0 (no membership)
    if (x < a || x > c)
    {
        return 0.0;
    }
    // If x is on the left side of the triangle, calculate the membership degree using the slope of the left side
    else if (x <= b)
    {
        if (b - a == 0)
        {
            return 1.0;
        }
        else
        {
            return (x - a) / (b - a);
        }
    }
    // If x is on the right side of the triangle, calculate the membership degree using the slope of the right side
    else
    {
        return (c - x) / (c - b);
    }
}

/**
 * Calculates the membership degree of a trapezoidal membership function.
 *
 * The trapezoidal membership function is defined by four parameters a, b, c, and d,
 * which represent the start, peak start, peak end, and end points of the trapezoid, respectively.
 *
 * @param x The input value to calculate the membership degree for.
 * @param a The start point of the trapezoid.
 * @param b The peak start point of the trapezoid.
 * @param c The peak end point of the trapezoid.
 * @param d The end point of the trapezoid.
 * @return The membership degree of the input value.
 */
double trapezoidalMembershipFunction(double x, double a, double b, double c, double d)
{
    // If x is outside the trapezoid, return 0 (no membership)
    if (x <= a || x >= d)
    {
        return 0.0;
    }
    // If x is on the left side of the trapezoid, calculate the membership degree using the slope of the left side
    else if (x <= b)
    {
        return (x - a) / (b - a);
    }
    // If x is on the right side of the trapezoid, calculate the membership degree using the slope of the right side
    else if (x >= c)
    {
        return (d - x) / (d - c);
    }
    // If x is in the flat top of the trapezoid, return 1 (full membership)
    else
    {
        return 1.0;
    }
}

/**
 * Calculates the membership degree of a rectangular membership function.
 *
 * The rectangular membership function is defined by two parameters a and b,
 * which represent the start and end points of the rectangle, respectively.
 *
 * @param x The input value to calculate the membership degree for.
 * @param a The start point of the rectangle.
 * @param b The end point of the rectangle.
 * @return The membership degree of the input value.
 */
double rectangularMembershipFunction(double x, double a, double b)
{
    // If x is outside the rectangle, return 0 (no membership)
    if (x < a || x >= b)
    {
        return 0.0;
    }
    // If x is inside the rectangle, return 1 (full membership)
    else
    {
        return 1.0;
    }
}

/**
 * Calculates the membership degree of a generic membership function.
 *
 * This function takes an input x and a MembershipFunction struct as arguments.
 * The MembershipFunction struct is not defined in this code snippet, but it is
 * assumed to have a type field that indicates the type of membership function to use
 * (either TRIANGULAR or TRAPEZOIDAL) and fields a, b, c, and d that represent the
 * parameters of the membership function.
 *
 * @param x The input value to calculate the membership degree for.
 * @param mf The MembershipFunction struct that defines the membership function.
 * @return The membership degree of the input value.
 */
double membershipFunction(double x, MembershipFunction mf)
{
    // Use a switch statement to determine which membership function to use based on the type field of the MembershipFunction struct
    switch (mf.type)
    {
    case TRIANGULAR:
        // Call the triangular membership function with the input x and the membership function parameters
        return triangularMembershipFunction(x, mf.a, mf.b, mf.c);
    case TRAPEZOIDAL:
        // Call the trapezoidal membership function with the input x and the membership function parameters
        return trapezoidalMembershipFunction(x, mf.a, mf.b, mf.c, mf.d);
    case RECTANGULAR:
        // Call the rectangular membership function with the input x and the membership function parameters
        return rectangularMembershipFunction(x, mf.a, mf.b);
    default:
        // If the membership function type is not recognized, return 0 (no membership)
        return 0.0;
    }
}
