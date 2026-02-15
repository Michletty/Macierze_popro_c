#include "MatrixFile.h"
#include "MatrixCalc.h"
#include "MainFunctions.h"


int main(void){
    int originalStdin=dup(fileno(stdin));
    
    char inputLine[BUFSIZE];


    while(997){

        memset(inputLine,0,sizeof(inputLine));
        if(!fgets(inputLine, BUFSIZE, stdin)){
            doFile(NULL, originalStdin, CLOSE_FILE);
        }

        if(!nlOrNu(inputLine, BUFSIZE)){
            fputs(TOO_LONG, stderr);
            continue;
        }
        trimLeading(inputLine);


        int commandValue = whichCommand(inputLine);

        if(commandValue < 2 && commandValue>-1){
            commandCallZero(commandValue);
        } 
        else if (commandValue>4 && commandValue<9){
            commandCallOne(commandValue, inputLine);
        } 
        else if (commandValue>1 && commandValue<5){
            commandCallTwo(commandValue, inputLine);
        }
        else if (commandValue>8 && commandValue<12){
            commandCallThree(commandValue, inputLine);
        }
        

    }
    return 0;
}