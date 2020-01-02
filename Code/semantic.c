#include "semantic.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
FieldList Head;
FieldList Tail;
int anonymous_count = 0;
struct func* if_func_exist(char* funcname)
{
	struct func *temp = funcHead;
	while (temp != NULL)
	{
		if (!strcmp(temp->name, funcname))
			return temp;
		temp = temp->next;
	}
	return NULL;
}
int Count_para(struct parameter *p)//计算形参个数；
{
	struct parameter *para = p;
	int mycount = 0;
	while (para != NULL)
	{
		mycount = mycount + 1;
		para = para->pnext;
	}
	return mycount;
}
bool if_para_match(struct parameter *ptemp1, struct parameter *ptemp2)
{
	while (ptemp1 != NULL && ptemp2 != NULL && (TypeEqual(ptemp1->ptype, ptemp2->ptype)))
	{
		ptemp1 = ptemp1->pnext;
		ptemp2 = ptemp2->pnext;
	}
	return ((ptemp1 == NULL) && (ptemp2 == NULL)) ? true : false;
}
struct parameter *Handle_Args(struct node *args)
{
	struct parameter *p = (struct parameter *)malloc(sizeof(struct parameter));
	struct node *exp = args->cld;
//	printf("11\n");
	if (exp->bro == NULL)
	{
		p->ptype = Handle_Exp(exp);
		p->pnext = NULL;
		return p;
	}
	else
	{
		p->ptype = Handle_Exp(exp);
		p->pnext = Handle_Args(exp->bro->bro);
		return p;
	}
}
int insert_func(struct func *f)
{
	if (funcHead == NULL)
		funcHead = f;
	else
		funcTail->next = f;
	funcTail = f;

	return 0;
}
void print_var()
{
  //      printf("ready print var\n");
        FieldList head=Head;
        while(head!=NULL)
        {
           
    //       printf("var name is %s",head->name);
           int pa;
        //       if(head->type->kind==BASIC)pa=head->type->basic;else pa=3;
      //         printf("type is %d\n",pa);
           head=head->next;
        }
      // printf("var print over\n");
}
void print_func()
{
       struct func *head=funcHead;
       while(head!=NULL)
       {
        //    printf("myname is %s\n",head->name);
            int re;
            if(head->retype->kind==BASIC)re=head->retype->basic;else re=4;
          //  printf("myretype is %d\n",re);
           // printf("pnum is %d\n",head->pnum);
            struct parameter *p=head->para;
            while(p!=NULL)
            {
               int pa;
               if(p->ptype->kind==BASIC)pa=p->ptype->basic;else pa=9;
             //  printf("ptype is %d\n",pa);
               p=p->pnext;
            }
            head=head->next;
       }
      // printf("over func print\n");
}
void Realse(FieldList func_local_val)
{
       if(func_local_val == NULL)
       {
               Tail = NULL;
               free(Head);
               Head = NULL;
       }
       else{
                Tail = func_local_val;
                FieldList ff = Tail->next;
                Tail->next=NULL;
                free(ff);
            
       }
	//delete!!!!
}
struct node *Get_Child(struct node *root,int i)
{
    struct node *temp = root -> cld;
    while(i--)
    {
        temp = temp->bro;
    }
    return temp;
}

unsigned int Count_Child(struct node *root)
{
        unsigned int count = 0;
        struct node *temp = root->cld;
        while(temp != NULL)
        {
                temp = temp -> bro;
                count++;
        }
        return count;
}  

void Init_Hash()
{

        Head = NULL;
        Tail = NULL;
}

bool Insert_Symbol_Var(FieldList f)
{
       // printf("insert %s",f->name);        
        if(Head == NULL)
        {
                Head = f;
                Tail = f;
        }
        else{
                Tail->next = f;
                Tail = f;
                Tail->next = NULL;
        }
        return true;
}


FieldList Search_Symbol_name(char *sname)
{
        FieldList temp = Head;
        while(temp != NULL)
        {
             //   printf("name:%s:%d\n",temp->name,temp->type->kind);
                if(strcmp(temp->name,sname) == 0)
                {
                        return temp;
                        break;
                }
                temp = temp->next;
        }
        
        if(temp == NULL)
                return NULL;
}
bool TypeEqual(Type p1,Type p2)
{
        if(p1 == NULL && p2 == NULL)
            return true;
        else if(p1 == NULL || p2 == NULL)
            return false;
        else if (p1->kind != p2->kind)
            return false;
        else {
                switch(p1->kind)
                {
                        case BASIC:
                                {
                                        return p1->basic == p2->basic;
                                        break;
                                }
                        case ARRAY:
                                {  
                                         return TypeEqual(p1->array.elem,p2->array.elem);
                                         break;
                                }
                        case STRUCTURE:
                                {
                                
                                FieldList h1 = p1->structure;
                                FieldList h2 = p2->structure;
                                if(h1 == NULL &&h2 == NULL)
                                        return true;
                                else {
                                        if(h1 == NULL || h2 == NULL)
                                                return false;
                                        else {
                                                if(strcmp(h1->name,h2->name) == 0)
                                                        return true;
                                                else return false;
                                        }
                                }
                                }

                        default :
                                assert(0);
                }
        }
        
}


