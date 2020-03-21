/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef NARY_TREE
#define NARY_TREE
#include <stdio.h>
#include "lexerDef.h"
#include "astdef.h"
typedef struct treenode{
    token* lexeme;
    int tok;
    struct treenode** children;
    int n;
    astnode * node;
} treenode;

typedef struct Nary_tree{
    treenode* root;
} Nary_tree;


Nary_tree* get_nary_tree();

treenode* make_treenode(int a, token* lex);

void insert_children(treenode* node, tokentype* children,int n,token* lex);

void inorder(Nary_tree root,FILE* fptr);

#endif
