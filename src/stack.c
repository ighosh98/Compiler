#include<stdio.h>
#include"stack.h"
#include<stdlib.h>
stack getStack()
{
    stack s;
    s.head = (stacknode**)malloc(sizeof(stacknode*));
    *(s.head) = NULL;
    return s;
}

void stack_push(stack s , treenode* a)
{
    stacknode* temp = (stacknode*)malloc(sizeof(stacknode));
    stacknode* head = *(s.head);
    temp->next = NULL;
    temp->val = a;
    if(head==NULL)
	head = temp;
    else
    {
	temp->next = head;
	head = temp;
    }
    *(s.head) = head;
}

void stack_pop(stack s)
{
    stacknode* head = *(s.head);
    if(head == NULL)
    {
	printf("UnderFlow\n");
    }
    else
    {
	stacknode* temp = head;
	head = head->next;
	free(temp);
    }
    *(s.head)= head;
}

treenode* stack_top(stack s)
{
    stacknode* head = *(s.head);
    if(head==NULL)
	return make_treenode(UNDERFLOW,NULL);    //stack error condition
    else
	return head->val;
}
