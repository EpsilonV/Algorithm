#include <stdio.h>
#include <stdlib.h>
#include "bs_tree.h"

struct _BSTreeNode {
    BSTreeKey key;
    BSTreeValue value;
    struct _BSTreeNode *left;
    struct _BSTreeNode *right;
    struct _BSTreeNode *parent;
};

struct _BSTree {
    BSTreeNode *root;
    BSTreeKeyCompareFunc compare_func;
};

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

static void bs_tree_free_subtree(BSTreeNode *node)
{
    if (node == NULL)
        return;

    bs_tree_free_subtree(node->left);
    bs_tree_free_subtree(node->right);

    free(node);
}

static BSTreeNode *bs_tree_node_maximum(BSTreeNode *node)
{
    if (node == NULL)
        return NULL;

    while (node->right != NULL)
        node = node->right;

    return node;
}


static BSTreeNode *bs_tree_node_minimum(BSTreeNode *node)
{
    if (node == NULL)
        return NULL;

    while(node->left != NULL)
        node = node->left;

    return node;
}

static void bs_tree_transplant(BSTree *tree, BSTreeNode *nodeX, BSTreeNode *nodeY)
{
    if (nodeX->parent == NULL)
        tree->root = nodeY;
    else if (nodeX->parent->left == nodeX)
        nodeX->parent->left = nodeY;
    else
        nodeX->parent->right = nodeY;
    if (nodeY != NULL)
        nodeY->parent = nodeX->parent;
}

static BSTreeNode *bs_tree_delete_node(BSTree *tree, BSTreeNode *node)
{
    if (node->left == NULL)
    {
        bs_tree_transplant(tree, node, node->right);
    }
    else if (node->right == NULL)
    {
        bs_tree_transplant(tree, node, node->left);
    }
    else{
        BSTreeNode *nodeY;
        nodeY = bs_tree_successor(node);
        if (nodeY->parent != node)
        {
            bs_tree_transplant(tree, nodeY, nodeY->right);
            nodeY->right = node->right;
            node->right->parent = nodeY;
        }

        bs_tree_transplant(tree, node, nodeY);
        nodeY->left = node->left;
        nodeY->left->parent = nodeY;
    }

    return node;
}
 
BSTree *bs_tree_new(BSTreeKeyCompareFunc compare_func)
{
     BSTree *tree = (BSTree *)malloc(sizeof(BSTree));
     if (tree == NULL) 
     {
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

void bs_tree_preorder(BSTree *tree, BSTreeKeyPrintFunc print_func)
{
    if (print_func != NULL && tree != NULL)
    {
        bs_tree_node_preorder(tree->root, print_func);
    }
}

void bs_tree_postorder(BSTree *tree, BSTreeKeyPrintFunc print_func)
{
    if (print_func !=NULL && tree != NULL)
    {
        bs_tree_node_postorder(tree->root, print_func);
    }
}

BSTreeNode *bs_tree_insert(BSTree *tree, BSTreeKey key, BSTreeValue value)
{
    BSTreeNode *root = tree->root;
    BSTreeNode *parent = NULL;

    if (root == NULL) {
        root = (BSTreeNode *)malloc(sizeof(BSTreeNode));
        if (root == NULL){
            return NULL;
        }

        root->key = key;
        root->value = value;
        root->parent = NULL;
        root->left = NULL;
        root->right = NULL;
        tree->root = root;

        return tree->root;
    }

    
    while(root != NULL) {
        parent = root;
        if (tree->compare_func(key, root->key) < 0)
        {
            root = root->left;
        }else
        {
            root = root->right;
        }
    }

    BSTreeNode *node = (BSTreeNode *)malloc(sizeof(BSTreeNode));
    if (node == NULL)
    {
        return NULL;
    }

    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = parent;
    if (tree->compare_func(node->key, parent->key) < 0)
        parent->left = node;
    else
        parent->right = node;

    return node;
    
}

BSTreeNode *bs_tree_search(BSTree *tree, BSTreeKey key)
{
    BSTreeNode *root = tree->root;

    if (root == NULL)
        return NULL;

    while(root)
    {
        if (tree->compare_func(key, root->key) == 0)
            return root;
        else if (tree->compare_func(key, root->key) < 0)
            root = root->left;
        else
            root = root->right;
    }

    return NULL;
}

BSTreeNode *bs_tree_maximum(BSTree *tree)
{
    BSTreeNode *root = tree->root;

    return bs_tree_node_maximum(root);
}

BSTreeNode *bs_tree_minimum(BSTree *tree)
{
    BSTreeNode *root = tree->root;

    return bs_tree_node_minimum(root);
}

BSTreeNode *bs_tree_predecessor(BSTreeNode *node)
{
    if (node->left != NULL)
        return bs_tree_node_maximum(node->left);

    BSTreeNode *parent = node->parent;

    while((parent != NULL) && (node == parent->left))
    {
        node = parent;
        parent = node->parent;
    }

    return parent;
}

BSTreeNode *bs_tree_successor(BSTreeNode *node)
{
    if (node->right != NULL)
        return bs_tree_node_minimum(node->right);

    BSTreeNode *parent = node->parent;

    while((parent != NULL) && (node == parent->right))
    {
        node = parent;
        parent = node->parent;
    }

    return parent;
}

BSTreeNode *bs_tree_delete(BSTree *tree, BSTreeKey key)
{
    BSTreeNode *node;

    node = bs_tree_search(tree, key);

    if (node != NULL)
    {
        node = bs_tree_delete_node(tree, node);

        return node;
    }

    return NULL;
}

void bs_tree_free(BSTree *tree) 
{
    if (tree) 
    {
        bs_tree_free_subtree(tree->root);

        free(tree);
    }
}

void bs_tree_print_node(BSTreeNode *node, BSTreeKeyPrintFunc print_func)
{
    if(node != NULL)
    {
        print_func(node->key);
    }
}


