#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LINE_LEN 256
#define MAX_MACROS 100

struct Macro {
    char key[50];
    char value[100];
};

struct Macro macros[MAX_MACROS];
int macro_count = 0;

// Function to trim newline
void trim_newline(char *line) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
}

// Replace macros in a line
void replace_macros(char *line) {
    for (int i = 0; i < macro_count; i++) {
        char *pos = strstr(line, macros[i].key);
        if (pos) {
            char new_line[MAX_LINE_LEN] = "";
            strncat(new_line, line, pos - line);
            strcat(new_line, macros[i].value);
            strcat(new_line, pos + strlen(macros[i].key));
            strcpy(line, new_line);
        }
    }
}

int main() {
    FILE *fp = fopen("input", "r");
    if (!fp) {
        printf("Cannot open input.c\n");
        return 1;
    }

    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), fp)) {
        trim_newline(line);

        // Handle single-line comments
        if (strstr(line, "//") == line) continue;

        // Handle #define
        if (strncmp(line, "#define", 7) == 0) {
            char key[50], value[100];
            if (sscanf(line + 8, "%s %s", key, value) == 2) {
                strcpy(macros[macro_count].key, key);
                strcpy(macros[macro_count].value, value);
                macro_count++;
            }
        } else {
            replace_macros(line);
            printf("%s\n", line);
        }
    }

    fclose(fp);
    return 0;
}
