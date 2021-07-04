#include <iostream>
#include "Tree.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stack>
#include <list>
#include <vector>
#include <ctime>
#include <set>
using namespace std;

enum class LexemeTokenClass { AT, COMPOSITION, FIND, INVERT, MERGE, ARROW_LEFT, ARROW_RIGHT, INT, TREE, CONST, VARIABLE, ARITHMET, RELATION, ASSIGNMENT, DECLARE, SET, WHILE, DO, FOR, TO, IF, THEN, ELSE, CIN, COUT, MARK, JUMP, SWITCH, CASE, DEFAULT, BAD, BEGIN, END, COMMENT, SEMICOLON, COMMA, OPEN_BRACKET, CLOSED_BRACKET, COLON, ERROR, ENDMARK, END_OF_FILE };
enum class SymbolicTokenClass { Letter, Digit, Arithmet, Relation, Space, LF, Semicolon, Colon, OpenCurveBracket, ClosedCurveBracket, OpenBracket, ClosedBracket, Comma, Error, EndOfFile };
enum class State { A1, B1, C1, C2, C3, C4, D1, E1, E2, E3, F1, F2, F3, G1, G2, G3, G4, H1, I1, I2, J1, K1, K2, K3, K4, Exit };

const string LexemeTokenClassName[50] = { "AT", "COMPOSITION", "FIND", "INVERT", "MERGE","ARROW_LEFT", "ARROW_RIGHT", "INT", "TREE", "CONST", "VARIABLE", "ARITHMET", "RELATION", "ASSIGNMENT", "DECLARE", "SET", "WHILE", "DO", "FOR", "TO", "IF", "THEN", "ELSE", "CIN", "COUT", "MARK", "JUMP", "SWITCH", "CASE", "DEFAULT", "BAD", "BEGIN", "END", "COMMENT", "SEMICOLON", "COMMA", "OPEN_BRACKET", "CLOSED_BRACKET", "COLON","ERROR", "ENDMARK", "END_OF_FILE" };
const string keywords[50] = { "int" , "tree", "while", "for", "declare", "set", "to", "do", "if", "then", "else", "cin", "cout", "jump", "switch", "case", "default", "end", "bad", "begin" };

typedef LexemeTokenClass Lexeme;
typedef SymbolicTokenClass Symbol;


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


struct SymbolicToken
{
	Symbol symbol;
	int value;
};

struct LexemeToken
{
	Lexeme lexeme;
	int line;
	void* value;
	friend ostream& operator<<(ostream& os, const LexemeToken& lex)
	{
		os << "  " << lex.line << "\t" << LexemeTokenClassName[(int)lex.lexeme];
		if (lex.lexeme == Lexeme::COMMENT) {
			os << '\t' << *(string*)lex.value;
		}
		return os;
	}
};

class Parser {
private:
	vector<Variable*> constTable;
	vector<Variable*> nameTable;
	vector<string> markTable;
	vector<LexemeToken> lexemeList;

	SymbolicToken s;

	typedef State(Parser::* function_pointer)();
	map<State, map<Symbol, function_pointer>> automata;

	LexemeTokenClass lexeme_class;
	void* ptr;
	int num;
	int rel;
	int pos;
	int value;
	int line = 1;
	bool flag_const;
	string name;
	string tree;
	string comment;

	struct SearchTableClass
	{
		char letter;
		int alt;
		function_pointer f;
	};

	SearchTableClass table_search[400];

	int init_vector[26] = { 1, 2, 8, 25, 37, 42, 0, 0, 47, 54, 0, 0, 57, 0, 0, 0, 0, 0, 61, 68, 0, 0, 75, 0, 0, 0 };

	static SymbolicToken transliterator(int ch)
	{
		SymbolicToken s;
		s.value = 0;
		if (isdigit(ch))
		{
			s.symbol = Symbol::Digit;
			s.value = ch - '0';
		}
		else if (isalpha(ch))
		{
			s.symbol = Symbol::Letter;
			s.value = ch;
		}
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
		{
			s.symbol = Symbol::Arithmet;
			s.value = ch;
		}
		else if (ch == '=' || ch == '!' || ch == '>' || ch == '<')
		{
			s.symbol = Symbol::Relation;
			s.value = ch;
		}
		else if (ch == ' ' || ch == '\t')
		{
			s.symbol = Symbol::Space;
			s.value = ch;
		}
		else if (ch == '\n')
		{
			s.symbol = Symbol::LF;
			s.value = ch;
		}
		else if (ch == ':')
		{
			s.symbol = Symbol::Colon;
			s.value = ch;
		}
		else if (ch == ';')
		{
			s.symbol = Symbol::Semicolon;
			s.value = ch;
		}
		else if (ch == '{')
		{
			s.symbol = Symbol::OpenCurveBracket;
			s.value = ch;
		}
		else if (ch == '}')
		{
			s.symbol = Symbol::ClosedCurveBracket;
			s.value = ch;
		}
		else if (ch == '(')
		{
			s.symbol = Symbol::OpenBracket;
			s.value = ch;
		}
		else if (ch == ')')
		{
			s.symbol = Symbol::ClosedBracket;
			s.value = ch;
		}
		else if (ch == ',')
		{
			s.symbol = Symbol::Comma;
			s.value = ch;
		}
		else if (ch == EOF)
		{
			s.symbol = Symbol::EndOfFile;
			s.value = ch;
		}
		else
		{
			s.symbol = Symbol::Error;
		}
		return s;
	}

	void CreateLexeme()
	{
		LexemeToken lexeme_token;
		lexeme_token.lexeme = lexeme_class;
		lexeme_token.line = line;

		if (lexeme_class == Lexeme::ARITHMET || lexeme_class == Lexeme::COLON || lexeme_class == Lexeme::SEMICOLON || lexeme_class == Lexeme::COMMA || lexeme_class == Lexeme::OPEN_BRACKET || lexeme_class == Lexeme::CLOSED_BRACKET) {
			int* a = new int(value);
			lexeme_token.value = a;
		}
		else if (lexeme_class == Lexeme::RELATION)
		{
			int* token_rel = new int;
			*token_rel = rel;
			lexeme_token.value = token_rel;
		}
		else if (lexeme_class == Lexeme::COMMENT) {
			lexeme_token.value = new string(comment);
		}

		lexemeList.push_back(lexeme_token);

	}
	int AddVariable()
	{
		//for (string s : keywords)
		//{
		//	if (s == name) return Err();
		//}
		for (Variable* i : nameTable)
		{
			if (name == i->getName())
			{
				ptr = i;
				return 0;
			}
		}
		nameTable.push_back(new Variable(name, 0, 0));
		ptr = nameTable[nameTable.size() - 1];
		return 0;
	}

