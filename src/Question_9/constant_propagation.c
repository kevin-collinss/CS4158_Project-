#include <stdio.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LEN 50

typedef struct {
    char var[10];
    char val[10];
} ConstMap;

int main() {
    char op[5], arg1[20], arg2[20], res[20];
    char lines[MAX_LINES][MAX_LEN];
    ConstMap consts[MAX_LINES];
    int constCount = 0, lineCount = 0;

    printf("Enter 3-address code (end with empty line):\n");

    // Input all lines
    while (fgets(lines[lineCount], MAX_LEN, stdin)) {
        if (lines[lineCount][0] == '\n') break;
        lineCount++;
    }

    printf("\nOptimised Code (Constant Propagation):\n");

    for (int i = 0; i < lineCount; i++) {
        sscanf(lines[i], "%s %s %s %s", op, arg1, arg2, res);

        if (strcmp(op, "=") == 0) {
            // Constant assignment: = value var
            int found = 0;
            for (int j = 0; j < constCount; j++) {
                if (strcmp(consts[j].var, res) == 0) {
                    strcpy(consts[j].val, arg1);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(consts[constCount].var, res);
                strcpy(consts[constCount].val, arg1);
                constCount++;
            }
        } else {
            // Try to replace arg1 and arg2 with constants
            for (int j = 0; j < constCount; j++) {
                if (strcmp(arg1, consts[j].var) == 0)
                    strcpy(arg1, consts[j].val);
                if (strcmp(arg2, consts[j].var) == 0)
                    strcpy(arg2, consts[j].val);
            }
            printf("%s %s %s %s\n", op, arg1, arg2, res);
        }
    }

    return 0;
}
