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
#include "astdef.h"

astnode* make_astnode(treenode* parsenode)
{
    if(parsenode == NULL)return NULL;

    astnode* temp = (astnode*)malloc(sizeof(astnode));
    temp->lexeme = parsenode->lexeme;
    temp->tok = parsenode->tok;
    temp->children = NULL;
    temp->node = NULL;
    temp->n = 0;
    temp->type = NONE;  //datatype assigned during type checking phase
    temp->attr = NO_ACTION; 
    return temp;
}

void printAST(astnode* root)
{
    if(root==NULL)return;

    printf("%s --> ",symbol_map[root->tok]);
    for(int i=0;i<root->n;i++)
	printf("%s  ", symbol_map[root->children[i]->tok]);
    printf("\n\n");
    for(int i=0;i<root->n;i++)
	printAST(root->children[i]);


    
}

astnode* makeAST_helper(treenode* root)
{
    if(root==NULL)return NULL;
    if(root->children)
    {   
	switch(root->tok)
	{
	    case PROGRAM: 
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(4*sizeof(astnode*));
		    temp->n = 4;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    temp->children[2] = makeAST_helper(root->children[2]);
		    temp->children[3] = makeAST_helper(root->children[3]);
		    return temp; 
		}break;
	    case MODULEDECLARATIONS:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = NULL;
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(2*sizeof(astnode*));
			temp->n = 2;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[1]);
			return temp;
		    }
		}break;
	    case OTHERMODULES:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = NULL;
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(2*sizeof(astnode*));
			temp->n = 2;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[1]);
			return temp;
		    }
		}break;
	    case MODULE:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(4*sizeof(astnode*));
		    temp->n = 4;

		    temp->children[0] = makeAST_helper(root->children[2]);
		    temp->children[1] = makeAST_helper(root->children[7]);
		    temp->children[2] = makeAST_helper(root->children[10]);
		    temp->children[3] = makeAST_helper(root->children[11]);
		    return temp;
		}break;
	     case DRIVERMODULE:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[4]);
		    return temp;
		}break;

	     case RET:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = NULL;
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[2]);
			return temp;
		    }
		}break;
   





	}
    }
    else
    {
	return make_astnode(root);
    }    
}

AST makeAST(treenode* root)
{
    AST temp;
    temp.root = makeAST_helper(root);
    return temp;
}

