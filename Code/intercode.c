//#include "semantic.h"
#include "intercode.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
void Print_Test(InterCode i);
extern InterCodes InterCodeHead,InterCodeTail;
//extern unsigned int Count_Child(struct node *root);
//extern struct node *Get_Child(struct node *root, int i);
//int arg_list_num = 0;
//Operand arg_list[32];
int temp_var_index = 1;
int label_index = 1;
void Init_InterCode()
{
        InterCodeHead = NULL;
        InterCodeTail = NULL;
}

bool Insert_InterCode(InterCode i)
{
    if(i == NULL){
            assert(0);
            return false;
    }
    else{ 
        Print_Test(i);
        if(InterCodeHead == NULL && InterCodeTail == NULL)
        {
                InterCodeHead = malloc(sizeof(struct InterCodes_));
                
                InterCodeHead->code = i;
                

                InterCodeHead->next = NULL;
                InterCodeHead->prev = NULL;

                InterCodeTail = InterCodeHead;
                
        }

        else if(InterCodeHead == NULL && InterCodeTail != NULL)
        {
                assert(0);
                return false;
        }
        else if(InterCodeHead != NULL && InterCodeTail == NULL)
        {
                assert(0);
                return false;
        }
        else{
                InterCodes p = malloc(sizeof(struct InterCodes_));
                p->code = i;
                p->next = NULL;
                p->prev = InterCodeTail;
                InterCodeTail->next = p;
                InterCodeTail = p;
        }

    }
}

void Print_InterCode(char *filename)
{
        FILE *fptr;
        fptr = fopen(filename,"w");
        if(fptr == NULL)
        {
                assert(0);
        }
        else {
                InterCodes temps = InterCodeHead;
                                
                while(temps != NULL)
                {
                        InterCode temp = temps->code;
                        if(temp->kind == ASSIGN)
                        {
                                Print_Operand(temp->assign.left,fptr);
                                fprintf(fptr," := ");
                                Print_Operand(temp->assign.right,fptr);
                        }
                        else if(temp->kind == ADD || temp->kind == SUB ||\
                                        temp->kind == MUL || temp->kind ==DIV)
                        {
                                Print_Operand(temp->binop.result,fptr);
                                fprintf(fptr," := ");
                                Print_Operand(temp->binop.op1,fptr);
                                switch(temp->kind)
                                {
                                        case ADD :{
                                                        fprintf(fptr," + "); 
                                                        break;
                                                  }
                                        case SUB :{
                                                        fprintf(fptr," - ");
                                                        break;
                                                  }
                                        case MUL :{
                                                        fprintf(fptr," * ");
                                                        break;
                                                  }
                                        case DIV :{
                                                        fprintf(fptr," / ");
                                                        break;
                                                  }
                                        default :assert(0);
                                }
                                Print_Operand(temp->binop.op2,fptr);
                        }
                        else if(temp->kind == IFGOTO)
                        {
                                fprintf(fptr,"IF ");
                                Print_Operand(temp->ifgoto.left,fptr);
                                fprintf(fptr," %s ",temp->ifgoto.rel);
                                Print_Operand(temp->ifgoto.right,fptr);
                                fprintf(fptr," GOTO ");
                                Print_Operand(temp->ifgoto.label,fptr);
                        }
                        else if(temp->kind == IGOTO)
                        {
                                fprintf(fptr,"GOTO ");
                                Print_Operand(temp->igoto.label,fptr);
                        }
                        else if(temp->kind == DEC_L)
                        {
                                fprintf(fptr,"LABEL ");
                                Print_Operand(temp->dec_l.label,fptr);
								fprintf(fptr, " :");
                        }
                        else if(temp->kind == DEC_ARR)
                        {
                                fprintf(fptr,"DEC ");
                                Print_Operand(temp->dec_arr.op,fptr);
                                fprintf(fptr, " %d ",temp->dec_arr.size);
                        }
						else if (temp->kind == RET)
						{
							fprintf(fptr, "RETURN ");
							Print_Operand(temp->ret.result, fptr);
                            fprintf(fptr,"\n");
						}
						else if (temp->kind == IREAD)
						{
							fprintf(fptr, "READ ");
							Print_Operand(temp->iread.op, fptr);
						}
						else if (temp->kind == ICALL)//
						{
							Print_Operand(temp->icall.result, fptr);
							fprintf(fptr, " := CALL ");
							Print_Operand(temp->icall.func, fptr);
						}
						else if (temp->kind == IWRITE)
						{
							fprintf(fptr, "WRITE ");
							Print_Operand(temp->iwrite.op, fptr);
						}
						else if (temp->kind == IARG)
						{
							fprintf(fptr, "ARG ");
							Print_Operand(temp->iarg.op, fptr);
						}
						else if (temp->kind == IFUNC)
						{
							fprintf(fptr, "FUNCTION ");
							Print_Operand(temp->ifunc.op, fptr);
							fprintf(fptr, " :");
						}
						else if (temp->kind == PARAM)
						{
							fprintf(fptr, "PARAM ");
							Print_Operand(temp->param.op, fptr);
						}
                        else assert(0);
                        fprintf(fptr, "\n");
                        temps = temps->next;
                }
        }
}

