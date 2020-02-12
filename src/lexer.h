#ifndef LEXER_FILE
#define LEXER_FILE
#include<stdio.h>

typedef enum type{ID,NUM,REAL,ASSIGNOP} type;
typedef struct token{
    char* str;
    type tag;
    struct token * next;    
}token;

extern token *getNextToken();

extern FILE* getStream(FILE *fp);

extern void removeComments(char* testfile, char* cleanfile);

extern int getLineNumber();
#endif
