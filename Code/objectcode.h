#ifndef OBJECTCODE_H
#define OBJECTCODE_H

#include "semantic.h"
#include "intercode.h"
#define bool int
#define true 1
#define false 0
typedef struct Reg_Obj_* Reg_Obj;
typedef struct Operand_list_ * Operand_list;


struct Reg_Obj_ {
        char name[8];
        int reg_index;
        bool is_free;   
        Operand place;
} Reg_Arr[32];

struct Operand_list_{
        Operand op;
        int offset;//
        Operand_list next;
        Operand_list prev;
};
Operand_list OpHead,OpTail;

void Init_Reg_Arr();
void Init_ObjCode();
Operand_list Search_Operand_list(Operand key);
void Insert_Operand(Operand key, int offset);
void Traversal_InterCodes(FILE *fptr);
void Traversal_InterCode(InterCode i,FILE *fptr);
int Allocate_Reg(Operand place, bool flag,FILE *fptr);
void Free_Reg(FILE *fptr);
bool Match_Operand(Operand t1, Operand t2);
/*struct ObjectCode_ {
        enum {LOADI,MOVE,ADD,ADDI,SUB,MUL,DIV,MFLO, LOADW,SAVEW} kind;
        struct {Reg_Obj dest; Operand src; }loadi;
        struct {Reg_Obj} move
}*/
#endif
