#include <stdio.h>
#include <string.h>

#define MAX 10

char terminals[MAX];
char precedence[MAX][MAX]; // precedence table
int n; // number of terminals

int getIndex(char symbol) {
    for (int i = 0; i < n; i++) {
        if (terminals[i] == symbol)
            return i;
    }
    return -1;
}

char getPrecedence(char stackTop, char inputSymbol) {
    int row = getIndex(stackTop);
    int col = getIndex(inputSymbol);
    if (row == -1 || col == -1) return 'E'; // Error
    return precedence[row][col];
}

int main() {
    char stack[50] = "$", input[50];
    int top = 1;

    printf("Enter the no. of terminals: ");
    scanf("%d", &n);

    printf("Enter the terminals: ");
    for (int i = 0; i < n; i++) {
        scanf(" %c", &terminals[i]);
    }

    // Fill precedence table
    printf("Enter the table values:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("Enter the value for %c %c: ", terminals[i], terminals[j]);
            scanf(" %c", &precedence[i][j]);
        }
    }

    // Print the table
    printf("\n**** OPERATOR PRECEDENCE TABLE ****\n   ");
    for (int i = 0; i < n; i++) printf(" %c", terminals[i]);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf(" %c", terminals[i]);
        for (int j = 0; j < n; j++) {
            printf(" %c", precedence[i][j]);
        }
        printf("\n");
    }

    printf("Enter the input string: ");
    scanf("%s", input);
    strcat(input, "$");

    printf("\n%-15s %-15s %-20s\n", "STACK", "INPUT STRING", "ACTION");
    printf("-------------------------------------------------------------\n");

    int ip = 0;
    while (1) {
        char a = stack[top - 1];     // top of stack
        char b = input[ip];          // current input symbol

        printf("%-15s %-15s ", stack, input + ip);

        if (a == '$' && b == '$') {
            printf("String is accepted\n");
            break;
        }

        char prec = getPrecedence(a, b);

        if (prec == '<' || prec == '=') {
            stack[top++] = b;
            stack[top] = '\0';
            ip++;
            printf("Shift %c\n", b);
        } else if (prec == '>') {
            // Reduce logic: remove one symbol (simplified)
            top--;
            stack[top] = '\0';
            printf("Reduce\n");
        } else if (prec == 'A') {
            printf("String is accepted\n");
            break;
        } else {
            printf("Error: Invalid precedence or symbol\n");
            break;
        }
    }

    return 0;
}
