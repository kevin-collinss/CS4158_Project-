%{
#include <stdio.h>
#include <string.h>

int comment_count = 0;

int is_keyword(const char* str) {
    const char* keywords[] = {"void", "main", "int", "float", "char", "if", "else", "while", "return"};
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}
%}

%option noyywrap

%%

"//".*                      { comment_count++; }        // Ignore single-line comment
[ \t\n]+                    ;                          // Ignore whitespace

"("                         { printf("%s Special Operator\n", yytext); }
")"                         { printf("%s Special Operator\n", yytext); }
"{"                         { printf("%s Special Operator\n", yytext); }
"}"                         { printf("%s Special Operator\n", yytext); }
","                         { printf("%s Special Operator\n", yytext); }
";"                         { printf("%s Special Operator\n", yytext); }

"="                         { printf("%s Arithmetic Operator\n", yytext); }
"+"                         { printf("%s Arithmetic Operator\n", yytext); }
"-"                         { printf("%s Arithmetic Operator\n", yytext); }
"*"                         { printf("%s Arithmetic Operator\n", yytext); }
"/"                         { printf("%s Arithmetic Operator\n", yytext); }

[0-9]+\.[0-9]+              { printf("%s Floating Number\n", yytext); }
[0-9]+                      { printf("%s Constant\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*      {
                              if (is_keyword(yytext)) {
                                  printf("%s Reserved Keyword\n", yytext);
                              } else {
                                  printf("%s Identifier\n", yytext);
                              }
                            }

.                           { printf("%s Unknown Symbol\n", yytext); }

%%

int main() {
    yylex();
    printf("%d Comments Ignored\n", comment_count);
    return 0;
}