	void AddConst()
	{
		if (tree.empty())
		{
			for (Variable* i : constTable)
			{
				if (i->getType() != 0) continue;
				if (num == *(int*)(i->getValue()))
				{
					ptr = i;
					return;
				}
			}
			constTable.push_back(new Variable("", num, 0));
			ptr = constTable[constTable.size() - 1];
		}
		else
		{
			Tree h(tree);
			tree.clear();
			for (Variable* i : constTable)
			{
				if (i->getType() != 1) continue;
				if ((*(Tree*)(i->getValue())).getRoot())
					if (h.equal(*(Tree*)(i->getValue())))
					{
						ptr = i;
						return;
					}
			}
			constTable.push_back(new Variable("", h, 1));
			ptr = constTable[constTable.size() - 1];
		}
	}

	void AddMark() {
		/*for (string s : keywords)
		{
			if (s == name) return Err();
		}*/
		for (string i : markTable)
		{
			if (name == i)
			{
				return;
			}
		}
		markTable.push_back(name);
		return;
	}

	State A1()
	{
		return State::A1;
	}

	State B1()
	{
		return State::B1;
	}

	State C1()
	{
		return State::C1;
	}
	State C2()
	{
		return State::C1;
	}
	State C3()
	{
		return State::C3;
	}
	State C4()
	{
		return State::C4;
	}

	State D1()
	{
		return State::D1;
	}

	State E1()
	{
		return State::E1;
	}

	State E2()
	{
		return State::E2;
	}

	State E3()
	{
		return State::E3;
	}
	State F1()
	{
		return State::F1;
	}
	State F2()
	{
		return State::F2;
	}
	State F3()
	{
		return State::F3;
	}

	State G1()
	{
		return State::G1;
	}
	State G2()
	{
		return State::G2;
	}
	State G3()
	{
		return State::G3;
	}
	State G4()
	{
		return State::G4;
	}

	State H1()
	{
		return State::H1;
	}

	State I1()
	{
		return State::I1;
	}

	State I2()
	{
		return State::I2;
	}

