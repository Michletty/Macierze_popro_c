#include "MatrixFile.h"

// Funkcja załadowuje macierz z pliku binarnego, nadaje jej nazwę w systemie i wstawia w odpowiednie miejsce
// w liście
// fileName - nazwa pliku, matrixName - nadana nazwa macierzy, firstMatrix - pointer na head'a listy
ptr_matrix load(const char *fileName, char *matrixName, ptr_matrix firstMatrix)
{
    // Sprawdzanie poprawności nazwy (Czy pierwsza litera mała i czy imię nie jest zajętę)

    if(isupper(matrixName[0]) || !isalpha(matrixName[0]))
    {
        fputs(WRONG_MATRIX_NAME_ERR, stderr);
        return firstMatrix;
    }

    // Sprawdzenie czy podany plik ma odpowiednie rozszerzenie
    if(strncmp(fileName + strlen(fileName)-4, ".bin", 4))
    {
        fputs(WRONG_FILE_EXTENSION, stderr);
        return firstMatrix;
    }
    
    if(firstMatrix != NULL && nameCheck(firstMatrix, matrixName))
    {
        fputs(MATRIX_EXISTS_ERR, stderr);
        return firstMatrix;
    }

    char *PATH = (char*)malloc(strlen(fileName)+7);
    strcpy(PATH,"");
    strcat(PATH, "files\\");
    strcat(PATH, fileName);

    FILE *fptr = fopen(PATH, "rb");

    if(checkPointerErr(fptr, FILE_MODE))
    {
        return firstMatrix;
    }

    free(PATH);

    uint32_t rows;
    uint32_t columns;

    // Sprawdzanie rozmiaru pliku i jego poprawności
    // oraz określenie ilości kolumn macierzy

    fseek(fptr, 0, SEEK_END);
    unsigned long long int columns_size_in_bytes = ftell(fptr) - sizeof(uint32_t);
    rewind(fptr);

    fread(&rows, sizeof(uint32_t), 1, fptr);

    if((columns_size_in_bytes % (rows * sizeof(double))) == 0)
    {
        columns = (uint32_t)(columns_size_in_bytes / (rows * sizeof(double)));
    }
    else
    {
        fclose(fptr);
        fputs(INVALID_BIN_FILE, stderr);
        return firstMatrix;
    }

    //Tworzenie macierzy wypełnionej zerami
    ptr_matrix newMatrix = createBlankMatrix(matrixName, rows, columns);

    // Edytowanie nowej macierzy
    if(newMatrix != NULL)
    {
        for(uint32_t j = 0; j < newMatrix->m; j++)
        {
           for(uint32_t i = 0; i < newMatrix->n; i++)
            {
              fread(&(newMatrix->tab[i][j]), sizeof(double), 1, fptr);
            }
        }
    }
    else
    {
        fclose(fptr);
        return firstMatrix;
    }

    fclose(fptr);

    // Wstawienie macierzy do listy
    return putMatrixInList(firstMatrix, newMatrix);
}

