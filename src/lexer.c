#include<stdio.h>
#include "lexer.h"
#include<stdlib.h>
#include"hashtable.h"
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include "color.h"
#define BUFFER_LEN 4096   //4k buffer length for reading source file
#define LEX_BUFFER 100    //100 char lexeme buffer. 
#define EOB -1    //sentinel to mark end of buffer
char buffer0[BUFFER_LEN+1];
char buffer1[BUFFER_LEN+1];
int lexemeBegin, forward;
bool lexflag=0, forflag=0;
FILE* fptr;
int line;
int check = 1;
hashtable lextable;                    
char* symbol_map[] = {"ENUM_START","PROGRAM", "MODULEDECLARATIONS","OTHERMODULES","DRIVERMODULE",
    "MODULEDECLARATION","MODULEDEF","MODULE1","INPUT_PLIST","N1","RET","OUTPUT_PLIST",
    "N2","DATATYPE","TYPE","RANGE","STATEMENTS","STATEMENT","IOSTMT","SIMPLESTMT","DECLARESTMT",
    "CONDITIONALSTMT","ITERATIVESTMT","VAR","WHICHID","ASSIGNMENTSTMT","MODULEREUSESTMT",
    "WHICHSTMT","LVALUEIDSTMT","LVALUEARRSTMT","EXPRESSION","INDEX","OPTIONAL","IDLIST","N3",
    "ARITHMETICEXPR","OP1","TERM","OP2","FACTOR","N4","ARITHMETICORBOOLEANEXPR","ANYTERM",
    "LOGICALOP","RELATIONALOP","CASESTMTS","DEFAULT1","N7","N8","N9","VALUE",  
    "RANGE_ARRAYS","N5","BOOLCONSTT","VAR_ID_NUM","U","NEW_NT","UNARY_OP",                          //Non-Terminals
    "$",       //sentinel
    "INTEGER","REAL","BOOLEAN","OF","ARRAY","MODULE","START","END","DECLARE","DRIVER","UNION","TAGGED","RECORD","GET_VALUE","PRINT",
    "USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK",
    "WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF",
    "COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","ID","NUM","RNUM","DEFAULT",
    "FALSE1","TRUE1","EPS","ENUM_END"};								   //Terminals


void print_ID_length_error(token* temp)
{
    blue();
    printf("Line: %d. ",temp->line_no);
    reset();
    printf("Identifier ");
    red();
    printf("\"%s\"", temp->str);
    reset();
    printf(" too long. \n");
    yellow();
    printf("(Max allowed length is 20 characters.)\n");
    reset();
}
void print_invalid_No_error(token* temp)
{
    blue();
    printf("Line: %d. ",temp->line_no);
    reset();
    printf("Number ");
    red();
    printf("\"%s\"", temp->str);
    reset();
    printf(" is not a valid number. \n");
}
void print_invalid_OP_error(token* temp)
{
    blue();
    printf("Line: %d. ",temp->line_no);
    reset();
    printf("Operation ");
    red();
    printf("\"%s\"", temp->str);
    reset();
    printf(" is not a valid operation. \n");
}


void removeComments(char* testfile, char* cleanfile)
{
    return;
}

token *makeToken(char* str,type tag, int line)
{
    char * str1 = (char *)malloc((strlen(str)+1)*sizeof(char));
    strcpy(str1,str);
    token * temp = (token *) malloc(sizeof(token));
    temp->str = str1;
    temp->tag = tag;
    temp->line_no = line;
    return temp;
}

FILE* getStream(FILE* fptr)
{
    if(forward==BUFFER_LEN) //end of buffer
    {
	if(forflag == 0)
	{
	    int count = fread(buffer1,1,BUFFER_LEN,fptr);
	    buffer1[count]=EOB ;
	    forflag = 1;
	    forward = 0;
	}
	else
	{
	    int count = fread(buffer0,1,BUFFER_LEN,fptr);
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
    char ans[LEX_BUFFER+1];
    int len= 0;
    char peek;
    hashnode* k;
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
				k = searchTable(lextable,ans);
				return k==NULL? makeToken(ans,ID,line)
				    :makeToken(ans,k->val,line);
			}
		}
    }
}


