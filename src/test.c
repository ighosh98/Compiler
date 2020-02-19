#include <stdio.h>
#include<stdbool.h>
#include "lexer.h"
#include "stack.h"
#include "parser.h"
int main()
{
    openfile("lextest.txt");
    token* a;
    while(a = getNextToken())
	printf("Line %d: %10s: %20s\n",a->line_no,symbol_map[a->tag],a->str);

    make_str_to_enum();
    productions p = read_grammar();

    prodn* p1 = p.rules;
    int n = p.no_productions;

    for(int i=0;i<n;i++)
    {
	printf("nonterm: %s\n",symbol_map[p1[i].non_terminal]);
	for(int j=0;j<p1[i].size;j++)
	    printf("%s ",symbol_map[p1[i].rule[j]]);
	printf("\n");
    }

}
