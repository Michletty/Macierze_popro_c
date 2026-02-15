#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H
#define BUFSIZE 32784

struct command{   
    const char *key;
    const int value;
    const int filesNum;
};

struct thNames{
    char *name1; 
    char *name2; 
    char *name3;
};

struct twNames{
    char *name1;
    char *name2;
};

void trimLeading(char *string);
void trimTrailing(char *string);
void trimBoth(char *string);
int getOffset(const int cmdNumber);
int charIndex(const char wanted, const char *line);
int whichCommand(char *line);
int getOneName(const char *line, const int cmdVal, const int offset, char *result);
int getTwoNames(const char *line, const int cmdVal, const int offset, struct twNames *result);
int getThreeNames(const char *line, int *cmdVal, struct thNames *result);
void commandCallZero(const int cmdVal);
void commandCallOne(const int cmdVal, const char *line);
void commandCallTwo(const int cmdVal, const char *line);
void commandCallThree(const int cmdVal, const char *line);
struct twNames splitString(const char *line, char delimiter);
int chooseDelimiterIndex(const char *line, char delimiter);
int selectValue(int plus, int star, int pipe);
int nlOrNu(const char *array, int length);

#endif