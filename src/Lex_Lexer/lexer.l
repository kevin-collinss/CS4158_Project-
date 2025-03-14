%{
#include <stdio.h>
%}
%option noyywrap

%%
[ \t\n]+               ;    /* Ignore whitespace */
[a-zA-Z_][a-zA-Z0-9_]* { printf("IDENTIFIER: %s\n", yytext); }
[0-9]+                { printf("NUMBER: %s\n", yytext); }
.                     { printf("SYMBOL: %s\n", yytext); }
%%

int main(void) {
    yylex();
    return 0;
}