	State J1()
	{
		return State::J1;
	}
	State A1a()
	{
		line++;
		return State::A1;
	}
	State A1b()
	{
		CreateLexeme();
		line++;
		return State::A1;
	}
	State A1c() {
		CreateLexeme();
		return State::A1;
	}
	State A1d()
	{
		if (rel == '!') return Err();
		else if (rel == '<')
		{
			rel = 3;
		}
		else if (rel == '>')
		{
			rel = 4;
		}
		lexeme_class = Lexeme::RELATION;
		if (rel == '=') {
			lexeme_class = Lexeme::ASSIGNMENT;
		}
		CreateLexeme();
		return State::A1;
	}
	State A1e()
	{
		if (rel == '!') return Err();
		else if (rel == '<')
		{
			rel = 3;
		}
		else if (rel == '>')
		{
			rel = 4;
		}
		lexeme_class = Lexeme::RELATION;
		if (rel == '=') {
			lexeme_class = Lexeme::ASSIGNMENT;
		}
		CreateLexeme();
		line++;
		return State::A1;
	}
	State A1_const()
	{
		AddConst();
		CreateLexeme();
		return State::A1;
	}
	State A1_constP()
	{
		AddConst();
		CreateLexeme();
		line++;
		return State::A1;
	}
	State A1_mark()
	{
		AddMark();
		CreateLexeme();
		return State::A1;
	}
	State A1_markP()
	{
		AddMark();
		CreateLexeme();
		line++;
		return State::A1;
	}
	State A1_var()
	{
		AddVariable();
		CreateLexeme();
		return State::A1;
	}
	State A1_varP()
	{
		AddVariable();
		CreateLexeme();
		line++;
		return State::A1;
	}
	State B1a()
	{
		pos = init_vector[s.value - 'a'];
		if (pos == 0)
		{
			return F1a();
		}
		lexeme_class = Lexeme::VARIABLE;
		if (!isalpha(s.value)) s.value += '0';
		name = (char)s.value;
		return State::B1;
	}
	State B1b()
	{
		pos++;
		return State::B1;
	}
	State C1c()
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
			return Err();
		}
		lexeme_class = Lexeme::RELATION;
		return State::C1;
	}
	State C1_constTree() {
		lexeme_class = Lexeme::CONST;
		tree = tree + (char)s.value;
		AddConst();
		return State::C1;
	}
	State C1_semicolon() {
		lexeme_class = Lexeme::SEMICOLON;
		value = s.value;
		return State::C1;
	}
	State C1_openBracket() {
		lexeme_class = Lexeme::OPEN_BRACKET;
		value = s.value;
		return State::C1;
	}
	State C1_int() {
		lexeme_class = Lexeme::INT;
		return State::C1;
	}
	State C1_at() {
		lexeme_class = Lexeme::AT;
		return State::C1;
	}
	State C1_composition() {
		lexeme_class = Lexeme::COMPOSITION;
		return State::C1;
	}
	State C1_find() {
		lexeme_class = Lexeme::FIND;
		return State::C1;
	}
	State C1_invert() {
		lexeme_class = Lexeme::INVERT;
		return State::C1;
	}
	State C1_merge() {
		lexeme_class = Lexeme::MERGE;
		return State::C1;
	}
	State C1_closedBracekt() {
		lexeme_class = Lexeme::CLOSED_BRACKET;
		value = s.value;
		return State::C1;
	}
	State C1_comma() {
		lexeme_class = Lexeme::COMMA;
		value = s.value;
		return State::C1;
	}
	State C1_bad() {
		lexeme_class = Lexeme::BAD;
		return State::C1;
	}
	State C1_begin() {
		lexeme_class = Lexeme::BEGIN;
		return State::C1;
	}
	State C1_case() {
		lexeme_class = Lexeme::CASE;
		return State::C1;
	}
	State C1_cin() {
		lexeme_class = Lexeme::CIN;
		return State::C1;
	}
	State C1_cout() {
		lexeme_class = Lexeme::COUT;
		return State::C1;
	}
	State C1_declare() {
		lexeme_class = Lexeme::DECLARE;
		return State::C1;
	}
	State C1_default() {
		lexeme_class = Lexeme::DEFAULT;
		return State::C1;
	}
	State C1_do() {
		lexeme_class = Lexeme::DO;
		return State::C1;
	}
	State C1_else() {
		lexeme_class = Lexeme::ELSE;
		return State::C1;
	}
	State C1_end() {
		lexeme_class = Lexeme::END;
		return State::C1;
	}
	State C1_for() {
		lexeme_class = Lexeme::FOR;
		return State::C1;
	}
	State C1_if() {
		lexeme_class = Lexeme::IF;
		return State::C1;
	}
	State C1_tree() {
		lexeme_class = Lexeme::TREE;
		return State::C1;
	}
	State C1_set() {
		lexeme_class = Lexeme::SET;
		return State::C1;
	}
	State C1_switch() {
		lexeme_class = Lexeme::SWITCH;
		return State::C1;
	}
	State C1_then() {
		lexeme_class = Lexeme::THEN;
		return State::C1;
	}
	State C1_to() {
		lexeme_class = Lexeme::TO;
		return State::C1;
	}
	State C1_while() {
		lexeme_class = Lexeme::WHILE;
		return State::C1;
	}
	State C1_ArrowL() {
		if (s.value == '-' && rel == '<') {
			lexeme_class = Lexeme::ARROW_LEFT;
		}
		else {
			return Err();
		}
		return State::C1;
	}
	State C1_ArrowR() {
		if (value == '-' && s.value == '>') {
			lexeme_class = Lexeme::ARROW_RIGHT;
		}
		else {
			return Err();
		}
		return State::C1;
	}
	State C2a() {
		lexeme_class = Lexeme::ARITHMET;
		value = s.value;
		return State::C2;
	}
	State C3_jump() {
		lexeme_class = Lexeme::JUMP;
		name.clear();
		return State::C3;
	}
	State C4a() {
		if (!isalpha(s.value)) s.value += '0';
		name += (char)s.value;
		return State::C4;
	}
	State D1a() {
		lexeme_class = Lexeme::RELATION;
		rel = s.value;
		return State::D1;
	}
	State E1a() {
		lexeme_class = Lexeme::COLON;
		value = s.value;
		return E1();
	}
	State E2a() {
		lexeme_class = Lexeme::MARK;
		if (!isalpha(s.value)) s.value += '0';
		name = (char)s.value;
		return State::E2;
	}
	State E2b() {
		if (!isalpha(s.value)) s.value += '0';
		name += (char)s.value;
		return State::E2;
	}
	State F1a() {
		lexeme_class = Lexeme::VARIABLE;
		if (!isalpha(s.value)) s.value += '0';
		name = (char)s.value;
		return State::F1;
	}
	State F1b() {
		lexeme_class = Lexeme::VARIABLE;
		if (!isalpha(s.value)) s.value += '0';
		name += (char)s.value;
		return State::F1;
	}
	State G1a() {
		tree.clear();
		lexeme_class = Lexeme::CONST;
		num = s.value;
		return State::G1;
	}
	State G1b()
	{
		num = 10 * num + s.value;
		return State::G1;
	}
	State G2a() {
		rel = 0;
		tree = (char)s.value;
		return State::G2;
	}
	State G2b() {
		tree = tree + (char)(s.value + '0');
		return State::G2;
	}
	State G3a()
	{
		if (s.value != '>') return Err();
		tree = tree + (char)s.value;
		return State::G3;
	}
	State G3b() {
		tree = tree + (char)(s.value + '0');
		return State::G3;
	}
	State G4a() {
		tree = tree + (char)s.value;
		return State::G4;
	}
	State I1a() {
		if (value == '-' && s.value == '-') {
			lexeme_class = Lexeme::COMMENT;
			comment = "";
			return State::I1;
		}
		else {
			return Err();
		}
	}
	State I1b() {
		if (s.symbol == Symbol::Digit)
		{
			comment = comment + (char)(s.value + '0');
		}
		else {
			comment = comment + (char)s.value;
		}
		return State::I1;
	}
	State M1()
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



	State Err() {
		lexeme_class = Lexeme::ERROR;
		CreateLexeme();
		cout << "Error line: " << line << endl;
		return State::J1;
	}
	State Exit1()
	{
		lexeme_class = Lexeme::ENDMARK;
		CreateLexeme();
		return State::Exit;
	}
	State Exit2()
	{
		CreateLexeme();
		lexeme_class = Lexeme::ENDMARK;
		CreateLexeme();
		return State::Exit;
	}
	State Exit3()
	{
		AddConst();
		CreateLexeme();
		lexeme_class = Lexeme::ENDMARK;
		CreateLexeme();
		return State::Exit;
	}
	State Exit4()
	{
		AddVariable();
		CreateLexeme();
		lexeme_class = Lexeme::ENDMARK;
		CreateLexeme();
		return State::Exit;
	}
	State Exit5()
	{
		AddMark();
		CreateLexeme();
		lexeme_class = Lexeme::ENDMARK;
		CreateLexeme();
		return State::Exit;
	}