token* number()
{
    //printf("inside number\n");
    char ans[LEX_BUFFER+1];
    char peek;
	int len = 0;
	int state = 0;
    hashnode* k;
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
							red();
							printf("Wrong call. The function should not be called without a num\n");
							reset();
							return makeToken("",-1,line);
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
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,NUM,line)
							    :makeToken(ans,k->val,line);
						    
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
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,NUM,line)
							    :makeToken(ans,k->val,line);							
						}
						else
						{
							ans[len]=0;
							return makeToken(ans,-1,line);
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
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,RNUM,line)
							    :makeToken(ans,k->val,line);							
							
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
							ans[len]=0;
							return makeToken(ans,-1,line);
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
						    ans[len]=0;
						    return makeToken(ans,-1,line);

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
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,RNUM,line)
							    :makeToken(ans,k->val,line);}
						break;
				default: 
						printf("unknown state reached");
						return makeToken("",-1,line);
						break;
			}
		}
    }
}


token* operation()
{
    char ans[LEX_BUFFER+1];
    char peek;
	int state = 0;
	int len = 0;
    hashnode* k;
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
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,PLUS,line)
							    :makeToken(ans,k->val,line);							
							break;
					case '-':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,MINUS,line)
							    :makeToken(ans,k->val,line);		
							break;
					case '/':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;			
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,DIV,line)
							    :makeToken(ans,k->val,line);	
							break;
					case ';':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;		
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,SEMICOL,line)
							    :makeToken(ans,k->val,line);	
							break;
					case ',': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;			
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,COMMA,line)
							    :makeToken(ans,k->val,line);	
							break;
					case ']': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;		
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,SQBC,line)
							    :makeToken(ans,k->val,line);	
							break;
					case '[': ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;		
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,SQBO,line)
							    :makeToken(ans,k->val,line);	
							break;
					case ')':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;		
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,BC,line)
							    :makeToken(ans,k->val,line);	
							break;
					case '(':ans[len++]=peek;
							ans[len]=0;
							forward++;
							lexemeBegin = forward;		
							k = searchTable(lextable,ans);
							return k==NULL? makeToken(ans,BO,line)
							    :makeToken(ans,k->val,line);	
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
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,MUL,line)
					    :makeToken(ans,k->val,line);	
				}
				break;
			case 2:
				if(peek=='<')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,DEF,line)
					    :makeToken(ans,k->val,line);	
				}
				else if(peek == '=')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,LE,line)
					    :makeToken(ans,k->val,line);	
				}
				else
				{
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,LT,line)
					    :makeToken(ans,k->val,line);
				}
				break;
			case 3:
				if(peek=='>')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,ENDDEF,line)
					    :makeToken(ans,k->val,line);
				}
				else if(peek == '=')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,GE,line)
					    :makeToken(ans,k->val,line);	
				}
				else
				{
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,GT,line)
					    :makeToken(ans,k->val,line);
				}
				break;
			case 4:
				if(peek=='=')
				{
					ans[len++] = peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,NE,line)
					    :makeToken(ans,k->val,line);
				}
				else
				{
					ans[len]=0;
					return makeToken(ans,-1,line);

				}
				break;
			case 5:
				if(peek=='=')
				{
					ans[len++] = peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,EQ,line)
					    :makeToken(ans,k->val,line);
				}
				else
				{
					ans[len]=0;
					return makeToken(ans,-1,line);
				}
				break;
			case 6:
				if(peek=='=')
				{
					ans[len++]= peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,ASSIGNOP,line)
					    :makeToken(ans,k->val,line);
				}
				else
				{
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,COLON,line)
					    :makeToken(ans,k->val,line);
				}
				break;
			case 7:
				if(peek=='.')
				{
					ans[len++] = peek;
					forward++;
					ans[len] = 0;
					lexemeBegin = forward;
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,RANGEOP,line)
					    :makeToken(ans,k->val,line);
				}
				else
				{
					ans[len]=0;
					return makeToken(ans,-1,line);
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
					k = searchTable(lextable,ans);
					return k==NULL? makeToken(ans,COMMENTMARK,line)
					    :makeToken(ans,k->val,line);
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
	check = 1;
	fptr = fopen(sourcefile, "rb");
}

