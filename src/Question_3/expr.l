%{
#include "expr.tab.h"
%}

%option noyywrap

%%

[0-9]+          { yylval = atoi(yytext); return NUMBER; }
[\n\t ]+        ;               // Skip whitespace
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }
"("             { return LPAREN; }
")"             { return RPAREN; }

.               { return yytext[0]; } // Return invalid characters as-is

%%
