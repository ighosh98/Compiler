#ifndef STACK
#define STACK
#include<stdio.h>
#define UNDERFLOW -1
typedef struct stacknode
{
    int val;
    struct stacknode* next;   
} stacknode;

typedef struct stack
{
    stacknode** head;
} stack;

void stack_push(stack s,int a);

void stack_pop(stack s);

int stack_top(stack s);

stack getStack();

#endif
