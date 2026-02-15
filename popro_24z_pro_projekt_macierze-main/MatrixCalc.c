#include "MatrixCalc.h"


ptr_matrix sumMatrix(char *newMatrixName,char *firstMatrixName,  char *secondMatrixName, ptr_matrix firstMatrix)
{
    // Sprawdzanie poprawności nazwy

    if(isupper(newMatrixName[0]) || !isalpha(newMatrixName[0]))
    {
        fputs(WRONG_MATRIX_NAME_ERR, stderr);
        return firstMatrix;
    }

    if(firstMatrix == NULL){
         fputs(MATRIX_NOT_FOUND, stderr); // Komunikat o braku macierzy
         return firstMatrix;
    }

    // Sprawdzenie, czy nazwa nowej macierzy nie jest już zajęta
    if (nameCheck(firstMatrix, newMatrixName) != NULL)
    {
        fputs(MATRIX_EXISTS_ERR, stderr);
        return firstMatrix; // nic nie zmieniamy, zwracamy za to naszą pierwotną macierz
    }

    // Sprawdzenie, czy mamy w ogóle listę macierzy oraz czy istnieją podane macierze
    ptr_matrix matrix1 = NULL;
    ptr_matrix matrix2 = NULL;

    if (!firstMatrix || !(matrix1 = nameCheck(firstMatrix, firstMatrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return firstMatrix;
    }
    if (!(matrix2 = nameCheck(firstMatrix, secondMatrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return firstMatrix;
    }

    // Sprawdzenie zgodności wymiarów (muszą być identyczne)
    if (matrix1->n != matrix2->n || matrix1->m != matrix2->m)
    {
        fputs(MATRIX_DIMENSION_ERR, stderr);
        return firstMatrix;
    }

    // Nowa macierz o tych samych wymiarach
    ptr_matrix result = createBlankMatrix(newMatrixName, matrix1->n, matrix1->m);
    if (!result)
    {
        // Informacja o błędzie alokacji jest w createBlankMatrix więc zostawiam resztę tej funkcji
        return firstMatrix;
    }

    // Sumowanie elementów
    for (uint32_t i = 0; i < result->n; i++)
    {
        for (uint32_t j = 0; j < result->m; j++)
        {
            if(checkAddOverflow(matrix1->tab[i][j],matrix2->tab[i][j]))
            {
                    fputs(MATRIX_DOUBLE_OVERFLOW,stderr);
                    freeMatrixMem(result);
                    return firstMatrix;
            }
            result->tab[i][j] = matrix1->tab[i][j] + matrix2->tab[i][j];
        }
    }

    // Dodanie nowej macierzy do listy i zwrócenie zaktualizowanego wskaźnika
    firstMatrix = putMatrixInList(firstMatrix, result);
    return firstMatrix;
}

ptr_matrix multiplyMatrices(char *newMatrixName,char *firstMatrixName,char *secondMatrixName,ptr_matrix firstMatrix)
{
    // Sprawdzanie poprawności nazwy

    if(isupper(newMatrixName[0]) || !isalpha(newMatrixName[0]))
    {
        fputs(WRONG_MATRIX_NAME_ERR, stderr);
        return firstMatrix;
    }

    if(firstMatrix == NULL){
        fputs(MATRIX_NOT_FOUND, stderr); // Komunikat o braku macierzy
        return firstMatrix;
    }

    // Sprawdzenie, czy nowa nazwa macierzy nie jest już zajęta
    if (nameCheck(firstMatrix, newMatrixName) != NULL)
    {
        fputs(MATRIX_EXISTS_ERR, stderr);
        return firstMatrix;
    }

    // Sprawdzenie istnienia macierzy w liście
    ptr_matrix matrix1 = NULL;
    ptr_matrix matrix2 = NULL;

    if (!firstMatrix || !(matrix1 = nameCheck(firstMatrix, firstMatrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return firstMatrix;
    }
    if (!(matrix2 = nameCheck(firstMatrix, secondMatrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return firstMatrix;
    }

    // Sprawdzenie czy wymiary pozwalają na mnożenie (A[n x m], B[m x p] => C[n x p])
    if (matrix1->m != matrix2->n)
    {
        fputs(MATRIX_DIMENSION_ERR, stderr);
        return firstMatrix;
    }

    // Tworzymy nową macierz wynikową o wymiarach n x p
    uint32_t n = matrix1->n;
    uint32_t p = matrix2->m;
    ptr_matrix result = createBlankMatrix(newMatrixName, n, p);
    if (!result)
    {
        return firstMatrix;
    }

    // Mnożenie macierzy 
    for (uint32_t i = 0; i < n; i++)
    {
        for (uint32_t j = 0; j < p; j++)
        {
            double sum = 0.0;
            for (uint32_t k = 0; k < matrix1->m; k++)
            {
                if(checkMultiOverflow(matrix1->tab[i][k],matrix2->tab[k][j]))
                {
                    fputs(MATRIX_DOUBLE_OVERFLOW,stderr);
                    freeMatrixMem(result);
                    return firstMatrix;
                }
                if(checkAddOverflow(sum,matrix1->tab[i][k]*matrix2->tab[k][j]))
                {
                    fputs(MATRIX_DOUBLE_OVERFLOW,stderr);
                    freeMatrixMem(result);
                    return firstMatrix;
                }
                sum += matrix1->tab[i][k] * matrix2->tab[k][j];
            }
            result->tab[i][j] = sum;
        }
    }

    // Dodajemy nową macierz do listy, zwracamy nową głowę
    firstMatrix = putMatrixInList(firstMatrix, result);
    return firstMatrix;
}

ptr_matrix matricesVertically(char *newMatrixName,char *topMatrixName, char *bottomMatrixName, ptr_matrix firstMatrix)
{
    // Sprawdzanie poprawności nazwy

    if(isupper(newMatrixName[0]) || !isalpha(newMatrixName[0]))
    {
        fputs(WRONG_MATRIX_NAME_ERR, stderr);
        return firstMatrix;
    }

    if(firstMatrix == NULL){
        fputs(MATRIX_NOT_FOUND, stderr); // Komunikat o braku macierzy
        return firstMatrix;
    }

    // Sprawdzamy czy nowa nazwa macierzy nie jest już zajęta
    if (nameCheck(firstMatrix, newMatrixName) != NULL)
    {
        fputs(MATRIX_EXISTS_ERR, stderr);
        return firstMatrix;
    }

    // Wyszukujemy macierze w liście
    ptr_matrix matrix1 = NULL;
    ptr_matrix matrix2 = NULL;

    if (!firstMatrix || !(matrix1 = nameCheck(firstMatrix, topMatrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return firstMatrix;
    }
    if (!(matrix2 = nameCheck(firstMatrix, bottomMatrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return firstMatrix;
    }

    // Sprawdzamy liczbę kolumn (m), musi być taka sama
    if (matrix1->m != matrix2->m)
    {
        fputs(MATRIX_COLUMN_ERR, stderr);
        return firstMatrix;
    }

    // Nowa macierz: liczba wierszy = suma wierszy obu macierzy, liczba kolumn = matrix1->m
    uint32_t newRows = matrix1->n + matrix2->n;
    uint32_t cols = matrix1->m;

    ptr_matrix result = createBlankMatrix(newMatrixName, newRows, cols);
    if (!result)
    {
        return firstMatrix;
    }

    // Przepisanie danych z matrix1
    for (uint32_t i = 0; i < matrix1->n; i++)
    {
        for (uint32_t j = 0; j < cols; j++)
        {
            result->tab[i][j] = matrix1->tab[i][j];
        }
    }

    // Przepisanie danych z matrix2 (od wiersza matrix1)
    for (uint32_t i = 0; i < matrix2->n; i++)
    {
        for (uint32_t j = 0; j < cols; j++)
        {
            result->tab[matrix1->n + i][j] = matrix2->tab[i][j];
        }
    }

    // Dodanie nowej macierzy do listy i zwrócenie zaktualizowanego wskaźnika
    firstMatrix = putMatrixInList(firstMatrix, result);
    return firstMatrix;
}

