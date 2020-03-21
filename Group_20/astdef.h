#ifndef AST_DEF
#define AST_DEF

#include <stdio.h>

typedef enum {integer, real, boolean, NONE} datatype;

typedef struct ast_node{
    token* lexeme;
    int tok;
    struct ast_node** children;
    int n;
    struct ast_node * node;
} ast_node;

typedef struct AST{
    ast_node* root;
} AST;


#endif
