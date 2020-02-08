#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#include "lexer.h"

node* create_node()
{
	node* temp = (node*)(malloc(sizeof(node)));
	return temp;
}
header set_header()
{

}
void flush(char* str)
{

}
char* keyword_check(char* val)//check if it is a keyword
{
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(strcmp(val,keyword_dict[i])==0)// it's a keyword
		{
			char *s = (char*)(malloc(sizeof(char)*(MAX_SIZE+1)));
			strcpy(s,token_dict[i]);
			return s;
		}
	}
	char temp[] = "NO";
	return temp;	
}
int remove_lines(char* in,int x)
{

}
int remove_comments(char* in,int x)
{


}
FILE* reloadBuffer(char* file)// input text from test file into buffer 
{

}
void remove(char* file)
{

}
node* getToken()
{
//getch

}
