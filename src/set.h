#ifndef SET_FILE
#define SET_FILE
#include<stdio.h>
#include"hashtable.h"
#include "lexDef.h"
#include <stdbool.h>
typedef struct set
{
    hashtable set;
} set;

void insertSet(set s, type t);

bool isSetMember(set s, type t);

void setUnion(set dest_set, set source_set);

set getSet();

void printSet(set s);
#endif
