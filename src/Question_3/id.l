%{
#include <stdio.h>
#include <ctype.h>
%}

%option noyywrap

%%
[a-zA-Z][a-zA-Z0-9]*   { printf("Valid Identifier\n"); return 0; }
[0-9]+                 { printf("Error. Failed to parse.\n"); return 0; }
.|\n                   { printf("Error. Failed to parse.\n"); return 0; }
%%
int main() {
    printf("Enter identifier: ");
    yylex();
    return 0;
}
