#ifndef NARY_TREE
#define NARY_TREE
#include <stdio.h>
#include "lexDef.h"
typedef struct treenode{
    token* lexeme;
    int tok;
    struct treenode** children;
    int n; 
} treenode;

typedef struct Nary_tree{
    treenode* root;  
} Nary_tree;


Nary_tree* get_nary_tree();

treenode* make_treenode(int a, token* lex);

void insert_children(treenode* node, type* children,int n,token* lex);

void inorder(Nary_tree root,FILE* fptr);

#endif
