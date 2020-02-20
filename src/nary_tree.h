#ifndef NARY_TREE
#define NARY_TREE
#include <stdio.h>
#include "lexDef.h"
typedef struct treenode{
    int tok;
    struct treenode** children;
    int n; 
} treenode;

typedef struct Nary_tree{
    treenode* root;  
} Nary_tree;


Nary_tree* get_nary_tree(int start);

treenode* make_treenode(int a);

void insert_children(treenode* node, type* children,int n);

void dfs(Nary_tree root);

#endif
