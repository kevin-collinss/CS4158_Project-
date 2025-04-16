%{
#include "calc.tab.h"
%}

%option noyywrap

%%

[0-9]+          { yylval = atoi(yytext); return NUMBER; }
[\t ]+          ; // Ignore whitespace
"\n"            { return NEWLINE; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }
"("             { return LPAREN; }
")"             { return RPAREN; }

.               { return yytext[0]; }  // catch invalid characters
%%
