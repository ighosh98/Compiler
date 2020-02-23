#include "parser.h"
#include<stdio.h>
#include"hashtable.h"
#include "parserDef.h"
#include <string.h>
#include<stdlib.h>
#include "lexDef.h"
#include <stdbool.h>
#include "set.h"
#include "color.h"
#include "stack.h"
#define DEBUG 1
#define RULES_BUFF 200

hashtable strToEnum;
set* nonterminal_FirstSet;
set* First_Set;
set* nonterminal_FollowSet;
prodn parsing_table[$][ENUM_END-$+1];

void printTerminalError(treenode * X, token* a)
{
    blue();
    printf("Line %d: ",a->line_no);
    reset();
    printf("Error Processing Terminal ");
    red();
    printf("%s ",symbol_map[X->tok]);
    reset();
    printf("and Terminal ");
    red();
    printf("%s. ",symbol_map[a->tag]);
    reset();
    printf("Check ");
    red();
    printf("\"%s\"\n",a->str);
    yellow();
    printf("Non-Matching Terminals\n");
    reset();

}
void printNonTerminalError(treenode * X, token* a)
{
    blue();
    printf("Line %d: ",a->line_no);
    reset();
    printf("Error Processing Non-Terminal ");
    red();
    printf("%s ",symbol_map[X->tok]);
    reset();
    printf("and Terminal ");
    red();
    printf("%s. ",symbol_map[a->tag]);
    reset();
    printf("Check ");
    red();
    printf("\"%s\"\n",a->str);
    yellow();
    printf("No Rule In Parsing Table\n");
    reset();

}


void printProduction(prodn p)
{
    
    printf("{%s ----> ",symbol_map[p.rule[0]]);
    for(int j=1;j<p.size;j++)
	printf("%s ",symbol_map[p.rule[j]]);
    printf("}\n");
}


bool isterminal(type t)
{
    return ((t>=$) && (t<ENUM_END));
}

bool isNonterminal(type t)
{
    return ((t>ENUM_START)&&(t<$));
}

void make_str_to_enum()
{
    strToEnum = getHashTable(100);
    for(int i=ENUM_START+1;i<ENUM_END;i++)
        insertTable(strToEnum,symbol_map[i],i);
}

void makeFirstAndFollow(productions grammar, type start_symbol)
{
    getFirstSet(grammar);
    getFollowSet(grammar, start_symbol);
}
bool isEmptyProduction(prodn p)
{
	if(p.size == 2 && p.rule[1]==EPS)
	    return true;

	for(int i =1;i<p.size;i++)
	{
	    if(isterminal(p.rule[i]) && p.rule[i]!=EPS)
	    {
		return false;
	    }
	    else if(!isSetMember(nonterminal_FirstSet[p.rule[i]],EPS))
	    {
	        return false;
	
	    }
	}	    
	return true;
}
void getFirstSet(productions grammar)
{
    nonterminal_FirstSet = (set *)malloc(sizeof(set)*($));
    for(int i=0;i<$;i++)
	nonterminal_FirstSet[i] = getSet();
    
    First_Set = (set *)malloc(sizeof(set)*(grammar.no_productions));
    for(int i =0;i<grammar.no_productions;i++)
	First_Set[i] = getSet();

    bool updated = true;
    while(updated)
    {
	updated = false;

	//for every production in the grammar
	for(int i=0;i<grammar.no_productions;i++)
	{
	    prodn temp = grammar.rules[i];  //one production rule at a time
	    for(int j=1;j<temp.size;j++) //start j from 1.
	    {
		if(isterminal(temp.rule[j]))
		{
			if(!isSetMember(First_Set[i],temp.rule[j]))
			{
			    insertSet(First_Set[i],temp.rule[j]);			
			    updated = true;
			}
			if(!isSetMember(nonterminal_FirstSet[temp.non_terminal],temp.rule[j]))
			{
			    insertSet(nonterminal_FirstSet[temp.non_terminal],temp.rule[j]);
			    updated = true;
			}
			break;	
		}
		else if(isNonterminal(temp.rule[j]))
		{

		    if(setUnionEPS(First_Set[i],nonterminal_FirstSet[temp.rule[j]]))
			updated = true;
		    
		    if(setUnionEPS(nonterminal_FirstSet[temp.non_terminal],nonterminal_FirstSet[temp.rule[j]]))
			updated = true;
		    
		    if(!isSetMember(nonterminal_FirstSet[temp.rule[j]],EPS))
		    {
		    break;
		    }
		}
		else
		{
		    printf("INCORRECT SYMBOL\n");
		    return;
		}

	    }	
	    

	    if(isEmptyProduction(temp))
	    {
		insertSet(First_Set[i],EPS);
		insertSet(nonterminal_FirstSet[temp.non_terminal],EPS);
	    }
	}

    }

    //############### print First Sets ######################
    if(DEBUG){
	red();
	printf("\n\n##################### First Sets of Non-Terminals ########################\n");
	reset();
	for(int i=ENUM_START+1;i<$;i++)
	{
	    blue();
	    printf("%s ---> ",symbol_map[i]);
	    reset();
	    printSet(nonterminal_FirstSet[i]);
	}
	red();
	printf("\n\n##################### First Sets of Grammar Rules. #######################\n");
	reset();
	for(int i =0;i<grammar.no_productions;i++)
	{
	    blue();
	    printProduction(grammar.rules[i]);
	    reset();
	    printSet(First_Set[i]);
	}
    }
}


