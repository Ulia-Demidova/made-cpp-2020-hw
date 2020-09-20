#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    /// Your code here...
    uint64_t mask = 1 << index;
    uint64_t temp = mask & number;
    temp >>= index;
    return temp;
}

bool sign (const uint64_t number) {
    return getBit(number, 63);
}

bool isAllExponent (const uint64_t number, uint8_t check_num) {
    uint64_t exponent = 0x7FF0000000000000;
    uint64_t temp = number & exponent;
    if check_num
        return temp == exponent;
    else
        return temp == 0x0000000000000000;
}

bool checkNormal (const uint64_t number) {
    return !isAllExponent(number, 0) && !isAllExponent(number, 1);
}

bool checkDenormal (const uint64_t number) {
    return isAllExponent(number, 0) && getBit(number, 0);
}

/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    /// Your code here.
    return number == 0x0000000000000000;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf (uint64_t number) {
    /// Your code here.
    return number == 0x7FF0000000000000;
}

bool checkForMinusInf (uint64_t number) {
    /// Your code here.
    return number == 0xFFF0000000000000;
}

bool checkForPlusNormal (uint64_t number) {
    /// Your code here.
    return !sign(number) && checkNormal(number);
}

bool checkForMinusNormal (uint64_t number) {
    /// Your code here.
    return sign(number) && checkNormal(number);
}

bool checkForPlusDenormal (uint64_t number) {
    /// Your code here.
    return !sign(number) && checkDenormal(number);
}

bool checkForMinusDenormal (uint64_t number) {
    /// Your code here.
    return sign(number) && checkDenormal(number);
}

bool checkForSignalingNan (uint64_t number) {
    /// Your code here.
    return isAllExponent(number, 1) && !getBit(number, 51) && (number<<12) != 0x0000000000000000;
}

bool checkForQuietNan (uint64_t number) {
    /// Your code here.
    return isAllExponent(number, 1) && getBit(number, 51);
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
