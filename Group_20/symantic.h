#ifndef SEMANTICS
#define SEMANTICS

#include <stdio.h>
#include "astdef.h"
#include "ast.h"
#include "lexerDef.h"

void type_symantics(astnode* root, symbolTable* current_table);

#endif