// Funkcja zapisuje macierz o podanej nazwię do pliku binarengo
// fileName - nazwa pliku, matrixName - nazwa macierzy, firstMatrix - pointer na head'a listy
void save(const char *fileName, char *matrixName, ptr_matrix firstMatrix)
{
    // Sprawdzenie czy podany plik ma odpowiednie rozszerzenie
    if(strncmp(fileName + strlen(fileName)-4, ".bin", 4))
    {
        fputs(WRONG_FILE_EXTENSION, stderr);
        return;
    }

    ptr_matrix exampleMatrix;

    // Wybranie z listy macierzy o podanej nazwię, jeżeli macierzy nie ma w systemie wyrzuca błąd
    if(firstMatrix == NULL || !(exampleMatrix = nameCheck(firstMatrix, matrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return;
    }

    char *PATH = (char*)malloc(strlen(fileName)+7);
    strcpy(PATH,"");
    strcat(PATH, "files\\");
    strcat(PATH, fileName);

    FILE *fptr = fopen(PATH, "wb");

    if(checkPointerErr(fptr, FILE_MODE))
    {
        return;
    }

    free(PATH);

    // Zapisanie macierzy do pliku binarnego
    fwrite(&exampleMatrix->n, sizeof(uint32_t), 1, fptr);

    for(uint32_t j = 0; j < exampleMatrix->m; j++)
    {
        for(uint32_t i = 0; i < exampleMatrix->n; i++)
        {
            fwrite(&(exampleMatrix->tab[i][j]), sizeof(double), 1, fptr);
        }
    }

    fclose(fptr);
}

// Funkcja zapisuje macierz o podanej nazwię do pliku tekstowego
// fileName - nazwa pliku, matrixName - nazwa macierzy, firstMatrix - pointer na head'a listy
void txt(const char *fileName, char *matrixName, ptr_matrix firstMatrix)
{
    // Sprawdzenie czy podany plik ma odpowiednie rozszerzenie
    if(strncmp(fileName + strlen(fileName)-4, ".txt", 4))
    {
        fputs(WRONG_FILE_EXTENSION, stderr);
        return;
    }

    ptr_matrix exampleMatrix;

    // Wybranie z listy macierzy o podanej nazwię, jeżeli macierzy nie ma w systemie wyrzuca błąd
    if(firstMatrix == NULL || !(exampleMatrix = nameCheck(firstMatrix, matrixName)))
    {
        fputs(MATRIX_NOT_FOUND, stderr);
        return;
    }

    // Ustalenie szerokości komórki
    unsigned int size_of_cell = biggestDigitsNumber(exampleMatrix);

    char *PATH = (char*)malloc(strlen(fileName)+7);
    strcpy(PATH,"");
    strcat(PATH, "files\\");
    strcat(PATH, fileName);

    FILE *fptr = fopen(PATH, "w");

    if(checkPointerErr(fptr, FILE_MODE))
    {
        return;
    }

    free(PATH);

    // Zapisanie macierzy w czytelny sposób do pliku txt
    
    fprintf(fptr, "%s\n", exampleMatrix->id);

    filePrintMatrixLine(fptr, exampleMatrix, size_of_cell);

    for(uint32_t i = 0; i < exampleMatrix->n; i++)
    {
        fputs("|", fptr);
        for(uint32_t j = 0; j < exampleMatrix->m; j++)
        {
            fprintf(fptr, "%-*.*lf", size_of_cell, PRECISION, exampleMatrix->tab[i][j]);
            fputs("|", fptr);
        }
        fputs("\n", fptr);
        filePrintMatrixLine(fptr, exampleMatrix, size_of_cell);
    }

    fclose(fptr);
}

// Funkcja przekierowuje strumień wejścia do pliku oraz wraca do standardowego strumienia
// zależnie od MODE, READ_FILE - zmiana strumienia na plik , CLOSE_FILE - zmiana strumienia na standardowe
void doFile(const char *fileName, int original_stdin_fd, enum TYPE MODE)
{

    if(MODE == READ_FILE)
    {
        // Sprawdzenie czy podany plik ma odpowiednie rozszerzenie
        if(strncmp(fileName + strlen(fileName)-4, ".txt", 4))
        {
            fputs(WRONG_FILE_EXTENSION, stderr);
            return;
        }

        // Sprawdzanie czy deskryptor do standardowego wyjścia jest dobrze zapisany
        if(original_stdin_fd == -1) {
            fputs(FILE_READ_ERROR, stderr);
            return;
        }

        char *PATH = (char*)malloc(strlen(fileName)+7);
        strcpy(PATH,"");
        strcat(PATH, "files\\");
        strcat(PATH, fileName);

        FILE *append_ptr = fopen(PATH, "a");

        if(checkPointerErr(append_ptr, FILE_MODE))
        {
            return;
        }
        
        fprintf(append_ptr, "\n");

        fclose(append_ptr);

        FILE *fptr = fopen(PATH, "r");

        if(checkPointerErr(fptr, FILE_MODE))
        {
            return;
        }
        
        free(PATH);

        // Kopiowanie deskryptora pliku do deskryptora standardowego wejścia
        if(dup2(fileno(fptr), fileno(stdin)) == -1) {
            fputs(FILE_READ_ERROR, stderr);
            fclose(fptr);
            return;
        }

        // Możemy zamknąć plik poniewać deskryptor pliku działa już niezależnie od pointera na plik
        fclose(fptr);
    }
    if(MODE == CLOSE_FILE)
    {  
        // Wracanie do standardowego strumienia wejścia
        if (dup2(original_stdin_fd, fileno(stdin)) == -1) {
            fputs(FILE_READ_ERROR, stderr);
            return;
        }
    }
}

