#include<stdio.h>
#include"hash.h"

unsigned int hashf(char* str)
{
    unsigned int ans = 5381;
    char c = *str;
    while(c!='\0')
    {
	ans = ((ans<<5)+ans)+c;
	str++;
	c = *str;
    }
    return ans;
}
