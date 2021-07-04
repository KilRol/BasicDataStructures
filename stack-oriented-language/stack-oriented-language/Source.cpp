#include <iostream>
#include <vector>
#include <fstream>
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
		root = nullptr;
		if (str[1] == '}') return;
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
			destroy_tree(root);
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
	//Operators
	Variable& operator=(Variable var)
	{
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
			return *(int*)value > * (int*)(var.value);
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

enum LexemeTokenClass { AT, COMPOSITION, FIND, INVERT, MERGE, ARROW_LEFT, ARROW_RIGHT, INT, TREE, CONST, VARIABLE, AR_OP, RELATION, ASSIGNMENT, DECLARE, SET, WHILE, DO, FOR, TO, IF, THEN, ELSE, CIN, COUT, MARK, JUMP, SWITCH, CASE, DEFAULT, BAD, BEGIN, END, COMMENT, SEMICOLON, COMMA, OPEN_BRACKET, CLOSED_BRACKET, COLON, ERROR, ENDMARK, END_OF_FILE };
enum SymbolicTokenClass { Letter, Digit, Ar_Op, Relation, Space, LF, Semicolon, Colon, OpenCurveBracket, ClosedCurveBracket, OpenBracket, ClosedBracket, Comma, Error, EndOfFile };
enum States { s_A1, s_B1, s_C1, s_C2, s_C3, s_C4, s_D1, s_E1, s_E2, s_E3, s_F1, s_F2, s_F3, s_G1, s_G2, s_G3, s_G4, s_H1, s_I1, s_I2, s_J1, s_K1, s_K2, s_K3, s_K4, s_Exit };

string LexemeTokenClassName[] = { "AT", "COMPOSITION", "FIND", "INVERT", "MERGE","ARROW_LEFT", "ARROW_RIGHT", "INT", "TREE", "CONST", "VARIABLE", "AR_OP", "RELATION", "ASSIGNMENT", "DECLARE", "SET", "WHILE", "DO", "FOR", "TO", "IF", "THEN", "ELSE", "CIN", "COUT", "MARK", "JUMP", "SWITCH", "CASE", "DEFAULT", "BAD", "BEGIN", "END", "COMMENT", "SEMICOLON", "COMMA", "OPEN_BRACKET", "CLOSED_BRACKET", "COLON","ERROR", "ENDMARK", "END_OF_FILE" };

const string keywords[] = { "int" , "tree", "while", "for", "declare", "set", "to", "do", "if", "then", "else", "cin", "cout", "jump", "switch", "case", "default", "end", "bad", "begin" };

int init_vector[] = { 1, 2, 8, 25, 37, 42, 0, 0, 47, 54, 0, 0, 57, 0, 0, 0, 0, 0, 61, 68, 0, 0, 75, 0, 0, 0 };

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

		cout << "  " << lex.line << "\t" << LexemeTokenClassName[lex.lexeme_token_class];
		if (lex.lexeme_token_class == AR_OP ) {
			cout << "\t\t\t\t\t\t" << *(char*)lex.value; 
		}
		if (lex.lexeme_token_class == ASSIGNMENT) {
			cout << "\t\t\t\t\t" << *(char*)lex.value;
		}
		else if (lex.lexeme_token_class == RELATION) {
			cout << "\t\t\t";
			switch (*(int*)lex.value) {
			case 1:
				cout << "\t\t==";
				break;
			case 2:
				cout << "\t\t!=";
				break;
			case 3:
				cout << "\t\t<";
				break;
			case 4:
				cout << "\t\t>";
				break;
			case 5:
				cout << "\t\t<=";
				break;
			case 6:
				cout << "\t\t>=";
				break;
			}
		}
		else if (lex.lexeme_token_class == VARIABLE) {
			cout << "\t\t"  << lex.value << "\t\t" << ((Variable*)lex.value)->getName();
		}
		else if (lex.lexeme_token_class == MARK) {
			cout << "\t\t\t" << lex.value << "\t\t" << (*(string*)lex.value);
		}
		else if (lex.lexeme_token_class == CONST) {
			if (((Variable*)lex.value)->getType() == 0)
				cout << "\t\t\t" << lex.value << "\t\t" << *(int*)(((Variable*)lex.value)->getValue()) ;
			else 
				cout << "\t\t\t" << lex.value << "\t\t" << *(Tree*)(((Variable*)lex.value)->getValue());
		}
		else if (lex.lexeme_token_class == JUMP) { 
			cout << "\t\t\t\t\t\t" << *(string*)lex.value;
		}

		return os;
	}
};

