#ifndef HASHTABLE
#define HASHTABLE
#include<stdio.h>
#include "lexer.h"
typedef struct hashtable
{
    struct token** ar;
    int size;

} hashtable;

//lextable expandTable(lextable table);

hashtable getHashTable(int n);

token* insertTable(hashtable table, char* str, type tag);

token* searchTable(hashtable table, char* str);

#endif
