#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"
#include <string.h>
#include "symboltable.h"
#include "semantic.h"
#include "color.h"
#include "codegen.h"
//create a way to get the total size of the function in the function table.
//for Driver and for all other functions

//return the next available offset
int declareVariablesOffset(symbolTable* table, astnode* idlist, astnode* datatype, int curr_offset)
{
    astnode* dataTypeVar = datatype;
    while(idlist->tok!=EPS)
    {
	if(dataTypeVar->children[0]->tok==INTEGER)
	{
	    idlist->children[0]->type = integer;
	    symbol_table_node* temp = insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, integer); //integer type variable
    
	    temp->offset = curr_offset;
	    curr_offset+=4;
	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==REAL)
	{
	    idlist->children[0]->type = real;
	    symbol_table_node* temp = insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, real); //integer type variable
	    
	    temp->offset = curr_offset;
	    curr_offset+=8;
	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==BOOLEAN)
	{
	    idlist->children[0]->type = boolean;
	    symbol_table_node* temp = insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, boolean); //integer type variable

	    temp->offset = curr_offset;
	    curr_offset+=4;
	    idlist = idlist->children[1];
	}	
	else
	{
	    //input is of type array. insert variable of type array.
	    symbol_table_node* a = insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, NONE); //integer type variable

	    a->isarr = true; //the variable is array;
	    a->type = dataTypeVar->children[1]->type; //what is the type of the array
	    idlist->children[0]->type = dataTypeVar->children[1]->type;

	    //static or dynamic range of array??
	    //datatype->rangearrays->index->(NUM/ID)
	    if(dataTypeVar->children[0]->children[0]->children[0]->tok==NUM
		    && dataTypeVar->children[0]->children[1]->children[0]->tok==NUM)
	    {
		//static range of array
		a->isdynamic = false;
		a->crange1 = atoi(dataTypeVar->children[0]->children[0]->children[0]->lexeme->str); //first index
		a->crange2 = atoi(dataTypeVar->children[0]->children[1]->children[0]->lexeme->str); //second index
		
		//array size in memory
		if(a->type==integer || a->type == boolean)
		{
		    a->offset = curr_offset;
		    curr_offset +=  4*(a->crange2 - a->crange1 + 1);
		}
		else if(a->type==real)
		{
		    a->offset = curr_offset;
		    curr_offset += 8*(a->crange2 - a->crange1 + 1);
		}
		
	    }
	    else
	    {

		a->offset = -1;  //the memory is allocated on heap. -1 says that this offset will come from heap.
		curr_offset+=0;  //current offset does not change since array on heap not stack

		//dynamic range of array
		astnode* indexVar1 = dataTypeVar->children[0]->children[0]; //index nodes
		astnode* indexVar2 = dataTypeVar->children[0]->children[1];
		
		if(indexVar1->children[0]->tok==ID && indexVar2->children[0]->tok==ID)
		{
		    //11 -> ID ID
		    a->isdynamic = 3;
		    a->drange1 = searchSymbolTable(table,indexVar1->children[0]->lexeme->str);
		    a->drange2 = searchSymbolTable(table,indexVar2->children[0]->lexeme->str);
		}
		else if(indexVar1->children[0]->tok==ID && indexVar2->children[0]->tok==NUM)
		{
		    //10 -> ID NUM
		    a->isdynamic = 2;
		    a->drange1 = searchSymbolTable(table,indexVar1->children[0]->lexeme->str); 
		    a->crange2 = atoi(indexVar2->children[0]->lexeme->str);
		}
		else if(indexVar1->children[0]->tok==NUM && indexVar2->children[0]->tok==ID)
		{
		    //01 -> NUM ID
		    a->isdynamic = 1;
		    a->crange1 = atoi(indexVar1->children[0]->lexeme->str); 
		    a->drange2 = searchSymbolTable(table,indexVar2->children[0]->lexeme->str); 
		}
	    }

	    idlist = idlist->children[1];
	}
    }

    return curr_offset;
}

