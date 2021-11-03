// =================================================================
// Q1 Assignments
// This C code char int value is represented through ASCII TABLE
// Char Comparison using ASCII TABLE
// =================================================================
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define MAX 32
#define LEN(x) (sizeof(x) / sizeof((x)[0]))
#define BSIZE 8
#define MSIZE 23

typedef struct
{
    int actexponent;
    char signbit;
    char biasedexponent[8];
    char mantissa[23];
    char binarynonfract[32];
    char binaryfract[32];
    double decresult;
} binfpstruct;

void converttobin(double val, char binarynonfract[], char binaryfract[], unsigned int len);
binfpstruct dectobinfp(float values); // To convert Decimal to IEEE 754 floating point
binfpstruct binfptodec(char *values); // To Convert IEEE 754 floating point to decimal
int main()
{
    int choice = 0;
    double decnumber = 0.;
    binfpstruct binfp = {};
    char binnum[37] = {};
    binfpstruct fptodecstore;
    printf("Choose an option:");
    printf("\n1. Decimal to IEEE 754 Binary floating point \n2. IEEE 754 Binary floating point to Decimal");
    printf("\nEnter your choice : ");
    scanf("%d", &choice);
    fflush(stdin); // Flush the output buffer of the stream
    if (choice == 2)
    {
        printf("\nEnter the 32-bits binary number in IEEE 754 floating point format : ");
        fgets(binnum, 40, stdin);
        /*         while (fgets(binnum, 40, stdin) != NULL)
        {
            fflush(stdin);
        } */
        fptodecstore = binfptodec(binnum);
        printf("\na) IEEE 754 floating point input               : %.34s", binnum);
        printf("\nb) Before conversion, the 1-bit sign bit is    : %c", fptodecstore.signbit);
        printf("\nc) The 8-bit biased exponent is                : %d", fptodecstore.actexponent); // to add in parameter for actual exponent
        printf("\nd) The 8-bit actual exponent is                : %.8s", fptodecstore.biasedexponent);
        printf("\ne) The 23-bit fraction is                      : %.23s", fptodecstore.mantissa);
        printf("\nf) After conversion, the decimal number is     : %lf", fptodecstore.decresult);
        // free(binnum);
    }
    else
    {
        //free(binnum);
        printf("\nEnter the number : ");
        scanf("%lf", &decnumber);
        binfp = dectobinfp(decnumber);
        printf("\na) The input of decimal number is          : %lf", decnumber);
        printf("\nb) After conversion, the 1-bit sign bit is : %c", binfp.signbit);
        printf("\nc) The actual exponent is                  : %d", binfp.actexponent); // to add in parameter for actual exponent
        printf("\nd) The 8-bit biased exponent is            : %.8s", binfp.biasedexponent);
        printf("\ne) The 23-bit fraction is                  : %.23s", binfp.mantissa);
        printf("\nf) IEEE754 binary floating point           : %c %.8s %.23s", binfp.signbit, binfp.biasedexponent, binfp.mantissa);
    }
}

// ----------------------------------------------------------------
// Function below will be to convert decimal to IEEE754 binary floating point
binfpstruct dectobinfp(float values)
{
    binfpstruct result = {.actexponent = 0, .biasedexponent = {}, .mantissa = {}, .binaryfract = {}, .binarynonfract = {}};
    int size1 = 200;
    int size2 = size1 / 2;
    char *binstore;
    double value1 = fabs((double)values);
    int expsize = 127;             // expoenent size for 32bit ieee745
    char temp[1] = {};             // using as a empty storage only
    int indxones = 0, counter = 0; // Index of decimal, Index of last ones
    int i, pos;                    // pos = first position of value 1 in non fract
    if (values < 1E+10 && values > 1E-1)
    {
        size1 = 64;
        size2 = size1 / 2;
    }

    binstore = malloc(sizeof(char) * size1);
    converttobin(value1, result.binarynonfract, result.binaryfract, size2);
    result.signbit = values < 0. ? 49 : 48;

    // combine 2 array
    for (i = 0; i < size1; ++i)
    {
        if (i < size2)
        {
            binstore[i] = result.binarynonfract[i];
        }
        else if (i == size2)
        {
            binstore[i] = 46;
        }
        else
        {
            binstore[i] = result.binaryfract[i - 33];
        }
    }

    // Get the position of first value 1
    for (i = 0; i < size1; ++i)
    {
        // if the value is 1
        if (binstore[i] == 49)
        {
            pos = i;
            break;
        }
    }

    if (pos < size2)
    {
        for (i = pos + 1; i < size1; ++i)
        {
            // if the value is .
            if (binstore[i] != 46)
            {
                result.mantissa[counter] = binstore[i];
                counter++;
            }
        }
        pos = size2 - pos - 1;
        expsize = expsize + pos;
        result.actexponent = pos;
    }
    else if (pos > size2)
    {
        for (i = pos + 1; i < size1; ++i)
        {
            result.mantissa[counter] = binstore[i];
            counter++;
        }
        pos = pos - size2;

        expsize = expsize - pos;
        result.actexponent = pos;
    }
    converttobin(expsize, result.biasedexponent, temp, 8);
    free(binstore);
    return result;
}

