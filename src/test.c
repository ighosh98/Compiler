#include <stdio.h>
#include<stdbool.h>

/*#define BUF 4096
static char buffer0[BUF+1];
static char buffer1[BUF+1];
static int lexemeBegin, forward;
static bool lexflag=0, forflag=0;
static FILE* fptr;
FILE* getStream(FILE* fptr)
{
    if(forward==BUF) //end of buffer
    {
	if(forflag == 0)
	{
	    int count = fread(buffer1,1,BUF,fptr);
	    buffer1[count]=EOF;
	    forflag = 1;
	    forward = 0;
	}
	else
	{
	    int count = fread(buffer0,1,BUF,fptr);
	    buffer0[count]=EOF;
	    forflag = 0;
	    forward = 0;
	}
	return fptr;
    }
    else
    {
	fclose(fptr);
	return NULL; //terminate lexical analysis
    }	
}*/
int main()
{
    buffer0[BUF]=EOF;
    buffer1[BUF]=EOF;
    fptr = fopen("abc.txt","rb");
    fread(buffer0,1,BUF,fptr);
    while(fptr)
    {
	if(forflag)
	{
	    printf("\n\n\n BUFFER1");
	    while(buffer1[forward]!=EOF)
		printf("%c ",buffer1[forward++]);
	    fptr = getStream(fptr);
	}
	else
	{
	    printf("\n\n\n BUFFER0");
	     while(buffer0[forward]!=EOF)
		printf("%c ",buffer0[forward++]);
	    fptr = getStream(fptr);
	}

    }
}
