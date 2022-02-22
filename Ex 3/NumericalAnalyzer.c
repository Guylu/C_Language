#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "infi.h"


#define USAGE "Usage: NumericalAnalyzer <function number> <a> <b> <N> <x0> <h>\n"
#define DEC 10
#define NUMOFARGS 7
#define FUNCNUM 1
#define SRT 2
#define END 3
#define PARTS 4
#define POINT 5
#define EPS 6
#define INVALID "Invalid input\n"
#define TRUE 1
#define FALSE 0
#define PRINTINTEG "Integral: %0.5f\n"
#define PRINTDER "Derivative: %0.5f\n"

//var to indicate that we had a point of undefinition in one of the functions :(
int undefined = FALSE;

/**
 * func 1
 * @param x param x
 * @return return val of func
 */
double f1(double x)
{
    return (x * x) / 4;
}

/**
 * func 2
 * @param x param x
 * @return return val of func
 */
double f2(double x)
{
    if (x <= 0)
    {
        undefined = TRUE;
    }
    return (-(x * x * x) + 3 * (x * x) + x - 4 * sqrt(x)) / (2 * x * sqrt(x));
}

/**
 * func 3
 * @param x param x
 * @return return val of func
 */
double f3(double x)
{
    return sin(x) * sin(x) - cos(x) * cos(x);
}

/**
* func 4
* @param x param x
* @return return val of func
*/
double f4(double x)
{
    if (cos(2 * x) == -1)
    {
        undefined = TRUE;
    }
    return sin(x) / (1 + cos(2 * x));
}

/**
 * func 5
 * @param x param x
 * @return return val of func
 */
double f5(double x)
{
    return pow(M_E, x);
}

/**
 * func 6
 * @param x param x
 * @return return val of func
 */
double f6(double x)
{
    if (x == 0)
    {
        undefined = TRUE;
    }
    return sinh(2 * x) / (M_E * x * x);
}

/**
 * this function will check thr givven input from the command line
 * it will parse it, check its correctness, and assign it to the correct variables.
 * @param numOfFunc pointer to the func
 * @param start pointer to start val for intergral
 * @param end pointer to end val for integral
 * @param partitions pointer to num of partitions in the intergral by reiman
 * @param point pointer to the point of derivative
 * @param epsilon pointer to value of epsilon for the derivative
 * @param argv array of vals from the command line
 * @return whether there was an error in the args
 */
int inputCheck(int *numOfFunc, double *start, double *end, unsigned int *partitions, double *point,
               double *epsilon, const char *argv[])
{
    char *stringPart = (char *) argv[FUNCNUM];
    *numOfFunc = (int) strtol(stringPart, &stringPart, DEC);
    if (strlen(stringPart))
    {
        return EXIT_FAILURE;
    }
    stringPart = (char *) argv[SRT];
    *start = strtod(stringPart, &stringPart);
    if (strlen(stringPart))
    {
        return EXIT_FAILURE;
    }
    stringPart = (char *) argv[END];
    *end = strtod(stringPart, &stringPart);
    if (strlen(stringPart))
    {
        return EXIT_FAILURE;
    }
    stringPart = (char *) argv[PARTS];
    *partitions = (unsigned int) strtol(stringPart, &stringPart, DEC);
    if (strlen(stringPart))
    {
        return EXIT_FAILURE;
    }
    stringPart = (char *) argv[POINT];
    *point = strtod(stringPart, &stringPart);
    if (strlen(stringPart))
    {
        return EXIT_FAILURE;
    }
    stringPart = (char *) argv[EPS];
    *epsilon = strtod(stringPart, &stringPart);
    if (strlen(stringPart))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * function to return pointer to the function of choise(by the number)
 * @param numOfFunc num of func
 * @return pointer to func
 */
void *funcChoise(int numOfFunc)
{
    switch (numOfFunc)
    {
        case 1:
            return f1;
        case 2:
            return f2;
        case 3:
            return f3;
        case 4:
            return f4;
        case 5:
            return f5;
        case 6:
            return f6;
        default:
            return NULL;

    }
}

/**
 * main function to run the entire program
 * @param argc num of args
 * @param argv array of args
 * @return failure/success
 */
int main(int argc, const char *argv[])
{
    // should be 7 vars
    if (argc != NUMOFARGS)
    {
        fprintf(stderr, USAGE);
        return EXIT_FAILURE;
    }
    int numOfFunc = 0;
    double start = 0;
    double end = 0;
    unsigned int partitions = 0;
    double point = 0;
    double epsilon = 0;

    // parse the input
    if (inputCheck(&numOfFunc, &start, &end, &partitions, &point, &epsilon, argv))
    {
        fprintf(stderr, INVALID);
        return EXIT_FAILURE;
    }
    //get the function
    void *func = funcChoise(numOfFunc);
    if (func == NULL)
    {
        fprintf(stderr, INVALID);
        return EXIT_FAILURE;
    }
    //check input vals
    if (!(start < end && partitions > 0 && epsilon > 0))
    {
        fprintf(stderr, INVALID);
        return EXIT_FAILURE;
    }
    //calc results
    double derivativeRes = derivative(func, point, epsilon);
    if (undefined == TRUE)
    {
        fprintf(stderr, INVALID);
        return EXIT_FAILURE;
    }

    double integralRes = integration(func, start, end, partitions);
    if (undefined == TRUE)
    {
        fprintf(stderr, INVALID);
        return EXIT_FAILURE;
    }
    //print results
    printf(PRINTINTEG, integralRes);
    printf(PRINTDER, derivativeRes);
    return EXIT_SUCCESS;
}