#ifndef LEXER_FILE
#define LEXER_FILE
#include<stdio.h>
#include <stdbool.h>
#include "lexDef.h"
token *getNextToken();

void openfile(char * sourcefile);

FILE* getStream(FILE*);

void removeComments(char* testfile, char* cleanfile);

int getLineNumber();
#endif
