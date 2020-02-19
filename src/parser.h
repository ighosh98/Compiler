#ifndef PARSER_FILE
#define PARSER_FILE
#include<stdio.h>
#include"lexer.h"
#include"hashtable.h"
#include "parserDef.h"
void make_str_to_enum();

productions read_grammar();

void getFirstSet(productions grammar);

void getFollowSet(productions grammar,type start_symbol);
#endif
