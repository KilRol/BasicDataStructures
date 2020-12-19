#pragma once
#include <iostream>
using namespace std;

struct MyPair {
	int arg;
	int value;
	MyPair(int arg = int(), int num2 = int()) {
		this->arg = arg;
		this->value = num2;
	}
	MyPair(const MyPair& p) {
		this->arg = p.arg;
		this->value = p.value;
	}
	bool operator<(MyPair& p) {
		if (this->arg < p.arg) return true;
		else if (this->arg > p.arg) return false;
		else if (this->value < p.value) return true;
		else return false;
	}
	bool operator==(MyPair& p) {
		if (this->arg == p.arg) return true;
		else return false;
	}

	MyPair& operator=(MyPair& p) {
		this->arg = p.arg;
		this->value = p.value;
		return *this;
	}

	//Arithmetic operations
	MyPair operator+(MyPair& p) {
		MyPair a(*this);
		a.value = a.value + p.value;
		return a;
	}
	MyPair operator-(MyPair& p) {
		MyPair a(*this);
		a.value = a.value - p.value;
		return a;
	}
	MyPair operator*(MyPair& p) {
		MyPair a(*this);
		a.value = a.value * p.value;
		return a;
	}
	MyPair operator/(MyPair& p) {
		MyPair a(*this);
		a.value = a.value / p.value;
		return a;
	}

	int getValue() {
		return this->value;
	}
	int getArg() {
		return this->arg;
	}

	friend ostream& operator<<(ostream& os, const MyPair& p) {
		return os << p.arg << ">" << p.value;
	}
	friend istream& operator>>(istream& is, MyPair& p) {
		char a;
		return is >> p.arg >> a >> p.value;
	}
};

struct Node {
	MyPair value;
	Node* left;
	Node* right;

	Node(MyPair value = MyPair()) {
		this->value = value;
		this->left = nullptr;
		this->right = nullptr;
	}
	Node(MyPair value, Node* left, Node* right) {
		this->value = value;
		this->left = left;
		this->right = right;
	}
};

class Tree
{
private:
	Node* root;
public:
	Tree() {
		root = nullptr;
	}

	Node*& getRoot() {
		return this->root;
	}

	Node*& findValue(Node*& root, int arg) {
		if (root->value.arg == arg) return root;
		if (arg < root->value.arg) findValue(root->left, arg);
		else findValue(root->right, arg);
	}

	int getValue(int arg) {
		return findValue(getRoot(), arg)->value.value;
	}

	Node*& findArg(Node*& root, int value) {
		if (root->value.value == value) return root;
		if (value < root->value.value) findValue(root->left, value);
		else findValue(root->right, value);
	}
	int getInvertable(int value) {
		return findArg(getRoot(), value)->value.arg;
	}

	void push(Node*& root, MyPair value) {
		if (!root)
		{
			root = new Node(value);
		}
		else
		{
			if (value < root->value)
				push(root->left, value);
			else {
				push(root->right, value);
			}
		}
	}

	void prefixTraverse(Node* root, ostream& os, bool flag = 1) {
		if (root) {
			if (flag) os << root->value;
			else os << ", " << root->value;
			prefixTraverse(root->left, os, 0);
			prefixTraverse(root->right, os, 0);
		}
	}

	friend ostream& operator<<(ostream& os, Tree& t) {
		os << '{';
		t.prefixTraverse(t.getRoot(), os);
		os << '}';
		return os;
	}

	friend istream& operator>>(istream& is, Tree& t) {
		MyPair a;
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

