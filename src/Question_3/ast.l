%{
#include "ast.tab.h"
#include <string.h>
%}

%option noyywrap

%%

"main"         { return MAIN; }
"int"          { return INT; }
[a-zA-Z_][a-zA-Z0-9_]*   { yylval.str = strdup(yytext); return ID; }
"="            { return ASSIGN; }
";"            { return SEMI; }
","            { return COMMA; }
"("            { return LPAREN; }
")"            { return RPAREN; }
"{"            { return LBRACE; }
"}"            { return RBRACE; }
"+"            { return PLUS; }
"-"            { return MINUS; }
"*"            { return MULT; }
"/"            { return DIV; }
[ \t\n]+       ;  // ignore whitespace
.              { return yytext[0]; }

%%
