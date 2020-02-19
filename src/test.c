#include <stdio.h>
#include<stdbool.h>
#include "lexer.h"
#include "stack.h"
#include "parser.h"
#include "set.h"
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
    printf("\n");
    set s = getSet();
    set s1 = getSet();
    insertSet(s,0);
    insertSet(s,1);
    insertSet(s1,2);
    insertSet(s1,3);

    printSet(s);
    printSet(s1);
    setUnion(s,s1);
    printSet(s);
    setUnion(s1,s);
    printSet(s1);
    getFirstSet(p);
}
