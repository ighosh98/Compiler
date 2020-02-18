#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hash.h"
#include "hashtable.h"

hashnode *makeHashnode(char* str,int val)
{
    char * str1 = (char *)malloc((strlen(str)+1)*sizeof(char));
    strcpy(str1,str);
    hashnode * temp = (hashnode *) malloc(sizeof(hashnode));
    temp->str = str1;
    temp->val = val;
    temp->next = NULL;
    return temp;
}

hashnode* insertToChain(hashnode* ar[], hashnode* t, int index)
{
    if(ar[index]==NULL)
	ar[index] = t;
    else
    {
	t->next = ar[index];
	ar[index] = t;
    }
    return t;
}

hashnode* searchChain(hashnode* head, char* str )
{
    hashnode* temp = head;
    while(temp)
    {
	if(strcmp(temp->str,str) == 0)
	    return temp;
	temp = temp->next;
    }
    return temp;
}

hashnode* searchTable(hashtable table, char* str)
{
    int index = (int)(hashf(str)%table.size);
    return searchChain(table.ar[index], str);
}

hashtable getHashTable(int n)
{
    hashtable temp;
    temp.ar= (hashnode **)malloc(sizeof(hashnode *)*n);
    temp.size = n;
    return temp;  
}

hashnode* insertTable(hashtable table,char* str, int val)
{
    hashnode * temp = searchTable(table, str);
    
    if(temp!=NULL)return temp;
    
    temp = makeHashnode(str,val);   

    int index = (int)(hashf(str)%table.size);

    hashnode* check = insertToChain(table.ar,temp,index);
    if(check!=NULL)
	return temp;
    else
    {
	printf("ERROR: Token could not be inserted\n");
	return NULL;
    }
}