public:
	Parser()
	{

		automata = {
			{State::A1,	{{ Symbol::Letter, &Parser::B1a},	{ Symbol::Digit, &Parser::G1a},	{ Symbol::Arithmet, &Parser::C2a},			{ Symbol::Relation, &Parser::D1a},			{ Symbol::Space, &Parser::A1},			{ Symbol::LF, &Parser::A1a},		{ Symbol::Semicolon, &Parser::C1_semicolon},	{ Symbol::Colon, &Parser::E1a},	{ Symbol::OpenCurveBracket, &Parser::G2a},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::C1_openBracket},	{ Symbol::ClosedBracket, &Parser::C1_closedBracekt},	{ Symbol::Comma, &Parser::C1_comma},	{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit1}}},
			{State::B1,	{{ Symbol::Letter, &Parser::M1},	{ Symbol::Digit, &Parser::Err},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::A1_var},		{ Symbol::LF, &Parser::A1_varP},	{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit4}}},
			{State::C1,	{{ Symbol::Letter, &Parser::F1b},	{ Symbol::Digit, &Parser::F1b},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::A1c},			{ Symbol::LF, &Parser::A1b},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit2}}},
			{State::C2,	{{ Symbol::Letter, &Parser::Err},	{ Symbol::Digit, &Parser::Err},	{ Symbol::Arithmet, &Parser::I1a},			{ Symbol::Relation, &Parser::C1_ArrowR},	{ Symbol::Space, &Parser::A1c},			{ Symbol::LF, &Parser::A1b},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit2}}},
			{State::C3,	{{ Symbol::Letter, &Parser::C4a},	{ Symbol::Digit, &Parser::Err},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::C3},			{ Symbol::LF, &Parser::Err},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Err}}},
			{State::C4,	{{ Symbol::Letter, &Parser::C4a},	{ Symbol::Digit, &Parser::C4a},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::A1c},			{ Symbol::LF, &Parser::A1b},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit2}}},
			{State::D1,	{{ Symbol::Letter, &Parser::Err},	{ Symbol::Digit, &Parser::Err},	{ Symbol::Arithmet, &Parser::C1_ArrowL},	{ Symbol::Relation, &Parser::C1c},			{ Symbol::Space, &Parser::A1d},			{ Symbol::LF, &Parser::A1e},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit2}}},
			{State::E1,	{{ Symbol::Letter, &Parser::E2a},	{ Symbol::Digit, &Parser::Err},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::A1c},			{ Symbol::LF, &Parser::A1b},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit2}}},
			{State::E2,	{{ Symbol::Letter, &Parser::E2b},	{ Symbol::Digit, &Parser::E2b},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::A1_mark},		{ Symbol::LF, &Parser::A1_markP},	{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit5}}},
			{State::F1,	{{ Symbol::Letter, &Parser::F1b},	{ Symbol::Digit, &Parser::F1b},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::A1_var},		{ Symbol::LF, &Parser::A1_varP},	{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit4}}},
			{State::G1,	{{ Symbol::Letter, &Parser::Err},	{ Symbol::Digit, &Parser::G1b},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::A1_const},	{ Symbol::LF, &Parser::A1_constP},	{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Exit3}}},
			{State::G2,	{{ Symbol::Letter, &Parser::Err},	{ Symbol::Digit, &Parser::G2b},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::G3a},			{ Symbol::Space, &Parser::Err},			{ Symbol::LF, &Parser::Err},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Err}}},
			{State::G3,	{{ Symbol::Letter, &Parser::Err},	{ Symbol::Digit, &Parser::G3b},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::Err},			{ Symbol::LF, &Parser::Err},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::C1_constTree},	{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::G4a},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Err}}},
			{State::G4,	{{ Symbol::Letter, &Parser::Err},	{ Symbol::Digit, &Parser::G2b},	{ Symbol::Arithmet, &Parser::Err},			{ Symbol::Relation, &Parser::Err},			{ Symbol::Space, &Parser::G4},			{ Symbol::LF, &Parser::Err},		{ Symbol::Semicolon, &Parser::Err},				{ Symbol::Colon, &Parser::Err},	{ Symbol::OpenCurveBracket, &Parser::Err},	{ Symbol::ClosedCurveBracket, &Parser::Err},			{ Symbol::OpenBracket, &Parser::Err},				{ Symbol::ClosedBracket, &Parser::Err},					{ Symbol::Comma, &Parser::Err},			{ Symbol::Error, &Parser::Err},	{ Symbol::EndOfFile, &Parser::Err}}},
			{State::I1,	{{ Symbol::Letter, &Parser::I1b},	{ Symbol::Digit, &Parser::I1b},	{ Symbol::Arithmet, &Parser::I1b},			{ Symbol::Relation, &Parser::I1b},			{ Symbol::Space, &Parser::I1b},			{ Symbol::LF, &Parser::A1b},		{ Symbol::Semicolon, &Parser::I1b},				{ Symbol::Colon, &Parser::I1b},	{ Symbol::OpenCurveBracket, &Parser::I1b},	{ Symbol::ClosedCurveBracket, &Parser::I1b},			{ Symbol::OpenBracket, &Parser::I1b},				{ Symbol::ClosedBracket, &Parser::I1b},					{ Symbol::Comma, &Parser::I1b},			{ Symbol::Error, &Parser::I1b},	{ Symbol::EndOfFile, &Parser::Exit2}}},
			{State::J1,	{{ Symbol::Letter, &Parser::J1},	{ Symbol::Digit, &Parser::J1},	{ Symbol::Arithmet, &Parser::J1},			{ Symbol::Relation, &Parser::J1},			{ Symbol::Space, &Parser::J1},			{ Symbol::LF, &Parser::A1a},		{ Symbol::Semicolon, &Parser::J1},				{ Symbol::Colon, &Parser::J1},	{ Symbol::OpenCurveBracket, &Parser::J1},	{ Symbol::ClosedCurveBracket, &Parser::J1},				{ Symbol::OpenBracket, &Parser::J1},				{ Symbol::ClosedBracket, &Parser::J1},					{ Symbol::Comma, &Parser::J1},			{ Symbol::Error, &Parser::J1},	{ Symbol::EndOfFile, &Parser::Exit1}}},
		};

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
		cout << "table_const\nAdress\t\t\tName" << endl;
		for (auto i : constTable) {
			cout << i << "\t\t";
			if (i->getType() == 0) {
				cout << *(int*)(i->getValue()) << endl;
			}
			else {
				cout << *(Tree*)(i->getValue()) << endl;
			}
		}

		cout << endl << "Variable table\nAdress\t\t\tName" << endl;
		for (auto i : nameTable)
			cout << i << "\t\t" << i->getName() << endl;

		cout << endl << "Label table\nAdress\t\t\tName" << endl;
		for (auto i : markTable)
			cout << &i << "\t\t" << i << endl;


		cout << endl << "Lexeme list\nLine\tClass" << endl;
		for (int i = 0; i < lexemeList.size(); i++) {
			cout << lexemeList[i] << endl;
		}
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
		State state = State::A1;
		while (state != State::Exit)
		{
			ch = in.get();
			s = transliterator(ch);
			state = (this->*automata[state][s.symbol])();
		}
		printLists();
		in.close();
	}

	vector<LexemeToken> getLexemeList() {
		return lexemeList;
	}
};