void Print_Operand(Operand p, FILE *fptr)
{
        if(p->kind == VARIBLE)
        {
                fprintf(fptr,"v%d",p->var_no);
        }
        else if(p->kind == VARIBLE_ADDRESS)
        {
                fprintf(fptr,"v%d",p->var_no);
        }
        else if(p->kind == CONSTANT)
        {
                fprintf(fptr,"#%d",p->value);
        }
        else if(p->kind == ADDRESS)
        {
                fprintf(fptr,"t%d",p->temp_var_no);
        }
        else if(p->kind == TEMP_VARIBLE)
        {
                fprintf(fptr,"t%d",p->temp_var_no);
        }
        else if(p->kind == LABEL)
        {
                fprintf(fptr,"label%d",p->label_no);
        }
        else if(p->kind == REF)
        {
                assert(p->op->kind == VARIBLE);
                fprintf(fptr,"&v%d",p->op->var_no);
               // Print_Operand(p->op,fptr);
        }
        else if(p->kind == DEREF)
        {
                //assert(p->op->kind == ADDRESS);
                fprintf(fptr,"*t%d",p->op->temp_var_no);
        }
		else if (p->kind == FUNC)
		{
			fprintf(fptr, "%s", p->funcname);
		}
        else assert(0);//TODO
}

