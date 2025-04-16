#include <stdio.h>
#include <string.h>
#include <ctype.h>

int tempCount = 0;

int getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
    }
    return 0;
}

// Generates next temp variable name (e.g., T0, T1...)
char nextTemp() {
    return 'Z' - (tempCount++);  // Z, Y, X, ...
}

void generateIC(char expr[]) {
    char stack[100][20];
    int top = -1;
    char postfix[100], output[100];
    int k = 0;

    // Step 1: Convert infix to postfix (Shunting Yard-like, but manual)
    char opStack[100];
    int opTop = -1;

    for (int i = 0; i < strlen(expr); i++) {
        char ch = expr[i];
        if (isspace(ch)) continue;

        if (isalnum(ch)) {
            postfix[k++] = ch;
        } else if (ch == '(') {
            opStack[++opTop] = ch;
        } else if (ch == ')') {
            while (opTop >= 0 && opStack[opTop] != '(') {
                postfix[k++] = opStack[opTop--];
            }
            opTop--;  // pop '('
        } else {
            while (opTop >= 0 && getPrecedence(opStack[opTop]) >= getPrecedence(ch)) {
                postfix[k++] = opStack[opTop--];
            }
            opStack[++opTop] = ch;
        }
    }
    while (opTop >= 0) postfix[k++] = opStack[opTop--];
    postfix[k] = '\0';

    // Step 2: Generate intermediate code from postfix
    char temp, a[10], b[10], result[10];
    for (int i = 0; i < strlen(postfix); i++) {
        char ch = postfix[i];
        if (isalnum(ch)) {
            char temp[2] = {ch, '\0'};
            strcpy(stack[++top], temp);
        } else {
            strcpy(b, stack[top--]);
            strcpy(a, stack[top--]);
            temp = nextTemp();
            printf("%c = %s %c %s\n", temp, a, ch, b);

            char tempstr[2] = {temp, '\0'};
            strcpy(stack[++top], tempstr);
        }
    }

    // Final assignment to LHS variable
    printf("%c = %s\n", expr[0], stack[top]);
}

int main() {
    char expr[100];

    printf("Enter the expression (e.g., x=a+b*c-d/e+f):\n");
    fgets(expr, sizeof(expr), stdin);
    expr[strcspn(expr, "\n")] = '\0';

    printf("Intermediate code:\n");
    generateIC(expr);

    return 0;
}
