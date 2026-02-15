#ifndef MATRIXFILE_H
#define MATRIXFILE_H

#include "MatrixCommon.h"
#include "MatrixHelpFunc.h"
ptr_matrix load(const char *fileName, char *matrixName, ptr_matrix firstMatrix);
void save(const char *fileName, char *matrixName, ptr_matrix firstMatrix);
void txt(const char *fileName, char *matrixName, ptr_matrix firstMatrix);
void doFile(const char *fileName, int original_stdin_fd, enum TYPE MODE);
#endif