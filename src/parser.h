/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef PARSER_FILE
#define PARSER_FILE
#include<stdio.h>
#include"lexer.h"
#include"hashtable.h"
#include "parserDef.h"
#include "nary_tree.h"
Nary_tree parse_input(type start_symbol, char* sourcefile,productions grammar);

void make_str_to_enum();

productions read_grammar();

void getFirstSet(productions grammar);

void getFollowSet(productions grammar,type start_symbol);

void makeFirstAndFollow(productions grammar, type start_symbol);

void makeParsingTable(productions grammar);
#endif
