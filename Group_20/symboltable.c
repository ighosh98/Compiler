#include <stdio.h>
#include "astdef.h"
#include "symboltable.h"
#include <string.h>
#include "hash.h"

void deleteSymbolTable(symbolTable* table)
{
    for(int i=0;i<table->size;i++)
    {
	symbol_table_node * head = table->ar[i];
	while(head!=NULL)
	{
	    symbol_table_node * temp = head->next;
	    free(head);
	    head = temp;
	}
    }
    free(table->ar);
    table->ar = NULL;
    table->size = 0;
}

void printSymbolNode(symbol_table_node* a)
{
    printf("%s  %d  %d  ",a->name,a->isarr,a->isdynamic);
    
    if(a->isdynamic)
	printf("%s  %s  ",a->drange1->name, a->drange2->name);
    else
	printf("%d  %d  ",a->crange1,a->crange2);
    if(a->lexeme)
	printf("%s  %d  ",a->lexeme->str,a->type);
    printf("\n\n");
}

symbol_table_node * makeSymbolNode(char* name , bool isarr,
       	int isdyn, symbol_table_node * d_range1, 
	symbol_table_node* d_range2, int c_range1, 
	int c_range2,token* lexeme, datatype type)
{
    symbol_table_node * temp = (symbol_table_node*)malloc(
					sizeof(symbol_table_node));
    //name cannot be greater than 20 chars
    strcpy(temp->name, name);
    temp->isarr = isarr;
    temp->isdynamic = isdyn;
    temp->drange1 = d_range1;  
    temp->drange2 = d_range2;
    temp->crange1 = c_range1;
    temp->crange2 = c_range2;
    temp->lexeme = lexeme;
    temp->type = type;
    temp->isUsed = false;
    temp->isDefined = false;
    temp->iplist =NULL;
    temp->oplist = NULL;
    temp->stackSize = 0;
    temp->offset = 0;
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

symbol_table_node* searchSymbolTableHelper(symbolTable* table, char* str)
{

    unsigned int index = hashf(str)%table->size;
    
    return searchSymChain(table->ar[index],str);
}

symbol_table_node* searchSymbolTable(symbolTable* table, char* str)
{
    if(table == NULL) return NULL;
    
    symbol_table_node* a = searchSymbolTableHelper(table, str);
    if(a == NULL)
    {
	return searchSymbolTable(table->parent, str);
    }
    else
	return a;

}

symbol_table_node* searchSymbolTableLocal(symbolTable* table, char* str)
{
    if(table == NULL) return NULL;
    
    symbol_table_node* a = searchSymbolTableHelper(table, str);
    return a; //does not search the parent
}


symbolTable* getSymbolTable(unsigned int n)
{
    symbolTable* temp = (symbolTable*)malloc(sizeof(symbolTable));;
    temp->parent = NULL;
    temp->ar = (symbol_table_node**) malloc(
	    sizeof(symbol_table_node*)*n);
    for(int i=0;i<n;i++)
	temp->ar[i]=NULL;
    temp->size=n;
    return temp;
}

symbol_table_node * insertSymbolTable(symbolTable* table,
       	char* name , bool isarr,
	int isdyn, symbol_table_node * d_range1, 
	symbol_table_node* d_range2, int c_range1, 
	int c_range2,token* lexeme, datatype type)
{
    if(table==NULL)return NULL;
    symbol_table_node * temp = searchSymbolTable(table,name);

    if(temp!=NULL)return temp;

    temp = makeSymbolNode(name ,isarr, isdyn, d_range1, d_range2,
	   c_range1, c_range2, lexeme, type);

    unsigned int index = hashf(name)%table->size;

    symbol_table_node* check = insertToSymChain(table->ar,temp,index);
    if(check!=NULL)
	return temp;
    else
    {
	printf("ERROR: Symbol table entry could not be inserted\n");
	return NULL;
    }

}