Type Handle_Exp(struct node *ast)
{
        /*exp: |ID
         *     |INT
         *     |FLOAT
         */
       
        //!!!
        if (Count_Child(ast) != 1 && !strcmp(ast->cld->nodename, "ID"))//函数调用
	{
		struct node *id = ast->cld;
		char *funcname = id->special_val;
		struct func * myfun = if_func_exist(funcname);
		if (myfun == NULL)
		{
			if (Search_Symbol_name(funcname) == NULL)//函数表里查不到此id
			{
				//变量表里也查不到此id，故为函数调用未定义
				printf("Error type 2 at Line %d: Undefined function %s.\n", id->lineno, id->special_val);
				
			}
			else
			{
				//否则是对普通变量使用（）;
				printf("Error type 11 at Line %d: %s is not a function.\n", id->lineno, id->special_val);
			}
			return NULL;
		}
		else
		{
			if (!strcmp(id->bro->bro->nodename, "Args"))//
			{
				struct node* args = id->bro->bro;
				if (!if_para_match(myfun->para, Handle_Args(args)))
					printf("Error type 9 at Line %d: Function %s is not applicable.\n", id->lineno, id->special_val);

			}
			else//函数调用不带参数
			{
				if (myfun->pnum != 0)
					printf("Error type 9 at Line %d: Function %s is not applicable.\n", id->lineno, id->special_val);
			}
			return myfun->retype;
		}
	}
        if(Count_Child(ast) == 1)
        {
         //       printf("??%s\n",ast->cld->nodename);
                if(ast->cld->typeno == 1)
                {
           //             printf("..%s\n",ast->cld->special_val);
                        FieldList f = Search_Symbol_name(ast->cld->special_val);
                        if(f == NULL)
                        {
                                printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",ast->cld->lineno,ast->cld->special_val); // error type 1
                                return NULL;
                        }
                        else {
                                return f->type;
                        }      
                
                }
                else if(ast->cld->typeno == 3)
                {
                        Type t = malloc(sizeof(struct Type_));
                        t->kind = BASIC;
                        t->basic = 1;
                        return t;
                }
                else if(ast->cld->typeno == 4)
                {
                        Type t = malloc(sizeof(struct Type_));
                        t->kind = BASIC;
                        t->basic = 2;
                        return t;              
                }
         }

    
        else if(Count_Child(ast) == 3)
        {
                if((!(strcmp(Get_Child(ast,0)->nodename,"LP") == 0 &&\
                                strcmp(Get_Child(ast,2)->nodename,"RP") == 0)) &&\
                                strcmp(Get_Child(ast,1)->nodename,"DOT" ) != 0) {
                        Type t1 = Handle_Exp(Get_Child(ast,0));
                        Type t2 = Handle_Exp(Get_Child(ast,2));
                        /*exp:|exp ASSIGNOP exp
                        */
                        if(Get_Child(ast,1)->typeno == 5 && \
                                strcmp(Get_Child(ast,1)->nodename,"ASSIGNOP")== 0)
                        {

                                if(Count_Child(ast->cld) == 1 &&\
                                                strcmp(Get_Child(ast->cld,0)->nodename,"ID" ) == 0)
                                        ;
                                else if(Count_Child(ast->cld) == 4 &&\
                                                strcmp(Get_Child(ast->cld,0)->nodename,"EXP" ) == 0)
                                        ;
                                else if(Count_Child(ast->cld) == 3 &&\
                                                strcmp(Get_Child(ast->cld,1)->nodename,"DOT" ) == 0)
                                        ;
                                else {
                                        if(t1 == NULL);
                                        //Error type 6
                                        else
                                        printf("Error type 6 at Line %d: The left-hand side of an assignment must be a varia-ble\n",ast->cld->lineno);
                                        return NULL;
                                }
                                if(!TypeEqual(t1,t2))
                                {
                                        printf("Error type 5 at Line %d: Type missmatched for assignment\n",ast->cld->lineno);
                                        // error type 5
                                        return NULL;
                                }
                                else {
                                        return t1;
                                }
                        }

                        /*exp :|exp AND exp
                        *     |exp RELOP exp
                        *     |exp PLUS exp
                         *     |exp MINUS exp
                          *     |exp STAR exp
                        *     |exp DIV exp
                        */
                        if(t1 == NULL || t2 == NULL)
                        {
                                return NULL;
                        }
                        if(Get_Child(ast,1)->typeno == 5 && \
                                        strcmp(Get_Child(ast,1)->nodename,"AND")== 0)
                        {
                                if(!(t1->kind == BASIC && t2->kind == BASIC && t1->basic == t2->basic && t1->basic == 1) )
                                {
                                        printf("Error type 7 at Line %d: Type missmatched for Operands\n",ast->cld->lineno);
                                        //error type 7
                                return NULL;
                               // assert(0);
                               //
                                }
                                else return t1;
                        }
                       
                

                if(Get_Child(ast,1)->typeno == 5 && \
                                strcmp(Get_Child(ast,1)->nodename,"RELOP")== 0)
                {
                        if(!(t1->kind == BASIC && t2->kind == BASIC && t1->basic == t2->basic && t1->basic == 1) )
                        {
                                printf("Error type 7 at Line %d: Type missmatched for Operands\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                               // assert(0);
                        }
                        else return t1;
                }


                if(Get_Child(ast,1)->typeno == 5 && \
                                strcmp(Get_Child(ast,1)->nodename,"OR")== 0)
                {
                        if(!(t1->kind == BASIC && t2->kind == BASIC && t1->basic == t2->basic && t1->basic == 1) )
                        {

                                printf("Error type 7 at Line %d: Type missmatched for Operands\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                                //assert(0);
                        }
                        else return t1;
                }

                
                if(Get_Child(ast,1)->typeno == 5 && \
                                strcmp(Get_Child(ast,1)->nodename,"PLUS")== 0)
                {
                        if(!(t1->kind == BASIC && t2->kind == BASIC && t1->basic == t2->basic) )
                        {

                                printf("Error type 7 at Line %d: Type missmatched for Operands\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                                //assert(0);
                        }
                        else return t1;
                }       
                if(Get_Child(ast,1)->typeno == 5 && \
                                strcmp(Get_Child(ast,1)->nodename,"MINUS")== 0)
                {
                        if(!(t1->kind == BASIC && t2->kind == BASIC && t1->basic == t2->basic) )
                        {
                                printf("Error type 7 at Line %d: Type missmatched for Operands\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                                //assert(0);
                        }
                        else return t1;
                }       
                if(Get_Child(ast,1)->typeno == 5 && \
                                strcmp(Get_Child(ast,1)->nodename,"STAR")== 0)
                {
                        if(!(t1->kind == BASIC && t2->kind == BASIC && t1->basic == t2->basic) )
                        {

                                printf("Error type 7 at Line %d: Type missmatched for Operands\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                               // assert(0);
                        }
                        else return t1;
                }       
                if(Get_Child(ast,1)->typeno == 5 && \
                                strcmp(Get_Child(ast,1)->nodename,"DIV")== 0)
                {
                        if(!(t1->kind == BASIC && t2->kind == BASIC && t1->basic == t2->basic) )
                        {
                                printf("Error type 7 at Line %d:Type missmatched for Operands\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                                //assert(0);
                        }
                        else return t1;
                }
                }
                /*exp :|LP exp RP */
                else{
                 if(strcmp(Get_Child(ast,1)->nodename,"DOT") == 0)
                {
                        //EXP DOT ID
                        Type t = Handle_Exp(Get_Child(ast,0));
                        char sname[32];
                        strcpy(sname,(Get_Child(ast,2))->special_val);
                        if(t->kind != STRUCTURE)
                        {
                                //Error type 13
                                printf("Error type 13 at Line %d: Illegal use of \".\"\n",ast->cld->bro->lineno);
                                return NULL;
                        }
                        else{
                                FieldList temp = t->structure;
                                while(temp != NULL)
                                {
                                        if(strcmp(temp->name,sname) == 0)
                                                return temp->type;
                                        else
                                                temp = temp->st_type;
                                }
                                if(temp == NULL)
                                {
                                        //error type 14
                                        printf("Error type 14 at Line %d: Non_existent field \"%s\"\n",ast->cld->bro->lineno,sname);
                                        return NULL;
                                }
                            
                        }
                }
                 else{
                Type tmid = Handle_Exp(Get_Child(ast,1));
                if(strcmp(Get_Child(ast,0)->nodename,"LP") == 0 && strcmp(Get_Child(ast,2)->nodename,"RP") == 0)
                { 
                        return tmid;
                }
                }
                /*TODO:Exp:|ID LP RP
                 */
                // assert(0); // it shouldn't happen
                } 
                
        }       

        else if(Count_Child(ast) == 2)
        {
                Type t2 = Handle_Exp(Get_Child(ast,1));
                
                /*Exp:|MINUS Exp
                 *    |NOT Exp
                 *
                 */
                if(strcmp(Get_Child(ast,0)->special_val,"MINUS") == 0)
                {
                        if(t2->kind == BASIC && (t2->basic == 1|| t2->basic == 2))
                            return t2;
                        else {
                                printf("Error type 7 at Line %d: Type missmatched for assignment\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                        }
                }
                else if(strcmp(Get_Child(ast,0)->special_val,"NOT") == 0)
                {
                        if(t2->kind == BASIC && (t2->basic == 1))
                                return t2;
                        else {
                                printf("Error type 7 at Line %d: Type missmatched for assignment\n",ast->cld->lineno);
                                return NULL;
                                //error type 7
                        }
                }
                else assert(0);  // It shouldn't happen  
        }
        else if(Count_Child(ast) == 4)
        {
                /*Exp|Exp LB Exp RB
                 *
                 * */
                if(strcmp(Get_Child(ast,1)->nodename,"LB") == 0 && strcmp(Get_Child(ast,3)->nodename,"RB") == 0)
                {
                        Type t1 = Handle_Exp(Get_Child(ast,0));
                        Type t3 = Handle_Exp(Get_Child(ast,2));
                        if((t3->kind != BASIC) ||(t3->basic != 1) )
                        {
                                printf("Error type 12 at Line %d: is not a integer\n",ast->cld->lineno);
                                return NULL;
                                //error type 12 : a[1.5]
                        }
                        else if(t1->kind != ARRAY)
                        {
                                printf("Error type 10 at Line %d: is not a array \n",ast->cld->lineno);
                                return NULL;
                                //error type 10   int a; a[10]
                        }
                        else return t1->array.elem;
                }
                
                
                /*TODO:Exp:|ID LP Args RP
                 */
                else assert(0);
        }
        else
        {
                assert(0); //It shouldn't happen..
                return NULL;
        }
}

/*
VarDec : ID {}   FieldList Handle_VarDec()
 * | VarDec LB INT RB{}
 * ;
 */

FieldList Handle_VarDec(struct node *root, Type t, int i)
{
	if(i == 0){
   // printf("Enter Handle_VarDec\n");
    FieldList f = malloc(sizeof(struct FieldList_));
    if(Count_Child(root) == 1)
    {
     //       printf("VarDec ->ID\n");
            strcpy(f->name,root->cld->special_val);
            f->type = t;
       //     printf("%s\n",f->name);       
            
    }

    else if(Count_Child(root) == 4 &&Count_Child(Get_Child(root,0)) ==1 )
    {
            struct node *id = root->cld->cld;
            strcpy(f->name,id->special_val);
            Type t1 = malloc(sizeof(struct Type_));
            t1->kind = ARRAY;
            t1->array.size = atoi(root->cld->bro->bro->special_val);
            t1->array.elem = t;
            f->type = t1;
    }

    
    else if(Count_Child(root) == 4 && Count_Child(Get_Child(root,0))== 4)
    {
            strcpy(f->name,root->cld->special_val);
            Type t1 = malloc(sizeof(struct Type_));
            Type t2 = malloc(sizeof(struct Type_));
            t1->kind = ARRAY;
            t1->array.size = atoi(root->cld->bro->bro->special_val);
            t1->array.elem = t;
            t2->kind = ARRAY;
            t2->array.size = atoi(root->cld->cld->bro->bro->special_val);
            t2->array.elem = t1;
            f->type = t2;
    }
    else {    
        printf("error in goVarDec case .\n");
        return NULL;
        }
   // printf("return \n");
    return f;
	
}
else
{
       // printf("Enter local array\n");
        FieldList f = malloc(sizeof(struct FieldList_));
	if (Count_Child(root) == 1)
	{
     //           printf("xxxxxxxxxxxxxvvvvvvvvv\n");
		strcpy(f->name, root->cld->special_val);
		f->type = t;
        if(Search_Symbol_name(f->name))
        {
                printf("?Error type 3 at Line %d:  Redefined variable \"%s\"\n",root->cld->lineno,f->name);
                //redefined var
                return NULL;
        }
        else
		Insert_Symbol_Var(f);
		return f;
	}
	else if (Count_Child(root) == 4 && Count_Child(Get_Child(root, 0)) == 1)
	{
        //    printf("Enter array[]\n");
		//！！！这里需要递归实现多维数组！！
		struct node *id = root->cld->cld;
		strcpy(f->name, id->special_val);
		Type t1 = malloc(sizeof(struct Type_));
		t1->kind = ARRAY;
		t1->array.size = atoi(root->cld->bro->bro->special_val);
		t1->array.elem = t;
		f->type = t1;
        if(Search_Symbol_name(f->name) != NULL)
        {
                printf("Error type 3 at Line %d:  Redefined variable \"%s\"\n",root->cld->lineno,f->name);
                return NULL;
        }
        else{
		Insert_Symbol_Var(f);//数组加入符号表。
		return f;
	    }
    }



	else if (Count_Child(root) == 4 && Count_Child(Get_Child(root, 0)) == 4)
	{
		strcpy(f->name, root->cld->cld->cld->special_val);
		Type t1 = malloc(sizeof(struct Type_));
		Type t2 = malloc(sizeof(struct Type_));
		t1->kind = ARRAY;
		t1->array.size = atoi(root->cld->bro->bro->special_val);
		t1->array.elem = t;
		t2->kind = ARRAY;
		t2->array.size = atoi(root->cld->cld->bro->bro->special_val);
		t2->array.elem = t1;
		f->type = t2;
        if(Search_Symbol_name(f->name) != NULL)
        {
                printf("Error type 3 at Line %d:Redefined variable \"%s\"\n",root->cld->lineno,f->name);
                return NULL;
        }
        else {
        Insert_Symbol_Var(f);
		return f;
        }
                return NULL;
	}
}
}
/* Specifier : Type {}
 * | StructSpecifier{}  Handle_StructSpecfier(Struct node *)
 *
*StructSpecifier : Struct OptTag LC DefList RC {}
 *   |Struct Tag {}
 */

Type Handle_Specifier(struct node *root)
{
       // printf("Enter Handle_Specifier!\n");
        if(Count_Child(root) == 1)
        {
                Type t = malloc(sizeof(struct Type_));
                if(root->cld->typeno == 2)   //
                {
         //               printf("Specifier -> Basic type!\n");
                        t->kind = BASIC;
                        if(strcmp(root->cld->special_val,"int") == 0)
                        {
                                t->basic = 1;
                        }
                        else if(strcmp(root->cld->special_val,"float") == 0)
                        {
                                t->basic = 2;
                        }
                        else {
                                printf("Error in Specifier node in ast\n");
                               // return NULL;
                                assert(0); // it shouldn't happen
                        }
                }
                else{
                        return Handle_StructSpecifier(root->cld); // Struct OptTag LC DefList RC
                }

        }
        else {
                printf("Error in Specifier node in ast\n");
                assert(0);
        }
}

Type Handle_StructSpecifier(struct node *root)
{
       // printf("Enter Handle_StructSpecifier\n");
        if(Count_Child(root) == 5)  //struct OptTag LC ... RC 
        {
               // printf("Enter StructSpecifier -> struct OptTag LC DefList RC!\n");
                FieldList f = malloc(sizeof(struct FieldList_));
                Type t = malloc(sizeof(struct Type_));
                Type t2 =malloc(sizeof(struct Type_));
                t->kind = STRUCTURE_TYPE;
                t2->kind = STRUCTURE;
                char sname[32];
                struct node *opttag = Get_Child(root,1);
               // printf("???\n"); 
                if(Count_Child(opttag) == 0)  // OptTag->NULL
                {
                       sprintf(sname,"%d",anonymous_count);
                       anonymous_count++;
                }
                else if (opttag->cld->typeno == 1) // OptTag->ID
                {
                       // printf("??? %s\n",opttag->cld->special_val);
                        strcpy(sname,opttag->cld->special_val);
                       // printf("??? %s\n",sname);
                }
                else{
                        assert(0);
                }
                //printf("???\n");
                strcpy(f->name,sname);
                //printf("OptTag name : %s\n",f->name);                
                struct node *deflist = Get_Child(root,3);
                t->structure_type =t2;
                t2->structure = Handle_DefList(deflist,0);
                f->type = t;
                //printf("Flag %s,%s,%s,%s\n",t2->structure->name,t2->structure->st_type->name,\
                                t2->structure->st_type->next->name,t2->structure->next->next->next->name);
                if(Search_Symbol_name(sname) != NULL)
                {
                  //      printf("???\n");
                        printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",opttag->cld->lineno,f->name);
                        return NULL;
                        //Error type 16 use same var name to define struct;
                }
                else if(Insert_Symbol_Var(f))
                { 
                //        printf("Success Inserted %s\n",f->name);
                        return t->structure_type;
                }
                else assert(0);
        }
        else if(Count_Child(root) == 2)  // struct tag
        {
           //     printf("Structure -> Struct tag\n");
                struct node *tag = Get_Child(root,1);
                //assert(tag->typeno == 1);
                FieldList f = Search_Symbol_name(tag->cld->special_val);
             //   printf("..%d,..%d\n",f->type->kind,STRUCTURE_TYPE);
                
                if(f == NULL)
                {
                        printf("Error type 17 at Line %d: Undefined structure: \"%s\"\n",root->cld->bro->lineno,tag->cld->special_val);
                        return NULL;
                        //Error type 17    use undefined struct define var
                }
                else  if(f->type->kind != STRUCTURE_TYPE)
                {
                        printf("Error type 17 at Line %d:  Undefined structure: \"%s\"\n",root->cld->bro->lineno,tag->cld->special_val);
                        return NULL;
                        //Error type 17
                }
                else
                {
                    return f->type->structure;
                
                }
        }
        else{
                printf("Error in Handle_StructSpecifier \n");
                assert(0); // It shouldn't happen! 
        }
}

/*DefList | Def DefList
 *        | Null
 *Def     |Specifier Declist SEMI
 */

FieldList Handle_DefList(struct node *root,int i)
{
        if(i == 0){
       // printf("Enter Handle_DefList : struct\n");

        if(Count_Child(root) == 0)
        {
         //       printf("Enter DefList -> NULL\n");
                return NULL;
        }
        else 
        {
           //     printf("Enter DefList->Def DefList\n");
                FieldList f = malloc(sizeof(struct FieldList_));
                Type t = malloc(sizeof(struct Type_));

                assert(strcmp(root->cld->cld->bro->bro->nodename, "SEMI") == 0);

                struct node *def = root->cld;
                struct node *specifier = def->cld;
                struct node *declist = def->cld->bro;
                
                t = Handle_Specifier(specifier);
                
                f = Handle_DecList(declist,t,i);
                
                struct node *deflist = root->cld->bro;
                FieldList next_deflist = Handle_DefList(deflist,i);
                //printf("next: %s\n",next_deflist->name);
                if(f == NULL)
                {
                //    assert(0);    
                    return next_deflist;
                }
                FieldList temp = f;
                
                //printf("name :%s\n",temp->next->name);
                while(temp ->st_type != NULL)
                        {
             //                   printf("key:%s ",temp->name);
                                temp = temp->st_type;
                        }
               // printf("%s",temp->name);
               // printf("\n");
                //printf("%s => %s\n",temp->name,next_deflist->name);
                    temp->st_type = next_deflist;

                return f;
        }
        }
        else {
                /*TODO:Funcdec part*/
		if (root->cld == NULL)
			return NULL;
		else
		{
             //          printf("jjjjjjjjjjjjjjjjj\n");
			//FieldList f = malloc(sizeof(struct FieldList_));

			//assert(strcmp(root->cld->cld->bro->bro->name, "SEMI") == 0);

			struct node *def = root->cld;
			struct node *specifier = def->cld;
			struct node *declist = def->cld->bro;

			//处理def
			Type t = Handle_Specifier(specifier);
			Handle_DecList(declist, t, 1);

			//处理deflist
			struct node *deflist = root->cld->bro;
			Handle_DefList(deflist,1);

			return NULL;
		}
                assert(0);
        }
}

/*DecList |Dec
 *        |Dec COMMA Declist
 *Dec     |VarDec
 *        |VarDec AssignOp Exp
 */

FieldList Handle_DecList(struct node *root ,Type t, int i)
{
        if(i == 0){
                //printf("Enter Handle_DecList :struct\n");
        if(Count_Child(root) == 1 )
        {
                
                FieldList f = malloc(sizeof(struct FieldList_));
                if(Count_Child(root->cld) == 1)
                {
                        struct node *vardec = root->cld->cld;
                        f =  Handle_VarDec(vardec,t,0);
                        if(Search_Symbol_name(f->name) == NULL)
                        {
                                if(Insert_Symbol_Var(f))
                                       ;
                                        // printf("Success Inserted :%s \n",f->name);
                                else assert(0);
                        }
                        else{
                                //Error type 15
                                printf("Error type 15 at Line %d: Redefined field \"%s\"\n",root->cld->lineno,f->name);
                                return NULL;
                        }
                        f->st_type = NULL;
                              
                        return f;               
                }
                else if(Count_Child(root->cld) == 3)
                {
                                printf("Error type 15 at Line %d: Redefined field \"%s\"\n",root->cld->lineno,f->name);
                        //Error type 15

                }
                else assert(0);
        }
        else if(Count_Child(root) == 3)
        {
                struct node *dec = root->cld;
                FieldList f = malloc(sizeof(struct FieldList_));
                if(Count_Child(dec) == 1)
                {
                        struct node *vardec = root->cld->cld;
                        f =  Handle_VarDec(vardec,t,0);

                        if(Search_Symbol_name(f->name) == NULL)
                        {
                                if(Insert_Symbol_Var(f))
                                        ;
                     //                   printf("Success Inserted :%s \n",f->name);
                                else assert(0);
                        }
                        else{
                                //Error type 15
                                printf("Error type 15 at Line %d: Redefined field \"%s\"\n",root->cld->lineno,f->name);
                                return NULL;
                            }
                }
                else if(Count_Child(dec) == 3)
                {
                                printf("Error type 15 at Line %d: Redefined field \"%s\"\n",root->cld->lineno,f->name);
                        return NULL;
                       //Error type 15 
                        
                }
                else assert(0);
                f->st_type = Handle_DecList(root->cld->bro->bro,t,i);
                
                //printf("\nname:%s => %s\n",f->name,f->next->name) ;
                return f;
        }
        else assert(0);

        }
        else {//函数的局部变量定义
		/*TODO*/


		if (Count_Child(root->cld) == 1)
		{
			struct node *vardec = root->cld->cld;
			Handle_VarDec(vardec, t, 1);
			return NULL;
		}
		else if (Count_Child(root->cld) == 3)//定义局部变量的同时进行赋值
		{
			struct node *vardec = root->cld->cld;
			struct node *exp = vardec->bro->bro;
			Handle_VarDec(vardec, t, 1);

			Type exp_type = Handle_Exp(exp);
			if (!TypeEqual(exp_type, t))
				printf("Error type 5 at Line %d:Type missmatched for assignment\n",root->cld->cld->lineno);

		}
		else assert(0);

		if (Count_Child(root) == 1)
		{
			;
		}
		else if (Count_Child(root) == 3)
		{
			Handle_DecList(root->cld->bro->bro, t, 1);
		}
        else {
                assert(0);
        }
}}

/*ExtDefList: |ExtDef ExtDefList
 *            |NUll
 *ExtDef      |Specifier ExtDecList SEMI
              |Specifier SEMI
              |Specifier FunDec CompSt
 ExtDecList   |VarDec
              |VarDec COMMA ExtDeclist
*/



struct parameter *Handle_VarList(struct node *varlist)
{
	struct parameter *p = (struct parameter *)malloc(sizeof(struct parameter));
	struct node* paramdec = varlist->cld;
	p->ptype = Handle_Specifier(paramdec->cld);

	//这里把形参作为局部变量存进符号表
	FieldList new_var = (FieldList)malloc(sizeof(struct FieldList_));
	struct node* vardec = paramdec->cld->bro;
	if (strcmp(vardec->cld->nodename, "ID"))//不为id，说明形参出现数组，报错！
	{
		//error
	}
	else
	{
		strcpy(new_var->name,vardec->cld->special_val);
                new_var->type=p->ptype;
		Insert_Symbol_Var(new_var);
	}
	//end

	if (paramdec->bro == NULL)
	{
		p->pnext = NULL;
		return p;
	}
	else
	{
		p->pnext = Handle_VarList(paramdec->bro->bro);
		return p;
	}
}
/*
FunDec  ID LP VarList RP                  func(…) //4：函数重定义
          | ID LP RP                       func()//4：函数重定义
VarList  ParamDec COMMA VarList         一个或多个形参，以逗号隔开
          | ParamDec
ParamDec  Specifier VarDec
*/
int Handle_FunDec(struct node *fundec, Type t)
{
	char* func_name;
	func_name = fundec->cld->special_val;
	if (if_func_exist(func_name) != NULL)//函数重定义
	{
		printf("Error type 4 at Line %d: Redefined function %s.\n", fundec->cld->lineno, fundec->cld->special_val);
		return -1;
	}
	/*
	//返回类型不匹配错误
	struct node* compst = fundec->bro;
	//printf("4\n");
	//printf("%s\n", specifier->special_val);
	struct node* stmtlist = compst->cld->bro->bro;
	int judge = if_return_match(t, stmtlist);
	printf("5\n");
	if (judge != 0)
		printf("Error type 8 at Line %d: Type mismatched for return.\n", judge);
	printf("6n");
	*/
	struct func *myfun = (struct func *)malloc(sizeof(struct func));
	//printf("5\n");
	myfun->name = func_name;
	//printf("6\n");
	myfun->retype = t;
	myfun->next = NULL;
	if (Count_Child(fundec)==4&&(!strcmp(Get_Child(fundec,2)->nodename, "VarList")))
	{
		struct node* varlist = fundec->cld->bro->bro;
		myfun->para = Handle_VarList(varlist);
		myfun->pnum = Count_para(myfun->para);
	}
	else if(Count_Child(fundec) == 3 && (!strcmp(Get_Child(fundec, 2)->nodename, "RP")))
	{
		myfun->para = NULL;
		myfun->pnum = 0;
	}
	//printf("7\n");
	insert_func(myfun);
	return 0;
}
void Handle_Stmt(struct node *stmt, Type t)
{
	int num = Count_Child(stmt);
    //printf("num :%d\n",num);
	if (num == 2)
	{
      //           printf("200000\n");
		Handle_Exp(stmt->cld);
	}
	else if (num == 1)
	{
                 //printf("100000\n");
		Handle_CompSt(stmt->cld,t);
	}
	else if (num == 3)
	{
                 //printf("300000\n");
		Type exp_type = Handle_Exp(stmt->cld->bro);
		if (!TypeEqual(exp_type, t))
		{
			printf("Error Type 8 at Line %d: Type mismatched for return.\n",stmt->cld->bro->lineno);
		}
	}
	else if (num == 7)
	{
         //        printf("700000\n");
		// IF LP Exp RP Stmt ELSE Stmt
		//这里的exp需要处理吗？
		struct node *exp = stmt->cld->bro->bro;
		struct node *temp = exp->bro->bro;
		Handle_Exp(exp);
		Handle_Stmt(temp,t);
		temp = temp->bro->bro;
		Handle_Stmt(temp, t);
	}
	else if (num == 5)
	{
                 //printf("500000\n");
		struct node *exp = stmt->cld->bro->bro;
		struct node *temp = exp->bro->bro;
        Handle_Exp(exp);
		Handle_Stmt(temp, t);
	}
}
int Handle_StmtList(struct node *stmtlist, Type t)
{
	struct node* stmt = stmtlist->cld;
	if (stmt==NULL)
	{
		return 0;
	}
	else
	{
                 //printf("1111\n");
		Handle_Stmt(stmt, t);
                //  printf("22222\n");
                //   printf("%s\n",stmt->bro->nodename);
		Handle_StmtList(stmt->bro, t);
                //  printf("33333\n");
	}
}
int Handle_CompSt(struct node *compst, Type t)
{
        struct node* deflist=compst->cld->bro;
	FieldList func_local_val = Tail;
        /// printf("ready to handle deflist\n");
	Handle_DefList(deflist,1);
          //printf("deflist over\n");
          //printf("ready to handle stmtlist\n");
	Handle_StmtList(deflist->bro,t); 
           //printf("stmtlist over\n");
       // print_func();
        // print_var();
        // printf("ready to printfunc\n");
	Realse(func_local_val);//局部变量语句块结束后释放
        
}
void Handle_ExtDefList(struct node *root)
{

/*ExtDefList: |ExtDef ExtDefList
 *            |NUll
 *ExtDef      |Specifier ExtDecList SEMI
              |Specifier SEMI
              |Specifier FunDec CompSt
*/
     //   printf("Enter Handle_ExtDefList!\n");
       // printf("%d\n",Count_Child(root));
//        printf("%s\n",root->cld->bro->nodename);
        if(Count_Child(root) == 0)  // NULL
        {
          //      printf("Enter ExtDefList -> null!\n");
                return ;
        }
        else if(Count_Child(root) == 2) // ExtDef ExtDefList
        {
            //    printf("Enter ExtDefList->ExtDef ExtDefList! \n");
                struct node *extdef = root->cld;
                struct node *specifier = root->cld->cld;
                Type t = Handle_Specifier(specifier);
                if(t != NULL){

                        if(Count_Child(extdef) == 3 && \
                                        strcmp(Get_Child(extdef,2)->nodename,"SEMI") == 0)  // Specifier ExtDecList SEMI
                        {
             //                   printf("Enter ExtDef -> Specifier ExtDecList SEMI\n");
                                FieldList f = malloc(sizeof(struct FieldList_)); 
                          //      printf("???\n");
                                f = Handle_ExtDecList(Get_Child(extdef,1),t);
                        //        printf("\n");
                        
                        }

                        else if(Count_Child(extdef) == 2)  // Specifier SEMI
                        {
                   
                            ;
                        }
                /*TODO: Specifier Fundexc
                 */
                        else {
								//  printf("ready to handle func\n");
                                //struct func *f = malloc(sizeof(struct func));
								FieldList func_local_val = Tail;
								Handle_FunDec(Get_Child(extdef, 1), t);
								//     printf("handle fundec over\n");
								Handle_CompSt(Get_Child(extdef, 2), t);
								//        printf("handle compst over\n");
                                //assert(0);
                        }
                        Handle_ExtDefList(root->cld->bro);
                }
                else {
                        Handle_ExtDefList(root->cld->bro);
                        
                }
        }

        else {
                assert(0);
        }
}


FieldList Handle_ExtDecList(struct node *root, Type t)
{
 /*ExtDecList   |VarDec
              |VarDec COMMA ExtDeclist
*/
       // printf("Enter Handle_ExtDecList!\n");
        if(Count_Child(root) == 1)  // VarDec
        {
         //      printf("ExtDecList -> VarDec\n");
               FieldList f = malloc(sizeof(struct FieldList_));
               f = Handle_VarDec(root->cld,t,0);
              //  assert(0);
//               printf("??? %s \n",f->name);
               if(Search_Symbol_name(f->name) != NULL)
               {
                        //Error type 3
                        printf("Error type 3 at Line %d: Redefined variable \"%s\"\n",root->cld->lineno,f->name);
                        return NULL;
               }
               else {
                       if(Insert_Symbol_Var(f))
                         {
           //                      printf("Success Inserted %s\n",f->name);
                                 ;
                         }
                       else assert(0);
               }
               return f;
        }

        else if(Count_Child(root) == 3 && \
                        strcmp(Get_Child(root,1)->nodename,"COMMA") == 0)  //VarDec COMMA ExtDecList
        {
                FieldList f1 = malloc(sizeof(struct FieldList_));
                FieldList f2 = malloc(sizeof(struct FieldList_));
                f1 = Handle_VarDec(root->cld,t,0);

               if(Search_Symbol_name(f1->name) != NULL)
               {
                        //Error type 3
                        printf("Error type 3 at Line %d: Redefined variable \"%s\"\n",root->cld->lineno,f1->name);
                        return NULL;
               }
               else {
                       if(Insert_Symbol_Var(f1))
                       {
                               ;
           //                    printf("Success inserted %s\n",f1->name);
                       }
                       else assert(0);
               }

                struct node *extdeclist = Get_Child(root,2);
                f2 = Handle_ExtDecList(extdeclist,t);

                return f1;
        }
        else assert(0); // It shouldn't happen!
}

/*Program : ExtDefList {}
 * ;
 */

void GoProgram(struct node *root)
{
       // printf("Enter GoProgram!\n");
        Handle_ExtDefList(root->cld);
}
