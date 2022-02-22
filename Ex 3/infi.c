#include <stdio.h>
#include <assert.h>

#define M_E         2.71828182845904523536028747135266250   /* e */
#define M_PI        3.14159265358979323846264338327950288   /* pi */

/* Define the anonymous function signature */
typedef double (*RealFunction)(double);


/**
 * Computes the numerical integration for the given rationale function
 * at the given numbers range, based on Riemann sums.
 * @return The integration result.
 */
double integration(RealFunction func, double start, double end, unsigned int partitions)
{
    //asserts of lib for this function
    assert(func != NULL);
    assert(start < end);
    assert(partitions > 0);
    double delta = (end - start) / partitions;
    double integral = 0;
    //calc integral:
    for (unsigned int i = 0; i < partitions; i++)
    {
        integral += func(start + (delta / 2) + delta * i);
    }
    integral *= delta;
    return integral;
}


/**
 * Computes a numerical derivative for the given rationale function
 * at the point, for a given epsilon.
 * @return The differentiate.
 */
double derivative(RealFunction func, double point, double epsilon)
{
    //asserts of lib for this function
    assert(func != NULL);
    assert(epsilon > 0);
    // calc derivative
    double derivative = (func(point + epsilon) - func(point - epsilon)) / (2 * epsilon);
    return derivative;
}
