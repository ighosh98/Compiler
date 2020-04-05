#ifndef SEMANTICS
#define SEMANTICS

#include <stdio.h>
#include "astdef.h"
#include "ast.h"
#include "lexerDef.h"
extern symbolTable* function_table;
void type_semantics(astnode* root, symbolTable* curr_table);
void check_semantics(astnode* root);
#endif
