%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
int result;
%}

%token NUMBER
%token PLUS MINUS MULT DIV LPAREN RPAREN NEWLINE

%%

input:
    expr NEWLINE     { result = $1; }
;

expr:
      expr PLUS term   { $$ = $1 + $3; }
    | expr MINUS term  { $$ = $1 - $3; }
    | term             { $$ = $1; }
;

term:
      term MULT factor { $$ = $1 * $3; }
    | term DIV factor  { $$ = $1 / $3; }
    | factor           { $$ = $1; }
;

factor:
      NUMBER             { $$ = $1; }
    | LPAREN expr RPAREN { $$ = $2; }
;

%%

void yyerror(const char *s) {
    printf("Error. Failed to parse.\n");
}

int main() {
    printf("Enter arithmetic expression: ");
    if (yyparse() == 0)
        printf("Result: %d\n", result);
    return 0;
}
