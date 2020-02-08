/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan		 2016B1A70929P
*/
/*
Function prototype for Lexer 
*/
#ifndef _lexer_
#define _lexer_

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "lexerDef.h"
extern node* create_node();
extern header set_header();
extern void flush(char* str);
extern char* keyword_check(char val[]);//check the list of keywords
extern int remove_lines(char* in,int x);//remove lines
extern int remove_comments(char* in,int x);// remove comments 
extern FILE* reloadBuffer(char* file);//file ptr must be handled efficiently
extern void remove(char* file);
extern node* getToken();

#endif
