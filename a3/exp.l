/*Connor Rowland, CSSC0403, 820436793
 *CS 530, Spring 2019
 *Lab #3, Simple Parser
 *FILENAME: exp.l
 */
%{
# include "exp.tab.h"
# include <cstdio>
# include <iostream>
using namespace std;
extern FILE* yyin;
extern int yyparse();
%}
%option noyywrap
%option yylineno
%%
"=" { yylval.opval = strdup(yytext); return EQUALS; }
";" { yylval.opval = strdup(yytext); return SEMI; }
"+" { yylval.opval = strdup(yytext); return OP; }
"-" { yylval.opval = strdup(yytext); return OP; }
"*" { yylval.opval = strdup(yytext); return OP; }
"/" { yylval.opval = strdup(yytext); return OP; }
"%" { yylval.opval = strdup(yytext); return OP; }
"(" { yylval.opval = strdup(yytext); return OPEN_PAREN; }
")" { yylval.opval = strdup(yytext); return CLOSE_PAREN; }
[a-zA-Z][a-zA-Z0-9]* { yylval.sval = strdup(yytext); return ID; }
[ \t\r] { /* ignore whitespace */ }
"\n" { return EOL;}
<<EOF>> {return END;}
. {yylval.opval = strdup(yytext); return INVALID_ID;}
%%
int yyerror(const char *s)
{
 fprintf(stderr, "Line: %d | Error: %s\n", yylineno,s);
 return 1;
}
