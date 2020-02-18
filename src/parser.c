#include "parser.h"
#include<stdio.h>
#include"hashtable.h"
#include "parserDef.h"

hashtable strToEnum;

void make_str_to_enum()
{
    printf("\n\n");
    strToEnum = getHashTable(100);
    printf("62: %s\n",symbol_map[62]);
    for(int i=ENUM_START+1;i<ENUM_END;i++)
    {
	printf("%d\n",i);
        insertTable(strToEnum,symbol_map[i],i);
    }
}