Type Translate_Exp(struct node *ast , Operand o,Type t)
{
        if(Count_Child(ast) == 1)
        {
                if(o == NULL)
                       return NULL ;
                
                else if(strcmp(ast->cld->nodename,"INT") == 0)  // place := #value
                {
                        o->kind = CONSTANT;
                        o->value = atoi(ast->cld->special_val);
            /*         InterCode i = malloc(sizeof(struct InterCode_));
                        i->kind = ASSIGN;
                        i->assign.left = o;
                        o->kind = TEMP_VARIBLE;

                        Operand t1 = malloc(sizeof(struct Operand_));
                        t1->kind = CONSTANT;
                        t1->value = atoi(ast->cld->special_val);

                        i->assign.right = t1;
                        Insert_InterCode(i);
                        t->kind = BASIC;
                        t->basic = 1;
                        }
                        // TODO: return ...*/
                }

                else if(strcmp(ast->cld->nodename,"ID") == 0) // place := variable.name
                {
                    
                        FieldList f = Search_Symbol_name(ast->cld->special_val);
                        if(f->type->kind == BASIC){
                                o->kind = VARIBLE;
                                o->var_no = f->var_no;
                                /*
                                InterCode i = malloc(sizeof(struct InterCode_));
                                i->kind = ASSIGN;
                                i->assign.left = o;
                                o->kind = TEMP_VARIBLE;

                                Operand t1 = malloc(sizeof(struct Operand_));
                                t1->kind = VARIBLE;
                                t1->var_no = f->var_no;             
                        
                                i->assign.right = t1;
                                
                              //  t = f->type;
                                Insert_InterCode(i);*/
                        }
                        else if(f->type->kind == ARRAY && f->var_typeno == 2)
                        {
                                Operand t1 = malloc(sizeof(struct Operand_));
                                t1->kind = VARIBLE;
                                t1->var_no = f->var_no;
                                o->kind = REF;
                                o->op = t1;
                                /*
                                o->temp_var_no = temp_var_index++;
                                
                                InterCode i = malloc(sizeof(struct InterCode_));
                                i->kind = REF;
                                i->assign.left = o;
                                o->kind = ADDRESS;
                                
                                Operand t1 = malloc(sizeof(struct Operand_));
                                t1->kind = VARIBLE;
                                t1->var_no = f->var_no;

                                i->assign.right = t1;

                                Insert_InterCode(i);*/
                        }
                        else if(f->type->kind == ARRAY && f->var_typeno == 1)
                        {
                                o->kind = VARIBLE_ADDRESS;
                                o->var_no = f->var_no; 
                        }
                        else {
                                assert(0);
                        }
                        return f->type;
                        // TODO: return ..
                }
                else assert(0); //
        }
        if(Count_Child(ast) == 3)
        {
                if(strcmp(ast->cld->bro->nodename,"ASSIGNOP") == 0)  //Exp1 ASSIGNOP Exp2
                {
                        Operand t1 = malloc(sizeof(struct Operand_));
                       // t1->temp_var_no = temp_var_index++;
                        t1->flag = 0;
                        Translate_Exp(Get_Child(ast,0),t1,t);
                        Operand t2 = malloc(sizeof(struct Operand_));
                        t2->flag = 1;
                       // t2->temp_var_no = temp_var_index++;
                        Translate_Exp(Get_Child(ast,2),t2,t);

                        InterCode i1 = malloc(sizeof(struct InterCode_));
                        i1->kind = ASSIGN;
                        i1->assign.left = t1;
                        i1->assign.right = t2;
                        Insert_InterCode(i1);
                        if(o != NULL)
                        {
                                InterCode i2 = malloc(sizeof(struct InterCode_));
                                i2->kind = ASSIGN;
                                i2->assign.right = t1;
                                i2->assign.left = o;
                                o->kind = TEMP_VARIBLE;
                                o->temp_var_no = temp_var_index++;
                              //  o->kind = t1->kind;
                        }
                        else ;
                }
                       /* if(strcmp(Get_Child(ast->cld,0)->nodename,"ID") == 0) // ID ASSIGNOP Exp2
                        {
                                Operand t1 = malloc(sizeof(struct Operand_)); // Operand t1; t1 := #value
                                //t1->kind = TEMP_VARIABLE;
                                t1->temp_var_no = temp_var_index;
                                temp_var_index++;
                                Translate_Exp(Get_Child(ast,2),t1,t);
                                
                                Operand var = malloc(sizeof(struct Operand_)); //Operand var;
                                var->kind = VARIBLE;
                                FieldList f = Search_Symbol_name(ast->cld->special_val);
                                var->var_no = f->var_no;
                                
                                

                                InterCode i1 = malloc(sizeof(struct InterCode_)); // variable.name := t1;
                                i1->kind = ASSIGN;
                                i1->assign.left = var;
                                i1->assign.right = t1;
                                Insert_InterCode(i1);

                                InterCode i2 = malloc(sizeof(struct Operand_)); // place := variable.name
                                i2->kind = ASSIGN;
                                i2->assign.left = o;
                                i2->assign.right = var;
                                Insert_InterCode(i2);
                        }
                        else if(Count_Child(ast->cld) == 4 && strcmp(Get_Child(ast->cld,1)->nodename,"LB") == 0)
                        {
                                Operand t1 = malloc(sizeof(struct Operand_));
                                //TODO?;
                                t1->temp_var_no = temp_var_index++;
                                
                                Operand t2 = malloc(sizeof(struct Operand_));
                                t2->temp_var_no = temp_var_index++;

                                Translate_Exp(ast->cld,t1,t);
                                Translate_Exp(ast->cld->bro->bro,t2,t);

                                InterCode i1 = malloc(sizeof(struct InterCode_));
                                i1->kind = ASSIGN;
                                i1->left = t1;
                                i1->right = t2;
                                Insert_InterCode(i1);

                                InterCode i2 = malloc(sizeof(struct InterCode_));
                                i2->kind = ASSIGN;
                                i2->left = o;
                                i2->right = t1;
                                o->kind = t1->kind;
                                Insert_InterCode(i2);
                                                          
                        }*/
                //        else assert(0); //TODO:
                
                else if(strcmp(ast->cld->bro->nodename,"PLUS") == 0 || \
                                strcmp(ast->cld->bro->nodename,"MINUS") == 0 || \
                                strcmp(ast->cld->bro->nodename,"STAR") == 0 || \
                                strcmp(ast->cld->bro->nodename,"DIV") == 0)
                {                
                        InterCode i = malloc(sizeof(struct InterCode_));
                        Operand leftop = malloc(sizeof(struct Operand_));
                        leftop->flag = 1;
                       // leftop->temp_var_no = temp_var_index++;

                        Operand rightop = malloc(sizeof(struct Operand_));
                       // rightop->temp_var_no = temp_var_index++;
                        rightop->flag = 1;

                        Translate_Exp(ast->cld,leftop,t);
                        Translate_Exp(Get_Child(ast,2),rightop,t);

                        if(o != NULL)
                        {
                                i->binop.op1 = leftop;
                                i->binop.op2 = rightop;
                                i->binop.result = o;

                                o->kind = TEMP_VARIBLE;
                                o->temp_var_no = temp_var_index++;

                                if(strcmp(ast->cld->bro->nodename,"PLUS") == 0)
                                {
                                        i->kind = ADD;
                                }
                
                                 else if(strcmp(ast->cld->bro->nodename,"MINUS") == 0)
                                {
                                        i->kind = SUB;
                                }

                                else if(strcmp(ast->cld->bro->nodename,"STAR") == 0)
                                {
                                        i->kind = MUL;
                                }

                                else if(strcmp(ast->cld->bro->nodename,"DIV") == 0)
                                {
                                        i->kind = DIV;
                                }
                                else assert(0);              
                                Insert_InterCode(i);
                        }
                }

                else if(strcmp(Get_Child(ast,1)->nodename,"RELOP") == 0 || \
                                strcmp(Get_Child(ast,1)->nodename,"AND") == 0|| \
                                strcmp(Get_Child(ast,1)->nodename,"OR") == 0)
                {
                        o->flag = 1;
                        Translate_Logical(ast,o,t);

                }

                else if(strcmp(ast->cld->nodename,"LP") == 0 && strcmp(Get_Child(ast,2)->nodename,"RP") == 0)
                {
                        Translate_Exp(ast->cld->bro,o,t);
                }
                else if(strcmp(ast->cld->nodename,"ID") == 0)
                {
                        Operand t1 = malloc(sizeof(struct Operand_));
                        t1->kind = FUNC;
                        strcpy(t1->funcname,ast->cld->special_val);
                        if(strcmp(t1->funcname,"read") == 0)
                        {
                                InterCode i = malloc(sizeof(struct InterCode_));
                                i->kind = IREAD;
                                o->kind = TEMP_VARIBLE;
                                o->temp_var_no = temp_var_index++;
                                i->iread.op = o;
                                Insert_InterCode(i);
                                return NULL;
                        }
                        if(o == NULL)
                                return NULL;
                        else{
                                InterCode i = malloc(sizeof(struct InterCode_));
                                i->kind = ICALL;
                                o->kind = TEMP_VARIBLE;
                                o->temp_var_no = temp_var_index++;
                                i->icall.result = o;
                                i->icall.func = t1;
                                Insert_InterCode(i);
                        }

                }
                else if(strcmp(ast->cld->bro->nodename,"DOT") == 0)
                {
                        printf("Cannot translate: Code contains varibles or parameters of structure type\n");
                        exit(0);
                }
                else assert(0);
        }

        else if(Count_Child(ast) == 2)
        {
               if(strcmp(ast->cld->nodename,"MINUS") == 0) //Exp -> MINUS Exp
               {
                       
                       Operand t2 = malloc(sizeof(struct Operand_));
                       t2->flag = 1;
              //         t2->temp_var_no = temp_var_index++;
                       
                       struct node *exp = Get_Child(ast,1);
                       Translate_Exp(exp,t2,t);
                       
                       if(o == NULL)
                               return NULL;
                       InterCode i = malloc(sizeof(struct InterCode_));
                       i->kind = SUB;

                       Operand t1 = malloc(sizeof(struct Operand_));
                       t1->kind = CONSTANT;
                       t1->value = 0;

                       i->binop.result = o;
                       i->binop.op1 = t1;
                       i->binop.op2 = t2;

                       o->kind = TEMP_VARIBLE;
                       o->temp_var_no = temp_var_index++;

                       Insert_InterCode(i);
               }
               else if(strcmp(ast->cld->nodename,"NOT") == 0)
               {
                    o->flag = 1;
                    Translate_Logical(ast,o,t);

               }
        }
        else if(Count_Child(ast) == 4){      
                if(strcmp(Get_Child(ast,1)->nodename,"LB") == 0 && strcmp(Get_Child(ast,3)->nodename,"RB") == 0)
                {
                       Operand place1 = malloc(sizeof(struct Operand_));
                       Operand place2 = malloc(sizeof(struct Operand_));
                       //place1->kind = ADDRESS;
          //             place1->temp_var_no = temp_var_index++;
                       
                       //place2->kind = TEMP_VARIBLE;
            //           place2->temp_var_no = temp_var_index++;

                       Type ty = Translate_Exp(Get_Child(ast,0),place1,t);
                      // printf("t->kind %d\n",t->kind);
                       assert(ty->kind == ARRAY); // It shouldnot happen

                       Translate_Exp(Get_Child(ast,2),place2,ty->array.elem);
                       if(o == NULL)
                               return NULL;
                       int factor = 4;
                       Type temp = ty->array.elem;
                       while(temp->kind  == ARRAY)
                       {
                               factor = factor * (temp->array.size);
                               temp = temp->array.elem;
                       }

                       if(!(ty->kind == ARRAY && ty->array.elem->kind == BASIC))
                        {

                       Operand t1 = malloc(sizeof(struct Operand_));
                       t1->kind = CONSTANT;
                       t1->value = factor;

                       Operand t2 = malloc(sizeof(struct Operand_));
                       t2->kind = TEMP_VARIBLE;
                       t2->temp_var_no = temp_var_index++;
                       
                       InterCode i1 = malloc(sizeof(struct InterCode_));
                       i1->kind = MUL;
                       i1->binop.op1 = place2;
                       i1->binop.op2 = t1;
                       i1->binop.result = t2;
                       Insert_InterCode(i1);

                       InterCode i2 = malloc(sizeof(struct InterCode_));
                       i2->kind = ADD;
                       i2->binop.op1 = place1;
                       i2->binop.op2 = t2;

                       o->kind = ADDRESS;
                       o->temp_var_no = temp_var_index++;
                       i2->binop.result = o;
                       Insert_InterCode(i2);
                        }
                    //   t = t->array.elem;
                       
                       else if(ty->kind == ARRAY && ty->array.elem->kind == BASIC)
                       {

                       Operand t1 = malloc(sizeof(struct Operand_));
                       t1->kind = CONSTANT;
                       t1->value = factor;

                       Operand t2 = malloc(sizeof(struct Operand_));
                       t2->kind = TEMP_VARIBLE;
                       t2->temp_var_no = temp_var_index++;
                       
                       InterCode i1 = malloc(sizeof(struct InterCode_));
                       i1->kind = MUL;
                       i1->binop.op1 = place2;
                       i1->binop.op2 = t1;
                       i1->binop.result = t2;
                       Insert_InterCode(i1);

                       InterCode i2 = malloc(sizeof(struct InterCode_));
                       i2->kind = ADD;
                       i2->binop.op1 = place1;
                       i2->binop.op2 = t2;

                       Operand t4 = malloc(sizeof(struct Operand_));
                       t4->kind = ADDRESS;
                       t4->temp_var_no = temp_var_index++;
                       i2->binop.result = t4;
                       Insert_InterCode(i2);
                       /* add in lab4 */

                      if(o->flag == 1)
                      {
                            InterCode i3 = malloc(sizeof(struct InterCode_));
                            i3->kind = ASSIGN;
                       
                            Operand t5 = malloc (sizeof(struct InterCode_));

                            t5->kind = DEREF;
                            t5->op = t4;

                            o->kind = TEMP_VARIBLE;
                            o->temp_var_no = temp_var_index++;

                            i3->assign.left = o;
                            i3->assign.right = t5;
                            Insert_InterCode(i3);
                      }
                      else {
                              o->kind = DEREF;
                              o->op = t4;
                      }
                       /* add in lab4 */
                       
                       }
                       else assert(0);
                       
                       return ty->array.elem;
                }
				else if (strcmp(ast->cld->nodename, "ID") == 0)
				{
						Operand f = malloc(sizeof(struct Operand_));
						f->kind = FUNC;
						strcpy(f->funcname, ast->cld->special_val);

						//Init_Args();
                                                
						struct node *args = Get_Child(ast, 2);
			
						//Translate_Args(args);//code1
                       // printf("%s\n",f->funcname);
                                                Operand arg_list[32];
						for (int i = 0; i < 32; i++)
							arg_list[i] = NULL;
						int arg_list_num = 0;
						Translate_Args(args,arg_list);//code1
						while (arg_list_num < 32 && arg_list[arg_list_num] != NULL)
						{
							arg_list_num = arg_list_num + 1;
						}


						if (strcmp(f->funcname, "write") == 0)
						{
						        InterCode i = malloc(sizeof(struct InterCode_));
						        i->kind = IWRITE;
						        i->iwrite.op = arg_list[0];
						        Insert_InterCode(i);
                                return NULL;
						}

	                    if(o == NULL)
                        {
                                for(int j = arg_list_num - 1; j >= 0 ; j--)
                                {
                                        if(arg_list[j]->kind == REF || arg_list[j]->kind == ADDRESS || arg_list[j]->kind == VARIBLE_ADDRESS)
                                        {
                                                o = malloc(sizeof(struct Operand_));
                                                o->kind = TEMP_VARIBLE;
                                                o->temp_var_no = temp_var_index++;
                                                break;
                                        }
                                }
                                if(o == NULL)
                                        return NULL;
                        }
					/*	else if(strcmp(f->funcname,"read") == 0)
                        {
                                printf("??\n");
                                InterCode i = malloc(sizeof(struct InterCode_));
                                i->kind = IREAD;
                                
                                o->kind = TEMP_VARIBLE;
                                o->temp_var_no = temp_var_index++;
                                i->iread.op = o;
                                Insert_InterCode(i);
                        }*/
                        
				        	
						//传入实参
						for (int j = arg_list_num - 1; j >= 0; j--)
						{
								InterCode p = malloc(sizeof(struct InterCode_));
								p->kind = IARG;
								p->iarg.op = arg_list[j];
								Insert_InterCode(p);
						}

						InterCode i = malloc(sizeof(struct InterCode_));
						i->kind = ICALL;
                        o->kind = TEMP_VARIBLE;
                        o->temp_var_no = temp_var_index++;
						i->icall.result = o;
						i->icall.func = f;
						Insert_InterCode(i);
				        	

		    }
        }
       return NULL; 
        //assert(0); //TODO:
}



