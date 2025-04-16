#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define SIZE 20

char nonterminals[MAX];
char terminals[MAX];
char productions[MAX][SIZE][SIZE];
int prodCount[MAX] = {0};

char first[MAX][SIZE], follow[MAX][SIZE];
char parseTable[MAX][MAX][SIZE];

int ntCount = 0, tCount = 0;

int findNTIndex(char ch) {
    for (int i = 0; i < ntCount; i++) {
        if (nonterminals[i] == ch) return i;
    }
    return -1;
}

int findTIndex(char ch) {
    for (int i = 0; i < tCount; i++) {
        if (terminals[i] == ch) return i;
    }
    return -1;
}

void addToSet(char *set, char ch) {
    if (!strchr(set, ch)) {
        int len = strlen(set);
        set[len] = ch;
        set[len + 1] = '\0';
    }
}

void computeFirst(int ntIndex) {
    for (int p = 0; p < prodCount[ntIndex]; p++) {
        char *rule = productions[ntIndex][p];
        char firstSym = rule[0];

        if (!isupper(firstSym)) {
            addToSet(first[ntIndex], firstSym);
        } else {
            int i = findNTIndex(firstSym);
            if (i != -1 && i != ntIndex) {
                if (first[i][0] == '\0')
                    computeFirst(i);
                for (int k = 0; k < strlen(first[i]); k++) {
                    addToSet(first[ntIndex], first[i][k]);
                }
            }
        }
    }
}

void computeFollow(int ntIndex) {
    if (ntIndex == 0) addToSet(follow[ntIndex], '$');

    for (int i = 0; i < ntCount; i++) {
        for (int p = 0; p < prodCount[i]; p++) {
            char *rule = productions[i][p];
            int len = strlen(rule);
            for (int j = 0; j < len; j++) {
                if (rule[j] == nonterminals[ntIndex]) {
                    if (j + 1 < len) {
                        char next = rule[j + 1];
                        if (!isupper(next)) {
                            addToSet(follow[ntIndex], next);
                        } else {
                            int ni = findNTIndex(next);
                            for (int k = 0; k < strlen(first[ni]); k++) {
                                addToSet(follow[ntIndex], first[ni][k]);
                            }
                        }
                    } else {
                        if (i != ntIndex) {
                            if (follow[i][0] == '\0')
                                computeFollow(i);
                            for (int k = 0; k < strlen(follow[i]); k++) {
                                addToSet(follow[ntIndex], follow[i][k]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void buildParseTable() {
    for (int i = 0; i < ntCount; i++) {
        for (int p = 0; p < prodCount[i]; p++) {
            char *rhs = productions[i][p];
            char sym = rhs[0];

            if (!isupper(sym)) {
                int tIndex = findTIndex(sym);
                strcpy(parseTable[i][tIndex], rhs);
            } else {
                int fIndex = findNTIndex(sym);
                for (int k = 0; k < strlen(first[fIndex]); k++) {
                    int tIndex = findTIndex(first[fIndex][k]);
                    strcpy(parseTable[i][tIndex], rhs);
                }
            }
        }
    }
}

void printTable() {
    printf("Predictive Parsing Table:\n  ");
    for (int i = 0; i < tCount; i++) printf(" %c", terminals[i]);
    printf(" $\n----------------------------------------------------\n");

    for (int i = 0; i < ntCount; i++) {
        printf("%c |", nonterminals[i]);
        for (int j = 0; j < tCount; j++) {
            if (strlen(parseTable[i][j]) > 0)
                printf(" %c->%s", nonterminals[i], parseTable[i][j]);
            else
                printf(" _");
        }
        if (strchr(follow[i], '$'))
            printf(" %c->%s", nonterminals[i], "ε");
        else
            printf(" _");
        printf("\n");
    }
}

void parseString(char *input) {
    printf("Parsing sequence and actions\nSTACK\tINPUT\tACTION\n---------------------------------------------------\n");

    char stack[SIZE] = "$";
    strncat(stack, &nonterminals[0], 1);

    int ip = 0;
    while (1) {
        printf("%s\t%s\t", stack, input + ip);

        int top = strlen(stack) - 1;
        char X = stack[top];
        char a = input[ip];

        if (X == a && X == '$') {
            printf("Parsed successfully.\n");
            break;
        }

        if (X == a) {
            stack[top] = '\0';
            ip++;
            printf("Match: %c\n", a);
        } else if (isupper(X)) {
            int nt = findNTIndex(X);
            int t = findTIndex(a);
            if (strlen(parseTable[nt][t]) > 0) {
                printf("Apply: %c->%s\n", X, parseTable[nt][t]);
                stack[top] = '\0';
                for (int k = strlen(parseTable[nt][t]) - 1; k >= 0; k--) {
                    char ch = parseTable[nt][t][k];
                    if (ch != '#') {
                        char temp[2] = {ch, '\0'};
                        strcat(stack, temp);
                    }
                }
                
            } else {
                printf("Error: no rule for [%c,%c]\n", X, a);
                break;
            }
        } else {
            printf("Error: unexpected symbol\n");
            break;
        }
    }
}

int main() {
    char buffer[SIZE];
    printf("Enter grammar rules (e.g. S->aA). End with empty line:\n");

    while (1) {
        fgets(buffer, SIZE, stdin);
        if (buffer[0] == '\n') break;
        buffer[strcspn(buffer, "\n")] = '\0';

        char nt = buffer[0];
        char *rhs = buffer + 3;

        int ntIndex = findNTIndex(nt);
        if (ntIndex == -1) {
            nonterminals[ntCount++] = nt;
            ntIndex = ntCount - 1;
        }

        char *tok = strtok(rhs, "|");
        while (tok) {
            strcpy(productions[ntIndex][prodCount[ntIndex]++], tok);
            for (int i = 0; i < strlen(tok); i++) {
                char c = tok[i];
                if (!isupper(c) && !strchr(terminals, c)) {
                    terminals[tCount++] = c;
                }
            }
            tok = strtok(NULL, "|");
        }
    }

    for (int i = 0; i < ntCount; i++) computeFirst(i);
    for (int i = 0; i < ntCount; i++) computeFollow(i);
    buildParseTable();

    for (int i = 0; i < ntCount; i++) {
        printf("FIRST( %c ): { ", nonterminals[i]);
        for (int j = 0; j < strlen(first[i]); j++) printf("%c ", first[i][j]);
        printf("}\n");
    }

    for (int i = 0; i < ntCount; i++) {
        printf("FOLLOW( %c ): { ", nonterminals[i]);
        for (int j = 0; j < strlen(follow[i]); j++) printf("%c ", follow[i][j]);
        printf("}\n");
    }

    printTable();

    printf("Enter string for parsing: ");
    fgets(buffer, SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcat(buffer, "$");

    parseString(buffer);
    return 0;
}
