#pragma once
#include <iostream>
#include <iomanip>
using namespace std;

class Pair
{
private:
	int arg;
	int value;

public:
	Pair(int arg = int(), int num2 = int()) {
		this->arg = arg;
		this->value = num2;
	}
	Pair(const Pair& p) {
		this->arg = p.arg;
		this->value = p.value;
	}
	bool operator==(Pair& p) {
		if (this->arg == p.arg) return true;
		else return false;
	}
	bool operator!=(Pair& p) {
		return !(*this == p);
	}
	bool operator<(Pair& p) {
		if (this->arg < p.arg) return true;
		else if (this->arg > p.arg) return false;
		else if (this->value < p.value) return true;
		else return false;
	}
	bool operator>(Pair& p) {
		if (this->arg > p.arg) return true;
		else if (this->arg < p.arg) return false;
		else if (this->value > p.value) return true;
		else return false;
	}
	Pair& operator=(Pair& p) {
		this->arg = p.arg;
		this->value = p.value;
		return *this;
	}
	//Arithmetic operations
	Pair operator+(Pair& p) {
		Pair a(*this);
		a.value = a.value + p.value;
		return a;
	}
	Pair operator-(Pair& p) {
		Pair a(*this);
		a.value = a.value - p.value;
		return a;
	}
	Pair operator*(Pair& p) {
		Pair a(*this);
		a.value = a.value * p.value;
		return a;
	}
	Pair operator/(Pair& p) {
		Pair a(*this);
		a.value = a.value / p.value;
		return a;
	}
	Pair& operator+=(Pair& p) {
		this->arg = this->arg + p.arg;
		this->value = this->value + p.value;
		return *this;
	}
	Pair& operator-=(Pair& p) {
		this->arg = this->arg - p.arg;
		this->value = this->value - p.value;
		return *this;
	}
	Pair& operator*=(Pair& p) {
		this->arg = this->arg * p.arg;
		this->value = this->value * p.value;
		return *this;
	}
	Pair& operator/=(Pair& p) {
		this->arg = this->arg / p.arg;
		this->value = this->value / p.value;
		return *this;
	}
	int getValue() {
		return this->value;
	}
	int getArg() {
		return this->arg;
	}
	friend ostream& operator<<(ostream& os, const Pair& p) {
		return os << p.arg << ">" << p.value;
	}
	friend istream& operator>>(istream& is, Pair& p) {
		char a;
		return is >> p.arg >> a >> p.value;
	}
};

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
		if (root->right) {
			return minimum(root->right);
		}

		Node* y = root->parent;
		while (y && root == y->right) {
			root = y;
			y = y->parent;
		}

		return y;
	}

	static Node*& prev(Node*& root) {
		if (root->left) {
			return maximum(root->left);
		}

		Node* y = root->parent;
		while (y && root == y->left) {
			root = y;
			y = y->parent;
		}

		return y;
	}

	static Node*& minimum(Node*& root) {
		if (!root->left) {
			return root;
		}

		return minimum(root->left);
	}

	static Node*& maximum(Node*& root) {
		if (!root->right) {
			return root;
		}

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
		Iterator operator++(int) {
			Iterator iter = *this;
			ptr = next(ptr);
			return iter;
		}
		Iterator& operator++() {
			ptr = next(ptr);
			return *this;
		}
		Iterator operator--(int) {
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
		bool operator<(const Iterator& iter) {
			return (iter.ptr < ptr);
		}
		bool operator>(const Iterator& iter) {
			return (iter.ptr > ptr);
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

	Pair& at(const int index) {
		try {
			Iterator iter = begin();
			int i = 0;

			for (; i < index && iter != end(); i++, iter++);
			if (i == index) {
				return *iter;
			}
			else throw "out of range";
		}
		catch (const char* e) {
			cout << e;
		}
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
			else {
				break;
			}
		}
	}

	void prefixTraverse(Node* root, ostream& os, bool flag = 1) {
		if (root) {
			try {

				prefixTraverse(root->left, os, 0);
				if (flag) os << root->value;
				else os << ", " << root->value;
				prefixTraverse(root->right, os, 0);
			}
			catch (const char* e) {
				cout << e;
			}
		}
	}

	static void postorder(Node* p, int indent)
	{
		if (p != NULL) {
			if (p->right) {
				postorder(p->right, indent + 4);
			}
			if (indent) {
				std::cout << std::setw(indent) << ' ';
			}
			if (p->right) std::cout << " /\n" << std::setw(indent) << ' ';
			std::cout << p->value << "\n ";
			if (p->left) {
				std::cout << std::setw(indent) << ' ' << " \\\n";
				postorder(p->left, indent + 4);
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

	Tree merge(Tree& t) {
		Tree tree;
		Iterator iter1 = begin();
		Iterator iter2 = t.begin();

		while (iter1 != end() && iter2 != t.end()) {
			if (*iter1 < *iter2) {
				tree.push(tree.getRoot(), *(iter1++));
			}
			else if (*iter1 > * iter2) {
				tree.push(tree.getRoot(), *(iter2++));
			}
			else {
				tree.push(tree.getRoot(), *(iter1));
				iter1++;
				iter2++;
			}
		}
		while (iter1 != end()) {
			tree.push(tree.getRoot(), *(iter1++));
		}
		while (iter2 != t.end()) {
			tree.push(tree.getRoot(), *(iter2++));
		}

		return tree;
	}

	Tree composition(Tree& t) {
		Tree tree;
		Iterator iter = begin();
		while (iter != end()) {
			if (t.search_by_arg(t.getRoot(), (*iter).getValue())) {
				int a = (*iter).getArg();
				int b = t.search_by_arg(t.getRoot(), (*iter).getValue())->value.getValue();
				tree.push(tree.getRoot(), Pair(a, b));
			}
			iter++;
		}
		return tree;
	}
};

