#ifndef BS_TREE_H
#define BS_TREE_H


typedef struct _BSTree BSTree;

typedef void *BSTreeKey;

typedef void *BSTreeValue;

typedef struct _BSTreeNode BSTreeNode;

typedef int (*BSTreeKeyCompareFunc)(BSTreeKey key1, BSTreeKey key2);

typedef void (*BSTreeKeyPrintFunc)(BSTreeKey key);

BSTree *bs_tree_new(BSTreeKeyCompareFunc compare_func);

void bs_tree_inorder(BSTree *tree, BSTreeKeyPrintFunc print_func);

void bs_tree_preorder(BSTree *tree, BSTreeKeyPrintFunc print_func);

void bs_tree_postorder(BSTree *tree, BSTreeKeyPrintFunc print_func); 

BSTreeNode *bs_tree_insert(BSTree *tree, BSTreeKey key, BSTreeValue value);

BSTreeNode *bs_tree_search(BSTree *tree, BSTreeKey key);

BSTreeNode *bs_tree_maximum(BSTree *tree);

BSTreeNode *bs_tree_minimum(BSTree *tree);

BSTreeNode *bs_tree_predecessor(BSTreeNode *node);

BSTreeNode *bs_tree_successor(BSTreeNode *node);

BSTreeNode *bs_tree_delete(BSTree *tree, BSTreeKey key);

void bs_tree_print_node(BSTreeNode *node, BSTreeKeyPrintFunc print_func);

void bs_tree_free(BSTree *tree);

#endif