void Translate_Logical(struct node *ast, Operand o,Type t)
{
        o->kind = TEMP_VARIBLE;
        o->temp_var_no = temp_var_index++;


        Operand l1 = malloc(sizeof(struct Operand_)); //lab1 = new temp()
        l1->kind = LABEL;
        l1->label_no = label_index;
        label_index++;

        Operand l2 = malloc(sizeof(struct Operand_)); //lab2 = new temp()
        l2->kind = LABEL;
        l2->label_no = label_index;
        label_index++;

        InterCode i0 = malloc(sizeof(struct InterCode_));
        i0->kind = ASSIGN;
        i0->assign.left = o;
        Operand t1 = malloc(sizeof(struct Operand_));
        t1->kind = CONSTANT;
        t1->value = 0;
        i0->assign.right = t1;
        Insert_InterCode(i0); //code 0 :place :#0

        Translate_Cond(ast,l1,l2,t); // code 1

        InterCode i2_1 = malloc(sizeof(struct InterCode_));
        i2_1->kind = DEC_L;
        i2_1->dec_l.label = l1;
        Insert_InterCode(i2_1); //code 2_1

        InterCode i2_2 = malloc(sizeof(struct InterCode_));
        i2_2->kind = ASSIGN;
        i2_2->assign.left = o;
        Operand t2 = malloc(sizeof(struct Operand_));
        t2->kind = CONSTANT;
        t2->value = 1;
        i2_2->assign.right = t2;
        Insert_InterCode(i2_2); // code 2_2

        InterCode i3 = malloc(sizeof(struct InterCode_));
        i3->kind = DEC_L;
        i3->dec_l.label = l2;
        Insert_InterCode(i3); //code 3
}

