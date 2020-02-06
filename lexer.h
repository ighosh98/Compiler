#ifndef _lexer_
#define _lexer_

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#define MAX_SIZE 20
#define BUFFER_SIZE 100

int state;
int line;
typedef enum
{
INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE
,DRIVER,PROGRAM,RECORD,TAGGED,UNION,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TRUE,FALSE
,TAKES,INPUT,RETURNS,AND,OR,FOR,IN,SWITCH,CASE,BREAK,DEFAULT
} Token;

const Token token_dict[MAX_SIZE]={INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE
,DRIVER,PROGRAM,RECORD,TAGGED,UNION,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TRUE,FALSE
,TAKES,INPUT,RETURNS,AND,OR,FOR,IN,SWITCH,CASE,BREAK,DEFAULT};//all tokens

const char* keyword_dict[MAX_SIZE]={"integer","real","boolean","of","array","start","end","declare","module","driver","program","record"
,"tagged","union","get_value","print","use","with" ,"parameters","true","false","takes","input"
,"returns","AND","OR","for","in","switch","case","break","default","while"};//store all keywords

typedef char* buffer;

typedef struct 
{
	Token token;
	char val[MAX_SIZE];
	int line;
}TokenInfo;

typedef struct 
{
	TokenInfo* tk;
}node;

typedef struct {
	node* link;
}head;

extern node* create_node();
extern header set_header();
extern void flush(char* str);
extern char* keyword_check(char val[]);
extern int remove_lines(char* in,int x);
extern int remove_comments(char* in,int x);
extern FILE* reloadBuffer(char* file);//file ptr must be handled efficiently
extern void remove(char* file);
extern node* getToken();

#endif
