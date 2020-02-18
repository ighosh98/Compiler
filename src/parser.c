#include "parser.h"
#include<stdio.h>
#include"hashtable.h"
#include "parserDef.h"

hashtable strToEnum;

void make_str_to_enum()
{
    strToEnum = getHashTable(100);
    for(int i=ENUM_START+1;i<ENUM_END;i++)
        insertTable(strToEnum,symbol_map[i],i);
}


