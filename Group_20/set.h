/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef SET_FILE
#define SET_FILE
#include<stdio.h>
#include"hashtable.h"
#include "lexerDef.h"
#include <stdbool.h>
typedef struct set
{
	hashtable set;
} set;

void insertSet(set s, tokentype t);

bool isSetMember(set s, tokentype t);

bool setUnion(set dest_set, set source_set);

bool setUnionEPS(set dest_set, set source_set);

set getSet();

void printSet(set s);
#endif
