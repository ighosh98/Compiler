#include <stdio.h>
#include<stdbool.h>
#include "lexer.h"
#include "stack.h"
#include "parser.h"
#include "set.h"
#include "color.h"
#include <time.h>
int main(int argc,char **argv)
{
    printf("Implementation Status\n");
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
	printf("Enter Choice: ");
	scanf("%d",&choice);
	
	if(choice==0)
	    break;
	else if(choice==1)
	{
	    removeComments(argv[1]);
	}
	else if(choice==2)
	{
	    openfile(argv[1]);
	    token* a;
	    red();
	    printf("\nToken List From Lexer\n");
	    reset();
	    while(a = getNextToken())
	    {
		printf("Line %d: %20s: %10s\n",a->line_no,a->str,symbol_map[a->tag]);
		if(a->tag == $)
		    break;
	    }
   
	}
	else if(choice==3)
	{
	    FILE * fptr = fopen(argv[2],"w");
	    if(!fptr)
	    {
		printf("could not open file %s\n",argv[2]);
		break;
	    }
	    red();
	    printf("\nParsing Input File\n");
	    reset();
	    makeFirstAndFollow(p, PROGRAM);
	    makeParsingTable(p);
	    Nary_tree t = parse_input(PROGRAM, argv[1]);
	    inorder(t,fptr);
	    fclose(fptr);
	}
	else if(choice ==4)
	{
	    clock_t start_time, end_time;
	    double total_cpu_time, total_cpu_time_secs;
	    start_time = clock();

	    FILE * fptr = fopen(argv[2],"w");
	    if(!fptr)
	    {
		printf("could not open file %s\n",argv[2]);
		break;
	    }

	    red();
	    printf("\nParsing Input File\n");
	    reset();
	    makeFirstAndFollow(p, PROGRAM);
	    makeParsingTable(p);
	    Nary_tree t = parse_input(PROGRAM, argv[1]);
	    inorder(t,fptr);
	    fclose(fptr);

	    end_time = clock();
	    total_cpu_time = (double)(end_time - start_time);
	    total_cpu_time_secs = total_cpu_time/CLOCKS_PER_SEC;
	    printf("Total CPU time: %lf\nTotal CPU time in secs: %lf\n",
		    total_cpu_time, total_cpu_time_secs);
	}
	else
	{
	    printf("Invalid Choice\n");
	}
    }
    return 0;
}
