#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
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
	void invert_pair() {
		swap(arg, value);
	}

	//Relations
	bool operator==(Pair& p) {
		return this->arg == p.arg;
	}
	bool operator!=(Pair& p) {
		return !(*this == p);
	}
	bool operator<(Pair& p) {
		return this->arg < p.arg;
	}
	bool operator>(Pair& p) {
		return this->arg > p.arg;
	}
	bool equal(Pair& p) {
		return this->arg == p.arg && this->value == p.value;
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
	Pair operator%(Pair& p) {
		Pair a(*this);
		a.value = a.value % p.value;
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
	Pair& operator%=(Pair& p) {
		this->value = this->value % p.value;
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
	Node* search_by_arg(Node* r, int arg) {
		if (!r || arg == r->value.get_arg()) return r;
		if (arg < r->value.get_arg()) return search_by_arg(r->left, arg);
		else return search_by_arg(r->right, arg);
	}
	Node* search_by_value(int value) {
		Iterator iter;
		for (iter = begin(); iter != end() && (*iter).get_value() != value; iter++);
		return iter.get_ptr();
	}


	bool isCorrect() {
		for (Iterator iter1 = begin(); iter1 != end(); iter1++) {
			int count = 0;
			for (Iterator iter2 = iter1; iter2 != end(); iter2++) {
				if (iter1->get_arg() == iter2->get_arg()) count++;
			}
			if (count > 1) return false;
		}
		return true;
	}
	void invert_tree() {
		for (Iterator iter = begin(); iter != end(); iter++)
			iter->invert_pair();
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
	Node*& getRoot() {
		return this->root;
	}
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
		root = nullptr;
	}
	Tree(const Tree& t) {
		if (!t.root) root = nullptr;
		else {
			root = new Node(t.root->value);
			if (t.root->left) {
				root->left = new Node(t.root->left->value);
				copy_tree(root, t.root->left, root->left);
			}
			if (t.root->right) {
				root->right = new Node(t.root->right->value);
				copy_tree(root, t.root->right, root->right);
			}
		}
	}
	Tree(string& str) {
		for (int i = 1; str[i]; i++)
		{
			int arg = 0;
			int val = 0;

			for (i; str[i] != '>'; i++) {
				arg = 10 * arg + str[i] - '0';
			}
			i++;
			for (i; str[i] != ',' && str[i] != '}'; i++)
				val = 10 * val + str[i] - '0';
			push(getRoot(), Pair(arg, val));
		}
	}
	~Tree() {
		delete root;
	}

	void destroy_tree(Node* r) {
		delete root;
		root = nullptr;
	}
	void copy_tree(Node* from, Node* s, Node* to) {
		if (s->left) to->left = new Node(s->left->value);
		if (s->right) to->right = new Node(s->right->value);
		to->parent = from;
		if (to->left) copy_tree(to, s->left, to->left);
		if (to->right) copy_tree(to, s->right, to->right);
	}

	Tree& operator=(const Tree& t) {
		if (this == &t) return *this;
		destroy_tree(root);
		if (!t.root) root = nullptr;
		else {
			root = new Node(t.root->value);
			if (t.root->left) {
				root->left = new Node(t.root->left->value);
				copy_tree(root, t.root->left, root->left);
			}
			if (t.root->right) {
				root->right = new Node(t.root->right->value);
				copy_tree(root, t.root->right, root->right);
			}
		}
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
	int getValue(int arg) {
		return search_by_arg(root, arg)->value.get_value();
	}

	//Access to elem
	Pair& at(int index) {
		Iterator iter = begin();
		int i = 0;
		for (i; i < index && iter != end(); i++, iter++);
		if (i == index) {
			return *iter;
		}
	}
	Tree get_invertable() {
		Tree newTree = *this;
		Tree result_tree;
		if (newTree.root) {
			newTree.invert_tree();
			if (newTree.isCorrect()) {
				for (Iterator iter = newTree.begin(); iter != newTree.end(); iter++)
					result_tree.push(result_tree.root, *iter);
			}
			else {
				cout << "Invertable mapping doesn't exist" << endl;
			}
		}
		return result_tree;
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
					if (iter1->get_value() == iter2->get_value()) {
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
			if (search_by_arg(root, iter->get_value())) {
				int a = iter->get_arg();
				int b = search_by_arg(root, iter->get_value())->value.get_value();
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
		while (iter1 != end() && iter2 != t.end()) {
			if ((*iter1++) != (*iter2++)) return false;
		}
		if (iter1 != end() || iter2 != t.end()) return false;
		return true;
	}
	bool operator!=(Tree& t) {
		return !(*this == t);
	}
	bool equal(Tree& t) {
		Iterator iter1 = begin();
		Iterator iter2 = t.begin();
		while (iter1 != end() && iter2 != t.end()) {
			if (!(*iter1++).equal(*iter2++)) return false;
		}
		if (iter1 != end() || iter2 != t.end()) return false;
		return true;
	}
	//Arithmetic operations
	Tree operator+(Tree& t) {
		Tree result_tree;
		Iterator iter = begin();
		for (iter; iter != end(); iter++)
		{
			Pair h = (*iter);
			try {
				if (t.search_by_arg(t.root, iter->get_arg())) {
					h += t.search_by_arg(t.root, iter->get_arg())->value;
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
				if (t.search_by_arg(t.root, iter->get_arg())) {
					h -= t.search_by_arg(t.root, iter->get_arg())->value;
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
				if (t.search_by_arg(t.root, iter->get_arg())) {
					h *= t.search_by_arg(t.root, iter->get_arg())->value;
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
				if (t.search_by_arg(t.root, iter->get_arg())) {
					h /= t.search_by_arg(t.root, iter->get_arg())->value;
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

	Tree operator%(Tree& t) {
		Tree result_tree;
		Iterator iter = begin();
		for (iter; iter != end(); iter++)
		{
			Pair h = (*iter);
			try {
				if (t.search_by_arg(t.root, iter->get_arg())) {
					h %= t.search_by_arg(t.root, iter->get_arg())->value;
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
		try
		{
			Pair a;
			char b;
			is >> b;
			if (b != '{') throw "Incorrect Input";
			do {
				is >> a;
				t.push(t.getRoot(), a);
				is >> b;
			} while (b != '}');
		}
		catch (const char* e)
		{
			is.clear();
			cout << e << endl;
		}
		return is;
	}
};

enum LexemeTokenClass { PUSH, POP, AR_OP, RELATION, JMP, JI, READ, WRITE, END, COMMENT, ERROR, ENDMARK, FIND, COMPOSITION, INVERT, MERGE };
enum SymbolicTokenClass { Letter, Digit, Ar_Op, Relation, Space, LF, Semicolon, Error, EndOfFile, Open_Brace, Closed_Brace, Comma };
enum States { s_A1, s_A2, s_B1, s_C1, s_D1, s_E1, s_E2, s_E3, s_F1, s_F2, s_F3, s_G1, s_H1, s_I1, s_I2, s_J1, s_K1, s_K2, s_K3, s_K4, s_Exit };

string LexemeTokenClassName[] = { "PUSH", "POP", "AR_OP", "RELATION", "JMP", "JI", "READ", "WRITE", "END", "COMMENT", "ERROR", "ENDMARK", "FIND", "COMPOSITION", "INVERT", "MERGE" };

const string keywords[] = { "end", "ji", "jmp", "pop", "push", "read", "write", "find", "composition", "invert", "merge" };

int init_vector[] = { 0, 0, 21, 0, 1, 18, 0, 0, 31, 3, 0, 0, 36, 0, 0, 6, 0, 11, 0, 0, 0, 0, 14, 0, 0, 0 };

struct SymbolicToken
{
	SymbolicTokenClass symbolic_token_class;
	int value;
};

struct LexemeToken
{
	LexemeTokenClass lexeme_token_class;
	int line;
	void* value;
	friend ostream& operator<<(ostream& os, const LexemeToken& lex)
	{
		cout << LexemeTokenClassName[lex.lexeme_token_class];
		if (lex.lexeme_token_class == PUSH || lex.lexeme_token_class == POP)
		{
			cout << '\t' << lex.value;
		}
		else if (lex.lexeme_token_class == AR_OP)
		{
			cout << '\t' << *((char*)(lex.value));
		}
		else if (lex.lexeme_token_class == RELATION)
		{
			cout << '\t' << (*(int*)(lex.value)) - 1;
		}
		else if (lex.lexeme_token_class == JMP || lex.lexeme_token_class == JI)
		{
			cout << '\t' << *((int*)(lex.value));
		}
		else {
			cout << '\t' << "<empty>\t\t";
		}
		cout << '\t' << lex.line;
		return os;
	}
};

class Variable {
private:
	string name;
	int type;
	void* value;
public:
	//Getters
	string getName()
	{
		return name;
	}
	int getType()
	{
		return type;
	}
	void* getValue()
	{
		return value;
	}
	//Constructors
	Variable()
	{
		name = "";
		type = -1;
		value = nullptr;
	}
	Variable(string name, Tree value, int type) 
	{
		this->name = name;
		this->value = new Tree(value);
		this->type = type;
	}
	Variable(string name, int value, int type) 
	{
		this->name = name;
		this->value = new int(value);
		this->type = type;
	}
	Variable(const Variable& v)
	{
		name = v.name;
		type = v.type;
		if (type == 0)
		{
			value = new int(*(int*)(v.value));
		}
		else
		{
			value = new Tree(*(Tree*)(v.value));
		}
	}
	~Variable()
	{
		if (value)
		{
			delete value;
		}
	}
	//Operators
	Variable& operator=(Variable var)
	{
		if (value)
		{
			delete value;
		}
		if (var.type == 0)
		{
			value = new int(*(int*)(var.value));
		}
		if (var.type == 1)
		{
			value = new Tree(*(Tree*)(var.value));
		}
		type = var.type;
		return *this;
	}
	Variable operator+(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
		}
		if (type == 0)
		{
			return Variable("", *(int*)value + *(int*)var.value, type);
		}
		else if (type == 1)
		{
			return Variable("", *(Tree*)value + *(Tree*)var.value, type);
		}
	}
	Variable operator-(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
		}
		if (type == 0)
		{
			return Variable("", *(int*)value - *(int*)var.value, type);
		}
		else if (type == 1)
		{
			return Variable("", *(Tree*)value - *(Tree*)var.value, type);
		}
	}
	Variable operator*(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
		}
		if (type == 0)
		{
			return Variable("", *(int*)value * *(int*)var.value, type);
		}
		else if (type == 1)
		{
			return Variable("", *(Tree*)value * *(Tree*)var.value, type);
		}
	}
	Variable operator/(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
		}
		if (type == 0)
		{
			return Variable("", *(int*)value / *(int*)var.value, type);
		}
		else if (type == 1)
		{
			return Variable("", *(Tree*)value / *(Tree*)var.value, type);
		}
	}
	Variable operator%(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
		}
		if (type == 0)
		{
			return Variable("", *(int*)value % *(int*)var.value, type);
		}
		else if (type == 1)
		{
			return Variable("", *(Tree*)value % *(Tree*)var.value, type);
		}
	}
	//Relations
	bool operator>(const Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal" << endl;
			exit(1);
		}
		if (type == 0)
		{
			return *(int*)value > *(int*)(var.value);
		}
		else if (type == 1)
		{
			cout << "Operator > is not define for Tree" << endl;
			exit(1);
		}
	}
	bool operator<(const Variable& var)
	{
		if (type != var.type)
		{
			cout << "Argument types are not equal" << endl;
			exit(1);
		}
		if (type == 0)
		{
			return *(int*)value < *(int*)(var.value);
		}
		else if (type == 1)
		{
			cout << "Operator < is not define for Tree" << endl;
			exit(1);
		}
	}
	bool operator==(const Variable& var)
	{
		if (type != var.type)
		{
			cout << "Argument types are not equal" << endl;
			exit(1);
		}
		if (type == 0)
		{
			return *(int*)value == *(int*)(var.value);
		}
		else if (type == 1)
		{
			return *(Tree*)value == *(Tree*)(var.value);
		}
	}
	bool operator!=(const Variable& var)
	{
		return !(*this == var);
	}
	bool operator>=(const Variable& var)
	{
		return (*this > var || *this == var);
	}
	bool operator<=(const Variable& var)
	{
		return (*this < var || *this == var);
	}
	friend ostream& operator<<(ostream& os, const Variable& var)
	{
		if (var.type == 0)
		{
			return os << *(int*)(var.value) << endl;
		}
		else if (var.type == 1)
		{
			return os << *(Tree*)(var.value) << endl;
		}
	}
};

vector<Variable*> table_const;
vector<Variable*> table_name;
vector<LexemeToken> list_lexeme;

const int state_number = 23;
const int class_number = 11;

class Parser
{
	SymbolicToken s;                    // Символьная лексема

	LexemeTokenClass lexeme_class;      // Регистр класса
	void* ptr;                          // Регистр указателя
	int num;                            // Регистр числа
	int rel;                            // Регистр отношения
	string name;                        // Регистр имени
	int pos;                            // Регистр обнаружения
	int value;                          // Регистр значение
	int line = 1;                       // Регистр строки
	string tree;						// Регистр дерева
	bool flag_const;

	typedef int (Parser::* function_pointer)();
	function_pointer table[state_number][class_number];

	struct SearchTableClass
	{
		char letter;
		int alt;
		function_pointer f;
	};

	SearchTableClass table_search[40];

	static SymbolicToken transliterator(int ch)
	{
		SymbolicToken s;
		s.value = 0;
		if (isdigit(ch))
		{
			s.symbolic_token_class = Digit;
			s.value = ch - '0';
		}
		else if (isalpha(ch))
		{
			s.symbolic_token_class = Letter;
			s.value = ch;
		}
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
		{
			s.symbolic_token_class = Ar_Op;
			s.value = ch;
		}
		else if (ch == '=' || ch == '!' || ch == '>' || ch == '<')
		{
			s.symbolic_token_class = Relation;
			s.value = ch;
		}
		else if (ch == ' ' || ch == '\t')
		{
			s.symbolic_token_class = Space;
			s.value = ch;
		}
		else if (ch == '\n')
		{
			s.symbolic_token_class = LF;
			s.value = ch;
		}
		else if (ch == ';')
		{
			s.symbolic_token_class = Semicolon;
			s.value = ch;
		}
		else if (ch == '{')
		{
			s.symbolic_token_class = Open_Brace;
			s.value = ch;
		}
		else if (ch == '}')
		{
			s.symbolic_token_class = Closed_Brace;
			s.value = ch;
		}
		else if (ch == ',')
		{
			s.symbolic_token_class = Comma;
			s.value = ch;
		}
		else if (ch == EOF)
		{
			s.symbolic_token_class = EndOfFile;
			s.value = ch;
		}
		else
		{
			s.symbolic_token_class = Error;
		}
		return s;
	}

	void CreateLexeme()
	{
		LexemeToken lexeme_token;
		lexeme_token.lexeme_token_class = lexeme_class;
		lexeme_token.line = line;
		lexeme_token.value = ptr;
		if (lexeme_class == PUSH)
		{
			lexeme_token.value = ptr;
		}
		else if (lexeme_class == POP)
		{
			lexeme_token.value = ptr;
		}
		else if (lexeme_class == AR_OP)
		{
			int* token_ar_op = new int;
			*token_ar_op = value;
			lexeme_token.value = token_ar_op;
		}
		else if (lexeme_class == RELATION)
		{
			int* token_rel = new int;
			*token_rel = rel;
			lexeme_token.value = token_rel;
		}
		else if (lexeme_class == JMP || lexeme_class == JI)
		{
			int* token_l = new int;
			*token_l = num;
			lexeme_token.value = token_l;
		}
		list_lexeme.push_back(lexeme_token);
	}

	int AddVariable()
	{
		for (string s : keywords)
		{
			if (s == name) return Error1();
		}
		for (Variable* i : table_name)
		{
			if (name == i->getName())
			{
				ptr = i;
				return 0;
			}
		}
		table_name.push_back(new Variable(name, 0, 0));
		ptr = table_name[table_name.size() - 1];
		return 0;
	}

	void AddConst()
	{
		if (flag_const == 0) return;
		if (tree.empty()) 
		{
			for (Variable* i : table_const)
			{
				if (i->getType() != 0) continue;
				if (num == *(int*)(i->getValue()))
				{
					ptr = i;
					return;
				}
			}
			table_const.push_back(new Variable("", num, 0));
			ptr = table_const[table_const.size() - 1];
		}
		else
		{
			Tree h(tree);
			tree.clear();
			for (Variable* i : table_const)
			{
				if (i->getType() != 1) continue;
				if (h.equal(*(Tree*)(i->getValue())))
				{
					ptr = i;
					return;
				}
			}
			table_const.push_back(new Variable("", h, 1));
			ptr = table_const[table_const.size() - 1];
		}
	}

	int A1()
	{
		return s_A1;
	}

	int A2()
	{
		return s_A2;
	}

	int B1()
	{
		return s_B1;
	}

	int C1()
	{
		return s_C1;
	}

	int D1()
	{
		return s_D1;
	}

	int E1()
	{
		return s_E1;
	}

	int E2()
	{
		return s_E2;
	}

	int E3()
	{
		return s_E3;
	}

	int F1()
	{
		return s_F1;
	}

	int F2()
	{
		return s_F2;
	}

	int F3()
	{
		return s_F3;
	}

	int G1()
	{
		return s_G1;
	}

	int H1()
	{
		return s_H1;
	}

	int I1()
	{
		return s_I1;
	}

	int I2()
	{
		return s_I2;
	}

	int J1()
	{
		return s_J1;
	}

	int K1()
	{
		return s_K1;
	}
	int K2()
	{
		return s_K2;
	}
	int K3()
	{
		return s_K3;
	}
	int K4()
	{
		return s_K4;
	}
	int Error1()
	{
		lexeme_class = ERROR;
		CreateLexeme();
		cout << "Error line: " << line << endl;
		return s_J1;
	}
	int A1a()
	{
		CreateLexeme();
		line++;
		return s_A1;
	}
	int A1b()
	{
		line++;
		return s_A1;
	}
	int A2a()
	{
		line++;
		return s_A2;
	}
	int A2b()
	{
		CreateLexeme();
		line++;
		return s_A2;
	}
	int A2c() {
		AddConst();
		CreateLexeme();
		line++;
		return s_A2;
	}
	int A2d()
	{
		AddVariable();
		CreateLexeme();
		line++;
		return s_A2;
	}
	int A2e()
	{
		if (rel == '!') return Error1();
		if (rel == '=')
		{
			rel = 1;
		}
		else if (rel == '<')
		{
			rel = 3;
		}
		else if (rel == '>')
		{
			rel = 4;
		}
		CreateLexeme();
		line++;
		return s_A2;
	}
	int A2f()
	{
		lexeme_class = ERROR;
		CreateLexeme();
		line++;
		return s_A2;
	}
	int B1a()
	{
		pos = init_vector[s.value - 'a'];
		if (pos == 0)
		{
			return Error1();
		}
		else
		{
			return s_B1;
		}
	}
	int B1b()
	{
		pos++;
		return s_B1;
	}
	int C1a()
	{
		lexeme_class = AR_OP;
		value = s.value;
		CreateLexeme();
		return s_C1;
	}
	int C1b()
	{
		lexeme_class = END;
		CreateLexeme();
		return s_C1;
	}
	int C1c()
	{
		lexeme_class = READ;
		CreateLexeme();
		return s_C1;
	}
	int C1d()
	{
		lexeme_class = WRITE;
		CreateLexeme();
		return s_C1;
	}
	int C1e()
	{
		AddConst();
		CreateLexeme();
		return s_C1;
	}
	int C1f()
	{
		AddVariable();
		CreateLexeme();
		return s_C1;
	}
	int C1g()
	{
		if (rel == '!') return Error1();
		if (rel == '=')
		{
			rel = 1;
		}
		else if (rel == '<')
		{
			rel = 3;
		}
		else if (rel == '>')
		{
			rel = 4;
		}
		CreateLexeme();
		return s_C1;
	}
	int C1h()
	{
		if (s.value == '=')
		{
			if (rel == '!')
			{
				rel = 2;
			}
			else if (rel == '<')
			{
				rel = 5;
			}
			else if (rel == '>')
			{
				rel = 6;
			}
		}
		else
		{
			return Error1();
		}
		CreateLexeme();
		return s_C1;
	}
	int C1i()
	{
		tree = tree + (char)s.value;
		AddConst();
		CreateLexeme();
		return s_C1;
	}
	int C1j()
	{
		lexeme_class = FIND;
		CreateLexeme();
		return s_C1;
	}
	int C1k()
	{
		lexeme_class = COMPOSITION;
		CreateLexeme();
		return s_C1;
	}
	int C1l()
	{
		lexeme_class = INVERT;
		CreateLexeme();
		return s_C1;
	}
	int C1m()
	{
		lexeme_class = MERGE;
		CreateLexeme();
		return s_C1;
	}
	int D1a()
	{
		lexeme_class = RELATION;
		rel = s.value;
		return s_D1;
	}
	int E1a()
	{
		lexeme_class = PUSH;
		flag_const = 1;
		return s_E1;
	}
	int E2a()
	{
		lexeme_class = JI;
		flag_const = 0;
		return s_E2;
	}
	int E2b()
	{
		lexeme_class = JMP;
		flag_const = 0;
		return s_E2;
	}
	int E3a()
	{
		lexeme_class = POP;
		return s_E3;
	}
	int G1a()
	{
		num = s.value;
		return s_G1;
	}
	int G1b()
	{
		num = 10 * num + s.value;
		return s_G1;
	}
	int H1a()
	{
		name = (char)s.value;
		return s_H1;
	}
	int H1b()
	{
		name = name + (char)s.value;
		return s_H1;
	}
	int I1a()
	{
		lexeme_class = COMMENT;
		return s_I1;
	}
	int I2a()
	{
		lexeme_class = COMMENT;
		return s_I2;
	}
	int I2b()
	{
		AddConst();
		CreateLexeme();
		lexeme_class = COMMENT;
		return s_I2;
	}
	int I2c()
	{
		AddVariable();
		CreateLexeme();
		lexeme_class = COMMENT;
		return s_I2;
	}
	int I2d()
	{
		if (rel == '!') return Error1();
		CreateLexeme();
		lexeme_class = COMMENT;
		return s_I2;
	}
	int K1a()
	{
		tree = (char)s.value;
		return s_K1;
	}
	int K1b()
	{
		tree = tree + (char)(s.value + '0');
		return s_K1;
	}
	int K2a()
	{
		if (s.value != '>') return Error1();
		tree = tree + (char)s.value;
		return s_K2;
	}
	int K2b()
	{
		tree = tree + (char)(s.value + '0');
		return s_K2;
	}
	int K3a()
	{
		tree = tree + (char)s.value;
		return s_K3;
	}
	int K4a()
	{
		tree = tree + (char)s.value;
		return s_K4;
	}
	int M1()
	{
		if (table_search[pos].letter == s.value)
		{
			return (this->*table_search[pos].f)();
		}
		else
		{
			pos = table_search[pos].alt;
			if (pos == 0)
			{
				return Error1();
			}
			else
			{
				return M1();
			}
		}
	}
	int Exit1()
	{
		lexeme_class = ENDMARK;
		CreateLexeme();
		return s_Exit;

	}
	int Exit2()
	{
		if (rel == '!') lexeme_class = ERROR;
		CreateLexeme();
		lexeme_class = ENDMARK;
		CreateLexeme();
		return s_Exit;
	}
	int Exit3()
	{
		AddConst();
		CreateLexeme();
		lexeme_class = ENDMARK;
		CreateLexeme();
		return s_Exit;
	}
	int Exit4()
	{
		AddVariable();
		CreateLexeme();
		lexeme_class = ENDMARK;
		CreateLexeme();
		return s_Exit;
	}

	void interpretator() {
		stack<Variable> Stack;

		for (int i = 0; list_lexeme[i].lexeme_token_class != END && list_lexeme[i].lexeme_token_class != ENDMARK; i++)
		{

			if (list_lexeme[i].lexeme_token_class == READ)
			{
				cout << "Enter 1 to int, 2 to Tree" << endl;
				int type;
				cin >> type;
				if (type == 1)
				{
					int val;
					cin >> val;
					Stack.push(Variable("", val, 0));
				}
				else if (type == 2)
				{
					Tree val;
					cin >> val;
					Stack.push(Variable("", val, 1));
				}
				else
				{
					cout << "Incorrect input, try again" << endl;
					continue;
				}
			}
			else if (list_lexeme[i].lexeme_token_class == WRITE) {
				if (!Stack.empty())
				{
					cout << Stack.top() << endl;
					Stack.pop();
				}
				else
				{
					cout << "Stack already empty" << endl;
				}
			}
			else if (list_lexeme[i].lexeme_token_class == PUSH)
			{
				Stack.push(Variable(*(Variable*)(list_lexeme[i].value)));
			}
			else if (list_lexeme[i].lexeme_token_class == POP)
			{
				*(Variable*)(list_lexeme[i].value) = Stack.top();
				Stack.pop();
			}
			else if (list_lexeme[i].lexeme_token_class == AR_OP)
			{
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable res;
				switch (*(int*)(list_lexeme[i].value))
				{
				case '+':
					res = a + b;
					break;
				case '-':
					res = a - b;
					break;
				case '*':
					res = a * b;
					break;
				case '/':
					res = a / b;
					break;
				case '%':
					res = a % b;
					break;
				}
				Stack.push(res);
			}
			else if (list_lexeme[i].lexeme_token_class == RELATION)
			{
				if (Stack.size() < 2)
				{
					cout << "Error: not enough variables" << endl;
					exit(1);
				}
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				int res;
				switch (*(int*)(list_lexeme[i].value))
				{
				case 1:
					res = a == b;
					break;
				case 2:
					res = a != b;
					break;
				case 3:
					res = a < b;
					break;
				case 4:
					res = a > b;
					break;
				case 5:
					res = a <= b;
					break;

				case 6:
					res = a >= b;
					break;
				}
				Stack.push(Variable("", res, 0));
			}
			else if (list_lexeme[i].lexeme_token_class == JMP)
			{
				int tmp_line = *((int*)(list_lexeme[i].value));
				for (i = tmp_line - 2; list_lexeme[i + 1].line < tmp_line; i++);
			}
			else if (list_lexeme[i].lexeme_token_class == JI) {
				if ((Stack.top().getType() == 0 && (*(int*)(Stack.top().getValue())) > 0) ||
					(Stack.top().getType() == 1 && (*(Tree*)(Stack.top().getValue())).getRoot())) {
					int tmp_line = *((int*)(list_lexeme[i].value));
					for (i = tmp_line - 2; list_lexeme[i + 1].line < tmp_line; i++);
				}
				Stack.pop();
			}
			else if (list_lexeme[i].lexeme_token_class == COMMENT);
			else if (list_lexeme[i].lexeme_token_class == FIND)
			{
				if (Stack.top().getType() == 0)
				{
					int ch = *(int*)(Stack.top().getValue());
					Stack.pop();
					if (Stack.top().getType() != 1)
					{
						cout << "Error: Incorrect argument type" << endl;
						exit(1);
					}
					Tree b = *(Tree*)(Stack.top().getValue());
					Stack.pop();
					int res = b.getValue(ch);
					Stack.push(Variable("", res, 0));
				}
				else
				{
					cout << "Error: Incorrect argument type" << endl;
					exit(1);
				}
			}
			else if (list_lexeme[i].lexeme_token_class == COMPOSITION)
			{
				if (Stack.top().getType() == 1)
				{
					Tree b = *(Tree*)(Stack.top().getValue());
					Stack.pop();
					if (Stack.top().getType() != 1)
					{
						cout << "Error: Argument types are not equal" << endl;
						exit(1);
					}
					Tree c = *(Tree*)(Stack.top().getValue());
					Stack.pop();
					Tree res = b.composition(c);
					Stack.push(Variable("", res, 1));
				}
				else
				{
					cout << "Error: Incorrect argument type" << endl;
					exit(1);
				}
			}
			else if (list_lexeme[i].lexeme_token_class == INVERT)
			{
				if (Stack.top().getType() == 1)
				{
					Tree b = *(Tree*)(Stack.top().getValue());
					Stack.pop();
					Tree res = b.get_invertable();
					Stack.push(Variable("", res, 1));
				}
				else
				{
					cout << "Error: Incorrect argument type" << endl;
					exit(1);
				}
			}
			else if (list_lexeme[i].lexeme_token_class == MERGE)
			{
				if (Stack.top().getType() == 1)
				{
					Tree b = *(Tree*)(Stack.top().getValue());
					Stack.pop();
					if (Stack.top().getType() != 1)
					{
						cout << "Error: Argument types are not equal"  << endl;
						exit(1);
					}
					Tree c = *(Tree*)(Stack.top().getValue());
					Stack.pop();
					Tree res = b.merge(c);
					Stack.push(Variable("", res, 1));
				}
				else
				{
					cout << "Error: Incorrect argument type" << endl;
					exit(1);
				}
			}
			else {
				cout << "Lexeme = Error, Line: " << list_lexeme[i].line << endl;
				exit(1);
			}
		}
	}

public:
	Parser()
	{
		for (int i = 0; i < state_number; i++)
			for (int j = 0; j < class_number; j++)
				table[i][j] = &Parser::Error1;

		table[s_A1][Letter] = &Parser::B1a;     table[s_A1][Ar_Op] = &Parser::C1a;      table[s_A1][Relation] = &Parser::D1a;   table[s_A1][Space] = &Parser::A1;       table[s_A1][LF] = &Parser::A1b;      table[s_A1][Semicolon] = &Parser::I1a;
		table[s_A2][Letter] = &Parser::B1a;     table[s_A2][Ar_Op] = &Parser::C1a;      table[s_A2][Relation] = &Parser::D1a;   table[s_A2][Space] = &Parser::A2;       table[s_A2][LF] = &Parser::A2a;      table[s_A2][Semicolon] = &Parser::I2a;     table[s_A2][EndOfFile] = &Parser::Exit1;
		table[s_B1][Letter] = &Parser::M1;      table[s_B1][LF] = &Parser::A2f;
		table[s_C1][Space] = &Parser::C1;       table[s_C1][LF] = &Parser::A2a;			table[s_C1][Semicolon] = &Parser::I2a;  table[s_C1][EndOfFile] = &Parser::Exit1;
		table[s_D1][Relation] = &Parser::C1h;   table[s_D1][Space] = &Parser::C1g;      table[s_D1][LF] = &Parser::A2e;         table[s_D1][Semicolon] = &Parser::I2d;                                                                                  table[s_D1][EndOfFile] = &Parser::Exit2;
		table[s_E1][Space] = &Parser::F1;       table[s_E1][LF] = &Parser::A2f;
		table[s_E2][Space] = &Parser::F2;       table[s_E2][LF] = &Parser::A2f;
		table[s_E3][Space] = &Parser::F3;       table[s_E3][LF] = &Parser::A2f;
		table[s_F1][Letter] = &Parser::H1a;     table[s_F1][Digit] = &Parser::G1a;      table[s_F1][Space] = &Parser::F1;       table[s_F1][Open_Brace] = &Parser::K1a;         table[s_F1][LF] = &Parser::A2f;
		table[s_F2][Digit] = &Parser::G1a;      table[s_F2][Space] = &Parser::F2;       table[s_F2][LF] = &Parser::A2f;
		table[s_F3][Letter] = &Parser::H1a;     table[s_F3][Space] = &Parser::F3;       table[s_F3][LF] = &Parser::A2f;
		table[s_G1][Digit] = &Parser::G1b;      table[s_G1][Space] = &Parser::C1e;      table[s_G1][LF] = &Parser::A2c;         table[s_G1][Semicolon] = &Parser::I2b;          table[s_G1][EndOfFile] = &Parser::Exit3;
		table[s_H1][Letter] = &Parser::H1b;     table[s_H1][Digit] = &Parser::H1b;      table[s_H1][Space] = &Parser::C1f;      table[s_H1][LF] = &Parser::A2d;                 table[s_H1][Semicolon] = &Parser::I2c;      table[s_H1][EndOfFile] = &Parser::Exit4;
		table[s_I1][Letter] = &Parser::I1;      table[s_I1][Digit] = &Parser::I1;       table[s_I1][Ar_Op] = &Parser::I1;       table[s_I1][Relation] = &Parser::I1;            table[s_I1][Space] = &Parser::I1;           table[s_I1][LF] = &Parser::A1a;  table[s_I1][Semicolon] = &Parser::I1;  table[s_I1][Error] = &Parser::I1;
		table[s_I2][Letter] = &Parser::I2;      table[s_I2][Digit] = &Parser::I2;       table[s_I2][Ar_Op] = &Parser::I2;       table[s_I2][Relation] = &Parser::I2;            table[s_I2][Space] = &Parser::I2;           table[s_I2][LF] = &Parser::A2b;  table[s_I2][Semicolon] = &Parser::I2;  table[s_I2][Error] = &Parser::I2; table[s_I2][EndOfFile] = &Parser::Exit1;
		table[s_J1][Letter] = &Parser::J1;      table[s_J1][Digit] = &Parser::J1;       table[s_J1][Ar_Op] = &Parser::J1;       table[s_J1][Relation] = &Parser::J1;            table[s_J1][Space] = &Parser::J1;           table[s_J1][LF] = &Parser::A2a;  table[s_J1][Semicolon] = &Parser::J1;  table[s_J1][Error] = &Parser::J1; table[s_J1][EndOfFile] = &Parser::Exit1;
		table[s_K1][Digit] = &Parser::K1b;		table[s_K1][Relation] = &Parser::K2a;	
		table[s_K2][Digit] = &Parser::K2b;      table[s_K2][Space] = &Parser::K3;       table[s_K2][Comma] = &Parser::K4a;     table[s_K2][Closed_Brace] = &Parser::C1i;
		table[s_K3][Space] = &Parser::K3;       table[s_K3][Comma] = &Parser::K4a;
		table[s_K4][Digit] = &Parser::K1b;      table[s_K4][Space] = &Parser::K4;

		for (int i = 0; i < 40; i++) table_search[i].f = &Parser::B1b;
		table_search[1].letter = 'n';
		table_search[2].letter = 'd';  table_search[2].f = &Parser::C1b;
		table_search[3].letter = 'i';  table_search[3].f = &Parser::E2a;    table_search[3].alt = 4;
		table_search[4].letter = 'm';
		table_search[5].letter = 'p';  table_search[5].f = &Parser::E2b;
		table_search[6].letter = 'o';                                       table_search[6].alt = 8;
		table_search[7].letter = 'p';  table_search[7].f = &Parser::E3a;
		table_search[8].letter = 'u';
		table_search[9].letter = 's';
		table_search[10].letter = 'h'; table_search[10].f = &Parser::E1a;
		table_search[11].letter = 'e';
		table_search[12].letter = 'a';
		table_search[13].letter = 'd'; table_search[13].f = &Parser::C1c;
		table_search[14].letter = 'r';
		table_search[15].letter = 'i';
		table_search[16].letter = 't';
		table_search[17].letter = 'e'; table_search[17].f = &Parser::C1d;
		table_search[18].letter = 'i';
		table_search[19].letter = 'n';
		table_search[20].letter = 'd'; table_search[20].f = &Parser::C1j;
		table_search[21].letter = 'o';
		table_search[22].letter = 'm';
		table_search[23].letter = 'p';
		table_search[24].letter = 'o';
		table_search[25].letter = 's';
		table_search[26].letter = 'i';
		table_search[27].letter = 't';
		table_search[28].letter = 'i';
		table_search[29].letter = 'o';
		table_search[30].letter = 'n'; table_search[30].f = &Parser::C1k;
		table_search[31].letter = 'n';
		table_search[32].letter = 'v';
		table_search[33].letter = 'e';
		table_search[34].letter = 'r';
		table_search[35].letter = 't'; table_search[35].f = &Parser::C1l;
		table_search[36].letter = 'e';
		table_search[37].letter = 'r';
		table_search[38].letter = 'g';
		table_search[39].letter = 'e'; table_search[39].f = &Parser::C1m;
	}

	void printLists() {
		cout << "table_const\nAdress\t\t\tValue" << endl;
		for (auto i : table_const) {
			cout << i << '\t';
			if (i->getType() == 0) cout << *(int*)(i->getValue()) << endl;
			else cout << *(Tree*)(i->getValue()) << endl;
		}

		cout << endl << "table_name\nAdress\t\t\tName" << endl;
		for (auto i : table_name)
			cout << i << '\t' << i->getName() << endl;


		cout << endl << "Lexeme list\nClass\tAdress\t\t\tLine" << endl;
		for (auto i : list_lexeme)
			cout << i << endl;

		cout << endl;
	}

	void parse(const char* filename)
	{
		ifstream in(filename);
		if (!in)
		{
			cout << "Не удалось открыть файл " << filename << endl;
			return;
		}

		int ch;             
		int state = s_A1;
		while (state != s_Exit)
		{	
			ch = in.get();
			s = transliterator(ch);
			state = (this->*table[state][s.symbolic_token_class])();
		}
		printLists();
		interpretator();
		in.close();
	}
};

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	if (argc != 2)
	{
		cout << "Формат: ./имя-исполняемого-файла имя-входного-файла" << endl;
		return 1;
	}

	Parser p;
	p.parse(argv[1]);

	return 0;
}