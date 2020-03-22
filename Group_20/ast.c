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
#include "ast.h"
astnode* make_astnode(treenode* parsenode)
{
    if(parsenode == NULL)return NULL;

    astnode* temp = (astnode*)malloc(sizeof(astnode));
    temp->lexeme = parsenode->lexeme;
    temp->tok = parsenode->tok;
    temp->children = NULL;
    temp->n = 0;
    temp->type = NONE;  //datatype assigned during type checking phase
    return temp;
}

void printAST(astnode* root)
{
    if(root==NULL)return;

    if(root->children){
	printf("%s --> ",symbol_map[root->tok]);
	for(int i=0;i<root->n;i++)
	    if(root->children[i])
		printf("%s  ", symbol_map[root->children[i]->tok]);
	printf("\n\n");
	for(int i=0;i<root->n;i++)
	    printAST(root->children[i]);
    }
    else
    {
	printf("%s\n\n",symbol_map[root->tok]);

    }
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
			return root->node = makeAST_helper(root->children[0]);
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
	    case MODULEDECLARATION:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[2]);
		    return temp;

		}break;
	    case OTHERMODULES:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
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
	    case MODULE1:
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
			return root->node = makeAST_helper(root->children[0]);
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

	    case INPUT_PLIST:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(3*sizeof(astnode*));
		    temp->n = 3;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[2]);
		    temp->children[2] = makeAST_helper(root->children[3]);
		    return temp;

		}

	    case N1:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3;

			temp->children[0] = makeAST_helper(root->children[1]);
			temp->children[1] = makeAST_helper(root->children[3]);
			temp->children[2] = makeAST_helper(root->children[4]);
			return temp;
		    }
		}
	    case OUTPUT_PLIST:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(3*sizeof(astnode*));
		    temp->n = 3;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[2]);
		    temp->children[2] = makeAST_helper(root->children[3]);
		    return temp;
		}
	    case N2:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3;

			temp->children[0] = makeAST_helper(root->children[1]);
			temp->children[1] = makeAST_helper(root->children[3]);
			temp->children[2] = makeAST_helper(root->children[4]);
			return temp;
		    }
		}
	    case DATATYPE:
		{
		    if(root->children[0]->tok==INTEGER || root->children[0]->tok==REAL
			|| root->children[0]->tok==BOOLEAN)
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[0]);
			return temp;

			
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(2*sizeof(astnode*));
			temp->n = 2;

			temp->children[0] = makeAST_helper(root->children[2]);
			temp->children[1] = makeAST_helper(root->children[5]);
			return temp;
		    }
		}
	    case RANGE_ARRAYS:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[2]);
		    return temp;
		}
	    case TYPE:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;

		}
	    case MODULEDEF:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[1]);
		    return temp;
		}
	    case STATEMENTS:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
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
		}
	    case STATEMENT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;
		}
	    case IOSTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1+1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[2]);
		    return temp;
		}
	    case BOOLCONSTT:
		{	
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;

		}
	    case VAR:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;
		}
	    case VAR_ID_NUM:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;
		}
	    case WHICHID:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[1]);
			return temp;
		    }
		}
	    case SIMPLESTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;
		}
	    case ASSIGNMENTSTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    return temp;
		}
	    case WHICHSTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;
		}
	    case LVALUEIDSTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    return temp;
		}
	    case LVALUEARRSTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(3*sizeof(astnode*));
		    temp->n = 3;

		    temp->children[0] = makeAST_helper(root->children[1]);
		    temp->children[1] = makeAST_helper(root->children[3]);
		    temp->children[2] = makeAST_helper(root->children[4]);
		    return temp;
		}
	    case INDEX:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;


		}break;
	    case MODULEREUSESTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(3*sizeof(astnode*));
		    temp->n = 3;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[3]);
		    temp->children[2] = makeAST_helper(root->children[6]);
		    return temp;
		}break;
	    case OPTIONAL:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[1]);
			return temp;
		    }
		}break;
	    case IDLIST:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    return temp;
		}break;
	    case N3:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(2*sizeof(astnode*));
			temp->n = 2;

			temp->children[0] = makeAST_helper(root->children[1]);
			temp->children[1] = makeAST_helper(root->children[2]);
			return temp;
		    }
		}break;
	    case EXPRESSION:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;
		}break;
	    case U:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    return temp;
		}break;
	    case NEW_NT:
		{
		    if(root->children[0]->tok==BO)
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[1]);
			return temp;
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[0]);
			return temp;
		    }
		}break;
	    case UNARY_OP:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;


		}break;
	    case ARITHMETICORBOOLEANEXPR:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    return temp;
		}break;
	    case N7:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[1]);
			temp->children[2] = makeAST_helper(root->children[2]);
			return temp;
		    }
		}break;
	    case ANYTERM:
		{
		    if(root->children[0]->tok==BOOLCONSTT)
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[0]);
			return temp;
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
	    case N8:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[1]);
			temp->children[2] = makeAST_helper(root->children[2]);
			return temp;
		    }
		}break;
	    case ARITHMETICEXPR:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    return temp;
		}break;
	    case N4:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[1]);
			temp->children[2] = makeAST_helper(root->children[2]);
			return temp;
		    }
		}break;
	    case TERM:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[1]);
		    return temp;
		}break;
	    case N5:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[1]);
			temp->children[2] = makeAST_helper(root->children[2]);
			return temp;
		    }
		}break;
	    case FACTOR:
		{
		    if(root->children[0]->tok==BO)
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[1]);
			return temp;
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(1*sizeof(astnode*));
			temp->n = 1;

			temp->children[0] = makeAST_helper(root->children[0]);
			return temp;
		    }
		}break;
	    case OP1:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;

		}break;
	    case OP2:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;

		}break;
	    case LOGICALOP:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;

		}break;
	    case RELATIONALOP:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(1*sizeof(astnode*));
		    temp->n = 1;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    return temp;
		}break;
	    case DECLARSTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[1]);
		    temp->children[1] = makeAST_helper(root->children[3]);
		    return temp;
		}break;
	    case CONDITIONALSTMT:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(3*sizeof(astnode*));
		    temp->n = 3;

		    temp->children[0] = makeAST_helper(root->children[2]);
		    temp->children[1] = makeAST_helper(root->children[5]);
		    temp->children[2] = makeAST_helper(root->children[6]);
		    return temp;
		}break;
	    case CASESTMTS:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(3*sizeof(astnode*));
		    temp->n = 3;

		    temp->children[0] = makeAST_helper(root->children[1]);
		    temp->children[1] = makeAST_helper(root->children[3]);
		    temp->children[2] = makeAST_helper(root->children[6]);
		    return temp;
		}break;
	    case N9:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3;

			temp->children[0] = makeAST_helper(root->children[1]);
			temp->children[1] = makeAST_helper(root->children[3]);
			temp->children[2] = makeAST_helper(root->children[6]);
			return temp;
		    }
		}break;
	    case VALUE:
		{
		    return root->node = makeAST_helper(root->children[0]);
		    
		}break;
	    case DEFAULT1:
		{
		    if(root->children[0]->tok==EPS)
		    {
			return root->node = makeAST_helper(root->children[0]);
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
	    case ITERATIVESTMT:
		{
		    if(root->children[0]->tok==FOR)
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(3*sizeof(astnode*));
			temp->n = 3+1;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[2]);
			temp->children[2] = makeAST_helper(root->children[4]);
			temp->children[3] = makeAST_helper(root->children[7]);
			return temp;
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(2*sizeof(astnode*));
			temp->n = 2+1;

			temp->children[0] = makeAST_helper(root->children[0]);
			temp->children[1] = makeAST_helper(root->children[2]);
			temp->children[2] = makeAST_helper(root->children[5]);
			return temp;
		    }
		}break;
	    case RANGE:
		{
		    astnode* temp = root->node = make_astnode(root);
		    temp->children = (astnode**)malloc(2*sizeof(astnode*));
		    temp->n = 2;

		    temp->children[0] = makeAST_helper(root->children[0]);
		    temp->children[1] = makeAST_helper(root->children[2]);
		    return temp;
		}


	}
    }
    else
    {
	return root->node = make_astnode(root);
    }
}

AST makeAST(treenode* root)
{
    AST temp;
    temp.root = makeAST_helper(root);
    return temp;
}

