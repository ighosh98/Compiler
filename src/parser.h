#ifndef PARSER_FILE
#define PARSER_FILE
#include<stdio.h>
#include"lexer.h"
#include"hashtable.h"
#include "parserDef.h"
#include "nary_tree.h"
Nary_tree parse_input(type start_symbol, char* sourcefile);

void make_str_to_enum();

productions read_grammar();

void getFirstSet(productions grammar);

void getFollowSet(productions grammar,type start_symbol);

void makeFirstAndFollow(productions grammar, type start_symbol);

void makeParsingTable(productions grammar);
#endif
