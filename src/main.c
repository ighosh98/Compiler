#include<stdio.h>
#include"stack.h"
int main()
{
    FILE* fptr = fopen("productions.txt","r");
    char buff[100];
    while(fgets(buff, 100, fptr))
	printf("%s", buff);



}

