#include "objectcode.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
int Eip = 1;
int stack_offset = 0;
//int num_args = 0;//函数形参的数目
int param_num = 0;
void Init_Reg_Arr()
{
        int i;
        for(i = 0; i < 32; i++)
        {
                Reg_Arr[i].reg_index = i;
                Reg_Arr[i].is_free = true;
        }
        
        strcpy(Reg_Arr[0].name,"zero");
        strcpy(Reg_Arr[1].name,"at");

        for (i = 2; i < 4;i++)
        {
                sprintf(Reg_Arr[i].name,"%s%d","v",i - 2);
        }

        for (i = 4;i < 8; i++)
        {
                sprintf(Reg_Arr[i].name,"%s%d","a",i - 4);
        }

        for(i = 8;i < 16;i++)
        {
                sprintf(Reg_Arr[i].name,"%s%d","t",i - 8);
        }

        for(i = 16;i < 24;i++)
        {
                sprintf(Reg_Arr[i].name,"%s%d","s",i - 8);
        }

        for(i = 24;i < 26;i++)
        {
                sprintf(Reg_Arr[i].name,"%s%d","t",i - 16);
        }


        for(i = 26;i < 28;i++)
        {
                sprintf(Reg_Arr[i].name,"%s%d","k",i - 26 );
        }

        strcpy(Reg_Arr[28].name,"gp");
        strcpy(Reg_Arr[29].name,"sp");
        strcpy(Reg_Arr[30].name,"fp");
        strcpy(Reg_Arr[31].name,"ra");
}
//初始化寄存器名称编号

void Init_ObjCode()
{
        OpHead = NULL;
        OpTail = NULL;
}

Operand_list Search_Operand_list(Operand key)
{
        if(key == NULL)
                assert(0);
        Operand_list temp = OpHead;
        while(temp != NULL)
        {
                Operand left = temp->op;
                if(left->kind == key->kind)
                {
                        if(key->kind == VARIBLE)
                                if(key->var_no == left->var_no)
                                        return temp;
                                else ;
                        else if(key->kind == ADDRESS || key->kind == TEMP_VARIBLE)
                                if(key->temp_var_no == left->temp_var_no)
                                        return temp;
                                else ;
                        else if(key->kind == CONSTANT)
                                if(key->value == left->value)
                                        return temp;
                                else ;
                        else if(key->kind == REF)
                                if(Match_Operand(key->op,left->op))
                                        return temp;
                                else ;
                        else {
                               // printf("key->kind %d, Eip %d\n",key->kind,Eip);
                                assert(0);
                        }
                }
                else;
               temp = temp->next;
        }
        return temp;
}
//查找

void Insert_Operand(Operand key, int offset)
{
        if(key == NULL)
                assert(0);
        if(OpHead == NULL && OpTail == NULL)
        {
                OpHead = malloc(sizeof(struct Operand_list_));
                OpHead->op = key;
                OpHead->offset = offset;
                OpHead->next = NULL;
                OpHead->prev = NULL;
                OpTail = OpHead ;
        }
        else if((OpHead == NULL && OpTail != NULL) || (OpHead != NULL && OpTail == NULL))
                assert(0);
        else{
                Operand_list p = malloc(sizeof(struct Operand_list_));
                p->op = key;
                p->offset = offset;
                p->next = NULL;
                p->prev = OpTail;
                OpTail->next = p;
                OpTail = p;
        }
}
//插入