void init_lextable()
{
	lextable = getHashTable(100);

	insertTable(lextable,"integer",INTEGER);
	insertTable(lextable,"real",REAL);
	insertTable(lextable,"boolean",BOOLEAN);
	insertTable(lextable,"of",OF);
	insertTable(lextable,"array",ARRAY);
	insertTable(lextable, "start",START);
	insertTable( lextable,"end",END);
	insertTable( lextable,"declare",DECLARE);
	insertTable( lextable,"module",MODULE);
	insertTable(lextable, "driver",DRIVER);
	insertTable(lextable, "program",PROGRAM);
	insertTable( lextable,"record",RECORD);
	insertTable( lextable,"tagged",TAGGED);
	insertTable( lextable,"union",UNION);
	insertTable(lextable, "get_value",GET_VALUE);
	insertTable(lextable, "print",PRINT);
	insertTable(lextable, "use",USE);
	insertTable(lextable,"with",WITH);
	insertTable(lextable, "parameters",PARAMETERS);
	insertTable(lextable, "true",TRUE1);
	insertTable( lextable,"false",FALSE1);
	insertTable(lextable,"takes",TAKES);
	insertTable(lextable, "input",INPUT);
	insertTable(lextable, "returns",RETURNS);
	insertTable(lextable, "AND",AND);
	insertTable(lextable, "OR",OR);
	insertTable(lextable, "for",FOR);
	insertTable(lextable, "in",IN);
	insertTable(lextable, "switch",SWITCH);
	insertTable(lextable, "case",CASE);
	insertTable(lextable, "break",BREAK);
	insertTable(lextable, "default",DEFAULT);
	insertTable(lextable, "while",WHILE);
	
}

token* getNextToken()
{
    //initialization code
    if(!fptr)
    {
	red();
	printf("Error:");
	reset();
	printf(" Source File Not Open\n");
	yellow();
	printf("Check if File Exists\nCheck if openfile function is called.\n");
	reset();
	exit(1);
    	return NULL;
    }	
    if(check)
    {
		check = 0;
		init_lextable();
		int count = fread(buffer0, 1, BUFFER_LEN,fptr);
		line = 1;
		buffer0[count]=EOB ; //sentinel value to mark the end of buffer
		lexemeBegin = 0;
		forward = 0;
    }
    while(1)
    {
	if(!fptr)
	    return makeToken("",$,line);
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

	    case ' ': case '\t': case'\v':  //ignore spaces
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
				    print_ID_length_error(temp);
				}
				else
					return temp;
				break;
			}
	    

	    case '0': case '1': case '2': case '3': case '4':    // tokenize a INT or REAL.
	    case '5': case '6': case '7': case '8': case '9':
		    {	token* temp = number();
			if(temp->tag != -1)
			    return temp;
			else
			{
			    print_invalid_No_error(temp);
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
			if(temp->tag!=-1)
			{
			    if(temp->tag == COMMENTMARK)
			    {
				//yellow();
				//printf("Ignoring Comment\n");
				//reset();
			    }
			    else
				return temp;
			}
			else
			{
			    print_invalid_OP_error(temp);
			    lexemeBegin = forward;
			}
		    break;}

	    default:
		{
		    blue();
		    printf("Line: %d. ", line);
		    reset();
		    printf("The symbol ");
		    red();
		    printf("\"%c\"",peek);
		    reset();
		    printf(" is not recognized in this context.\n");

		    lexemeBegin++;
		    forward++;
		    break;
		}
	}
    }

}
