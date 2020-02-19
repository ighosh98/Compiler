#include<stdio.h>
#include "hashtable.h"
#include "set.h"
#include "lexDef.h"
#include<stdbool.h>
#include "lexer.h"
void insertSet(set s, type a)
{
    insertTable(s.set,symbol_map[a],a); //insert (Token, enum of token) into the set.
}   

bool isSetMember(set s, type a)
{
    if(searchTable(s.set,symbol_map[a])!=NULL)
	return true;
    else
	return false;
}
bool setUnion(set dest_set, set source_set)
{
    hashtable h = source_set.set;
    bool flag = false;
    for(int i=0;i<h.size;i++)
    {
	hashnode* temp = h.ar[i];
	while(temp)
	{
	    if(!isSetMember(dest_set,temp->val))
	    {
		insertSet(dest_set, temp->val);
		flag = true;
	    }
	    temp=temp->next; 
	}
    }
    return flag;
}

set getSet()
{
    set s;
    s.set = getHashTable(100);
    return s;
}

void printSet(set s)
{
    hashtable h = s.set;
    printf("{");
    for(int i=0;i<h.size;i++)
    {
	hashnode* temp = h.ar[i];
	while(temp)
	{
	    printf("%s,",temp->str);
	    temp=temp->next; 
	}
    }
    printf("}\n");
}
