#include "MatrixHelpFunc.h"

// Funckja sprawdza poprawną alokacje lub otworzenie pliku, zależnie od trybu zwraca odpowiedni błąd
// MODE do wyboru MATRIX_MODE - sprawdza pointery związane z alokacją macierzy lub FILE_MODE - sprawdza poprawne otwarcie plików,
// ptr - pointer który chcemy sprawdzić 
bool checkPointerErr(void *ptr, enum TYPE MODE)
{
    if(ptr == NULL && MODE == MATRIX_MODE )
    {
        fputs(ALLOC_ERR, stderr);
        return true;
    }

    if(ptr == NULL && MODE == FILE_MODE)
    {
        fputs(FILE_ERR, stderr);
        return true;
    }

    return false;
}

// Funckja zwraca pointer na macierz o podanej nazwię, jeżeli macierzy nie ma w systemie zostanie
// zwrócony NULL
// Wymagany pointer na Heada listy - firstMatrix oraz nazwa macierzy - matrixName
ptr_matrix nameCheck(ptr_matrix firstMatrix, char *matrixName)
{
    ptr_matrix checkMatrix = firstMatrix;

    do
    {
       if(!strcmp(matrixName, checkMatrix->id))
        {
            break;
        }
        else
        {
            if(checkMatrix->nast != NULL)
            {
                checkMatrix = checkMatrix->nast;
            }
            else
            {
                return NULL;
            }
        }
    } while (checkMatrix != NULL);
    
    return checkMatrix;
}

// Funkcja zwraca ilość cyfr w liczbię zmiennoprzecinkowej - x, o ustalonej precyzji w MatrixCommon.h
unsigned int digitsNumber(double x)
{
    unsigned int digits = (unsigned int)log10(x) + 1;
    return digits + PRECISION + 1;
}

// Funkcja zwraca największą ilość cyfr która występuję w elementach podanej macierzy - exampleMatrix
unsigned int biggestDigitsNumber(ptr_matrix exampleMatrix)
{
    unsigned int max_digits = 0;

    for(uint32_t i = 0; i < exampleMatrix->n; i++)
    {
        for(uint32_t j = 0; j < exampleMatrix->m; j++)
        {
            if(digitsNumber(exampleMatrix->tab[i][j]) > max_digits)
            {
                max_digits = digitsNumber(exampleMatrix->tab[i][j]);
            }
        }
    }
    return max_digits;
}
// Funkcja wypisuje w pliku txt linijke rozdzielającą wiersze macierzy 
// ptr - pointer na plik, exampleMatrix - macierz która chcemy zapisać w pliku
// size_of_cell - szerokość jeden komórki
void filePrintMatrixLine(FILE *fptr, ptr_matrix exampleMatrix, unsigned int size_of_cell)
{
    fputs("|", fptr);
    for(uint32_t j = 0; j < exampleMatrix->m; j++)
    {
        for(unsigned int z = 0; z < size_of_cell; z++)
        {
            fputs("—", fptr);
        }
        fputs("|", fptr);
    }
    fputs("\n", fptr);
}

// Funkcja tworzy macierz o określonych wielkościach i wypełnia ją zerami, nadaję macierzy również okresloną nazwę - matrixName
// Aby wrzucić macierz do list skorzystać z funkcji putMatrixInList()
// UWAGA: przed edytowaniem stworzonej macierzy sprawdzić czy nie został przez funkcję zwrócony NULL co oznacza że wystąpił błąd alokacji pamięci
// UWAGA 2 : Edytować macierz PRZED!!! dodaniem do listy

ptr_matrix createBlankMatrix(char *matrixName, uint32_t rows, uint32_t columns)
{
    ptr_matrix exampleMatrix = (ptr_matrix)malloc(sizeof(struct matrix));

    if(checkPointerErr(exampleMatrix, MATRIX_MODE))
    {
        freeMatrixMem(exampleMatrix);
        return NULL;
    }

    exampleMatrix->id = (char*)malloc(strlen(matrixName)+1);

    if(checkPointerErr(exampleMatrix->id, MATRIX_MODE))
    {
        freeMatrixMem(exampleMatrix);
        return NULL;
    }
    else
    {
        strcpy(exampleMatrix->id, matrixName);
    }

    exampleMatrix->n = rows;
    exampleMatrix->m = columns;

    exampleMatrix->tab = (double**)malloc(exampleMatrix->n * sizeof(double*));

    if(checkPointerErr(exampleMatrix->tab, MATRIX_MODE))
    {
        freeMatrixMem(exampleMatrix);
        return NULL;
    }

    for(uint32_t i = 0; i < exampleMatrix->n; i++)
    {
        exampleMatrix->tab[i] = (double*)malloc(exampleMatrix->m*sizeof(double));

        if(checkPointerErr(exampleMatrix->tab[i], MATRIX_MODE))
        {
            freeMatrixMem(exampleMatrix);
            return NULL;
        }
    }

    for(uint32_t i = 0; i < exampleMatrix->n; i++)
    {
        for(uint32_t j = 0; j < exampleMatrix->m; j++)
        {
            exampleMatrix->tab[i][j] = 0;
        }
    }

    exampleMatrix->nast = NULL;

    return exampleMatrix;
}

// Funkcja zwalniająca pamięc zaalokowaną przez exampleMatrix
void freeMatrixMem(ptr_matrix exampleMatrix)
{
    for(uint32_t i = 0; i < exampleMatrix->n; i++)
    {
        free(exampleMatrix->tab[i]);
    }

    free(exampleMatrix->tab);
    free(exampleMatrix->id);
    free(exampleMatrix);
}

// Funkcja alfabetycznie dodaje macierz do listy i zwraca wskaźnik na nowego head'a listy
// Przyjmuje aktualnego heada listy - firstMatrix i macierz do dodania - exampleMatrix

ptr_matrix putMatrixInList(ptr_matrix firstMatrix, ptr_matrix exampleMatrix)
{
    ptr_matrix check = firstMatrix;
    ptr_matrix lastCheck = check;

    while(check && (strcmp(check->id, exampleMatrix->id) == -1))
    {
        lastCheck = check;
        check = check->nast;
    }

    exampleMatrix->nast = check;
    if(!firstMatrix || check == firstMatrix)
    {
        return exampleMatrix;
    }
    else
    {
        lastCheck->nast = exampleMatrix;
        return firstMatrix;
    }
}

int checkAddOverflow(double a, double b) {
    if (!isfinite(a) || !isfinite(b)) {
        return -1; // Jeżeli któraś z liczb jest NaN lub nieskończoność
    }
    double result = a + b;
    return isinf(result); // Zwraca 1 jeżeli jest overflow i 0 w przeciwnym przypadku
}

int checkMultiOverflow(double a, double b) {
    if (!isfinite(a) || !isfinite(b)) {
        return -1; // Jeżeli któraś z liczb jest NaN lub nieskończoność
    }
    double result = a * b;
    return isinf(result); // Zwraca 1 jeżeli jest overflow i 0 w przeciwnym przypadku
}

void clearBuff(void)
{
    char c;
    do
    {
       c = getchar();
    }while ( c!='\n' && c!=EOF );
}