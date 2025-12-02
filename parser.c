#include <stdio.h>
#include <string.h>

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[64];
} Token;

Token tokens[1000];
int token_count = 0;
int current = 0;

// Function prototypes
void parse_statement();
void parse_expression();
void parse_term();
void parse_factor();
int match(TokenType type, const char *value);

// Utility to get current token
Token* peek() {
    return &tokens[current];
}

// Advance to next token
void advance() {
    if (current < token_count)
        current++;
}

// Check and consume a specific token
int match(TokenType type, const char *value) {
    if (current < token_count &&
        tokens[current].type == type &&
        strcmp(tokens[current].value, value) == 0) {
        advance();
        return 1;
    }
    return 0;
}

// Grammar Rules

void parse_statement() {
    if (peek()->type == TOKEN_KEYWORD && strcmp(peek()->value, "float") == 0) {
        printf("Parsing variable declaration...\n");
        advance();
        if (peek()->type == TOKEN_IDENTIFIER) {
            printf("Declared variable: %s\n", peek()->value);
            advance();
            if (match(TOKEN_OPERATOR, "=")) {
                parse_expression();
            }
            match(TOKEN_SYMBOL, ";");
        } else {
            printf("Syntax error: Expected identifier\n");
        }
    } else if (peek()->type == TOKEN_IDENTIFIER) {
        printf("Parsing assignment...\n");
        advance();
        if (match(TOKEN_OPERATOR, "=")) {
            parse_expression();
        }
        match(TOKEN_SYMBOL, ";");
    } else {
        printf("Syntax error at token: %s\n", peek()->value);
        advance();
    }
}

void parse_expression() {
    parse_term();
    while (match(TOKEN_OPERATOR, "+") || match(TOKEN_OPERATOR, "-")) {
        parse_term();
    }
}

void parse_term() {
    parse_factor();
    while (match(TOKEN_OPERATOR, "*") || match(TOKEN_OPERATOR, "/")) {
        parse_factor();
    }
}

void parse_factor() {
    if (peek()->type == TOKEN_NUMBER || peek()->type == TOKEN_IDENTIFIER) {
        printf("Factor: %s\n", peek()->value);
        advance();
    } else if (match(TOKEN_SYMBOL, "(")) {
        parse_expression();
        match(TOKEN_SYMBOL, ")");
    } else {
        printf("Syntax error in factor: %s\n", peek()->value);
        advance();
    }
}

// Load dummy tokens for testing
void load_sample_tokens() {
    Token sample[] = {
        {TOKEN_KEYWORD, "float"},
        {TOKEN_IDENTIFIER, "area"},
        {TOKEN_OPERATOR, "="},
        {TOKEN_NUMBER, "3.14"},
        {TOKEN_OPERATOR, "*"},
        {TOKEN_IDENTIFIER, "r"},
        {TOKEN_OPERATOR, "*"},
        {TOKEN_IDENTIFIER, "r"},
        {TOKEN_SYMBOL, ";"}
    };
    memcpy(tokens, sample, sizeof(sample));
    token_count = sizeof(sample) / sizeof(Token);
}

int main() {
    load_sample_tokens();
    while (current < token_count) {
        parse_statement();
    }
    return 0;
}
