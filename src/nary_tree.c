#include <stdio.h>
#include "nary_tree.h"
#include<stdlib.h>
#include"lexDef.h"
#include "color.h"
void inorder_helper(treenode* root, FILE* fptr, char* parentSymbol)
{
    if(root==NULL)
    {
	return;
    }

//print first child    
    if(root->children!=NULL)
	inorder_helper(root->children[0],fptr,symbol_map[root->tok]);
// print current node
    
    // lexeme
    blue();
    if(root->lexeme)
	fprintf(fptr,"%-20s\t",root->lexeme->str);
    else
	fprintf(fptr,"%-20s\t","----");
    reset();
    //line no
    if(root->lexeme)
	fprintf(fptr,"%-5d\t",root->lexeme->line_no);
    else
	fprintf(fptr,"%-5d\t",1);

blue();
    //token name
    fprintf(fptr,"%-20s\t",symbol_map[root->tok]);
reset();
    //value if number
    if(root->lexeme)
	if(root->lexeme->tag == NUM)
	    fprintf(fptr,"%-10d\t",atoi(root->lexeme->str));
	else if(root->lexeme->tag == RNUM) 
	    fprintf(fptr,"%-lf\t",atof(root->lexeme->str));
	else
	    fprintf(fptr,"NaN       \t");
    else
	    fprintf(fptr,"NaN       \t");
blue();
    // parent node symbol
    fprintf(fptr,"%-20s\t",parentSymbol);
reset();
    if(root->children)
	fprintf(fptr,"%-3s\t","No");
    else
	fprintf(fptr,"%-3s\t","Yes");
blue();
    //non terminal symbol of non leaf node
    if(root->children)
	fprintf(fptr,"%-20s\t",symbol_map[root->tok]);
    else
	fprintf(fptr,"%-20s\t","");
reset();
    fprintf(fptr,"\n");
//print rest of the children
    for(int i=1;i<root->n;i++)
	inorder_helper(root->children[i],fptr, symbol_map[root->tok]); // passing parent
									// node symbol

}

void inorder(Nary_tree root, FILE* fptr)
{
    inorder_helper(root.root,fptr, "ROOT");
}

Nary_tree* get_nary_tree()
{
    Nary_tree* temp = (Nary_tree*)malloc(sizeof(Nary_tree));
    temp->root = NULL;
    return temp;
}


treenode* make_treenode(int a, token* lex)
{
    treenode* temp = (treenode*)malloc(sizeof(treenode));
    temp->lexeme = lex;
    temp->tok = a;
    temp->children = NULL;
    temp->n = 0;
    return temp;
}

void insert_children(treenode* node, type* children, int n, token* lex)
{
    treenode** temp = (treenode**)malloc(sizeof(treenode*)*n);    
    for(int i=0;i<n;i++)
    {
	treenode* x = make_treenode(children[i],lex);
	temp[i] = x;
    }
    node->children = temp;
    node->n = n;
}