void Translate_Cond(struct node *ast, Operand l_true, Operand l_false,Type t)
{
        if(Count_Child(ast) == 3)
        {
                if(strcmp(Get_Child(ast,1)->nodename,"RELOP") == 0)
                {
                        Operand t1 = malloc(sizeof(struct Operand_));
                        Operand t2 = malloc(sizeof(struct Operand_));
                        t1->flag = 1;
                        t2->flag = 1;
                       // t1->kind = TEMP_VARIBLE;
        //                t1->temp_var_no = temp_var_index;
          //              temp_var_index++;

                       // t2->kind = TEMP_VARIBLE;
                       // t2->kind = TEMP_VARIBLE;
            //            t2->temp_var_no = temp_var_index;
              //          temp_var_index++;

                        Translate_Exp(ast->cld,t1,t);
                        Translate_Exp(Get_Child(ast,2),t2,t);

                        InterCode i1 = malloc(sizeof(struct InterCode_)); // IF GOTO LABEL_TRUE
                        i1->kind = IFGOTO;
                        strcpy(i1->ifgoto.rel,Get_Child(ast,1)->special_val);
                        i1->ifgoto.left = t1;
                        i1->ifgoto.right = t2;
                        i1->ifgoto.label = l_true;
                        Insert_InterCode(i1);

                        InterCode i2 = malloc(sizeof(struct InterCode_));
                        i2->kind = IGOTO;
                        i2->igoto.label = l_false;
                        Insert_InterCode(i2); // GOTO LABEL_FALSE
                }
                else if(strcmp(Get_Child(ast,1)->nodename,"AND") == 0)
                {
                        Operand label1 = malloc(sizeof(struct Operand_)); // lab1 = new label()
                        label1->kind = LABEL;
                        label1->label_no = label_index;
                        label_index++;

                        Translate_Cond(ast->cld,label1,l_false,t); // code1

                        InterCode i = malloc(sizeof(struct InterCode_));
                        i->kind = DEC_L;
                        i->dec_l.label = label1;
                        Insert_InterCode(i); // LABEL label1


                        Translate_Cond(Get_Child(ast,2),l_true,l_false,t); //code2
                }
                else if(strcmp(Get_Child(ast,1)->nodename,"OR") == 0)
                {
                        Operand label1 = malloc(sizeof(struct Operand_)); // lab1 = new label()
                        label1->kind = LABEL;
                        label1->label_no = label_index;
                        label_index++;

                        Translate_Cond(ast->cld,l_true,label1,t); // code1


                        InterCode i = malloc(sizeof(struct InterCode_));
                        i->kind = DEC_L;
                        i->dec_l.label = label1;
                        Insert_InterCode(i); // LABEL label1



                        Translate_Cond(Get_Child(ast,2),l_true,l_false,t); //code2
                }
                else Translate_Cond_Other(ast,l_true,l_false,t);
        }


        else if(Count_Child(ast) == 2 && strcmp(ast->cld->nodename,"NOT")==0)  //NOT EXP
        {
                Translate_Cond(Get_Child(ast,1),l_false,l_true,t);
        }
        else Translate_Cond_Other(ast,l_true,l_false,t);
}

