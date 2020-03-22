#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"

void type_symantics(astnode* root, astnode* parent)
{
    if(root==NULL)return;
    
    //postorder traversal of the tree
    //apply rules on the children 
    for(int i =0;i<root->n;i++)
	type_symantics(root->children[i], root);


    //after children are done then apply rule
    //on the current node
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
		}break;

	    case N1:
		{
		}break;
	    case OUTPUT_PLIST:
		{
		}break;
	    case N2:
		{
		}break;
	    case DATATYPE:
		{
		}break;
	    case RANGE_ARRAYS:
		{
		}break;
	    case TYPE:
		{
		}break;
	    case MODULEDEF:
		{
		}break;
	    case STATEMENTS:
		{
		}break;
	    case STATEMENT:
		{
		}break;
	    case IOSTMT:
		{
		}break;
	    case BOOLCONSTT:
		{	

		}break;
	    case VAR:
		{
		}break;
	    case VAR_ID_NUM:
		{
		}break;
	    case WHICHID:
		{
		}break;
	    case SIMPLESTMT:
		{
		}break;
	    case ASSIGNMENTSTMT:
		{
		}break;
	    case WHICHSTMT:
		{
		}break;
	    case LVALUEIDSTMT:
		{
		}break;
	    case LVALUEARRSTMT:
		{
		}break;
	    case INDEX:
		{
		}break;
	    case MODULEREUSESTMT:
		{
		}break;
	    case OPTIONAL:
		{
		}break;
	    case IDLIST:
		{
		}break;
	    case N3:
		{
		}break;
	    case EXPRESSION:
		{
		}break;
	    case U:
		{
		}break;
	    case NEW_NT:
		{
		}break;
	    case UNARY_OP:
		{

		}break;
	    case ARITHMETICORBOOLEANEXPR:
		{
		}break;
	    case N7:
		{
		}break;
	    case ANYTERM:
		{
		}break;
	    case N8:
		{
		}break;
	    case ARITHMETICEXPR:
		{
		}break;
	    case N4:
		{
		}break;
	    case TERM:
		{
		}break;
	    case N5:
		{
		}break;
	    case FACTOR:
		{
		}break;
	    case OP1:
		{
		}break;
	    case OP2:
		{

		}break;
	    case LOGICALOP:
		{

		}break;
	    case RELATIONALOP:
		{
		}break;
	    case DECLARSTMT:
		{
		}break;
	    case CONDITIONALSTMT:
		{
		}break;
	    case CASESTMTS:
		{
		}break;
	    case N9:
		{
		}break;
	    case VALUE:
		{
		}break;
	    case DEFAULT1:
		{
		}break;
	    case ITERATIVESTMT:
		{
		}break;
	    case RANGE:
		{
		}break;
	}
    }
    else
    {
	return root->node = make_astnode(root);
    }
}
    