class SyntaxAnalyzer
{
private:
	class Production {
	public:
		string name;
		Production() : name("") {}
		Production(string n) : name(n) {}
		string getName() const {
			return name;
		}
		bool operator<(Production* p) const {
			return name < p->name;
		}
		bool operator==(const Production* p) const {
			return name == p->name;
		}
		virtual int getType() = 0;
		friend ostream& operator<<(ostream& os, const Production* p) {
			return os << p->name;
		}
	};
	class Terminal : virtual public Production {
	public:
		Terminal() : Production() {}
		Terminal(string n) : Production(n) {}
		bool operator<(const Terminal& t) const {
			return name < t.name;
		}
		bool operator==(const Terminal& t) const {
			return name == t.name;
		}
		bool operator==(const Production* p) const {
			return name == p->name;
		}
		int getType() {
			return 0;
		}
		friend ostream& operator<<(ostream& os, const Terminal& t) {
			return os << t.getName();
		}
	};
	class NonTerminal : virtual public Production {
	public:
		NonTerminal() : Production() {}
		NonTerminal(string n) : Production(n) {}
		bool operator<(const NonTerminal& nt) const {
			return name < nt.name;
		}
		bool operator==(const NonTerminal& nt) const {
			return name == nt.name;
		}
		bool operator==(const Production* p) const {
			return name == p->name;
		}
		int getType() {
			return 1;
		}
		friend ostream& operator<<(ostream& os, const NonTerminal& nt) {
			return os << nt.getName();
		}
	};
	class Item {
	private:
		NonTerminal nonTerminal;
		vector<Production*> production;
		Terminal lookahead;
		int position;
	public:
		Item() : position(0) {}
		Item(NonTerminal nt, vector<Production*> prod, Terminal la = Terminal("$"), int pos = 0)
			: nonTerminal(nt), production(prod), lookahead(la), position(pos) {}
		bool operator==(const Item& item) const {
			return nonTerminal == item.nonTerminal && production == item.production && lookahead == item.lookahead && position == item.position;
		}
		bool operator<(const Item& item) const {
			return nonTerminal < item.nonTerminal&& production < item.production&& lookahead < item.lookahead&& position < item.position;
		}
		void incrPosition() {
			position++;
		}
		NonTerminal getNonTerminal() {
			return nonTerminal;
		}
		vector<Production*> getProduction() {
			return production;
		}
		Terminal getLookahead() {
			return lookahead;
		}
		int getPosition() {
			return position;
		}
		friend ostream& operator<<(ostream& os, Item& item) {
			os << item.nonTerminal << "->";
			for (int i = 0; i < item.production.size(); i++) {
				if (i == item.getPosition()) {
					os << ".";
				}
				os << item.production[i];
			}
			return os << "; " << item.lookahead;
		}
	};

	vector<Terminal> FIRST(Item item) {
		vector<Terminal> firstList;
		stack<NonTerminal> st;
		Production* f = item.getProduction()[0];
		if (f->getType() == 0) {
			firstList.push_back(Terminal(f->getName()));
			return firstList;
		}
		st.push(NonTerminal(f->getName()));

		while (!st.empty()) {
			NonTerminal tmp = st.top();
			st.pop();
			for (int i = 0; i < rules.size(); i++) {
				if (tmp == rules[i].getNonTerminal()) {
					Production* p = rules[i].getProduction()[0];
					if (p->getType() == 0) {
						firstList.push_back(Terminal(p->getName()));
					}
					else if (p->getName() == tmp.getName()) {
						continue;
					}
					else {
						st.push(NonTerminal(p->getName()));
					}

				}
			}
		}
		return firstList;
	}

	map<NonTerminal, set<Terminal>> firstAll;
	void setFirstAll() {
		for (auto r : rules) {
			for (auto p : FIRST(r)) {
				firstAll[r.getNonTerminal()].insert(p);
			}
		}
	}

	vector<Item> rules;
	vector<vector<Item>> tables;
	vector<vector<Item>> GOTOAll;
	vector<vector<Item>> CLOSUREAll;
	vector<Terminal>  termList;
	map<Production*, map<vector<Item>, int>> tableForAction;

	bool checkTable(vector<Item> v, Item item) {
		for (int i = 0; i < v.size(); i++) {
			if (v[i] == item) {
				return 0;
			}
		}
		return 1;
	}

	void optCLOSURE(vector<Item> v) {
		vector<Item> closureV(v);
		for (int i = 0; i < closureV.size(); i++) {
			for (int j = 0; j < rules.size(); j++) {
				vector<Terminal> firstList;
				if (closureV[i].getPosition() < closureV[i].getProduction().size()) {
					if (rules[j].getNonTerminal() == closureV[i].getProduction()[closureV[i].getPosition()]) {
						if (closureV[i].getPosition() + 1 < closureV[i].getProduction().size()) {
							if (closureV[i].getProduction()[closureV[i].getPosition() + 1]->getType() == 0) {
								firstList.push_back(Terminal(closureV[i].getProduction()[closureV[i].getPosition() + 1]->getName()));
							}
							else {
								NonTerminal nt(closureV[i].getProduction()[closureV[i].getPosition() + 1]->getName());
								for (auto d : firstAll[nt]) {
									firstList.push_back(d);
								}
							}
							for (int k = 0; k < firstList.size(); k++) {
								Item t(rules[j].getNonTerminal(), rules[j].getProduction(), firstList[k]);
								if (checkTable(closureV, t)) {
									closureV.push_back(t);
								}
							}
						}
						else {
							Item t(rules[j].getNonTerminal(), rules[j].getProduction(), closureV[i].getLookahead());
							if (checkTable(closureV, t)) {
								closureV.push_back(t);
							}
						}
					}
				}
			}
		}
		CLOSUREAll.push_back(closureV);

	}

	int optGOTO(vector<Item> v, Production* p, bool calcClosure) {
		vector<Item> res;
		for (int i = 0; i < v.size(); i++) {
			if (v[i].getPosition() < v[i].getProduction().size()) {
				if (v[i].getProduction()[v[i].getPosition()]->getName() == p->getName()) {
					Item tmp(v[i]);
					tmp.incrPosition();
					res.push_back(tmp);
				}
			}
		}
		if (!res.empty()) {
			for (int i = 0; i < GOTOAll.size(); i++) {
				if (GOTOAll[i] == res) return i;
			}
			if (calcClosure) {
				GOTOAll.push_back(res);
				optCLOSURE(res);
			}
		}
		return -1;
	}

	void optItems() {
		GOTOAll.push_back({ rules.front() });
		optCLOSURE({ rules.front() });

		for (int i = 0; i < CLOSUREAll.size(); i++) {
			for (int j = 0; j < rules.size(); j++) {
				optGOTO(CLOSUREAll[i], new NonTerminal(rules[j].getNonTerminal()), true);
			}
			for (int j = 0; j < termList.size(); j++) {
				optGOTO(CLOSUREAll[i], &termList[j], true);
			}

		}
	}

	struct Act {
		string action;
		int rule;
		Act() : action(""), rule(-1) {}
		Act(string act, int r = -1) : action(act), rule(r) {}
		bool operator==(const Act& a) const {
			return action == a.action && rule == a.rule;
		}
		friend ostream& operator<<(ostream& os, Act& a) {
			return os << a.action << a.rule;
		}
	};

	map<int, map<Terminal, Act>> actionTable;

