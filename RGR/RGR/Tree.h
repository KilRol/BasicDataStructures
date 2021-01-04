#pragma once
#include <iostream>
using namespace std;
class Pair
{
private:
	int arg;
	int value;
public:
	Pair(int arg = int(), int value = int()) {
		this->arg = arg;
		this->value = value;
	}
	Pair(const Pair& p) {
		this->arg = p.arg;
		this->value = p.value;
	}
	Pair& operator=(Pair& p) {
		this->arg = p.arg;
		this->value = p.value;
		return *this;
	}

	//Relations
	bool operator==(Pair& p) {
		if (this->arg == p.arg) return true;
		else return false;
	}
	bool operator!=(Pair& p) {
		return !(*this == p);
	}
	bool operator<(Pair& p) {
		if (this->arg < p.arg) return true;
		else return false;
	}
	bool operator>(Pair& p) {
		if (this->arg > p.arg) return true;
		else return false;
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
		this->value = this->value + p.value;
		return *this;
	}
	Pair& operator-=(Pair& p) {
		this->value = this->value - p.value;
		return *this;
	}
	Pair& operator*=(Pair& p) {
		this->value = this->value * p.value;
		return *this;
	}
	Pair& operator/=(Pair& p) {
		this->value = this->value / p.value;
		return *this;
	}

	//Getters
	int get_value() {
		return this->value;
	}
	int get_arg() {
		return this->arg;
	}

	//Stream I/O
	friend ostream& operator<<(ostream& os, const Pair& p) {
		try {
			if (&p != nullptr) {
				os << p.arg << ">" << p.value;
			}
			else throw "Not a Pair";
		}
		catch (const char* e) {
			os << e;
			return os;
		}
		return os;
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
		~Node() {
			delete left;
			delete right;
		}
	};