void Translate_Cond_Other(struct node *ast,Operand l_true,Operand l_false,Type t)
{
        Operand t1 = malloc(sizeof(struct Operand_)); //t1 = new temp()
        //t1->kind = TEMP_VARIBLE;
    //    t1->temp_var_no = temp_var_index++;
        t1->flag = 1;
        Translate_Exp(ast,t1,t); // code1

        InterCode i2  = malloc(sizeof(struct InterCode_));
        i2->kind = IFGOTO;
        strcpy(i2->ifgoto.rel,"!=");
        i2->ifgoto.left = t1;

        Operand t2 = malloc(sizeof(struct Operand_));
        t2->kind = CONSTANT;
        t2->value = 0;

        i2->ifgoto.right = t2;
        i2->ifgoto.label = l_true; // IF GOTO LABEL_TRUE
        Insert_InterCode(i2);

        InterCode i3 = malloc(sizeof(struct InterCode_));
        i3->kind = IGOTO;
        i3->igoto.label = l_false; //GOTO LABEL_FALSE
        Insert_InterCode(i3);
}

void Translate_Stmt(struct node *ast)
{
	int mycount = Count_Child(ast);
	if (mycount == 1)//CompSt 
	{
		Translate_CompSt(ast->cld);
	}
	else if (mycount == 2)//Exp SEMI 
	{
        Type t = malloc(sizeof(struct Type_));
		Translate_Exp(ast->cld, NULL,t);
	}
	else if (mycount == 3)//RETURN Exp SEMI 
	{
        Type t = malloc(sizeof(struct Type_));

		Operand t1 = malloc(sizeof(struct Operand_));
		struct node *exp = ast->cld->bro;
		Translate_Exp(exp, t1,t);

		InterCode i = malloc(sizeof(struct InterCode_));
		i->kind = RET;
		i->ret.result = t1;

		Insert_InterCode(i);
	}
	else if (mycount == 5)
	{
		if (!(strcmp(ast->cld->nodename, "IF")))//IF LP Exp RP Stmt
		{
			Operand label_one = malloc(sizeof(struct Operand_)); //lab1 = new temp()
			label_one->kind = LABEL;
			label_one->label_no = label_index;
			label_index++;

			Operand label_two = malloc(sizeof(struct Operand_)); //lab2 = new temp()
			label_two->kind = LABEL;
			label_two->label_no = label_index;
			label_index++;

            Type t = malloc(sizeof(struct Type_));
			struct node *exp = Get_Child(ast,2);
			Translate_Cond(exp, label_one, label_two,t);

			InterCode i1 = malloc(sizeof(struct InterCode_));
			i1->kind = DEC_L;
			i1->dec_l.label = label_one;
			Insert_InterCode(i1);

			struct node *stmt = Get_Child(ast, 4);
			Translate_Stmt(stmt);

			InterCode i2 = malloc(sizeof(struct InterCode_));
			i2->kind = DEC_L;
			i2->dec_l.label = label_two;
			Insert_InterCode(i2);

			
		}
		else//WHILE LP Exp RP Stmt
		{
			Operand label_one = malloc(sizeof(struct Operand_)); //lab1 = new temp()
			label_one->kind = LABEL;
			label_one->label_no = label_index;
			label_index++;

			Operand label_two = malloc(sizeof(struct Operand_)); //lab2 = new temp()
			label_two->kind = LABEL;
			label_two->label_no = label_index;
			label_index++;

			Operand label_three = malloc(sizeof(struct Operand_)); //lab3 = new temp()
			label_three->kind = LABEL;
			label_three->label_no = label_index;
			label_index++;

			InterCode i0 = malloc(sizeof(struct InterCode_)); //0
			i0->kind = DEC_L;
			i0->dec_l.label = label_one;
			Insert_InterCode(i0);
			
            Type t = malloc(sizeof(struct Type_));
			struct node *exp = Get_Child(ast, 2); //1
			Translate_Cond(exp, label_two, label_three,t);

			InterCode i2 = malloc(sizeof(struct InterCode_)); //2
			i2->kind = DEC_L;
			i2->dec_l.label = label_two;
			Insert_InterCode(i2);

			struct node *stmt1 = Get_Child(ast, 4); //3
			Translate_Stmt(stmt1);

			InterCode i4 = malloc(sizeof(struct InterCode_)); //4
			i4->kind = IGOTO;
			i4->igoto.label = label_one;
			Insert_InterCode(i4);

			InterCode i5 = malloc(sizeof(struct InterCode_)); //5
			i5->kind = DEC_L;
			i5->dec_l.label = label_three;
			Insert_InterCode(i5);

		}
	}
	else if (mycount == 7)//IF LP Exp RP Stmt ELSE Stmt
	{
		Operand label_one = malloc(sizeof(struct Operand_)); //lab1 = new temp()
		label_one->kind = LABEL;
		label_one->label_no = label_index;
		label_index++;

		Operand label_two = malloc(sizeof(struct Operand_)); //lab2 = new temp()
		label_two->kind = LABEL;
		label_two->label_no = label_index;
		label_index++;

		Operand label_three = malloc(sizeof(struct Operand_)); //lab3 = new temp()
		label_three->kind = LABEL;
		label_three->label_no = label_index;
		label_index++;

        Type t = malloc(sizeof(struct Type_));
		struct node *exp = Get_Child(ast, 2); //0
		Translate_Cond(exp, label_one, label_two,t);

		InterCode i1 = malloc(sizeof(struct InterCode_)); //1
		i1->kind = DEC_L;
		i1->dec_l.label = label_one;
		Insert_InterCode(i1);

        struct node *stmt1 = Get_Child(ast, 4); //2
		Translate_Stmt(stmt1);

		InterCode i3 = malloc(sizeof(struct InterCode_)); //3
		i3->kind = IGOTO;
		i3->igoto.label = label_three;
		Insert_InterCode(i3);

		InterCode i4 = malloc(sizeof(struct InterCode_)); //4
		i4->kind = DEC_L;
		i4->dec_l.label = label_two;
		Insert_InterCode(i4);
		
		struct node *stmt2 = Get_Child(ast, 6); //5
		Translate_Stmt(stmt2);

		InterCode i6 = malloc(sizeof(struct InterCode_)); //6
		i6->kind = DEC_L;
		i6->dec_l.label = label_three;
		Insert_InterCode(i6);
	}

}

