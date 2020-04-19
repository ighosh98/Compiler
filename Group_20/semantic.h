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
