#include <stdio.h>
#include <stdlib.h> // Imported for "EXIT_FAILURE" and "EXIT_SUCCESS"
#include <string.h> // Imported for "strtok"
#include <math.h>   // Imported for math operations


/**
 * @brief Defines the maximum line length.
 */
#define LINEMAXSIZE 1024

/**
 * @brief The delimiters used to separate the input
 */
#define INPUTDELIMS " \t\r\n"

//My Consts:
#define ERROR -1
#define ASCII0 48
#define ASCII9 57
#define ASCIIDOT 46
#define DECIMAL 10.0
#define START 0
#define MAXARRAYLENGTH 100
#define MAXDIGITLENGTH 9
#define ROUNDING3 1000.0
#define EPSILON 0.00000001
#define MAXNUMSTARS 20
#define STAR "*"
#define PRINTERROR "ERROR\n"
#define NOINPUT "No input\n"
#define FLO 1
#define WHOLEINT 0

// took inspiration from https://stackoverflow.com/questions/36018074/c-string-to-double-conversion
//and from the pdf explanation
/**
 * will convert a string into a double
 * @param string string var to place results into
 * @param length length of the input
 * @param bool 1=float 0=int
 * @return number from the string
 */
double stringToDoubleConverter(char *string, size_t length, short bool)
{
    double result = 0;
    int dotPosition = 0;
    // run though the strings vals:
    for (int i = 0; i < length; i++)
    {
        //check if the val is a digit
        if (string[i] >= ASCII0 && string[i] <= ASCII9)
        {
            // if it is a digit it cannot start with 0
            // (unless it in in the format 0.x)
            if (i == START && string[i] == ASCII0 && length > 1 && string[i + 1] != ASCIIDOT)
            {
                return ERROR;
            }
            // conversion:
            result = result * DECIMAL + (string[i] - ASCII0);
        }
            // if it is not a digit it must be a '.'
        else if (string[i] == ASCIIDOT)// check for the dot
        {
            // and if it is in fact a '.' it has to follow certain criteria
            if (dotPosition != 0 || i == length - 1 || bool == WHOLEINT)
            {
                return ERROR;
            }
            //save the '.' position for later
            dotPosition = length - i - 1;
        }
            // not a digit.. not a '.' --> Error!
        else
        {
            return ERROR;
        }
    }
    // if the is a floating point we should place it
    while (dotPosition)
    {
        result /= DECIMAL;
        dotPosition--;
    }

    return result;
}

/**
 * reads line of text and places numbers it reads into an var f
 * @param f pointer to put result in
 * @return the length of the array. or -1 if error
 */
int readNextLine(double *f)
{
    char line[LINEMAXSIZE];
    char *value = NULL;

    if (fgets(line, LINEMAXSIZE, stdin) == NULL)
    {
        fprintf(stderr, NOINPUT);
        return ERROR;
    }

    value = strtok(line, INPUTDELIMS);
    int i = 0;
    double temp = 0;
    while (value != NULL)
    {
        if (i >= MAXARRAYLENGTH)
        {
            return ERROR;
        }
        size_t length = strlen(value);
        if (length > MAXDIGITLENGTH)
        {
            return ERROR;
        }
        temp = stringToDoubleConverter(value, length, FLO);
        if (temp == ERROR)
        {
            return ERROR;
        }
        f[i] = temp;
        value = strtok(NULL, INPUTDELIMS);
        i++;
    }
    return i;
}

/**
 * reads a single number
 * @param n pointer to place results
 * @return success or not
 */
int readInt(int *n)
{
    char line[LINEMAXSIZE];
    char *value = NULL;

    if (fgets(line, LINEMAXSIZE, stdin) == NULL)
    {
        fprintf(stderr, NOINPUT);
        return ERROR;
    }

    value = strtok(line, INPUTDELIMS);
    int temp = 0;
    // check it is not empty. and that it is singular
    if (value == NULL || strtok(NULL, INPUTDELIMS) != NULL)
    {
        return EXIT_FAILURE;
    }
    size_t length = strlen(value);
    temp = (int) stringToDoubleConverter(value, length, WHOLEINT);
    if (temp == ERROR)
    {
        return EXIT_FAILURE;
    }

    *n = temp;
    return EXIT_SUCCESS;
}


/**
 * copies arrays
 * @param a1 copy to
 * @param a2 copy from
 * @param n length
 */
void copyArray(double *a1, double *a2, int n)
{
    for (int i = 0; i < n; i++)
    {
        a1[i] = a2[i];
    }
}

/**
 * puts 0 in the entire array
 * @param a array
 * @param n length
 */
void clear(double *a, int n)
{
    for (int i = 0; i < n; ++i)
    {
        a[i] = 0.0;
    }
}

/**
 * will centralize an array
 * @param f array to centralize
 * @param len length of array
 */
void centralize(double *f, int len)
{
    int zeros = (MAXARRAYLENGTH - len);
    int offset = floor(zeros / 2.0);
    double temp[MAXARRAYLENGTH];
    copyArray(temp, f, MAXARRAYLENGTH);
    clear(f, MAXARRAYLENGTH);

    for (int j = offset, i = 0; i < len; j++, i++)
    {
        f[j] = temp[i];
    }
}

/**
 * normalizes an array
 * @param f array to normalize
 * @param len length of the array
 */