vector<Variable*> table_const;
vector<Variable*> table_name;
vector<string*> table_mark;
vector<LexemeToken> list_lexeme;

const int state_number = 100;
const int class_number = 100;

class Parser {
	SymbolicToken s;

	LexemeTokenClass lexeme_class;
	void* ptr;
	int num;
	int rel;
	string name;
	int pos;
	int value;
	int line = 1;
	string tree;
	bool flag_const;

	typedef int (Parser::* function_pointer)();
	function_pointer table[state_number][class_number];

	struct SearchTableClass
	{
		char letter;
		int alt;
		function_pointer f;
	};

	SearchTableClass table_search[400];

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
		else if (ch == ':')
		{
			s.symbolic_token_class = Colon;
			s.value = ch;
		}
		else if (ch == ';')
		{
			s.symbolic_token_class = Semicolon;
			s.value = ch;
		}
		else if (ch == '{')
		{
			s.symbolic_token_class = OpenCurveBracket;
			s.value = ch;
		}
		else if (ch == '}')
		{
			s.symbolic_token_class = ClosedCurveBracket;
			s.value = ch;
		}
		else if (ch == '(')
		{
			s.symbolic_token_class = OpenBracket;
			s.value = ch;
		}
		else if (ch == ')')
		{
			s.symbolic_token_class = ClosedBracket;
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

		if (lexeme_class == AR_OP || lexeme_class == COLON || lexeme_class == SEMICOLON || lexeme_class == COMMA || lexeme_class == OPEN_BRACKET || lexeme_class == CLOSED_BRACKET) {
			int* a = new int(value);
			lexeme_token.value = a;
		}
		else if (lexeme_class == RELATION)
		{
			int* token_rel = new int;
			*token_rel = rel;
			lexeme_token.value = token_rel;
		}
		else if (lexeme_class == ASSIGNMENT)
		{
			int* token_rel = new int('=');
			lexeme_token.value = token_rel;
		}
		else if (lexeme_class == VARIABLE || lexeme_class == CONST || lexeme_class == MARK) {
			lexeme_token.value = ptr;
		}
		else if (lexeme_class == JUMP) {
			lexeme_token.value = new string(name);
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
				if ((*(Tree*)(i->getValue())).getRoot())
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

	int AddMark() {
		for (string s : keywords)
		{
			if (s == name) return Error1();
		}
		for (string* i : table_mark)
		{
			if (name == *i)
			{
				ptr = i;
				return 0;
			}
		}
		table_mark.push_back(new string(name));
		ptr = table_mark[table_mark.size() - 1];
		return 0;
	}

	int A1()
	{
		return s_A1;
	}

	int B1()
	{
		return s_B1;
	}

	int C1()
	{
		return s_C1;
	}
	int C2()
	{
		return s_C1;
	}
	int C3()
	{
		return s_C3;
	}
	int C4()
	{
		return s_C4;
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
	int G2()
	{
		return s_G2;
	}
	int G3()
	{
		return s_G3;
	}
	int G4()
	{
		return s_G4;
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
	int A1a()
	{
		line++;
		return s_A1;
	}
	int A1b()
	{
		CreateLexeme();
		line++;
		return s_A1;
	}
	int A1c() {
		CreateLexeme();
		return s_A1;
	}
	int A1d()
	{
		if (rel == '!') return Error1();
		else if (rel == '<')
		{
			rel = 3;
		}
		else if (rel == '>')
		{
			rel = 4;
		}
		lexeme_class = RELATION;
		if (rel == '=') {
			lexeme_class = ASSIGNMENT;
		}
		CreateLexeme();
		return s_A1;
	}
	int A1e()
	{
		if (rel == '!') return Error1();
		else if (rel == '<')
		{
			rel = 3;
		}
		else if (rel == '>')
		{
			rel = 4;
		}
		lexeme_class = RELATION;
		if (rel == '=') {
			lexeme_class = ASSIGNMENT;
		}
		CreateLexeme();
		line++;
		return s_A1;
	}
	int A1_const()
	{
		AddConst();
		CreateLexeme();
		return s_A1;
	}
	int A1_constP()
	{
		AddConst();
		CreateLexeme();
		line++;
		return s_A1;
	}
	int A1_mark()
	{
		AddMark();
		CreateLexeme();
		return s_A1;
	}
	int A1_markP()
	{
		AddMark();
		CreateLexeme();
		line++;
		return s_A1;
	}
	int A1_var()
	{
		AddVariable();
		CreateLexeme();
		return s_A1;
	}
	int A1_varP()
	{
		AddVariable();
		CreateLexeme();
		line++;
		return s_A1;
	}
	int B1a()
	{
		pos = init_vector[s.value - 'a'];
		if (pos == 0)
		{
			return F1a();
		}
		lexeme_class = VARIABLE;
		if (!isalpha(s.value)) s.value += '0';
		name = (char)s.value;
		return s_B1;
	}
	int B1b()
	{
		pos++;
		return s_B1;
	}
	int C1c()
	{
		if (s.value == '=')
		{
			if (rel == '=')
			{
				rel = 1;
			}
			else if (rel == '!')
			{
				rel = 2;
			}
			else if (rel == 3)
			{
				rel = 5;
			}
			else if (rel == 4)
			{
				rel = 6;
			}
		}
		else
		{
			return Error1();
		}
		lexeme_class = RELATION;
		return s_C1;
	}
	int C1_constTree() {
		lexeme_class = CONST;
		tree = tree + (char)s.value;
		AddConst();
		return s_C1;
	}
	int C1_semicolon() {
		lexeme_class = SEMICOLON;
		value = s.value;
		return s_C1;
	}
	int C1_openBracket() {
		lexeme_class = OPEN_BRACKET;
		value = s.value;
		return s_C1;
	}
	int C1_int() {
		lexeme_class = INT;
		return s_C1;
	}
	int C1_at() {
		lexeme_class = AT;
		return s_C1;
	}
	int C1_composition() {
		lexeme_class = COMPOSITION;
		return s_C1;
	}
	int C1_find() {
		lexeme_class = FIND;
		return s_C1;
	}
	int C1_invert() {
		lexeme_class = INVERT;
		return s_C1;
	}
	int C1_merge() {
		lexeme_class = MERGE;
		return s_C1;
	}
	int C1_closedBracekt() {
		lexeme_class = CLOSED_BRACKET;
		value = s.value;
		return s_C1;
	}
	int C1_comma() {
		lexeme_class = COMMA;
		value = s.value;
		return s_C1;
	}
	int C1_bad() {
		lexeme_class = BAD;
		return s_C1;
	}
	int C1_begin() {
		lexeme_class = BEGIN;
		return s_C1;
	}
	int C1_case() {
		lexeme_class = CASE;
		return s_C1;
	}
	int C1_cin() {
		lexeme_class = CIN;
		return s_C1;
	}
	int C1_cout() {
		lexeme_class = COUT;
		return s_C1;
	}
	int C1_declare() {
		lexeme_class = DECLARE;
		return s_C1;
	}
	int C1_default() {
		lexeme_class = DEFAULT;
		return s_C1;
	}
	int C1_do() {
		lexeme_class = DO;
		return s_C1;
	}
	int C1_else() {
		lexeme_class = ELSE;
		return s_C1;
	}
	int C1_end() {
		lexeme_class = END;
		return s_C1;
	}
	int C1_for() {
		lexeme_class = FOR;
		return s_C1;
	}
	int C1_if() {
		lexeme_class = IF;
		return s_C1;
	}
	int C1_tree() {
		lexeme_class = TREE;
		return s_C1;
	}
	int C1_set() {
		lexeme_class = SET;
		return s_C1;
	}
	int C1_switch() {
		lexeme_class = SWITCH;
		return s_C1;
	}
	int C1_then() {
		lexeme_class = THEN;
		return s_C1;
	}
	int C1_to() {
		lexeme_class = TO;
		return s_C1;
	}
	int C1_while() {
		lexeme_class = WHILE;
		return s_C1;
	}
	int C1_ArrowL() {
		if (s.value == '-' && rel == 3) {
			lexeme_class = ARROW_LEFT;
		}
		else {
			return Error1();
		}
		return s_C1;
	}
	int C1_ArrowR() {
		if (value == '-' && s.value == '>') {
			lexeme_class = ARROW_RIGHT;
		}
		else {
			return Error1();
		}
		return s_C1;
	}
	int C2a() {
		lexeme_class = AR_OP;
		value = s.value;
		return s_C2;
	}
	int C3_jump() {
		lexeme_class = JUMP;
		name.clear();
		return s_C3;
	}
	int C4a() {
		if (!isalpha(s.value)) s.value += '0';
		name += (char)s.value;
		return s_C4;
	}
	int D1a() {
		lexeme_class = RELATION;
		if (s.value == '=') {
			lexeme_class = ASSIGNMENT;
			rel = s.value;
		}
		else if (s.value == '<') rel = 3;
		else if (s.value == '>') rel = 4;
		else rel = s.value;
		return s_D1;
	}
	int E1a() {
		lexeme_class = COLON;
		value = s.value;
		return E1();
	}
	int E2a() {
		lexeme_class = MARK;
		if (!isalpha(s.value)) s.value += '0';
		name = (char)s.value;
		return s_E2;
	}
	int E2b() {
		if (!isalpha(s.value)) s.value += '0';
		name += (char)s.value;
		return s_E2;
	}
	int F1a() {
		lexeme_class = VARIABLE;
		if (!isalpha(s.value)) s.value += '0';
		name = (char)s.value;
		return s_F1;
	}
	int F1b() {
		lexeme_class = VARIABLE;
		if (!isalpha(s.value)) s.value += '0';
		name += (char)s.value;
		return s_F1;
	}
	int G1a() {
		tree.clear();
		lexeme_class = CONST;
		num = s.value;
		return s_G1;
	}
	int G1b()
	{
		num = 10 * num + s.value;
		return s_G1;
	}
	int G2a() {
		rel = 0;
		tree = (char)s.value;
		return s_G2;
	}
	int G2b() {
		tree = tree + (char)(s.value + '0');
		return s_G2;
	}
	int G3a()
	{
		if (s.value != '>') return Error1();
		tree = tree + (char)s.value;
		return s_G3;
	}
	int G3b() {
		tree = tree + (char)(s.value + '0');
		return s_G3;
	}
	int G4a() {
		tree = tree + (char)s.value;
		return s_G4;
	}
	int I1a() {
		if (value == '-' && s.value == '-') {
			lexeme_class = COMMENT;
			CreateLexeme();
			return s_I1;
		}
		else {
			return Error1();
		}
	}
	int M1()
	{
		if (table_search[pos].letter == s.value)
		{
			if (!isalpha(s.value)) s.value += '0';
			name += (char)s.value;
			return (this->*table_search[pos].f)();
		}
		else
		{
			pos = table_search[pos].alt;
			if (pos == 0)
			{
				return F1b();
			}
			else
			{
				return M1();
			}
		}
	}



	int Error1() {
		lexeme_class = ERROR;
		CreateLexeme();
		cout << "Error line: " << line << endl;
		return s_J1;
	}
	int Exit1()
	{
		lexeme_class = ENDMARK;
		CreateLexeme();
		return s_Exit;
	}
	int Exit2()
	{
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
	int Exit5()
	{
		AddMark();
		CreateLexeme();
		lexeme_class = ENDMARK;
		CreateLexeme();
		return s_Exit;
	}
public:
	Parser()
	{
		for (int i = 0; i < state_number; i++)
			for (int j = 0; j < class_number; j++)
				table[i][j] = &Parser::Error1;

		table[s_A1][Letter] = &Parser::B1a;		table[s_A1][Digit] = &Parser::G1a;		table[s_A1][Ar_Op] = &Parser::C2a;			table[s_A1][Relation] = &Parser::D1a;			table[s_A1][Space] = &Parser::A1;			table[s_A1][LF] = &Parser::A1a;			table[s_A1][Semicolon] = &Parser::C1_semicolon;		table[s_A1][Colon] = &Parser::E1a;		table[s_A1][OpenCurveBracket] = &Parser::G2a;															table[s_A1][OpenBracket] = &Parser::C1_openBracket;		table[s_A1][ClosedBracket] = &Parser::C1_closedBracekt;		table[s_A1][Comma] = &Parser::C1_comma;											table[s_A1][EndOfFile] = &Parser::Exit1;
		table[s_B1][Letter] = &Parser::M1;																																			table[s_B1][Space] = &Parser::A1_var;		table[s_B1][LF] = &Parser::A1_varP;																																																																																																				table[s_B1][EndOfFile] = &Parser::Exit4;
		table[s_C1][Letter] = &Parser::F1b;		table[s_C1][Digit] = &Parser::F1b;																									table[s_C1][Space] = &Parser::A1c;			table[s_C1][LF] = &Parser::A1b;																																																																																																					table[s_C1][EndOfFile] = &Parser::Exit2;
		table[s_C2][Ar_Op] = &Parser::I1a;			table[s_C2][Relation] = &Parser::C1_ArrowR;		table[s_C2][Space] = &Parser::A1c;			table[s_C2][LF] = &Parser::A1b;																																																																																																					table[s_C2][EndOfFile] = &Parser::Exit2;
		table[s_C3][Letter] = &Parser::C4a;																																			table[s_C3][Space] = &Parser::C3;
		table[s_C4][Letter] = &Parser::C4a;		table[s_C4][Digit] = &Parser::C4a;																									table[s_C4][Space] = &Parser::A1c;			table[s_C4][LF] = &Parser::A1b;																																																																																																					table[s_C4][EndOfFile] = &Parser::Exit2;
		table[s_D1][Ar_Op] = &Parser::C1_ArrowL;	table[s_D1][Relation] = &Parser::C1c;			table[s_D1][Space] = &Parser::A1d;			table[s_D1][LF] = &Parser::A1e;																																																																																																					table[s_D1][EndOfFile] = &Parser::Exit2;
		table[s_E1][Letter] = &Parser::E2a;																																			table[s_E1][Space] = &Parser::A1c;			table[s_E1][LF] = &Parser::A1b;																																																																																																					table[s_E1][EndOfFile] = &Parser::Exit2;
		table[s_E2][Letter] = &Parser::E2b;		table[s_E2][Digit] = &Parser::E2b;																									table[s_E2][Space] = &Parser::A1_mark;		table[s_E2][LF] = &Parser::A1_markP;	table[s_E2][EndOfFile] = &Parser::Exit5;
		table[s_F1][Letter] = &Parser::F1b;		table[s_F1][Digit] = &Parser::F1b;																									table[s_F1][Space] = &Parser::A1_var;		table[s_F1][LF] = &Parser::A1_varP;																																																																																																				table[s_F1][EndOfFile] = &Parser::Exit4;
		table[s_G1][Digit] = &Parser::G1b;																									table[s_G1][Space] = &Parser::A1_const;		table[s_G1][LF] = &Parser::A1_constP;																																																																																																			table[s_G1][EndOfFile] = &Parser::Exit3;
		table[s_G2][Digit] = &Parser::G2b;													table[s_G2][Relation] = &Parser::G3a;
		table[s_G3][Digit] = &Parser::G3b;																																																																																	table[s_G3][ClosedCurveBracket] = &Parser::C1_constTree;																													table[s_G3][Comma] = &Parser::G4a;
		table[s_G4][Digit] = &Parser::G2b;																									table[s_G4][Space] = &Parser::G4;
		table[s_I1][Letter] = &Parser::I1;		table[s_I1][Digit] = &Parser::I1;		table[s_I1][Ar_Op] = &Parser::I1;			table[s_I1][Relation] = &Parser::I1;			table[s_I1][Space] = &Parser::I1;			table[s_I1][LF] = &Parser::A1a;			table[s_I1][Semicolon] = &Parser::I1;				table[s_I1][Colon] = &Parser::I1;		table[s_I1][OpenCurveBracket] = &Parser::I1;	table[s_I1][ClosedCurveBracket] = &Parser::I1;			table[s_I1][OpenBracket] = &Parser::I1;					table[s_I1][ClosedBracket] = &Parser::I1;					table[s_I1][Comma] = &Parser::I1;			table[s_I1][Error] = &Parser::I1; 	table[s_I1][EndOfFile] = &Parser::Exit1;
		table[s_J1][Letter] = &Parser::J1;		table[s_J1][Digit] = &Parser::J1;		table[s_J1][Ar_Op] = &Parser::J1;			table[s_J1][Relation] = &Parser::J1;			table[s_J1][Space] = &Parser::J1;			table[s_J1][LF] = &Parser::A1a;			table[s_J1][Semicolon] = &Parser::J1;				table[s_J1][Colon] = &Parser::J1;		table[s_J1][OpenCurveBracket] = &Parser::J1;	table[s_J1][ClosedCurveBracket] = &Parser::J1;			table[s_J1][OpenBracket] = &Parser::J1;					table[s_J1][ClosedBracket] = &Parser::J1;					table[s_J1][Comma] = &Parser::J1;			table[s_J1][Error] = &Parser::J1;	table[s_J1][EndOfFile] = &Parser::Exit1;


		for (int i = 0; i < 400; i++) table_search[i].f = &Parser::B1b;

		//at
		table_search[1].letter = 't';	table_search[1].f = &Parser::C1_at;
		//bad
		table_search[2].letter = 'a';												table_search[2].alt = 4;
		table_search[3].letter = 'd';	table_search[3].f = &Parser::C1_bad;
		//begin
		table_search[4].letter = 'e';
		table_search[5].letter = 'g';
		table_search[6].letter = 'i';
		table_search[7].letter = 'n';	table_search[7].f = &Parser::C1_begin;
		//case
		table_search[8].letter = 'a';												table_search[8].alt = 11;
		table_search[9].letter = 's';
		table_search[10].letter = 'e';	table_search[10].f = &Parser::C1_case;
		//cin
		table_search[11].letter = 'i';												table_search[11].alt = 13;
		table_search[12].letter = 'n';	table_search[12].f = &Parser::C1_cin;
		//composition
		table_search[13].letter = 'o';
		table_search[14].letter = 'm';												table_search[14].alt = 23;
		table_search[15].letter = 'p';
		table_search[16].letter = 'o';
		table_search[17].letter = 's';
		table_search[18].letter = 'i';
		table_search[19].letter = 't';
		table_search[20].letter = 'i';
		table_search[21].letter = 'o';
		table_search[22].letter = 'n';	table_search[22].f = &Parser::C1_composition;
		//cout
		table_search[23].letter = 'u';
		table_search[24].letter = 't';	table_search[24].f = &Parser::C1_cout;
		//declare
		table_search[25].letter = 'e';												table_search[25].alt = 36;
		table_search[26].letter = 'c';												table_search[26].alt = 31;
		table_search[27].letter = 'l';
		table_search[28].letter = 'a';
		table_search[29].letter = 'r';
		table_search[30].letter = 'e';	table_search[30].f = &Parser::C1_declare;
		//default
		table_search[31].letter = 'f';
		table_search[32].letter = 'a';
		table_search[33].letter = 'u';
		table_search[34].letter = 'l';
		table_search[35].letter = 't';	table_search[35].f = &Parser::C1_default;
		//do
		table_search[36].letter = 'o';	table_search[36].f = &Parser::C1_do;
		//else
		table_search[37].letter = 'l';												table_search[37].alt = 40;
		table_search[38].letter = 's';
		table_search[39].letter = 'e';	table_search[39].f = &Parser::C1_else;
		//end
		table_search[40].letter = 'n';
		table_search[41].letter = 'd';	table_search[41].f = &Parser::C1_end;
		//find
		table_search[42].letter = 'i';												table_search[42].alt = 45;
		table_search[43].letter = 'n';
		table_search[44].letter = 'd';	table_search[44].f = &Parser::C1_find;
		//for
		table_search[45].letter = 'o';
		table_search[46].letter = 'r';	table_search[46].f = &Parser::C1_for;
		//if
		table_search[47].letter = 'f';	table_search[47].f = &Parser::C1_if;		table_search[47].alt = 48;
		//int
		table_search[48].letter = 'n';
		table_search[49].letter = 't';	table_search[49].f = &Parser::C1_int;		table_search[49].alt = 50;
		//invert
		table_search[50].letter = 'v';
		table_search[51].letter = 'e';
		table_search[52].letter = 'r';
		table_search[53].letter = 't';	table_search[53].f = &Parser::C1_invert;
		//jump
		table_search[54].letter = 'u';
		table_search[55].letter = 'm';
		table_search[56].letter = 'p';	table_search[56].f = &Parser::C3_jump;
		//merge
		table_search[57].letter = 'e';
		table_search[58].letter = 'r';
		table_search[59].letter = 'g';
		table_search[60].letter = 'e'; table_search[60].f = &Parser::C1_merge;
		//set
		table_search[61].letter = 'e';												table_search[61].alt = 63;
		table_search[62].letter = 't';	table_search[62].f = &Parser::C1_set;
		//switch
		table_search[63].letter = 'w';
		table_search[64].letter = 'i';
		table_search[65].letter = 't';
		table_search[66].letter = 'c';
		table_search[67].letter = 'h';	table_search[67].f = &Parser::C1_switch;
		//then
		table_search[68].letter = 'h';												table_search[68].alt = 71;
		table_search[69].letter = 'e';
		table_search[70].letter = 'n';	table_search[70].f = &Parser::C1_then;
		//to
		table_search[71].letter = 'o';	table_search[71].f = &Parser::C1_to;		table_search[71].alt = 72;
		//tree
		table_search[72].letter = 'r';
		table_search[73].letter = 'e';
		table_search[74].letter = 'e';	table_search[74].f = &Parser::C1_tree;
		//while
		table_search[75].letter = 'h';
		table_search[76].letter = 'i';
		table_search[77].letter = 'l';
		table_search[78].letter = 'e';	table_search[78].f = &Parser::C1_while;
	}

	void printLists() {
		cout << "Const table\nAdress\t\t\tName" << endl;
		for (auto i : table_const) {
			cout << i << "\t\t";
			if (i->getType() == 0) {
				cout << *(int*)(i->getValue()) << endl;
			}
			else {
				cout << *(Tree*)(i->getValue()) << endl;
			}
		}

		cout << endl << "Variable table\nAdress\t\t\tName" << endl;
		for (auto i : table_name)
			cout << i << "\t\t" << i->getName() << endl;

		cout << endl << "Label table\nAdress\t\t\tName" << endl;
		for (auto i : table_mark)
			cout << i << "\t\t" << *i << endl;


		cout << endl << "Lexeme list\nLine\tClass\t\t\tAdress\t\t\tValue" << endl;
		for (auto i : list_lexeme)
			cout << i << endl;

		cout << endl;
	}

	void parse(const char* filename) {
		ifstream in(filename);
		if (!in)
		{
			cout << "Open Error " << filename << endl;
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
		in.close();
	}
};
int main(int argc, char* argv[]) {
	if (argc != 2)
	{
		cout << "File Error";
		return 1;
	}

	Parser p;
	p.parse(argv[1]);

	return 0;
}
