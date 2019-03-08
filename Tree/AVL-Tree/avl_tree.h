#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct _AVLTree AVLTree;

typedef void *AVLTreeKey;

typedef void *AVLTreeValue;

#define AVL_TREE_NULL ((void *)0)

typedef struct _AVLTreeNode AVLTreeNode;

typedef int (*AVLTreeKeyCompareFunc)(AVLTreeKey key1, AVLTreeKey key2);

AVLTree *avl_tree_new(AVLTreeKeyCompareFunc compre_func);

AVLTreeNode *avl_tree_insert(AVLTree *tree, AVLTreeKey key, AVLTreeValue value);

int avl_tree_delete(AVLTree *tree, AVLTreeKey key);

AVLTreeNode *avl_tree_lookup_node(AVLTree *tree, AVLTreeKey key);

AVLTreeValue avl_tree_lookup(AVLTree *tree, AVLTreeKey key);

#endif 