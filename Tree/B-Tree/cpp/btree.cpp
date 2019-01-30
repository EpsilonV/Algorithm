#include <iostream>

using namespace std;

class BTreeNode {
	int *keys;      //
	int t;  		//最小度
	BTreeNode **C;
	int n;
	bool leaf;
public:
	BTreeNode(int _t, bool _leaf);

	void insertNonFull(int k);

	void splitChild(int i, BTreeNode *y);

	void traverse();

	BTreeNode *search(int k);

friend class Btree;
};

BTreeNode::BTreeNode(int _t, bool _leaf) {
	t = _t;
	leaf = _leaf;
	keys = new int[2*t-1];
	C = new BTreeNode *[2*t];
	n = 0;
}

void BTreeNode::insertNonFull(int k) {

}

void BTreeNode::splitChild(int i, BTreeNode *y) {
	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->n = t-1;
	//y->keys[t:2t-2]  ===>z->keys[0:t-2]
	//y->C[t+1:2t-1] ===>z
	int j = 0;
	for (j = 0; j < t -1; j++ )

	for (i = 0; i < t-1; i ++)
		z->keys[i] = y->keys[i+t]

	y->n = t-1;
}

void BTreeNode::traverse() {
	for (int i = 0; i < n; ++i)
	{
		if (leaf == FALSE)
			C[i]->traverse();
		cout << keys[i] << " ";
	}

	if (leaf == FALSE)
		C[i]->traverse();
}

BTreeNode *BTreeNode::search(int k) {
	int i = 0;
	while (i < n && k > keys[i])
		++i;
	if (k == keys[i])
		return this;

	if (leaf == TRUE)
		return NULL;

	return C[i]->search(k);
}

void 

class Btree {
	BTreeNode *root;
	int t;    //最小度
public:
	Btree(int _t) { 
		t = _t;
	}
	
};