	int findTable(vector<Item> v) {
		for (int i = 0; i < tables.size(); i++) {
			if (tables[i].size() != v.size()) continue;
			if (tables[i] == v) return i;
		}
		return -1;
	}

	int getRule(Item nt) {
		for (int i = 0; i < rules.size(); i++) {
			if (rules[i].getNonTerminal() == nt.getNonTerminal() && rules[i].getProduction() == nt.getProduction())
				return i;
		}
	}

	void optGetActionTable() {
		for (int i = 0; i < CLOSUREAll.size(); i++) {
			clock_t a, b, c, d;
			a = clock();
			for (int j = 0; j < CLOSUREAll[i].size(); j++) {
				if (CLOSUREAll[i][j].getPosition() < CLOSUREAll[i][j].getProduction().size()) {
					bool flag = 0;
					c = clock();
					for (Terminal term : termList) {
						int point = optGOTO(CLOSUREAll[i], &term, false);
						if (point != -1) {
							if (actionTable[i][term] == Act() || actionTable[i][term] == Act("s", point)) {
								actionTable[i][term] = Act("s", point);
							}
							else {
								cout << "CONFLICT:" << "I" << i << ", " << term << endl;
							}
						}
					}
					d = clock();
					cout << "I" << i << " " << j << " " << (d - c) << "ms" << endl;
				}
				else if (CLOSUREAll[i][j].getPosition() == CLOSUREAll[i][j].getProduction().size()) {
					int rule = getRule(CLOSUREAll[i][j]);
					if (actionTable[i][CLOSUREAll[i][j].getLookahead()] == Act() || actionTable[i][CLOSUREAll[i][j].getLookahead()] == Act("r", rule))
						actionTable[i][CLOSUREAll[i][j].getLookahead()] = Act("r", rule);
					else {
						cout << "GOTO CONFLICT:" << "I" << i << endl;
						cout << "ATTEMPT TO ASSIGN RULE " << rule;
					}
				}
			}
			b = clock();
			cout << endl << "T" << i << " " << (b - a) / 1000 << "s" << endl << endl;
		}
	}

	struct Step {
		int rule;
		Step() : rule(-1) {}
		Step(int r) : rule(r) {}
		bool operator==(const Step& s) const {
			return rule == s.rule;
		}
		friend ostream& operator<<(ostream& os, const Step& s) {
			return os << s.rule;
		}
	};

	map<int, map<NonTerminal, Step>> gotoTable;
	set<NonTerminal> nonTermList;

	void optGetGotoTable() {
		for (int i = 0; i < CLOSUREAll.size(); i++) {
			for (int j = 0; j < CLOSUREAll[i].size(); j++) {
				if (CLOSUREAll[i][j].getPosition() < CLOSUREAll[i][j].getProduction().size()) {
					for (NonTerminal nt : nonTermList) {
						int point = optGOTO(CLOSUREAll[i], &nt, false);
						if (point != -1) {
							if (gotoTable[i][nt] == Step() || gotoTable[i][nt] == Step(point)) {
								gotoTable[i][nt] = Step(point);
							}
							else {
								cout << "CONFLICT" << endl;
							}
						}
					}
					break;
				}
			}
		}
	}