void Traversal_InterCodes(FILE *fptr)
{
	/*
	        li $v0, 4  
	   8    la $a0, _prompt  
	   9    syscall 
	   10    li $v0, 5 
	   11    syscall 
	   12    jr $ra 
	   13 
	   14  write: 
	   15    li $v0, 1 
	   16    syscall 
	   17    li $v0, 4 
	   18    la $a0, _ret 
	   19    syscall 
	   20    move $v0, $0 
	   21    jr $ra
	*/
        fprintf(fptr,".data\n");
		fprintf(fptr, " _prompt: .asciiz \"Enter an integer : \"\n");
		fprintf(fptr, "_ret: .asciiz \"\\n\"\n");
        fprintf(fptr,".globl main\n");
        fprintf(fptr,".text\n");
		fprintf(fptr, " read: \n");
		fprintf(fptr, "    li $v0, 4 \n");
		fprintf(fptr, "    la $a0, _prompt \n");
		fprintf(fptr, "    syscall \n");
		fprintf(fptr, "    li $v0, 5 \n");
		fprintf(fptr, "    syscall \n");
		fprintf(fptr, "    jr $ra \n");
		fprintf(fptr, "\n");
		fprintf(fptr, " write: \n");
		fprintf(fptr, "    li $v0, 1 \n");
		fprintf(fptr, "    syscall \n");
		fprintf(fptr, "    li $v0, 4 \n");
		fprintf(fptr, "    la $a0, _ret \n");
		fprintf(fptr, "    syscall \n");
		fprintf(fptr, "    move $v0, $0 \n");
		fprintf(fptr, "    jr $ra \n");	
		fprintf(fptr, "\n");

        Init_Reg_Arr();
        Init_ObjCode();
        InterCodes temp = InterCodeHead;
        while(temp != NULL)
        {
			if (temp->code->kind == PARAM)
			{
				if (temp->prev != NULL && temp->prev->code->kind != PARAM)
				{
					int mycount = 1;
					InterCodes ListHead = temp;
					while (temp->next->code->kind == PARAM)
					{
						temp = temp->next;
						mycount = mycount + 1;
					}
					InterCodes ListTail = temp;
					InterCodes ff = ListHead;
					for (int i = 1; i <= mycount; i++)
					{
						param_num = i;
						Traversal_InterCode(ff->code, fptr);
						ff = ff->next;
					}
					temp = ListTail->next;
					Eip = Eip + mycount;
				}
				else
					assert(0);
			}
			else if (temp->code->kind == IARG)
			{
				if (temp->prev != NULL && temp->prev->code->kind != IARG)
				{
					int mycount = 1;
					InterCodes ListHead = temp;
					while (temp->next->code->kind == IARG)
					{
						temp = temp->next;
						mycount = mycount + 1;
					}
					InterCodes ListTail = temp;
					InterCodes ff = ListHead;
					for (int i = mycount; i >= 1; i--)
					{
						param_num = i;
						Traversal_InterCode(ff->code, fptr);
						ff = ff->next;
					}
					temp = ListTail->next;
					Eip = Eip + mycount;
				}
				else
					assert(0);
			}
			else
			{
			//	printf("Eip %d\n", Eip);
				InterCode i = temp->code;
				Traversal_InterCode(i, fptr);
				temp = temp->next;
				Eip++;
			}
        }

        //NEED TO BE MODIFIED
       // fprintf(fptr,"move $v0, $0\n");
       // fprintf(fptr,"jr $ra\n");

}


