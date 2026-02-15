#ifndef MATRIXCOMMON_H
#define MATRIXCOMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include "ErrorCodes.h"

#define PRECISION 2


enum TYPE
{
    FILE_MODE,
    MATRIX_MODE,
    READ_FILE,
    CLOSE_FILE,
};

struct matrix{
    uint32_t m;
    uint32_t n;
    double **tab;
    char *id;
    struct matrix *nast;
};

typedef struct matrix* ptr_matrix;

#endif