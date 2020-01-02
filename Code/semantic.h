#ifndef SEMANTIC
#define SEMANTIC
#include "tree.h"
#define HASH_VAL 0x3fff
#include <assert.h>
#include <stdlib.h>
#define bool int
#define true 1
#define false 0
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_
{
        
        enum {BASIC, ARRAY, STRUCTURE,STRUCTURE_TYPE} kind;
        union
        {
            int basic;
            struct {Type elem; int size;} array;
            FieldList structure;
            Type structure_type;
        };
};

struct FieldList_
{
        char name[32];
        Type type;
        FieldList next;
        FieldList st_type;
};
struct parameter { //函数各形参的类型，以链表方式储存
	Type ptype;
	struct parameter *pnext;
};
struct func { //函数
	char *name;
	Type retype;
	int pnum;
	struct parameter * para;
	struct func *next;
}*funcHead, *funcTail;

struct node *Get_Child(struct node *root,int i);
unsigned int Count_Child(struct node *root);
unsigned int hash_pjw(char *name);
void Init_Hash();
bool Insert_Symbol_Var(FieldList f);
FieldList Search_Symbol_name(char *name);
bool TypeEqual(Type p1,Type p2);
Type Handle_Exp(struct node *ast);
FieldList Handle_VarDec(struct node *root,Type t, int i);
Type Handle_Specifier(struct node *root);
Type Handle_StructSpecifier(struct node *root);
FieldList Handle_DefList(struct node *root,int i );
FieldList Handle_DecList(struct node *root,Type t,int i);
int Handle_Compt(struct node *compst,Type t);
void Handle_ExtDefList(struct node *root);
FieldList Handle_ExtDecList(struct node *root,Type t);
void Realse(FieldList func_local_val);
void GoProgram(struct node *root);

#endif
