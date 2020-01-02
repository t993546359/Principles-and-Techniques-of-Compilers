#include<stdio.h>
#include"syntax.tab.h"
struct node *root;
int main(int argc, char** argv)
{
  if(argc <= 1) return 1;
  FILE* f = fopen(argv[1], "r");
  if(!f)
  {
    perror(argv[1]);
    return 1;
  }
  yyrestart(f);
  yyparse();
  Init_Hash();
  Init_Func();
  printf("Init finished\n");
  GoProgram(root);
  Translate_Program(root);
//  Print_InterCode(argv[2]);
  
  FILE *fptr = fopen(argv[2],"w");
  if(!fptr)
  {
        perror(argv[2]);
        return 1;
  }
  Traversal_InterCodes(fptr);
  return 0;
}

/*
int main(int argc, char **argv)
{
	if(argc > 1){
		if(!(yyin = fopen(argv[1],"r"))){
			perror(argv[1]);
			return 1;
		}
	}
	yylex();
	return 0;
}
*/
