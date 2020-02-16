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

struct lextable getLexTable(int n);

struct token* insertTable(lextable table, char* str, type tag);

struct token* searchTable(lextable table, char* str);

//void deleteTable(lextable table);

#endif
