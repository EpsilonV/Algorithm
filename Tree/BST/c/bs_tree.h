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



#endif
