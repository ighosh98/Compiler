/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef LEXERDEF_FILE
#define LEXERDEF_FILE
#include<stdio.h>
#include <stdbool.h>

extern char* symbol_map[];
typedef enum {ENUM_START,
              PROGRAM,
              MODULEDECLARATIONS,
              OTHERMODULES,
              DRIVERMODULE,
              MODULEDECLARATION,
              MODULEDEF,
              MODULE1,
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
              DECLARESTMT,
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
              OP2,
              FACTOR,
              N4,
              ARITHMETICORBOOLEANEXPR,
              ANYTERM,
              LOGICALOP,
              RELATIONALOP,
              CASESTMTS,
              DEFAULT1,
              N7,
              N8,
              N9,
              VALUE,
              RANGE_ARRAYS,
              N5,
              BOOLCONSTT,
              VAR_ID_NUM,
              U,
              NEW_NT,
              UNARY_OP,                          //Non-Terminals

              $,            //Sentinel
              PROGRAMT,
              INTEGER,
              REAL,
              BOOLEAN,
              OF,
              ARRAY,
              MODULE,
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
              DEFAULT,
              FALSE1,
              TRUE1,
              EPS,
              ENUM_END
             } tokentype;                                                                          //Terminals


typedef struct token {
	char* str;
	tokentype tag;
	int line_no;
} token;
#endif
