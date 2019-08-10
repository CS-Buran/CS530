/*Connor Rowland, CSSC0403, 820436793
 *CS 530, Spring 2019
 *Lab #3, Simple Parser
 *FILENAME:exp.y
 */
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <cstdio>
  #include <iostream>
  #include <string>
  using namespace std;
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  int yyerror(const char *s);
%}
%error-verbose
%union{
  char *sval;
  char *opval;
}
%token <sval>EQUALS <sval>SEMI
%token EOL <opval>OP
%token <sval>ID
%token <sval>INVALID_ID
%token <sval>OPEN_PAREN <sval>CLOSE_PAREN END
%locations
%%
/*The END token means the end of file has been reached.
 *If that happens YYACCEPT stops accepting inputs and
 *return yyparse()
 */

lines:
line {}
| lines line {}
;

line: EOL {}
| END {YYACCEPT;}
| assign EOL {cout << "Valid assignment" << endl; }
| exp EOL {cout << "Valid Expression" << endl;}
| error EOL {cout << endl << endl; yyerrok;}
;

assign:
 id equals exp semi {}
;

exp:
id {}
 | id op exp {}
 | openParen id op id closeParen {}
;
id: ID {cout << $1 << " ";};
op: OP {cout << $1 << " ";};
semi: SEMI {cout << $1 << " ";};
equals: EQUALS {cout << $1 << " ";};
openParen: OPEN_PAREN {cout << $1 << " ";};
closeParen: CLOSE_PAREN {cout << $1 << " ";};
%%

int main(int argc, char **argv) {
  FILE *myfile = fopen("ex.txt", "r");

  // make sure file can be open
  if (!myfile) {
    printf("I can't open ex.txt\n");
    return -1;
  }

  // set lex to read from file instead of defaulting to STDIN
  yyin = myfile;

  // parse through the input
  yyparse();
  return 0;
}
