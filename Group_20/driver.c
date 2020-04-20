/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#include <stdio.h>
#include<stdbool.h>
#include "lexer.h"
#include "stack.h"
#include "parser.h"
#include "set.h"
#include "color.h"
#include <time.h>
#include <string.h>
#include "ast.h"
#include "astdef.h"
#include "symboltable.h"
#include "semantic.h"
#include "codegen.h"
#include "treefiles.h"

void printSymbolTableDriver(symbolTable* symbol_table)
{
    if(symbol_table == NULL)return;

    printf("%s\n",searchSymbolTable(symbol_table,"_currentfunction")->iplist->name);
    for(int i=0;i<symbol_table->no_children;i++)
	printSymbolTableDriver(symbol_table->children[i]);
}

int main(int argc,char **argv)
{
    printf("############ Implementation Status ##############\n");
    printf("a) First Set and Follow Set Automated.\n");
    printf("b) Lexer and Syntax Analyser implemented completely.\n");
    printf("c) Code runs successfully for all given test cases.\n");
    printf("d) Parse Tree generated Successfully for all cases.\n");

    //verify both input and output files***********************
    if(argc<2)
    {
	printf("No files given\n");
	return(1);
    }

    make_str_to_enum();
    productions p = read_grammar();
    int n = p.no_productions;

    int choice;

    while(1)
    {
	printf("\nEnter Choice: ");
	scanf("%d",&choice);
	
	if(choice == 0)
	    break;
	else if(choice == 1)
	{
	    openfile(argv[1]);
	    token* a;
	    red();
	    printf("\n##############  Token List From Lexer  ###############\n");
	    reset();
	    while(a = getNextToken())
	    {
		printf("Line %d: %20s: %10s\n",a->line_no,a->str,symbol_map[a->tag]);
		if(a->tag == $)
		    break;
	    }
	   // closefile();
	}
	else if(choice == 2)
	{
	    parsing_error = false;
	    red();
	    printf("\n##############  Printing Parse Tree  ##############\n");
	    reset();
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);
	    printTree(t.root);
	}
	else if(choice == 3)
	{
	    parsing_error = false;
	    pass_no = 0; //reset the semantic analyser so that the option can be used multiple times
	    
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);

	    AST a;
	    if(parsing_error == false)
		a = makeAST(t.root);
	    else
	    {
		red();
		printf("Could not parse input file\n");
		reset();
		continue;
	    }

	    red();
	    printf("Printing AST in PREORDER traversal\n");
	    reset();
	    printAST(a.root);
	}
	else if(choice == 4)
	{
	    parsing_error = false;
	    pass_no = 0; //reset the semantic analyser so that the option can be used multiple times
	    
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);
	    
	    AST a;
	    if(parsing_error == false)
		a = makeAST(t.root);
	    else
	    {
		red();
		printf("Could Not parse Input file\n");
		reset();
		continue;
	    }
	    red();
	    printf("############## Created AST ##############\n");
	    reset();
	    long unsigned int a1, b1;
	    a1 = sizeof(treenode)*naryTreesize(t.root);
	    b1 = sizeof(astnode)*ASTsize(a.root);
	    printf("Parse Tree number of nodes: %d    Allocated Memory: %ld\n",naryTreesize(t.root),a1);
	    printf("AST number of nodes:        %d    Allocated Memory: %ld\n",ASTsize(a.root),b1);

	    printf("Compression Percentage: %lf\n", (((a1-b1)/(double)a1))*100);

	}
	else if(choice == 5)
	{
	    parsing_error = false;
	    semantic_error = false;
	    pass_no = 0; //reset the semantic analyser so that the option can be used multiple times
	    red();
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);
	    
	    AST a;
	    if(parsing_error == false)
		a = makeAST(t.root);
	    else
	    {
		red();
		printf("Could not Parse Input file\n");
		reset();
		continue;
	    }

	    symbolTable* symbol_table;
	    symbol_table = check_semantics(a.root); //helper function that does 2 passes automatically

	    top_table= getSymbolTable(10);


	    if(semantic_error == true)
	    {
		red();
		printf("\nFile contains semantic errors. Therefore the symbol tables may not be correct\n");
		reset();
		for(int i=0;i<symbol_table->no_children;i++)
		{
		    printSymbolTables(symbol_table->children[i],0);
		}
	    }
	    else
	    {
		code_file = fopen("unused.asm","w");
		codegen(a.root,NULL,0);
		fclose(code_file);
		red();
		printf("############ Printing Symbol tables ############\n");
		reset();

		for(int i=0;i<top_table->no_children;i++)
		{
		    printSymbolTables(top_table->children[i],0);
		}
	    }
	}
	else if (choice == 6)
	{
	    parsing_error = false;
	    semantic_error = false;
	    pass_no = 0; //reset the semantic analyser so that the option can be used multiple times
	    red();
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);
	    AST a;
	    if(parsing_error == false)
		a = makeAST(t.root);
	    else
	    {
		red();
		printf("Could not parse input file\n");
		reset();
		continue;
	    }
	    
	    symbolTable* symbol_table;
	    symbol_table = check_semantics(a.root); //helper function that does 2 passes automatically
	    
	    red();
	    printf("########### Printing Function Record Sizes ############\n");
	    reset();
	    for(int i=0;i<function_table->size;i++)
	    {
		symbol_table_node * head = function_table->ar[i];
		while(head)
		{
		    printf("%s  %d\n",head->name, head->stackSize);
		    head = head->next;
		}
	    }
	    
	}
	else if(choice == 7)
	{
	    parsing_error = false;
	    semantic_error = false;
	    pass_no = 0; //reset the semantic analyser so that the option can be used multiple times
	    red();
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);
	    
	    AST a;
	    if(parsing_error == false)
		a = makeAST(t.root);
	    else
	    {
		red();
		printf("Could not Parse Input file\n");
		reset();
		continue;
	    }

	    symbolTable* symbol_table;
	    symbol_table = check_semantics(a.root); //helper function that does 2 passes automatically

	    top_table= getSymbolTable(10);


	    if(semantic_error == true)
	    {
		red();
		printf("\nFile contains semantic errors. Therefore output may not be correct\n");
		reset();
		for(int i=0;i<symbol_table->no_children;i++)
		{
		    printSymbolTables(symbol_table->children[i],0);
		}
	    }
	    else
	    {
		code_file = fopen("unused.asm","w");
		codegen(a.root,NULL,0);
		fclose(code_file);
		red();
		printf("############ Printing Arrays ############\n");
		reset();

		for(int i=0;i<top_table->no_children;i++)
		{
		    printSymbolTablesArr(top_table->children[i],0);
		}
	    }
	}
	else if(choice == 8 )
	{
	    parsing_error = false;
	    semantic_error = false;
	    pass_no = 0;
	    clock_t start_time, end_time;
	    double total_cpu_time, total_cpu_time_secs;
	    start_time = clock();

	    red();
	    printf("\n##############  Parsing Input File  ##############\n");
	    reset();
	    
	    //makeFirstAndFollow(p, PROGRAM);
	    //makeParsingTable(p);
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);
	   
	    AST a;
	    if(parsing_error == false)
	    {
		a = makeAST(t.root);
		red();
		printf("\n##############  Checking Semantics  ##############\n");
		reset();
		symbolTable* symbol_table;
		symbol_table = check_semantics(a.root); //helper function that does 2 passes automatically
	    }
	    else
	    {
		red();
		printf("Could not parse input file\n");
		reset();
	    }

	    end_time = clock();
	    total_cpu_time = (double)(end_time - start_time);
	    total_cpu_time_secs = total_cpu_time/CLOCKS_PER_SEC;
	    printf("Total CPU time: %lf\nTotal CPU time in secs: %lf\n",
		    total_cpu_time, total_cpu_time_secs);
	
	}
	else if(choice == 9)
	{
	    parsing_error = false;
	    semantic_error = false;
	    pass_no = 0; //reset the semantic analyser so that the option can be used multiple times
	    FILE * fptr = fopen(argv[2],"w");
	    if(!fptr)
	    {
		red();
		printf("could not open file %s\n",argv[2]);
		reset();
		break;
	    }
	    red();
	    Nary_tree t = parse_input(PROGRAM, argv[1],p);
	    
	    AST a;
	    if(parsing_error == false)
		a = makeAST(t.root);
	    else
	    {
		red();
		printf("Could not Parse Input file\n");
		reset();
		continue;
	    }
	    symbolTable* symbol_table;
	    symbol_table = check_semantics(a.root); //helper function that does 2 passes automatically
	    
	    if(semantic_error == false)
	    {

		red();
		printf("\nWriting code to file\n");
		reset();

		top_table = getSymbolTable(10);
		code_file = fptr;
		codegen(a.root,NULL,0);
	    }
	    else
	    {
		red();
		printf("Code contains Semantic Errors. Could not generate ASM code\n");
		reset();
	    }
	    fclose(fptr);

	}
	else
	{
	    printf("Invalid Choice\n");
	}
    }
    return 0;
}
