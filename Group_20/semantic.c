/*
   Group 20
   Ayush Vachaspati 2016B3A70398P
   Indraneel Ghosh  2016B1A70938P
   G Adityan	 2016B1A70929P
*/

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
bool semantic_error = false;

bool checkCases(astnode* root, datatype type)
{
	astnode* head = root;
	symbolTable* table = getSymbolTable(100);
	while (root->tok != EPS)
	{
		//checking that all cases are unique
		symbol_table_node* a = searchSymbolTable(table, root->children[0]->lexeme->str);
		if (a != NULL)
		{
			blue(); semantic_error = true;
			printf("Line No %d: ", root->lexeme->line_no);
			reset();
			printf("Cases must be unique\n");
			return false;
		}
		else
		{
			insertSymbolTable(table, root->children[0]->lexeme->str, false, false,
			                  NULL, NULL, -1, -1, NULL, NONE);
		}
		root = root->children[2];
	}
	if (type == boolean)
	{
		//check that if boolean cases. then true and false are both included
		symbol_table_node* a = searchSymbolTable(table, "false");
		symbol_table_node* b = searchSymbolTable(table, "true");
		if (a == NULL || b == NULL)
		{
			blue(); semantic_error = true; semantic_error = true;
			printf("Line no %d: ", head->lexeme->line_no);
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
	while (1)
	{
		if ((input_list->tok == EPS && iplist != NULL))
		{
			blue(); semantic_error = true; semantic_error = true;
			printf("Line no %d: ", head->lexeme->line_no);
			reset();
			printf("Incorrect number of arguments in Input\n");
			return false;
		}
		if ((iplist == NULL && input_list->tok != EPS))
		{
			blue(); semantic_error = true; semantic_error = true;
			printf("Line no %d: ", head->lexeme->line_no);
			reset();
			printf("Incorrect number of arguments in Input\n");
			return false;
		}//ERROR: Number of arguments do not match the Function signature

		if (input_list->children[0]->type != iplist->type)
		{
			blue(); semantic_error = true; semantic_error = true;
			printf("Line no %d: ", input_list->lexeme->line_no);
			reset();
			printf("Argument '%s' is incompatible with the function arguments\n", input_list->children[0]->lexeme->str);
			return false;   //ERROR: Type mismatch in function argument
		}
		if (iplist->isarr == true) //verify array ranges
		{
			symbol_table_node* temp = searchSymbolTable(table, input_list->children[0]->lexeme->str);
			if (temp->isarr == false)
			{
				blue(); semantic_error = true; semantic_error = true;
				printf("Line no %d: ", input_list->lexeme->line_no);
				reset();
				printf("Argument '%s' is incompatible with the function arguments(Array Expected)\n", input_list->children[0]->lexeme->str);
				return false; //Array type argument expected for function.
			}
			/*if(temp->isdynamic==false)
			{
			if(temp->crange1!=iplist->crange1 || temp->crange2!=iplist->crange2)
			{
			    blue(); semantic_error = true;
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
			}*/
		}

		iplist = iplist->iplist;
		input_list = input_list->children[1];

		if (iplist == NULL && input_list->tok == EPS)
			return true;
	}
}

bool checkCallOutput(symbolTable* table, astnode* output_list, symbol_table_node* func)
{
	astnode* head = output_list;
	symbol_table_node* oplist = func->oplist; //checking output list (oplist)
	while (1)
	{
		if ((output_list->tok == EPS && oplist != NULL))
		{
			blue(); semantic_error = true;
			printf("Line no %d: ", head->lexeme->line_no);
			reset();
			printf("Incorrect number variable in function Output\n");
			return false;
		}
		if ((oplist == NULL && output_list->tok != EPS))
		{
			blue(); semantic_error = true;
			printf("Line no %d: ", head->lexeme->line_no);
			reset();
			printf("Incorrect number of variable in function Output\n");
			return false;
		}//ERROR: Number of arguments do not match the Function signature

		if (output_list->children[0]->type != oplist->type)
		{
			blue(); semantic_error = true;
			printf("Line no %d: ", output_list->lexeme->line_no);
			reset();
			printf("Argument '%s' is Incompatible with function output (Type Mismatch)\n", output_list->children[0]->lexeme->str);
			return false;   //ERROR: Type mismatch in function argument
		}

		oplist = oplist->oplist;
		output_list = output_list->children[1];

		if (oplist == NULL && output_list->tok == EPS)
			return true;
	}
}

void fillCheckTable(symbolTable* check_table, astnode* root)
{
	if (root->tok == ID)
		insertSymbolTable(check_table, root->lexeme->str, false, false,
		                  NULL, NULL, -1, -1, NULL, NONE);

	for (int i = 0; i < root->n; i++)
		fillCheckTable(check_table, root->children[i]);
}
bool checkForDeclare(astnode* id, astnode* root)
{
	if (root->tok == DECLARESTMT)
	{
		astnode* idlist = root->children[0];
		while (idlist->tok != EPS)
		{
			if (strcmp(id->lexeme->str, idlist->children[0]->lexeme->str) == 0)
				return false;
			idlist = idlist->children[1];
		}
	}
	else
	{
		bool ans = true;
		for (int i = 0; i < root->n; i++)
			ans = ans && checkForDeclare(id, root->children[i]);
		return ans;
	}

}

bool checkFor(astnode* id, astnode* root)
{
	if (root->tok == ASSIGNMENTSTMT)
	{
		if (strcmp(id->lexeme->str, root->children[0]->lexeme->str) == 0)
			return false;
	}
	else if (root->tok == IOSTMT && root->children[0]->tok == GET_VALUE)
	{
		if (strcmp(id->lexeme->str, root->children[1]->lexeme->str) == 0)
			return false;
	}
	else if (root->tok == OPTIONAL)
	{
		astnode* idlist = root->children[0];
		while (idlist->tok != EPS)
		{
			if (strcmp(id->lexeme->str, idlist->children[0]->lexeme->str) == 0)
				return false;
			idlist = idlist->children[1];
		}
	}
	else
	{
		bool ans = true;
		for (int i = 0; i < root->n; i++)
			ans = ans && checkFor(id, root->children[i]);
		return ans;
	}

}
bool checkWhile(symbolTable* check_table, astnode* root)
{
	if (root->tok == ASSIGNMENTSTMT)
	{
		if (searchSymbolTable(check_table, root->children[0]->lexeme->str) != NULL)
			return true;
	}
	else if (root->tok == IOSTMT && root->children[0]->tok == GET_VALUE)
	{
		if (searchSymbolTable(check_table, root->children[1]->lexeme->str) != NULL)
			return true;
	}
	else if (root->tok == OPTIONAL)
	{
		astnode* idlist = root->children[0];
		while (idlist->tok != EPS)
		{
			if (searchSymbolTable(check_table, idlist->children[0]->lexeme->str))
				return true;
			idlist = idlist->children[1];
		}
	}
	else
	{
		bool ans = false;
		for (int i = 0; i < root->n; i++)
			ans = ans | checkWhile(check_table, root->children[i]);
		return ans;
	}
}

void declareVariables(symbolTable* table, astnode* idlist, astnode* datatype)
{
	symbol_table_node* curr_func = searchSymbolTable(table, "_currentfunction")->iplist;

	astnode* dataTypeVar = datatype;
	while (idlist->tok != EPS)
	{
		if (searchSymbolTableLocal(table, idlist->children[0]->lexeme->str))
		{
			//ERROR: Redeclaration of variable that already exists
			if (pass_no == 1) {
				blue(); semantic_error = true;
				printf("Line no %d: ", idlist->children[0]->lexeme->line_no);
				reset();
				printf("Redeclaration of '%s' not allowed.\n", idlist->children[0]->lexeme->str);
			}
			idlist = idlist->children[1];
		}
		else if (dataTypeVar->children[0]->tok == INTEGER)
		{
			pass_no == 1 ? curr_func->stackSize += 4 : pass_no;
			idlist->children[0]->type = integer;
			insertSymbolTableLocal(table, idlist->children[0]->lexeme->str, false, false,
			                       NULL, NULL, -1, -1, idlist->children[0]->lexeme, integer); //integer type variable

			idlist = idlist->children[1];
		}
		else if (dataTypeVar->children[0]->tok == REAL)
		{
			pass_no == 1 ? curr_func->stackSize += 8 : pass_no;
			idlist->children[0]->type = real;
			insertSymbolTableLocal(table, idlist->children[0]->lexeme->str, false, false,
			                       NULL, NULL, -1, -1, idlist->children[0]->lexeme, real); //integer type variable
			idlist = idlist->children[1];
		}
		else if (dataTypeVar->children[0]->tok == BOOLEAN)
		{
			pass_no == 1 ? curr_func->stackSize += 4 : pass_no;
			idlist->children[0]->type = boolean;
			insertSymbolTableLocal(table, idlist->children[0]->lexeme->str, false, false,
			                       NULL, NULL, -1, -1, idlist->children[0]->lexeme, boolean); //integer type variable

			idlist = idlist->children[1];
		}
		else
		{
			//input is of type array. insert variable of type array.
			symbol_table_node* a = insertSymbolTableLocal(table, idlist->children[0]->lexeme->str, false, false,
			                       NULL, NULL, -1, -1, idlist->children[0]->lexeme, NONE); //integer type variable

			a->isarr = true; //the variable is array;
			a->type = dataTypeVar->children[1]->type; //what is the type of the array
			idlist->children[0]->type = dataTypeVar->children[1]->type;

			//static or dynamic range of array??
			//datatype->rangearrays->index->(NUM/ID)
			if (dataTypeVar->children[0]->children[0]->children[0]->tok == NUM
			        && dataTypeVar->children[0]->children[1]->children[0]->tok == NUM)
			{
				//static range of array
				a->isdynamic = false;
				a->crange1 = atoi(dataTypeVar->children[0]->children[0]->children[0]->lexeme->str); //first index
				a->crange2 = atoi(dataTypeVar->children[0]->children[1]->children[0]->lexeme->str); //second index

				//array size in memory
				if (a->type == integer)
					pass_no == 1 ? curr_func->stackSize +=  4 : pass_no; //all arrays have size 4bytes as they only store pointer
				else if (a->type == real)
					pass_no == 1 ? curr_func->stackSize +=  4 : pass_no;
				else if (a->type == boolean)
					pass_no == 1 ? curr_func->stackSize +=  4 : pass_no;

			}
			else
			{

				pass_no == 1 ? curr_func->stackSize += 4 : pass_no; //the memory is allocated on heap. hence 4 bytes to store heap location
				//dynamic range of array

				astnode* indexVar1 = dataTypeVar->children[0]->children[0]; //index nodes
				astnode* indexVar2 = dataTypeVar->children[0]->children[1];

				if (indexVar1->children[0]->tok == ID && indexVar2->children[0]->tok == ID)
				{
					//11 -> ID ID
					a->isdynamic = 3;
					a->drange1 = searchSymbolTable(table, indexVar1->children[0]->lexeme->str); //assign the symbol table entry
					a->drange2 = searchSymbolTable(table, indexVar2->children[0]->lexeme->str); //of the variable.
				}
				else if (indexVar1->children[0]->tok == NUM && indexVar2->children[0]->tok == ID)
				{
					//01 -> NUM ID
					a->isdynamic = 1;
					a->crange1 = atoi(indexVar1->children[0]->lexeme->str); //assign the symbol table entry
					a->drange2 = searchSymbolTable(table, indexVar2->children[0]->lexeme->str); //of the variable.

				}
				else if (indexVar1->children[0]->tok == ID && indexVar2->children[0]->tok == NUM)
				{
					//10  -> ID NUM
					a->isdynamic = 2;
					a->drange1 = searchSymbolTable(table, indexVar1->children[0]->lexeme->str); //assign the symbol table entry
					a->crange2 = atoi(indexVar2->children[0]->lexeme->str); //of the variable.
				}
			}

			idlist = idlist->children[1];
		}

	}
}

symbol_table_node* makeInputList(astnode* inputTree, symbolTable* table)
{
	symbol_table_node* curr_func = searchSymbolTable(table, "_currentfunction")->iplist;
	if (inputTree->tok == EPS)return NULL;

	//insert FIRST variable into inputlist

	symbol_table_node * nextinput = makeSymbolNode(inputTree->children[0]->lexeme->str,
	                                false, false, NULL, NULL, -1, -1, inputTree->children[0]->lexeme, NONE);

	//update the datatype of the variable
	astnode* dataTypeVar = inputTree->children[1];

	if (dataTypeVar->children[0]->tok == INTEGER)
	{
		type_semantics(dataTypeVar, table);
		pass_no == 1 ? curr_func->stackSize += 4 : pass_no;
		nextinput->type = integer;
	}
	else if (dataTypeVar->children[0]->tok == REAL)
	{
		type_semantics(dataTypeVar, table);
		pass_no == 1 ? curr_func->stackSize += 8 : pass_no;
		nextinput->type = real;
	}
	else if (dataTypeVar->children[0]->tok == BOOLEAN)
	{
		type_semantics(dataTypeVar, table);
		pass_no == 1 ? curr_func->stackSize += 4 : pass_no;
		nextinput->type = boolean;
	}
	else
	{
		//input is of type array.

		nextinput->isarr = true;

		//static or dynamic range of array??
		//datatype->rangearrays->index->(NUM/ID)
		if (dataTypeVar->children[0]->children[0]->children[0]->tok == NUM
		        && dataTypeVar->children[0]->children[1]->children[0]->tok == NUM)
		{
			type_semantics(dataTypeVar, table);
			nextinput->type = dataTypeVar->children[1]->type; //what isthe type of the array
			//static range of array
			nextinput->isdynamic = false;
			nextinput->crange1 = atoi(dataTypeVar->children[0]->children[0]->children[0]->lexeme->str); //first index
			nextinput->crange2 = atoi(dataTypeVar->children[0]->children[1]->children[0]->lexeme->str); //second index

			//array size in memory
			if (nextinput->type == integer)
				pass_no == 1 ? curr_func->stackSize +=  4 : pass_no;
			else if (nextinput->type == real)
				pass_no == 1 ? curr_func->stackSize +=  4 : pass_no;
			else if (nextinput->type == boolean)
				pass_no == 1 ? curr_func->stackSize +=  4 : pass_no;
		}
		else
		{
			//dynamic range of array
			nextinput->isdynamic = true;
			pass_no == 1 ? curr_func->stackSize += 4 : pass_no;


			if (dataTypeVar->children[1]->children[0]->tok == INTEGER)
				nextinput->type = integer;
			else if (dataTypeVar->children[1]->children[0]->tok == REAL)
				nextinput->type = real;
			else
				nextinput->type = boolean;

			/*
			astnode* indexVar1 = dataTypeVar->children[0]->children[0];
			astnode* indexVar2 = dataTypeVar->children[0]->children[1];

			symbol_table_node* a = nextinput;
			if(indexVar1->children[0]->tok==ID && indexVar2->children[0]->tok==ID)
			{
			//11 -> ID ID
			a->isdynamic = 3;
			a->drange1 = searchSymbolTable(table,indexVar1->children[0]->lexeme->str);
			a->drange2 = searchSymbolTable(table,indexVar2->children[0]->lexeme->str);
			}
			else if(indexVar1->children[0]->tok==NUM && indexVar2->children[0]->tok==ID)
			{
			//01 -> NUM ID
			a->isdynamic = 1;
			a->crange1 = atoi(indexVar1->children[0]->lexeme->str);
			a->drange2 = searchSymbolTable(table,indexVar2->children[0]->lexeme->str);

			}
			else if(indexVar1->children[0]->tok==ID && indexVar2->children[0]->tok==NUM)
			{
			//10  -> ID NUM
			a->isdynamic = 2;
			a->drange1 = searchSymbolTable(table,indexVar1->children[0]->lexeme->str);
			a->crange2 = atoi(indexVar2->children[0]->lexeme->str);
			}
			*/

		}
	}

	nextinput->iplist = makeInputList(inputTree->children[2], table);
	return nextinput;
}

symbol_table_node* makeOutputList(astnode* outputTree, symbolTable* table)
{

	symbol_table_node* curr_func = searchSymbolTable(table, "_currentfunction")->iplist;
	if (outputTree->tok == EPS)return NULL;

	//insert FIRST variable into inputlist

	symbol_table_node * nextoutput = makeSymbolNode(outputTree->children[0]->lexeme->str,
	                                 false, false, NULL, NULL, -1, -1, outputTree->children[0]->lexeme, NONE);

	//update the datatype of the variable
	astnode* TypeVar = outputTree->children[1];
	if (TypeVar->children[0]->tok == INTEGER)
	{
		pass_no == 1 ? curr_func->stackSize += 4 : pass_no;
		nextoutput->type = integer;
	}
	else if (TypeVar->children[0]->tok == REAL)
	{
		pass_no == 1 ? curr_func->stackSize += 8 : pass_no;
		nextoutput->type = real;
	}
	else if (TypeVar->children[0]->tok == BOOLEAN)
	{
		pass_no == 1 ? curr_func->stackSize += 4 : pass_no;
		nextoutput->type = boolean;
	}
	else
	{
		blue(); semantic_error = true;
		printf("Line no: %d ", TypeVar->children[0]->lexeme->line_no);
		reset();
		printf("Array cannot be returned from function.\n");
	}//ERROR: ARRAY OUTPUT NOT ALLOWED

	nextoutput->oplist = makeOutputList(outputTree->children[2], table);
	return nextoutput;
}

symbol_table_node* getArrVar(astnode* root, symbolTable* current_table)
{
	//finding varidnum while checking that expression only has one var
	//root is expression
	if (root->children[0]->tok == U)
		return NULL;
	root = root->children[0]->children[0];
	//root is anyterm
	if (root->children[0]->tok == BOOLCONSTT)
		return NULL;
	root = root->children[0];
	if (root->children[1]->tok != EPS)return NULL;
	root = root->children[0];
	if (root->children[1]->tok != EPS)return NULL;
	root = root->children[0];

	//factor to varidnum
	root =  root->children[0];
	if (root->tok == VAR_ID_NUM)
	{
		if (root->children[0]->tok == ID)
		{
			return searchSymbolTable(current_table, root->children[0]->lexeme->str);
		}
		else
			return NULL;
	}
	else
		return NULL;

}

bool hasInvalidArray(astnode* root, symbolTable* current_table)
{
	if (root->tok == VAR_ID_NUM)
	{
		if (root->children[0]->tok == ID && root->children[1]->tok == EPS)
		{
			root = root->children[0];
			symbol_table_node* temp = searchSymbolTable(current_table, root->lexeme->str);
			if (temp == NULL || temp->isarr)
				return true;
			else
				return false;
		}
		return false;
	}
	else
	{
		for (int i = 0; i < root->n; i++)
			if (hasInvalidArray(root->children[i], current_table))
				return true;
		return false;
	}

}

void checkModuleOutputHelper(symbol_table_node* head, astnode* root)
{
	for (int i = 0; i < root->n; i++)
		checkModuleOutputHelper(head, root->children[i]);

	if (root->tok == ASSIGNMENTSTMT)
	{
		while (head)
		{
			if (strcmp(head->name,  root->children[0]->lexeme->str) == 0)
				head->isUsed = true;

			head = head->iplist;
		}
	}
	else if (root->tok == IOSTMT && root->children[0]->tok == GET_VALUE)
	{
		while (head)
		{
			if (strcmp(head->name,  root->children[1]->lexeme->str) == 0)
				head->isUsed = true;

			head = head->iplist;
		}
	}
	else if (root->tok == OPTIONAL)
	{
		astnode* idlist = root->children[0];
		while (idlist->tok != EPS)
		{
			symbol_table_node* temp = head;
			while (temp)
			{
				if (strcmp(temp->name,  idlist->children[0]->lexeme->str) == 0)
					temp->isUsed = true;;
				temp = temp->iplist;
			}
			idlist = idlist->children[1];
		}
	}

}
bool checkModuleOutput(symbol_table_node* head, astnode* root)
{
	checkModuleOutputHelper(head, root);

	bool ans = true;
	symbol_table_node* temp = head;
	while (temp)
	{
		if (temp->isUsed == false)
			ans = false;
		temp = temp->iplist;
	}

	//reset isUsed variables to false
	while (head)
	{
		head->isUsed = false;
		head = head->iplist;
	}

	return ans;
}
void type_semantics(astnode* root, symbolTable* current_table)   //pass a table to current table in order to get it back
{
//    printf("%s\n",symbol_map[root->tok]);
	if (root == NULL)return;

	if (root->children)
	{
		switch (root->tok)
		{
		case PROGRAM:
		{
			pass_no++;
			//initialize the function table and move forward
			if (pass_no == 1)
				function_table = getSymbolTable(100);

			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			return;
		} break;

		case MODULEDECLARATIONS:
		{

			//if empty then return. else process the subtree
			if (root->tok == EPS)
				return;
			else
				for (int i = 0; i < root->n; i++)
					type_semantics(root->children[i], current_table);
			return;
		} break;

		case MODULEDECLARATION:
		{
			if (pass_no == 1) {
				//add entry for the function in the function table. and mark IsCalled = False and isDefined = false
				symbol_table_node* temp = insertSymbolTable(function_table,
				                          root->children[0]->lexeme->str , false, false, NULL, NULL,
				                          -1, -1, root->children[0]->lexeme, function); //type = function

				temp -> isUsed = false;
				temp -> isDefined = false;

				//assign function type to the ID node.
				root->children[0]->type = function;
			}
			return;
		} break;

		case OTHERMODULES:
		{
			if (root->tok == EPS)
				return;
			else
				for (int i = 0; i < root->n; i++)
					type_semantics(root->children[i], current_table);
			return;
		} break;

		case MODULE1:
		{
			symbol_table_node* temp = searchSymbolTable(function_table, root->children[0]->lexeme->str);

			//if entry already exists
			if (temp != NULL)
			{
				//function has already been declared
				if (pass_no == 1 && temp->isUsed == false)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Redundant Declaration and Definition of function\n");
				} //Error: function not used after declaration
				else if (pass_no == 1 && temp->isDefined == true)
				{
					blue(); semantic_error = true;
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
			                         false, false, NULL, NULL, -1, -1,
			                         root->children[0]->lexeme, function);


			//insert current function
			symbolTable* input_table = getSymbolTable(100); //input_list scope
			input_table->parent = current_table; //new function has no parent scope
			current_table->children[current_table->no_children++] = input_table;  //assign the child of the current table


			symbolTable* new_table = getSymbolTable(100); //table for the function scope.
			new_table->parent =  input_table;		  //function scope shadows input scope
			input_table->children[input_table->no_children++] = new_table;

			symbol_table_node * a = insertSymbolTable(input_table, "_currentfunction", false, false, NULL, NULL,
			                        -1, -1, NULL, NONE);

			a->iplist = temp;

			//add input list into function
			temp->iplist = makeInputList(root->children[1], new_table);

			//add output_plist into function
			if (root->children[2]->tok != EPS)
				temp->oplist = makeOutputList(root->children[2]->children[0], new_table);

			//function is now defined. redefinition not allowed
			temp->isDefined = true;

			//go onto moduledef with a new symbol table which has
			//the entries for input_plist vars and out_plist vars

			//insert input vars
			a = temp->iplist;
			while (a)
			{
				if (pass_no == 1 && searchSymbolTable(input_table, a->name))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Reused variable '%s' in function Input\n", a->name);
				}//ERROR: cannot have vars with same name
				insertSymbolTable(input_table, a->name, a->isarr, a->isdynamic,
				                  a->drange1, a->drange2, a->crange1, a->crange2, a->lexeme, a->type);
				a = a->iplist;
			}
			//insert output vars
			a = temp->oplist;

			if (pass_no == 1 && checkModuleOutput(a, root->children[3]) == false) //outputlist , moduledef
			{
				blue(); semantic_error = true;
				printf("Line no: %d ", root->lexeme->line_no);
				reset();
				printf("All output variables not assigned\n");
			}

			while (a)
			{
				if (pass_no == 1 && searchSymbolTable(input_table, a->name))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Reused variable '%s' in function Output\n", a->name);

				}//ERROR: cannot have vars with same name
				insertSymbolTable(input_table, a->name, a->isarr, a->isdynamic,
				                  a->drange1, a->drange2, a->crange1, a->crange2, a->lexeme, a->type);
				a = a->oplist;
			}

			//call moduledef
			type_semantics(root->children[3], new_table);

			input_table->start_line = new_table->start_line;
			input_table->end_line = new_table->end_line;
			return;
		} break;

		case DRIVERMODULE:
		{
			//create symbol table for the program
			symbolTable* new_table = getSymbolTable(100);
			new_table->parent = current_table;
			current_table->children[current_table->no_children++] = new_table;

			//function table entry for driver module
			insertSymbolTable(function_table, "driver", false, false, NULL, NULL, -1, -1, NULL, function);
			//set current funtion as the driver function
			symbol_table_node* a = insertSymbolTable(new_table, "_currentfunction", false, false, NULL, NULL,
			                       -1, -1, NULL, NONE);

			a->iplist = searchSymbolTable(function_table, "driver");

			//call moduledef
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], new_table);
			return;
		} break;


		case RET: case INPUT_PLIST: case N1: case OUTPUT_PLIST: case N2:
		{
			//handled by module1.
			return;
		} break;


		case DATATYPE:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (root->children[0]->tok != RANGE_ARRAYS)
				root->type = root->children[0]->type;
			else
			{
				root->type = root->children[1]->type;
			}
			// handle by input_list and declare statement
			return;
		} break;

		case RANGE_ARRAYS:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if ((root->children[0]->children[0]->tok == ID && root->children[1]->children[0]->tok == NUM))
			{
				//dynamic range check. arg1->ID arg2->NUM
			}
			if ((root->children[0]->children[0]->tok == NUM && root->children[1]->children[0]->tok == ID))
			{
				//dynamic range check. arg1->NUM arg2->ID
			}

			//           INDEX1!=integer                                    INDEX2!=integer
			if (pass_no == 1 && (root->children[0]->children[0]->type != integer || root->children[1]->children[0]->type != integer))
			{
				blue(); semantic_error = true;
				printf("Line no: %d ", root->children[0]->lexeme->line_no);
				reset();
				printf("Arguments of range must be integers\n");


			}//ERROR: range can only be integer.

			else if (pass_no == 1 &&
			         (root->children[0]->children[0]->tok == NUM && root->children[1]->children[0]->tok == NUM &&
			          (atoi(root->children[0]->children[0]->lexeme->str) > atoi(root->children[1]->children[0]->lexeme->str))))
			{

				blue(); semantic_error = true;
				printf("Line no: %d ", root->children[0]->lexeme->line_no);
				reset();
				printf("Invalid range, %d is greater than %d\n",
				       atoi(root->children[0]->children[0]->lexeme->str),
				       atoi(root->children[1]->children[0]->lexeme->str));

			}//ERROR: Invalid range. arg1 greater then arg2.

			else if (root->children[0]->children[0]->tok == ID && root->children[1]->children[0]->tok == ID)
			{
				//dynamic check of index1 < index2
			}

			root->type = root->children[0]->type;
			return;
		} break;

		case TYPE:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[0]->type;
			//handle by outputplist and datatype
			return;
		} break;

		case MODULEDEF:
		{

			//new symbol table is created by whatever calls moduledef

			//then pass the new table as the current_table in further calls.
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);
			return;
		} break;


		case STATEMENTS:
		{
			if (root->tok == EPS)
				return;
			else
				//move forward
				for (int i = 0; i < root->n; i++)
					type_semantics(root->children[i], current_table);

			return;
		} break;

		case STATEMENT:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);
			return;
		} break;

		case IOSTMT:
		{
			//first child stores print/get info. hence nothing needs to be done.
			//move forward to check variables.
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);
			return;
		} break;
		case GET_VALUE: case PRINT:
		{
			return;
		} break;


		case BOOLCONSTT:
		{
			//move forward then assign type
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);
			root->type = root->children[0]->type;
			return;
		} break;
		case VAR:
		{
			//move forward then assign type
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[0]->type;
			return;
		} break;

		case VAR_ID_NUM:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			//assign type
			root->type = root->children[0]->type;

			//if array statement then check bounds. WhichId is handled here.
			if (root->children[0]->tok == ID)
			{
				if (root->children[1]->tok == EPS)
					return;
				else
				{
					//search the entry for the array
					symbol_table_node* arr = searchSymbolTable(current_table, root->children[0]->lexeme->str);

					// temp = INDEX
					astnode* temp = root->children[1]->children[0];

					if (pass_no == 1 && temp->children[0]->type != integer)
					{
						blue(); semantic_error = true;
						printf("Line no: %d ", root->lexeme->line_no);
						reset();
						printf("Array index must be an integer\n");
					}	//ERROR: index must be integer


					if (arr->isdynamic != false)
					{
						//code for bound check in dynamic array.
					}
					else
					{
						if (temp->children[0]->tok == NUM)
						{
							//static check in static array and static index
							int value = atoi(temp->children[0]->lexeme->str);

							if (pass_no == 1 && (value < arr->crange1 || value > arr->crange2))
							{
								blue(); semantic_error = true;
								printf("Line no: %d ", root->lexeme->line_no);
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
		} break;


		case WHICHID:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			return;
		} break;

		case SIMPLESTMT:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);
			return;
		} break;

		case ASSIGNMENTSTMT:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			symbol_table_node* a = searchSymbolTable(current_table, root->children[0]->lexeme->str);
			astnode* lvalue = root->children[1]->children[0];

			if (lvalue->tok == LVALUEIDSTMT)
			{
				//lvalue id statement

				if (a->isarr == true)
				{
					//////////// assigning to array is only possible if other array is of the same type./////////////////
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					symbol_table_node* rightvar = getArrVar(lvalue->children[0], current_table);
					if (rightvar == NULL)
					{
						if (pass_no == 1)
						{
							blue(); semantic_error = true;
							printf("Line no: %d ", root->lexeme->line_no);
							reset();
							printf("Cannot assign to array type. Index not provided\n");
						}//ERROR: The variable is an array. Index not provided.
					}
					else
					{
						//variable is found now check if it is correct array type
						if (rightvar->isarr == false)
						{
							blue(); semantic_error = true;
							printf("Line no: %d ", root->lexeme->line_no);
							reset();
							printf("Cannot assign to array type. Index not provided\n");
						}
						else
						{
							if (rightvar->isdynamic || a->isdynamic)
							{
								//code for dynamic range checks//////////////////////////////////
								////////////////////////////////////////////////////////////////
								////////////////////////////////////////////////////////////////////
							}
							else
							{
								if (rightvar->type != a->type)
								{
									blue(); semantic_error = true;
									printf("Line no: %d ", root->lexeme->line_no);
									reset();
									printf("Array type mismatch\n");
									printf("%s: %d....%s:%d\n", rightvar->lexeme->str, rightvar->type, a->lexeme->str, a->type);

								}
								else if (rightvar->crange1 != a->crange1 || rightvar->crange2 != a->crange2)
								{
									blue(); semantic_error = true;
									printf("Line no: %d ", root->lexeme->line_no);
									reset();
									printf("Array Range Mismatch\n");

								}
							}
						}
					}
				}
				else if (pass_no == 1 && hasInvalidArray(lvalue->children[0], current_table))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Array cannot be used without index\n");
				}
				else if (pass_no == 1 && lvalue->children[0]->type != a->type)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Incompatible types in assignment\n");

				}//ERROR: type mismatch. The lavalue and rvalue have incompatible types.

			}
			else
			{
				//lvalue array statement

				if (pass_no == 1 && a->isarr == false)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("The variable is not an array. Cannot be indexed\n");

				}//ERROR: The variable is not an array. Cannot be Indexed.
				if (pass_no == 1 && lvalue->children[1]->type != a->type)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Incompatible types in assignment\n");


				}//ERROR: type mismatch. The lavalue and rvalue have incompatible types.

				// temp = INDEX
				astnode* temp = lvalue->children[0];

				if (pass_no == 1 && temp->children[0]->type != integer)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Index of array must be integer\n");


				}	//ERROR: index must be integer

				if (a->isdynamic)
				{
					//code for bound check in dynamic array.
				}
				else
				{
					if (temp->children[0]->tok == NUM)
					{
						//static check in static array and static index
						int value = atoi(temp->children[0]->lexeme->str);

						if (pass_no == 1 && (value < a->crange1 || value > a->crange2))
						{
							blue(); semantic_error = true;
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
			return;
		} break;

		case WHICHSTMT:
		{
			//move forward and assign type of child to whichstmt
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[0]->type;
			return;
		} break;

		case LVALUEIDSTMT:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			//move forward and assign type of child to lvalueidstmt
			root->type = root->children[0]->type;
			return;
		} break;

		case LVALUEARRSTMT:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[1]->type;
			return;
		} break;

		case INDEX:
		{
			//move forward and assign type
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[0]->type;
			return;
		} break;

		case MODULEREUSESTMT:
		{
			type_semantics(root->children[0], current_table);
			type_semantics(root->children[2], current_table);

			//get the function entry
			symbol_table_node* a = searchSymbolTable(function_table, root->children[1]->lexeme->str); //ID i.e function name

			//verify that the ID is really a function
			if (a == NULL)
			{
				if (pass_no == 1)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("'%s' is not a function\n", root->children[1]->lexeme->str);
				}
				return;
			}//ERROR: the ID is not a Fucntion.
			else
			{
				if (pass_no == 2 && a->isDefined == false)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("'%s' function definition missing\n", root->children[1]->lexeme->str);
				}
			}


			//the function has been called once;
			a->isUsed = true;
			//check if recursion. if yes then error
			symbol_table_node* current_func = searchSymbolTable(current_table, "_currentfunction");
			char* tempstr = current_func->iplist->name;

			if (pass_no == 1 && strcmp(a->name, tempstr) == 0)
			{
				blue(); semantic_error = true;
				printf("Line no: %d ", root->lexeme->line_no);
				reset();
				printf("The functin '%s' cannot call itself. Recursion id not allowed\n",
				       root->children[1]->lexeme->str);
			}//ERROR: Recursion Not allowed.

			if (pass_no == 2)
			{
				//verify the function signature in pass 2 when it is defined

				//verify idlist is same as function inputplist

				astnode* input_list = root->children[2];
				if (!checkCallInput(current_table, input_list, a))
				{
					//handled inside
				}//ERROR: Input parameters dont match the function called.

				//if option not eps then verify if output idlist is same as output plist also.
				if (root->children[0]->tok != EPS)
				{
					astnode* output_list = root->children[0]->children[0];
					if (!checkCallOutput(current_table, output_list, a))
					{
						//handled inside
					}//ERROR: Output Parameters dont match the function called

				}
			}
			return;
		} break;

		case OPTIONAL:
		{
			if (root->tok == EPS)
				return;
			else
				for (int i = 0; i < root->n; i++)
					type_semantics(root->children[i], current_table);
			return;

		} break;
		case IDLIST: case N3:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);
			return;
		} break;

		case EXPRESSION:
		{
			//move forward and assign type
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[0]->type;
			return;
		} break;
		case U:
		{
			//move forward and assign type
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[1]->type;  //type of NEW_NT
			return;
		} break;
		case NEW_NT:
		{
			//move forward and assign type
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[0]->type;

			return;
		} break;
		case UNARY_OP:
		{
			//do nothing
			//this is instruction for code generator to
			//take know the sign
			return;
		} break;

		case ARITHMETICORBOOLEANEXPR:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			//then check
			if (root->children[1]->tok != EPS)
			{
				if (pass_no == 1 && root->children[0]->type != boolean)
				{
					blue(); semantic_error = true;
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
		} break;

		case N7:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			//then check
			if (root->tok == EPS)
				return;
			else
			{
				if (pass_no == 1 && root->children[1]->type != boolean)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Logical operations can only have boolean operands\n");

					//ERROR: Logical Operation can only have boolean type.
				}
				root->type = boolean;
			}
			return;

		} break;
		case ANYTERM:
		{
			//move forward then check
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			//then check
			if (root->children[0]->tok == BOOLCONSTT)
				root->type = root->children[0]->type;
			else
			{
				if (root->children[1]->tok != EPS)
				{
					if (pass_no == 1 && (root->children[0]->type != integer && root->children[0]->type != real)) //only integer/real can be used in relational op
					{
						blue(); semantic_error = true;
						printf("Line no: %d ", root->lexeme->line_no);
						reset();
						printf("Relational operations can only have integer/real operands\n");
						//ERROR: Relational ops expect integer operands
					}
					else if (pass_no == 1 && root->children[0]->type != root->children[1]->type)
					{
						blue(); semantic_error = true;
						printf("Line no: %d ", root->lexeme->line_no);
						reset();
						printf("Type mismatch in relational expression\n");

					}
					root->type = boolean;
				}
				else
				{
					root->type = root->children[0]->type; //arithExpr.type
				}
			}
			return;
		} break;

		case N8:
		{
			//move forward then check
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (root->tok == EPS)
				return;
			else
			{
				if (pass_no == 1 && (root->children[1]->type != integer && root->children[1]->type != real)) //Only integer comparison allowed
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Relational operations can only have integer/real operands\n");

					//ERROR: relational Operation can only have integer type.
				}
				if (pass_no == 1 && root->children[2]->tok != EPS)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Cannot chain relational operations. Cannot compare boolean operands\n");
				}//ERROR: cannot compare boolean operands.

				root->type = root->children[1]->type;
			}
			return;
		} break;

		case ARITHMETICEXPR:
		{
			//move forward then check
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (root->children[1]->tok != EPS)
			{
				if (pass_no == 1 && root->children[0]->type == boolean)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Arithmetic operations cannot boolean operands\n");
					//ERROR: Arithmetic operations cannot have boolean oprands
				}
				if (pass_no == 1 && root->children[1]->type != NONE &&
				        (root->children[0]->type != root->children[1]->type))
				{
					blue(); semantic_error = true;
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
		} break;

		case N4:
		{
			//move forward then check
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (root->children[0]->tok == EPS)
				return;
			else
			{
				if (pass_no == 1 && (root->children[1]->type == boolean))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Arithmetic operations cannot have boolean operands\n");

					//ERROR: Arithmetic ops cannot have bool operands
				}
				if (pass_no == 1 && root->children[2]->type != NONE &&
				        (root->children[1]->type != root->children[2]->type))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Type Mismatch in expression\n");
				}//ERROR: Type mismatch in expression

				root->type = root->children[1]->type;

			}
			return;
		} break;

		case TERM:
		{
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (root->children[1]->tok != EPS)
			{
				if (pass_no == 1 && root->children[0]->type == boolean)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Arithmetic operations cannot boolean operands\n");
					//ERROR: Arithmetic operations cannot have boolean oprands
				}
				if (pass_no == 1 && root->children[1]->type != NONE &&
				        (root->children[0]->type != root->children[1]->type))
				{
					blue(); semantic_error = true;
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

		} break;

		case N5:
		{
			//move forward then check
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (root->children[0]->tok == EPS)
				return;
			else
			{
				if (pass_no == 1 && (root->children[1]->type == boolean))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Arithmetic operations cannot have boolean operands\n");

					//ERROR: Arithmetic ops cannot have bool operands
				}
				if (pass_no == 1 && root->children[2]->type != NONE &&
				        (root->children[1]->type != root->children[2]->type))
				{
					blue(); semantic_error = true;
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
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			//assign type
			root->type = root->children[0]->type;

			return;
		} break;
		case OP1:
		case OP2:
		case LOGICALOP:
		case RELATIONALOP:
		{
			//do nothing
			//this is for code generator
			return;
		} break;

		case DECLARESTMT:
		{
			//move forward
			type_semantics(root->children[1], current_table);
			declareVariables(current_table, root->children[0], root->children[1]);
			return;
		} break;

		case CONDITIONALSTMT:
		{
			//create a new scope(symbol table) and make the current table as the parent of the new table.
			symbolTable* new_table = getSymbolTable(100);
			new_table->parent = current_table;
			current_table->children[current_table->no_children++] = new_table;
			//move forward. type of ID is handled by the symbol table when it is called.


			new_table->start_line = root->lexeme->line_no;
			new_table->end_line = root->children[3]->lexeme->line_no;

			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], new_table);

			symbol_table_node* var = searchSymbolTable(current_table, root->children[0]->lexeme->str);

			//check if the type of the switch statemt matches.

			if (var->isarr == true)
			{
				if (pass_no == 1) {
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[1]->lexeme->line_no);
					reset();
					printf("Cannot have array as argument to switch\n");
				}

			}
			else if (pass_no == 1 && root->children[0]->type == integer)
			{
				if (root->children[0]->type != root->children[1]->type)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[1]->lexeme->line_no);
					reset();
					printf("Case type does not match argument type\n");

				}  //ERROR: case type does not match argument type
				if (root->children[2]->tok == EPS)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("No default case provided for integer argument\n");

				}  //ERROR: default case not included
				if (!checkCases(root->children[1], integer))
				{
					//prints inside function.
				}//cases must be unique

				//check all case statements are unique!!
			}
			else if (pass_no == 1 && root->children[0]->type == boolean)
			{
				if (root->children[0]->type != root->children[1]->type)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[1]->lexeme->line_no);
					reset();
					printf("Case type does not match argument type.\n");

				}//ERROR: case type does not match argument type
				if (root->children[2]->tok != EPS)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[2]->lexeme->line_no);
					reset();
					printf("Boolean argument does not require default case.\n");

				}  //ERROR: boolean case cannot have default case.
				if (!checkCases(root->children[1], boolean))
				{
					//prints inside function
				}//only true and false must be included in case  statement

			}
			else
			{
				if (pass_no == 1)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->lexeme->line_no);
					reset();
					printf("Switch can only have integer or boolean type\n");
				}

			}//ERROR: switch can only have integer of boolean argument

			return;
		} break;

		case CASESTMTS:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (pass_no == 1 && root->children[2]->type != NONE && (root->children[0]->type != root->children[2]->type))
			{
				blue(); semantic_error = true;
				printf("Line no: %d ", root->children[0]->lexeme->line_no);
				reset();
				printf("Case type does not match argument type.\n");
			}
			root->type = root->children[0]->type;
			return;
		} break;

		case N9:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if (root->tok == EPS)
				return;
			else
			{
				if (pass_no == 1 && root->children[2]->type != NONE && (root->children[0]->type != root->children[2]->type))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[0]->lexeme->line_no);
					reset();
					printf("Case type does not match argument type.\n");
				}

				root->type = root->children[0]->type;
			}
			return;
		} break;

		case VALUE:
		{
			//move forward
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			root->type = root->children[0]->type;
			return;
		} break;
		case DEFAULT1:
		{
			//move forward.
			//nothing else needs to be done
			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			return;
		} break;
		case ITERATIVESTMT:
		{
			if (root->children[0]->tok == FOR)
			{
				//check ID
				type_semantics(root->children[1], current_table);
				symbol_table_node* a = searchSymbolTable(current_table, root->children[1]->lexeme->str);
				if (pass_no == 1 && root->children[1]->type != integer)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[1]->lexeme->line_no);
					reset();
					printf("Loop variable must be of integer type\n");

				}//ERROR: The variable must be an integer
				if (pass_no == 1 && a->isarr == true)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[1]->lexeme->line_no);
					reset();
					printf("Loop iterator cannot be array variable\n");

				}//ERROR: The variable cannot be an array

				if (pass_no == 1 && !checkFor(root->children[1], root->children[3]))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[0]->lexeme->line_no);
					reset();
					printf("Loop variable cannot be updated in the body of the loop.\n");
				}
				if (pass_no == 1 && !checkForDeclare(root->children[1], root->children[3]))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[0]->lexeme->line_no);
					reset();
					printf("Loop variable cannot be Redeclared.\n");

				}

				//Check Range
				type_semantics(root->children[2], current_table);

				//for loop
				symbolTable* new_table = getSymbolTable(100);
				new_table->parent = current_table;
				current_table->children[current_table->no_children++] = new_table;

				new_table->start_line = root->lexeme->line_no;
				new_table->end_line = root->children[4]->lexeme->line_no;
				type_semantics(root->children[3], new_table);

			}
			else
			{
				//while loop

				//create check table
				symbolTable* check_table = getSymbolTable(100);
				astnode* exprnode = root->children[1];
				fillCheckTable(check_table, exprnode);

				if (pass_no == 1 && !checkWhile(check_table, root->children[2]))
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[0]->lexeme->line_no);
					reset();
					printf("No variable in the condition of while loop is updated in the body of the loop.\n");

				}//ERROR: None of the variables in the while condition are updated.

				//check that conditional expression is boolean
				type_semantics(root->children[1], current_table);
				if (pass_no == 1 && root->children[1]->type != boolean)
				{
					blue(); semantic_error = true;
					printf("Line no: %d ", root->children[0]->lexeme->line_no);
					reset();
					printf("Condition in the while loop must evaluate to boolean\n");

				}//ERROR: the condition inside while must evaluate to boolean;


				//create new scope(symbol table) and assign the current table as its parent
				//then move forward
				symbolTable* new_table = getSymbolTable(100);
				new_table->parent = current_table;
				current_table->children[current_table->no_children++] = new_table;
				new_table->start_line = root->lexeme->line_no;
				new_table->end_line = root->children[3]->lexeme->line_no;

				type_semantics(root->children[2], new_table);
			}
			return;
		} break;
		case RANGE:
		{

			for (int i = 0; i < root->n; i++)
				type_semantics(root->children[i], current_table);

			if ((root->children[0]->children[0]->tok == ID && root->children[1]->children[0]->tok == NUM))
			{
				//dynamic range check. arg1->ID arg2->NUM
			}
			if ((root->children[0]->children[0]->tok == NUM && root->children[1]->children[0]->tok == ID))
			{
				//dynamic range check. arg1->NUM arg2->ID
			}

			//           INDEX1!=integer                                    INDEX2!=integer
			if (pass_no == 1 && (root->children[0]->children[0]->type != integer || root->children[1]->children[0]->type != integer))
			{
				blue(); semantic_error = true;
				printf("Line no: %d ", root->children[0]->lexeme->line_no);
				reset();
				printf("Arguments of range must be integers\n");


			}//ERROR: range can only be integer.

			else if (pass_no == 1 &&
			         (root->children[0]->children[0]->tok == NUM && root->children[1]->children[0]->tok == NUM &&
			          (atoi(root->children[0]->children[0]->lexeme->str) > atoi(root->children[1]->children[0]->lexeme->str))))
			{

				blue(); semantic_error = true;
				printf("Line no: %d ", root->children[0]->lexeme->line_no);
				reset();
				printf("Invalid range, %d is greater than %d\n",
				       atoi(root->children[0]->children[0]->lexeme->str),
				       atoi(root->children[1]->children[0]->lexeme->str));


			}//ERROR: Invalid range. arg1 greater then arg2.

			else if (root->children[0]->children[0]->tok == ID && root->children[1]->children[0]->tok == ID)
			{
				//dynamic check of index1 < index2
			}

			root->type = root->children[0]->type;
			return;
		} break;
		default: return;
		}
	}
	else
	{
		switch (root->tok)
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
			symbol_table_node* temp = searchSymbolTable(current_table, root->lexeme->str);
			if (temp == NULL)
			{
				insertSymbolTable(current_table, root->lexeme->str, false, false,
				                  NULL, NULL, -1, -1, NULL, NONE); //add into symbol table so that seg faults dont occur
				//and the symbol is reported only once
				if (pass_no == 1)
				{
					blue(); semantic_error = true;
					printf("Line no %d: ", root->lexeme->line_no);
					reset();
					printf("Symbol '%s' Not Recognized. (Each symbol is reported only once)\n",
					       root->lexeme->str);
				}
			}//ERROR: symbol not recognized.

			else
				root->type = temp->type;

		} break;
		case EPS:
			root->type = NONE;
			break;
		}


	}
}

symbolTable* check_semantics(astnode* root)
{
	symbolTable* symbol_table = getSymbolTable(10);
	symbol_table = getSymbolTable(10);
	type_semantics(root, symbol_table);
	symbol_table = getSymbolTable(10);
	type_semantics(root, symbol_table);
	return symbol_table;
}
