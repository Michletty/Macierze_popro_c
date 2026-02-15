#include "MatrixCommon.h"
#include "MainFunctions.h"
int chooseDelimiterIndex(const char *line, char delimiter) {
    int count = 0;
    int capacity = 10;
    int *indices = malloc(capacity * sizeof(int));

    // Collect all delimiter indices
    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == delimiter) {
            if (count >= capacity) {
                capacity *= 2;
                indices = realloc(indices, capacity * sizeof(int));
            }
            indices[count++] = i;
        }
    }

    int selected_index;
    if (count == 1) {
        selected_index = indices[0];
    } else {
        // Present options to the user
        printf("Kilka wystapien znaku specjalnego. Prosze wybrac jedna z opcji podzialu:\n");
        for (int i = 0; i < count; ++i) {
            int pos = indices[i];
            
            // Create temporary preview of split parts
            char left_part[256], right_part[256];
            strncpy(left_part, line, pos);
            left_part[pos] = '\0';
            strncpy(right_part, line + pos + 1, strlen(line) - pos - 1);
            right_part[strlen(line) - pos - 1] = '\0';
            
            trimBoth(left_part);
            trimBoth(right_part);
            printf("Opcja %d: '%s' | '%s'\n", i+1, left_part, right_part);
        }

        // Get user input
        int choice;
        do {
            printf("Wybierz opcje (1-%d): ", count);
            scanf("%d", &choice);
            while(getchar() != '\n'); // Clear input buffer
        } while (choice < 1 || choice > count);

        selected_index = indices[choice - 1];
    }

    free(indices);
    return selected_index;
}