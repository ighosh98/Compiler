#include <stdio.h>
#include "stack.h"
#include "nary_tree.h"

int  main()
{
   stack s = getStack();
    stack_push(s,make_treenode(0));
    treenode* a =   stack_top(s);
    printf("%d\n",a->tok);
}
