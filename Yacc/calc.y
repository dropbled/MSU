%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void yyerror(const char *s) {
    fprintf(stderr, "Ошибка: %s\n", s);
}

int yylex(); 

int last_result = 0; 
%}

%token NUMBER
%left '+' '-'
%left '*' '/'
%left NEG
%start expression

%%

expression:
    expression '+' expression   { $$ = $1 + $3; last_result = $$; } 
  | expression '-' expression   { $$ = $1 - $3; last_result = $$; } 
  | expression '*' expression   { $$ = $1 * $3; last_result = $$; } 
  | expression '/' expression   { 
      if ($3 == 0) {
        yyerror("Деление на ноль");
        YYABORT;
      } else {
        $$ = $1 / $3;
        last_result = $$;
      }
    }
  | '-' expression %prec NEG    { $$ = -$2; last_result = $$; } 
  | '(' expression ')'          { $$ = $2; last_result = $$; } 
  | NUMBER                      { $$ = $1; last_result = $$; } 
  ;

%%

int main() {
    while (!feof(stdin)) { 
        if (yyparse() != 0) { 
            printf("Ошибка при разборе. Попробуйте снова.\n");
        }
    }

    printf("Конец ввода. Результат: %d\n", last_result); 
    return 0;
}
