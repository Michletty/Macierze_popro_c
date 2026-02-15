#ifndef MATRIXHELPFUNC_H
#define MATRIXHELPFUNC_H

#include "MatrixCommon.h"

bool checkPointerErr(void *ptr, enum TYPE mode);
ptr_matrix nameCheck(ptr_matrix firstMatrix, char *matrixName);
unsigned int digitsNumber(double x);
unsigned int biggestDigitsNumber(ptr_matrix exampleMatrix);
void filePrintMatrixLine(FILE *fptr, ptr_matrix exampleMatrix, unsigned int size_of_cell);
void freeMatrixMem(ptr_matrix exampleMatrix);
ptr_matrix createBlankMatrix(char *matrixName, uint32_t rows, uint32_t columns);
ptr_matrix putMatrixInList(ptr_matrix firstMatrix, ptr_matrix exampleMatrix);
int checkMultiOverflow(double a, double b);
int checkAddOverflow(double a, double b);
void clearBuff(void);

#endif