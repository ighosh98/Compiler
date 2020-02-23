/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef LEXER_FILE
#define LEXER_FILE
#include<stdio.h>
#include <stdbool.h>
#include "lexerDef.h"
token *getNextToken();

void openfile(char * sourcefile);

FILE* getStream(FILE*);

void removeComments(char* sourcefile);

int getLineNumber();
#endif