void getFollowSet(productions grammar,type start_symbol)
{
    nonterminal_FollowSet = (set *)malloc(sizeof(set)*($));
    for(int i=0;i<$;i++)
	nonterminal_FollowSet[i] = getSet();

    //rule1
    insertSet(nonterminal_FollowSet[start_symbol],$);

    //rule2
    bool updated = true;
    while(updated)
    {
	updated = false;
    for(int i=0;i<grammar.no_productions;i++)
    {
	    prodn temp = grammar.rules[i];
	    for(int j=temp.size-2;j>=1;j--) //start from 2nd last symbol 
	    {
		if(isterminal(temp.rule[j+1]))
		{
		    //if terminal then just at that and you are done.
		    if(isNonterminal(temp.rule[j]))
		    {
			if(!isSetMember(nonterminal_FollowSet[temp.rule[j]],temp.rule[j+1]))
			{
			    insertSet(nonterminal_FollowSet[temp.rule[j]],temp.rule[j+1]);
			    updated = true;
			}
		    }
		} 
		else if(isNonterminal(temp.rule[j+1]))
		{
		    if(isNonterminal(temp.rule[j]))
		    {
			//if non terminal then add the first set of j+1 into the follow set of j.
			if(setUnionEPS(nonterminal_FollowSet[temp.rule[j]],nonterminal_FirstSet[temp.rule[j+1]]))
			    updated = true;
			if(isSetMember(nonterminal_FirstSet[temp.rule[j+1]],EPS))
			{
			    if(setUnionEPS(nonterminal_FollowSet[temp.rule[j]],nonterminal_FollowSet[temp.rule[j+1]]))
				updated = true;
			}
		    }
		}
		else
		{
		    printf("INCORRECT SYMBOL\n");
		    return;
		}
	    }
   }
    }
    updated = true;
    while(updated)
    {
	updated = false;
	for(int i=0;i<grammar.no_productions;i++)
	{
	    prodn temp = grammar.rules[i];  //one production rule at a time
	    for(int j=temp.size-1;j>=1;j--) 
	    {
		if(isterminal(temp.rule[j]))
		{
		    break;
		}
		else if(isNonterminal(temp.rule[j]))
		{

		    if(setUnionEPS(nonterminal_FollowSet[temp.rule[j]],nonterminal_FollowSet[temp.rule[0]]))
			updated = true;
		    if(!isSetMember(nonterminal_FirstSet[j],EPS))
			break;
		}
		else
		{
		    printf("INCORRECT SYMBOL\n");
		    return;
		}
	    }
	}
    }

    //####################### print Follow Set ############################
    if(DEBUG){
	red();
	printf("\n\n####################### Follow Sets of Non-Terminals ########################\n");
	reset();
	for(int i=ENUM_START+1;i<$;i++)
	{
	    blue();
	    printf("%s --> ",symbol_map[i]);
	    reset();
	    printSet(nonterminal_FollowSet[i]);
	}
    }
}

