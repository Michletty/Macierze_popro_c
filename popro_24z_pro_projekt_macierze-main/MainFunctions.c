#include "MainFunctions.h"
#include "MatrixCommon.h"
#include "Matrix.h"

//wskaźnik na pierwszą macierz w liście
ptr_matrix pierwsza=NULL;


//słownik do zwracania numeru komendy
struct command commands[] = {
    {"Exit", 0, 0},
    {"List", 1, 0},
    {"Txt", 2, 2},
    {"Load", 3, 2},
    {"Save", 4, 2},
    {"Print", 5, 1},
    {"Delete", 6, 1},
    {"Do", 7, 1},
    {"Create", 8, 1},
    {"*", 9, 3},      //mnożenie
    {"+", 10, 3},      //dodawanie
    {"|", 11, 3}       //łączenie
};


//przyjmuje numer polecenia i zwraca jego długość
//skutecznie o ile miejsc przesunąć czytanie nazw wynikające z długości polecenia
int getOffset(const int cmdNumber){
    return strlen(commands[cmdNumber].key);
}




//usuwanie ' ' wiodących
void trimLeading(char *string){
    int i=0, j=0;
    while(isspace(string[i])) i++;
    while(string[j++] = string[i++]);
}
//usuwanie ' ' końcowych
void trimTrailing(char *string){
    int length = strlen(string);
    while (length > 0 && isspace((unsigned char)string[length - 1])) {
        string[length - 1] = '\0'; // Null-terminate the string at the found position
        length--;
    }
}
//usuwanie ' ' z wiodących i końcowych
void trimBoth(char *string){
    trimLeading(string);
    trimTrailing(string);
}



//sprawdzanie indeksu zadanego znaku
//zwraca -1 jeśli nie ma
//zwraca -2 jeśli więcej niż jeden
int charIndex(const char wanted, const char *line){
    int index = -1;
    for(int i = 0; i<strlen(line); i++){
        if(line[i]==wanted && index==-1){
            index=i;
        } else if(line[i]==wanted && index!=-1){
            return -2;
        }
    }
    return index;
}


//zwraca numer użytej komendy
//przyjmuje całą linijkę, szuka najpierw wysokich (|, + , *) potem pozostałych
//zwraca wartość komendy ze struktry commands
//zwraca -1 w przypadku niewykrycia
int whichCommand(char *line){
    //długość słownika, używana w pętli przy sprawdzaniu
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    int i = 0; //zaczynamy od końca
    //const int in = i;       //używana do liczenia, żeby przy 3 poleceniach nie było -4 

    for (i; i < num_commands-3; i++){   
        if (0 == strncmp(line, commands[i].key, strlen(commands[i].key))){      //przejście po poleceniahc słownych
            return commands[i].value;   
        }
    }

    
    for(i; i < num_commands; i++){
        if(charIndex(commands[i].key[0], line)>-1){         //przejście po poleceniach matematycznych
            return commands[i].value;  
        } else if(charIndex(commands[i].key[0], line)==-2){
            return commands[i].value;  
        }
    }

    

    if (*line=='\n' || *line=='\0'){    //jeśli pusta linijka
        return -1;
    }

    fputs(COMMAND_ERR, stderr);     //jeśli linijka niepusta ale polecenie niewykryte
    return -1;
}


//wywoływana w commandCallOne
//przyjmuje całą linijke, numer polecenia,
//offset - przesunięcie wynikające z długości polecenia np. print n1
//wskaźnik na strukture string result
//string wyprowadzony na string result
int getOneName(const char *line, const int cmdVal, const int offset, char *result){

    strncpy(result, line+offset, strlen(line)-offset);
    result[strlen(line)-offset]='\0';

    trimBoth(result);

    if(!strcmp(result, "\0")){
        fputs(NO_NAME, stderr);
        return -1;
    }

    return 0;
}
//wywoływana w commandCallTwo
//przyjmuje całą linijke, numer polecenia,
//offset - przesunięcie wynikające z długości polecenia np. save n1 > n2
//wskaźnik na strukture twNames
//stringi przypisywane do struktury result
//zwraca -1 dla niepoprawnej nazwy -2 dla braku strzałki
int getTwoNames(const char *line, const int cmdVal, const int offset, struct twNames *result) {
    int arrowIndex = charIndex('>', line);

    if (arrowIndex == -2){  //więcej niż 1 '>'
        arrowIndex = chooseDelimiterIndex(line, '>');
    }
    if (arrowIndex > 0) {//wszystko git
       
       //alokacja na nazwy
        result->name1 = malloc(arrowIndex - offset + 1);
        result->name2 = malloc(strlen(line) - arrowIndex);

        //kopiowanie i kończeniem '\0' name1
        strncpy(result->name1, line + offset, arrowIndex - offset);
        result->name1[arrowIndex - offset] = '\0';

        //kopiowanie i kończeniem '\0' name2
        strncpy(result->name2, line + arrowIndex + 1, strlen(line) - arrowIndex - 1);
        result->name2[strlen(line) - arrowIndex - 1] = '\0';

        //ucinanie spacji
        trimBoth(result->name1);
        trimBoth(result->name2);
    
    } else if (arrowIndex == -1) {
        //brak strzałki '>'
        fputs(NO_ARROW, stderr);
        return -2;
    }

    //sprawdzanie czy któraś nazwa jest niepoprawna
    if (!result->name1 || !result->name2 || 
        strlen(result->name1) == 0 || strlen(result->name2) == 0) {
        fputs(NO_NAME, stderr);
        return -1;
    }

    return 0;
}


