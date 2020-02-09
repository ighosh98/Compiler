#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#include "lexer.h"

static int linenum = 1;
char x[BUFFER_SIZE];
static int end = 0;
static int begin = 0;
int state=0;
int numTokens=0;

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

// Removes newline
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
	while(true){
		c = x[end];
		switch(state){
			case 0:	
				if(c=='+'){
					node* lexer = createnode();
					strcpy(lexer->token,"PLUS");
					strcpy(lexer->value,'+');
					lexer->tk->line = linenum;
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
					lexer->tk->line = linenum;
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
					lexer->tk->line = linenum;
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
					lexer->tk->line = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c=='['){
					node* lexer = createnode();
					strcpy(lexer->token,"SQO");
					strcpy(lexer->value,'[');
					lexer->tk->line = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c==']'){
					node* lexer = createnode();
					strcpy(lexer->token,"SQC");
					strcpy(lexer->value,';');
					lexer->tk->line = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c=='('){
					node* lexer = createnode();
					strcpy(lexer->token,"PO");
					strcpy(lexer->value,';');
					lexer->tk->line = linenum;
					end++;
					begin=end;
					state=0;
					numTokens++;
					len=0;
					return lexer;
					break;
				} else if(c==')'){
					node* lexer = createnode();
					strcpy(lexer->token,"PC");
					strcpy(lexer->value,';');
					lexer->tk->line = linenum;
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
					lexer->tk->line = linenum;
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
					strcpy(lexer->value,'$');
					lexer->tk->line = linenum;
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
