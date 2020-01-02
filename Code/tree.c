#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"
#define bool int
#define true 1
#define false 0
bool judge_flag = true;


//struct node* nodelist[7];

float char_to_float(char *a)
{
	return atof(a);
}

void initlist()
{
	for (int i = 0; i < 7; i++)
	{
		nodelist[i] = NULL;// (struct node *)malloc(sizeof(struct node));
	}
}

//叶子节点（即终结符）的生成，特例为空的语法单元，这里将它也作为一个终结符。
struct node* leafcreate(char* name, int lineno)
{
	struct node *leaf = (struct node *)malloc(sizeof(struct node));
        leaf->nodename = name;
	leaf->cld = NULL;//其子节点为空；
	leaf->lineno = lineno;//终结符所在行号；

	if (lineno == -1)//说明是空的语法单元
	{
		leaf->typeno = 0;
	}
	else {
		if (!strcmp(leaf->nodename, "ID"))
			leaf->typeno = 1;
		else if (!strcmp(leaf->nodename, "TYPE"))
			leaf->typeno = 2;
		else if (!strcmp(leaf->nodename, "INT"))
			leaf->typeno = 3;
		else if (!strcmp(leaf->nodename, "FLOAT"))
			leaf->typeno = 4;
        else if (!strcmp(leaf->nodename, "RELOP"))
            leaf->typeno = 6;
        else 
			leaf->typeno = 5;//其他终结符
	}

	//如果叶子节点为特殊type，则把其yytext转换成char* 存下待后续处理
	if (leaf->typeno < 5 || leaf->typeno == 6)
	{
		char* t = (char*)malloc(sizeof(char*) * 40);
		strcpy(t, yytext);
		leaf->special_val = t;
		
	}
        return leaf;
	
}

//非叶子节点的生成
//num表示产生式右侧语法单元个数，为0-7，0代表空的语法单元。
struct node* nodecreate(char *name, int num)//struct node* n1, struct node* n2, struct node* n3, struct node* n4, struct node* n5, struct node* n6, struct node* n7) {
{
	if (num == 0)//空的语法单元
		return leafcreate(name, -1);

	struct node* father = (struct node *)malloc(sizeof(struct node));

	father->nodename = name;
	father->bro = NULL;//初始化兄弟节点为null

	father->cld = nodelist[0];//其子节点为产生式的第一个语法单元
	father->lineno = father->cld->lineno;//父亲节点的行号跟随子节点的行号

	if (num > 1)
	{
		for (int i = 1; i < num; i++)
		{
			nodelist[i - 1]->bro = nodelist[i]; //依次串联兄弟节点
		}
	}
	return father;
}

/*struct node* mycreate(char *name, int num, int no)//struct node* n1, struct node* n2, struct node* n3, struct node* n4, struct node* n5, struct node* n6, struct node* n7) {
{
	struct node* father = (struct node *)malloc(sizeof(struct node));
	struct node* temp = (struct node *)malloc(sizeof(struct node));

	father->nodename = name;
	father->bro = NULL;
	father->cld = NULL;


	if (num > 0)
	{
		struct node* temp = (struct node *)malloc(sizeof(struct node));

		father->cld = nodelist[0];//其子节点为产生式的第一个语法单元
		father->lineno = temp->lineno;//父亲节点的行号跟随子节点的行号

		if (num > 1)
		{
			for (int i = 1; i < num; i++)
			{
				//temp->bro = nodelist[i];
				//temp = temp->bro;
				nodelist[i - 1]->bro = nodelist[i];
			}
		}
	}
	else
	{
		father->lineno = no;
		if (!strcmp(father->nodename, "ID"))
			father->typeno = 1;
		else if (!strcmp(father->nodename, "TYPE"))
			father->typeno = 2;
		else if (!strcmp(father->nodename, "INT"))
			father->typeno = 3;
		else if (!strcmp(father->nodename, "FLOAT"))
			father->typeno = 4;
		else
			father->typeno = 0;

		char* t = (char*)malloc(sizeof(char*) * 40);
		strcpy(t, yytext);
		father->special_val = t;
	}
	return father;
}
*/
void tree_print(struct node* root, int height)
{
    return ;
    if(!judge_flag) {root = NULL;}
	if (root == NULL)return;
	if(root->lineno != -1)
	for (int i = 0; i < height; i++)
		printf("  ");
	if (root->lineno != -1)
	{
                printf("%s", root->nodename);
                if(root->typeno==1 ||root->typeno==2)
                    printf(": %s", root->special_val);
		else if(root ->typeno ==3)
		    printf(": %d", atoi(root->special_val));
		else if(root->typeno ==4)
		    {
			float temp = char_to_float(root->special_val);
			printf(": %f",temp);
                    }
		else if(root->typeno==5);
                else
		    printf(" (%d)", root->lineno);
	}
        if(root->lineno!=-1)
	     printf("\n");
	tree_print(root->cld, height + 1);
	tree_print(root->bro, height);

}

void yyerror(char *s,...)
{
	judge_flag = false;
	extern yylineno;
	va_list ap;
	va_start(ap,s);
        struct node *temp = (struct node *)malloc(sizeof(struct node));
	temp =  va_arg(ap,struct node *);
	fprintf(stderr, "Error type B at Line %d: syntax error",yylineno);
//	fprintf(stderr,"%d",temp->lineno);
//	vfprintf(stderr,s,ap);
	fprintf(stderr,"\n");

}
void Free_tree(struct node *head)
{
        /*struct node * temp1,temp2,cld=head->cld,bro=head->bro;
        while(cld!=NULL||bro!=NULL)
        {
            temp1=cld;
            temp2=bro;
            if(cld!=NULL)free(cld);
            if(bro!=NULL)free(bro);
            h=h->next;
            free(temp->code);
            free(temp);
        }*/
        struct node * temp;
        struct node *h=head;
        while(h!=NULL)
        {
            temp=h;
            h=h->cld;
            Free_tree(temp->bro);
            free(temp);
        }
}
