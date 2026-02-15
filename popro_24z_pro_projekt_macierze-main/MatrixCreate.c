#include "MatrixCreate.h"

// Funkcja do tworzenia nowej macierzy
ptr_matrix createMatrix(ptr_matrix firstMatrix, char *matrixName) {
    uint32_t columns; // Zmienna przechowująca liczbę kolumn
    uint32_t rows;    // Zmienna przechowująca liczbę wierszy

    // Sprawdzanie, czy nazwa macierzy jest poprawna (nie może zaczynać się wielką literą)
    if (isupper(matrixName[0]) || !isalpha(matrixName[0])) {
        fputs(WRONG_MATRIX_NAME_ERR, stderr); // Wyświetlenie komunikatu o błędzie
        return firstMatrix; // Zwrócenie pierwszej macierzy bez zmian
    }

    if(firstMatrix != NULL && nameCheck(firstMatrix, matrixName))
    {
        fputs(MATRIX_EXISTS_ERR, stderr);
        return firstMatrix;
    }

    puts("Tworzenie nowej macierzy."); // Informacja dla użytkownika
    puts("Wszystko co zostanie podane po liczbie zostanie wyczysczone (wczytana zostanie tylko pierwsza liczba) ");
    // Pobieranie liczby wierszy od użytkownika
    puts("Podaj liczbe wierszy:");
    if(scanf("%u", &rows)!=1)
    {
        fputs(INVALID_INPUT_TYPE,stderr);
        clearBuff();
        return firstMatrix;
    }
    clearBuff();

    // Pobieranie liczby kolumn od użytkownika
    puts("Podaj liczbe kolumn");
    if(scanf("%u", &columns)!=1)
    {
        fputs(INVALID_INPUT_TYPE,stderr);
        clearBuff();
        return firstMatrix;
    }
    clearBuff();

    // Tworzenie pustej macierzy o zadanych wymiarach
    ptr_matrix new_matrix = createBlankMatrix(matrixName, rows, columns);

    // Sprawdzenie, czy udało się utworzyć macierz
    if (new_matrix == NULL) {
        return firstMatrix; // Zwrócenie pierwszej macierzy w przypadku niepowodzenia
    }

    // Instrukcja dla użytkownika dotycząca wprowadzania elementów macierzy
    puts("Podaj cialo macierzy zgodnie z instrukcjami, uzywaj liczb zmiennoprzecinkowych (typu double).\nWszystko co zostanie podane po liczbie zostanie wyczysczone (wczytana zostanie tylko pierwsza liczba) ");

    // Pętla wprowadzająca dane do macierzy
    for (uint32_t i = 0; i < rows; i++) {
        printf("%u wiersz:\n", i+1); // Informacja o aktualnym wierszu
        for (uint32_t j = 0; j < columns; j++) {
            printf("Podaj %u wyraz %u wiersza:\n", j+1, i+1); // Prośba o podanie wartości
            if (scanf("%lf", &(new_matrix->tab[i][j])) != 1) { // Sprawdzenie poprawności danych
                fputs(INVALID_INPUT_TYPE,stderr); // Komunikat o błędnym typie danych
                clearBuff();

                freeMatrixMem(new_matrix); // Zwolnienie pamięci macierzy
                return firstMatrix; // Zwrócenie pierwszej macierzy
            }
            clearBuff();
        }
    }

    // Dodanie nowej macierzy do listy i zwrócenie jej
    return putMatrixInList(firstMatrix, new_matrix);
}

// Funkcja do usuwania macierzy na podstawie jej nazwy
ptr_matrix deleteMatrix(char *matrixName, ptr_matrix firstMatrix) {
    // Sprawdzenie, czy lista macierzy jest pusta
    if (firstMatrix == NULL) {
        fputs(MATRIX_NOT_FOUND, stderr); // Komunikat o braku macierzy
        return firstMatrix; // Zwrócenie pustej listy
    }

    // Sprawdzenie, czy istnieje macierz o podanej nazwie
    ptr_matrix current = nameCheck(firstMatrix, matrixName);

    if (current == NULL) {
        fputs(MATRIX_NOT_FOUND, stderr); // Komunikat o braku macierzy
        return firstMatrix; // Zwrócenie listy bez zmian
    }

    // Jeśli usuwana macierz jest pierwsza w liście
    if (current == firstMatrix) {
        ptr_matrix new_firstMatrix = firstMatrix->nast; // Nowa głowa listy
        freeMatrixMem(current); // Zwolnienie pamięci macierzy
        return new_firstMatrix; // Zwrócenie zaktualizowanej listy
    }

    // Przeszukiwanie listy w celu znalezienia poprzednika usuwanej macierzy
    ptr_matrix check_matrix = firstMatrix;
    while (check_matrix->nast != current) {
        check_matrix = check_matrix->nast;
    }

    // Usuwanie macierzy z listy
    check_matrix->nast = current->nast;

    freeMatrixMem(current); // Zwolnienie pamięci macierzy
    return firstMatrix; // Zwrócenie zaktualizowanej listy
}
