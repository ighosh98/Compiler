#ifndef PARSER_DEF
#define PARSER_DEF
#include <stdio.h>
#include "lexDef.h"

typedef  struct prodn{
    type* rule;
    type non_terminal;
    int size;
} prodn;

typedef struct productions{
    prodn* rules;
    int no_productions;
}productions;

#endif
