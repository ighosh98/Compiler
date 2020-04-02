#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"
#include <string.h>
#include "symboltable.h"
#include "semantic.h"

//2 passes required for making sure that all the functions are defined and checked
//make sure errors are only printed once in the first pass for normal errors
//and for MODULEREUSE the errors are printed in the 2nd pass.
symbolTable * function_table;
int pass_no = 1;
bool checkCases(astnode* root, datatype type)
{
    astnode* head = root;
    symbolTable* table = getSymbolTable(100);
    while(root->tok!=EPS)
    {
	//checking that all cases are unique
	symbol_table_node* a = searchSymbolTable(table, root->children[0]->lexeme->str);
	if(a!=NULL)
	{
	    printf("Line No %d: Cases must be unique\n",root->lexeme->line_no);
	    return false;
	}
	else
	{
	    insertSymbolTable(table, root->children[0]->lexeme->str,false,false,
		    NULL,NULL,-1,-1,NULL,NONE);
	}
	root = root->children[2];
    }
    if(type==boolean)
    {
	//check that if boolean cases. then true and false are both included
	symbol_table_node* a = searchSymbolTable(table,"false");
	symbol_table_node* b = searchSymbolTable(table,"true");
	if(a==NULL || b== NULL)
	{
	    printf("Line no %d: Boolean cases must include true and false\n",head->lexeme->line_no);
	    return false;
	}
    }
    return true;
}

bool checkCallInput(symbolTable* table, astnode* input_list, symbol_table_node* func)
{
    astnode* head = input_list;
    symbol_table_node* iplist = func->iplist;  //checking input list (iplist)
    while(1)
    {
	if((input_list->tok==EPS && iplist!=NULL))
	{
	    printf("Line no %d: Incorrect Number of arguments\n",head->lexeme->line_no);
	    return false;
	} 
	if((iplist==NULL&&input_list->tok!=EPS))
	{
	    printf("Line no %d: Incorrect Number of arguments\n",head->lexeme->line_no);
	    return false;  
	}//ERROR: Number of arguments do not match the Function signature
    
	if(input_list->children[0]->type != iplist->type)
	{
	    printf("Line no %d: Argument '%s' is incompatible with function arguments\n",
		    input_list->lexeme->line_no,input_list->children[0]->lexeme->str);
	    return false;   //ERROR: Type mismatch in function argument
	}
	if(iplist->isarr==true) //verify array ranges
	{
	    symbol_table_node* temp = searchSymbolTable(table,input_list->children[0]->lexeme->str);
	    if(temp->isarr==false)
	    {
		printf("Line no %d: Argument '%s' is incompatible with function arguments (Array expected)\n",
		    input_list->lexeme->line_no,input_list->children[0]->lexeme->str);
	    
	    	return false; //Array type argument expected for function.
	    }
	    if(temp->isdynamic==false)
	    {
		if(temp->crange1!=iplist->crange1 || temp->crange2!=iplist->crange2)
		{
		    printf("Line no %d: Argument '%s' is incompatible with function arguments(Range Mismatch)\n",
		    input_list->lexeme->line_no,input_list->children[0]->lexeme->str);
		    return false; //Array range mismatch in function argument
		}
	    }
	    else
	    {
		return true;
		//code for verifying range if dynamic array passed.
	    }
	}

	iplist = iplist->iplist;
	input_list = input_list->children[1];

	if(iplist==NULL && input_list->tok==EPS)
	    return true;
    }
}

bool checkCallOutput(symbolTable* table, astnode* input_list, symbol_table_node* func)
{
    symbol_table_node* iplist = func->oplist; //checking output list (oplist)
    while(1)
    {
	if((input_list->tok==EPS && iplist!=NULL))
	{
	    return false;
	} 
	if((iplist==NULL&&input_list->tok!=EPS))
	{
	    return false;  
	}//ERROR: Number of arguments do not match the Function signature
    
	if(input_list->children[0]->type != iplist->type)
	{
	    return false;   //ERROR: Type mismatch in function argument
	}
	iplist = iplist->iplist;
	input_list = input_list->children[1];

	if(iplist==NULL && input_list->tok==EPS)
	    return true;
    }
}

