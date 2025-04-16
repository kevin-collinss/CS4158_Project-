#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS 100

const char *keywords[] = {"int", "float", "char", "if", "else", "return", "while", "for"};
int isKeyword(const char *str) {
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Store tokens
char *keywordsFound[MAX_TOKENS];
char *identifiers[MAX_TOKENS];
char *constants[MAX_TOKENS];
char *operators[MAX_TOKENS];
char *symbols[MAX_TOKENS];
int kCount = 0, idCount = 0, cCount = 0, oCount = 0, sCount = 0;

// Helper to add to token arrays (avoid duplicates)
void addToken(char *token, char **array, int *count) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(array[i], token) == 0)
            return;
    }
    array[*count] = strdup(token);
    (*count)++;
}

void skipComment() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

void processIdentifier(int firstChar) {
    char buffer[100];
    int i = 0;
    buffer[i++] = firstChar;

    int c;
    while ((c = getchar()) != EOF && (isalnum(c) || c == '_')) {
        buffer[i++] = c;
    }
    buffer[i] = '\0';

    if (c != EOF) ungetc(c, stdin);

    if (isKeyword(buffer)) {
        addToken(buffer, keywordsFound, &kCount);
    } else {
        addToken(buffer, identifiers, &idCount);
    }
}

void processNumber(int firstChar) {
    char buffer[100];
    int i = 0;
    buffer[i++] = firstChar;

    int c;
    while ((c = getchar()) != EOF && isdigit(c)) {
        buffer[i++] = c;
    }
    buffer[i] = '\0';

    if (c != EOF) ungetc(c, stdin);

    addToken(buffer, constants, &cCount);
}

int isOperatorChar(char c) {
    return (c == '=' || c == '+' || c == '-' || c == '*' || c == '/');
}

int isSymbolChar(char c) {
    return (c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}');
}

int main() {
    int c;

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') continue;

        if (c == '/') {
            int next = getchar();
            if (next == '/') {
                skipComment();
                continue;
            } else {
                ungetc(next, stdin);
                char op[] = {c, '\0'};
                addToken(op, operators, &oCount);
                continue;
            }
        }

        if (isalpha(c) || c == '_') {
            processIdentifier(c);
        } else if (isdigit(c)) {
            processNumber(c);
        } else if (isOperatorChar(c)) {
            char op[] = {c, '\0'};
            addToken(op, operators, &oCount);
        } else if (isSymbolChar(c)) {
            char sym[] = {c, '\0'};
            addToken(sym, symbols, &sCount);
        }
    }

    // Output
    printf("Constants: [");
    for (int i = 0; i < cCount; i++) {
        printf("'%s'%s", constants[i], i == cCount - 1 ? "" : " ");
    }
    printf("]\n");

    printf("KeyWord: [");
    for (int i = 0; i < kCount; i++) {
        printf("'%s'%s", keywordsFound[i], i == kCount - 1 ? "" : " ");
    }
    printf("]\n");

    printf("Identifier: [");
    for (int i = 0; i < idCount; i++) {
        printf("'%s'%s", identifiers[i], i == idCount - 1 ? "" : " ");
    }
    printf("]\n");

    printf("Operators: [");
    for (int i = 0; i < oCount; i++) {
        printf("'%s'%s", operators[i], i == oCount - 1 ? "" : " ");
    }
    printf("]\n");

    printf("Symbols: [");
    for (int i = 0; i < sCount; i++) {
        printf("'%s'%s", symbols[i], i == sCount - 1 ? "" : " ");
    }
    printf("]\n");

    return 0;
}
