#ifndef MATRIXCREATE_H
#define MATRIXCREATE_H

#include "MatrixCommon.h"
#include "MatrixHelpFunc.h"
ptr_matrix createMatrix(ptr_matrix firstMatrix, char* matrixName);
ptr_matrix deleteMatrix(char *matrixName, ptr_matrix firstMatrix);
#endif