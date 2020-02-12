/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
/*
Data structures used by lexer
*/
#ifndef _lexerdef_
#define _lexerdef_

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#define MAX_SIZE 30
#define BUFFER_SIZE 4096 //4KB 
#define MAX_LENGTH 20
typedef unsigned long long ull;
//check if we should use enum for tokens
extern int state =0;
static int end = 0;
static int begin = 0;
int numTokens=0;

ull line;
//discuss if code should be optimized further

const char* token_dict[MAX_SIZE]={"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE"
,"DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE"
,"TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT"};//all tokens

const char* keyword_dict[MAX_SIZE]={"integer","real","boolean","of","array","start","end","declare","module","driver","program","record"
,"tagged","union","get_value","print","use","with" ,"parameters","true","false","takes","input"
,"returns","AND","OR","for","in","switch","case","break","default","while"};//store all keywords

typedef char* buffer;
/*Handling multiple data types is left*/
typedef struct 
{
	char* token;
	char lexeme[MAX_LENGTH];//actual lexeme
	ull lineNo;
}node;// node stores info about token

typedef struct {
	node* link;
}head;

extern int retractions;//store number of retractions needed

#endif
