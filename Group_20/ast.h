/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
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
