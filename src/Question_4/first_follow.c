#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 20
#define EPSILON '#'

char productions[MAX][MAX];
char nonTerminals[MAX];
char terminals[MAX];
char firstSet[MAX][MAX];
char followSet[MAX][MAX];
int productionCount = 0, nonTermCount = 0, termCount = 0;

int isTerminal(char symbol) {
    return !isupper(symbol) && symbol != EPSILON;
}

int isNonTerminal(char symbol) {
    return isupper(symbol);
}

int findNonTerminalIndex(char symbol) {
    for (int i = 0; i < nonTermCount; i++)
        if (nonTerminals[i] == symbol) return i;
    return -1;
}

void addToSet(char *set, char symbol) {
    if (!strchr(set, symbol)) {
        int len = strlen(set);
        set[len] = symbol;
        set[len + 1] = '\0';
    }
}

void computeFirst(char symbol, char *result) {
    if (isTerminal(symbol)) {
        addToSet(result, symbol);
        return;
    }

    int index = findNonTerminalIndex(symbol);
    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            char *rhs = productions[i] + 2;
            if (rhs[0] == EPSILON) {
                addToSet(result, EPSILON);
            } else {
                for (int j = 0; rhs[j]; j++) {
                    char temp[MAX] = "";
                    computeFirst(rhs[j], temp);
                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != EPSILON) addToSet(result, temp[k]);
                    }
                    if (!strchr(temp, EPSILON)) break;
                    if (!rhs[j+1]) addToSet(result, EPSILON);
                }
            }
        }
    }
}

void computeFollow(char symbol, char *result) {
    if (symbol == productions[0][0]) addToSet(result, '$');

    for (int i = 0; i < productionCount; i++) {
        char *rhs = productions[i] + 2;
        for (int j = 0; rhs[j]; j++) {
            if (rhs[j] == symbol) {
                if (rhs[j+1]) {
                    char temp[MAX] = "";
                    computeFirst(rhs[j+1], temp);
                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != EPSILON) addToSet(result, temp[k]);
                    }
                    if (strchr(temp, EPSILON)) {
                        char nextFollow[MAX] = "";
                        computeFollow(productions[i][0], nextFollow);
                        for (int k = 0; nextFollow[k]; k++)
                            addToSet(result, nextFollow[k]);
                    }
                } else {
                    if (productions[i][0] != symbol) {
                        char temp[MAX] = "";
                        computeFollow(productions[i][0], temp);
                        for (int k = 0; temp[k]; k++)
                            addToSet(result, temp[k]);
                    }
                }
            }
        }
    }
}

void findTerminalsAndNonTerminals() {
    for (int i = 0; i < productionCount; i++) {
        char lhs = productions[i][0];
        if (!strchr(nonTerminals, lhs)) {
            nonTerminals[nonTermCount++] = lhs;
        }

        for (int j = 2; productions[i][j]; j++) {
            char sym = productions[i][j];
            if (isTerminal(sym) && sym != EPSILON && !strchr(terminals, sym)) {
                terminals[termCount++] = sym;
            }
        }
    }
}

int main() {
    char input[MAX];
    printf("Enter grammar rules (one per line). End with empty line:\n");

    while (fgets(input, MAX, stdin)) {
        if (input[0] == '\n') break;
        input[strcspn(input, "\n")] = 0;
        if (strchr(input, '=')) {
            char lhs = input[0];
            char *rhs = strtok(input + 2, "|");
            while (rhs) {
                sprintf(productions[productionCount++], "%c=%s", lhs, rhs);
                rhs = strtok(NULL, "|");
            }
        }
    }

    printf("Transitions Read: (%d)\n", productionCount);
    for (int i = 0; i < productionCount; i++) {
        printf(" %s\n", productions[i]);
    }

    findTerminalsAndNonTerminals();

    printf("Non-Terminals Encountered: (%d)\n ", nonTermCount);
    for (int i = 0; i < nonTermCount; i++)
        printf("%c ", nonTerminals[i]);
    printf("\n");

    printf("Terminals Encountered: (%d)\n ", termCount);
    for (int i = 0; i < termCount; i++)
        printf("%c ", terminals[i]);
    printf("\n");

    for (int i = 0; i < nonTermCount; i++) {
        computeFirst(nonTerminals[i], firstSet[i]);
    }

    for (int i = 0; i < nonTermCount; i++) {
        computeFollow(nonTerminals[i], followSet[i]);
    }

    for (int i = 0; i < nonTermCount; i++) {
        printf("First(%c) = { ", nonTerminals[i]);
        for (int j = 0; firstSet[i][j]; j++)
            printf("%c ", firstSet[i][j]);
        printf("}\n");
    }

    for (int i = 0; i < nonTermCount; i++) {
        printf("Follow(%c) = { ", nonTerminals[i]);
        for (int j = 0; followSet[i][j]; j++)
            printf("%c ", followSet[i][j]);
        printf("}\n");
    }

    return 0;
}
