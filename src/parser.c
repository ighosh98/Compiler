#include "parser.h"
#include<stdio.h>
#include"hashtable.h"
#include "parserDef.h"
#include <string.h>
#include<stdlib.h>
#define NO_OF_RULES 100
hashtable strToEnum;

void make_str_to_enum()
{
    strToEnum = getHashTable(100);
    for(int i=ENUM_START+1;i<ENUM_END;i++)
        insertTable(strToEnum,symbol_map[i],i);
}

productions read_grammar()
{
    FILE* fptr = fopen("productions.txt","r");
    char buff[1000];
    int rule_count = 0;

    //allocate space to store grammar rules
    productions grammar;
    grammar.rules = (prodn *)malloc(sizeof(prodn)*NO_OF_RULES);
    
    //read line by line rules
    while(fgets(buff, 1000, fptr))
    {
	int count = 0;
	type* rule =(type*)malloc(100*sizeof(type));
	char* tok;  
	tok = strtok(buff,"\n");
	tok = strtok(buff, " "); 
	//tokenize rules into Nonterminals and Terminals
	//then conver to enum and store in prodn struct.
	while (tok != 0) {
	    rule[count] = searchTable(strToEnum,tok)->val;
	    tok = strtok(0, " "); 
	    count++;
	    
	}
	//remove extra wasted space using realloc
	rule =(type *) realloc(rule, sizeof(type)*count);
	
	//make production from given rule
	prodn temp;
	temp.rule = rule;
	//count stores the size of the production rule
	//including rhs and lhs.
	temp.size = count;
	//LHS non terminal
	temp.non_terminal = rule[0];

	//add the parsed rule into the grammar
	grammar.rules[rule_count] = temp;
	rule_count++;
    }
    // resize grammar struct to remove wasted space
    grammar.rules = (prodn*)realloc(grammar.rules,sizeof(prodn)*rule_count);
    //no of rules in grammar
    grammar.no_productions = rule_count;
  
    return grammar;
}
