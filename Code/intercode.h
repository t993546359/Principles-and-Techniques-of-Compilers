#ifndef INTERCODE
#define INTERCODE
#include "semantic.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define bool int
#define true 1
#define false 0
typedef struct  Operand_* Operand;
typedef struct  InterCode_* InterCode;
typedef struct  InterCodes_* InterCodes;

struct Operand_ {
        enum {VARIBLE,VARIBLE_ADDRESS, CONSTANT,ADDRESS,TEMP_VARIBLE,LABEL,REF,DEREF,FUNC/*,....*/} kind;
        union {
                int var_no;
                int value;
                int temp_var_no;
                int label_no;
				char funcname[32];
                Operand op;                //...
        };
};

struct InterCode_ {
        enum {
			ASSIGN,
			ADD, 
			SUB, 
			MUL,
			DIV,
			IFGOTO,
			IGOTO,
			DEC_L,
			DEC_ARR,
			RET,
			IREAD,
            ICALL,
			IWRITE,
			IARG,
			IFUNC,
			PARAM/* ...*/} kind;
        union {
                struct {Operand right, left;} assign;
                struct {Operand result,op1,op2;} binop;
                struct {Operand left,right,label; char rel[32];} ifgoto;
                struct {Operand label;} igoto;
                struct {Operand label;} dec_l;
                struct {Operand op; int size;} dec_arr;
				struct { Operand result; } ret;
				struct { Operand op; } iread;
				struct { Operand result,func; } icall;
				struct { Operand op; } iwrite;
				struct { Operand op; } iarg;
				struct { Operand op; } ifunc;
				struct { Operand op; } param;
        };
        //InterCode prev;
        //InterCode next;
};

struct InterCodes_ {
        InterCode code;
        InterCodes prev;
        InterCodes next;
};

InterCodes InterCodeHead, InterCodeTail;

void Free_intercodes(InterCodes head);
void myfree2();

void Init_InterCode();
bool Insert_InterCode(InterCode i);
void Print_InterCode(char *filename);
void Print_Operand(Operand p, FILE *P);
//bool Delete_InterCode(InterCode i);
Type Translate_Exp(struct node *ast,Operand o,Type t);
void Translate_Logical(struct node *ast,Operand o,Type t);
void Translate_Cond(struct node *ast,Operand l1,Operand l2,Type t);
void Translate_Cond_Other(struct node *ast,Operand l1,Operand l2,Type t);
void Translate_Program(struct node *ast);
void Translate_ExtDefList(struct node *ast);
void Translate_ExtDef(struct node *ast);
Type Translate_Specifier(struct node *ast);
void Translate_DefList(struct node *ast);
void Translate_Def(struct node *ast);
void Translate_DecList(struct node *ast,Type t);
void Translate_Dec(struct node *ast,Type t);
Operand Translate_VarDec(struct node *ast,Type t);
Operand Translate_VarDec2(struct node *ast,Type t);
void Translate_Stmt(struct node *ast);
void Translate_CompSt(struct node *ast);
void Translate_StmtList(struct node *ast);
void Insert_Args(Operand arg_list[],Operand t);
//void Init_Args();
void Translate_Args(struct node *ast,Operand arg_list[]);
void Translate_ParamDec(struct node *ast);
void Translate_FunDec(struct node *ast);
#endif
