#include<stdio.h>
#ifndef LEXER_FILE
#define LEXER_FILE
typedef enum type{ID,NUM,REAL,ASSIGNOP} type;
typedef struct token{
    char* str;
    type tag;
    struct token * next;    
}token;

token *getNextToken();

FILE* getStream(FILE *fp);

void removeComments(char* testfile, char* cleanfile);

int getLineNumber();
#endif
