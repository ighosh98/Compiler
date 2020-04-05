#include <stdio.h>
#include "lexerDef.h"
#include "ast.h"
#include "astdef.h"
#include <string.h>
#include "symboltable.h"
#include "semantic.h"
#include "color.h"

//create a way to get the total size of the function in the function table.
//for Driver and for all other functions

symbolTable * function_table;
int pass_no = 0;
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
	    blue();
	    printf("Line No %d: ",root->lexeme->line_no);
	    reset();
	    printf("Cases must be unique\n");
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
	    blue();
	    printf("Line no %d: ",head->lexeme->line_no);
	    reset();
	    printf("Boolean cases must include true and false\n");
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
	    blue();
	    printf("Line no %d: ",head->lexeme->line_no);
	    reset();
	    printf("Incorrect number of arguments in Input\n");
	    return false;
	} 
	if((iplist==NULL&&input_list->tok!=EPS))
	{
	    blue();
	    printf("Line no %d: ",head->lexeme->line_no);
	    reset();
	    printf("Incorrect number of arguments in Input\n");
	    return false;  
	}//ERROR: Number of arguments do not match the Function signature
    
	if(input_list->children[0]->type != iplist->type)
	{
	    blue();
	    printf("Line no %d: ",input_list->lexeme->line_no);
	    reset();
	    printf("Argument '%s' is incompatible with the function arguments\n",input_list->children[0]->lexeme->str);
	    return false;   //ERROR: Type mismatch in function argument
	}
	if(iplist->isarr==true) //verify array ranges
	{
	    symbol_table_node* temp = searchSymbolTable(table,input_list->children[0]->lexeme->str);
	    if(temp->isarr==false)
	    {
		blue();
		printf("Line no %d: ",input_list->lexeme->line_no);
		reset();
		printf("Argument '%s' is incompatible with the function arguments(Array Expected)\n",input_list->children[0]->lexeme->str);
		return false; //Array type argument expected for function.
	    }
	    if(temp->isdynamic==false)
	    {
		if(temp->crange1!=iplist->crange1 || temp->crange2!=iplist->crange2)
		{
		    blue();
		    printf("Line no %d: ",input_list->lexeme->line_no);
		    reset();
		    printf("Argument '%s' is incompatible with the function arguments(Range Mismatch)\n",input_list->children[0]->lexeme->str);
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
    astnode* head = input_list;
    symbol_table_node* iplist = func->oplist; //checking output list (oplist)
    while(1)
    {
	if((input_list->tok==EPS && iplist!=NULL))
	{
	    blue();
	    printf("Line no %d: ",head->lexeme->line_no);
	    reset();
	    printf("Incorrect number variable in function Output\n");
	    return false;
	} 
	if((iplist==NULL&&input_list->tok!=EPS))
	{
	    blue();
	    printf("Line no %d: ",head->lexeme->line_no);
	    reset();
	    printf("Incorrect number of variable in function Output\n");
	    return false;  
	}//ERROR: Number of arguments do not match the Function signature
    
	if(input_list->children[0]->type != iplist->type)
	{
	    blue();
	    printf("Line no %d: ",input_list->lexeme->line_no);
	    reset();
	    printf("Argument '%s' is Incompatibel with function output (Type Mismatch)\n",input_list->children[0]->lexeme->str);
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
    symbol_table_node* curr_func = searchSymbolTable(table, "_currentfunction")->iplist;

    astnode* dataTypeVar = datatype;
    while(idlist->tok!=EPS)
    {
	if(searchSymbolTableLocal(table, idlist->children[0]->lexeme->str))
	{
	    //ERROR: Redeclaration of variable that already exists
	    if(pass_no==1){
	    blue();
	    printf("Line no %d: ",idlist->children[0]->lexeme->line_no);
	    reset();
	    printf("Redeclaration of %s not allowed.\n",idlist->children[0]->lexeme->str);
	    }
	    idlist = idlist->children[1];
	}
	else if(dataTypeVar->children[0]->tok==INTEGER)
	{
	    pass_no==1? curr_func->stackSize+=4 : pass_no;
	    idlist->children[0]->type = integer;
	    insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, integer); //integer type variable

	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==REAL)
	{
	    pass_no==1? curr_func->stackSize+=8 : pass_no;
	    idlist->children[0]->type = real;
	    insertSymbolTable(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, real); //integer type variable
	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==BOOLEAN)
	{
	    pass_no==1? curr_func->stackSize+=4 : pass_no;
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
	    if(dataTypeVar->children[0]->children[0]->children[0]->tok==NUM
		    && dataTypeVar->children[0]->children[1]->children[0]->tok==NUM)
	    {
		//static range of array
		a->isdynamic = false;
		a->crange1 = atoi(dataTypeVar->children[0]->children[0]->children[0]->lexeme->str); //first index
		a->crange2 = atoi(dataTypeVar->children[0]->children[1]->children[0]->lexeme->str); //second index
		
		//array size in memory
		if(a->type==integer)
		    pass_no==1? curr_func->stackSize +=  4*(a->crange2 - a->crange1 + 1) : pass_no;
		else if(a->type==real)
		    pass_no==1? curr_func->stackSize +=  8*(a->crange2 - a->crange1 + 1) : pass_no;
		else if(a->type == boolean)
		    pass_no==1?curr_func->stackSize +=  4*(a->crange2 - a->crange1 + 1) : pass_no;

	    }
	    else
	    {

		curr_func->stackSize+=0;  //the memory is allocated on heap.
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
    symbol_table_node* curr_func = searchSymbolTable(table,"_currentfunction")->iplist;
    if(inputTree->tok==EPS)return NULL;

    //insert FIRST variable into inputlist

    symbol_table_node * nextinput = makeSymbolNode(inputTree->children[0]->lexeme->str,
	    false,false, NULL,NULL,-1,-1,inputTree->children[0]->lexeme,NONE);

    //update the datatype of the variable
    astnode* dataTypeVar = inputTree->children[1];
    type_semantics(dataTypeVar,table);
    if(dataTypeVar->children[0]->tok==INTEGER)
    {
	pass_no==1? curr_func->stackSize+=4: pass_no;
	nextinput->type = integer;
    }
    else if(dataTypeVar->children[0]->tok==REAL)
    {
	pass_no==1? curr_func->stackSize+=4: pass_no;
	nextinput->type = real;
    }
    else if(dataTypeVar->children[0]->tok==BOOLEAN)
    {	
	pass_no==1? curr_func->stackSize+=4: pass_no;
	nextinput->type = boolean;
    }
    else
    {
	//input is of type array.
	 
	nextinput->type = dataTypeVar->children[1]->type; //what isthe type of the array
	nextinput->isarr = true;
	
	//static or dynamic range of array??
	//datatype->rangearrays->index->(NUM/ID)
	if(dataTypeVar->children[0]->children[0]->children[0]->tok==NUM 
		&& dataTypeVar->children[0]->children[1]->children[0]->tok==NUM)
	{
	    //static range of array
	    nextinput->isdynamic = false;
	    nextinput->crange1 = atoi(dataTypeVar->children[0]->children[0]->children[0]->lexeme->str); //first index
	    nextinput->crange2 = atoi(dataTypeVar->children[0]->children[1]->children[0]->lexeme->str); //second index
	    
	    //array size in memory
		if(nextinput->type==integer)
		    pass_no==1? curr_func->stackSize +=  4*(nextinput->crange2 - nextinput->crange1 + 1) : pass_no;
		else if(nextinput->type==real)
		    pass_no==1? curr_func->stackSize +=  8*(nextinput->crange2 - nextinput->crange1 + 1) : pass_no;
		else if(nextinput->type == boolean)
		    pass_no==1?curr_func->stackSize +=  4*(nextinput->crange2 - nextinput->crange1 + 1) : pass_no;


	}
	else
	{
	    //dynamic range of array
	    
	    curr_func->stackSize+=0;
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

symbol_table_node* makeOutputList(astnode* outputTree, symbolTable* table)
{

    symbol_table_node* curr_func = searchSymbolTable(table,"_currentfunction")->iplist;
    if(outputTree->tok==EPS)return NULL;

    //insert FIRST variable into inputlist

    symbol_table_node * nextoutput = makeSymbolNode(outputTree->children[0]->lexeme->str,
	    false,false, NULL,NULL,-1,-1,outputTree->children[0]->lexeme,NONE);

    //update the datatype of the variable
    astnode* TypeVar = outputTree->children[1];
    if(TypeVar->children[0]->tok==INTEGER)
    {
	pass_no==1? curr_func->stackSize+=4: pass_no;
	nextoutput->type = integer;
    }
    else if(TypeVar->children[0]->tok==REAL)
    {
	pass_no==1? curr_func->stackSize+=8: pass_no;
	nextoutput->type = real;
    }
    else if(TypeVar->children[0]->tok==BOOLEAN)
    {
	pass_no==1? curr_func->stackSize+=4: pass_no;
	nextoutput->type = boolean;
    }
    else
    {
	blue();
	printf("Line no: %d ", TypeVar->children[0]->lexeme->line_no);
	reset();
	printf("Array cannot be returned from function.\n");
    }//ERROR: ARRAY OUTPUT NOT ALLOWED

    nextoutput->oplist = makeOutputList(outputTree->children[2],table);
    return nextoutput;
}

void type_semantics(astnode* root, symbolTable* current_table)
{
//    printf("%s\n",symbol_map[root->tok]);
    if(root==NULL)return;

    if(root->children)
    {
	switch(root->tok)
	{   
	    case PROGRAM:
		{
		    pass_no++; 
		    //initialize the function table and move forward    
		    if(pass_no==1)
			function_table = getSymbolTable(100);
		    
		    for(int i =0;i<root->n;i++)
			type_semantics(root->children[i], current_table);
		    
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
			    if(pass_no==1 && temp->isUsed==false)
			    {
				blue();
				printf("Line no: %d ", root->lexeme->line_no);
				reset();
				printf("Redundant Declaration and Definition of function\n");
			    } //Error: function not used after declaration
			    else if(pass_no==1 && temp->isDefined==true)
			    {
				blue();
				printf("Line no: %d ", root->lexeme->line_no);
				reset();
				printf("Redefinition of Module '%s'\n", root->children[0]->lexeme->str);

			    }  //Error: Redefinition of the function
			}


			///////// add the function definition/signature to the function table.///////////

			//type of the id is function.
			root->children[0]->type = function;

			//insert function name into symboltable/ retreive the entry if  already present
			temp = insertSymbolTable(function_table, root->children[0]->lexeme->str,
				false, false, NULL, NULL, -1,-1, 
				root->children[0]->lexeme, function);

			
			//insert current function
			symbolTable* input_table = getSymbolTable(100); //input_list scope
			input_table->parent = NULL; //new function has no parent scope

			symbolTable* new_table = getSymbolTable(100); //table for the function scope.
			new_table->parent =  input_table;		  //function scope shadows input scope

			symbol_table_node * a = insertSymbolTable(new_table,"_currentfunction",false,false,NULL,NULL,
			    -1,-1,NULL,NONE);

			a->iplist = temp;

			//add input list into function
			temp->iplist = makeInputList(root->children[1],new_table); 

			//add output_plist into function
			if(root->children[2]->tok!=EPS)
			    temp->oplist = makeOutputList(root->children[2]->children[0],new_table);

			//function is now defined. redefinition not allowed
			temp->isDefined = true;
		    
		    //go onto moduledef with a new symbol table which has
		    //the entries for input_plist vars and out_plist vars

		    //insert input vars
		    a = temp->iplist;
		    while(a)
		    {
			if(pass_no == 1 && searchSymbolTable(input_table,a->name))
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Reused variable '%s' in function Input\n",a->name);
			}//ERROR: cannot have vars with same name
			insertSymbolTable(input_table, a->name,a->isarr,a->isdynamic,
				a->drange1,a->drange2,a->crange1,a->crange2,a->lexeme,a->type);
			a = a->iplist;
		    }
		    //insert output vars
		    a = temp->oplist;
		    while(a)
		    {
			if(pass_no==1 && searchSymbolTable(new_table, a->name))
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Reused variable '%s' in function Output\n",a->name);

			}//ERROR: cannot have vars with same name
			insertSymbolTable(new_table, a->name,a->isarr,a->isdynamic,
				a->drange1,a->drange2,a->crange1,a->crange2,a->lexeme,a->type);
			a = a->oplist;
		    }



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
		    symbol_table_node* a = insertSymbolTable(new_table,"_currentfunction",false,false,NULL,NULL,
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
		    
		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM))
		    {
			//dynamic range check. arg1->ID arg2->NUM
		    }
		    if((root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {
			//dynamic range check. arg1->NUM arg2->ID
		    }

		    //           INDEX1!=integer                                    INDEX2!=integer
		    if(pass_no==1&&(root->children[0]->children[0]->type!=integer || root->children[1]->children[0]->type!=integer))
		    {
			    blue();
			    printf("Line no: %d ", root->children[0]->lexeme->line_no);
			    reset();
			    printf("Arguments of range must be integers\n"); 
		    

		    }//ERROR: range can only be integer.

		    else if(pass_no==1 && 
			    (root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==NUM &&
			    (atoi(root->children[0]->children[0]->lexeme->str)>atoi(root->children[1]->children[0]->lexeme->str))))
		    {
		    
			    blue();
			    printf("Line no: %d ", root->children[0]->lexeme->line_no);
			    reset();
			  printf("Invalid range, %d is greater than %d\n",
				atoi(root->children[0]->children[0]->lexeme->str),
				atoi(root->children[1]->children[0]->lexeme->str));
		    
		    }//ERROR: Invalid range. arg1 greater then arg2.

		    else if(root->children[0]->tok==ID)
		    {
			//dynamic check of index1 < index2
		    }

		    root->type = root->children[0]->type;

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
			    
			    if(pass_no==1 && temp->children[0]->type!=integer) 
				{
				    blue();
				    printf("Line no: %d ",root->lexeme->line_no);
				    reset();
				    printf("Array index must be an integer\n");
				}	//ERROR: index must be integer


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
				    
				    if(pass_no==1 && (value<arr->crange1 || value>arr->crange2))
				    {
					blue();
					printf("Line no: %d ",root->lexeme->line_no);
					reset();
					printf("Index out of bound\n");
				    }//ERROR: index out of bounds
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
			
			if(pass_no==1 && a->isarr == true)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Cannot assign to array type. Index not provided\n"); 
			}//ERROR: The variable is an array. Index not provided.
			
			if(pass_no==1 && lvalue->children[0]->type != a->type)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Incompatible types in assignment\n"); 

			}//ERROR: type mismatch. The lavalue and rvalue have incompatible types.

		    }
		    else
		    {
			//lvalue array statement

			if(pass_no==1 && a->isarr==false)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("The variable is not an array. Cannot be indexed\n"); 

			}//ERROR: The variable is not an array. Cannot be Indexed.
			if(pass_no==1 && lvalue->children[1]->type != a->type)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Incompatible types in assignment\n"); 


			}//ERROR: type mismatch. The lavalue and rvalue have incompatible types.

			    // temp = INDEX
			    astnode* temp = lvalue->children[0];
			    
			    if(pass_no==1 && temp->children[0]->type!=integer) 
			    {
				blue();
				printf("Line no: %d ", root->lexeme->line_no);
				reset();
				printf("Index of array must be integer\n"); 


			    }	//ERROR: index must be integer

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
				    
				    if(pass_no==1 && (value<a->crange1 || value>a->crange2))
				    {
					blue();
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Index out of bounds\n"); 


				    }//ERROR: index out of bounds
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
			{
			    if(pass_no==1)
			    {
				blue();
				printf("Line no: %d ", root->lexeme->line_no);
				reset();
				printf("'%s' is not a function\n",root->children[1]->lexeme->str); 
			    }
			    return;
			}//ERROR: the ID is not a Fucntion.
			else
			{
			    if(pass_no==2 && a->isDefined==false)
			    {
			    	blue();
				printf("Line no: %d ", root->lexeme->line_no);
				reset();
				printf("'%s' function definition missing\n",root->children[1]->lexeme->str); 
			    }
			}
			

			//the function has been called once;
			a->isUsed = true;
			//check if recursion. if yes then error
			symbol_table_node* current_func = searchSymbolTable(current_table,"_currentfunction");
			char* tempstr = current_func->iplist->name;
    
			if(pass_no==1 && strcmp(a->name,tempstr)==0)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("The functin '%s' cannot call itself. Recursion id not allowed\n", 
				    root->children[1]->lexeme->str); 
			}//ERROR: Recursion Not allowed.
			
		    if(pass_no==2)
		    {
			//verify the function signature in pass 2 when it is defined

			//verify idlist is same as function inputplist
			type_semantics(root->children[2],current_table);

			astnode* input_list = root->children[2];
			if(!checkCallInput(current_table, input_list, a))
			{
			    //handled inside
			}//ERROR: Input parameters dont match the function called.

			//if option not eps then verify if output idlist is same as output plist also.
			if(root->children[0]->tok!=EPS)
			{
			    astnode* output_list = root->children[0]->children[0];
			    type_semantics(output_list, current_table);
			    if(!checkCallOutput(current_table, output_list, a))
			    {
				//handled inside
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
		    if(root->children[1]->tok!=EPS)
		    {
			if(pass_no==1 && root->children[0]->type!=boolean)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Logical operations can only have boolean operands\n"); 
			    //ERROR: Logical Operations expect boolean type.
			}
			root->type = boolean;
		    }
		    else
		    {
			root->type = root->children[0]->type; //Anyterm.type
		    }
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
			if(pass_no==1 && root->children[1]->type!=boolean)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Logical operations can only have boolean operands\n"); 

			    //ERROR: Logical Operation can only have boolean type.
			}
			root->type = boolean;
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
		       if(root->children[1]->tok!=EPS)
		       {
			   if(pass_no==1 && root->children[0]->type!=integer)  //only integer can be used in relational op
			   {
			       blue();
			       printf("Line no: %d ", root->lexeme->line_no);
			       reset();
			       printf("Relational operations can only have integer operands\n"); 
			       //ERROR: Relational ops expect integer operands
			   }
			   root->type = boolean;
		       }
		       else
		       {
			   root->type = root->children[0]->type; //arithExpr.type
		       }
		   }
		   return;
		}break;
	    
	    case N8:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    if(root->tok==EPS)
			return;
		    else
		    {
			if(pass_no==1 && (root->children[1]->type!=integer)) //Only integer comparison allowed
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Relational operations can only have integer operands\n"); 

			    //ERROR: relational Operation can only have integer type.
			}
			if(pass_no==1 && root->children[2]->tok!=EPS)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Cannot chain relational operations. Cannot compare boolean operands\n"); 
			}//ERROR: cannot compare boolean operands.
			root->type = boolean;
		    }
		    return;
		}break;

	    case ARITHMETICEXPR:
		{
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    if(root->children[1]->tok!=EPS)
		    {
			if(pass_no==1 && root->children[0]->type==boolean)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Arithmetic operations cannot boolean operands\n"); 
			    //ERROR: Arithmetic operations cannot have boolean oprands
			}
			if(pass_no==1 && root->children[1]->type!=NONE &&
				(root->children[0]->type!=root->children[1]->type))
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Type Mismatch in expression\n"); 
			    //ERROR: Type mismatch in expression
			}
			root->type = root->children[0]->type;
		    }
		    else
		    {
			root->type = root->children[0]->type; //Anyterm.type
		    }
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
			if(pass_no==1 && (root->children[1]->type==boolean)) 
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Arithmetic operations cannot have boolean operands\n"); 

			    //ERROR: Arithmetic ops cannot have bool operands
			}
			if(pass_no==1 && root->children[2]->type!=NONE &&
				(root->children[1]->type!=root->children[2]->type))
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Type Mismatch in expression\n"); 
			}//ERROR: Type mismatch in expression

			root->type = root->children[1]->type;

		    }
		    return;
		}break;

	    case TERM:
		{
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    if(root->children[1]->tok!=EPS)
		    {
			if(pass_no==1 && root->children[0]->type==boolean)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Arithmetic operations cannot boolean operands\n"); 
			    //ERROR: Arithmetic operations cannot have boolean oprands
			}
			if(pass_no==1 && root->children[1]->type!=NONE &&
				(root->children[0]->type!=root->children[1]->type))
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Type Mismatch in expression\n"); 
			    //ERROR: Type mismatch in expression
			}
			root->type = root->children[0]->type;
		    }
		    else
		    {
			root->type = root->children[0]->type; //Anyterm.type
		    }
		    root->type = root->children[0]->type;
		    return;

		}break;

	    case N5:
		{  
		    //move forward then check
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    if(root->children[0]->tok == EPS)
			return;
		    else
		    {
			if(pass_no==1 && (root->children[1]->type==boolean)) 
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Arithmetic operations cannot have boolean operands\n"); 

			    //ERROR: Arithmetic ops cannot have bool operands
			}
			if(pass_no==1 && root->children[2]->type!=NONE&&
				(root->children[1]->type!=root->children[2]->type))
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Type Mismatch in expression\n"); 
			}//ERROR: Type mismatch in expression
			root->type = root->children[1]->type;

		    }
		    return;
		    }
		break;
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
		    if(pass_no==1 && root->children[0]->type==integer)
		    {
			if(root->children[0]->type != root->children[1]->type)
			{
			    blue();
			    printf("Line no: %d ", root->children[1]->lexeme->line_no);
			    reset();
			    printf("Case type does not match argument type\n"); 
			
			}  //ERROR: case type does not match argument type
			if(root->children[2]->tok==EPS)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("No default case provided for integer argument\n"); 
			
			}  //ERROR: default case not included
			if(!checkCases(root->children[1],integer))
			{
			    //prints inside function.
			}//cases must be unique
			    
			//check all case statements are unique!!
		    }
		    else if(pass_no==1 && root->children[0]->type==boolean)
		    {
			if(root->children[0]->type != root->children[1]->type)
			{
			    blue();
			    printf("Line no: %d ", root->children[1]->lexeme->line_no);
			    reset();
			    printf("Case type does not match argument type.\n"); 
			
			}//ERROR: case type does not match argument type
			if(root->children[2]->tok!=EPS)
			{
			    blue();
			    printf("Line no: %d ", root->children[2]->lexeme->line_no);
			    reset();
			    printf("Boolean argument does not require default case.\n"); 
			
			}  //ERROR: boolean case cannot have default case.
			if(!checkCases(root->children[1],boolean))
			{
			    //prints inside function
			}//only true and false must be included in case  statement     

		    }
		    else
		    {
			if(pass_no==1)
			{
			    blue();
			    printf("Line no: %d ", root->lexeme->line_no);
			    reset();
			    printf("Switch can only have integer or boolean type\n"); 
			}

		    }//ERROR: switch can only have integer of boolean argument

		    return;
		}break;

	    case CASESTMTS:
		{
		    //move forward 
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
		    root->type = root->children[0]->type;
		    return;
		}break;

	    case N9:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    type_semantics(root->children[i], current_table);
		    
			if(root->tok == EPS)
			    return;
			else
			{
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
			symbol_table_node* a = searchSymbolTable(current_table, root->children[1]->lexeme->str);
			if(pass_no==1&&root->children[1]->type!=integer) 
			{
			    blue();
			    printf("Line no: %d ", root->children[1]->lexeme->line_no);
			    reset();
			    printf("Loop variable must be of integer type\n"); 
			
			}//ERROR: The variable must be an integer
			if(pass_no==1 && a->isarr==true)
			{
			    blue();
			    printf("Line no: %d ", root->children[1]->lexeme->line_no);
			    reset();
			    printf("Loop iterator cannot be array variable\n"); 
			
			}//ERROR: The variable cannot be an array

			//Check Range
			type_semantics(root->children[2],current_table);

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
			
			if(pass_no==1 && !checkWhile(check_table,root->children[2]))
			{
			    blue();
			    printf("Line no: %d ", root->children[0]->lexeme->line_no);
			    reset();
			    printf("No variable in the condition of while loop is updated in the body of the loop.\n"); 

			}//ERROR: None of the variables in the while condition are updated.

			//check that conditional expression is boolean
			type_semantics(root->children[1],current_table);
			if(pass_no==1 && root->children[1]->type!=boolean)
			{
			    blue();
			    printf("Line no: %d ", root->children[0]->lexeme->line_no);
			    reset();
			    printf("Condition in the while loop must evaluate to boolean\n"); 
		    
			}//ERROR: the condition inside while must evaluate to boolean;


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

		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM))
		    {
			//dynamic range check. arg1->ID arg2->NUM
		    }
		    if((root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {
			//dynamic range check. arg1->NUM arg2->ID
		    }

		    //           INDEX1!=integer                                    INDEX2!=integer
		    if(pass_no==1&&(root->children[0]->children[0]->type!=integer || root->children[1]->children[0]->type!=integer))
		    {
			    blue();
			    printf("Line no: %d ", root->children[0]->lexeme->line_no);
			    reset();
			    printf("Arguments of range must be integers\n"); 
		    

		    }//ERROR: range can only be integer.

		    else if(pass_no==1 && 
			    (root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==NUM &&
			    (atoi(root->children[0]->children[0]->lexeme->str)>atoi(root->children[1]->children[0]->lexeme->str))))
		    {
		    
			    blue();
			    printf("Line no: %d ", root->children[0]->lexeme->line_no);
			    reset();
			  printf("Invalid range, %d is greater than %d\n",
				atoi(root->children[0]->children[0]->lexeme->str),
				atoi(root->children[1]->children[0]->lexeme->str));
		    

		    }//ERROR: Invalid range. arg1 greater then arg2.
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
			insertSymbolTable(current_table, root->lexeme->str,false, false,
				NULL,NULL,-1,-1,NULL,NONE); //add into symbol table so that seg faults dont occur
							    //and the symbol is reported only once
			if(pass_no==1)
			{
			    blue();
			    printf("Line no %d: ",root->lexeme->line_no);
			    reset();
			    printf("Symbol '%s' Not Recognized. (Each symbol is reported only once)\n",
				    root->lexeme->str);
			}
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


