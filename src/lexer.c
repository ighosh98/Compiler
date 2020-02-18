#include<stdio.h>
#include "lexer.h"
#include<stdlib.h>
#include"hashtable.h"
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#define BUF 4096
#define EOB -1    //sentinel to mark end of buffer
char buffer0[BUF+1];
char buffer1[BUF+1];
int lexemeBegin, forward;
bool lexflag=0, forflag=0;
FILE* fptr;
int line;
int check = 1;
hashtable table;                    


char* symbol_map[] = {"PROGRAM", "MODULEDECLARATIONS","OTHERMODULES","DRIVERMODULE",
    "MODULEDECLARATION","MODULEDEF","MODULE","INPUT_PLIST","N1","RET","OUTPUT_PLIST",
    "N2","DATATYPE","TYPE","RANGE","STATEMENTS","STATEMENT","IOSTMT","SIMPLESTMT","DECLARSTMT",
    "CONDITIONALSTMT","ITERATIVESTMT","VAR","WHICHID","ASSIGNMENTSTMT","MODULEREUSESTMT",
    "WHICHSTMT","LVALUEIDSTMT","LVALUEARRSTMT","EXPRESSION","INDEX","OPTIONAL","IDLIST","N3",
    "ARITHMETICEXPR","OP1","TERM","OP2","FACTOR","N4","ARITHMETICORBOOLEANEXPR","ANYTERM",
    "LOGICALOP","RELATIONALOP","CASESTMTS","DEFAULT","N7","N8","N9","VALUE",                            //Non-Terminals
    "$",
    "INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","DRIVER","GET_VALUE","PRINT",
    "USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK",
    "WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF",
    "COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","ID","NUM","RNUM",
    "EPS","FALSE1","TRUE1"};                                                                          //Terminals


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
	    buffer1[count]=EOB ;
	    forflag = 1;
	    forward = 0;
	}
	else
	{
	    int count = fread(buffer0,1,BUF,fptr);
	    buffer0[count]=EOB ;
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
 //   printf("inside id\n");
    char ans[100];
    int len= 0;
    char peek;

    while(1)
    {
		if(!forflag)
			peek = buffer0[forward];
		else
			peek = buffer1[forward];
		
		if(peek == EOB )
			fptr = getStream(fptr);
		else
		{
			if(isalpha(peek) || isdigit(peek) || peek == '_')
			{
			    ans[len++] = peek; 
			    forward++;
			}
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
    //printf("inside number\n");
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

		if(peek == EOB )
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
}


token* operation()
{
    //printf("inside operation\n");
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
	if(peek == EOB )
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
							return insertTable(table, ans, PLUS);
							break;
					case '-':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, MINUS);
							break;
					case '/':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, DIV);
							break;
					case ';':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, SEMICOL);
							break;
					case ',': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, COMMA);
							break;
					case ']': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, SQBC);
							break;
					case '[': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, SQBO);
							break;
					case ')':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, BC);
							break;
					case '(':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;							
							return insertTable(table, ans, BO);
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
					return insertTable(table,ans,MUL);
				}
				break;
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
					if(peek == '\n')
					    line++;
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

void init_lextable()
{
	table = getHashTable(100);

	insertTable(table,"declare",DECLARE);
	insertTable(table, "driver",DRIVER);
	insertTable(table, "program",PROGRAM);
	insertTable(table, "for",FOR);
	insertTable(table, "start",START);
	insertTable( table,"end",END);
	insertTable(table, "module",MODULE);
	insertTable(table, "get_value",GET_VALUE);
	insertTable(table, "print",PRINT);
	insertTable(table, "use",USE);
	insertTable(table,"with",WITH);
	insertTable(table, "parameters",PARAMETERS);
	insertTable(table, "true",TRUE1);
	insertTable( table,"false",FALSE1);
	insertTable(table,"takes",TAKES);
	insertTable(table, "input",INPUT);
	insertTable(table, "returns",RETURNS);
	insertTable(table, "AND",AND);
	insertTable(table, "OR",OR);
	insertTable(table, "switch",SWITCH);
	insertTable(table, "case",CASE);
	insertTable(table, "break",BREAK);
	insertTable(table, "default",DEFAULT);
	insertTable(table, "while",WHILE);
	
}

token* getNextToken()
{
    //initialization code
    if(!fptr)
    {
	printf("Error: Source File Not Open\nCheck if File Exists\nCheck if openfile function is called.\n");
	exit(1);
    	return NULL;
    }	
    if(check)
    {
		check = 0;
		init_lextable();
		int count = fread(buffer0, 1, BUF,fptr);
		line = 1;
		buffer0[count]=EOB ; //sentinel value to mark the end of buffer
		lexemeBegin = 0;
		forward = 0;
    }
    while(1)
    {
	if(!fptr)
	    return NULL;
	//start state for the dfa
	char peek;
	if(!forflag)
	    peek = buffer0[forward];
	else
	    peek = buffer1[forward];

	switch(peek)
	{
	    case EOB :
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
			{
			    if(temp->tag == COMMENTMARK)
			    {
				printf("Ignoring Comment\n");
			    }
			    else
				return temp;
			}
			else
			{
			    printf("Invalid Operation Error: Line %d\n",line);
			    lexemeBegin = forward;
			}
		    break;}

	    default:
		{
		    printf("Invalid Symbol Error: Line %d\n", line);
		    lexemeBegin++;
		    forward++;
		    break;
		}
	}
    }

}