	void getCanonicalTable() {

		clock_t a, b;

		a = clock();
		optItems();
		b = clock();

		cout << endl << "TABLES COMPLETE " << (b - a) / 1000 << "s." << endl;
		cout << GOTOAll.size() << " tables" << endl;

		a = clock();
		optGetActionTable();
		b = clock();

		for (auto t : tableForAction) {
			cout << t.first << endl;
			for (auto r : t.second) {
				cout << "GOTO" << endl;
				for (auto g : r.first) {
					cout << g << endl;
				}
				cout << "ENDGOTO" << endl;
				cout << r.second << endl;
			}

			cout << endl << endl;;
		}

		cout << endl << "ACTION TABLE COMPLETE " << (b - a) / 1000 << "s." << endl;

		a = clock();
		optGetGotoTable();
		b = clock();

		cout << endl << "GOTO TABLE COMPLETE " << (b - a) / 1000 << "s." << endl;

		cout << endl;
		cout << "---%TABLES%---" << endl;
		int iee = 0;
		for (auto t : tables) {
			cout << "I" << iee++ << endl;
			for (auto r : t) {
				cout << r << endl;
			}
			cout << endl;
		}
		cout << "CANONICAL ACTION TABLE" << endl;

		for (auto t : actionTable) {
			cout << t.first << " ";
			for (auto r : t.second) {
				cout << r.first << "->" << r.second << " ";
			}
			cout << endl;
		}

		cout << endl;
		cout << "CANONICAL GOTO TABLE" << endl;

		for (auto t : gotoTable) {
			cout << t.first << " ";
			for (auto r : t.second) {
				cout << r.first << "->" << r.second << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void analyze(vector<Terminal> buf) {
		list<int> state;

		cout << "input:\t";

		for (auto t : buf) {
			cout << t << " ";
		}
		cout << endl << endl;

		state.push_back(0);
		for (int i = 0; i < buf.size();) {
			if (actionTable[state.back()][buf[i]].action == "s") {
				state.push_back(actionTable[state.back()][buf[i]].rule);
				cout << "shift:\t" << buf[i] << endl;
				i++;
			}
			else if (actionTable[state.back()][buf[i]].action == "r" && actionTable[state.back()][buf[i]].rule != 0) {
				int rule = actionTable[state.back()][buf[i]].rule;
				int count = rules[actionTable[state.back()][buf[i]].rule].getProduction().size();
				NonTerminal nt = rules[actionTable[state.back()][buf[i]].rule].getNonTerminal();
				for (int j = 0; j < count; j++) {
					state.pop_back();
				}
				state.push_back(gotoTable[state.back()][nt].rule);
				cout << "reduce:\t" << rules[rule] << endl;
			}
			else if (actionTable[state.back()][buf[i]].rule == 0) {
				cout << endl << "Success" << endl;
				break;
			}
			else {
				cout << "ERROR " << state.back() << " " << buf[i] << endl;
				exit(-1);
			}
			cout << "Magazine: ";
			for (auto p : state) {
				cout << p << " ";
			}
			cout << endl << endl;
		}
	}

	vector<Terminal> buf;

	bool checkTermVector(Production* t) {
		for (auto p : termList) {
			if (t == &p) return 0;
		}
		return 1;
	}

public:
	SyntaxAnalyzer() {
		rules = {
			Item(NonTerminal("S"), { new NonTerminal("<program>") }),
			Item(NonTerminal("<program>"), { new NonTerminal("<operator>"), new NonTerminal("<program>") }),
			Item(NonTerminal("<program>"), { new NonTerminal("<operator>") }),
			Item(NonTerminal("<operator>"), { new Terminal("DECLARE"), new Terminal("VARIABLE"), new NonTerminal("<type>"), new NonTerminal("<declare_list>"), new Terminal("SEMICOLON")}),
			Item(NonTerminal("<operator>"), { new Terminal("DECLARE"), new Terminal("VARIABLE"), new NonTerminal("<type>"), new Terminal("SEMICOLON")}),
			Item(NonTerminal("<declare_list>"), { new Terminal("COMMA"), new Terminal("VARIABLE"), new NonTerminal("<type>"), new NonTerminal("<declare_list>")}),
			Item(NonTerminal("<declare_list>"), { new Terminal("COMMA"), new Terminal("VARIABLE"), new NonTerminal("<type>")}),
			Item(NonTerminal("<operator>"), { new Terminal("SET"), new Terminal("VARIABLE"), new NonTerminal("<set_list>"), new Terminal("TO") , new NonTerminal("<exp>"), new Terminal("SEMICOLON")}),
			Item(NonTerminal("<operator>"), { new Terminal("SET"), new Terminal("VARIABLE"), new Terminal("TO") , new NonTerminal("<exp>"), new Terminal("SEMICOLON")}),
			Item(NonTerminal("<set_list>"), { new Terminal("COMMA"), new Terminal("VARIABLE"), new NonTerminal("<set_list>")}),
			Item(NonTerminal("<set_list>"), {  new Terminal("COMMA"), new Terminal("VARIABLE")}),
			//Item(NonTerminal("<operator>"), { new Terminal("WHILE"), new NonTerminal("<test>"), new Terminal("DO"), new NonTerminal("<operator>")}),
			//Item(NonTerminal("<test>"), { new NonTerminal("<exp>"), new Terminal("RELATION"), new NonTerminal("<exp>") }),
			//Item(NonTerminal("<operator>"), { new Terminal("FOR"), new Terminal("VARIABLE"), new Terminal("ASSIGNMENT"), new NonTerminal("<exp>"), new Terminal("TO"),  new NonTerminal("<exp>"), new Terminal("DO"), new NonTerminal("<operator>") }),
			//Item(NonTerminal("<operator>"), { new Terminal("IF"), new NonTerminal("<test>"), new Terminal("THEN"), new NonTerminal("<operator>") }),
			//Item(NonTerminal("<operator>"), { new Terminal("IF"), new NonTerminal("<test>"), new Terminal("THEN"), new NonTerminal("<operator>"), new NonTerminal("<else>"), new NonTerminal("<operator>")}),
			//Item(NonTerminal("<operator>"), { new Terminal("BEGIN"),new NonTerminal("<program>"), new Terminal("END")}),
			Item(NonTerminal("<operator>"), { new Terminal("BAD"), new Terminal("SEMICOLON")}),
			Item(NonTerminal("<operator>"), { new Terminal("COMMENT")}),
			//Item(NonTerminal("<operator>"), { new NonTerminal("<cin_list>"), new Terminal("CIN"), new Terminal("SEMICOLON")}),
			//Item(NonTerminal("<cin_list>"), { new Terminal("VARIABLE"), new Terminal("ARROW_LEFT"), new Terminal("<cin_list>")}),
			//Item(NonTerminal("<cin_list>"), { new Terminal("VARIABLE"), new Terminal("ARROW_LEFT")}),
			//Item(NonTerminal("<operator>"), { new NonTerminal("<cout_list>"), new Terminal("COUT"), new Terminal("SEMICOLON")}),
			//Item(NonTerminal("<cout_list>"), { new NonTerminal("<exp>"), new Terminal("ARROW_RIGHT"), new NonTerminal("<cout_list>")}),
			//Item(NonTerminal("<cout_list>"), { new NonTerminal("<exp>"), new Terminal("ARROW_RIGHT")}),
			//Item(NonTerminal("<operator>"), { new Terminal("SWITCH"),  new Terminal("OPEN_BRACKET"), new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET"), new NonTerminal("<case_list>"),  new NonTerminal("<default>"), new Terminal("END"), new Terminal("SWITCH"), new Terminal("SEMICOLON"), }),
			//Item(NonTerminal("<operator>"), { new Terminal("SWITCH"),  new Terminal("OPEN_BRACKET"), new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET"), new NonTerminal("<case_list>"), new Terminal("END"), new Terminal("SWITCH"), new Terminal("SEMICOLON"), }),
			//Item(NonTerminal("<default>"), {new Terminal("DEFAULT"),  new Terminal("COLON"), new NonTerminal("<operator>")}),
			//Item(NonTerminal("<case_list>"), { new Terminal("CASE"), new Terminal("CONST"), new NonTerminal("<const_list>"), new Terminal("COLON"), new NonTerminal("<operator>"), new NonTerminal("<case_list>")}),
			//Item(NonTerminal("<case_list>"), { new Terminal("CASE"), new Terminal("CONST"), new NonTerminal("<const_list>"), new Terminal("COLON"), new NonTerminal("<operator>")}),
			//Item(NonTerminal("<case_list>"), { new Terminal("CASE"), new Terminal("CONST"), new Terminal("COLON"), new NonTerminal("<operator>"), new NonTerminal("<case_list>")}),
			//Item(NonTerminal("<case_list>"), { new Terminal("CASE"), new Terminal("CONST"), new Terminal("COLON"), new NonTerminal("<operator>")}),
			//Item(NonTerminal("<const_list>"), {new NonTerminal("<const_el>"), new NonTerminal("<const_list>")}),
			//Item(NonTerminal("<const_list>"), {new NonTerminal("<const_el>")}),
			//Item(NonTerminal("<const_el>"), { new Terminal("COMMA"), new Terminal("CONST")}),
			//Item(NonTerminal("<operator>"), { new Terminal("SEMICOLON")}),
			//Item(NonTerminal("<operator>"), { new Terminal("MARK")}),
			//Item(NonTerminal("<operator>"), { new Terminal("JUMP"), new Terminal("SEMICOLON")}),
			Item(NonTerminal("<exp>"), { new NonTerminal("<exp>"), new Terminal("ADD"), new NonTerminal("<T>")}),
			Item(NonTerminal("<exp>"), { new NonTerminal("<exp>"), new Terminal("SUBT"), new NonTerminal("<T>")}),
			Item(NonTerminal("<exp>"), { new NonTerminal("<T>") }),
			Item(NonTerminal("<T>"), { new NonTerminal("<T>"), new Terminal("MULT"), new NonTerminal("<F>") }),
			Item(NonTerminal("<T>"), { new NonTerminal("<T>"), new Terminal("DIV"), new NonTerminal("<F>") }),
			Item(NonTerminal("<T>"), { new NonTerminal("<T>"), new Terminal("MOD"), new NonTerminal("<F>") }),
			Item(NonTerminal("<T>"), { new NonTerminal("<F>") }),
			Item(NonTerminal("<F>"), { new Terminal("VARIABLE") }),
			Item(NonTerminal("<F>"), { new Terminal("CONST") }),
			//Item(NonTerminal("<F>"), { new Terminal("FIND"), new Terminal("OPEN_BRACKET"), new NonTerminal("<exp>"), new Terminal("COMMA"), new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET") }),
			//Item(NonTerminal("<F>"), { new Terminal("COMPOSITION"), new Terminal("OPEN_BRACKET"), new NonTerminal("<exp>"), new Terminal("COMMA"), new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET") }),
			//Item(NonTerminal("<F>"), { new Terminal("MERGE"), new Terminal("OPEN_BRACKET"), new NonTerminal("<exp>"), new Terminal("COMMA"), new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET") }),
			//Item(NonTerminal("<F>"), { new Terminal("AT"), new Terminal("OPEN_BRACKET"), new NonTerminal("<exp>"), new Terminal("COMMA"), new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET") }),
			//Item(NonTerminal("<F>"), { new Terminal("INVERT"), new Terminal("OPEN_BRACKET"), new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET") }),
			//Item(NonTerminal("<F>"), { new Terminal("OPEN_BRACKET") , new NonTerminal("<exp>"), new Terminal("CLOSED_BRACKET")  }),
			Item(NonTerminal("<type>"), { new Terminal("INT") }),
			Item(NonTerminal("<type>"), { new Terminal("TREE") }),

			Item(NonTerminal("S'"), { new NonTerminal("S") }),
			Item(NonTerminal("S"), { new NonTerminal("C"), new NonTerminal("C") }),
			Item(NonTerminal("C"), { new Terminal("c"), new NonTerminal("C") }),
			Item(NonTerminal("C"), { new Terminal("d") }),

			/*Item(NonTerminal("E"), {new NonTerminal("E"), new Terminal("ADD"), new NonTerminal("T")} ),
			Item(NonTerminal("E"), {new NonTerminal("T")} ),
			Item(NonTerminal("T"), {new NonTerminal("F")} ),
			Item(NonTerminal("F"), {new Terminal("VARIABLE")} ),
			Item(NonTerminal("F"), {new Terminal("CONST")} ),*/

		};

		cout << "---%RULES%---" << endl;
		for (int i = 0; i < rules.size(); i++) {
			cout << i << " " << rules[i].getNonTerminal() << "->";
			for (auto p : rules[i].getProduction()) {
				cout << p;
			}
			cout << endl;
		}
		cout << endl;

		for (auto r : rules) {
			for (auto p : r.getProduction()) {
				if (p->getType() == 0) {
					if (checkTermVector(p)) termList.push_back(Terminal(p->getName()));
				}
			}
		}

		for (auto r : rules) {
			nonTermList.insert(r.getNonTerminal());
		}

		setFirstAll();
		cout << endl << "ALL FIRST" << endl;
		for (auto t : firstAll) {
			cout << t.first << ": ";
			for (auto r : t.second) {
				cout << r << " ";
			}
			cout << endl;
		}

		//vector<Terminal> buf = { Terminal("c"), Terminal("d"), Terminal("d"), Terminal("$") };
		//analyze(buf);
	}

	void getTable() {
		getCanonicalTable();
	}

	void parse(vector<LexemeToken> lexemeList) {
		vector<Terminal> buf;
		for (auto l : lexemeList) {
			if (l.lexeme == Lexeme::ARITHMET) {
				switch (*(int*)l.value) {
				case 43: buf.push_back(Terminal("ADD")); break;
				case 45: buf.push_back(Terminal("SUBT")); break;
				case 42: buf.push_back(Terminal("MULT")); break;
				case 47: buf.push_back(Terminal("DIV")); break;
				case 37: buf.push_back(Terminal("MOD")); break;
				}
			}
			else if (l.lexeme == Lexeme::ENDMARK) {
				buf.push_back(Terminal("$"));
			}
			else {
				buf.push_back(Terminal(LexemeTokenClassName[(int)l.lexeme]));
			}
		}
		analyze(buf);
	}

	void putTable() {
		ofstream out("canonicalTable.txt");

		for (auto t : actionTable) {
			out << t.first << endl;
			for (auto r : t.second) {
				out << r.first << " " << r.second.action << " " << r.second.rule << " ";
			}
			out << ";" << endl;
		}

		out << "-1" << endl;

		for (auto t : gotoTable) {
			out << t.first << endl;
			for (auto r : t.second) {
				out << r.first << " " << r.second << " ";
			}
			out << ";" << endl;
		}
		out << "-2" << endl;
	}

	void getTableFromFile() {
		ifstream in("canonicalTable.txt");
		int q;
		while (1) {
			in >> q;
			if (q == -1) break;
			string T;
			string A;
			int R;
			while (1) {
				in >> T;
				if (T == ";") break;
				in >> A;
				in >> R;
				actionTable[q][Terminal(T)] = Act(A, R);
			}
		}
		while (1) {
			in >> q;
			if (q == -2) break;
			string T;
			int R;
			while (1) {
				in >> T;
				if (T == ";") break;
				in >> R;
				gotoTable[q][NonTerminal(T)] = Step(R);
			}
		}
	}

	void printTable() {
		cout << "CANONICAL ACTION TABLE" << endl;

		for (auto t : actionTable) {
			cout << t.first << " ";
			for (auto r : t.second) {
				cout << r.first << "->" << r.second << " ";
			}
			cout << endl;
		}

		cout << endl;
		cout << "CANONICAL GOTO TABLE" << endl;

		for (auto t : gotoTable) {
			cout << t.first << " ";
			for (auto r : t.second) {
				cout << r.first << "->" << r.second << " ";
			}
			cout << endl;
		}
		cout << endl;
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

	SyntaxAnalyzer sa;
//	sa.getTable();
//	sa.putTable();
	sa.getTableFromFile();
	sa.printTable();
	sa.parse(p.getLexemeList());

	return 0;
}