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
	printf("%10s: %20s: %10d\n",symbol_map[a->tag],a->str,a->line_no);

    make_str_to_enum();

}
