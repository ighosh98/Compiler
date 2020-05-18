/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef STACK
#define STACK
#include<stdio.h>
#include "nary_tree.h"
#define UNDERFLOW -1
typedef struct stacknode
{
	treenode* val;
	struct stacknode* next;
} stacknode;

typedef struct stack
{
	stacknode** head;
} stack;

void stack_push(stack s, treenode* a);

void stack_pop(stack s);

treenode* stack_top(stack s);

stack getStack();

#endif
