/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
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

int no_switch = 0;
int no_while = 0;
int no_for = 0;
int output_no = 0;
int input_no = 0;
FILE* code_file;

symbolTable* top_table;

void makeCaseJumps(astnode* root,symbolTable* current_table) //also assigns the switch number to case statements
{
    root->casehandle = no_switch;
    if(root->children[0]->type == integer)
    {
	//integer switch
	astnode* id = root->children[0];
	astnode* casestmt = root->children[1];
	astnode* def = root->children[2];

	while(casestmt->tok!=EPS)
	{
	    symbol_table_node* var = searchSymbolTable(current_table,id->lexeme->str);
	    //get value of id into edx
	    fprintf(code_file,"	mov edx, [ebp+%d]\n",var->offset);
	    fprintf(code_file,"	cmp edx, %d\n", atoi(casestmt->children[0]->lexeme->str));
	    fprintf(code_file,"	je switch_%d_%d\n", no_switch, atoi(casestmt->children[0]->lexeme->str));
	    
	    casestmt->casehandle = no_switch;
	    casestmt = casestmt->children[2];
	}
	fprintf(code_file,"	jmp switch_default_%d\n",no_switch);
	def->casehandle = no_switch;
    }
    else
    {
	//boolean switch
	astnode* id = root->children[0];
	astnode* casestmt = root->children[1];
	while(casestmt->tok!=EPS)
	{
	    symbol_table_node* var = searchSymbolTable(current_table,id->lexeme->str);
	    //get value of id into edx
	    fprintf(code_file,"	mov edx, [ebp+%d]\n",var->offset);

	    if(casestmt->children[0]->tok==FALSE1){
	    fprintf(code_file,"	cmp edx,0\n");
	    fprintf(code_file,"	je switch_%d_false\n", no_switch);
	    }
	    else{
	    fprintf(code_file,"	cmp edx,1\n");
	    fprintf(code_file,"	je switch_%d_true\n",no_switch);
	    }
	    casestmt->casehandle = no_switch;
	    casestmt = casestmt->children[2];
	}

    }
}
int declareVariablesOffset(symbolTable* table, astnode* idlist, astnode* datatype, int curr_offset)
{
    astnode* dataTypeVar = datatype;
    while(idlist->tok!=EPS)
    {
	if(dataTypeVar->children[0]->tok==INTEGER)
	{
	    idlist->children[0]->type = integer;
	    symbol_table_node* temp = insertSymbolTableLocal(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, integer); //integer type variable
    
	    temp->offset = curr_offset;
	    curr_offset+=4;
	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==REAL)
	{
	    idlist->children[0]->type = real;
	    symbol_table_node* temp = insertSymbolTableLocal(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, real); //integer type variable
	    
	    temp->offset = curr_offset;
	    curr_offset+=8;
	    idlist = idlist->children[1];
	}	
	else if(dataTypeVar->children[0]->tok==BOOLEAN)
	{
	    idlist->children[0]->type = boolean;
	    symbol_table_node* temp = insertSymbolTableLocal(table,idlist->children[0]->lexeme->str,false, false,
		    NULL,NULL,-1,-1,idlist->children[0]->lexeme, boolean); //integer type variable

	    temp->offset = curr_offset;
	    curr_offset+=4;
	    idlist = idlist->children[1];
	}	
	else
	{
	    //input is of type array. insert variable of type array.
	    symbol_table_node* a = insertSymbolTableLocal(table,idlist->children[0]->lexeme->str,false, false,
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
		    curr_offset +=  4; //arrays only store pointer

		    fprintf(code_file,"\tpushad\n"
				    "\tpush dword %d\n"
				    "\tcall malloc\n"
				    "\tmov [ebp+%d],eax	;store the allocated memory pointer\n"
				    "\tpop eax\n"
				    "\tmov edi,[ebp+%d]	;base pointer to the array\n"
				    "\tmov [edi], dword %d\n"
				    "\tmov [edi+4], dword %d\n"
				    "\tpopad\n",
				    ((a->crange2 - a->crange1 + 1)*4 + 2*4),
				    a->offset,a->offset,
				    a->crange1,a->crange2);


		}
		else if(a->type==real)
		{
		    ////////// allocate the array space using malloc///////////////
		    fprintf(code_file,"ALLOCATING SPACE FOR REAL ARRAY\n");
		    a->offset = curr_offset;
		    curr_offset += 4;   //arrays only store pointer
		}
		
	    }
	    else
	    {

		a->offset = curr_offset;  //the memory is allocated on heap. memory location is stores in the current offset mem location.
		curr_offset+=4;  //4 bit pointer to the heap memory location

		//dynamic range of array
		astnode* indexVar1 = dataTypeVar->children[0]->children[0]; //index nodes
		astnode* indexVar2 = dataTypeVar->children[0]->children[1];
		
		if(indexVar1->children[0]->tok==ID && indexVar2->children[0]->tok==ID)
		{
		    //11 -> ID ID
		    a->isdynamic = 3;
		    a->drange1 = searchSymbolTable(table,indexVar1->children[0]->lexeme->str);
		    a->drange2 = searchSymbolTable(table,indexVar2->children[0]->lexeme->str);
		    
		    //both index are variables
		    if(a->type==integer || a->type == boolean)
		    {
			fprintf(code_file,"\tpushad\n"
				    "\tmov eax,[ebp+%d]	    ;value of index 2\n"
				    "\tsub eax,[ebp+%d]	    ;subtract value of index1\n"
				    "\tadd eax, 1\n"
				    "\tpush edx\n"
				    "\tmov edx,4\n"
				    "\timul edx ;eax has the result of 4*eax the multiplication\n"
				    "\tpop edx\n"
				    "\tadd eax, 8   ;space for index values\n"
				    "\tpush eax\n" 
				    "\tcall malloc\n"
				    "\tmov [ebp+%d],eax	;store the allocated memory pointer\n"
				    "\tpop eax\n"
				    "\tmov edi,[ebp+%d]	;base pointer to the array\n"
				    "\tmov eax, [ebp+%d]\n"
				    "\tmov [edi], eax\n"
				    "\tmov eax, [ebp+%d]\n"
				    "\tmov [edi+4], eax\n"
				    "\tpopad\n",
				    a->drange2->offset, a->drange1->offset,
				    a->offset,a->offset,
				    a->drange1->offset,a->drange2->offset);
		    }
		    else
		    {
			////// for real array allocate space
		    }

		}
		else if(indexVar1->children[0]->tok==ID && indexVar2->children[0]->tok==NUM)
		{
		    //10 -> ID NUM
		    a->isdynamic = 2;
		    a->drange1 = searchSymbolTable(table,indexVar1->children[0]->lexeme->str); 
		    a->crange2 = atoi(indexVar2->children[0]->lexeme->str);
		    if(a->type==integer || a->type == boolean)
		    {fprintf(code_file,"\tpushad\n"
				    "\tmov eax, dword %d    ;value of index 2\n"
				    "\tsub eax, [ebp,%d]    ;subtract value of index1\n"
				    "\tadd eax, 1\n"
				    "\tpush edx\n"
				    "\tmov edx,4\n"
				    "\timul edx ;eax has the result of 4*eax the multiplication\n"
				    "\tpop edx\n"
				    "\tadd eax, 8   ;space to store indexes\n"
				    "\tpush eax\n" 
				    "\tcall malloc\n"
				    "\tmov [ebp+%d],eax	;store the allocated memory pointer\n"
				    "\tpop eax\n"
				    "\tmov edi,[ebp+%d]	;base pointer to the array\n"
				    "\tmov eax, [ebp+%d]\n"
				    "\tmov [edi], eax\n"
				    "\tmov [edi+4], dword %d\n"
				    "\tpopad\n",
				    a->crange2, a->drange1->offset,
				    a->offset,a->offset,
				    a->drange1->offset,a->crange2);

		    }
		    else
		    {
			//allocate space for real array
		    }
		}
		else if(indexVar1->children[0]->tok==NUM && indexVar2->children[0]->tok==ID)
		{
		    //01 -> NUM ID
		    a->isdynamic = 1;
		    a->crange1 = atoi(indexVar1->children[0]->lexeme->str); 
		    a->drange2 = searchSymbolTable(table,indexVar2->children[0]->lexeme->str); 
		    if(a->type==integer || a->type == boolean)
		    {


			fprintf(code_file,"\tpushad\n"
				    "\tmov eax,[ebp+%d]	;value of index 2\n"
				    "\tsub eax, dword %d    ;subtract value of index1\n"
				    "\tadd eax, 1\n"
				    "\tpush edx\n"
				    "\tmov edx,4\n"
				    "\timul edx ;eax has the result of 4*eax the multiplication\n"
				    "\tpop edx\n"
				    "\tadd eax, 8   ;space to store indexes\n"
				    "\tpush eax\n" 
				    "\tcall malloc\n"
				    "\tmov [ebp+%d],eax	    ;store the allocated memory pointer\n"
				    "\tpop eax\n"
				    "\tmov edi,[ebp+%d]	    ;base pointer to the array\n"
				    "\tmov [edi], dword %d\n"
				    "\tmov eax, [ebp+%d]\n"
				    "\tmov [edi+4], eax\n"
				    "\tpopad\n",
				    a->drange2->offset, a->crange1,
				    a->offset,a->offset,
				    a->crange1,a->drange2->offset);


		    }
		    else
		    {
			//allocate space for real array
		    }
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
		    fprintf(code_file,"extern malloc\n"
			    "extern printf\n"
			    "extern scanf\n"
			    "extern exit\n"
			    "\nSECTION .data\n"
			    "bound_error_str: db 'RUNTIME ERROR: Array Index Out of Bounds',10,0\n"
			    "input_arr_int: db 'Input: Enter %%d array elemts of integer type for range %%d to %%d',10,0\n"
			    "input_arr_boolean: db 'Input: Enter %%d array elemts of boolean type for range %%d to %%d',10,0\n"
			    "input_str_int: db 'Input: Enter an integer value',10,0\n"
			    "input_str_boolean: db 'Input: Enter a boolean value',10,0\n"
			    "input_format_int: db '%%d',0\n"
			    "true_output: db 'Output: true',10,0\n"
			    "false_output: db 'Output: false',10,0\n"
			    "integer_output: db 'Output: %%d',10,0\n"
			    "output_str: db 'Output: ',0\n"
			    "single_int: db '%%d ',0\n"
			    "single_false: db 'false ',0\n"
			    "single_true: db 'true ',0\n"
			    "nextline: db 10,0\n"
			    "\nSECTION .text\n"
			    "bits 32\n"
			    "global main\n\n");
		    //move forward
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,0); //the program 
		  

		    fprintf(code_file, "BOUND_ERROR:\n"
					"\tpushad\n"  
					"\tpush dword bound_error_str\n"  
					"\tcall printf\n"  
					"\tpop eax\n"  
					"\tpopad\n"  
					"\tcall exit\n");  
		    //no code generation
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
		    
		    //no code generation
		    return curr_offset;
		}break;

	    case MODULEDECLARATION:
		{
		    //no code generations
		    return curr_offset;
		}break;

	    case OTHERMODULES:
		{
		    if(root->tok == EPS)
			return curr_offset;
		    else
			for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,0);
		    
		    //no code generation
		    return curr_offset;
		}break;

	    case MODULE1:
		{
		    symbol_table_node* temp = searchSymbolTable(function_table, root->children[0]->lexeme->str);
		    
			///////// add the function definition/signature to the function table.///////////
			
		
			symbolTable* input_table = getSymbolTable(100); //input_list scope
			input_table->parent = top_table; //new function has no parent scope
			top_table->children[top_table->no_children++] =input_table;

			symbol_table_node* x = insertSymbolTable(input_table, "_currentfunction",0,0,NULL,NULL,-1,-1,NULL,NONE);
			x->iplist = searchSymbolTable(function_table, root->children[0]->lexeme->str);

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
				curr_offset += 4;
			    else if(input_var->type==real)
				curr_offset += 4;
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
			symbol_table_node* output_var = insertSymbolTable(input_table, a->name,a->isarr,a->isdynamic,
				a->drange1,a->drange2,a->crange1,a->crange2,a->lexeme,a->type);
			
			output_var->offset = curr_offset;

			if(output_var->isarr == true)
			{
			    //if array input variable
			    if(output_var->type==integer || output_var->type==boolean)
				curr_offset += 4;
			    else if(output_var->type==real)
				curr_offset += 4;
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
		    
		    
		    
		    ///////// produce code for procedure in asm /////////////////////
		    ////////////////////////////////////////////////////////////////
		    fprintf(code_file,"%s:\n",temp->name);
		    fprintf(code_file,"\tpushad\n");
		    
		    //call moduledef to produce content of the procedure
		    curr_offset = codegen(root->children[3],new_table, curr_offset);
		    

		    /////////// code for end of the procedure //////////////////
		    ////////////////////////////////////////////////////////////
		    fprintf(code_file,"\tpopad\n");
		    fprintf(code_file,"ret\n");
		    //fprintf(code_file,"%s endp\n\n",temp->name);
		    return curr_offset; /////////// ################### this is changed after testing for offsets ####################
					//				therefore it may lead to errors.
		}break;

	    case DRIVERMODULE:
		{
		    //create symbol table for the program
		    symbolTable* new_table = getSymbolTable(100);
		    new_table->parent = top_table;
		    top_table->children[top_table->no_children++]=new_table;
		    symbol_table_node* x = insertSymbolTable(new_table, "_currentfunction",0,0,NULL,NULL,-1,-1,NULL,NONE);
		    x->iplist = searchSymbolTable(function_table, "driver");

  
		    ///// produce code to define the main procedure in asm//////////////
		    fprintf(code_file,"main:\n");
		    fprintf(code_file,"	sub esp,%d  ;allocating space on the stack\n",searchSymbolTable(function_table,"driver")->stackSize);
		    fprintf(code_file,"\tmov ebp, esp	;ebp accesses upwards, while stack grows downwards\n");
		    //call moduledef
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], new_table,0);
		    
		    //////// produce code to finish the main procudure in asm //////////////
		    fprintf(code_file,"exit_main:  call exit\n\n");
		    return curr_offset;
		}break;

	    
	    case RET: case INPUT_PLIST: case N1: case OUTPUT_PLIST: case N2:
		{
		    //handled by module1.
		    //no code produced
		    return curr_offset;    
		}break;


	    case DATATYPE:
		{

		    //no code produced

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
		    
		    ///////////// produce code for dynamic range check ///////////////

		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM))
		    {
			////////dynamic range check. arg1->ID arg2->NUM
		    }
		    if((root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {
			///////dynamic range check. arg1->NUM arg2->ID
		    }
		    else if(root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==ID)
		    {
			////////dynamic check of index1 < index2
		    }

		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case TYPE:
		{
		    // no code produced
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;
	    
	    case MODULEDEF:
		{
		    // produce code by moving forward
		    //move forward
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;


	    case STATEMENTS:
		{
		    //produce code by moving to statement
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
		    //produce code by moving forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;

	    case IOSTMT:
		{

		    //move forward to check variables and assign offsets.
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset); 
		    


		    //edx has the value of var_id_num depending on if array or whatever
		    if(root->children[0]->tok==PRINT)
		    {
			astnode* var_id_num = root->children[1]->children[0];
			
			if(var_id_num->tok == BOOLCONSTT)
			{
			    if(var_id_num->children[0]->tok==FALSE1)
			    {
				fprintf(code_file,  "\tpushad\n"
						    "\tpush dword false_output\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n");
			    }
			    else
			    {
				//print output: true
				fprintf(code_file,  "\tpushad\n"
						    "\tpush dword true_output\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n");

			    }
			}
			else
			{
			    if(var_id_num->children[0]->tok == NUM)
			    {
				fprintf(code_file,  "\tpushad\n"
						    "\tpush edx\n"
						    "\tpush dword integer_output\n"
						    "\tcall printf\n"
						    "\tadd esp, 8\n"
						    "\tpopad\n");
			    }
			    else if(var_id_num->children[0]->tok==RNUM)
			    {
				fprintf(code_file,"OUTPUT FLOATING POINT NUMBER\n");
			    }
			    else
			    {
				symbol_table_node* var = searchSymbolTable(current_table,
							    var_id_num->children[0]->lexeme->str);
				if(var_id_num->children[1]->tok == EPS)
				{
				    //variable or whole array. pointer in edx
				    if(var->isarr)
				    {
					output_no++;
					//print the whole array somehow
					if(var->type==integer)
					{
					    fprintf(code_file,"\tpushad\n"
						    "\tpush dword output_str\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n");

					    fprintf(code_file,"\tpushad\n"
						    "\tmov edi, [ebp+%d]\n"
						    "\tmov ebx, [edi]\n"
						    "\tmov eax, [edi+4]\n"
						    "\tsub eax, ebx\n"
						    "\tmov ecx, 0\n"
						    "OUTPUT_LABEL_%d:\n"
						    "\tmov ebx, [edi+ecx*4+2*4]\n"
						    "\tpushad\n"
						    "\tpush ebx\n"
						    "\tpush dword single_int\n"
						    "\tcall printf\n"
						    "\tadd esp, 8\n"
						    "\tpopad\n"
						    "\tadd ecx,1\n"
						    "\tcmp ecx,eax\n"
						    "\tjle OUTPUT_LABEL_%d\n"
						    "\tpopad\n", var->offset,output_no,output_no); 

					    fprintf(code_file,	"\tpushad\n"
						    "\tpush dword nextline\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n");
					}
					else if(var->type == boolean)
					{
					     fprintf(code_file,"\tpushad\n"
						    "\tpush dword output_str\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n");

					    fprintf(code_file,"\tpushad\n"
						    "\tmov edi, [ebp+%d]\n"
						    "\tmov ebx, [edi]\n"
						    "\tmov eax, [edi+4]\n"
						    "\tsub eax, ebx\n"
						    "\tmov ecx, 0\n"
						    "OUTPUT_LABEL_%d:\n"
						    "\tmov ebx, [edi+ecx*4+2*4]\n"
						    "\tpushad\n"
						    "\tcmp ebx, 0\n"
						    "\tmov eax, dword single_false\n"
						    "\tmov edx, single_true\n"
						    "\tcmove ebx, eax\n"
						    "\tcmovne ebx, edx\n"
						    "\tpush ebx\n"
						    "\tcall printf\n"
						    "\tpop ebx\n"
						    "\tpopad\n"
						    "\tadd ecx,1\n"
						    "\tcmp ecx,eax\n"
						    "\tjle OUTPUT_LABEL_%d\n"
						    "\tpopad\n", var->offset,output_no,output_no); 

					    fprintf(code_file,	"\tpushad\n"
						    "\tpush dword nextline\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n");
   
					}
					else
					{
					    fprintf(code_file,"OUTPUT REAL ARRAY\n");
					}

				    }
				    else
				    {
					if(var->type == integer)
					{
					    //printing an integer variable, value in edx
					    fprintf(code_file,  "\tpushad\n"
						    "\tpush edx\n"
						    "\tpush dword integer_output\n"
						    "\tcall printf\n"
								"\tadd esp, 8\n"
								"\tpopad\n");
					}
					else if(var->type== real)
					    fprintf(code_file,"OUTPUT FLOAT NUMBER\n");
					else
					{
					    //printing a boolean variable. value in edx

					    fprintf(code_file,	"\tpushad\n"
								"\tpush dword output_str\n"
								"\tcall printf\n"
								"\tpop eax\n"
								"\tpopad\n");

					    fprintf(code_file,"\tpushad\n"
						    "\tcmp edx,0\n"
						    "\tmov edx, single_false\n"
						    "\tcmove eax, edx\n"
						    "\tmov edx, single_true\n"
						    "\tcmovne eax, edx\n"
						    "\tpush eax\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n"); 
					    
					    fprintf(code_file,	"\tpushad\n"
								"\tpush dword nextline\n"
								"\tcall printf\n"
								"\tpop eax\n"
								"\tpopad\n");
					}
				    }
				}
				else 
				{
				    //print array element value in edx
					if(var->type == integer)
					{
					    //printing an integer variable, value in edx
					    fprintf(code_file,  "\tpushad\n"
								"\tpush edx\n"
								"\tpush dword integer_output\n"
								"\tcall printf\n"
								"\tadd esp, 8\n"
								"\tpopad\n");
					}
					else if(var->type== real)
					    fprintf(code_file,"OUTPUT FLOAT NUMBER\n");
					else
					{
					    //printing a boolean variable. value in edx
					    fprintf(code_file,	"\tpushad\n"
								"\tpush dword output_str\n"
								"\tcall printf\n"
								"\tpop eax\n"
								"\tpopad\n");
					    
					    fprintf(code_file,"\tpushad\n"
						    "\tcmp edx,0\n"
						    "\tmov edx, single_false\n"
						    "\tcmove eax, edx\n"
						    "\tmov edx, single_true\n"
						    "\tcmovne eax, edx\n"
						    "\tpush eax\n"
						    "\tcall printf\n"
						    "\tpop eax\n"
						    "\tpopad\n"); 
					    
					    fprintf(code_file,	"\tpushad\n"
								"\tpush dword nextline\n"
								"\tcall printf\n"
								"\tpop eax\n"
								"\tpopad\n");
					}
				}
			    }
			}
		    }
		    else
		    {
			//get the variable from table
			symbol_table_node* var = searchSymbolTable(current_table, root->children[1]->lexeme->str);

			if(var->isarr)
			{
			    //get input for the whole array
			    input_no++;
			    //print the whole array somehow
			    if(var->type==integer)
			    {
				fprintf(code_file,"\tpushad\n"
					"\tmov edi, [ebp+%d]\n"
					"\tpush dword [edi+4]\n"
					"\tpush dword [edi]\n"
					"\tmov eax, [edi+4]\n"
					"\tsub eax, [edi]\n"
					"\tadd eax, 1\n"
					"\tpush eax\n"
					"\tpush dword input_arr_int\n"
					"\tcall printf\n"
					"\tadd esp, 16\n"
					"\tpopad\n" , var->offset);

				fprintf(code_file,"\tpushad\n"
					"\tmov edi, [ebp+%d]\n"
					"\tmov ebx, [edi]\n"
					"\tmov eax, [edi+4]\n"
					"\tsub eax, ebx\n"
					"\tmov ecx, 0\n"
					"\tadd edi, 4\n"
					"INPUT_LABEL_%d:\n"
					"\tadd edi, 4\n"
					"\tpushad\n"
					"\tpush edi\n"
					"\tpush dword input_format_int\n"
					"\tcall scanf\n"
					"\tadd esp, 8\n"
					"\tpopad\n"
					"\tadd ecx,1\n"
					"\tcmp ecx,eax\n"
					"\tjle INPUT_LABEL_%d\n"
					"\tpopad\n", var->offset,input_no,input_no); 

			    }
			    else if(var->type==boolean)
			    {
				    fprintf(code_file,"\tpushad\n"
					"\tmov edi, [ebp+%d]\n"
					"\tpush dword [edi+4]\n"
					"\tpush dword [edi]\n"
					"\tmov eax, [edi+4]\n"
					"\tsub eax, [edi]\n"
					"\tadd eax, 1\n"
					"\tpush eax\n"
					"\tpush dword input_arr_boolean\n"
					"\tcall printf\n"
					"\tadd esp, 16\n"
					"\tpopad\n" , var->offset);


				fprintf(code_file,"\tpushad\n"
					"\tmov edi, [ebp+%d]\n"
					"\tmov ebx, [edi]\n"
					"\tmov eax, [edi+4]\n"
					"\tsub eax, ebx\n"
					"\tmov ecx, 0\n"
					"\tadd edi, 4\n"
					"INPUT_LABEL_%d:\n"
					"\tadd edi, 4\n"
					"\tpushad\n"
					"\tpush edi\n"
					"\tpush dword input_format_int\n"
					"\tcall scanf\n"
					"\tadd esp, 8\n"
					"\tpopad\n"
					"\tadd ecx,1\n"
					"\tcmp ecx,eax\n"
					"\tjle INPUT_LABEL_%d\n"
					"\tpopad\n", var->offset,input_no,input_no); 

			    }


			}
			else
			{
			    //get input for single variable
			    if(var->type==integer)
			    {
				fprintf(code_file,"\tpushad\n"
					"\tpush dword input_str_int\n"
					"\tcall printf\n"
					"\tpop eax\n"
					"\tpopad\n");

				fprintf(code_file,"\tpushad\n"
					"\tmov eax, ebp\n"
					"\tadd eax, %d\n"    //offset of the variable to be read
					"\tpush eax\n"
					"\tpush dword input_format_int\n"
					"\tcall scanf\n"
					"\tadd esp, 8\n"
					"\tpopad\n", var->offset);
			    }
			    else if(var->type==real)
			    {
				fprintf(code_file,"TAKE INPUT FOR REAL VARIABLE\n");
			    }
			    else if(var->type == boolean)
			    {
				fprintf(code_file,"\tpushad\n"
					"\tpush dword input_str_boolean\n"
					"\tcall printf\n"
					"\tpop eax\n"
					"\tpopad\n");
				
				fprintf(code_file,"\tpushad\n"
					"\tmov eax, ebp\n"
					"\tadd eax, %d\n"    //offset of the variable to be read
					"\tpush eax\n"
					"\tpush dword input_format_int\n"
					"\tcall scanf\n"
					"\tadd esp, 8\n"
					"\tpopad\n",var->offset);

			    }
			}
		    }
		    return curr_offset;
		}break;
	    case GET_VALUE: case PRINT:
		{
		    //handled by IOSTMT
		    return curr_offset;
		}break;


	    case BOOLCONSTT:
		{
		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    root->type = root->children[0]->type;
		    
		    /// code produced by caller (var/anyterm) /////////////
		    
		    return curr_offset;
		}break;
	    case VAR:
		{
		    /////////////////////////////////////////////////////////////////////////
		    /////// WILL THIS CALL VAR_ID_NUM???? //////////////////////////////////
		    //////////////////////////////////////////////////////////////////////////

		    //move forward then assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;
		    /////////////////////////////////////////////////////////////////////////////
		    /// code produced according to print or get value requirements /////////////
		    ////////////////////////////////////////////////////////////////////////////
		    //
		    //
		    return curr_offset;
		}break;

	    case VAR_ID_NUM:
		{
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		   
		    //place the variable value into edx for further computation

		    if(root->children[0]->tok==NUM)
		    {
			fprintf(code_file,"	mov edx, %s\n",root->children[0]->lexeme->str);
		    }
		    else if(root->children[0]->tok==RNUM)
		    {
			fprintf(code_file,"	mov edx, %s\n",root->children[0]->lexeme->str);
		    }
		    else if(root->children[0]->tok==ID)
		    {
			symbol_table_node* var = searchSymbolTable(current_table, root->children[0]->lexeme->str);
			if(var->isarr==true)
			{

			    /////////// ONLY HANDLES INTEGER TYPE //////////////
			    /////////// SEPERATE CODE FOR REAL NEEDED /////////

			    if(root->children[1]->tok==EPS)
				fprintf(code_file,"	mov edx, [ebp+%d]\n",var->offset);  //pointer copy
			    else
			    {
				if(var->type == integer || var->type==boolean)
				{
				    astnode* index = root->children[1]->children[0];
				    if(index->children[0]->tok == NUM)
				    {
					//move base of array into edi
					fprintf(code_file,"	mov edi, [ebp+%d]\n",var->offset); //moving data from array element
					//move index into esi
					fprintf(code_file,"	mov esi, %d\n", atoi(index->children[0]->lexeme->str));
					
					//check bounds
					fprintf(code_file, "\tcmp esi,[edi]\n"
							    "\tjl BOUND_ERROR\n"
							    "\tcmp esi,[edi+4]\n"
							    "\tjg BOUND_ERROR\n");

					fprintf(code_file,"\tsub esi, [edi]	;subtract the base index\n");
					//move element from array to edx. 2*4 included as first 2 elements store range
					fprintf(code_file,"	mov edx, [edi+esi*4+2*4]\n");
				    }
				    else
				    {
					symbol_table_node* temp_index = searchSymbolTable(current_table,
						index->children[0]->lexeme->str);
					//move base of array into edi
					fprintf(code_file,"	mov edi, [ebp+%d]\n",var->offset); //moving data from array element
					//move index into esi
					fprintf(code_file,"	mov esi, [ebp+%d]\n", temp_index->offset);
					
					//check bounds
					fprintf(code_file, "\tcmp esi,[edi]\n"
							    "\tjl BOUND_ERROR\n"
							    "\tcmp esi,[edi+4]\n"
							    "\tjg BOUND_ERROR\n");

					fprintf(code_file,"\tsub esi, [edi]	;subtract the base index\n");
					//move element from array to edx. 2*4 included as first 2 elements store range
					fprintf(code_file,"	mov edx, [edi+esi*4+2*4]\n");
				    }
				}
				else
				{
				    /////// code required for real array access
				}
			    }
			}
			else
			{
			    fprintf(code_file,"	mov edx, [ebp+%d]\n",var->offset); // copy data from offset of variable
			}
		    }

		    //assign type
		    root->type = root->children[0]->type;
		    
		    return curr_offset;
		}break;
	    
		
	    case WHICHID:
		{
		    //no code produced
		    //move forward
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    return curr_offset;
		}break;

	    case SIMPLESTMT:
		{
		    //produce code by moving forward
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;

	    case ASSIGNMENTSTMT:
		{
		    //move forward to evaluate expression
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		  
		    symbol_table_node* a = searchSymbolTable(current_table, root->children[0]->lexeme->str);
		    astnode* lvalue = root->children[1]->children[0];

		    
		    if(lvalue->tok==LVALUEIDSTMT)
		    {
			//////// code for assigning result of expression to ID
			//////// if ID is an array then array assignment is required
			symbol_table_node* var = searchSymbolTable(current_table, root->children[0]->lexeme->str);
			if(var->isarr==true)
			{
			    //assign array pointer. i.e copy the address of the array
			    fprintf(code_file,"\tmov [ebp+%d],edx   ;assign pointer of the array\n",var->offset);
			}
			else
			{
			   fprintf(code_file,"\tmov [ebp+%d], edx   ;assign value to a variable\n",var->offset);
			}
		    }
		    else
		    {
			//code for assigning result of expression to Array element
			symbol_table_node* var = searchSymbolTable(current_table, root->children[0]->lexeme->str);
			// temp = INDEX
			astnode* index = lvalue->children[0];


			//////////// CODE ONLY FOR INTEGER TYPE /////////////
			//////////// NEED CODE FOR REAL TYPE ///////////

			if(index->children[0]->tok == NUM)
			{
			    fprintf(code_file,"\tmov edi,[ebp+%d]   ;edi has base address of array\n",var->offset);
			    fprintf(code_file,"\tmov esi,%d\n",atoi(index->children[0]->lexeme->str));
			    
			    //check bounds
			    fprintf(code_file, "\tcmp esi,[edi]\n"
				    "\tjl BOUND_ERROR\n"
				    "\tcmp esi,[edi+4]\n"
				    "\tjg BOUND_ERROR\n");

			    fprintf(code_file,"\tsub esi, [edi]  ;subtract base index of the array\n");
			    fprintf(code_file,"\tmov [edi+4*esi+2*4],edx	;first 2 bytes store the bounds\n");
			}
			else
			{
			    symbol_table_node* index_var = searchSymbolTable(current_table,index->children[0]->lexeme->str);
			    fprintf(code_file,"\tmov esi, [ebp+%d]  ;place value of index var\n",index_var->offset);
			    fprintf(code_file,"\tmov edi,[ebp+%d]   ;edi has base address of array\n",var->offset);
			    
			    //check bounds
			    fprintf(code_file, "\tcmp esi,[edi]\n"
				    "\tjl BOUND_ERROR\n"
				    "\tcmp esi,[edi+4]\n"
				    "\tjg BOUND_ERROR\n");

			    fprintf(code_file,"\tsub esi, [edi]  ;subtract base index of the array\n");
			    fprintf(code_file,"\tmov [edi+4*esi+2*4],edx    ;first 2 bytes store the bounds\n");

			}
		    }
		    return curr_offset;
		}break;

	    case WHICHSTMT:
		{
		    //no code produced
		    //move forward and assign type of child to whichstmt
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}

	    case LVALUEIDSTMT:
		{

		    //no code produced
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    //move forward and assign type of child to lvalueidstmt
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case LVALUEARRSTMT:
		{

		    //no code produced
		    for(int i =0;i<root->n;i++)
			curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[1]->type;
		    return curr_offset;
		}break;
	    
	    case INDEX:
		{
		    //no code produced
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case MODULEREUSESTMT:
		{
		    //////// produce code to call the function ////////////
		    // allocate space of the stack. save the base pointer then store the input and output parameters.
		    // then change base pointer
		    
		    astnode* input_list = root->children[2];
		    symbol_table_node* func_node = searchSymbolTable(function_table,root->children[1]->lexeme->str);
		    
		    fprintf(code_file,"\tsub esp, %d\n", func_node->stackSize);

		    int offset = 0;
		    while(input_list->tok!=EPS)
		    {
			//copy all the input parameters
			symbol_table_node* temp = searchSymbolTable(current_table,input_list->children[0]->lexeme->str);
			fprintf(code_file,  "\tmov edx, [ebp+%d]\n"
					    "\tmov  [esp+%d], edx\n", temp->offset, offset);	
			if(temp->isarr || temp->type==integer || temp->type == boolean)
			    offset+=4;
			else
			    offset+=8;

			input_list = input_list->children[1];
		    }
		    
		    int output_offset = offset;
		    if(root->children[0]->tok!=EPS)
		    {	astnode* output_list = root->children[0]->children[0];
			while(output_list->tok!=EPS)
			{
			    //copy all the output parameters into the call stack
			    symbol_table_node* temp = searchSymbolTable(current_table,output_list->children[0]->lexeme->str);
			    fprintf(code_file,  "\tmov edx, [ebp+%d]\n"
				    "\tmov  [esp+%d], edx\n", temp->offset, offset);	
			    if(temp->isarr || temp->type==integer || temp->type == boolean)
				offset+=4;
			    else
				offset+=8;

			    output_list = output_list->children[1];
			}
		    }

		    fprintf(code_file, "\tpush ebp\n"
					"\tmov ebp, esp\n"
					"\tadd ebp, 4\n"
					"\tcall %s\n"
				       "\tpop ebp\n",	func_node->name);

		    
		    //return the values of the function
		    if(root->children[0]->tok!=EPS)
		    {
			astnode* output_list = root->children[0]->children[0];
			while(output_list->tok!=EPS)
			{
			    symbol_table_node* temp = searchSymbolTable(current_table,output_list->children[0]->lexeme->str);
			    fprintf(code_file,  "\tmov edx, [esp+%d]\n"
				    "\tmov  [ebp+%d], edx\n", output_offset, temp->offset);	
			    if(temp->isarr || temp->type==integer || temp->type == boolean)
				output_offset+=4;
			    else
				output_offset+=8;

			    output_list = output_list->children[1];

			}	

		    }
		    fprintf(code_file, "\tadd esp, %d\n", func_node->stackSize);
		    return curr_offset;	
		}break;

	    case OPTIONAL:
		{
		    // no code produced
		    if(root->tok==EPS)
			return curr_offset;
		    else
			for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;

		}
	    case IDLIST: case N3:
		{
		    // no code produced
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    return curr_offset;
		}break;
	    
	    case EXPRESSION:
		{
		    // produce code by moving forward
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
		    
		    /////// produce code to negate the answer if necessary //////////
		    if(root->children[0]->children[0]->tok==MINUS)
			fprintf(code_file,"	neg edx\n");
		    else
			fprintf(code_file,"	mov edx,edx\n");

		    return curr_offset;
		}break;
	    case NEW_NT:
		{
		    // produce code by moving forward
		    //move forward and assign type
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);
		    
		    root->type = root->children[0]->type;

		    return curr_offset;
		}break;
	    case UNARY_OP:
		{
		    // no code produced. Handled by "U"
		    return curr_offset;
		}break;
	    
	    case ARITHMETICORBOOLEANEXPR:
		{
		    //PROCEDURE FOR EXPRESSION
		    /* solve anyterm ans stored in edx
		     * if N7 == EPS then return (ans in edx)
		     * else mov eax, edx (save ans of anyterm in eax)
		     * solve N7 (ans in edx)
		     * then perform   OP edx,eax
		     * return (ans of the arithboolexpr is now in edx
		     */


		    //move forward
		    curr_offset = codegen(root->children[0], current_table,curr_offset); //evaluate anyterm

		    if(root->children[1]->tok==EPS)
		    {
			curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N7
			//do nothing as answer is already in edx
		    }
		    else
		    {
			fprintf(code_file,"	push edx\n");

			//evaluate N7
			curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N7
		    
			fprintf(code_file,"	pop eax\n");
			if(root->children[1]->children[0]->children[0]->tok==AND)
			    fprintf(code_file,"	and edx, eax\n");
			else
			    fprintf(code_file,"	or edx, eax\n");
		    }
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
		    // use procedure for expression
		    //move forward 
		    //then check 
		    if(root->tok==EPS)
			return curr_offset;
		    else
		    {
			curr_offset = codegen(root->children[0], current_table,curr_offset);
			curr_offset = codegen(root->children[1], current_table,curr_offset);

			if(root->children[2]->tok==EPS)
			{
			    curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N7 empty
			    //do nothing as answer is already in edx
			}
			else
			{
			    fprintf(code_file,"	push edx \n");

			    //evaluate N7
			    curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N7

			    fprintf(code_file,"	pop eax\n");
			    if(root->children[2]->children[0]->children[0]->tok==AND)
				fprintf(code_file,"	and edx, eax\n");
			    else
				fprintf(code_file,"	or edx, eax\n");

			}


			root->type = boolean;
		    }
		    return curr_offset;

		}break;
	    case ANYTERM:
		{
		    
		    // use procedure for expression
		    //move forward then check
     
		    //then check
		   if(root->children[0]->tok == BOOLCONSTT)
		   {
		      curr_offset = codegen(root->children[0], current_table,curr_offset); //evaluate boolconst
		      
		      ////////// code for bool constant
		      if(root->children[0]->children[0]->tok==FALSE1)
			  fprintf(code_file,"	mov edx, 0   ; assigning false\n");
		      else
			  fprintf(code_file,"	mov edx, 1   ; assigning true\n");
		      root->type = root->children[0]->type;
		   }
		   else
		   {
		       curr_offset = codegen(root->children[0], current_table,curr_offset); //evaluate arithmetic expr

		       if(root->children[1]->tok==EPS)
		       {
			   curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N8 emtpy
			   //do nothing as answer is already in edx
		       }
		       else
		       {
			   fprintf(code_file,"	push edx \n");

			   //evaluate N8
			   curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N8

			   fprintf(code_file,"	pop eax\n");
			   fprintf(code_file,"	cmp eax,edx\n");  //compare eax and edx. Performas eax - edx

			    fprintf(code_file,"	mov edx,0   ;assign false initially in comparing\n"
					    "\tmov eax, 1   ;cmov requires register, therefore place true in eax\n");
			    
			   if(root->children[1]->children[0]->children[0]->tok==LT)
			       fprintf(code_file,"	cmovl  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==LE)
			       fprintf(code_file,"	cmovle  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==GT)
			       fprintf(code_file,"	cmovg  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==GE)
			       fprintf(code_file,"	cmovge  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==EQ)
			       fprintf(code_file,"	cmove  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==NE)
			       fprintf(code_file,"	cmovne  edx , eax\n");

		       }

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
		    // use procedure for expression
		    //move forward then check
		    if(root->tok==EPS)
			return curr_offset;
		    else
		    {
			curr_offset = codegen(root->children[0], current_table,curr_offset);
			curr_offset = codegen(root->children[1], current_table,curr_offset);

			if(root->children[2]->tok==EPS)
			{
			    curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N8 empty
			    //do nothing as answer is already in edx
			}
			else
			{
			    fprintf(code_file,"	push edx \n");

			   //evaluate N8
			   curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N8

			   fprintf(code_file,"	pop eax\n");
			   fprintf(code_file,"	cmp eax,edx\n");  //compare eax and edx. Performas eax - edx

			    fprintf(code_file,"	mov edx,0   ;assign false initially in comparing\n"
					    "\tmov eax, 1   ;cmov requires register, therefore place true in eax\n");
			    
			   if(root->children[1]->children[0]->children[0]->tok==LT)
			       fprintf(code_file,"	cmovl  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==LE)
			       fprintf(code_file,"	cmovle  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==GT)
			       fprintf(code_file,"	cmovg  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==GE)
			       fprintf(code_file,"	cmovge  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==EQ)
			       fprintf(code_file,"	cmove  edx , eax	;place true if condition met\n");
			   else if(root->children[1]->children[0]->children[0]->tok==NE)
			       fprintf(code_file,"	cmovne  edx , eax\n");


			}


			root->type = boolean;
		    }
		    return curr_offset;
		}break;

	    case ARITHMETICEXPR:
		{
		    // use procedure for expression
		    //move forward then check
		    
		    curr_offset = codegen(root->children[0], current_table,curr_offset); //evaluate term

		    if(root->children[1]->tok==EPS)
		    {
			curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N4 empty
			//do nothing as answer is already in edx
		    }
		    else
		    {
			fprintf(code_file,"	push edx \n");

			//evaluate N4
			curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N4
		    
			fprintf(code_file,"	pop eax\n"); //eax stores the left side result

			if(root->children[1]->children[0]->children[0]->tok==PLUS)
			    fprintf(code_file,"\tadd edx, eax\n");
			else
			{
			    fprintf(code_file,"	sub eax, edx    ;perform (eax - edx) subtraction\n");
			    fprintf(code_file,"	mov edx, eax    ;store result of subtraction in edx\n");
		    
			}
		    }
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
		    // use procedure for expression
		    //move forward then check
		    if(root->tok==EPS)
			return curr_offset;
		    else
		    {
			curr_offset = codegen(root->children[0], current_table,curr_offset);
			curr_offset = codegen(root->children[1], current_table,curr_offset);

			if(root->children[2]->tok==EPS)
			{
			    curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N4 empty
			    //do nothing as answer is already in edx
			}
			else
			{
			    fprintf(code_file,"	push edx \n");

			    //evaluate N4
			    curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N4

			    fprintf(code_file,"	pop eax\n"); //eax stores the left side result

			    if(root->children[2]->children[0]->children[0]->tok==PLUS)
				fprintf(code_file,"	add edx, eax\n");
			    else
			    {
				fprintf(code_file,"	sub eax, edx    ;perform (eax - edx) subtraction\n");
				fprintf(code_file,"	mov edx, eax    ;store result of subtraction in edx\n");

			    }

			}

			root->type = root->children[1]->type;
		    }
		    return curr_offset;
		}break;

	    case TERM:
		{
		    // use procedure for expression   
		    curr_offset = codegen(root->children[0], current_table,curr_offset); //evaluate factor

		    if(root->children[1]->tok==EPS)
		    {
			curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N5 empty
			//do nothing as answer is already in edx
		    }
		    else
		    {
			fprintf(code_file,"	push edx \n");

			//evaluate N5
			curr_offset = codegen(root->children[1], current_table,curr_offset); //evaluate N5
		    
			fprintf(code_file,"	pop eax\n");
			if(root->children[1]->children[0]->children[0]->tok==MUL)
			{
			    fprintf(code_file,"	imul edx	;eax*edx stored in edx:eax \n");
			    fprintf(code_file,"	mov edx, eax    ;truncated result moved to edx \n");
			}
			else
			{
			    fprintf(code_file,"	push ebx\n"); //save ebx for temp use
			    fprintf(code_file,"	mov ebx,edx\n");
			    fprintf(code_file,"	mov edx, 0\n");
			    fprintf(code_file,"	idiv ebx	;edx:eax divided by ebx result stored in eax\n");
			    fprintf(code_file,"	mov edx, eax    ;move the quotient into edx\n");
			    fprintf(code_file,"	pop ebx\n"); //restore ebx
			}
		    }

  
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
		    // use procedure for expression
		    //move forward then check
		    if(root->tok==EPS)
			return curr_offset;
		    else
		    {
			curr_offset = codegen(root->children[0], current_table,curr_offset);
			curr_offset = codegen(root->children[1], current_table,curr_offset);

			if(root->children[2]->tok==EPS)
			{
			    curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N4 empty
			    //do nothing as answer is already in edx
			}
			else
			{
			    fprintf(code_file,"	push edx \n");

			    //evaluate N5
			    curr_offset = codegen(root->children[2], current_table,curr_offset); //evaluate N5

			    fprintf(code_file,"	pop eax\n");
			    if(root->children[2]->children[0]->children[0]->tok==MUL)
			    {
				fprintf(code_file,"	imul edx	;eax*edx stored in edx:eax \n");
				fprintf(code_file,"	mov edx, eax    ;truncated result moved to edx \n");
			    }
			    else
			    {
				fprintf(code_file,"	push ebx\n"); //save ebx for temp use
				fprintf(code_file,"	mov ebx,edx\n");
				fprintf(code_file,"	mov edx, 0\n");
				fprintf(code_file,"	idiv ebx	;edx:eax divided by ebx result stored in eax\n");
				fprintf(code_file,"	mov edx, eax    ;move the quotient into edx\n");
				fprintf(code_file,"	pop ebx\n"); //restore ebx
			    }

			}

			root->type = root->children[1]->type;
		    }
		    return curr_offset;
		    }
		break;
	    case FACTOR:
		{
		    /* If arithbool expression then evaluate the expression and return ans in edx.
		     * else do stuff relating to var_id_num
		     */

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
		    //no code produced
		    return curr_offset;
		}break;
	    
	    case DECLARESTMT:
		{
		    // no code produced
		    //move forward
		    curr_offset = codegen(root->children[1], current_table,curr_offset);
		    curr_offset = declareVariablesOffset(current_table, root->children[0], root->children[1],curr_offset);
		    return curr_offset;
		}break;

	    case CONDITIONALSTMT:
		{
		    /*	increment the no_switch
		     *	function to produce the compare statements for 
		     *	all the case statements and to assign the current
		     *	switch number to the cases as casestmt.type = no_switch.
		     *	
		     *	Function to produce the code for all the case statements 
		     *	labels are created as switch_(switch_no)_(labelvalue):
		     *
		     *	create the exit label as switch_exit_(switch_no):
		     */

		    //create a new scope(symbol table) and make the current table as the parent of the new table.
		    symbolTable* new_table = getSymbolTable(100);
		    
		    new_table->parent = current_table;
		    current_table->children[current_table->no_children++]=new_table;
		    

		    
		    no_switch++;
		    curr_offset = codegen(root->children[0],new_table,curr_offset); //evaluate ID
		    makeCaseJumps(root,current_table);

		    //produce code from casestmts and default
		    curr_offset = codegen(root->children[1], new_table, curr_offset);
		    curr_offset = codegen(root->children[2], new_table, curr_offset);
		    
		    fprintf(code_file,"	switch_exit_%d:\n",root->casehandle);
		    return curr_offset;
		}break;

	    case CASESTMTS:
		{
		    //code generation handled by conditional 
		    //statement through functions
		    
		    fprintf(code_file,"switch_%d_%s:\n",root->casehandle,root->children[0]->lexeme->str);
		    //move forward 
		    curr_offset = codegen(root->children[1], current_table, curr_offset);

		    fprintf(code_file,"	jmp switch_exit_%d\n",root->casehandle);

		    curr_offset = codegen(root->children[2],current_table, curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;

	    case N9:
		{
		    //handled by casestmt

		    //move forward
			if(root->tok == EPS)
			    return curr_offset;
			else
			{
			    fprintf(code_file,"switch_%d_%s:\n",root->casehandle,root->children[0]->lexeme->str);
			    //move forward 
			    curr_offset = codegen(root->children[1], current_table, curr_offset);

			    fprintf(code_file,"	jmp switch_exit_%d\n",root->casehandle);

			    curr_offset = codegen(root->children[2],current_table, curr_offset);

			    root->type = root->children[0]->type;
			}
		    return curr_offset;
		}break;

	    case VALUE:
		{
		    //handled by casestmt
		    //move forward
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);
		    
		    root->type = root->children[0]->type;
		    return curr_offset;
		}break;
	    case DEFAULT1:
		{
		    //code handled by conditional statement
		    //move forward.
		    
		    if(root->tok==EPS)
			return curr_offset;
		    else
		    {
			fprintf(code_file,"switch_default_%d:\n", root->casehandle);

			for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table, curr_offset);

			fprintf(code_file,"	jmp switch_exit_%d\n",root->casehandle);

			return curr_offset;
		    }
		}break;
	    case ITERATIVESTMT:
		{
		    if(root->children[0]->tok == FOR)
		    {

			/* increment no_for
			 * create for loop
			 * start label as for_(no_for)
			 * create code for the function body by moving forwaard
			 * exit label as for_exit_(no_for)
			 */

			//check ID
			curr_offset = codegen(root->children[1],current_table,curr_offset);
			symbol_table_node* a = searchSymbolTable(current_table, root->children[1]->lexeme->str);
			
			//Check Range
			curr_offset = codegen(root->children[2],current_table,curr_offset);

			no_for++;
			root->casehandle= no_for;

			//assign to ecx the lower bound of range
			astnode* range = root->children[2];
			astnode* index1 = range->children[0];
			astnode* index2 = range->children[1];


			fprintf(code_file,"	push ecx    ;save ecx before loop start\n");
			if(index1->children[0]->tok == NUM)
			{
			    //const 1st range
			    fprintf(code_file,"	mov ecx,%d\n",atoi(index1->children[0]->lexeme->str));
			    fprintf(code_file,"\tmov [ebp+%d],ecx   ;mov first index into loop var\n",a->offset);
			}
			else
			{
			    // variable 1st range
			    symbol_table_node* var = searchSymbolTable(current_table, index1->children[0]->lexeme->str);
			    fprintf(code_file,"	mov ecx,[ebp+%d]\n",var->offset);
			    fprintf(code_file,"\tmov [ebp+%d],ecx   ;mov first index into loop var\n",a->offset);
			}

			fprintf(code_file,"FOR_LOOP_%d:\n",root->casehandle);

			//for loop
			symbolTable* new_table = getSymbolTable(100);
	
			new_table->parent = current_table;
			current_table->children[current_table->no_children++]=new_table;
		    

			curr_offset = codegen(root->children[3],new_table,curr_offset);

			fprintf(code_file,"	push edx\n");
			if(index2->children[0]->tok == NUM)
			{
			    //const 1st range
			    fprintf(code_file,"	mov edx,%d\n",atoi(index2->children[0]->lexeme->str));
			}
			else
			{
			    // variable 2nd range
			    symbol_table_node* var = searchSymbolTable(current_table, index2->children[0]->lexeme->str);
			    fprintf(code_file,"	mov edx,[ebp+%d]\n",var->offset);
			}

			fprintf(code_file,"	mov ecx, [ebp+%d]\n",a->offset);
			fprintf(code_file,"\tadd ecx,1\n");
			fprintf(code_file,"\tmov [ebp+%d],ecx	;add 1 to loop variable\n",a->offset);
			fprintf(code_file,"	cmp ecx,edx\n");
			fprintf(code_file,"	pop edx\n");
			fprintf(code_file,"	jle FOR_LOOP_%d\n",root->casehandle);
			fprintf(code_file,"	pop ecx	;restore ecx after the loop\n");

		    }
		    else
		    {
			/* increment no_while
			 * create while loop
			 * start label as while_(no_while)_check
			 * create code to check expression
			 * if false then jump to exit else jump to code
			 * code label: while_(no_while)_code
			 * create code for the function body by moving forwaard
			 * jump to check label
			 * exit label as while_exit_(no_while)
			 */

			//while loop
			
			no_while++;
			root->casehandle = no_while;
			fprintf(code_file,"WHILE_LABEL_%d:\n",root->casehandle);

			//check that conditional expression is boolean
			curr_offset = codegen(root->children[1],current_table,curr_offset);
			
			//resut of expression stored in edx
			fprintf(code_file,"	cmp edx, 0\n");
			fprintf(code_file,"	je EXIT_WHILE_%d\n",root->casehandle);
			
			//create new scope(symbol table) and assign the current table as its parent
			//then move forward
			symbolTable* new_table = getSymbolTable(100);
			new_table->parent = current_table;
			current_table->children[current_table->no_children++]=new_table;
		    

			curr_offset = codegen(root->children[2],new_table,curr_offset);
		    
			fprintf(code_file,"	jmp WHILE_LABEL_%d\n",root->casehandle);
			fprintf(code_file,"EXIT_WHILE_%d:\n",root->casehandle);
		    }
		    return curr_offset;
		}break;
	    case RANGE:
		{
		    
		    for(int i =0;i<root->n;i++)
			    curr_offset = codegen(root->children[i], current_table,curr_offset);

		    if((root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==NUM))
		    {
			////////dynamic range check. arg1->ID arg2->NUM
		    }
		    if((root->children[0]->children[0]->tok==NUM && root->children[1]->children[0]->tok==ID))	
		    {
			//////dynamic range check. arg1->NUM arg2->ID
		    }
		    if(root->children[0]->children[0]->tok==ID && root->children[1]->children[0]->tok==ID)
		    {
			////////dynamic check of index1 < index2
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
