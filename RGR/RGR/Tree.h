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
	friend ostream& operator<<(ostream& os, const MyPair& p) {
		return os << "(" << p.arg << ", " << p.value << ")";
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

	void infixTraverse(Node* root) {
		if (root) {
			infixTraverse(root->left);
			cout << root->value << " ";
			infixTraverse(root->right);
		}
	}

};

