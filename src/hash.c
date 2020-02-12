#include<stdio.h>
#include"hash.h"

unsigned long hashf(char* str)
{
    unsigned long ans = 5381;
    char c = *str;
    while(c!='\0')
    {
	ans = ((ans<<5)+ans)+c;
	str++;
	c = *str;
    }
    return ans;
}
