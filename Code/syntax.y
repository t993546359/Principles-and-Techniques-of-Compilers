%{

   #include<stdio.h>
   #include <assert.h>
   //#include "lex.yy.c"
#define true 1
#define false 0
extern int yylineno;
extern struct node *root;
extern struct Type_* Type;
extern struct FieldList_* FieldList;
extern struct node* leafcreate(char *, int);
extern struct node* nodecreate(char *, int);
extern void tree_print(struct node* , int);
extern struct node* nodelist[7];

%}

/*declared types*/
/*%union {
  int type_int;
  float type_float;
  //double type_double;
}*/

%union{
struct node* o;
double d;
}

%token <o> INT
%token <o> FLOAT
%token <o> ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE SPACE LINE AERROR

%type <o> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList  ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args error 
/*combine*/
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV 
%right NOT
%left LP RP LB RB DOT

%precedence ELSE

%%
/*High-level Definitions*/
Program : ExtDefList {nodelist[0]=$1;$$=nodecreate("Program",1);tree_print($$,0);
        root = $$;}
   ;
ExtDefList : {$$=nodecreate("ExtDefList",0);}
   | ExtDef ExtDefList {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("ExtDefList",2);}
   ;
ExtDef : Specifier ExtDecList SEMI {
        nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("ExtDef",3);
        }
   | Specifier SEMI {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("ExtDef",2);}
   | Specifier FunDec CompSt {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("ExtDef",3);}
   ;
ExtDecList : VarDec {nodelist[0]=$1;$$=nodecreate("ExtDecList",1);   
        }
   | VarDec COMMA ExtDecList {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("ExtDecList",3);}
    ;
/*Specifiers*/
Specifier : TYPE {nodelist[0]=$1;$$=nodecreate("Specifier",1);}
   | StructSpecifier {nodelist[0]=$1;$$=nodecreate("Specifier",1);}
   ;
StructSpecifier : STRUCT OptTag LC DefList RC {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;nodelist[4]=$5;$$=nodecreate("StructSpecifier",5);
                }
                
   | STRUCT Tag {   nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("StructSpecifier",2);
                }
   ;
OptTag :   {$$=nodecreate("OptTag",0);}
       |  ID  {nodelist[0]=$1;$$=nodecreate("OptTag",1);}
   ;
Tag : ID {nodelist[0]=$1;$$=nodecreate("Tag",1);}
   ;

/*Declarators*/
VarDec : ID {nodelist[0]=$1;$$=nodecreate("VarDec",1);}
   | VarDec LB INT RB {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;$$=nodecreate("VarDec",4);}
   ;
FunDec : ID LP VarList RP {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;$$=nodecreate("FunDec",4);}
   | ID LP RP {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("FunDec",3);}
   ;
VarList : ParamDec COMMA VarList {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("VarList",3);}
   | ParamDec {nodelist[0]=$1;$$=nodecreate("VarList",1);}
   ;
ParamDec : Specifier VarDec {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("ParamDec",2);}
   ;  

/*Statements*/
CompSt : LC DefList StmtList RC {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;$$=nodecreate("CompSt",4);}
   ;
StmtList : {$$=nodecreate("StmtList",0);}
         | Stmt StmtList {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("StmtList",2);}
   ;
Stmt : Exp SEMI {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("Stmt",2);}
   | CompSt {nodelist[0]=$1;$$=nodecreate("Stmt",1);}
   | RETURN Exp SEMI {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Stmt",3);} 
//to remove if_else conflict 
   | IF LP Exp RP Stmt {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;nodelist[4]=$5;$$=nodecreate("Stmt",5);}
   | IF LP Exp RP Stmt ELSE Stmt {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;nodelist[4]=$5;nodelist[5]=$6;nodelist[6]=$7;$$=nodecreate("Stmt",7);}
   | WHILE LP Exp RP Stmt {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;nodelist[4]=$5;$$=nodecreate("Stmt",5);}
   | error SEMI {nodelist[0] = $1;nodelist[1] = $2;$$=nodecreate("error",2);yyclearin;yyerrok;}
   | error RC {nodelist[0] = $1;nodelist[1] = $2;$$=nodecreate("error",2);yyclearin;yyerrok;}
   ;
/*Local Definitions*/
DefList : {$$=nodecreate("DefList",0);}
   | Def DefList {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("DefList",2);}
   ;
Def : Specifier DecList SEMI {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Def",3);
   }
   |error SEMI{}
   |error RC {}
   ;
DecList : Dec {nodelist[0]=$1;$$=nodecreate("DecList",1);}
   | Dec COMMA DecList {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("DecList",3);}
   ;
Dec : VarDec {nodelist[0]=$1;$$=nodecreate("Dec",1);}
   | VarDec ASSIGNOP Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Dec",3);}
   ;

/*Expressions*/

Exp : Exp ASSIGNOP Exp {
    nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);
    }
   | Exp AND Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | Exp OR Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | Exp RELOP Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | Exp PLUS Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | Exp MINUS Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | Exp STAR Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | Exp DIV Exp {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | LP Exp RP {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | MINUS Exp {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("Exp",2);}
   | NOT Exp {nodelist[0]=$1;nodelist[1]=$2;$$=nodecreate("Exp",2);}
   | ID LP Args RP {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;$$=nodecreate("Exp",4);}
   | ID LP RP {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | Exp LB Exp RB{nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;nodelist[3]=$4;$$=nodecreate("Exp",4);}
   | Exp DOT ID {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Exp",3);}
   | ID {nodelist[0]=$1;$$=nodecreate("Exp",1);}
   | INT {nodelist[0]=$1;$$=nodecreate("Exp",1);}
   | FLOAT {nodelist[0]=$1;$$=nodecreate("Exp",1);}
   ;

Args : Exp COMMA Args {nodelist[0]=$1;nodelist[1]=$2;nodelist[2]=$3;$$=nodecreate("Args",3);}
   | Exp {nodelist[0]=$1;$$=nodecreate("Args",1);}
   ;

%%