	//Search
	Node* search_by_arg(int arg) {
		Iterator iter;
		for (iter = begin(); iter != end() && (*iter).get_arg() != arg; iter++);
		return iter.get_ptr();
	}
	Node* search_by_value(int value) {
		Iterator iter;
		for (iter = begin(); iter != end() && (*iter).get_value() != value; iter++);
		return iter.get_ptr();
	}
	Node*& getRoot() {
		return this->root;
	}
protected:
	Node* root;
	static Node* next(Node*& r) {
		if (r->right) {
			return minimum(r->right);
		}
		Node* y = r->parent;
		while (y && r == y->right) {
			r = y;
			y = y->parent;
		}
		return y;
	}
	static Node* prev(Node*& r) {
		if (r->left) {
			return maximum(r->left);
		}

		Node* y = r->parent;
		while (y && r == y->left) {
			r = y;
			y = y->parent;
		}
		return y;
	}
	static Node*& minimum(Node*& r) {
		if (!r->left) {
			return r;
		}
		return minimum(r->left);
	}
	static Node*& maximum(Node*& r) {
		if (!r->right) {
			return r;
		}
		return maximum(r->right);
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
			return ptr->value;
		}
		Pair* operator->() {
			return &(ptr->value);
		}
		Node* get_ptr() {
			return ptr;
		}
		bool operator!=(const Iterator& iter) {
			return (iter.ptr != ptr);
		}
		bool operator==(const Iterator& iter) {
			return (iter.ptr == ptr);
		}
	};

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

	Tree() {
		cout << "Init tree" << endl;
		root = nullptr;
	}
	//IN_DEV
	Tree(const Tree& t) {
		cout << "Tree Copy Const" << endl;
		delete root;
		copy_tree(t.root);
	}
	~Tree() {
		cout << "Tree Destruct" << endl;
		delete root;
	}

	void destroy_tree(Node* r) {
		delete root;
	}

	//Legacy
	void copy_tree(Node* r) {
		if (!r) return;
		push(this->root, r->value);
		copy_tree(r->left);
		copy_tree(r->right);
	}

	//IN_DEV
	Tree& operator=(const Tree& t) {
		cout << "OPERATOR=" << endl;
		delete root;
		root = nullptr;
		copy_tree(t.root);
		return *this;
	}

	void push(Node* r, Pair value) {
		Node* nd = new Node(value);
		if (!r) {
			this->root = nd;
			return;
		}
		try {
			while (r) {
				if (nd->value > root->value) {
					if (r->right) {
						r = r->right;
					}
					else {
						nd->parent = r;
						r->right = nd;
						break;
					}
				}
				else if (nd->value < root->value) {
					if (r->left) {
						r = r->left;
					}
					else {
						nd->parent = r;
						r->left = nd;
						break;
					}
				}
				else {
					if (nd->value.get_value() != r->value.get_value())
						throw "Push Error! Equal arg and different value";
					break;
				}
			}
		}
		catch (const char* e) {
			cout << e << endl;
			exit(-1);
		}
	}

	//Access to elem
	Pair& at(int index) {
		Iterator iter = begin();
		int i = 0;
		for (; i < index && iter != end(); i++, iter++);
		if (i == index) {
			return *iter;
		}
	}
	Pair& getInvertable(Tree& t, Pair& p) {
		Node* res = t.search_by_value(p.get_arg());
		return res->value;
	}

	Tree merge(Tree& t) {
		Tree tree;
		Iterator iter1 = begin();
		Iterator iter2 = t.begin();
		try {
			while (iter1 != end() && iter2 != t.end()) {
				if ((*iter1) < (*iter2)) {
					tree.push(tree.getRoot(), *(iter1++));
				}
				else if ((*iter1) > (*iter2)) {
					tree.push(tree.getRoot(), *(iter2++));
				}
				else {
					if ((*iter1).get_value() == (*iter2).get_value()) {
						tree.push(tree.getRoot(), *(iter1++));
						iter2++;
					}
					else throw "Merge Error! Equal arg and different value";
				}
			}
			while (iter1 != end()) {
				tree.push(tree.getRoot(), *(iter1++));
			}
			while (iter2 != t.end()) {
				tree.push(tree.getRoot(), *(iter2++));
			}
		}
		catch (const char* e) {
			cout << e << endl;
			tree.destroy_tree(tree.root);
			return tree;
		}
		return tree;
	}
	Tree composition(Tree& t) {
		Tree tree;
		Iterator iter = t.begin();
		while (iter != t.end()) {
			if (search_by_arg(iter->get_value())) {
				int a = iter->get_arg();
				int b = search_by_arg(iter->get_value())->value.get_value();
				tree.push(tree.getRoot(), Pair(a, b));
			}
			iter++;
		}
		return tree;
	}

	void prefixTraverse(Node* r, ostream& os, bool flag = 1) {
		if (r) {
			if (flag) os << r->value;
			else os << ", " << r->value;
			prefixTraverse(r->left, os, 0);
			prefixTraverse(r->right, os, 0);
		}
	}

	//Relations
	bool operator==(Tree& t) {
		Iterator iter1 = begin();
		Iterator iter2 = t.begin();
		while (iter1 != end() && iter2 != end()) {
			if ((*iter1++) != (*iter2++)) return false;
		}
		if (iter1 != end() || iter2 != end()) return false;
		return true;
	}
	bool operator!=(Tree& t) {
		return !(*this == t);
	}

	//Arithmetic operations
	Tree operator+(Tree& t) {
		Tree result_tree;
		Iterator iter = begin();
		for (iter; iter != end(); iter++)
		{
			Pair h = (*iter);
			try {
				if (t.search_by_arg((*iter).get_arg())) {
					h += t.search_by_arg((*iter).get_arg())->value;
					result_tree.push(result_tree.getRoot(), h);
				}
				else throw "Result not defined";
			}
			catch (const char* e) {
				cout << "For elem " << h << " defined only one mapping. " << e << endl;
			}
		}
		return result_tree;
	}
	Tree operator-(Tree& t) {
		Tree result_tree;
		Iterator iter = begin();
		for (iter; iter != end(); iter++)
		{
			Pair h = (*iter);
			try {
				if (t.search_by_arg((*iter).get_arg())) {
					h -= t.search_by_arg((*iter).get_arg())->value;
					result_tree.push(result_tree.getRoot(), h);
				}
				else throw "Result not defined";
			}
			catch (const char* e) {
				cout << "For elem " << h << " defined only one mapping. " << e << endl;
			}
		}
		return result_tree;
	}
	Tree operator*(Tree& t) {
		Tree result_tree;
		Iterator iter = begin();
		for (iter; iter != end(); iter++)
		{
			Pair h = (*iter);
			try {
				if (t.search_by_arg((*iter).get_arg())) {
					h *= t.search_by_arg((*iter).get_arg())->value;
					result_tree.push(result_tree.getRoot(), h);
				}
				else throw "Result not defined";
			}
			catch (const char* e) {
				cout << "For elem " << h << " defined only one mapping. " << e << endl;
			}
		}
		return result_tree;
	}
	Tree operator/(Tree& t) {
		Tree result_tree;
		Iterator iter = begin();
		for (iter; iter != end(); iter++)
		{
			Pair h = (*iter);
			try {
				if (t.search_by_arg((*iter).get_arg())) {
					h /= t.search_by_arg((*iter).get_arg())->value;
					result_tree.push(result_tree.getRoot(), h);
				}
				else throw "Result not defined";
			}
			catch (const char* e) {
				cout << "For elem " << h << " defined only one mapping. " << e << endl;
			}
		}
		return result_tree;
	}

	//Stream I/O
	friend ostream& operator<<(ostream& os, Tree& t) {
		os << '{';
		t.prefixTraverse(t.getRoot(), os);
		os << '}';
		return os;
	}
	friend istream& operator>>(istream& is, Tree& t) {
		t.destroy_tree(t.getRoot());
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
};