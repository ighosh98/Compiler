/*
   Group 20
   Ayush Vachaspati 2016B3A70398P
   Indraneel Ghosh  2016B1A70938P
   G Adityan     2016B1A70929P
*/

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include "lexerDef.h"
#include "lexer.h"
#include "stdbool.h"
#include "astdef.h"
typedef struct symbol_table_node {
    char name[21];
    bool isarr;
    int isdynamic;
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

    int stackSize;
    int offset;


} symbol_table_node;



typedef struct symbolTable {
    struct symbol_table_node** ar;
    struct symbolTable* parent;
    unsigned int size;

    struct symbolTable* children[50];
    int no_children;

    int start_line;
    int end_line;

} symbolTable;

void deleteSymbolTable(symbolTable* table);
void printSymbolNode(symbol_table_node * a, symbol_table_node* curr_func, int nesting, int start, int end);
void printSymbolTables(symbolTable* table, int nesting);
void printSymbolNodeArr(symbol_table_node * a, symbol_table_node* curr_func, int nesting, int start, int end);
void printSymbolTablesArr(symbolTable* table, int nesting);
symbol_table_node * makeSymbolNode(char* name , bool isarr,
                                   int isdyn, symbol_table_node * d_range1,
                                   symbol_table_node* d_range2, int c_range1,
                                   int c_range2, token* lexeme, datatype type);

symbol_table_node* insertToSymChain(symbol_table_node* ar[],
                                    symbol_table_node* t, unsigned int index);

symbol_table_node* searchSymChain(symbol_table_node* head, char* str );

symbol_table_node* searchSymbolTable(symbolTable* table, char* str);

symbol_table_node* searchSymbolTableLocal(symbolTable* table, char* str);

symbolTable* getSymbolTable(unsigned int n);

symbol_table_node * insertSymbolTable(symbolTable* table,
                                      char* name , bool isarr,
                                      int isdyn, symbol_table_node * d_range1,
                                      symbol_table_node* d_range2, int c_range1,
                                      int c_range2, token* lexeme, datatype type);
symbol_table_node * insertSymbolTableLocal(symbolTable* table,
        char* name , bool isarr,
        int isdyn, symbol_table_node * d_range1,
        symbol_table_node* d_range2, int c_range1,
        int c_range2, token* lexeme, datatype type);

#endif
