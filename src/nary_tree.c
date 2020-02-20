#include <stdio.h>
#include "nary_tree.h"
#include<stdlib.h>
#include"lexDef.h"
void dfs_helper(treenode* root)
{
    if(root==NULL)return;
    if(root->children==NULL)
	printf("%s\n",symbol_map[root->tok]);
    else
    {
	for(int i=0;i<root->n;i++)
	    dfs_helper(root->children[i]);
	printf("%s\n",symbol_map[root->tok]);
    }
}

void dfs(Nary_tree root)
{
    dfs_helper(root.root);
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


