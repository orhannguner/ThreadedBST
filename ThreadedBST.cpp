#include "ThreadedBST.h"
using namespace std;
///-----------------------------------------------
/// Erases all nodes in the tree
/// 
void ThreadedBST::eraseTreeNodes(BSTNode* node) {
	if (node == NULL) return;
	if (node->leftLinkType != CHILD) {
		delete node;
		return;
	}
	eraseTreeNodes(node->left);
	if (node->rightLinkType != CHILD) {
		delete node;
		return;
	}
	eraseTreeNodes(node->right);
	delete node;
} //end-eraseTreeNodes

///-----------------------------------------------
/// Adds a given key to the BST
/// 
void ThreadedBST::add(int key) {
	BSTNode* p = this->root;
	int l = 0, r = 0;
	if (this->root == NULL) {
		this->root = new BSTNode(key);
		return;
	}
	while (p != NULL) {
		if (p->key > key){
			if (p->left == NULL) {
				p->left = new BSTNode(key);
				p->leftLinkType = CHILD;
				p = p->left;
				l++;
				
				break;
			}
			else if (p->leftLinkType == CHILD) {
				p = p->left;
				l++;
			}
			else {
				p->left = NULL;
				p->left = new BSTNode(key);
				p->leftLinkType = CHILD;
				p = p->left;
				l++;
				break;
			}
		}
		else if (p->key < key) {
			if (p->right == NULL) {
				p->right = new BSTNode(key);
				p->rightLinkType = CHILD;
				p = p->right;
				r++;
				break;
			}
			else if (p->rightLinkType == CHILD) {
				p = p->right;
				r++;
			}
			else {
				p->right = NULL;
				p->right = new BSTNode(key);
				p->rightLinkType = CHILD;
				p = p->right;
				r++;
				break;
			}
		}
		else {
			cout << "Already exist!!" << endl;
			return;
		}

	}
	BSTNode* p2 = root;
	if (l == 0) {
		for (int i = 0; i < r - 1; i++) {
			p2 = p2->right;
		}
		p->left = p2;
		return;
	}
	else if (r == 0) {
		for (int i = 0; i < l - 1; i++) {
			p2 = p2->left;
		}
		p->right = p2;
		return;
	}
	while (true) {
		if (l == 1 && r == 1) {
			if (key < p2->key) {
				p->right = p2;
				p2 = p2->left;
				p->left = p2;
				break;
			}
			else {
				p->left = p2;
				p2 = p2->right;
				p->right = p2;
				break;
			}
		}
		else if (r == 1) {
			p->left = p2;
		}
		else if (r == 0) {
			while (l != 1) {
				p2 = p2->left;
				l--;
			}
			p->right = p2;
			break;
		}
		else if (l == 1) {
			p->right = p2;
		}
		else if (l == 0) {
			while (r != 1) {
				p2 = p2->right;
				r--;
			}
			p->left = p2;
			break;
		}
		if (key < p2->key) {
			p2 = p2->left;
			l--;
		}
		else {
			p2 = p2->right;
			r--;
		}
	}
} // end-add

