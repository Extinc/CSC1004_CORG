// =================================================================
// Q1 Assignments :
// Floating Point To Decimal Conversion
// Decimal to Floating Point Conversion
// This C code char int value is represented through ASCII TABLE
// Char Comparison using ASCII TABLE
// =================================================================
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define BSIZE 8
#define MSIZE 23
#define LEN(x) (sizeof(x) / sizeof((x)[0]))
typedef struct
{
    int actexponent;      // Actual exponent Store
    char signbit;         // Sign bit Store
    char *biasedexponent; // biased exponent Store
    char *mantissa;       // Mantissa Store
    char *binarynonfract; // binary non fract Store
    char *binaryfract;    // binary fract Store
    double decresult;     // Decimal Result Store
} binfpstruct;

char *reverse(char *str);
void converttobin(float val, char *binarynonfract, unsigned int len, int option); // Converting Decimal to Binary format
binfpstruct *dectobinfp(float values);                                            // To convert Decimal to IEEE 754 floating point
binfpstruct *binfptodec(char *values);                                            // To Convert IEEE 754 floating point to decimal
int main()
{
    int choice = 0;
    float decnumber = 0.;
    binfpstruct *binfp;
    char binnum[40] = {};
    binfpstruct *fptodecstore;
    printf("Choose an option:");
    printf("\n\n1. Decimal to IEEE 754 Binary floating point \n\n2. IEEE 754 Binary floating point to Decimal");
    printf("\n\nEnter your choice :");
    scanf("%d", &choice);
    fflush(stdin); // Flush the input buffer of the stream
    if (choice == 2)
    {
        // Option 2 : To convert 32-bits Binary Number to Decimal Number

        printf("\nEnter the 32-bits binary number in IEEE 754 floating point format : ");
        fgets(binnum, sizeof(binnum), stdin);

        fptodecstore = binfptodec(binnum);
        printf("\na) IEEE 754 floating point input               : %.34s", binnum);
        printf("\nb) Before conversion, the 1-bit sign bit is    : %c", fptodecstore->signbit);
        printf("\nc) The 8-bit biased exponent is                : %.8s", fptodecstore->biasedexponent);
        printf("\nd) The 8-bit actual exponent is                : %d", fptodecstore->actexponent);
        printf("\ne) The 23-bit fraction is                      : %.23s", fptodecstore->mantissa);
        printf("\nf) After conversion, the decimal number is     : %.12g", fptodecstore->decresult);

        // Below is to free the memory allocated when not needed
        free(fptodecstore->biasedexponent);
        free(fptodecstore->mantissa);
        free(fptodecstore);
    }
    else
    {
        // Option 1 : To Convert
        printf("\nEnter the decimal value to convert : ");
        scanf("%f", &decnumber);
        binfp = dectobinfp(decnumber);
        printf("\na) The input of decimal number is          : %.10g", decnumber);
        printf("\nb) After conversion, the 1-bit sign bit is : %c", binfp->signbit);
        printf("\nc) The actual exponent is                  : %d", binfp->actexponent);
        printf("\nd) The 8-bit biased exponent is            : %.8s", binfp->biasedexponent);
        printf("\ne) The 23-bit fraction is                  : %.23s", binfp->mantissa);
        printf("\nf) IEEE754 binary floating point           : %c %.8s %.23s", binfp->signbit, binfp->biasedexponent, binfp->mantissa);

        // Below is to free the memory allocated when not needed
        free(binfp->mantissa);
        free(binfp->biasedexponent);
        free(binfp);
    }
}

// ----------------------------------------------------------------------------
// Function below will be to convert decimal to IEEE754 binary floating point
// ----------------------------------------------------------------------------

binfpstruct *dectobinfp(float values)
{
    binfpstruct *result = (binfpstruct *)malloc(sizeof(binfpstruct));
    int size1 = 160;
    float value = fabs(values);
    int expsize = 127; // expoenent size for 32bit ieee745
    int i, length2 = 0, t = 0;

    result->binaryfract = (char *)calloc(size1, sizeof(result->binaryfract));    // To allocate memorry and initialize
    result->binarynonfract = (char *)calloc(size1, sizeof(result->binaryfract)); // To allocate memory and initialize
    converttobin(value, result->binarynonfract, size1, 0);                       // Convert To binary for Integer Part
    converttobin(value, result->binaryfract, size1, 1);                          // Convert To binary for Fractional Part
    result->signbit = values < 0. ? 49 : 48;                                     // Get the Sign Bit
    // Calculate the total length of binary fractional part(binaryfract) and the integer part(binarynonfract)
    int sizeez = strlen(result->binaryfract) + strlen(result->binarynonfract);
    // Allocate memory to mantissa storage
    result->mantissa = (char *)calloc(sizeez, sizeof(result->mantissa));
    // get the length of binary fraction side
    length2 = strlen(result->binaryfract);
    if (value < 1)
    {
        for (i = 0; i < length2; i++)
        {
            if ((int)result->binaryfract[i] == 49)
            {
                t = i;
                expsize = expsize - (i + 1);
                break;
            }
        }
        if (expsize != 0)
        {
            strcpy(result->mantissa, result->binaryfract + t + 1); // copy
        }
        else
        {
            strcpy(result->mantissa, result->binaryfract + t); // copy
        }
    }
    else
    {
        strcpy(result->mantissa, result->binarynonfract + 1); // copy
        strcat(result->mantissa, result->binaryfract);        // concat
        expsize = expsize + (strlen(result->binarynonfract) - 1);
    }

    free(result->binaryfract);     // To free the allocated memory towards binaryfraact
    free(result->binarynonfract);  // To free the allocated memory towards binarynonfract
    result->actexponent = expsize; // Store the exponent size
    if (expsize != 0)
    {
        result->biasedexponent = (char *)calloc(20, sizeof(result->mantissa));
        converttobin((float)expsize, result->biasedexponent, 8, 2); // Convert to Biased Exponent
    }
    else
    {
        result->biasedexponent = (char *)calloc(8, sizeof(result->mantissa));
        memcpy(result->biasedexponent, "00000000", 8);
    }

    return result;
}

