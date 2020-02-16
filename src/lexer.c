#include<stdio.h>
#include "lexer.h"
#include<stdlib.h>
#include"lextable.h"
#include<string.h>
#include<stdbool.h>
#include<ctype.h>



void removeComments(char* testfile, char* cleanfile)
{
    return;
}

int getLineNumber()
{
    return line;
}


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
}


token* id()
{
    char ans[100];
    int len= 0;
    char peek;
    while(1)
    {
	if(!forflag)
	    peek = buffer0[forward];
	else
	    peek = buffer1[forward];
	forward++;
	if(peek == EOF)
	    fptr = getStream(fptr);
	else
	{
	    if(isalpha(peek) || isdigit(peek) || peek == '_')
		ans[len++] = peek;
	    else
	    {
		ans[len]=0;
		lexemeBegin = forward;
		return insertTable(table,ans,ID);
	    }
	}
    }
}


token* number()
{
    char ans[100];
    char peek;
    while(1)
    {
	if(!forflag)
	    peek = buffer0[forward];
	else
	    peek = buffer1[forward];

	if(peek == EOF)
	    fptr = getStream(fptr);
	else
	{
	
	    //make dfa for number literals
	}
    }
}


token* operation()
{
    char ans[3];
    char peek;
    while(1)
    {
	if(!forflag)
	    peek = buffer0[forward];
	else
	    peek = buffer1[forward];

	if(peek == EOF)
	    fptr = getStream(fptr);
	else
	{
	    //if else tree
	}
    }
}

token* getNextToken(char* sourcefile)
{
    //initialization code
    if(check)
    {
	check = 0;
	table = getLexTable(10);
	fptr = fopen(sourcefile, "rb");
	int count = fread(buffer0, 1, BUF,fptr);
	line = 1;
	buffer0[count]=EOF; //sentinel value to mark the end of buffer
	lexemeBegin = 0;
	forward = 0;
    }
    while(1)
    {
	//start state for the dfa
	char peek;
	if(!forflag)
	    peek = buffer0[forward];
	else
	    peek = buffer1[forward];

	switch(peek)
	{
	    case EOF:
		    fptr = getStream(fptr);
		    break;

	    case ' ': case '\t':  //ignore spaces
		    forward++;
		    break;

	    case '\n':
		    forward++;  //ignore spaces and count line number
		    line++;
		    break;
	    
	    case 'A': case 'B': case 'C': case 'D': case 'E':
	    case 'F': case 'G': case 'H': case 'I': case 'J':
	    case 'K': case 'L': case 'M': case 'N': case 'O':
	    case 'P': case 'Q': case 'R': case 'S': case 'T':
	    case 'U': case 'V': case 'W': case 'X': case 'Y':
	    case 'Z': case 'a': case 'b': case 'c': case 'd':
	    case 'e': case 'f': case 'g': case 'h': case 'i': 
	    case 'j': case 'k': case 'l': case 'm': case 'n':
	    case 'o': case 'p': case 'q': case 'r': case 's':
	    case 't': case 'u': case 'v': case 'w': case 'x':
	    case 'y': case 'z':
		    {token* temp = id();
		    if(strlen(temp->str)>20) //raise error
		    {
			printf("Token Length error: Line %d\n",line);
		    }
		    else
			return temp;
		    break;}
	    

	    case '0': case '1': case '2': case '3': case '4':    // tokenize a INT or REAL.
	    case '5': case '6': case '7': case '8': case '9':
		    {	token* temp = number();
			if(temp)
			    return temp;
			else
			{
			    printf("Invalid Number Error: Line %d\n", line);
			    lexemeBegin++;
			    forward++;
			}
			break;
		    }
	    
	    
	    case '+': case '-': case '*': case '/': case '<':   //Tokenize Other symbols.
	    case '>': case '=': case '!': case ';': case '.':
	    case ':': case ',': case '[': case ']': case '(':
	    case ')': 
		    {
			token* temp = operation();
			if(temp)
			    return temp;
			else
			{
			    printf("Invalid Operation Error: Line %d\n",line);
			    lexemeBegin++;
			    forward++;
			}
		    break;}

	    default:
		{
		    printf("Unrecognized Symbol Error: Line %d\n", line);
		    lexemeBegin++;
		    forward++;
		    break;
		}
	}
    }

}