//wywoływana w commandCallThree
//przyjmuje całą linijke, numer polecenia
//offsetu nie ma bo wszystkie trzynazwowe mają offset 0
//wskaźnik na structure thNames
//stringi wpisywane na strukture result
//zwraca -1 jeśli pusta nazwa -2 dla braku '=' -3 dla '+' wcześniej niż '='
int getThreeNames(const char *line, int *cmdVal, struct thNames *result) {
    int pluses = charIndex('+', line);
    int stars = charIndex('*', line);
    int pipes = charIndex('|', line);

    char specialDelimiter = selectValue(pluses, stars, pipes);

    int equalsIndex = charIndex('=', line);
    int specialIndex = charIndex(specialDelimiter, line);

    if (equalsIndex == -2){ //więcej niż 1 = 
        equalsIndex = chooseDelimiterIndex(line, '=');
    } 

    if (specialIndex == -2){ //więcej niż 1 specjalny
        specialIndex = chooseDelimiterIndex(line, specialDelimiter);
    } 

    if (equalsIndex == -1) { //nie ma '='
        fputs(NO_EQUAL, stderr);
        return -2;
    } else if (equalsIndex >= specialIndex) { //specjalny przed '='
        fputs(BAD_ORDER, stderr);
        return -3;
    } 

    //alokowanie na nazwy + 1 na znak końca
    result->name1 = malloc(equalsIndex + 1);  // +1 dla '\0'
    result->name2 = malloc(specialIndex - equalsIndex);  // (specialIndex - equalsIndex -1) +1
    result->name3 = malloc(strlen(line) - specialIndex);  // (strlen(line) - specialIndex -1) +1

    //kopiowanie i kończenie \0 name1
    strncpy(result->name1, line, equalsIndex);
    result->name1[equalsIndex] = '\0';

    //kopiowanie i kończenie \0 name2
    int name2_len = specialIndex - equalsIndex - 1;
    strncpy(result->name2, line + equalsIndex + 1, name2_len);
    result->name2[name2_len] = '\0';

    //kopiowanie i kończenie \0 name3
    int name3_len = strlen(line) - specialIndex - 1;
    strncpy(result->name3, line + specialIndex + 1, name3_len);
    result->name3[name3_len] = '\0';

    //ucięcie spacji
    trimBoth(result->name1);
    trimBoth(result->name2);
    trimBoth(result->name3);

    //sprawdzamy czy nazwa pusta
    if (strlen(result->name1) == 0 || strlen(result->name2) == 0 || strlen(result->name3) == 0) {
        fputs(NO_NAME, stderr);
        return -1;
    }

    return 0;
}







//do wywoływania poleceń nieprzyjmujących nazw (exit, list)
//przyjmuje numer komendy ze słownika
//niczego nie zwraca, tylko wywołuje
void commandCallZero(const int cmdVal){
    switch(cmdVal){
        case 0:
            puts("Wyjście z programu"); //exit
            exit(1000);
            break;
        
        case 1:
            listMatrices(pierwsza); //list
            break;
    }
}






//do wywoływania poleceń przyjmujących jedną nazwę (print, delete, do)
//przyjmuje numer komendy ze słownika
//niczego nie zwraca, tylko wywołuje
void commandCallOne(const int cmdVal, const char *line){
    int originalStdin=dup(fileno(stdin));   //coś z do

    char result[BUFSIZE];
    memset(result, 0, BUFSIZE); // dziwne błędy mogą się dziać bez tego
    if(!getOneName(line, cmdVal, getOffset(cmdVal), result)){   //jeśli wyszło bez błędu

    switch(cmdVal){
        case 5:
            printMatrix(result, pierwsza);//print
            break;
        case 6:
            pierwsza = deleteMatrix(result, pierwsza);//delete
            break;
        case 7:
            doFile(result, originalStdin, READ_FILE);//do
            break;
        case 8:
            pierwsza = createMatrix(pierwsza, result);//create
            break;
        }
    }

}





//do wywoływania poleceń przyjmujących dwie nazwę (txt, load, save)
//przyjmuje numer komendy ze słownika
//niczego nie zwraca, tylko wywołuje
void commandCallTwo(const int cmdVal, const char *line){
    struct twNames result={NULL, NULL};

    int errorName = getTwoNames(line, cmdVal, getOffset(cmdVal), &result);
    
    if(errorName){
        return;
    } else {

        
    switch(cmdVal){
        case 2:
            txt(result.name2, result.name1, pierwsza);
            break;
        case 3:
            pierwsza = load(result.name1, result.name2, pierwsza);
            break;
        case 4:
            save(result.name2, result.name1, pierwsza);
            break;
    }


    free(result.name1);
    free(result.name2);
    result.name1 = NULL;
    result.name2 = NULL;

    }

}







//do wywoływania poleceń przyjmujących trzy nazwę (|, +, *)
//przyjmuje numer komendy ze słownika
//niczego nie zwraca, tylko wywołuje
void commandCallThree(const int cmdVal, const char *line){
    struct thNames result;
    int commandValue = cmdVal;

    int errorName = getThreeNames(line, &commandValue, &result);

    if(!errorName){
    switch (commandValue)
    {
    case 9:
        pierwsza = multiplyMatrices(result.name1, result.name2, result.name3, pierwsza);
        break;
    
    case 10:
        pierwsza = sumMatrix(result.name1, result.name2, result.name3, pierwsza);
        break;

    case 11:
        pierwsza = matricesVertically(result.name1, result.name2, result.name3, pierwsza); //łączenie
        break;
    }

    free(result.name1);
    free(result.name2);
    free(result.name3);
    result.name1 = NULL;    
    result.name2 = NULL;
    result.name3 = NULL;
    }
}



int nlOrNu(const char *array, int length) {
    for (int i = 0; i < length; i++) {
        if (array[i] == '\n' || array[i] == '\0') {
            return true;
        }
    }
    return false;
}