/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef HASHTABLE
#define HASHTABLE
#include<stdio.h>
#include "lexer.h"

typedef struct hashnode {
	char * str;
	int val;
	struct hashnode* next;
} hashnode;

typedef struct hashtable
{
	struct hashnode** ar;
	unsigned int size;

} hashtable;

//lextable expandTable(lextable table);

hashtable getHashTable(unsigned int n);

hashnode* insertTable(hashtable table, char* str, int val);

hashnode* searchTable(hashtable table, char* str);

#endif