void fillCheckTable(symbolTable* check_table, astnode* root)
{
    if(root->tok==ID)
	insertSymbolTable(check_table,root->lexeme->str,false,false,
		NULL,NULL,-1,-1,NULL,NONE);

    for(int i =0;i<root->n;i++)
	fillCheckTable(check_table, root->children[i]);
}

bool checkWhile(symbolTable* check_table, astnode* root)
{
    if(root->tok==ASSIGNMENTSTMT)
    {
	if(searchSymbolTable(check_table,root->children[0]->lexeme->str)!=NULL)
	    return true;
    }
    else
    {
	bool ans = false;
	for(int i =0;i<root->n;i++)
	    ans = ans | checkWhile(check_table,root->children[i]);
	return ans;
    }
}

void declareVariables(symbolTable* table, astnode* idlist, astnode* datatype)
{
    ////////////////////////////////////////////////////////////////
    /////////////// Check for redeclaration ////////////////////////
    ////////////////////////////////////////////////////////////////
    astnode* dataTypeVar = datatype;
    while(idlist->tok!=EPS)
    {
	if(dataTypeVar->children[0]->tok==INTEGER)
	{
	    idlist->children[0]->type = integer;
	    insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, integer); //integer type variable

	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==REAL)
	{
	    idlist->children[0]->type = real;
	    insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, real); //integer type variable
	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==BOOLEAN)
	{
	    idlist->children[0]->type = boolean;
	    insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, boolean); //integer type variable

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
	    if(dataTypeVar->children[0]->children[0]->children[0]->tok==NUM)
	    {
		//static range of array
		a->isdynamic = false;
		a->crange1 = atoi(dataTypeVar->children[0]->children[0]->children[0]->lexeme->str); //first index
		a->crange2 = atoi(dataTypeVar->children[0]->children[1]->children[0]->lexeme->str); //second index
	    }
	    else
	    {
		//dynamic range of array
		a->isdynamic = true;
		astnode* indexVar1 = dataTypeVar->children[0]->children[0]; //index nodes
		astnode* indexVar2 = dataTypeVar->children[0]->children[1];

		a->drange1 = searchSymbolTable(table,indexVar1->children[0]->lexeme->str); //assign the symbol table entry
		a->drange2 = searchSymbolTable(table,indexVar2->children[0]->lexeme->str); //of the variable.
	    }

	    idlist = idlist->children[1];
	}
    }
}

symbol_table_node* makeInputList(astnode* inputTree, symbolTable* table)
{

    if(inputTree->tok==EPS)return NULL;

    //insert FIRST variable into inputlist

    symbol_table_node * nextinput = makeSymbolNode(inputTree->children[0]->lexeme->str,
	    false,false, NULL,NULL,-1,-1,inputTree->children[0]->lexeme,NONE);

    //update the datatype of the variable
    astnode* dataTypeVar = inputTree->children[1];
    type_semantics(dataTypeVar,table);
    if(dataTypeVar->children[0]->tok==INTEGER)
	nextinput->type = integer;
    
    else if(dataTypeVar->children[0]->tok==REAL)
	nextinput->type = real;

    else if(dataTypeVar->children[0]->tok==BOOLEAN)
	nextinput->type = boolean;

    else
    {
	//input is of type array.
	 
	nextinput->type = dataTypeVar->children[1]->type; //what isthe type of the array
	nextinput->isarr = true;
	
	//static or dynamic range of array??
	//datatype->rangearrays->index->(NUM/ID)
	if(dataTypeVar->children[0]->children[0]->children[0]->tok==NUM)
	{
	    //static range of array
	    nextinput->isdynamic = false;
	    nextinput->crange1 = atoi(dataTypeVar->children[0]->children[0]->children[0]->lexeme->str); //first index
	    nextinput->crange2 = atoi(dataTypeVar->children[0]->children[1]->children[0]->lexeme->str); //second index
	}
	else
	{
	    //dynamic range of array
	    nextinput->isdynamic = true;
	    astnode* indexVar1 = dataTypeVar->children[0]->children[0];
	    astnode* indexVar2 = dataTypeVar->children[0]->children[1];

	    nextinput->drange1 = makeSymbolNode(indexVar1->children[0]->lexeme->str,
		    false,false, NULL,NULL,-1,-1,indexVar1->children[0]->lexeme,integer); //first index variable

	    nextinput->drange2 = makeSymbolNode(indexVar2->children[0]->lexeme->str,
		    false,false, NULL,NULL,-1,-1,indexVar2->children[0]->lexeme,integer);  //second index variable
	}
    }

    nextinput->iplist = makeInputList(inputTree->children[2],table);
    return nextinput;
}

