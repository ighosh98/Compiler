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
	    case MODULEDECLARATIONS:
	    case MODULEDECLARATION:
	    case OTHERMODULES:
	    case MODULE1:
	    case DRIVERMODULE:
	    case RET:
		{
		    return;
		}break;

	    case INPUT_PLIST:
	    case N1:
	    case OUTPUT_PLIST:
	    case N2:
		{
		    root->children[0]->type = root->children[1]->type;
		}break;
	    
	    case DATATYPE:
		{
		    root->type = root->children[0]->type;
		}break;
	    case RANGE_ARRAYS:
		{
		    //root->isarr = true;
		    //root->range_low = root->children[0]->lexeme;   //convert to int
		    //root->range_high = root->children[1]->lexeme; //convert to int

		    //how to handle var in array range??
//###############################################################################################
		   
		    
		}break;
	    case TYPE:
		{
		    root->type = root->children[0]->type;
		}break;
	    case MODULEDEF:
	    case STATEMENTS:
	    case STATEMENT:
		{
		    return;
		}break;
	    
	    case IOSTMT:
		{
		    //first child stores while/for info. hence nothing needs to be done.
		    return;
		}break;
	    case BOOLCONSTT:
	    case VAR:
		{
		    root->type = root->children[0]->type;
		}break;
	   
	    case VAR_ID_NUM:
		{
		    root->type = root->children[0]->type;

		    //if var_id_num -> id whichstmt. then type of ID can only be seen from symbol table.

		   
		    
		}break;
	    case WHICHID:
		{
		    root->type = root->children[0]->type;
		    if(root->type!=NONE && root->type!=integer)
		    {}
			//ERROR;
		   
		    
		}break;
	    case SIMPLESTMT:
		{
		    return;
		}break;
	    case ASSIGNMENTSTMT:
		{
		    if(root->children[0]->type!=root->children[1]->type)
		    {}
			///ERROR;
		   
		    
		}break;
	    case WHICHSTMT:
	    case LVALUEIDSTMT:
		{
		    root->type = root->children[0]->type;
		}break;
	    
	    case LVALUEARRSTMT:
		{
		    if(root->children[0]->type!=integer){}//ERROR;
		    root->type = root->children[1]->type;
		}break;
	    
	    case INDEX:
		{
		    root->type = root->children[0]->type;	   
		}break;

	    case MODULEREUSESTMT:
		{
		    root->children[1]->type = NONE; //this idlist has no type

		   
		    
		}break;
	    case OPTIONAL:
		{
		    root->children[0]->type = NONE;  //idlist has no type
		   
		    
		}break;
	    case IDLIST:
		{
		    if(root->type == NONE)
			root->children[1]->type = NONE;
		    else
			root->children[0]->type = root->type;
		   
		    
		}break;
	    case N3:
		{
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
		   
		    

		}break;
	    case EXPRESSION:
		{
		    root->type = root->children[0]->type;
		   
		    
		}break;
	    case U:
		{
		    root->type = root->children[1]->type;

		   
		    
		}break;
	    case NEW_NT:
		{
		    root->type = root->children[0]->type;
		   
		    
		}break;
	    case UNARY_OP:
		{
		    return;
		   
		    

		}break;
	    case ARITHMETICORBOOLEANEXPR:
		{
		    if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
		    {}//	ERROR;
		    else
			root->type = root->children[0]->type;
		   
		    
		}break;
	    case N7:
		{
		    if(root->children[0]->tok==EPS)
			root->type = root->children[0]->type;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{}//    ERROR;
			else
			    root->type = root->children[1]->type;
		    }
		    
		    
		}break;
	    case ANYTERM:
		{
		   if(root->children[0]->tok == BOOLCONSTT)
		      root->type = root->children[0]->type;
		   else
		   {
		       if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
		       {} // ERROR;
		       else
			   root->type = root->children[0]->type;
		   } 
		   
		    
		}break;
	    case N8:
		{
		    if(root->children[0]->tok == EPS)
			root->type = root->children[0]->type;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{} // ERROR;
			else
			    root->type = root->children[1]->type;
		    }
		   
		    
		}break;
	    case ARITHMETICEXPR:
		{
		    if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
		    {} //error
		    else
		    {
			root->type = root->children[0]->type;
		    }
		   
		    
		}break;
	    case N4:
		{
		    if(root->children[0]->tok == EPS)
			root->type = root->children[0]->type;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{} // ERROR;
			else
			    root->type = root->children[1]->type;
		    }

		   
		    
		}break;
	    case TERM:
		{
		    if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
		    {} //error
		    else
		    {
			root->type = root->children[0]->type;
		    }
		}break;
	    case N5:
		{
		    if(root->children[0]->tok == EPS)
			root->type = root->children[0]->type;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{} // ERROR;
			else
			    root->type = root->children[1]->type;
		    }
		}break;
	    case FACTOR:
		{
		    root->type = root->children[0]->type;
		}break;
	    case OP1:
	    case OP2:
	    case LOGICALOP:
	    case RELATIONALOP:
		{
		    return;
		}break;
	    case DECLARSTMT:
		{
		    root->children[0]->type = root->children[1]->type;
		}break;
	    case CONDITIONALSTMT:
		{
		    if(root->children[0]->type!=integer || root->children[0]->type!=boolean
			    || root->children[0]->type!=root->children[1]->type)
		    {}//error;

		   
		    
		}break;
	    case CASESTMTS:
		{
		    if(root->children[2]->type!=NONE && root->children[0]->type!=root->children[2]->type)
		    {} //ERROR
		    else
			root->type = root->children[0]->type;
		   
		    
		}break;
	    case N9:
		{

			if(root->children[0]->tok == EPS)
			{
			    root->type = root->children[0]->type;
			}
			else
			{
			    if(root->children[2]->type!=NONE && root->children[0]->type!=root->children[2]->type)
			    {}//ERROR
			    else
			    {
				root->type = root->children[0]->type;
			    }
			}

		   
		    
		}break;
	    case VALUE:
		{
		    root->type = root->children[0]->type;
		}break;
	    case DEFAULT1:
	    case ITERATIVESTMT:
		{
		    return;
		}break;
	    case RANGE:
		{
		    if((root->children[0]->type!=integer && root->children[1]->type!=integer) || 
			(root->children[0]->type!=variable && root->children[1]->type!=variable))
		    {} //ERROR
		   
		    
		}break;
	}
    }
    else
    {
	if(root->tok == NUM)
	    root->type = integer;
    }
}