int codegen(astnode* root, symbolTable* current_table,int curr_offset)
{
    if(root==NULL)return 0;
    if(root->children)
    {
	switch(root->tok)
	{   
	    case PROGRAM:
		{
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,0); //the program starts with zero
		    
		    return curr_offset;
		}break;

	    case MODULEDECLARATIONS:
		{

		    //if empty then return. else process the subtree
		    if(root->tok == EPS)
			return curr_offset;
		    else
			for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;

	    case MODULEDECLARATION:
		{
		    return curr_offset;
		}break;

	    case OTHERMODULES:
		{
		    if(root->tok == EPS)
			return curr_offset;
		    else
			for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,0);
		    return curr_offset;
		}break;

	    case MODULE1:
		{
		    symbol_table_node* temp = searchSymbolTable(function_table, root->children[0]->lexeme->str);
		    
			///////// add the function definition/signature to the function table.///////////
			
		
			symbolTable* input_table = getSymbolTable(100); //input_list scope
			input_table->parent = NULL; //new function has no parent scope

			symbolTable* new_table = getSymbolTable(100); //table for the function scope.
			new_table->parent =  input_table;		  //function scope shadows input scope

		    //go onto moduledef with a new symbol table which has
		    //the entries for input_plist vars and out_plist vars

		    //insert input vars
		    symbol_table_node* a = temp->iplist;
		    while(a)
		    {
			symbol_table_node* input_var = insertSymbolTable(input_table, a->name,a->isarr,a->isdynamic,
				a->drange1,a->drange2,a->crange1,a->crange2,a->lexeme,a->type);
			
			input_var->offset = curr_offset;

			if(input_var->isarr == true)
			{
			    //if array input variable
			    if(input_var->type==integer || input_var->type==boolean)
				curr_offset += 4*(input_var->crange2 - input_var->crange1 + 1);
			    else if(input_var->type==real)
				curr_offset += 8*(input_var->crange2 - input_var->crange1 + 1 );
			}
			else
			{
			    //normal variable
			    if(input_var->type==integer || input_var->type==boolean)
				curr_offset+=4;
			    else if(input_var->type = real)
				curr_offset+=8;
			}

			a = a->iplist;
		    }

		    //insert output vars
		    a = temp->oplist;
		    while(a)
		    {
			symbol_table_node* output_var = insertSymbolTable(new_table, a->name,a->isarr,a->isdynamic,
				a->drange1,a->drange2,a->crange1,a->crange2,a->lexeme,a->type);
			
			output_var->offset = curr_offset;

			if(output_var->isarr == true)
			{
			    //if array input variable
			    if(output_var->type==integer || output_var->type==boolean)
				curr_offset += 4*(output_var->crange2 - output_var->crange1 + 1);
			    else if(output_var->type==real)
				curr_offset += 8*(output_var->crange2 - output_var->crange1 + 1 );
			}
			else
			{
			    //normal variable
			    if(output_var->type==integer || output_var->type==boolean)
				curr_offset+=4;
			    else if(output_var->type = real)
				curr_offset+=8;
			}
			a = a->oplist;
		    }
		    //call moduledef
		    curr_offset = codegen(root->children[3],new_table, curr_offset);
		    
		}break;

	    case DRIVERMODULE:
		{
		    //create symbol table for the program
		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = NULL;
		    
		    //call moduledef
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], new_table,0);
		    return curr_offset;
		}break;

	    
	    case RET: case INPUT_PLIST: case N1: case OUTPUT_PLIST: case N2:
		{
		    //handled by module1.
		    return curr_offset;    
		}break;


	    case DATATYPE:
		{
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);

		    if(root->children[0]->tok!=RANGE_ARRAYS)
			root->type = root->children[0]->type;
		    else
		    {
			root->type = root->children[1]->type;
		    }
		    // handle by input_list and declare statement
		    return curr_offset;
		}break;
   
	    case RANGE_ARRAYS:
		{
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM))
		    {
			//dynamic range check. arg1->ID arg2->NUM
		    }
		    if((root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {
			//dynamic range check. arg1->NUM arg2->ID
		    }
		    else if(root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==ID)
		    {
			//dynamic check of index1 < index2
		    }

		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case TYPE:
		{
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;
	    
	    case MODULEDEF:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;


	    case STATEMENTS:
		{
		    if(root->tok == EPS)
			return curr_offset;
		    else
			//move forward
			for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);

		    return curr_offset;
		}break;

	    case STATEMENT:
		{
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;

	    case IOSTMT:
		{
		    //first child stores print/get info. hence nothing needs to be done.
		    //move forward to check variables.
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset); 
		    return curr_offset;
		}break;
	    case GET_VALUE: case PRINT:
		{
		    return curr_offset;
		}break;


	    case BOOLCONSTT:
		{
		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;
	    case VAR:
		{
		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case VAR_ID_NUM:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    //assign type
		    root->type = root->children[0]->type;
		    
		    //if array statement then check bounds. WhichId is handled here.
		    if(root->children[0]->tok==ID)
		    {
			if(root->children[1]->tok==EPS)
			    return curr_offset;
			else
			{
			    //search the entry for the array
			    symbol_table_node* arr = searchSymbolTable(current_table, root->children[0]->lexeme->str);
			    
			    // temp = INDEX
			    astnode* temp = root->children[1]->children[0];
			    if(arr->isdynamic)
			    {
				//code for bound check in dynamic array.
			    }
			    else
			    {
				if(temp->children[0]->tok!=NUM)
				{
				    //code for bound check in static array but dynamic index.
				}	
			    }
			}
		    }
		    return curr_offset;
		}break;
	    
		
	    case WHICHID:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    return curr_offset;
		}break;

	    case SIMPLESTMT:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;

	    case ASSIGNMENTSTMT:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		  
		    symbol_table_node* a = searchSymbolTable(current_table, root->children[0]->lexeme->str);
		    astnode* lvalue = root->children[1]->children[0];

		    
		    if(lvalue->tok==LVALUEIDSTMT)
		    {
			//code for assigning to ID
		    }
		    else
		    {
			//lvalue array statement

			// temp = INDEX
			astnode* temp = lvalue->children[0];

			if(a->isdynamic)
			{
			    //code for bound check in dynamic array.
			}
			else
			{
			    if(temp->children[0]->tok!=NUM)
			    {
				//code for bound check in static array but dynamic index.
			    }	
			}
		    }
		    return curr_offset;
		}break;

	    case WHICHSTMT:
		{
		    //move forward and assign type of child to whichstmt
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}

	    case LVALUEIDSTMT:
		{
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    //move forward and assign type of child to lvalueidstmt
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case LVALUEARRSTMT:
		{
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[1]->type;
		    return curr_offset;
		}break;
	    
	    case INDEX:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case MODULEREUSESTMT:
		{
		    //code to call the function
		    return curr_offset;	
		}break;

	    case OPTIONAL:
		{
		    if(root->tok==EPS)
			return curr_offset;
		    else
			for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;

		}
	    case IDLIST: case N3:
		{
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;
	    
	    case EXPRESSION:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;
	    case U:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[1]->type;  //type of NEW_NT
		    return curr_offset;
		}break;
	    case NEW_NT:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;

		    return curr_offset;
		}break;
	    case UNARY_OP:
		{
		    //do nothing
		    //this is instruction for code generator to 
		    //take know the sign 
		    return curr_offset;
		}break;
	    
	    case ARITHMETICORBOOLEANEXPR:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    //then check 
		    if(root->children[1]->tok!=EPS)
		    {
			root->type = boolean;
		    }
		    else
		    {
			root->type = root->children[0]->type; //Anyterm.type
		    }
		    return curr_offset;
		}break;

	    case N7:
		{
		    //move forward 
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		   
		    //then check 
		    if(root->tok==EPS)
			return curr_offset;
		    else
		    {
			root->type = boolean;
		    }
		    return curr_offset;

		}break;
	    case ANYTERM:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    //then check
		   if(root->children[0]->tok == BOOLCONSTT)
		      root->type = root->children[0]->type;
		   else
		   {
		       if(root->children[1]->tok!=EPS)
		       {
			   root->type = boolean;
		       }
		       else
		       {
			   root->type = root->children[0]->type; //arithExpr.type
		       }
		   }
		   return curr_offset;
		}break;
	    
	    case N8:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    if(root->tok==EPS)
			return curr_offset;
		    else
		    {
			root->type = boolean;
		    }
		    return curr_offset;
		}break;

	    case ARITHMETICEXPR:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    if(root->children[1]->tok!=EPS)
		    {
			root->type = root->children[0]->type;
		    }
		    else
		    {
			root->type = root->children[0]->type; //Anyterm.type
		    }
		    return curr_offset;
		}break;

	    case N4:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    if(root->children[0]->tok == EPS)
			return curr_offset;
		    else
		    {
			root->type = root->children[1]->type;
		    }
		    return curr_offset;
		}break;

	    case TERM:
		{
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    if(root->children[1]->tok!=EPS)
		    {
			root->type = root->children[0]->type;
		    }
		    else
		    {
			root->type = root->children[0]->type; //Anyterm.type
		    }
		    return curr_offset;

		}break;

	    case N5:
		{  
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    if(root->children[0]->tok == EPS)
			return curr_offset;
		    else
		    {
			root->type = root->children[1]->type;
		    }
		    return curr_offset;
		    }
		break;
	    case FACTOR:
		{
		    //move forward then assign type
		   for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    //assign type
		    root->type = root->children[0]->type;
		    
		    return curr_offset;
		}break;
	    case OP1:
	    case OP2:
	    case LOGICALOP:
	    case RELATIONALOP:
		{
		    //do nothing
		    //this is for code generator
		    return curr_offset;
		}break;
	    
	    case DECLARESTMT:
		{
		    //move forward
		    curr_offset = codegen(root->children[1], current_table,curr_offset);
		    curr_offset = declareVariablesOffset(current_table, root->children[0], root->children[1],curr_offset);
		    return curr_offset;
		}break;

	    case CONDITIONALSTMT:
		{
		    //create a new scope(symbol table) and make the current table as the parent of the new table.
		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = current_table;

		    //move forward. type of ID is handled by the symbol table when it is called.
		    
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], new_table, curr_offset);
		    return curr_offset;
		}break;

	    case CASESTMTS:
		{
		    //move forward 
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case N9:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
			if(root->tok == EPS)
			    return curr_offset;
			else
			{
			    root->type = root->children[0]->type;   
			}
		    return curr_offset;
		}break;

	    case VALUE:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;
	    case DEFAULT1:
		{
		    //move forward.
		    //nothing else needs to be done
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    return curr_offset;
		}break;
	    case ITERATIVESTMT:
		{
		    if(root->children[0]->tok == FOR)
		    {
			//check ID
			curr_offset = codegen(root->children[1],current_table,curr_offset);
			symbol_table_node* a = searchSymbolTable(current_table, root->children[1]->lexeme->str);
			
			//Check Range
			curr_offset = codegen(root->children[2],current_table,curr_offset);

			//for loop
			symbolTable* new_table = getSymbolTable(100);
			new_table->parent = current_table;
			curr_offset = codegen(root->children[3],new_table,curr_offset);

		    }
		    else
		    {
			//while loop
			
			//check that conditional expression is boolean
			curr_offset = codegen(root->children[1],current_table,curr_offset);

			//create new scope(symbol table) and assign the current table as its parent
			//then move forward
			symbolTable* new_table = getSymbolTable(100);
			new_table->parent = current_table;
			curr_offset = codegen(root->children[2],new_table,curr_offset);
		    }
		    return curr_offset;
		}break;
	    case RANGE:
		{
		    
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);

		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM))
		    {
			//dynamic range check. arg1->ID arg2->NUM
		    }
		    if((root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {
			//dynamic range check. arg1->NUM arg2->ID
		    }
		    if(root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==ID)
		    {
			//dynamic check of index1 < index2
		    }

		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;
	    default: return curr_offset;
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
		    {
			printf("Undeclared variable '%s'",root->lexeme->str);
			exit(1);
		    }
		    else
			root->type = temp->type;

		}break;
	    case EPS:
		root->type = NONE;
		break;
	}
	return curr_offset; 
    }
}


