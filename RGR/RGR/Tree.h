#pragma once
#include <iostream>
#include "Pair.h"
using namespace std;

class Tree
{
private:
	class Node
	{
	public:
		Pair value;
		Node* left;
		Node* right;
		Node* parent;
		Node(Pair value = Pair()) {
			this->value = value;
			this->left = nullptr;
			this->right = nullptr;
			this->parent = nullptr;
		}
		Node(Pair value, Node* left, Node* right, Node* parent) {
			this->value = value;
			this->left = left;
			this->right = right;
			this->parent = parent;
		}
	};

protected:
	Node* root;
	static Node*& next(Node*& root) {
		if (root->right) return minimum(root->right);
		Node* y = root->parent;
		while (y && root == y->right) {
			root = y;
			y = y->parent;
		}
		return y;
	}

	static Node*& prev(Node*& root) {
		if (root->left) return maximum(root->left);
		Node* y = root->parent;
		while (y && root == y->left) {
			root = y;
			y = y->parent;
		}
		return y;
	}

	static Node*& minimum(Node*& root) {
		if (!root->left) return root;
		return minimum(root->left);
	}

	static Node*& maximum(Node*& root) {
		if (!root->right) return root;
		return minimum(root->right);
	}
public:

	class Iterator {
	public:
		friend class Tree;
		Node* ptr;
		Iterator() {
			this->ptr = nullptr;
		}
		Iterator(const Iterator& iter) {
			this->ptr = iter.ptr;
		}
		Iterator& operator=(const Iterator& iter) {
			this->ptr = iter.ptr;
			return *this;
		}
		Iterator& operator++(int) {
			Iterator iter = *this;
			ptr = next(ptr);
			return iter;
		}
		Iterator& operator++() {
			ptr = next(ptr);
			return *this;
		}
		Iterator& operator--(int) {
			Iterator iter = *this;
			ptr = prev(ptr);
			return iter;
		}
		Iterator& operator--() {
			ptr = prev(ptr);
			return *this;
		}
		Pair& operator*() {
			return (*ptr).value;
		}
		Pair* operator->() {
			return &(ptr->value);
		}

		bool operator!=(const Iterator& iter) {
			return (iter.ptr != ptr);
		}

		bool operator==(const Iterator& iter) {
			return (iter.ptr == ptr);
		}

	};

	Tree() {
		root = nullptr;
	}

	Node*& getRoot() {
		return this->root;
	}

	Iterator begin() {
		Iterator it;
		it.ptr = minimum(this->root);
		return it;
	}
	Iterator end() {
		Iterator it;
		it.ptr = nullptr;
		return it;
	}

	Node*& search_by_arg(Node* root, int arg) {
		if ((!root) || arg == root->value.getArg()) return root;
		if (arg < root->value.getArg()) return search_by_arg(root->left, arg);
		else return search_by_arg(root->right, arg);
	}

	Pair& at(const int index) {
		try {
			Iterator iter = begin();
			int i = 0;
			for (; i < index && iter != end(); i++, iter++);
			if (i == index) return *iter;
			else throw "out of range";
		}
		catch (const char* e) {
			cout << e;
		}
	}

	int getValue(int arg) {
		return search_by_arg(getRoot(), arg)->value.getValue();
	}

	Node*& search_by_value(Node* root, int value) {
		if ((!root) || value == root->value.getValue()) return root;
		if (value < root->value.getValue()) return search_by_value(root->left, value);
		else return search_by_value(root->right, value);
	}

	int getInvertable(int value) {
		return search_by_value(getRoot(), value)->value.getArg();
	}

	void push(Node* root, Pair value) {
		Node* nd = new Node(value);
		if (!root) {
			this->root = nd;
			return;
		}
		while (root) {
			if (nd->value > root->value) {
				if (root->right) root = root->right;
				else {
					nd->parent = root;
					root->right = nd;
					break;
				}
			}
			else if (nd->value < root->value) {
				if (root->left) root = root->left;
				else {
					nd->parent = root;
					root->left = nd;
					break;
				}
			}
		}
	}

	void prefixTraverse(Node* root, ostream& os, bool flag = 1) {
		if (root) {
			try {
				if (flag) os << root->value;
				else os << ", " << root->value;
				prefixTraverse(root->left, os, 0);
				prefixTraverse(root->right, os, 0);
			}
			catch (const char* e) {
				cout << e;
			}
		}
	}

	friend ostream& operator<<(ostream& os, Tree& t) {
		os << '{';
		t.prefixTraverse(t.getRoot(), os);
		os << '}';
		return os;
	}

	friend istream& operator>>(istream& is, Tree& t) {
		Pair a;
		char b;
		
		is >> b;
		do {
			is >> a;
			t.push(t.getRoot(), a);
			is >> b;
		} while (b != '}');
		return is;
	}

	static Node* static_merge(Node* t1, Node* t2) {
		if (!t1) {
			return t2;
		}
		if (!t2) {
			return t1;
		}

		Node* a = static_merge(t1->right, t2);
		t1->right = a;
		return t1;
	}

	void merge(Tree& t) {
		this->root = static_merge(this->root, t.getRoot());
	}


};