// To convert to binary
// Option : 1 To get with fraction //
void converttobin(double value, char binarynonfract[], char binaryfract[], unsigned int len)
{

    double bin = 0.;
    double rem = 0, rem2 = 0; // rem <-- remainder for non fraction side, rem2 <--- remainder for fraction side
    double n = trunc(value);  // n value without decimal place
    double m = value - n;     // value with decimal
    int mcount = 0, tcount = 0;
    while (mcount < len)
    {
        rem = trunc(fmod(n, 2));
        if (m > 0.)
        {
            m = (m - trunc(m)) * 2;
        }
        else
        {
            m *= 2;
        }
        rem2 = trunc(m);
        binaryfract[mcount] = rem2 == 1 ? 49 : 48;
        if (n != 0)
        {
            binarynonfract[(len - 1) - mcount] = rem == 1 ? 49 : 48;
            n /= 2;
        }
        else
        {
            binarynonfract[(len - 1) - mcount] = 48;
        }
        mcount++;
    }
}

//================================================================
// Convert from floating point to decimal
//================================================================
binfpstruct binfptodec(char *values)
{
    binfpstruct store = {};
    store.signbit = values[0];
    int signbits = (int)values[0] == 49 ? 1 : 0;
    int count = 0, indxempt = 0, i;
    int biaseddec = 0;
    double mantissaresult = 0.;
    int tempstore = 0;
    // Array to store Data into object name : store
    for (i = 1; i < 37; i++)
    {
        tempstore = (int)values[i];
        if ((int)values[i] != 32)
        {
            //printf("\n SUCCESS (Func) index : %d  - %c", i, values[i]);
            if (i > 0 && indxempt < i)
            {
                if (indxempt == 0)
                {
                    // If there is no empty index
                    if (i < 9)
                    {
                        store.biasedexponent[i - 1] = values[i];
                        // Check if value is 1
                        if (tempstore == 49)
                        {
                            biaseddec = biaseddec + (pow(2, (BSIZE - (count + 1))));
                        }
                        count++;
                    }
                    else
                    {
                        store.mantissa[i - 1] = values[i];
                        if (i == 9)
                        {
                            count = 0;
                        }
                        if (tempstore == 49)
                        {
                            mantissaresult = mantissaresult + (pow(2, ((count + 1) * -1)));
                        }
                        count++;
                    }
                }
                else if (indxempt == 1)
                {
                    // first empty space after store biased exponent
                    store.biasedexponent[count] = values[i];
                    // Check if value is 1
                    if (tempstore == 49)
                    {
                        biaseddec = biaseddec + (pow(2, (BSIZE - (count + 1))));
                    }
                    count++;
                }
                else if (indxempt == 10)
                {
                    // second empty space after to store mantissa

                    store.mantissa[count] = values[i];
                    if (tempstore == 49)
                    {
                        mantissaresult = mantissaresult + (pow(2, ((count + 1) * -1)));
                    }
                    count++;
                }
            }
        }
        else
        {
            count = 0;
            indxempt = i;
        }
    }

    //printf("\nBIASED DECIMAL FORMAT b4 : %d", biaseddec);
    biaseddec -= 127;
    store.actexponent = biaseddec;
    //printf("\nBIASED DECIMAL FORMAT : %lf", mantissaresult);
    store.decresult = pow(-1, signbits) * (1 + mantissaresult) * pow(2, biaseddec);

    return store;
}