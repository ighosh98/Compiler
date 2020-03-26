#include <stdio.h>
#include "astdef.h"
#include "symboltable.h"
#include <string.h>
#include "hash.h"

void printSymbolNode(symbol_table_node* a)
{
    printf("%s  %d  %d  ",a->name,a->isarr,a->isdynamic);
    
    if(a->isdynamic)
	printf("%s  %s  ",a->dyn_range1->name, a->dyn_range2->name);
    else
	printf("%d  %d  ",a->const_range1,a->const_range2);
    if(a->lexeme)
	printf("%s",a->lexeme->str);
    printf("\n\n");
}

symbol_table_node * makeSymbolNode(char* name , bool isarr,
       	bool isdyn, symbol_table_node * d_range1, 
	symbol_table_node* d_range2, int c_range1, 
	int c_range2,token* lexeme, datatype type)
{
    symbol_table_node * temp = (symbol_table_node*)malloc(
					sizeof(symbol_table_node));
    //name cannot be greater than 20 chars
    strcpy(temp->name, name);
    temp->isarr = isarr;
    temp->isdynamic = isdyn;
    temp->dyn_range1 = d_range1;
    temp->dyn_range2 = d_range2;
    temp->const_range1 = c_range1;
    temp->const_range2 = c_range2;
    temp->lexeme = lexeme;
    temp->type = type;

    return temp;
}


symbol_table_node* insertToSymChain(symbol_table_node* ar[],
       	symbol_table_node* t, unsigned int index)
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

symbol_table_node* searchSymChain(symbol_table_node* head, char* str )
{   
    symbol_table_node* temp = head;
    while(temp)
    {
	if(strcmp(temp->name,str) == 0)
	    return temp;
	temp = temp->next;
    }
    return temp;
}

symbol_table_node* searchSymTable(symbolTable table, char* str)
{
    unsigned int index = hashf(str)%table.size;
    return searchSymChain(table.ar[index],str);
}

symbolTable getSymbolTable(unsigned int n)
{
    symbolTable temp;
    temp.ar = (symbol_table_node**) malloc(
	    sizeof(symbol_table_node*)*n);
    for(int i=0;i<n;i++)
	temp.ar[i]=NULL;
    temp.size=n;
    return temp;
}

symbol_table_node * insertSymbolTable(symbolTable table,
       	char* name , bool isarr,
	bool isdyn, symbol_table_node * d_range1, 
	symbol_table_node* d_range2, int c_range1, 
	int c_range2,token* lexeme, datatype type)
{
    symbol_table_node * temp = searchSymTable(table,name);

    if(temp!=NULL)return temp;

    temp = makeSymbolNode(name ,isarr, isdyn, d_range1, d_range2,
	   c_range1, c_range2, lexeme, type);

    unsigned int index = hashf(name)%table.size;

    symbol_table_node* check = insertToSymChain(table.ar,temp,index);
    if(check!=NULL)
	return temp;
    else
    {
	printf("ERROR: Symbol table entry could not be inserted\n");
	return NULL;
    }

}

