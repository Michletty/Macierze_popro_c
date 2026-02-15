#ifndef MATRIXPRINT_H
#define MATRIXPRINT_H

#include "MatrixCommon.h"
#include "MatrixHelpFunc.h"

// Funkcja wypisuje macierz o podanej nazwie na ekranie.
// parametr matrixName - Nazwa macierzy, ktora ma zostac wypisana.
// parametr firstMatrix - Wskaznik na pierwsza macierz w liscie polaczonej.
void printMatrix(char *matrixName, ptr_matrix firstMatrix);

// Funkcja wypisuje liste wszystkich macierzy w systemie na ekranie.
// Wyswietla ich nazwy, wymiary oraz rozmiar pamieci w bajtach.
// parametr firstMatrix - Wskaznik na pierwsza macierz w liscie polaczonej.
void listMatrices(ptr_matrix firstMatrix);

#endif