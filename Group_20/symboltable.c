/*
   Group 20
   Ayush Vachaspati 2016B3A70398P
   Indraneel Ghosh  2016B1A70938P
   G Adityan     2016B1A70929P
*/

#include <stdio.h>
#include "astdef.h"
#include "symboltable.h"
#include <string.h>
#include "hash.h"

void deleteSymbolTable(symbolTable* table)
{
    for (int i = 0; i < table->size; i++)
    {
        symbol_table_node * head = table->ar[i];
        while (head != NULL)
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
void printSymbolNodeArr(symbol_table_node* a, symbol_table_node* curr_func, int nesting, int start, int end)
{
    if (strcmp(a->name, "_currentfunction") == 0) return;


    int width = -1; //calculate width of the variable

    if (a->isarr)
    {
        if (a->isdynamic)
        {
            width = 12;
            if (a->isdynamic == 1)
            {
                printf("%20s	%d-%d	%10s	dynamic array	[%d, %s]    %10s",
                       curr_func->name, start, end, a->name, a->crange1, a->drange2->name, datatype_map[a->type]);
            }
            else if (a->isdynamic == 2)
            {
                printf("%20s	%d-%d	%10s	dynamic array	[%s, %d]    %10s",
                       curr_func->name, start, end, a->name, a->drange1->name, a->crange2, datatype_map[a->type]);
            }
            else if (a->isdynamic == 3)
            {
                printf("%20s	%d-%d	%10s	dynamic array	[%s, %s]    %10s",
                       curr_func->name, start, end, a->name, a->drange1->name, a->drange2->name, datatype_map[a->type]);
            }
        }
        else
        {
            if (a->type == real) width = (a->crange2 - a->crange1) * 8 + 12;
            else width = (a->crange2 - a->crange1) * 4 + 12;
            printf("%20s    %d-%d	%10s	static array	[%d, %d]    %10s",
                   curr_func->name, start, end, a->name, a->crange1, a->crange2, datatype_map[a->type]);
        }

        printf("\n");
    }
}


void printSymbolTablesArr(symbolTable* table, int nesting)
{
    if (table == NULL)return;
    //print the current symbol table
    for (int i = 0; i < table->size; i++)
    {
        symbol_table_node* head = table->ar[i];
        while (head)
        {
            printSymbolNodeArr(head, searchSymbolTable(table, "_currentfunction")->iplist, nesting, table->start_line, table->end_line);
            head = head->next;
        }
    }

    //print the rest of the tables
    for (int i = 0; i < table->no_children; i++)
        printSymbolTablesArr(table->children[0], nesting + 1);
}



void printSymbolNode(symbol_table_node* a, symbol_table_node* curr_func, int nesting, int start, int end)
{
    if (strcmp(a->name, "_currentfunction") == 0) return;


    int width = -1; //calculate width of the variable

    if (a->isarr)
    {
        if (a->isdynamic)
        {
            width = 12;
            if (a->isdynamic == 1)
            {
                printf("%10s  %20s  %d-%d  %3d yes  dynamic array  [%d, %s]	%10s  %3d  %2d", a->name,
                       curr_func->name, start, end, width, a->crange1, a->drange2->name, datatype_map[a->type], a->offset, nesting);
            }
            else if (a->isdynamic == 2)
            {
                printf("%10s  %20s  %d-%d  %3d yes  dynamic array  [%s, %d]	%10s  %3d  %2d", a->name,
                       curr_func->name, start, end, width, a->drange1->name, a->crange2, datatype_map[a->type], a->offset, nesting);
            }
            else if (a->isdynamic == 3)
            {
                printf("%10s  %20s  %d-%d  %3d yes  dynamic array  [%s, %s]	 %10s  %3d  %2d", a->name,
                       curr_func->name, start, end, width, a->drange1->name, a->drange2->name, datatype_map[a->type], a->offset, nesting);
            }
        }
        else
        {
            if (a->type == real) width = (a->crange2 - a->crange1) * 8 + 12;
            else width = (a->crange2 - a->crange1) * 4 + 12;
            printf("%10s  %20s  %d-%d  %3d yes  static array  [%d, %d]	    %10s  %3d %2d", a->name,
                   curr_func->name, start, end, width, a->crange1, a->crange2, datatype_map[a->type], a->offset, nesting);
        }
    }
    else
    {
        if (a->type == real)width = 8;
        else width = 4;
        printf("%10s  %20s  %d-%d  %3d no   --------      --------	%10s  %3d  %2d", a->name,
               curr_func->name, start, end, width, datatype_map[a->type], a->offset, nesting);

    }
    printf("\n");
}

void printSymbolTables(symbolTable* table, int nesting)
{
    if (table == NULL)return;
    //print the current symbol table
    for (int i = 0; i < table->size; i++)
    {
        symbol_table_node* head = table->ar[i];
        while (head)
        {
            printSymbolNode(head, searchSymbolTable(table, "_currentfunction")->iplist, nesting, table->start_line, table->end_line);
            head = head->next;
        }
    }

    //print the rest of the tables
    for (int i = 0; i < table->no_children; i++)
        printSymbolTables(table->children[0], nesting + 1);
}

symbol_table_node * makeSymbolNode(char* name , bool isarr,
                                   int isdyn, symbol_table_node * d_range1,
                                   symbol_table_node* d_range2, int c_range1,
                                   int c_range2, token* lexeme, datatype type)
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
    temp->iplist = NULL;
    temp->oplist = NULL;
    temp->stackSize = 0;
    temp->offset = 0;
    return temp;
}


symbol_table_node* insertToSymChain(symbol_table_node* ar[],
                                    symbol_table_node* t, unsigned int index)
{
    if (ar[index] == NULL)
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
    while (temp != NULL)
    {
        if (strcmp(temp->name, str) == 0)
            return temp;
        temp = temp->next;
    }
    return temp;
}

symbol_table_node* searchSymbolTableHelper(symbolTable* table, char* str)
{

    unsigned int index = hashf(str) % table->size;

    return searchSymChain(table->ar[index], str);
}

symbol_table_node* searchSymbolTable(symbolTable* table, char* str)
{

    if (table == NULL) return NULL;

    symbol_table_node* a = searchSymbolTableHelper(table, str);
    if (a == NULL)
        return searchSymbolTable(table->parent, str);
    else
        return a;

}

symbol_table_node* searchSymbolTableLocal(symbolTable* table, char* str)
{
    if (table == NULL) return NULL;

    symbol_table_node* a = searchSymbolTableHelper(table, str);
    return a; //does not search the parent
}


symbolTable* getSymbolTable(unsigned int n)
{
    symbolTable* temp = (symbolTable*)malloc(sizeof(symbolTable));;
    temp->parent = NULL;
    temp->ar = (symbol_table_node**) malloc(
                   sizeof(symbol_table_node*)*n);
    for (int i = 0; i < n; i++)
        temp->ar[i] = NULL;
    temp->size = n;

    temp->no_children = 0;
    for (int i = 0; i < 20; i++)
        temp->children[i] = NULL;

    temp->start_line = 0;
    temp->end_line = 0;
    return temp;
}

symbol_table_node * insertSymbolTable(symbolTable* table,
                                      char* name , bool isarr,
                                      int isdyn, symbol_table_node * d_range1,
                                      symbol_table_node* d_range2, int c_range1,
                                      int c_range2, token* lexeme, datatype type)
{
    if (table == NULL)return NULL;
    symbol_table_node * temp = searchSymbolTable(table, name);

    if (temp != NULL)return temp;

    temp = makeSymbolNode(name , isarr, isdyn, d_range1, d_range2,
                          c_range1, c_range2, lexeme, type);

    unsigned int index = hashf(name) % table->size;

    symbol_table_node* check = insertToSymChain(table->ar, temp, index);
    if (check != NULL)
        return temp;
    else
    {
        printf("ERROR: Symbol table entry could not be inserted\n");
        return NULL;
    }

}
symbol_table_node * insertSymbolTableLocal(symbolTable* table,
        char* name , bool isarr,
        int isdyn, symbol_table_node * d_range1,
        symbol_table_node* d_range2, int c_range1,
        int c_range2, token* lexeme, datatype type)
{
    if (table == NULL)return NULL;
    symbol_table_node * temp = searchSymbolTableLocal(table, name);

    if (temp != NULL)return temp;

    temp = makeSymbolNode(name , isarr, isdyn, d_range1, d_range2,
                          c_range1, c_range2, lexeme, type);

    unsigned int index = hashf(name) % table->size;

    symbol_table_node* check = insertToSymChain(table->ar, temp, index);
    if (check != NULL)
        return temp;
    else
    {
        printf("ERROR: Symbol table entry could not be inserted\n");
        return NULL;
    }

}

