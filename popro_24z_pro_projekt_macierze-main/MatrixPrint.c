#include "MatrixPrint.h"

void printMatrix(char *matrixName, ptr_matrix firstMatrix) {

    if(firstMatrix == NULL){
        fputs(MATRIX_NOT_FOUND, stderr); // Komunikat o braku macierzy
        return;
    }

    // Użycie funkcji nameCheck do znalezienia macierzy
    ptr_matrix current = nameCheck(firstMatrix, matrixName);

    if (current == NULL) {
        fputs(MATRIX_NOT_FOUND, stderr); // Komunikat o braku macierzy
        return;
    }

    // Obliczenie maksymalnej liczby cyfr dla poprawnego formatowania
    unsigned int maxCellWidth = biggestDigitsNumber(current);

    // Wyswietlenie macierzy
    printf("Macierz '%s' (%ux%u):\n", current->id, current->m, current->n);

    printf("|");
    for (uint32_t j = 0; j < current->m; j++) {
        for (unsigned int z = 0; z < maxCellWidth + 2; z++) {
            printf("-");
        }
        printf("|");
    }
    printf("\n");

    for (uint32_t i = 0; i < current->n; i++) {
        // Linia pozioma między wierszami
        printf("|");
        for (uint32_t j = 0; j < current->m; j++) {
            printf(" %*.*lf |", maxCellWidth, PRECISION, current->tab[i][j]);
        }
        printf("\n");

        // Dodanie poziomej linii oddzielającej wiersze
        printf("|");
        for (uint32_t j = 0; j < current->m; j++) {
            for (unsigned int z = 0; z < maxCellWidth + 2; z++) {
                printf("-");
            }
            printf("|");
        }
        printf("\n");
    }
}

// wypisywanie listy macierzy
void listMatrices(ptr_matrix firstMatrix) {
    if (firstMatrix == NULL) {
        fputs(NO_MATRIX_IN_LIST, stderr);
        return;
    }

    printf("Lista macierzy w systemie:\n");
    ptr_matrix current = firstMatrix;
    while (current != NULL) {
        size_t memorySize = current->m * current->n * sizeof(double);
        printf("Nazwa: '%s', Wymiary: %ux%u, Pamiec: %zu bajtow\n",
               current->id, current->m, current->n, memorySize);
        current = current->nast;
    }
}