/*
   Group 20
   Ayush Vachaspati 2016B3A70398P
   Indraneel Ghosh  2016B1A70938P
   G Adityan	 2016B1A70929P
*/

#ifndef SEMANTICS
#define SEMANTICS

#include <stdio.h>
#include "astdef.h"
#include "ast.h"
#include "lexerDef.h"
extern symbolTable* function_table;
extern int pass_no;
extern bool semantic_error;
void type_semantics(astnode* root, symbolTable* curr_table);
symbolTable* check_semantics(astnode* root);
#endif
