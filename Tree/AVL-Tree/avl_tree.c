#include <stdio.h>
#include "avl_tree.h"

#define HEIGHT(p) ((p == NULL) ? 0 : ((AVLTreeNode *)(p))->height)

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct _AVLTreeNode {
	AVLTreeKey key;
	AVLTreeValue value;
	AVLTreeNode *left;
	AVLTreeNode *right;
	int height; 
}

struct _AVLTree {
	AVLTreeNode *root;
	AVLTreeKeyCompareFunc compare_func;
	int num_nodes;
}
/* 左左单旋转
*   
*         E                       C
*        / \                     / \
*       C   F      ===>         B   E
*      / \                     /   / \
*     B   D                   A   D   F
*    /
*   A
*/

static AVLTreeNode *left_left_rotation(AVLTreeNode *node)
{
	AVLTreeNode *root = node->left;
	node->left = root->right;
	root->right = node;

	node->height = MAX(HEIGHT(node->left), HEIGHT(node->right)) + 1;
	root->height = MAX(HEIGHT(root->left), node->height) + 1;

	return root;
}

/* 右右单旋转
*   
*         B                       D
*        / \                     / \
*       A   D      ===>         B   E
*          / \                 / \   \
*         C   E               A   C   F
*              \
*               F
*/
static AVLTreeNode *right_right_rotation(AVLTreeNode *node)
{
	AVLTreeNode *root = node->right;
	node->right = root->left;
	root->left = node;

	node->height = MAX(HEIGHT(node->left), HEIGHT(node->right)) + 1;
	root->height = MAX(HEIGHT(root->right), node->height) + 1;

	return root;
}

/* 左右单旋转
*      
*         E                       E                  C
*        / \                     / \                / \
*       B   F      ===>         C   F   ==>        B   E
*      / \                     / \                /   / \
*     A   C                   B   D              A   D   F
*          \                 /
*           D               A
*/
static AVLTreeNode *left_right_rotation(AVLTreeNode *node)
{
	node->left = right_right_rotation(node->left);
	left_left_rotation(node);

	return root;
}

/* 右左单旋转
*      
*         B                 	B  		    			D
*        / \                   / \	 	   			   / \
*       A   E      ===>       A   D	   	===>		  B   E
*          / \                 	 / \				 / \   \
*         D   F               	C  	E				A   C   F
*        /     						 \	
*       C                             F
*/
static AVLTreeNode *right_left_rotation(AVLTreeNode *node)
{
	node->right = left_left_rotation(node->right);
	right_right_rotation(node);

	return root;
}

static AVLTreeNode *avl_tree_insert_node(AVLTreeNode *root, AVLTreeKey key, AVLTreeValue value, AVLTreeKeyCompareFunc compare_func)
{
	if (root == NULL)
	{
		root = (AVLTreeNode *)malloc(sizeof(AVLTreeNode));
		if (root == NULL){
			return NULL;
		}
		root->key = key;
		root->value = value;
		root->left = NULL;
		root->right = NULL;
		root->height == 0;
	}
	else if (compare_func(key, root->key) < 0 )
	{
		root->left = avl_tree_insert_node(root->left, key, value, compare_func);
		if (HEIGHT(root->left) - HEIGHT(root->right) == 2)
		{
			if (key < root->left->key)
				root = left_left_rotation(root);
			else
				root = left_right_rotation(root);
		}
	}else if(compare_func(key, root->key) >= 0)
	{
		root->right = avl_tree_insert_node(root->right, key, value, compare_func);
		if (HEIGHT(root->right) - HEIGHT(root->left) == 2)
		{
			if (key > root->right->key)
				root = right_right_rotation(root);
			else
				root = right_left_rotation(root);
		}
	}

	root->HEIGHT = MAX(HEIGHT(root->left), HEIGHT(root->right)) + 1;

	return root;
}

static AVLTreeNode *avl_tree_delete_node(AVLTreeNode *root, AVLTreeNode *node, compare_func)
{
	if (root == NULL || node == NULL)
		return NULL;

	if (compare_func(node->key, root->key) < 0)
	{
		root->left = avl_tree_delete_node(root->left, node, compare_func);
		if (HEIGHT(root->right) - HEIGHT(root->left) == 2)
		{
			AVLTreeNode *r = root->right;
			if (HEIGHT(r->right) > HEIGHT(r->left))
				right_left_rotation(root);
			else
				right_right_rotation(root);
		}
	}
	else if (compare_func(node->key, root->key) > 0)
	{
		root->right = avl_tree_delete_node(root->right, node, compare_func);
		if (HEIGHT(root->left) - HEIGHT(root->right) == 2)
		{
			AVLTreeNode *l = root->left;
			if (HEIGHT(r->left) > HEIGHT(r->right))
				left_left_rotation(root);
			else
				left_right_rotation(root);
		}
	}
	else
	{
		if (root->left && root->right)
		{
			if (HEIGHT(root->left) > HEIGHT(root->right))
			{
				AVLTreeNode *l = avltree_maximum(root->left);
				root.key = l.key;
				root.value = l.value;
				root->left = avl_tree_delete_node(root, l, compare_func);
			}else
			{
				AVLTreeNode *r = avltree_maximum(root->right);
				root.key = r.key;
				root.value = r.value;
				root->right = avl_tree_delete_node(root, r, compare_func);
			}
		}
		else
		{
			AVLTreeNode *tmp = root;
			root = root->left ? root->left : root->right;
			free(root);
		}
	}

	return root;
}

AVLTreeNode *avltree_maximum(AVLTreeNode *node)
{
	while(node->right)
		node = node->right;

	return node;
}

AVLTree *avl_tree_new(AVLTreeKeyCompareFunc compare_func)
{
	AVLTree *tree = (AVLTree *)malloc(sizeof(AVLTree));
	if (tree == NULL)
		return NULL;

	tree->root = NULL;
	tree->compre_func = compare_func;
	tree->num_nodes = 0;

	return tree;
}

AVLTreeNode *avl_tree_insert(AVLTree *tree, AVLTreeKey key, AVLTreeValue value)
{
	return avl_tree_insert_node(tree->root, key, value, tree->compare_func);
}


AVLTreeNode *avl_tree_delete(AVLTree *tree, AVLTreeKey key)
{
	AVLTreeNode *node = avl_tree_lookup_node(tree, key);
	return avl_tree_delete_node(tree->root, node, tree->compare_func);
}

AVLTreeNode *avl_tree_lookup_node(AVLTree *tree, AVLTreeKey key)
{
	if (tree->root == NULL)
		return NULL

	AVLTreeNode *node = tree->root;

	while(node)
	{
		if (tree.compare_func(key, node->tree) == 0)
		{
			return node;
		}
		else if (tree.compare_func(key, node->tree) < 0)
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}

	return NULL;
}

AVLTreeValue avl_tree_lookup(AVLTree *tree, AVLTreeKey key)
{
	AVLTreeNode node = avl_tree_lookup_node(tree, key);

	if (node)
		return node->value;

	return NULL;
}

















