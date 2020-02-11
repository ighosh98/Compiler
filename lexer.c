#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#include "lexer.h"
//lines are 1 indexed
static ull linenum = 1;
char x[BUFFER_SIZE];
//block must be modularized
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

//check if it is a keyword
char* keyword_check(char* inp)
{
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(strcmp(inp,keyword_dict[i])==0)// it's a keyword
		{
			char *s = (char*)(malloc(sizeof(char)*(MAX_SIZE+1)));
			strcpy(s,token_dict[i]);
			return s;
		}
	}
	char temp[] = "NO";
	return temp;	
}

// Removes newline '\n' '\r' present in lang spec?
int remove_lines(char* in,int x)
{
	while(in[x]=='\r' && in[++x]=='\n'){
		linenum++;
		x++;
	}
	return x;
}

// Removes comments
int remove_comments(char* in,int x)
{
	while(!(in[x]=='*' && in[x+1]=='*')){
		x++;
		if(in[x] == '\n'){
			linunum++;	
		}
	}
	return x;
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
	char* inp = (char*)malloc(sizeof(char)*(MAX_SIZE+1));
	char* token = (char*)malloc(sizeof(char)*(MAX_SIZE+1));
	//flush(inp);
	//flush(token);
	char c;
	int len = 0;
	//need to use peek
	while(true){
		c = x[end];
		switch(state){
			case 0:	
				if(c=='+'){
					node* lexer = createnode();
					strcpy(lexer->token,"PLUS");
					strcpy(lexer->value,'+');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					return lexer;
					break;
				} else if(c=='-'){
					node* lexer = createnode();
					strcpy(lexer->token,"MINUS");
					strcpy(lexer->value,'-');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					return lexer;
					break;
				} else if(c=='/'){
					node* lexer = createnode();
					strcpy(lexer->token,"DIV");
					strcpy(lexer->value,'/');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					return lexer;
					break;
				} else if(c==';'){
					node* lexer = createnode();
					strcpy(lexer->token,"SEMCOL");
					strcpy(lexer->value,';');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c=='['){
					node* lexer = createnode();
					strcpy(lexer->token,"SQBO");
					strcpy(lexer->value,'[');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c==']'){
					node* lexer = createnode();
					strcpy(lexer->token,"SQBC");
					strcpy(lexer->value,';');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c=='('){
					node* lexer = createnode();
					strcpy(lexer->token,"BO");
					strcpy(lexer->value,';');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c==')'){
					node* lexer = createnode();
					strcpy(lexer->token,"BC");
					strcpy(lexer->value,';');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c==','){
					node* lexer = createnode();
					strcpy(lexer->token,"COMMA");
					strcpy(lexer->value,';');
					lexer->lineNo = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c=='$'){
					node* lexer = createnode();
					strcpy(lexer->token,"END");
					strcpy(lexer->value,'$');//$ is End of string marker?
					lexer->lineNo = linenum;
					linenum=1;
					end=0;
					begin=0;
					state=0;
					return lexer;
					break;
				} 
			case 1:
				
			case 2:
				
			case 3:
				
			case 4:
		}
	}
}
