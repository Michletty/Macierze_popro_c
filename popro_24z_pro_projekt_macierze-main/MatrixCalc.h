#ifndef MATRIXCALC_H
#define MATRIXCALC_H

#include "MatrixCommon.h"
#include "MatrixHelpFunc.h"

ptr_matrix sumMatrix(char *newMatrixName,char *firstMatrixName,  char *secondMatrixName, ptr_matrix firstMatrix);
ptr_matrix multiplyMatrices(char *newMatrixName,char *firstMatrixName,char *secondMatrixName,ptr_matrix firstMatrix);
ptr_matrix matricesVertically(char *newMatrixName,char *topMatrixName, char *bottomMatrixName, ptr_matrix firstMatrix);
#endif

