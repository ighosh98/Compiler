#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include "lexerDef.h"
#include "lexer.h"
#include "stdbool.h"
#include "astdef.h"
typedef struct symbol_table_node{
    char name[21];
    bool isarr;
    bool isdynamic;
    struct symbol_table_node * drange1;
    struct symbol_table_node * drange2;
    int crange1;
    int crange2;

    bool isUsed;
    bool isDefined;
    struct symbol_table_node* iplist;
    struct symbol_table_node* oplist;
    token * lexeme;
    datatype type;
    struct symbol_table_node* next;

} symbol_table_node;



typedef struct symbolTable{
    struct symbol_table_node** ar;
    struct symbolTable* parent;
    unsigned int size;
} symbolTable;

void deleteSymbolTable(symbolTable* table);
void printSymbolNode(symbol_table_node * a);

symbol_table_node * makeSymbolNode(char* name , bool isarr,
       	bool isdyn, symbol_table_node * d_range1, 
	symbol_table_node* d_range2, int c_range1, 
	int c_range2,token* lexeme, datatype type);

symbol_table_node* insertToSymChain(symbol_table_node* ar[],
       	symbol_table_node* t, unsigned int index);

symbol_table_node* searchSymChain(symbol_table_node* head, char* str );

symbol_table_node* searchSymbolTable(symbolTable* table, char* str);

symbolTable* getSymbolTable(unsigned int n);

symbol_table_node * insertSymbolTable(symbolTable* table, 
	char* name , bool isarr,
	bool isdyn, symbol_table_node * d_range1, 
	symbol_table_node* d_range2, int c_range1, 
	int c_range2,token* lexeme, datatype type);
#endif
