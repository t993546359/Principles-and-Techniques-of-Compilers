/*define include */
%{
#include<stdio.h>
#include<stdlib.h>
#include"syntax.tab.h"
#define bool int
#define true 1
#define false 0
extern bool judge_flag;
extern struct node* leafcreate(char* , int);
%}
/*define definations */
%option yylineno
TYPE int|float
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
SPACE [ \t\r]*
LINE \n
FLOAT ([0-9]+\.[0-9]+)|((([0-9]*\.[0-9]+)|([0-9]+\.))[Ee][+-]?[0-9]+) 
INT [0-9][0-9]*
/* FLOAT [0-9]+\.[0-9]* */
ID [a-z_A-Z][a-z_A-Z0-9]*
SEMI ;
COMMA [,]
ASSIGNOP [=]
RELOP >=|<=|==|!=|>|<
PLUS \+
MINUS -
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT !
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
AERROR .
/* define rules */
%%

{TYPE} {yylval.o=leafcreate("TYPE", yylineno); return TYPE;}
{STRUCT} {yylval.o=leafcreate("STRUCT", yylineno);return STRUCT;}  
{RETURN} {yylval.o=leafcreate("RETURN", yylineno);return RETURN;}
{IF}   {yylval.o=leafcreate("IF", yylineno); return IF;}
{ELSE} {yylval.o=leafcreate("ELSE", yylineno); return ELSE;}
{WHILE} {yylval.o=leafcreate("WHILE", yylineno); return WHILE;}
{SPACE} { }
{LINE} { }
{INT} {yylval.o=leafcreate("INT", yylineno);return INT;}
{FLOAT} {yylval.o=leafcreate("FLOAT", yylineno);return FLOAT;}
{ID} {yylval.o=leafcreate("ID", yylineno);return ID;}
{SEMI} {yylval.o=leafcreate("SEMI", yylineno); return SEMI;}
{COMMA} {yylval.o=leafcreate("COMMA", yylineno); return COMMA;}
{ASSIGNOP}  {yylval.o=leafcreate("ASSIGNOP", yylineno); return ASSIGNOP;}
{RELOP} {yylval.o=leafcreate("RELOP", yylineno); return RELOP;}
{PLUS} {yylval.o=leafcreate("PLUS", yylineno); return PLUS;}
{MINUS} {yylval.o=leafcreate("MINUS", yylineno); return MINUS;}
{STAR} {yylval.o=leafcreate("STAR", yylineno); return STAR;}
{DIV}  {yylval.o=leafcreate("DIV", yylineno); return DIV;}
{AND}  {yylval.o=leafcreate("AND", yylineno); return AND;}
{OR} {yylval.o=leafcreate("OR", yylineno); return OR;}
{DOT}  {yylval.o=leafcreate("DOT", yylineno); return DOT;}
{NOT}  {yylval.o=leafcreate("NOT", yylineno); return NOT;}
{LP}   {yylval.o=leafcreate("LP", yylineno); return LP;}
{RP}   {yylval.o=leafcreate("RP", yylineno); return RP;}
{LB}   {yylval.o=leafcreate("LB", yylineno); return LB;}
{RB}   {yylval.o=leafcreate("RB", yylineno); return RB;}
{LC}   {yylval.o=leafcreate("LC", yylineno); return LC;}
{RC}   {yylval.o=leafcreate("RC", yylineno); return RC;}
{AERROR} {judge_flag = false;printf("Error type A at line %d Mysterious character %s \n",yylineno,yytext); }
%%



