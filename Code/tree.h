extern int yylineno;
extern char* yytext;
#include<stdarg.h>
/*枚举所有可能出现的特殊类型
typedef enum special_type {
	MINT,
	MFLOAT, 
	MID,
	MTYPE,
	MEMPTY
};*/

struct node {
	char *nodename;//节点名称
	int lineno;//节点所在行号

	int typeno;//节点类型
	// id为1，type为2，int为3，float为4，其他终结符为5, 空的语法单元为0；
	char *special_val;
	struct node* cld;//子节点
	struct node* bro;//兄弟节点
};

struct node* nodelist[7];

void Free_tree(struct node *head);

void initlist();
float char_to_float(char *a);
struct node* leafcreate(char *, int);
struct node* nodecreate(char *, int);
void tree_print(struct node*, int);
void yyerror(char *s,...);
