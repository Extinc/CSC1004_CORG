// =================================================================
// Q1 Assignments
// This C code char int value is represented through ASCII TABLE
// Char Comparison using ASCII TABLE
// Latest working copy as of 3 Nov 2021
// This program
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
    int actexponent;
    char signbit;
    char *biasedexponent;
    char *mantissa;
    char *binarynonfract;
    char *binaryfract;
    double decresult;
} binfpstruct;

char *strrev(char *str);
void converttobin(float val, char *binarynonfract, unsigned int len, int option);
binfpstruct *dectobinfp(float values); // To convert Decimal to IEEE 754 floating point
binfpstruct *binfptodec(char *values); // To Convert IEEE 754 floating point to decimal
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
        printf("\nEnter the 32-bits binary number in IEEE 754 floating point format : ");
        fgets(binnum, sizeof(binnum), stdin);
        /*         while (fgets(binnum, 40, stdin) != NULL)
        {
            fflush(stdin);
        } */
        fptodecstore = binfptodec(binnum);
        printf("\na) IEEE 754 floating point input               : %.34s", binnum);
        printf("\nb) Before conversion, the 1-bit sign bit is    : %c", fptodecstore->signbit);
        printf("\nc) The 8-bit biased exponent is                : %.8s", fptodecstore->biasedexponent); // to add in parameter for actual exponent
        printf("\nd) The 8-bit actual exponent is                : %d", fptodecstore->actexponent);
        printf("\ne) The 23-bit fraction is                      : %.23s", fptodecstore->mantissa);
        printf("\nf) After conversion, the decimal number is     : %.12g", fptodecstore->decresult);
        free(fptodecstore->biasedexponent);
        free(fptodecstore->mantissa);
        
        free(fptodecstore);
    }
    else
    {
        printf("\nEnter the number : ");
        scanf("%f", &decnumber);
        binfp = dectobinfp(decnumber);
        printf("\na) The input of decimal number is          : %e", decnumber);
        printf("\nb) After conversion, the 1-bit sign bit is : %c", binfp->signbit);
        printf("\nc) The actual exponent is                  : %d", binfp->actexponent); // to add in parameter for actual exponent
        printf("\nd) The 8-bit biased exponent is            : %.8s", binfp->biasedexponent);
        printf("\ne) The 23-bit fraction is                  : %.23s", binfp->mantissa);
        printf("\nf) IEEE754 binary floating point           : %c %.8s %.23s", binfp->signbit, binfp->biasedexponent, binfp->mantissa);
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
    int i, length2 = 0, t=0;;
    //zeroarray(result.binarynonfract, 32);
    result->binaryfract = (char *)calloc(size1, sizeof(result->binaryfract));
    result->binarynonfract = (char *)calloc(size1, sizeof(result->binaryfract));
    converttobin(value, result->binarynonfract, size1, 0);
    converttobin(value, result->binaryfract, size1, 1);
    result->signbit = values < 0. ? 49 : 48;
    int sizeez = strlen(result->binaryfract) + strlen(result->binarynonfract);
    result->mantissa = (char *)calloc(sizeez, sizeof(result->mantissa));
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
        if(expsize != 0){
            strcpy(result->mantissa, result->binaryfract + t + 1); // copy
        }else{
            strcpy(result->mantissa, result->binaryfract + t); // copy
        }
    }
    else
    {
        strcpy(result->mantissa, result->binarynonfract + 1); // copy
        strcat(result->mantissa, result->binaryfract);        // concat
        expsize = expsize + (strlen(result->binarynonfract) - 1);
    }

    //memcpy(result->mantissa, binstore, strlen(result->binaryfract) + strlen(result->binarynonfract));
    free(result->binaryfract);
    free(result->binarynonfract);
    result->actexponent = expsize;
    if (expsize != 0)
    {
        result->biasedexponent = (char *)calloc(20, sizeof(result->mantissa));
        converttobin((float)expsize, result->biasedexponent, 8, 2);
    }
    else
    {
        result->biasedexponent = (char *)calloc(8, sizeof(result->mantissa));
        memcpy(result->biasedexponent, "00000000", 8);
    }

    //printf(" TTEEE : %lu", strlen(result.binarynonfract));

    return result;
}

// To convert to binary
// Option : 1 To get with fraction //
void converttobin(float value, char *result, unsigned int len, int option)
{
    float rem = 0, rem2 = 0; // rem <-- remainder for non fraction side, rem2 <--- remainder for fraction side
    float v = truncf(value);
    float m = value - v; // value with decimal
    int mcount = 0, tcount = 0;
    //char *reversestr = malloc(sizeof(char) * len);

    // Option 1 : Fraction Option2 : Non Decimal to binary
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
            //printf("\n TEST 2 11 : %f", m);
            tcount++;
        }
    }
    else
    {
        if (value >= 1.)
        {

            // For biased expoenent binary
            if (option == 2)
            {
                while (mcount < len)
                {
                    //printf("\n TEST 2 : %f", rem);
                    rem = truncf(fmodf(v, 2));
                    //printf("\n TEST 1 : %f", rem);
                    result[mcount] = (int)rem == 1 ? 49 : 48;

                    v /= 2;
                    //printf("\n TEST 2 : %f", n);
                    mcount++;
                }
            }
            else
            {
                // For non fraction binary
                while ((int)v > 0)
                {
                    // printf("\n TEST 2 : %f", rem);
                    rem = truncf(fmodf(v, 2));
                    //printf("\n TEST 1 : %f", rem);
                    result[mcount] = (int)rem == 1 ? 49 : 48;

                    v /= 2;
                    //printf("\n TEST 2 : %f", n);
                    mcount++;
                }
            }
        }

        strrev(result);
    }
}

char *strrev(char *str)
{
    char c, *front, *back;

    if (!str || !*str)
        return str;
    for (front = str, back = str + strlen(str) - 1; front < back; front++, back--)
    {
        c = *front;
        *front = *back;
        *back = c;
    }
    return str;
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
    store->biasedexponent = (char *)calloc(8, sizeof(store->mantissa));
    store->mantissa = (char *)calloc(strlen(values), sizeof(store->mantissa));
    int sizetest = sizeof(store);
    printf("\test  :%d", sizetest);
    // Array to store Data into object name : store
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
                if (tempstore == 49)
                {
                    mantissaresult = mantissaresult + (pow(2, ((count + 1) * -1)));
                }
                count++;
            }
        }
    }
    //printf("\nBIASED DECIMAL FORMAT b4 : %d", biaseddec);
    biaseddec -= 127;
    store->actexponent = biaseddec;
    //printf("\nBIASED DECIMAL FORMAT : %lf", mantissaresult);
    store->decresult = pow(-1, signbits) * (1 + mantissaresult) * pow(2, biaseddec);

    return store;
}