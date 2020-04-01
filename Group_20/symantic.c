#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"
#include "symboltable.h"

symbolTable* function_table;

void type_symantics(astnode* root, symbolTable* current_table)
{
    printf("%s\n",symbol_map[root->tok]);
    if(root==NULL)return;

    if(root->children)
    {
	switch(root->tok)
	{   
	    case PROGRAM:
		{
		    
		    //initialize the function table and move forward    
		    function_table = getSymbolTable(100);
		    
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);
		    return;
		}break;

	    case MODULEDECLARATIONS:
		{

		    //if empty then return. else process the subtree
		    if(root->tok == EPS)
			return;
		    else
			for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    return;
		}break;

	    case MODULEDECLARATION:
		{
		    //add entry for the function in the function table. and mark IsCalled = False and isDefined = false
		    
		    symbol_table_node* temp = insertSymbolTable(function_table, 
			    root->children[0]->lexeme->str , false, false, NULL, NULL, 
			    -1,-1, root->children[0]->lexeme, function); //type = function
		    
		    temp -> isUsed = false;
		    temp -> isDefined = false;
		    
		    //assign function type to the ID node.
		    root->children[0]->type = function;
		    
		    return;
		}break;

	    case OTHERMODULES:
		{
		    if(root->tok == EPS)
			return;
		    else
			for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    return;
		}break;

	    case MODULE1:
		{
		    symbol_table_node* temp = searchSymbolTable(function_table, root->children[0]->lexeme->str);
		    if(temp!=NULL)
		    {
			//function has already been declared
			if(temp->isUsed==false)
			{} //Error: function not used after declaration
			else if(temp->isDefined==true)
			{}  //Error: Redefinition of the function
		    }
		    
//######################################################################################################
		    //add definition of the function. 
		    //add input_plist and output_plist if ret is not eps
//######################################################################################################
		    
		    root->children[0]->type = function;


		    temp = insertSymbolTable(function_table, root->children[0]->lexeme->str,
			    false, false, NULL, NULL, -1,-1, 
			    root->children[0]->lexeme, function);

		    
		    //add input_plist into temp
		    //
		    //add output_plist into temp
		    //

		    temp->isDefined = true;

  
		    //move forward
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);
		    
		    return;
		}break;
	    case DRIVERMODULE:
		{
		    //move forward.
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);

		    return;
		}break;

	    
	    case RET: case INPUT_PLIST: case N1: case OUTPUT_PLIST: case N2:
		{
		    //handled by moduledef.
		    return;    
		}break;


	    case DATATYPE:
		{
		    // handle by input_list and declare statement
		}break;
   
	    case RANGE_ARRAYS:
		{
		    //handle by datatype

		}break;

	    case TYPE:
		{
		    //handle by outputplist and datatype
		}break;
	    
	    case MODULEDEF:
		{
		    //make a new symboltable. assign the current table as the parent of the new table.
		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = current_table;
		    
		    //then pass the new table as the current_table in further calls.
		    //move forward
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], new_table);
	     
		    
		    //destroy current table
		    deleteSymbolTable(new_table);
		    
		    return;
		}break;


	    case STATEMENTS:
		{
		    if(root->tok == EPS)
			return;
		    else
			//move forward
			for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    return;
		}break;

	    case STATEMENT:
		{
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    return;
		}break;

	    case IOSTMT:
		{
		    //first child stores print/get info. hence nothing needs to be done.
		    //move forward to check variables.
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);
		    
		    return;
		}break;
	    case GET_VALUE: case PRINT:
		{
		    return;
		}break;


	    case BOOLCONSTT:
		{
		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);
		    root->type = root->children[0]->type;
		    return;
		}break;
	    case VAR:
		{
		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    return;
		}break;

	    case VAR_ID_NUM:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    //assign type
		    root->type = root->children[0]->type;
    
		    //if array statement then check bounds. WhichId is handled here.
		    if(root->children[0]->tok==ID)
		    {
			if(root->children[1]->tok==EPS)
			    return;
			else
			{
			    //check if index is within the range of the array
			    // temp = INDEX
			    astnode* temp = root->children[1]->children[0];
			    if(temp->children[0]->tok!=NUM) 
			    {}	//ERROR: index must be integer
//######################################################################################################
			    if(temp->children[0]->tok==NUM)
			    {}//static check
			    else
			    {}	//dynamic check
//######################################################################################################
			}
		    }
		    return;
		}break;
	    
		
	    case WHICHID:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);
		    
		    return;
		}break;

	    case SIMPLESTMT:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    return;
		}break;

	    case ASSIGNMENTSTMT:
		{
		    //call a function to handle whichstmt , lvalueidstmt, lvaluearrstmt. 
		    //pass the ID to this function so that it can verify that the index 
		    //is within range in case of array.
		    //also check that the type of lhs == type of rhs.
		}break;

