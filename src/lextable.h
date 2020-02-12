#ifndef LEXTABLE
#define LEXTABLE
#include<stdio.h>
#include "lexer.h"

typedef struct lextable
{
    token** ar;
    int size;

} lextable;

//lextable expandTable(lextable table);

extern lextable getLexTable(int n);

extern token* insertTable(lextable table, char* str, type tag);

extern token* searchTable(lextable table, char* str);

//void deleteTable(lextable table);

#endif
