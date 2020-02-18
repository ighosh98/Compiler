#ifndef HASHTABLE
#define HASHTABLE
#include<stdio.h>
#include "lexer.h"

typedef struct hashnode{
    char * str;
    int val;
    struct hashnode* next;
} hashnode;

typedef struct hashtable
{
    struct hashnode** ar;
    int size;

} hashtable;

//lextable expandTable(lextable table);

hashtable getHashTable(int n);

hashnode* insertTable(hashtable table, char* str, int val);

hashnode* searchTable(hashtable table, char* str);

#endif