/////////////////////// Handled in assignment stmt /////////////////////////////
/////////////////////////////////////////////////////////////////////////////////]

	/*
	    case WHICHSTMT:
		{
		    //move forward and assign type of child to whichstmt
		}

	    case LVALUEIDSTMT:
		{
		    //move forward and assign type of child to lvalueidstmt
		    root->type = root->children[0]->type;
		}break;

	    case LVALUEARRSTMT:
		{
		    //check type of index.
		    //also check if index is within range of the array.
		    if(root->children[0]->type!=integer){}//ERROR;
		    root->type = root->children[1]->type;
		}break;
	*/
	    case INDEX:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		}break;

	    case MODULEREUSESTMT:
		{
		    //make a separate function for this.
		    //verify idlist is same as function inputplist
		    //if option not eps then verify output plist also.
		    //
		    //idlist in this case has no type.
		    //DO NOT ALLOW RECURSIVE CALL
		    root->children[1]->type = NONE; //this idlist has no type
		}break;

////////////////// handled above/////////////////////		
/*
	    case OPTIONAL:
		{
		    root->children[0]->type = NONE;  //idlist has no type


		}break;
*/


///////////////// different uses of idlist are handled by their using functions////////
///////////////////////////////////////////////////////////////////////////////////////

    	/*	case IDLIST:
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
			return;
		    }
		    else
		    {
			if(root->type == NONE)
			    root->children[1]->type = NONE;
			else
			    root->children[0]->type = root->type;
		    }



		}break;
	  */
	    case EXPRESSION:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    return;
		}break;
	    case U:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    root->type = root->children[1]->type;  //type of NEW_NT
		    return;
		}break;
	    case NEW_NT:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;

		    return;
		}break;
	    case UNARY_OP:
		{
		    //do nothing
		    //this is instruction for code generator to 
		    //take know the sign 
		    return;
		}break;
	    
	    case ARITHMETICORBOOLEANEXPR:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    //then check 
		    if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
		    {}//ERROR: Type mismatch in expression
		    
		    if(root->children[1]->tok!=EPS)
		    {
			//check that expression has type integer or boolean for logical op
			if(root->children[0]->type!=integer || root->children[0]->type!=boolean)
			{}  //ERROR: logical op can only have integer of boolean type
		    }
		    
		    
		    root->type = root->children[0]->type;

		    return;
		}break;

	    case N7:
		{
		    //move forward 
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		   
		    //then check 
		    if(root->tok==EPS)
			return;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{}//ERROR: Type mismatch in expression
			
			root->type = root->children[1]->type;
		    }
		    return;

		}break;
	    case ANYTERM:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    //then check
		   if(root->children[0]->tok == BOOLCONSTT)
		      root->type = root->children[0]->type;
		   else
		   {
			if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
			{}//ERROR: type mismatch in expression
			
			if(root->children[1]->tok!=EPS)
			{
			    if(root->children[1]->type==boolean)
			    {}//ERROR: relational op cannot have boolean type
			}
			
			root->type = root->children[0]->type;
		   return;
		   }
		}break;
	    
	    case N8:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    if(root->tok == EPS)
			return;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{}//ERROR: type mismatch
			
			root->type = root->children[1]->type;
		    }

		    return;
		}break;

	    case ARITHMETICEXPR:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
		    {}//ERROR: type mismatch in expression
		    if(root->children[1]->tok!=EPS)
		    {
			if(root->children[1]->type == boolean)
			{}  //ERROR: arithmetic operation cannot have boolean type
		    }
		    
		    root->type = root->children[0]->type;
		    return;
		}break;

	    case N4:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    if(root->children[0]->tok == EPS)
			return;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{}  //ERROR: type mismatch in expression
			
			root->type = root->children[1]->type;
		    }
		    return;
		}break;

	    case TERM:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);
		    
		    if(root->children[1]->type!=NONE && root->children[0]->type!=root->children[1]->type)
		    {}//ERROR: type mismatch in expression
		    
		    if(root->children[1]->tok!=EPS)
		    {
			if(root->children[1]->type == boolean)
			{}//ERROR: arithmetic op cannot have boolean type

		    }
		    
		    root->type = root->children[0]->type;
		    return;
		}break;

	    case N5:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			type_symantics(root->children[i], current_table);
		    
		    if(root->tok == EPS)
			return;
		    else
		    {
			if(root->children[2]->type!=NONE && root->children[1]->type!=root->children[2]->type)
			{}// ERROR: type mismatch in expression
			
			root->type = root->children[1]->type;
		    }
		}break;
	    case FACTOR:
		{
		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    //assign type
		    root->type = root->children[0]->type;
		    
		    return;
		}break;
	    case OP1:
	    case OP2:
	    case LOGICALOP:
	    case RELATIONALOP:
		{
		    //do nothing
		    //this is for code generator
		    return;
		}break;
	    
	    case DECLARESTMT:
		{
		    //make a function for declare stmt. 
		    //pass the datatype inthe fucntion
		    //the function adds all the ids in to the table with 
		    //the correct type.
		    //
		    //the move forward.
		}break;

	    case CONDITIONALSTMT:
		{
		    //create a new scope(symbol table) and make the current table as the parent of the new table.
		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = current_table;

		    //move forward. type of ID is handled by the symbol table when it is called.
		    
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], new_table);
		    


		    //check if the type of the switch statemt matches.
		    if(root->children[0]->type==integer)
		    {
			if(root->children[0]->type != root->children[1]->type)
			{}  //ERROR: case type does not match argument type
			if(root->children[2]->children[0]->tok==EPS)
			{}  //ERROR: default case not included
			
			    
			//check all case statements are unique!!
		    }
		    else if(root->children[0]->type==boolean)
		    {
			if(root->children[0]->type != root->children[1]->type)
			{}//ERROR: case type does not match argument type
			if(root->children[2]->children[0]->tok!=EPS)
			{}  //ERROR: boolean case cannot have default case.
		    
			//check that TRUE and FALSE are included and all cases are unique
		    }
		    else
			//ERROR: switch can only have integer of boolean argument

		    return;
		}break;

	    case CASESTMTS:
		{
		    //move forward 
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    //keep checking somehow that all the cases are unique (maybe use symbol table somehow
		    //							    by inserting the value as a string)
		    
		    if(root->children[2]->type!=NONE && root->children[0]->type!=root->children[2]->type)
		    {}//ERROR: Case type mismatch. 
		    else
			root->type = root->children[0]->type;

		    return;
		}break;

	    case N9:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    //keep checking somehow that all the cases are unique
			if(root->tok == EPS)
			    return;
			else
			{
			    if(root->children[2]->type!=NONE && root->children[0]->type!=root->children[2]->type)
			    {}//ERROR: Case type mismatch.
			    
			    root->type = root->children[0]->type;   
			}
		    return;
		}break;

	    case VALUE:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    return;
		}break;
	    case DEFAULT1:
		{
		    //move forward.
		    //nothing else needs to be done
		    for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    return;
		}break;
	    case ITERATIVESTMT:
		{
		    /////Temporary///////
		     for(int i =0;i<root->n;i++)
			    type_symantics(root->children[i], current_table);
		    
		    if(FOR)
		    {}
		    //create new scope(symbol table) and insert the iterator variable.
		    //assign the current table as the parent of the new table. then use the new table for further fuction calls.
		    //if range is variable. then type should be integer
		    //if range is constant then range2 <= range1
		    //then move forward.
		    else if(WHILE)
		    {}
		    //create new scope(symbol table) and assign the current table as its parent
		    //then move forward
		    return;
		}break;
	    case RANGE:
		{
		    //handled by FOR loop. Iterative stmt.
		}break;
	    default: return;
	}
    }
    else
    {
	switch(root->tok)
	{
	    case INTEGER:
	    case NUM:
		root->type = integer;
		break;

	    case RNUM:
	    case REAL:
		root->type = real;
		break;

	    case FALSE1:
	    case TRUE1:
	    case BOOLEAN:
		root->type = boolean;
		break;

	    case ID:
		{
		    //check symbol table to find the type of the id.
		    //if symbol table has no entry then error.
		    
		    symbol_table_node* temp = searchSymbolTable(current_table,root->lexeme->str);
		    if(temp==NULL)
		    {}//ERROR: symbol not recognized.

		    else
			root->type = temp->type;

		}break;
	    case EPS:
		root->type = NONE;
		break;
	}
	    

    }
}


