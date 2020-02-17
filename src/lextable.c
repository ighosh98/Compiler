#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hash.h"
#include "lextable.h"
token *makeToken(char* str,type tag)
{
    token * temp = (token *) malloc(sizeof(token));
    temp->str = str;
    temp->tag = tag;
    temp->next = NULL;
    return temp;
}


token* insertToChain(token* ar[], token* t, int index)
{
    if(ar[index]==NULL)
	ar[index] = t;
    else
    {
	t->next = ar[index];
	ar[index] = t;
    }
    return t;
}

token* searchChain(token* head, char* str )
{
    token* temp = head;
    while(temp)
    {
	if(strcmp(temp->str, str)==0)
	    return temp;
	temp = temp->next;
    }
    return temp;
}

token* searchTable(lextable table, char* str)
{
    int index = (int)(hashf(str)%table.size);
    return searchChain(table.ar[index], str);
}

lextable getLexTable(int n)
{
    lextable temp;
    temp.ar= (token **)malloc(sizeof(token *)*n);
    temp.size = n;
    return temp;  
}

token* insertTable(lextable table,char* str, type tag)
{
    token * temp = searchTable(table, str);
    
    if(temp!=NULL)return temp;
    
    temp = makeToken(str,tag);   

    int index = (int)(hashf(str)%table.size);

    token* check = insertToChain(table.ar,temp,index);
    if(check!=NULL)
	return temp;
    else
    {
	printf("ERROR: Token could not be inserted\n");
	return NULL;
    }
}