void normalize(double *f, int len)
{
    double sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += f[i];
    }
    // as by the formula:
    for (int j = 0; j < len; j++)
    {
        f[j] /= sum;
    }
}

/**
 * function phi from the PDF
 * @param t input of index
 * @param f_len len of array
 * @return result
 */
int phi(int t, int f_len)
{
    return t + f_len / 2;
}

/**
 * this is the main function in the program
 * will actually calculate the discrete convolution as explained in the PDF
 * @param g sequence of volumes in each iteration
 * @param g_len len of g
 * @param h sequence of the amplifier
 * @param h_len len of h
 * @param f results holder
 * @param n number of recursive iterations
 */
void discreteConvolution(double *g, int g_len, double *h, int h_len, double *f, int n)
{
    int phi1 = 0, phi2 = 0, index = 0;
    for (int t = (int) -ceil(h_len / 2.0 + 1); t < floor(h_len / 2.0) - 1; t++)
    {
        for (int m = (int) -ceil(g_len / 2.0); m <= floor(g_len / 2.0); m++)
        {
            phi1 = phi(t - m, g_len);
            phi2 = phi(m, h_len);
            if (phi1 >= 0 && phi1 < g_len && phi2 >= 0 && phi2 < h_len)
            {
                // offsetting to keep index positive
                f[index] += g[phi1] * h[phi2];
            }
        }
        index++;
    }
    normalize(f, MAXARRAYLENGTH);
    //recursive call
    if (n > 1)
    {
        copyArray(g, f, MAXARRAYLENGTH);
        clear(f, MAXARRAYLENGTH);
        discreteConvolution(g, g_len, h, h_len, f, --n);
    }
}

/**
 * checks for the maximum in an array
 * @param a array to search
 * @param n len of array
 * @return maximum in a
 */
double maxInArray(double *a, int n)
{
    double max = 0;
    for (int i = 0; i < n; ++i)
    {
        if (max < a[i])
        {
            max = a[i];
        }
    }
    return max;
}

/**
 * rounds values in a array by the formula in the PDF
 * @param a array to round
 * @param n len of a
 */
void roundValsTo3(double *a, int n)
{
    int temp = 0;
    for (int i = 0; i < n; ++i)
    {
        temp = round(ROUNDING3 * a[i]);
        a[i] = temp / ROUNDING3;
    }
}

/**
 * will organize the input to be a explained in the PDF- centralized and normalized
 * @param g array g
 * @param h array h
 * @param g_len  len of g
 * @param h_len  len of h
 */
void organizeInput(double *g, double *h, int g_len, int h_len)
{
    normalize(g, g_len);
    normalize(h, h_len);

    centralize(g, g_len);
    centralize(h, h_len);
}

/**
 * will print the results
 * @param r array if results
 * @param max maximum in the array
 */
void printResults(double *r, double max)
{
    int numOfStars = 0;
    for (int j = 0; j < MAXARRAYLENGTH; ++j)
    {
        printf("%0.3f: ", r[j]);
        numOfStars = MAXNUMSTARS * r[j] / max;
        for (int i = 0; i < numOfStars; ++i)
        {
            printf(STAR);
        }
        printf("\n");
    }
}

/**
 *  will calculate the maximum in the array
 * @param f array to search
 * @param len len of array
 * @return the maximum.
 * although if the max is less than epsilon(described in the PDF) will return an error
 */
double checkMax(double *f, int len)
{
    double max = maxInArray(f, len);
    if (max < EPSILON)
    {
        return ERROR;
    }
    return max;
}

/**
 * @brief The main function that runs the program.
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated
 * multibyte strings that represent the arguments passed to the program from the execution
 * environment. The value of argv[argc] is guaranteed to be a null pointer.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has execution errors.
 */
int main(int argc, const char *argv[])
{
    double g[MAXARRAYLENGTH] = {0}, h[MAXARRAYLENGTH] = {0};
    int n = 0;
    int g_len = 0;
    int h_len = 0;

    // checks to do here are:
    //1. no more than 100 numbers
    //2. no more than 9 digits each
    //3. h<=g

    // input handling:
    if ((g_len = readNextLine(g)) < 0)
    {
        fprintf(stderr, PRINTERROR);
        return EXIT_FAILURE;
    }
    if ((h_len = readNextLine(h)) < 0)
    {
        fprintf(stderr, PRINTERROR);
        return EXIT_FAILURE;
    }
    if (h_len > g_len)
    {
        fprintf(stderr, PRINTERROR);
        return EXIT_FAILURE;
    }
    if (readInt(&n))
    {
        fprintf(stderr, PRINTERROR);
        return EXIT_FAILURE;
    }

    // organizing the input:
    organizeInput(g, h, g_len, h_len);


    //conducting the convolution:
    double r[MAXARRAYLENGTH] = {0};
    if (n > 0)
    {
        discreteConvolution(g, MAXARRAYLENGTH, h, MAXARRAYLENGTH, r, n);
    }
    else
    {
        copyArray(r, g, MAXARRAYLENGTH);
    }

    //rounding val's
    roundValsTo3(r, MAXARRAYLENGTH);

    // checking for max:
    double max = checkMax(r, MAXARRAYLENGTH);
    if (max == ERROR)
    {
        //if its too low just exit gracefully
        return EXIT_SUCCESS;
    }

    //printing the results:
    printResults(r, max);

    return EXIT_SUCCESS;
}