void Translate_CompSt(struct node * ast)
{
	struct node *deflist = ast->cld->bro;
	Translate_DefList(deflist);
	struct node *stmtlist = deflist->bro;
	Translate_StmtList(stmtlist);
}

void Translate_StmtList(struct node *ast)
{
	if (Count_Child(ast) == 0)
		return;
	else
	{
		struct node *stmt = ast->cld;
		Translate_Stmt(stmt);
		struct node *stmtlist = stmt->bro;
		Translate_StmtList(stmtlist);
	}
}

void Insert_Args(Operand arg_list[],Operand t)//把形参插入表中
{
	/*if (arg_list_num >= 32)
	{
		printf("arglist 空间不够\n");
	}*/
        int i = 0;
	while (i < 32 && arg_list[i] != NULL)
	{
		i++;
	}
	if (i == 32)
	{
		return;
	}
	
	Operand temp = malloc(sizeof(struct Operand_));
	temp->kind = t->kind;//其实是临时变量 TEMP_VARIBLE 
	temp->temp_var_no = t->temp_var_no;
	arg_list[i] = temp;
	//arg_list_num = arg_list_num + 1;
}

/*void Init_Args()
{
	//清空形参表；
	for (int i = 0; i < arg_list_num; i++)
	{
		Operand temp = arg_list[i];
		arg_list[i] = NULL;
		//delete temp;
	}
	arg_list_num = 0;
}*/

void Translate_Args(struct node *ast,Operand arg_list[])
{
    Type t = malloc(sizeof(struct Type_));
	if (Count_Child(ast) == 1)
	{
		Operand t1 = malloc(sizeof(struct Operand_)); // Operand t1; t1 := #value
        t1->flag = 1;
		//t1->kind = TEMP_VARIBLE;
		//t1->temp_var_no = temp_var_index;
		//temp_var_index++;
		Translate_Exp(ast->cld, t1,t);
		Insert_Args(arg_list,t1);
	}
	else
	{
		Operand t1 = malloc(sizeof(struct Operand_)); // Operand t1; t1 := #value
		//t1->kind = TEMP_VARIBLE;
		//t1->temp_var_no = temp_var_index;
		//temp_var_index++;
        t1->flag = 1;
		Translate_Exp(ast->cld, t1,t);
		Insert_Args(arg_list,t1);

		struct node *args = Get_Child(ast, 2);
		Translate_Args(args,arg_list);
	}
}

void Translate_ParamDec(struct node *ast)
{
	Translate_VarDec2(ast->cld->bro, Translate_Specifier(ast->cld));

}

Operand Translate_VarDec2(struct node *ast,Type t)
{
        
        if(Count_Child(ast) == 1)
        {
                struct node *id = ast->cld;
                FieldList f = Search_Symbol_name(id->special_val);
               // if(f->type->kind == ARRAY)
               // {
                        InterCode i1 = malloc(sizeof(struct InterCode_));
                        i1->kind = PARAM;
                        
                        Operand t1 = malloc(sizeof(struct Operand_));
                        t1->kind = VARIBLE;
                        t1->var_no = f->var_no;
                        i1->param.op = t1;


                       /* int result = 4;
                        Type temp = f->type;
                        while(temp->kind != BASIC)
                        {
                                result = result * temp->array.size;
                                temp = temp->array.elem;
                        }

                        i1->dec_arr.size = result;*/
                        Insert_InterCode(i1);

                        return t1;
                        
               // }
               // else {
                 //       Operand t2 = malloc(sizeof(struct Operand_));
                   //     t2->kind = VARIBLE;
                     //   t2->var_no = f->var_no;
                        
                       // return t2;
                //}
        }
        else if(Count_Child(ast) == 4){
                struct node *vardec = ast->cld;
                return Translate_VarDec2(vardec,t);
        }
        else assert(0);
}

void Translate_VarList(struct node *ast)
{
	int mycount = Count_Child(ast);
	if (mycount == 1)
	{
		Translate_ParamDec(ast->cld);
	}
	else if (mycount == 3)
	{
		Translate_ParamDec(ast->cld);
		Translate_VarList(Get_Child(ast, 2));
	}
	else assert(0);
}