///-----------------------------------------------
/// Removes a given key from the BST (if it exists)
/// 
void ThreadedBST::remove(int key) {
	BSTNode* p = root;
	BSTNode* b = root;
	while (p) {
		if (key == p->key) break;
		if (key < p->key)
		{
			if (p->leftLinkType == CHILD)
			{
				b = p;
				p = p->left;
			}
		}
		else if(key > p->key) {
			if (p->rightLinkType == CHILD)
			{
				b = p;
				p = p->right;
			}
		}
	}
	
	if (p == NULL) return;
	if (p->leftLinkType == CHILD && p->rightLinkType == CHILD){
		BSTNode* p2 = p;
		BSTNode* b2 = p;
		p2 = p2->right;
		while (p2) {
			if (p2->leftLinkType == CHILD) {
				b2 = p2;
				p2 = p2->left;
			}
			else break;
		}
		p->key = p2->key;
		if (b2->key > p2->key) {
			if (p2->rightLinkType == CHILD) {
				b2->left = p2->right;
				if (p2->leftLinkType != CHILD) p2->right->left = p;
				delete p2;
			}
			else {
				b2->left = p;
				b2->leftLinkType = THREAD;
				delete p2;
			}
		}
		else if (b2->key == p2->key) {
			b2->right = p2->right;
			b2->rightLinkType = THREAD;
			delete p2;
		}
		else {
			if (p2->rightLinkType == CHILD) {
				b2->right = p2->right;
				if (p2->leftLinkType != CHILD) p2->right->left = p;
				delete p2;
			}
			else {
				b2->right = b;
				delete p2;
			}
		}

	}
	else if (p->leftLinkType == CHILD) {
		if (b->key > p->key) {
			b->left = p->left;
			if (p->left->rightLinkType == THREAD) p->left->right = b;
			delete p;
		}
		else {
			b->right = p->left;
			if (p->left->rightLinkType == THREAD) p->left->right = p->right;
			delete p;
		}
	}
	else if (p->rightLinkType == CHILD) {
		if (b->key > p->key) {
			b->left = p->right;
			if (p->right->leftLinkType == THREAD) p->right->left = p->left;
			delete p;
		}
		else {
			b->right = p->right;
			if (p->right->leftLinkType == THREAD) p->right->left = b;
			delete p;
		}
	}
	else if(p->key > b->key) {
		b->right = p->right;
		b->rightLinkType = THREAD;
		delete p;
	}
	else {
		b->left = p->left;
		b->leftLinkType = THREAD;
		delete p;
	}
} // end-remove

///-----------------------------------------------
/// Searches a given key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::find(int key) {
	BSTNode* p = root;

	while (p) {
		if (key == p->key) return p;
		if (key < p->key)
		{
			if (p->leftLinkType == CHILD)
			{
				p = p->left;
			}
		}
		else {
			if (p->rightLinkType == CHILD)
			{
				p = p->right;
			}
		}
	}
	return NULL;
} // end-find

///-----------------------------------------------
/// Returns the minimum key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::min() {
	// Fill this in
	if (root == NULL) throw std::exception("BST::min - Tree is empty");

	BSTNode* p = root;
	while (p->left != NULL) p = p->left;

	return p;

} // end-min

///-----------------------------------------------
/// Returns the maximum key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::max() {
	// Fill this in
	if (root == NULL) throw std::exception("Tree is empty");

	BSTNode* p = root;
	while (p->right != NULL) p = p->right;

	return p;

} // end-max

///-----------------------------------------------
/// Given a valid pointer to a node in ThreadedBST,
/// returns a pointer to the node that contains the inorder predecessor
/// If the inorder predecessor does not exist, returns NULL
/// 
BSTNode* ThreadedBST::previous(BSTNode* node) {
	if (node->left == NULL) {
		cout << "it doesn't have any inorder predecessor" << endl;
		return NULL;
	}
	BSTNode* p = node;
	if (p->leftLinkType == CHILD) {
		p = p->left;
		while (true) {
			if (p->rightLinkType == THREAD || p->right == NULL) break;
			else p = p->right;
		}
		return p;
	}
	else {
		p = p->left;
		return p;
	}
	
} // end-previous

///-----------------------------------------------
/// Given a valid pointer to a node in the ThreadedBST,
/// returns a pointer to the node that contains the inorder successor
/// If the inorder successor does not exist, returns NULL
/// 
BSTNode* ThreadedBST::next(BSTNode* node) {
	if (node->right == NULL) {
		cout << "it doesn't have any inorder successor" << endl;
		return NULL;
	}
	BSTNode* p = node;
	if (p->rightLinkType == CHILD) {
		p = p->right;
		while (true) {
			if (p->leftLinkType == THREAD || p->left == NULL) break;
			else p = p->left;
		}
		return p;
	}
	else {
		p = p->right;
		return p;
	}
} // end-next