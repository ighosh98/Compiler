#ifndef AST_H
#define AST_H
#include <stdio.h>
#include "nary_tree.h"
#include<stdlib.h>
#include"lexerDef.h"
#include "color.h"
#include "astdef.h"

astnode* make_astnode(treenode* parsenode);
void printAST(astnode* root);
astnode* makeAST_helper(treenode* root);
AST makeAST(treenode* root);


#endif