symbol_table_node* makeOutputList(astnode* outputTree)
{

    if(outputTree->tok==EPS)return NULL;

    //insert FIRST variable into inputlist

    symbol_table_node * nextoutput = makeSymbolNode(outputTree->children[0]->lexeme->str,
	    false,false, NULL,NULL,-1,-1,outputTree->children[0]->lexeme,NONE);

    //update the datatype of the variable
    astnode* TypeVar = outputTree->children[1];
    if(TypeVar->children[0]->tok==INTEGER)
	nextoutput->type = integer;

    else if(TypeVar->children[0]->tok==REAL)
	nextoutput->type = real;

    else if(TypeVar->children[0]->tok==BOOLEAN)
	nextoutput->type = boolean;
    else
    {}//ERROR: ARRAY OUTPUT NOT ALLOWED

    nextoutput->oplist = makeOutputList(outputTree->children[2]);
    return nextoutput;
}

void type_semantics(astnode* root, symbolTable* current_table)
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
		    if(pass_no==1)
			function_table = getSymbolTable(100);
		    
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		    
		    pass_no++;
		    return;
		}break;

	    case MODULEDECLARATIONS:
		{

		    //if empty then return. else process the subtree
		    if(root->tok == EPS)
			return;
		    else
			for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    return;
		}break;

	    case MODULEDECLARATION:
		{
		    if(pass_no==1){
			//add entry for the function in the function table. and mark IsCalled = False and isDefined = false
			symbol_table_node* temp = insertSymbolTable(function_table, 
				root->children[0]->lexeme->str , false, false, NULL, NULL, 
				-1,-1, root->children[0]->lexeme, function); //type = function

			temp -> isUsed = false;
			temp -> isDefined = false;

			//assign function type to the ID node.
			root->children[0]->type = function;
		    }
		    return;
		}break;

	    case OTHERMODULES:
		{
		    if(root->tok == EPS)
			return;
		    else
			for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    return;
		}break;

	    case MODULE1:
		{
		    symbol_table_node* temp = searchSymbolTable(function_table, root->children[0]->lexeme->str);
		    
			//if entry already exists
			if(temp!=NULL)
			{
			    //function has already been declared
			    if(temp->isUsed==false)
			    {} //Error: function not used after declaration
			    else if(temp->isDefined==true)
			    {}  //Error: Redefinition of the function
			}


			///////// add the function definition/signature to the function table.///////////

			//type of the id is function.
			root->children[0]->type = function;

			//insert function name into symboltable/ retreive the entry if  already present
			temp = insertSymbolTable(function_table, root->children[0]->lexeme->str,
				false, false, NULL, NULL, -1,-1, 
				root->children[0]->lexeme, function);

			//add input list into function
			temp->iplist = makeInputList(root->children[1],current_table); 

			//add output_plist into function
			if(root->children[2]->tok!=EPS)
			    temp->oplist = makeOutputList(root->children[2]->children[0]);

			//function is now defined. redefinition not allowed
			temp->isDefined = true;
		    
		    //go onto moduledef with a new symbol table which has
		    //the entries for input_plist vars and out_plist vars


		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = NULL; //new function has no parent scope

		    //insert input vars
		    symbol_table_node* a = temp->iplist;
		    while(a)
		    {
			if(searchSymbolTable(new_table,a->name))
			{}//ERROR: cannot have vars with same name
			insertSymbolTable(new_table, a->name,a->isarr,a->isdynamic,
				a->drange1,a->drange2,a->crange1,a->crange2,a->lexeme,a->type);
			a = a->iplist;
		    }

		    //insert output vars
		    a = temp->oplist;
		    while(a)
		    {
			if(searchSymbolTable(new_table, a->name))
			{}//ERROR: cannot have vars with same name
			insertSymbolTable(new_table, a->name,a->isarr,a->isdynamic,
				a->drange1,a->drange2,a->crange1,a->crange2,a->lexeme,a->type);
			a = a->oplist;
		    }


		    //add current function
		    a = insertSymbolTable(new_table,"currentfunction",false,false,NULL,NULL,
			    -1,-1,NULL,NONE);
		    a->iplist = temp;

		    //call moduledef
		    type_semantics(root->children[3],new_table);
		    
		}break;

	    case DRIVERMODULE:
		{
		    //create symbol table for the program
		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = NULL;
		    
		    //function table entry for driver module
		    insertSymbolTable(function_table,"driver",false,false,NULL,NULL,-1,-1,NULL,function);
		    //set current funtion as the driver function
		    symbol_table_node* a = insertSymbolTable(new_table,"currentfunction",false,false,NULL,NULL,
			    -1,-1,NULL,NONE);

		    a->iplist = searchSymbolTable(function_table,"driver");

		    //call moduledef
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], new_table);
		    return;
		}break;

	    
	    case RET: case INPUT_PLIST: case N1: case OUTPUT_PLIST: case N2:
		{
		    //handled by module1.
		    return;    
		}break;


	    case DATATYPE:
		{
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);

		    if(root->children[0]->tok!=RANGE_ARRAYS)
			root->type = root->children[0]->type;
		    else
		    {
			root->type = root->children[1]->type;
		    }
		    // handle by input_list and declare statement
		}break;
   
	    case RANGE_ARRAYS:
		{
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);

		    if(root->children[0]->type!=root->children[1]->type)
		    {}//ERROR: range has to be of same type.

		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM)
			|| (root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {}//ERROR: range arrays should be both NUM or both ID
		    
		    if(root->children[0]->type!=integer)
		    {}//ERROR: range can only be integer.
		    else if(root->children[0]->tok==NUM && 
			    (atoi(root->children[0]->lexeme->str)>atoi(root->children[0]->lexeme->str)))
		    {}//ERROR: Invalid range. arg1 greater then arg2.
		    else if(root->children[0]->tok==ID)
		    {
			//dynamic check of index1 < index2
		    }

		    root->type = root->children[0]->type;
		    return;
		}break;

	    case TYPE:
		{
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    //handle by outputplist and datatype
		}break;
	    
	    case MODULEDEF:
		{

		    //new symbol table is created by whatever calls moduledef
		    
		    //then pass the new table as the current_table in further calls.
		    //move forward
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		    return;
		}break;


	    case STATEMENTS:
		{
		    if(root->tok == EPS)
			return;
		    else
			//move forward
			for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    return;
		}break;

	    case STATEMENT:
		{
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    return;
		}break;

	    case IOSTMT:
		{
		    //first child stores print/get info. hence nothing needs to be done.
		    //move forward to check variables.
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table); 
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
			type_semantics(root->children[i], current_table);
		    root->type = root->children[0]->type;
		    return;
		}break;
	    case VAR:
		{
		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    return;
		}break;

	    case VAR_ID_NUM:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    //assign type
		    root->type = root->children[0]->type;
		    
		    //if array statement then check bounds. WhichId is handled here.
		    if(root->children[0]->tok==ID)
		    {
			if(root->children[1]->tok==EPS)
			    return;
			else
			{
			    //search the entry for the array
			    symbol_table_node* arr = searchSymbolTable(current_table, root->children[0]->lexeme->str);
			    
			    // temp = INDEX
			    astnode* temp = root->children[1]->children[0];
			    
			    if(temp->children[0]->type!=integer) 
				{}	//ERROR: index must be integer


			    if(arr->isdynamic)
			    {
				//code for bound check in dynamic array.
			    }
			    else
			    {
				if(temp->children[0]->tok==NUM)
				{
				    //static check in static array and static index
				    int value = atoi(temp->children[0]->lexeme->str);
				    
				    if(value<arr->crange1 || value>arr->crange2)
				    {}//ERROR: index out of bounds
				}
				else
				{
				    //code for bound check in static array but dynamic index.
				}	
			    }
			}
		    }
		    return;
		}break;
	    
		
	    case WHICHID:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		    
		    return;
		}break;

	    case SIMPLESTMT:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    return;
		}break;

	    case ASSIGNMENTSTMT:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		  
		    symbol_table_node* a = searchSymbolTable(current_table, root->children[0]->lexeme->str);
		    astnode* lvalue = root->children[1]->children[0];

		    
		    if(lvalue->tok==LVALUEIDSTMT)
		    {
			//lvalue id statement
			
			if(a->isarr == true)
			{}//ERROR: The variable is an array. Index not provided.
			
			if(lvalue->children[0]->type != a->type)
			{}//ERROR: type mismatch. The lavalue and rvalue have incompatible types.

		    }
		    else
		    {
			//lvalue array statement

			if(a->isarr==false)
			{}//ERROR: The variable is not an array. Cannot be Indexed.
			if(lvalue->children[1]->type != a->type)
			{}//ERROR: type mismatch. The lavalue and rvalue have incompatible types.

			    // temp = INDEX
			    astnode* temp = lvalue->children[0];
			    
			    if(temp->children[0]->type!=integer) 
			    {}	//ERROR: index must be integer

			    if(a->isdynamic)
			    {
				//code for bound check in dynamic array.
			    }
			    else
			    {
				if(temp->children[0]->tok==NUM)
				{
				    //static check in static array and static index
				    int value = atoi(temp->children[0]->lexeme->str);
				    
				    if(value<a->crange1 || value>a->crange2)
				    {}//ERROR: index out of bounds
				}
				else
				{
				    //code for bound check in static array but dynamic index.
				}	
			    }
		    }
		}break;

	    case WHICHSTMT:
		{
		    //move forward and assign type of child to whichstmt
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		}

	    case LVALUEIDSTMT:
		{
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		    
		    //move forward and assign type of child to lvalueidstmt
		    root->type = root->children[0]->type;
		}break;

	    case LVALUEARRSTMT:
		{
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[1]->type;
		}break;
	    
	    case INDEX:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		
		    root->type = root->children[0]->type;
		}break;

	    case MODULEREUSESTMT:
		{
			//get the function entry
			symbol_table_node* a = searchSymbolTable(function_table, root->children[1]->lexeme->str); //ID i.e function name
			

			//verify that the ID is really a function
			if(a==NULL)
			{}//ERROR: the ID is not a Fucntion.

			//the function has been called once;
			a->isUsed = true;

			//check if recursion. if yes then error
			symbol_table_node* current_func = searchSymbolTable(current_table,"currentfunction");
			current_func = current_func->iplist;

			if(strcmp(a->name,current_func->name)!=0)
			{}//ERROR: Recursion Not allowed.
			
		    if(pass_no==2)
		    {
			//verify the function signature in pass 2 when it is defined

			//verify idlist is same as function inputplist
			type_semantics(root->children[2],current_table);

			astnode* input_list = root->children[2];
			if(!checkCallInput(current_table, input_list, a))
			{
			    printf("error found######################\n");
			}//ERROR: Input parameters dont match the function called.

			//if option not eps then verify if output idlist is same as output plist also.
			if(root->children[0]->tok!=EPS)
			{
			    astnode* output_list = root->children[0]->children[0];
			    type_semantics(output_list, current_table);
			    if(!checkCallOutput(current_table, output_list, a))
			    {
				printf("ERROR FOUND#################\n");
			    }//ERROR: Output Parameters dont match the function called

			}
		    }
		}break;

	    case OPTIONAL:
		{
		    if(root->tok==EPS)
			return;
		    else
			for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    return;

		}
	    case IDLIST: case N3:
		{
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    return;
		}break;
	    
	    case EXPRESSION:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    return;
		}break;
	    case U:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[1]->type;  //type of NEW_NT
		    return;
		}break;
	    case NEW_NT:
		{
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		   
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
			    type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		    
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
			type_semantics(root->children[i], current_table);
		    
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
			type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		    
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
		    //move forward
		    type_semantics(root->children[1], current_table);
		    
		    declareVariables(current_table, root->children[0], root->children[1]);
		}break;

	    case CONDITIONALSTMT:
		{
		    //create a new scope(symbol table) and make the current table as the parent of the new table.
		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = current_table;

		    //move forward. type of ID is handled by the symbol table when it is called.
		    
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], new_table);
		    


		    //check if the type of the switch statemt matches.
		    if(root->children[0]->type==integer)
		    {
			if(root->children[0]->type != root->children[1]->type)
			{
			}  //ERROR: case type does not match argument type
			if(root->children[2]->tok==EPS)
			{
			}  //ERROR: default case not included
			if(!checkCases(root->children[1],integer))
			{
			}//cases must be unique
			    
			//check all case statements are unique!!
		    }
		    else if(root->children[0]->type==boolean)
		    {
			if(root->children[0]->type != root->children[1]->type)
			{}//ERROR: case type does not match argument type
			if(root->children[2]->tok!=EPS)
			{}  //ERROR: boolean case cannot have default case.
			if(!checkCases(root->children[1],boolean))
			{
			}//only true and false must be included in case  statement     

		    }
		    else
		    {}//ERROR: switch can only have integer of boolean argument

		    return;
		}break;

	    case CASESTMTS:
		{
		    //move forward 
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		    
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
			    type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    return;
		}break;
	    case DEFAULT1:
		{
		    //move forward.
		    //nothing else needs to be done
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    return;
		}break;
	    case ITERATIVESTMT:
		{
		    if(root->children[0]->tok == FOR)
		    {
			//check ID
			type_semantics(root->children[1],current_table);
			symbol_table_node* a = searchSymbolTable(current_table, root->children[0]->lexeme->str);
			if(root->children[1]->type!=integer) 
			{}//ERROR: The variable must be an integer
			if(a->isarr==true)
			{}//ERROR: The variable cannot be an array

			//Check Range
			type_semantics(root->children[2],current_table);
			if(root->children[2]->type!=integer)
			{}//ERROR: type error


			//for loop
			symbolTable* new_table = getSymbolTable(100);
			new_table->parent = current_table;
			type_semantics(root->children[3],new_table);
		    }
		    else
		    {
			//while loop
			
			//create check table
			symbolTable* check_table = getSymbolTable(100);
			astnode* exprnode = root->children[1];
			fillCheckTable(check_table, exprnode);
			
			if(!checkWhile(check_table,root->children[2]))
			{}//ERROR: None of the variables in the while condition are updated.

			//check that conditional expression is boolean
			type_semantics(root->children[1],current_table);
			if(root->children[1]->type!=boolean)
			{}//ERROR: the condition inside while must evaluate to boolean;


			//create new scope(symbol table) and assign the current table as its parent
			//then move forward
			symbolTable* new_table = getSymbolTable(100);
			new_table->parent = current_table;
			type_semantics(root->children[2],new_table);
		    }
		    return;
		}break;
	    case RANGE:
		{
		    
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);

		    if(root->children[0]->type!=root->children[1]->type)
		    {}//ERROR: range has to be of same type.

		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM)
			|| (root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {}//ERROR: range arrays should be both NUM or both ID
		    
		    if(root->children[0]->type!=integer)
		    {}//ERROR: range can only be integer.
		    else if(root->children[0]->tok==NUM && 
			    (atoi(root->children[0]->lexeme->str)>atoi(root->children[0]->lexeme->str)))
		    {}//ERROR: Invalid range. arg1 greater then arg2.
		    else if(root->children[0]->tok==ID)
		    {
			//dynamic check of index1 < index2
		    }

		    root->type = root->children[0]->type;
		    return;
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
		    {
			printf("ERROR: Symbol '%s' Not Recognized\n",root->lexeme->str);
		    }//ERROR: symbol not recognized.

		    else
			root->type = temp->type;

		}break;
	    case EPS:
		root->type = NONE;
		break;
	}
	    

    }
}


