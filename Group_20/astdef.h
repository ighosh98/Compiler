#ifndef AST_DEF
#define AST_DEF

#include <stdio.h>
#include<stdlib.h>
#include"lexerDef.h"
#include "color.h"
typedef enum {integer, real, boolean, function,array, NONE} datatype;
typedef enum {for_loop, while_loop, switch_stmt, add, subtract, multiply, divide, NO_ACTION} action;
typedef struct astnode{
    token* lexeme;
    datatype type;
    int tok;
    struct astnode** children;
    int n;
} astnode;

typedef struct AST{
    struct astnode* root;
} AST;

#endif