void Traversal_InterCode(InterCode i, FILE *fptr)
{
	if (i->kind == ASSIGN)
	{
		Operand t1 = i->assign.left;
		Operand t2 = i->assign.right;
		if (t1->kind != DEREF && t2->kind == CONSTANT)
		{
//			printf("l\n");
			int index = Allocate_Reg(t1, true, fptr);    // true left false right
			fprintf(fptr, "li $t%d, %d\n", index - 8, t2->value);
			Free_Reg(fptr);
//			printf("ll\n");
		}
		else if (t1->kind != DEREF && (t2->kind == VARIBLE || t2->kind == TEMP_VARIBLE))
		{

//			printf("lll\n");
			int index1 = Allocate_Reg(t1, true, fptr);
			int index2 = Allocate_Reg(t2, false, fptr);
			fprintf(fptr, "move $t%d, $t%d\n", index1 - 8, index2 - 8);
			Free_Reg(fptr);
//			printf("llll\n");
		}
		else if (t1->kind != DEREF && t2->kind == DEREF)
		{
//			printf("???\n");
			int index1 = Allocate_Reg(t1, true, fptr);
			int index2 = Allocate_Reg(t2->op, false, fptr);
			fprintf(fptr, "lw $t%d, 0($t%d)\n", index1 - 8, index2 - 8);
			Free_Reg(fptr);
//			printf("????\n");
		}
		else if (t1->kind == DEREF && t2->kind != DEREF)
		{
//			printf("?\n");
			int index1 = Allocate_Reg(t1->op, false, fptr);
			int index2 = Allocate_Reg(t2, false, fptr);
			fprintf(fptr, "sw $t%d, 0($t%d)\n", index2 - 8, index1 - 8);
			Free_Reg(fptr);
//			printf("??\n");
		}




		else assert(0); //TODO:
	}

	else if (i->kind == ADD)
	{
		Operand result = i->binop.result;
		Operand op1 = i->binop.op1;
		Operand op2 = i->binop.op2;

		if (op1->kind == REF && op2->kind == CONSTANT)
		{
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op1, false, fptr);
			int index3 = op2->value;

			fprintf(fptr, "addi $t%d, $t%d, -%d\n", index1 - 8, index2 - 8, index3 - 8);
			Free_Reg(fptr);
		}
		else if (op1->kind == REF && op2->kind != CONSTANT)
		{
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op1, false, fptr);
			int index3 = Allocate_Reg(op2, false, fptr);

			fprintf(fptr, "sub $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
			Free_Reg(fptr);
		}
		else if (op1->kind == CONSTANT && op2->kind == CONSTANT)
		{
			Operand op3 = malloc(sizeof(struct Operand_));
			op3->kind = CONSTANT;
			op3->value = op1->value + op2->value;
			int index = Allocate_Reg(result, true, fptr);
			fprintf(fptr, "li $t%d, %d\n", index - 8, op3->value);
			Free_Reg(fptr);
		}
		else if (op1->kind == CONSTANT && op2->kind != CONSTANT)
		{
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op2, false, fptr);

			fprintf(fptr, "addi $t%d, $t%d, %d\n", index1 - 8, index2 - 8, op1->value);
			Free_Reg(fptr);
		}
		else if (op1->kind != CONSTANT && op2->kind == CONSTANT)
		{
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op1, false, fptr);


			fprintf(fptr, "addi $t%d, $t%d, %d\n", index1 - 8, index2 - 8, op2->value);
			Free_Reg(fptr);
		}
		else {
			if (op1->kind == DEREF || op2->kind == DEREF)
				assert(0);
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op1, false, fptr);
			int index3 = Allocate_Reg(op2, false, fptr);

			fprintf(fptr, "add $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
			Free_Reg(fptr);
		}
	}

	else if (i->kind == SUB)
	{
		Operand result = i->binop.result;
		Operand op1 = i->binop.op1;
		Operand op2 = i->binop.op2;

		if (op1->kind == CONSTANT && op2->kind == CONSTANT)
		{
			Operand op3 = malloc(sizeof(struct Operand_));
			op3->kind = CONSTANT;
			op3->value = op1->value - op2->value;
			int index = Allocate_Reg(result, true, fptr);
			fprintf(fptr, "li $t%d, %d\n", index - 8, op3->value);
			Free_Reg(fptr);
		}
		else if (op1->kind == CONSTANT && op2->kind != CONSTANT)
		{
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op2, false, fptr);

			fprintf(fptr, "addi $t%d, $t%d, -%d\n", index1 - 8, index2 - 8, op1->value);
			Free_Reg(fptr);
		}
		else if (op1->kind != CONSTANT && op2->kind == CONSTANT)
		{
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op1, false, fptr);


			fprintf(fptr, "addi $t%d, $t%d, -%d\n", index1 - 8, index2 - 8, op2->value);
			Free_Reg(fptr);
		}
		else {
			int index1 = Allocate_Reg(result, true, fptr);
			int index2 = Allocate_Reg(op1, false, fptr);
			int index3 = Allocate_Reg(op2, false, fptr);
			fprintf(fptr, "sub $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
			Free_Reg(fptr);
		}
	}
	else if (i->kind == MUL)
	{
		Operand result = i->binop.result;
		Operand op1 = i->binop.op1;
		Operand op2 = i->binop.op2;

		int index1 = Allocate_Reg(result, true, fptr);
		int index2 = Allocate_Reg(op1, false, fptr);
		int index3 = Allocate_Reg(op2, false, fptr);

		fprintf(fptr, "mul $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
		Free_Reg(fptr);
	}
	else if (i->kind == DIV)
	{

		Operand result = i->binop.result;
		Operand op1 = i->binop.op1;
		Operand op2 = i->binop.op2;

		int index1 = Allocate_Reg(result, true, fptr);
		int index2 = Allocate_Reg(op1, false, fptr);
		int index3 = Allocate_Reg(op2, false, fptr);

		fprintf(fptr, "div $t%d, $t%d\n", index2 - 8, index3 - 8);

		fprintf(fptr, "mflo $t%d\n", index1 - 8);
		Free_Reg(fptr);
	}
	else if (i->kind == IGOTO)
	{
		Operand op = i->igoto.label;
		fprintf(fptr, "j label%d\n", op->label_no);
	}
	else if (i->kind == IFGOTO)
	{
		Operand op1 = i->ifgoto.left;
		Operand op2 = i->ifgoto.right;
		Operand op3 = i->ifgoto.label;

		int index1 = Allocate_Reg(op1, false, fptr);
		int index2 = Allocate_Reg(op2, false, fptr);
		int label_index = op3->label_no;

		if (strcmp(i->ifgoto.rel, "==") == 0)
		{
			fprintf(fptr, "beq ");
		}
		else if (strcmp(i->ifgoto.rel, "!=") == 0)
		{
			fprintf(fptr, "bne");
		}
		else if (strcmp(i->ifgoto.rel, ">") == 0)
		{
			fprintf(fptr, "bgt");
		}
		else if (strcmp(i->ifgoto.rel, "<") == 0)
		{
			fprintf(fptr, "blt");
		}
		else if (strcmp(i->ifgoto.rel, ">=") == 0)
		{
			fprintf(fptr, "bge");
		}
		else if (strcmp(i->ifgoto.rel, "<=") == 0)
		{
			fprintf(fptr, "ble");
		}
		else
        {
            printf("%s\n",i->ifgoto.rel);
			assert(0);
        }
		fprintf(fptr, " $t%d, $t%d, label%d\n", index1 - 8, index2 - 8, label_index);
		Free_Reg(fptr);
	}
	else if (i->kind == DEC_L)
	{
		Operand op = i->dec_l.label;
		fprintf(fptr, "label%d:\n", op->label_no);
	}
	else if (i->kind == DEC_ARR)
	{
		int size = i->dec_arr.size;
		Operand op = i->dec_arr.op;

		stack_offset = stack_offset + size;
		fprintf(fptr, "addi $sp, $sp, -%d\n", size);

		Operand p = malloc(sizeof(struct Operand_));
		p->kind = REF;
		p->op = op;

		Insert_Operand(p, stack_offset - size);
	}
	else if (i->kind == RET)
	{
		Operand result = i->ret.result;
		int index = Allocate_Reg(result, false, fptr);
		fprintf(fptr, "move $v0, $t%d\n", index - 8);
		fprintf(fptr, "jr $ra \n");
		Free_Reg(fptr);
	}
	else if (i->kind == IREAD)
	{
		Operand op = i->iread.op;
		int index = Allocate_Reg(op, true, fptr);

		fprintf(fptr, "addi $sp, $sp, -4 \n");
		fprintf(fptr, "sw $ra, 0($sp) \n");
		fprintf(fptr, "jal read \n");
		fprintf(fptr, "lw $ra, 0($sp) \n");
		fprintf(fptr, "addi $sp, $sp, 4 \n");
		fprintf(fptr, "move $t%d, $v0 \n", index - 8);
		Free_Reg(fptr);
	}
	else if (i->kind == ICALL)
	{

                Operand result = i->icall.result;
		Operand func = i->icall.func;
		int index = Allocate_Reg(result, true, fptr);
		fprintf(fptr, "addi $sp, $sp, -8 \n");
		fprintf(fptr, "sw $ra, 0($sp) \n");
		fprintf(fptr, "sw $fp, 4($sp) \n");

       // fprintf(fptr,"move $t8, $sp\n");
        //fprintf(fptr,"move $t9, $fp\n");

       // int temp = stack_offset;
        //stack_offset = 0;
		fprintf(fptr, "jal %s \n", func->funcname);
       // fprintf(fptr,"move $sp,$t8\n");
       // fprintf(fptr,"move $fp,$t9\n");
                fprintf(fptr,"move $sp, $fp \n");
		fprintf(fptr, "lw $ra, 0($sp) \n");
		fprintf(fptr, "lw $fp, 4($sp) \n");
		fprintf(fptr, "addi $sp, $sp, 8 \n");
		fprintf(fptr, "move $t%d, $v0 \n", index - 8);
       // stack_offset = temp;
		Free_Reg(fptr);

/*
		Operand result = i->icall.result;
		Operand func = i->icall.func;
		int index = Allocate_Reg(result, true, fptr);
		fprintf(fptr, "addi $sp, $sp, -4 \n");
		fprintf(fptr, "sw $ra, 0($sp) \n");

        fprintf(fptr,"move $t8, $sp\n");
        fprintf(fptr,"move $t9, $fp\n");

       // int temp = stack_offset;
        //stack_offset = 0;
		fprintf(fptr, "jal %s \n", func->funcname);
        fprintf(fptr,"move $sp,$t8\n");
        fprintf(fptr,"move $fp,$t9\n");

		fprintf(fptr, "lw $ra, 0($sp) \n");
		fprintf(fptr, "addi $sp, $sp, 4 \n");
		fprintf(fptr, "move $t%d, $v0 \n", index - 8);
       // stack_offset = temp;
		Free_Reg(fptr);
*/
		/*
				Operand result = i->icall.result;
				Operand func = i->icall.func;
				int index = Allocate_Reg(result, true, fptr);
				fprintf(fptr, "addi $sp, $sp, -8 \n");
				fprintf(fptr, "sw $ra, 0($sp) \n");
				fprintf(fptr, "sw $fp, 4($sp) \n");
				fprintf(fptr, "jal %s \n", func->funcname);
				fprintf(fptr, "lw $fp, 4($sp) \n");
				fprintf(fptr, "lw $ra, 0($sp) \n");
				fprintf(fptr, "addi $sp, $sp, 8 \n");
				fprintf(fptr, "move $t%d, $v0 \n", index - 8);
				Free_Reg(fptr);
		*/
	}
	else if (i->kind == IWRITE)
	{
		Operand op = i->iwrite.op;
		int index = Allocate_Reg(op, false, fptr);
		fprintf(fptr, "move $a0, $t%d \n", index - 8);
		fprintf(fptr, "addi $sp, $sp, -4 \n");
		fprintf(fptr, "sw $ra, 0($sp) \n");
		fprintf(fptr, "jal write  \n");
		fprintf(fptr, "lw $ra, 0($sp) \n");
		fprintf(fptr, "addi $sp, $sp, 4  \n");
		Free_Reg(fptr);
	}
	else if (i->kind == IARG)
	{
		/*
	    Operand op = i->iarg.op;
		int index = Allocate_Reg(op, false, fptr);
		if (num_args < 4)
			fprintf(fptr, "move $a%d, $t%d \n", num_args, index - 8);
		else if (num_args < 4 + 8)
		{
			
			//int ff = (num_args - 5) * 4;
			//fprintf(fptr, "sw $t%d, %d($fp) \n", index - 8, ff);
			
			fprintf(fptr, "move $s%d, $t%d \n", num_args - 4, index - 8);
		}
		else
			assert(0);
		num_args = num_args + 1;
		Free_Reg(fptr);
        */
		Operand op = i->iarg.op;
		int index = Allocate_Reg(op, false, fptr);
		if (param_num <= 4)
			fprintf(fptr, "move $a%d, $t%d \n", param_num-1, index - 8);
		else if (param_num <= 4 + 8)
		{

			//int ff = (num_args - 5) * 4;
			//fprintf(fptr, "sw $t%d, %d($fp) \n", index - 8, ff);

			fprintf(fptr, "move $s%d, $t%d \n", param_num - 5, index - 8);
		}
		else
			assert(0);
		Free_Reg(fptr);


	}
	else if (i->kind == IFUNC)
	{
		Operand op = i->ifunc.op;
		fprintf(fptr, "%s: \n", op->funcname);
	    fprintf(fptr, "addi $fp, $sp, 0\n");
        stack_offset = 0;
		Free_Reg(fptr);
	}
	else if (i->kind == PARAM)
	{

		Operand op = i->param.op;
		int index = Allocate_Reg(op, true, fptr);
		if (param_num > 4 && param_num <= 4 + 8)
		{
			/*
			int ff = (num_args - 5) * 4;
			fprintf(fptr, "lw $t%d, %d($fp) \n", index - 8, ff);
			*/
			fprintf(fptr, "move $t%d, $s%d \n", index - 8, param_num - 5);
		}
		//fprintf(fptr, "move $a%d, $t%d \n", num_args, index - 8);
		//fprintf(fptr, "li $t%d, %d\n", index - 8, t2->value);
		else if (param_num <= 4)
		{
			fprintf(fptr, "move $t%d, $a%d \n", index - 8, param_num - 1);
		}
		else
			assert(0);
		Free_Reg(fptr);


	}
	else assert(0);





}
/*
void Clean_Args()
{
	for (int i = 4; i < 8; i++)
	{

		Reg_Arr[i].is_free = true;
		Reg_Arr[i].place = NULL;

	}
	for (int i = 16; i < 24; i++)
	{

		Reg_Arr[i].is_free = true;
		Reg_Arr[i].place = NULL;

	}
}
int Restore_Arg(Operand op)
{
	for (int i = 4; i < 8; i++)
	{
		if (Reg_Arr[i].is_free == false)
			continue;
		else
		{
			Reg_Arr[i].is_free = false;
			Reg_Arr[i].place = op;
			return i;
		}
	}

	for (int i = 16; i < 23; i++)
	{
		if (Reg_Arr[i].is_free == false)
			continue;
		else
		{
			Reg_Arr[i].is_free = false;
			Reg_Arr[i].place = op;
			return i;
		}
	}
	assert(0);//num of args > 12
	return 0;
}
*/

