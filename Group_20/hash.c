/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
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
