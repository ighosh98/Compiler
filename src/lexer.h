#ifndef LEXER_FILE
#define LEXER_FILE
#include<stdio.h>
#include <stdbool.h>
#include "lextable.h"

#define BUF 4096

typedef enum{
PROGRAM,
MODULEDECLARATIONS,
OTHERMODULES,
DRIVERMODULE,
MODULEDECLARATION,
MODULEDEF,
MODULE,
INPUT_PLIST,
N1,
RET,
OUTPUT_PLIST,
N2,
DATATYPE,
TYPE,
RANGE,
STATEMENTS,
STATEMENT,
IOSTMT,
SIMPLESTMT,
DECLARSTMT,
CONDITIONALSTMT,
ITERATIVESTMT,
VAR,
WHICHID,
ASSIGNMENTSTMT,
MODULEREUSESTMT,
WHICHSTMT,
LVALUEIDSTMT,
LVALUEARRSTMT,
EXPRESSION,
INDEX,
OPTIONAL,
IDLIST,
N3,
ARITHMETICEXPR,
OP1,
TERM,
//arithmeticExprprime,ARITHMETICEXPR
OP2,
FACTOR,
N4,
ARITHMETICORBOOLEANEXPR,
ANYTERM,
LOGICALOP,
RELATIONALOP,
CASESTMTS,
DEFAULT,
N7,
N8,
N9,
VALUE,
$,
INTEGER,
REAL,
BOOLEAN,
OF,
ARRAY,
START,
END,
DECLARE,
DRIVER,
GET_VALUE,
PRINT,
USE,
WITH,
PARAMETERS,
TAKES,
INPUT,
RETURNS,
AND,
OR,
FOR,
IN,
SWITCH,
CASE,
BREAK,
WHILE,
PLUS,
MINUS,
MUL,
DIV,
LT,
LE,
GE,
GT,
EQ,
NE,
DEF,
ENDDEF,
DRIVERDEF,
DRIVERENDDEF,
COLON,
RANGEOP,
SEMICOL,
COMMA,
ASSIGNOP,
SQBO,
SQBC,
BO,
BC,
COMMENTMARK,
ID,
NUM,
RNUM,
eps
}type;                    



typedef struct token{
    char* str;
    type tag;
    struct token * next;    
}token;
char buffer0[BUF+1];
char buffer1[BUF+1];
int lexemeBegin, forward;
bool lexflag=0, forflag=0;
FILE* fptr;
int line;
int check = 1;
lextable table;                    
token *getNextToken();

FILE* getStream(FILE*);

void removeComments(char* testfile, char* cleanfile);

int getLineNumber();
#endif