int  Allocate_Reg(Operand place, bool flag,FILE *fptr)
{
        //static int stack_offset = 0;
        int i;
        for (i = 8 ; i < 16 ; i++)
        {
                if(Reg_Arr[i].is_free == false)
                {
                        if(Reg_Arr[i].place->kind == CONSTANT)
                                continue;
                        if(Match_Operand(Reg_Arr[i].place,place))
                                return i;
                        else continue ;
                }
                else if(Reg_Arr[i].is_free == true)
                {
                        Reg_Arr[i].is_free = false;
                        Reg_Arr[i].place = place;
                        break;
                }
                else continue;
        }
        
        if(place->kind == CONSTANT)
        {
                fprintf(fptr,"li $t%d, %d\n",i - 8, place->value);
                return i;
        }
        if(place->kind == DEREF)
                assert(0);
        Operand_list temp = Search_Operand_list(place);
        if(temp == NULL)
        {
                Insert_Operand(place,stack_offset);
                stack_offset = stack_offset + 4;
                fprintf(fptr,"addi $sp, $sp, -4\n");
        }
        else if(flag == false && temp->op->kind != REF && temp->op->kind != DEREF) {
                int total_offset = 4 + temp->offset;
                fprintf(fptr,"lw $t%d, -%d($fp)\n",i - 8,total_offset);
        }
        else if(flag == true && temp->op->kind != REF && temp->op->kind != DEREF) {
                //int total_offset = 4 + temp->offset;
               // fprintf(fptr,"lw $t%d, -%d($fp)\n",i - 8,total_offset);
        }
        else if(flag == false && temp->op->kind == REF)
        {
                int total_offset = 4 + temp->offset;
                fprintf(fptr,"addi $t%d, $fp, -%d\n",i - 8, total_offset);   
        }
        else if(flag == true && temp->op->kind == REF)
        {
                int total_offset = 4 + temp->offset;
                fprintf(fptr,"addi $t%d, $fp, -%d\n",i - 8, total_offset);   
                
        }
        else
        {
                printf("%d,%d\n",flag,temp->op->kind);
                assert(0);
        }
        return i;
}

