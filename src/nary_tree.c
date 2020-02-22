#include <stdio.h>
#include "nary_tree.h"
#include<stdlib.h>
#include"lexDef.h"
void inorder_helper(treenode* root, FILE* fptr)
{
    if(root==NULL)
    {
	return;
    }

//print first child    
    if(root->children!=NULL)
	inorder_helper(root->children[0],fptr);
// print current node
    fprintf(fptr,"%s\n",symbol_map[root->tok]);

//print rest of the children
    for(int i=1;i<root->n;i++)
	inorder_helper(root->children[i],fptr);

}

void inorder(Nary_tree root, FILE* fptr)
{
    inorder_helper(root.root,fptr);
}

Nary_tree* get_nary_tree(int start)
{
    Nary_tree* temp = (Nary_tree*)malloc(sizeof(Nary_tree));
    temp->root = make_treenode(start);
    return temp;
}


treenode* make_treenode(int a)
{
    treenode* temp = (treenode*)malloc(sizeof(treenode));
    temp->tok = a;
    temp->children = NULL;
    temp->n = 0;
    return temp;
}

void insert_children(treenode* node, type* children, int n)
{
    treenode** temp = (treenode**)malloc(sizeof(treenode*)*n);    
    for(int i=0;i<n;i++)
    {
	treenode* x = make_treenode(children[i]);
	temp[i] = x;
    }
    node->children = temp;
    node->n = n;
}


