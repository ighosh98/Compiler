#include<stdio.h>
#include"stack.h"
#include<stdlib.h>

stack* getStack()
{
    stack* s = (stack*)malloc(sizeof(stack));
    s->head = NULL;
    return s;
}

void stack_push(stack* s,int a)
{
    stacknode* temp = (stacknode*)malloc(sizeof(stacknode));
    temp->next = NULL;
    temp->val = a;
    if(s->head==NULL)
	s->head = temp;
    else
    {
	temp->next = s->head;
	s->head = temp;
    }
}

void stack_pop(stack* s)
{
    if(s->head == NULL)
    {
	printf("UnderFlow\n");
    }
    else
    {
	stacknode* temp = s->head;
	s->head = s->head->next;
	free(temp);
    }
}

int stack_top(stack* s)
{
    if(s->head==NULL)
	return UNDERFLOW;    //stack error condition
    else
	return s->head->val;
}