void Free_Reg(FILE *fptr)
{
        int i;
        for(int i = 8 ; i < 16 ; i++)
        {
                if(Reg_Arr[i].is_free == false)
                {
                        Reg_Arr[i].is_free = true;

                        if(Reg_Arr[i].place->kind == CONSTANT)
                        {
                                Reg_Arr[i].place = NULL;
                                continue;
                        }

                        Operand_list temp = Search_Operand_list(Reg_Arr[i].place);

                        if(temp == NULL)
                                assert(0);

                        if(temp->op->kind == REF)
                        {
                                Reg_Arr[i].place = NULL;
                                continue;
                        }

                        int total_offset = 4 + temp->offset;
                        fprintf(fptr,"sw $t%d, -%d($fp)\n",i - 8,total_offset);
                        Reg_Arr[i].place = NULL;
                }
        }
}


bool Match_Operand(Operand t1, Operand t2)
{
	if (t1->kind != t2->kind)
		return false;
	else {
		if (t1->kind == VARIBLE)
		{
			if (t1->var_no == t2->var_no)
				return true;
			else return false;
		}
		else if (t1->kind == TEMP_VARIBLE || t1->kind == ADDRESS)
			if (t1->temp_var_no == t2->temp_var_no)
				return true;
			else return false;
		else if (t1->kind == REF)
			if (Match_Operand(t1->op, t2->op))
				return true;
			else return false;
		else {
			printf("t1->kind %d\n", t1->kind);
			assert(0);

		}
	}
}
