#include <stdio.h>
#include "bs_tree.h"

struct _BSTreeNode {
     BSTreeKey key;
     BSTreeValue value;
     struct _BSTreeNode *left;
     struct _BSTreeNode *right;
     struct _BSTreeNode *parent;
}

struct _BSTree {
     BSTreeNode *root;
     BSTreeKeyCompareFunc compare_func;
}

static void bs_tree_node_inorder(BSTreeNode *node, BSTreeKeyPrintFunc print_func)
{
      if (node != NULL) 
      {
           bs_tree_node_inorder(node->left, print_func);
	   print_func(node->key);
	   bs_tree_node_inorder(node->right, print_func);
      }
}

static void bs_tree_node_preorder(BSTreeNode *node, BSTreeKeyPrintFunc print_func)
{
     if (node != NULL)
     {
         print_func(node->key);
	 bs_tree_node_preorder(node->left, print_func);
	 bs_tree_node_preorder(node->right, print_func);
     } 
}	

static void bs_tree_node_postorder(BSTreeNode *node, BSTreeKeyPrintFunc print_func)
{
     if (node != NULL)
     {
          bs_tree_node_postorder(node->left, print_func);
	  bs_tree_node_postorder(node->right, print_func);
	  print_func(node->key); 
     }
}

 
BSTree *bs_tree_new(BSTreeKeyCompareFunc compare_func)
{
     BSTree *tree = (BSTree *)malloc(sizeof(BSTree));
     if (tree == NULL) {
         return NULL;
     }
    
     tree->root = NULL;
     tree->compare_func = compare_func;

     return tree;
}

void bs_tree_inorder(BSTree *tree, BSTreeKeyPrintFunc print_func)
{
   if (print_func != NULL && tree != NULL) 
   {
        bs_tree_node_inorder(tree->root, print_func);
   }
}	

void bs_tree_preorder(BSTree *tree, BSTreePrintFunc print_func)
{
    if (print_func != NULL && tree != NULL)
    {
        bs_tree_node_preorder(tree->root, print_func);
    }
}

void bs_tree_postorder(BSTree *tree, BSTreePrintFunc print_func)
{
    if (print_func !=NULL && tree != NULL)
    {
        bs_tree_node_postorder(tree->root, print_func);
    }
}



