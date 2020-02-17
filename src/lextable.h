#ifndef LEXTABLE
#define LEXTABLE
#include<stdio.h>
#include "lexer.h"
typedef struct lextable
{
    struct token** ar;
    int size;

} lextable;

//lextable expandTable(lextable table);

lextable getLexTable(int n);

token* insertTable(lextable table, char* str, type tag);

token* searchTable(lextable table, char* str);

//void deleteTable(lextable table);

#endif
