#include <stdio.h>
#include<stdbool.h>
#include "lexer.h"
#include "stack.h"
#include "parser.h"
#include "set.h"
#include "color.h"
int main(int argc,char **argv)
{
    if(argc<2)
    {
	printf("No file given\n");
	return(1);
    }
    openfile(argv[1]);
    token* a;
    while(a = getNextToken())
    {
	//printf("Line %d: %10s: %20s\n",a->line_no,symbol_map[a->tag],a->str);
	if(a->tag == $)break;
    }
    make_str_to_enum();
    productions p = read_grammar();

    prodn* p1 = p.rules;
    int n = p.no_productions;

    //############ print Rules #############

    /*
    red();
    printf("\n\nRules that are Read\n");
    reset();
    for(int i=0;i<n;i++)
    {
	for(int j=0;j<p1[i].size;j++)
	    printf("%s ",symbol_map[p1[i].rule[j]]);
	printf("\n");
    }
    */
    makeFirstAndFollow(p, PROGRAM);
    makeParsingTable(p);
    printf("complete\n");
}