productions read_grammar()
{
    FILE* fptr = fopen("productions.txt","r");
    char buff[1000];
    int rule_count = 0;

    //allocate space to store grammar rules
    productions grammar;
    grammar.rules = (prodn *)malloc(sizeof(prodn)*RULES_BUFF);
    
    //read line by line rules
    while(fgets(buff, 1000, fptr))
    {
	int count = 0;
	type* rule =(type*)malloc(RULES_BUFF*sizeof(type));
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


void makeParsingTable(productions grammar)
{
    //initialize all the elements to error entries
    for(int i=0;i<$;i++)
	for(type j=$;j<= ENUM_END;j++)
	{
	    parsing_table[i][j-$].rule = NULL;
	    parsing_table[i][j-$].size=0;
	    parsing_table[i][j-$].non_terminal = -1;
	}

    for(int i=0;i<grammar.no_productions;i++)
    {
	bool rule_2_flag = false;
        prodn rule = grammar.rules[i];  //one production rule at a time
	
	hashtable h = First_Set[i].set;  //First Set of ith grammar rule

	if(isSetMember(First_Set[i],EPS))
	    rule_2_flag = true;;

	//for each nonterminal in first set of production
	for(int j=0;j<h.size;j++)
	{
	    hashnode* terminal = h.ar[j];
	    while(terminal)
	    {
		if(terminal->val!=EPS)
		{
		    
		    if(parsing_table[rule.non_terminal][terminal->val-$].rule != NULL)
		    {
			yellow();
			printf("1) overwriting rule in parsing table(First Set)\n"); 
			reset();
			printf("%s, %s\n",symbol_map[rule.non_terminal],symbol_map[terminal->val]);
			printProduction(parsing_table[rule.non_terminal][terminal->val-$] );
			printProduction(rule);
		    }
		    //assign production to parsing table entry corresponding to (Non_Terminal, terminal in first set of rule)
		    parsing_table[rule.non_terminal][terminal->val-$].rule = rule.rule; //convert nonterminal to 0 base indexing
	    parsing_table[rule.non_terminal][terminal->val-$].size = rule.size;
		    parsing_table[rule.non_terminal][terminal->val-$].non_terminal = rule.non_terminal;
		}			
		terminal = terminal->next; 
	    }
	}

	//add to follow sets
	if(rule_2_flag)
	{
	    hashtable h = nonterminal_FollowSet[rule.non_terminal].set;
	    //for each nonterminal in follow set of production
	    for(int j=0;j<h.size;j++)
	    {
		hashnode* terminal = h.ar[j];
		while(terminal)
		{
		    if(parsing_table[rule.non_terminal][terminal->val-$].rule != NULL)
		    {
			yellow();
			printf("2) overwriting rule in parsing tablei(Follow Set)\n");
			reset();
			printf("%s, %s\n",symbol_map[rule.non_terminal],symbol_map[terminal->val]);
			printProduction(parsing_table[rule.non_terminal][terminal->val-$]);
			printProduction(rule);	
		    }
 
		    //assign production to parsing table entry corresponding to (Non_Terminal, terminal in follow set of Non_Terminal)
		    parsing_table[rule.non_terminal][terminal->val-$].rule = rule.rule; //convert nonterminal to 0 base indexing
		    parsing_table[rule.non_terminal][terminal->val-$].size = rule.size;
		    parsing_table[rule.non_terminal][terminal->val-$].non_terminal = rule.non_terminal;
		    		    
		    terminal = terminal->next; 
	    
		}
	    }
	}
    }
    //################ print parsing table #######################
    if(DEBUG){ 
	red();
	printf("\n\n################### Print Parsing Table #####################\n");
	reset();
	for(int i=0;i<$;i++)
	{	
	    for(int j=0;j<(ENUM_END-$+1);j++)
	    {
		if(parsing_table[i][j].rule)
		{
		    blue();
		    printf("[%s][%s] ---> ",symbol_map[i],symbol_map[j+$]);
		    reset();
		    printProduction(parsing_table[i][j]);
		}
	    }
	}
    }
}




Nary_tree parse_input(type start_symbol, char* sourcefile)
{
    bool error_flag = false;
    bool recovery_flag = false;
    int error_line;
    if(DEBUG)
    {
	red();
	printf("\n\n\n##################### Parsing ######################\n");
	reset();
    }

    stack s = getStack();
    stack_push(s,make_treenode($));
    //push the start symbol and save the root node for future
    treenode* root = make_treenode(start_symbol);
    stack_push(s,root);

    //open the source file for lexer
    openfile(sourcefile);

    token* a = getNextToken();

    treenode* X = stack_top(s);
    while(X->tok != -1) //stack is not empty
    {
	recovery_flag = false;
	if(DEBUG)
	{
	    blue();
	    printf("processing: %s,%s\n",symbol_map[X->tok],symbol_map[a->tag]);
	    reset();
	}
	
	if(X->tok == a->tag)
	{
	    if(a->tag == $ && X->tok== $)
		break;
	    stack_pop(s);
	    a = getNextToken();
	}
	else if(isterminal(X->tok))
	{
	if(X->tok == EPS)
		stack_pop(s);
	    else
	    {
		error_flag = true;
		if(a->line_no != error_line)
		{
		    error_line = a->line_no;
		    printTerminalError(X,a);
		}
		
		while(1)
		{
		    if(a->tag!=$ && a->tag!= SEMICOL)    
		    {
			a=getNextToken();
			if(a->tag == X->tok)
			{
			    recovery_flag = 1;
			    break;
			}
		    }
		    else
		    {

			if(a->tag == X->tok)
			{
			    recovery_flag = 1;
			    break;
			}

			if(a->tag==$)
			    while(X->tok != $)
			    {
				stack_pop(s);
				X = stack_top(s);
			    }
		    	if(a->tag==SEMICOL)
			    while(X->tok !=SEMICOL && X->tok!=$ && isterminal(X->tok))
			    {
				stack_pop(s);
				X = stack_top(s);
			    }
			if(X->tok == $)
			    while(a->tag!=$)
				a = getNextToken();

			
			recovery_flag = 1;
			break;
		    }
		}
	    }
	}
	else if(parsing_table[X->tok][a->tag-$].rule == NULL)
	{
	    error_flag = true;
	    if(a->line_no != error_line)
		{
		    error_line = a->line_no;
		    printNonTerminalError(X,a);
		}

	    
	    while(1)
	    {
		if(a->tag!=$)
		{
		    a = getNextToken();
		}
		else
		{
		    while(X->tok!=$)
		    {
			stack_pop(s);
			X = stack_top(s);
		    }
		    recovery_flag = true;
		    break;
		}
		if(isSetMember(nonterminal_FirstSet[X->tok],a->tag))
		{
		    recovery_flag = true;
		    break;
		}
		else if(isSetMember(nonterminal_FollowSet[X->tok],a->tag))
		{
		    recovery_flag = true;
		    stack_pop(s);
		    X = stack_top(s);
		    break;
		}
		
	    }

	}
	else
	{
	    prodn p = parsing_table[X->tok][a->tag-$];
	    
	    if(DEBUG){
		printf("Rule applied: ");
		printProduction(p);
	    }

	    //make children from the right side of the rule
	    insert_children(X,(p.rule)+1,p.size-1);
	    stack_pop(s);
	   //push the rule in the stack in reverse order
	   
	    for(int i=X->n-1;i>=0;i--)
	   {
	        stack_push(s,X->children[i]);
	   } 
	}
	if(!recovery_flag)
	    X = stack_top(s);
    }

    //make Tree from the root node and return
    Nary_tree syntax_tree;
    syntax_tree.root  = NULL;
    if(error_flag)
	return syntax_tree;
    else
    {
	syntax_tree.root = root;
	return syntax_tree;
    }
}
