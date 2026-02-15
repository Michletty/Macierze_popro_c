#include <stdio.h>

int selectValue(int plus, int star, int pipe) {
    int plus_valid = (plus != -1);
    int star_valid = (star != -1);
    int pipe_valid = (pipe != -1);
    int count = plus_valid + star_valid + pipe_valid;

    if (count == 0) {
        return -1;  //wszystkie to -1
    }

    char options[3];
    int num_options = 0;

    if (plus_valid) options[num_options++] = '+';
    if (star_valid) options[num_options++] = '*';
    if (pipe_valid) options[num_options++] = '|';

    if (count == 1) {
        switch (options[0]) {
            case '+': return '+';
            case '*': return '*';
            case '|': return '|';
        }
    } else {
        char choice;
        do {
            printf("Proszę wybrać żadana operacje: ");
            for (int i = 0; i < num_options; ++i) {
                printf("%c", options[i]);
                if (i < num_options - 1) printf(", ");
            }
            printf(": ");
            scanf(" %c", &choice);

            for (int i = 0; i < num_options; ++i) {
                if (options[i] == choice) {
                    switch (choice) {
                        case '+': return '+';
                        case '*': return '*';
                        case '|': return '|';
                    }
                }
            }
            printf("Nie ma takiej operacji\n");
        } while (1);
    }
    return -1; // Should never reach here
}