// ----------------------------------------------------------------------------
// Function below will be to convert to binary used in decimal to floating point conversion
// ----------------------------------------------------------------------------
void converttobin(float value, char *result, unsigned int len, int option)
{
    float rem = 0, rem2 = 0;    // rem <-- remainder for integer side, rem2 <--- remainder for fractional side
    float v = truncf(value);    // Gets the value of integer part
    float m = value - v;        // value with decimal
    int mcount = 0, tcount = 0; // Counter variable
    //char *reversestr = malloc(sizeof(char) * len);

    // Option 0 : Integer part conversion
    // Option 1 : Fractional part conversion
    // Option 2 : Biased Exponent Binary Conversion
    if (option == 1)
    {
        while (tcount < len)
        {
            //printf("\n TEST 2 : %f", m);
            if (m > 0.)
            {
                m = (m - truncf(m)) * 2;
            }
            else
            {
                m *= 2;
            }
            rem2 = truncf(m);
            result[tcount] = (int)rem2 == 1 ? 49 : 48;
            tcount++;
        }
    }
    else
    {
        if (value >= 1.)
        {

            if (option == 2)
            {
                // For biased expoenent binary
                while (mcount < len)
                {
                    rem = truncf(fmodf(v, 2));
                    result[mcount] = (int)rem == 1 ? 49 : 48;

                    v /= 2;

                    mcount++;
                }
            }
            else
            {
                // For non fraction binary
                while ((int)v > 0)
                {
                    rem = truncf(fmodf(v, 2));
                    result[mcount] = (int)rem == 1 ? 49 : 48;

                    v /= 2;
                    mcount++;
                }
            }
        }

        reverse(result);
    }
}

//================================================================
// Convert from floating point to decimal
//================================================================
binfpstruct *binfptodec(char *values)
{
    binfpstruct *store = (binfpstruct *)malloc(sizeof(binfpstruct));
    store->signbit = values[0];
    int signbits = (int)values[0] == 49 ? 1 : 0;
    int count = 0, indxempt = 0, i;
    int biaseddec = 0;
    double mantissaresult = 0.;
    int tempstore = 0;
    // Allocate memory for biased exponents
    store->biasedexponent = (char *)calloc(8, sizeof(store->mantissa));
    // Allocate memory for mantissa
    store->mantissa = (char *)calloc(strlen(values), sizeof(store->mantissa));
    for (i = 1; i < 37; i++)
    {
        tempstore = (int)values[i];
        if ((int)values[1] != 32)
        {
            // If there is no empty index
            if (i < 9)
            {
                store->biasedexponent[i - 1] = values[i];
                // Check if value is 1
                if (tempstore == 49)
                {
                    biaseddec = biaseddec + (pow(2, (BSIZE - (count + 1))));
                }
                count++;
            }
            else
            {

                if (i == 9)
                {
                    count = 0;
                }
                if ((int)values[i] != 10)
                {
                    store->mantissa[count] = values[i];
                }
                // Check if Value is 1
                if (tempstore == 49)
                {
                    mantissaresult = mantissaresult + (pow(2, ((count + 1) * -1)));
                }
                count++;
            }
        }
        else
        {
            // Floating point with space
            if (i == 1 || i == 10)
            {
                count = 0;
            }
            if (i > 1 && i < 10)
            {
                // first empty space after store biased exponent
                store->biasedexponent[count] = values[i];
                // Check if value is 1
                if (tempstore == 49)
                {
                    biaseddec = biaseddec + (pow(2, (BSIZE - (count + 1))));
                }
                count++;
            }
            if (i > 10)
            {
                // second empty space after to store mantissa
                if ((int)values[i] != 10)
                {
                    store->mantissa[count] = values[i];
                }
                // Check if value is 1
                if (tempstore == 49)
                {
                    mantissaresult = mantissaresult + (pow(2, ((count + 1) * -1)));
                }
                count++;
            }
        }
    }
    biaseddec -= 127;
    store->actexponent = biaseddec;                                                  // Store the actual exponent
    store->decresult = pow(-1, signbits) * (1 + mantissaresult) * pow(2, biaseddec); // To calculate and store the decimal result

    return store;
}

//================================================================
// Function for Reversing of String
//================================================================
char *reverse(char *str)
{
    char temp, *front, *back;

    // Check if str == null
    if (!str || !*str)
    {
        return str;
    }

    // Loop to swap the position of the string
    for (front = str, back = str + strlen(str) - 1; front < back; front++, back--)
    {
        // This is to swap the location of the string
        temp = *front;
        *front = *back;
        *back = temp;
    }
    return str;
}