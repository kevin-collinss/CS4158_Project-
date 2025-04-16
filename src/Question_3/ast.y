%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char* s);

int temp_count = 0;
int quad_count = 0;

struct Quad {
    char op[5];
    char arg1[20];
    char arg2[20];
    char result[20];
} quads[100];

char* new_temp() {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

void emit(const char* op, const char* arg1, const char* arg2, const char* result) {
    strcpy(quads[quad_count].op, op);
    strcpy(quads[quad_count].arg1, arg1);
    strcpy(quads[quad_count].arg2, arg2);
    strcpy(quads[quad_count].result, result);
    quad_count++;
}
%}

%union {
    char* str;
}

%token <str> ID
%token MAIN INT
%token ASSIGN SEMI COMMA
%token LPAREN RPAREN LBRACE RBRACE
%token PLUS MINUS MULT DIV

%type <str> expr term factor

%%

program:
    MAIN LPAREN RPAREN LBRACE decls stmts RBRACE
    {
        printf("-----------------------------------\n");
        printf("Pos Operator Arg1 Arg2 Result\n");
        printf("-----------------------------------\n");
        for (int i = 0; i < quad_count; i++) {
            printf("%d %s %s %s %s\n", i,
                quads[i].op,
                quads[i].arg1,
                quads[i].arg2,
                quads[i].result);
        }
        printf("-----------------------------------\n");
    }
;

decls:
    INT id_list SEMI
;

id_list:
    ID                 { /* optionally store variable names */ }
  | id_list COMMA ID   { }
;

stmts:
    stmts stmt
  | stmt
;

stmt:
    ID ASSIGN expr SEMI
    {
        emit("=", $3, "", $1);
    }
;

expr:
      expr PLUS term
      {
          char* temp = new_temp();
          emit("+", $1, $3, temp);
          $$ = temp;
      }
    | expr MINUS term
      {
          char* temp = new_temp();
          emit("-", $1, $3, temp);
          $$ = temp;
      }
    | term
      {
          $$ = $1;
      }
;

term:
      term MULT factor
      {
          char* temp = new_temp();
          emit("*", $1, $3, temp);
          $$ = temp;
      }
    | term DIV factor
      {
          char* temp = new_temp();
          emit("/", $1, $3, temp);
          $$ = temp;
      }
    | factor
      {
          $$ = $1;
      }
;

factor:
      ID
      {
          $$ = $1;
      }
;

%%

void yyerror(const char* s) {
    printf("Error: %s\n", s);
}

int main() {
    printf("Parsing program...\n");
    if (yyparse() == 0) {
        // Quadruples printed in grammar action
    }
    return 0;
}
