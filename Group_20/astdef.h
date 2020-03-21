#ifndef AST_DEF
#define AST_DEF

#include <stdio.h>

typedef enum {integer, real, boolean, function, NONE} datatype;
typedef enum {for_loop, while_loop, switch_stmt, add, subtract, multiply, divide, NO_ACTION} action;
typedef struct astnode{
    token* lexeme;
    action attr;
    datatype type;
    int tok;
    struct astnode** children;
    int n;
    struct astnode * node;
} astnode;

typedef struct AST{
    astnode* root;
} AST;


#endif
