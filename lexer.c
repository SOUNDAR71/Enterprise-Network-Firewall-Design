#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN
} TokenType;

const char *keywords[] = {"int", "float", "char", "return", "if", "else", "while", "for", "void"};
const char operators[] = "+-*/=%";
const char symbols[] = "();{},";

// Check if word is keyword
int is_keyword(const char *word) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Check if character is operator
int is_operator(char ch) {
    for (int i = 0; i < strlen(operators); i++) {
        if (ch == operators[i]) return 1;
    }
    return 0;
}

// Check if character is symbol
int is_symbol(char ch) {
    for (int i = 0; i < strlen(symbols); i++) {
        if (ch == symbols[i]) return 1;
    }
    return 0;
}

// Classify token
void classify_token(const char *token) {
    if (is_keyword(token)) {
        printf("[KEYWORD: %s]\n", token);
    } else if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1]))) {
        printf("[NUMBER: %s]\n", token);
    } else if (isalpha(token[0]) || token[0] == '_') {
        printf("[IDENTIFIER: %s]\n", token);
    } else {
        printf("[UNKNOWN: %s]\n", token);
    }
}

int main() {
    FILE *fp = fopen("output.pre.c", "r"); // Output from your preprocessor
    if (!fp) {
        printf("Cannot open preprocessed file.\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int i = 0;
        while (line[i]) {
            // Skip whitespace
            if (isspace(line[i])) {
                i++;
                continue;
            }

            // Handle operators
            if (is_operator(line[i])) {
                printf("[OPERATOR: %c]\n", line[i]);
                i++;
                continue;
            }

            // Handle symbols
            if (is_symbol(line[i])) {
                printf("[SYMBOL: %c]\n", line[i]);
                i++;
                continue;
            }

            // Handle words/numbers
            char token[64];
            int j = 0;
            while (isalnum(line[i]) || line[i] == '.' || line[i] == '_') {
                token[j++] = line[i++];
            }
            token[j] = '\0';
            if (j > 0) {
                classify_token(token);
            }
        }
    }

    fclose(fp);
    return 0;
}
