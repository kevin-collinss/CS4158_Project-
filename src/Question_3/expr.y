%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}

%token NUMBER
%token PLUS MINUS MULT DIV LPAREN RPAREN

%%
expr:
      expr PLUS term
    | expr MINUS term
    | term
    ;

term:
      term MULT factor
    | term DIV factor
    | factor
    ;

factor:
      NUMBER
    | LPAREN expr RPAREN
    ;

%%

void yyerror(const char *s) {
    printf("Error. Failed to parse.\n");
}

int main() {
    printf("Enter arithmetic expression: ");
    if (yyparse() == 0)
        printf("Valid Arithmetic Expression\n");
    return 0;
}
