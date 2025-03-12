#include <stdio.h>
#include <ctype.h>

// Define token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_SYMBOL
} TokenType;

// Function to process an identifier
void processIdentifier(int firstChar) {
    char buffer[100];
    int i = 0;
    buffer[i++] = firstChar;

    // Continue reading while the character is alphanumeric or an underscore
    int c;
    while ((c = getchar()) != EOF && (isalnum(c) || c == '_')) {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    
    // If we read one character too far, push it back for further processing
    if (c != EOF) {
        ungetc(c, stdin);
    }
    printf("IDENTIFIER: %s\n", buffer);
}

// Function to process a number
void processNumber(int firstChar) {
    char buffer[100];
    int i = 0;
    buffer[i++] = firstChar;

    int c;
    while ((c = getchar()) != EOF && isdigit(c)) {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    
    if (c != EOF) {
        ungetc(c, stdin);
    }
    printf("NUMBER: %s\n", buffer);
}

int main() {
    int c;
    // Read until end of file
    while ((c = getchar()) != EOF) {
        // Skip spaces, tabs, and new lines (redundant whitespace)
        if (c == ' ' || c == '\t' || c == '\n') {
            continue;
        }
        
        // Check if the character starts an identifier (letter) 
        // or a number (digit)
        if (isalpha(c) || c == '_') {
            processIdentifier(c);
        } else if (isdigit(c)) {
            processNumber(c);
        } else {
            // Process as a symbol or operator
            printf("SYMBOL: %c\n", c);
        }
    }
    
    return 0;
}
