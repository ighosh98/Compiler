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
		
		if(peek == EOF)
			fptr = getStream(fptr);
		else
		{
			if(isalpha(peek) || isdigit(peek) || peek == '_')
				ans[len++] = peek, forward++;
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
	int len = 0;
	int state = 0;
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
			switch(state)
			{
				case 0: if(isdigit(peek))
						{
							ans[len++] = peek; 
							forward++;
							state = 1;
						}
						else
						{
							printf("Wrong call. The function should not be called without a num\n");
							return NULL;
						}
						break;
				case 1: if(isdigit(peek))
						{
							ans[len++] = peek; 
							forward++;
							state = 1;
						}
						else if(peek == '.')
						{
							ans[len++] = peek;
							forward++;
							state = 2;
						}
						else
						{
							
							ans[len]=0;
							lexemeBegin = forward;
							return insertTable(table,ans,NUM);
						}
						break;
				case 2: if(isdigit(peek))
						{
							ans[len++] = peek; 
							forward++;
							state = 3;
						}
						else if(peek == '.')
						{
							forward--;
							ans[--len]=0;
							lexemeBegin = forward;
							return insertTable(table,ans,NUM);
						}
						else
						{
							return NULL;
						}
						break;
				case 3: if(isdigit(peek))
						{
							ans[len++] = peek; 
							forward++;
							state = 3;
						}
						else if(peek == 'e' || peek == 'E')
						{
							ans[len++]=peek;
							forward++;
							state = 4;
						}
						else
						{
							ans[len]=0;
							lexemeBegin = forward;
							return insertTable(table,ans, RNUM);
						}
						break;
				case 4: if(peek=='+' || peek=='-')
						{
							ans[len++] = peek; 
							forward++;
							state = 5;
						}
						else if(isdigit(peek))
						{
							ans[len++]=peek;
							forward++;
							state = 6;
						}
						else
						{
							return NULL;
						}
						break;
				case 5: if(isdigit(peek))
						{
							ans[len++] = peek; 
							forward++;
							state = 6;
						}
						else
						{
							return NULL;
						}
						break;
				case 6: if(isdigit(peek))
						{
							ans[len++] = peek; 
							forward++;
							state = 6;
						}
						else
						{
							ans[len]=0;
							lexemeBegin = forward;
							return insertTable(table,ans, RNUM);
						}
						break;
				default: 
						printf("unknown state reached");
						return NULL;
						break;
			}
    }
}


token* operation()
{
    char ans[3];
    char peek;
	int state = 0;
	int len = 0;
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
	    switch(state)
		{
			case 0: 
			{
				switch(peek)
				{
					case '*': ans[len++]=peek;
							  forward++;
							  state = 1;
							  break;
					case '<':ans[len++]=peek;
							  forward++;
							  state = 2;
							  break;
					case '>':ans[len++]=peek;
							  forward++;
							  state = 3;
							  break;
					case '!':ans[len++]=peek;
							  forward++;
							  state = 4;
							  break;
					case '=':ans[len++]=peek;
							  forward++;
							  state = 5;
							  break;
					case ':':ans[len++]=peek;
							  forward++;
							  state = 6;
							  break;
					case '.':ans[len++]=peek;
							  forward++;
							  state = 7;
							  break;
					
					
					
					case '+':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case '-':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case '/':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case ';':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case ',': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case ']': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case '[': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case ')':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
					case '(':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, Token);
							break;
				}
			}
			break;
			case 1:
				if(peek=='*')
				{
					ans[len++]=peek;
					forward++;
					state=8;					
				}
				else
				{
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table,ans,MULTIPLY);
				}
			case 2:
				if(peek=='<')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, DEF);
				}
				else if(peek == '=')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, LE);	
				}
				else
				{
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, LT);
				}
				break;
			case 3:
				if(peek=='>')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, ENDDEF);
				}
				else if(peek == '=')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, GE);	
				}
				else
				{
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, GT);
				}
				break;
			case 4:
				if(peek=='=')
				{
					ans[len++] = peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, NE);
				}
				else
				{
					return NULL;
				}
				break;
			case 5:
				if(peek=='=')
				{
					ans[len++] = peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, EQ);
				}
				else
				{
					return NULL;
				}
				break;
			case 6:
				if(peek=='=')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, ASSIGNOP);
				}
				else
				{
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, COLON);
				}
				break;
			case 7:
				if(peek=='.')
				{
					ans[len++] = peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					return insertTable(table, ans, RANGEOP);
				}
				else
				{
					return NULL;
				}
				break;
			case 8:
				if(peek=='*')
				{
					forward++;
					state = 9;
				}
				else
				{
					forward++;
				}
				break;
			case 9: 
				if(peek == '*')
				{
					forward++;
					return insertTable(table, ans, COMMENTMARK);
				}
				else
				{
					forward++;
					state = 8;
				}
				break;
		}
	}
    }
}

void openfile(char* sourcefile)
{
	fptr = fopen(sourcefile, "rb");
}

token* getNextToken()
{
    //initialization code
	if(!fptr)
	{
		printf("file not open\n");
		exit(1);
	}	
    if(check)
    {
		check = 0;
		table = getLexTable(100);
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
		    {
				token* temp = id();
				if(strlen(temp->str)>20) //raise error
				{
				printf("Token Length error: Line %d\n",line);
				}
				else
					return temp;
				break;
			}
	    

	    case '0': case '1': case '2': case '3': case '4':    // tokenize a INT or REAL.
	    case '5': case '6': case '7': case '8': case '9':
		    {	token* temp = number();
			if(temp)
			    return temp;
			else
			{
			    printf("Invalid Number Error: Line %d\n", line);
			    lexemeBegin = forward;
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
			    lexemeBegin = forward;
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
