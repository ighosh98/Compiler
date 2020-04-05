#ifndef CODEGEN_H
#define CODEGEN_H
#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"
#include <string.h>
#include "symboltable.h"
#include "semantic.h"
#include "color.h"

int codegen(astnode*, symbolTable*, int);

#endif