void Translate_FunDec(struct node *ast)
{
	int mycount = Count_Child(ast);
	if (mycount == 3) //ID LP RP 
	{
		Operand f = malloc(sizeof(struct Operand_));
		f->kind = FUNC;
		strcpy(f->funcname, ast->cld->special_val);

		InterCode i = malloc(sizeof(struct InterCode_));
		i->kind = IFUNC;
		i->ifunc.op = f;
		Insert_InterCode(i);
	}
	else if (mycount == 4) //ID LP VarList RP
	{
		Operand f = malloc(sizeof(struct Operand_));
		f->kind = FUNC;
		strcpy(f->funcname, ast->cld->special_val);

		InterCode i = malloc(sizeof(struct InterCode_));
		i->kind = IFUNC;
		i->ifunc.op = f;
		Insert_InterCode(i);

		struct node *varlist = Get_Child(ast, 2);
		Translate_VarList(varlist);
	}
	else assert(0);
}

void Translate_Program(struct node *ast)
{
        Translate_ExtDefList(ast->cld);
}

void Translate_ExtDefList(struct node *ast)
{
        if(Count_Child(ast) == 0)
        {
                return ;
        }

        else if(Count_Child(ast) == 2)
        {
                struct node *extdef = ast->cld;
                Translate_ExtDef(extdef);
                struct node *extdeflist = ast->cld->bro;
                Translate_ExtDefList(extdeflist);
        }
        else assert(0);
}

void Translate_ExtDef(struct node *ast)
{
         Type t = Translate_Specifier(ast->cld);
        if(Count_Child(ast) == 3 && strcmp(Get_Child(ast,1)->nodename,"FunDec") == 0)
        {
                Translate_FunDec(Get_Child(ast,1));
                Translate_CompSt(Get_Child(ast,2));
                //TODO:
        }
        else
        {
                ;
        }
}

Type Translate_Specifier(struct node *ast)
{
        if(Count_Child(ast) == 1)
        {
                if(ast->cld->typeno == 2 && strcmp(ast->cld->special_val,"int") == 0)
                {
                        Type t = malloc(sizeof(struct Type_));
                        t->kind  = BASIC;
                        t->basic = 1;
                        return t;
                        
                }
                else
                {
                      // printf("")
                        //TODO:
                       // printf("\n");
                      // assert(0);
                      printf("Cannot translate: Code contains variables or parameters of structure type\n");
                      exit(0);
                        return NULL;
                }
        }
        else assert(0);
}        

void Translate_DefList(struct node *ast)
{
        if(Count_Child(ast) == 0)
                return ;
        else if(Count_Child(ast) == 2)
        {
                struct node *def = ast->cld;
                struct node *deflist = ast->cld->bro;
                Translate_Def(def);
                Translate_DefList(deflist);

        }
        else assert(0);
        
}

void Translate_Def(struct node *ast)
{
        struct node *specifier = ast->cld;
        struct node *declist = ast->cld->bro;
        Type t = Translate_Specifier(specifier);
        Translate_DecList(declist,t);
        
}

void Translate_DecList(struct node *ast,Type t)
{
        if(Count_Child(ast) == 1)
        {
                struct node *dec = ast->cld;
                Translate_Dec(dec,t);
        }
        else if(Count_Child(ast) == 3)
        {
                struct node *dec = ast->cld;
                struct node *declist = Get_Child(ast,2);
                Translate_Dec(dec,t);
                Translate_DecList(declist,t);
        }
        else assert(0);
}

void Translate_Dec(struct node *ast, Type t)
{
        if(Count_Child(ast) == 1)
        {
                Translate_VarDec(ast->cld,t);
        }
        else if(Count_Child(ast) == 3)
        {
                
                struct node *exp = Get_Child(ast,2);
                
                Operand t1 = malloc(sizeof(struct Operand_));
                t1->temp_var_no = temp_var_index++;
                Translate_Exp(exp,t1,t);
                
                InterCode i1 = malloc(sizeof(struct InterCode_));
                i1->kind = ASSIGN;
                i1->assign.right = t1;
                
                Operand t2 = Translate_VarDec(ast->cld,t);
                i1->assign.left = t2;
                Insert_InterCode(i1);
        }
}

Operand Translate_VarDec(struct node *ast, Type t)
{
        if(Count_Child(ast) == 1)
        {
                struct node *id = ast->cld;
                FieldList f = Search_Symbol_name(id->special_val);
                if(f->type->kind == ARRAY)
                {
                        InterCode i1 = malloc(sizeof(struct InterCode_));
                        i1->kind = DEC_ARR;
                        
                        Operand t1 = malloc(sizeof(struct Operand_));
                        t1->kind = VARIBLE;
                        t1->var_no = f->var_no;
                        i1->dec_arr.op = t1;


                        int result = 4;
                        Type temp = f->type;
                        while(temp->kind != BASIC)
                        {
                                result = result * temp->array.size;
                                temp = temp->array.elem;
                        }

                        i1->dec_arr.size = result;
                        Insert_InterCode(i1);

                        return t1;
                        
                }
                else {
                        Operand t2 = malloc(sizeof(struct Operand_));
                        t2->kind = VARIBLE;
                        t2->var_no = f->var_no;
                        
                        return t2;
                }
        }
        else if(Count_Child(ast) == 4){
                struct node *vardec = ast->cld;
                return Translate_VarDec(vardec,t);
        }
        else assert(0);
}



//Test func
void Print_Test(InterCode i)
{
        ;
}

void Free_intercodes(InterCodes head)
{
        InterCodes temp;
        InterCodes h=head;
        while(h!=NULL)
        {
            temp=h;
            h=h->next;
            free(temp->code);
            free(temp);
        }
}
void myfree2()
{
     Free_intercodes(InterCodeHead);
}

