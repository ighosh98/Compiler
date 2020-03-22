#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"


//SYMBOL TABLE is needed for proper symantic analysis
//
//type of ID is difficult. as it depends on the context. 
//it might be a function, variable, var or array declaration, array index.
//


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
		   //ayush
		   return;
		   //adityan
		   //indraneel 
		}break;
	    case MODULEDECLARATIONS:
		{
		    //ayush
		    return;
		    //adityan
		    //indraneel 
		}break;
	    case MODULEDECLARATION:
		{
		    //ayush
		    return;
		   //adityan
		   //indraneel 

		}break;
	    case OTHERMODULES:
		{
		    //ayush
		    return;
		   //adityan
		   //indraneel 
		}break;
	    case MODULE1:
		{
		    //ayush
		    return;
		   //adityan
		   //indraneel 
		}break;
	    case DRIVERMODULE:
		{
		    //ayush
		    return;
		   //adityan
		   //indraneel 
		}break;

	    case RET:
		{
		    //ayush
		    return;
		   //adityan
		   //indraneel 
		}break;

	    case INPUT_PLIST:
		{
		    //ayush
		    root->children[0]->type = root->children[1]->type;

		   //adityan
		   //indraneel 
		}break;

	    case N1:
		{//ayush
		    root->children[0]->type = root->children[1]->type;
		   //adityan
		   //indraneel 
		}break;
	    case OUTPUT_PLIST:
		{//ayush
		    root->children[0]->type = root->children[1]->type;
		   //adityan
		   //indraneel 
		}break;
	    case N2:
		{//ayush
		    root->children[0]->type = root->children[1]->type;
		   //adityan
		   //indraneel 
		}break;
	    case DATATYPE:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case RANGE_ARRAYS:
		{//ayush
		    root->isarr = true;
		    root->range_low = root->children[0].lexeme;   //convert to int
		    root->range_high = root->children[1].lexeme; //convert to int

		    //how to handle var in array range??
//###############################################################################################
		   //adityan
		   //indraneel 
		}break;
	    case TYPE:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case MODULEDEF:
		{//ayush
		    return;
		   //adityan
		   //indraneel 
		}break;
	    case STATEMENTS:
		{//ayush
		    return;
		   //adityan
		   //indraneel 
		}break;
	    case STATEMENT:
		{//ayush
		    return;
		   //adityan
		   //indraneel 
		}break;
	    case IOSTMT:
		{//ayush
		    //first child stores while/for info. hence nothing needs to be done.
		    return;
		   //adityan
		   //indraneel 
		}break;
	    case BOOLCONSTT:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 	

		}break;
	    case VAR:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case VAR_ID_NUM:
		{//ayush
		    root->type = root->children[0].type;

		    //if var_id_num -> id whichstmt. then type of ID can only be seen from symbol table.

		   //adityan
		   //indraneel 
		}break;
	    case WHICHID:
		{//ayush
		    root->type = root->children[0]->type;
		    if(root->type!=NONE && root->type!=integer)
			ERROR;
		   //adityan
		   //indraneel 
		}break;
	    case SIMPLESTMT:
		{//ayush
		    return;
		   //adityan
		   //indraneel 
		}break;
	    case ASSIGNMENTSTMT:
		{//ayush
		    if(root->children[0]->type!=root->children[1]->type)
			ERROR;
		   //adityan
		   //indraneel 
		}break;
	    case WHICHSTMT:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case LVALUEIDSTMT:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case LVALUEARRSTMT:
		{//ayush
		    if(root->children[0].type!=integer)ERROR;
		    root->type = root->children[1]->type;
		   //adityan
		   //indraneel 
		}break;
	    case INDEX:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case MODULEREUSESTMT:
		{//ayush
		    root->children[1]->type = NONE; //this idlist has no type

		   //adityan
		   //indraneel 
		}break;
	    case OPTIONAL:
		{//ayush
		    root->children[0].type = NONE;  //idlist has no type
		   //adityan
		   //indraneel 
		}break;
	    case IDLIST:
		{//ayush
		    if(root->type == NONE)
			root->children[1]->type = NONE;
		    else
			root->children[0]->type = root->type;
		   //adityan
		   //indraneel 
		}break;
	    case N3:
		{//ayush
		    if(root->children[0]->tok == EPS)
		    {
			root->type = root->children[0]->type;
		    }
		    else
		    {
			if(root->type == NONE)
			    root->children[1]->type = NONE;
			else
			    root->children[0]->type = root->type;
		    }
		   //adityan
		   //indraneel 

		}break;
	    case EXPRESSION:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case U:
		{//ayush
		    root->type = root->children[1]->type;

		   //adityan
		   //indraneel 
		}break;
	    case NEW_NT:
		{//ayush
		    root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case UNARY_OP:
		{//ayush
		    return;
		   //adityan
		   //indraneel 

		}break;
	    case ARITHMETICORBOOLEANEXPR:
		{//ayush
		    if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
			ERROR;
		    else
			root->type = root->children[0]->type;
		   //adityan
		   //indraneel 
		}break;
	    case N7:
		{//ayush
		    if(root->children[0]->tok==EPS)
			root->type = root->children[0]->type;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			    ERROR;
			else
			    root->type = root->children[1]->type;
		    }
		    //adityan
		   //indraneel 
		}break;
	    case ANYTERM:
		{//ayush
		    
		   //adityan
		   //indraneel 
		}break;
	    case N8:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case ARITHMETICEXPR:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case N4:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case TERM:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case N5:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case FACTOR:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case OP1:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case OP2:
		{//ayush
		   //adityan
		   //indraneel 

		}break;
	    case LOGICALOP:
		{//ayush
		   //adityan
		   //indraneel 

		}break;
	    case RELATIONALOP:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case DECLARSTMT:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case CONDITIONALSTMT:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case CASESTMTS:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case N9:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case VALUE:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case DEFAULT1:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case ITERATIVESTMT:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	    case RANGE:
		{//ayush
		   //adityan
		   //indraneel 
		}break;
	}
    }
    else
    {
	return root->node = make_astnode(root);
    }
}
    
