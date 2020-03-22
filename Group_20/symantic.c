#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"

void type_symantics(AST root)
{
    if(root==NULL)return;
    
    if(root->children)
    {
	switch(root->tok)
	{
	    case PROGRAM:
		{
		}break;
	    case MODULEDECLARATIONS:
		{
		}break;
	    case MODULEDECLARATION:
		{

		}break;
	    case OTHERMODULES:
		{
		}break;
	    case MODULE1:
		{
		}break;
	    case DRIVERMODULE:
		{
		}break;

	    case RET:
		{
		}break;

	    case INPUT_PLIST:
		{
		}

	    case N1:
		{
		}
	    case OUTPUT_PLIST:
		{
		}
	    case N2:
		{
		}
	    case DATATYPE:
		{
		}
	    case RANGE_ARRAYS:
		{
		}
	    case TYPE:
		{
		}
	    case MODULEDEF:
		{
		}
	    case STATEMENTS:
		{
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
			return root->node = NULL;
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
			return root->node = NULL;
		    }
		    else
		    {
			astnode* temp = root->node = make_astnode(root);
			temp->children = (astnode**)malloc(2*sizeof(astnode*));
			temp->n = 2;

			temp->children[0] = makeAST_helper(root->children[1]);
			temp->children[1] = makeAST_helper(root->children[3]);
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
			return root->node = NULL;
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
			return root->node = NULL;
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
			return root->node = NULL;
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
			return root->node = NULL;
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
			return root->node = NULL;
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
			return root->node = NULL;
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
		    if(root->children[0]->tok==NUM)
		    {
			return root->node = make_astnode(root->children[0]);
			//return root->node = NULL;
		    }
		    else if(root->children[0]->tok==TRUE1)
		    {
			return root->node = make_astnode(root->children[0]);
			//return root->node = NULL;
		    }
		    else
		    {
			return root->node = make_astnode(root->children[0]);
			//return root->node = NULL;
		    }
		}break;
	    case DEFAULT1:
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
    
