#ifndef AST_DEF
#define AST_DEF

#include <stdio.h>
#include<stdlib.h>
#include"lexerDef.h"
#include "color.h"
typedef enum {integer, real, boolean, function,array, NONE} datatype;
extern char* datatype_map[];
typedef struct astnode{
    token* lexeme;
    datatype type;
    int tok;
    struct astnode** children;
    int n;
    int casehandle;
} astnode;

typedef struct AST{
    struct astnode* root;
} AST;

#endif
