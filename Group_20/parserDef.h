/*
Group 20
Ayush Vachaspati 2016B3A70398P
Indraneel Ghosh  2016B1A70938P
G Adityan	 2016B1A70929P
*/
#ifndef PARSER_DEF
#define PARSER_DEF
#include <stdio.h>
#include "lexerDef.h"

extern bool parsing_error;

typedef  struct prodn{
    tokentype* rule;
    tokentype non_terminal;
    int size;
} prodn;

typedef struct productions{
    prodn* rules;
    int no_productions;
}productions;

#endif
