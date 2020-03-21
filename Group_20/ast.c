/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#include <stdio.h>
#include "nary_tree.h"
#include<stdlib.h>
#include"lexerDef.h"
#include "color.h"
#include "ast_def.h"

ast_node* make_astnode(treenode* parsenode, datatype dt)
{
    if(parsenode == NULL)return NULL;

    ast_node* temp = (ast_node*)malloc(sizeof(ast_node));
    temp->lexeme = parsenode->lex;
    temp->tok = parsenode->a;
    temp->children = NULL;
    temp->node = NULL;
    temp->n = 0;
    temp->
    return temp;
}

/*void insert_children(node* node, type* children, int n, token* lex)
{
    treenode** temp = (treenode**)malloc(sizeof(treenode*)*n);
    for(int i=0;i<n;i++)
    {
	treenode* x = make_treenode(children[i],lex);
	temp[i] = x;
    }
    node->children = temp;
    node->n = n;
}
*/

AST* makeAST(treenode* root)
{
    if(root==NULL)return NULL;

    if(root->children)
    {
	//make ast for each of the children nodes
	for(int i=0;i<root->n;i++)
	    makeAST(root->children[i]);
    }
    else
    {
	//leaf node of ast
	return make_astnode(`)
    }
		    
    //make ast for the root node
    
    switch(root->tag)
    {
	case: 
    }

    return root->node;
    
}

