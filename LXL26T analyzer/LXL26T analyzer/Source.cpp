#include <iostream>
#include "Tree.h"
#include <vector>
#include <fstream>
using namespace std;

#define REDHB "\033[0;101m"
#define BWHT "\033[1;37m"
#define BLKHB "\033[0;100m"
#define BLUB "\033[44m"
#define reset "\033[0m"

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

enum LexemeTokenClass { ARROW_LEFT, ARROW_RIGHT, INT, TREE, CONST, VARIABLE, AR_OP, RELATION, ASSIGNMENT, DECLARE, SET, WHILE, DO, FOR, TO, IF, THEN, ELSE, CIN, COUT, MARK, JUMP, SWITCH, CASE, DEFAULT, BAD, BEGIN, END, COMMENT, SEMICOLON, COMMA, OPEN_BRACKET, CLOSED_BRACKET, COLON, ERROR, ENDMARK, END_OF_FILE };
enum SymbolicTokenClass { Letter, Digit, Ar_Op, Relation, Space, LF, Semicolon, Colon, OpenCurveBracket, ClosedCurveBracket, OpenBracket, ClosedBracket, Comma, Error, EndOfFile };
enum States {s_A1, s_B1, s_C1, s_C2, s_C3, s_C4, s_D1, s_E1, s_E2, s_E3, s_F1, s_F2, s_F3, s_G1, s_G2, s_G3, s_G4, s_H1, s_I1, s_I2, s_J1, s_K1, s_K2, s_K3, s_K4, s_Exit };

string LexemeTokenClassName[] = { "ARROW_LEFT", "ARROW_RIGHT", "INT", "TREE", "CONST", "VARIABLE", "AR_OP", "RELATION", "ASSIGNMENT", "DECLARE", "SET", "WHILE", "DO", "FOR", "TO", "IF", "THEN", "ELSE", "CIN", "COUT", "MARK", "JUMP", "SWITCH", "CASE", "DEFAULT", "BAD", "BEGIN", "END", "COMMENT", "SEMICOLON", "COMMA", "OPEN_BRACKET", "CLOSED_BRACKET", "COLON","ERROR", "ENDMARK", "END_OF_FILE" };

const string keywords[] = { "int" , "tree", "while", "for", "declare", "set", "to", "do", "if", "then", "else", "cin", "cout", "jump", "switch", "case", "default", "end", "bad", "begin" };

int init_vector[] = { 0, 1, 7, 15, 27, 32, 0, 0, 34, 37, 0, 0, 0, 0, 0, 0, 0, 0, 40, 47, 0, 0, 54, 0, 0, 0 };

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

		return os;
	}
};

vector<Variable*> table_const;
vector<Variable*> table_name;
vector<string> table_mark;
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
		for (string i : table_mark)
		{
			if (name == i)
			{
				return 0;
			}
		}
		table_mark.push_back(name);
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
		if (s.value == '-' && rel == '<') {
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
		rel = s.value;
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
		cout << REDHB << BWHT << "Error line: " << line << reset << endl;
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
																						table[s_D1][Ar_Op] = &Parser::C1_ArrowL;	table[s_D1][Relation] = &Parser::C1c;			table[s_D1][Space] = &Parser::A1d;			table[s_D1][LF] = &Parser::A1d;																																																																																																					table[s_D1][EndOfFile] = &Parser::Exit2;
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

		//bad
		table_search[1].letter = 'a';												table_search[1].alt = 3;
		table_search[2].letter = 'd';	table_search[2].f = &Parser::C1_bad;
		//begin
		table_search[3].letter = 'e';
		table_search[4].letter = 'g';
		table_search[5].letter = 'i';
		table_search[6].letter = 'n';	table_search[6].f = &Parser::C1_begin;
		//case
		table_search[7].letter = 'a';												table_search[7].alt = 10;
		table_search[8].letter = 's';
		table_search[9].letter = 'e';	table_search[9].f = &Parser::C1_case;
		//cin
		table_search[10].letter = 'i';												table_search[10].alt = 12;
		table_search[11].letter = 'n';	table_search[11].f = &Parser::C1_cin;
		//cout
		table_search[12].letter = 'o';
		table_search[13].letter = 'u';
		table_search[14].letter = 't';	table_search[14].f = &Parser::C1_cout;
		//declare
		table_search[15].letter = 'e';												table_search[15].alt = 26;
		table_search[16].letter = 'c';												table_search[16].alt = 21;
		table_search[17].letter = 'l';
		table_search[18].letter = 'a';
		table_search[19].letter = 'r';
		table_search[20].letter = 'e';	table_search[20].f = &Parser::C1_declare;
		//default
		table_search[21].letter = 'f';
		table_search[22].letter = 'a';
		table_search[23].letter = 'u';
		table_search[24].letter = 'l';
		table_search[25].letter = 't';	table_search[25].f = &Parser::C1_default;
		//do
		table_search[26].letter = 'o';	table_search[26].f = &Parser::C1_do;
		//else
		table_search[27].letter = 'l';												table_search[27].alt = 30;
		table_search[28].letter = 's';
		table_search[29].letter = 'e';	table_search[29].f = &Parser::C1_else;
		//end
		table_search[30].letter = 'n';
		table_search[31].letter = 'd';	table_search[31].f = &Parser::C1_end;
		//for
		table_search[32].letter = 'o';
		table_search[33].letter = 'r';	table_search[33].f = &Parser::C1_for;
		//if
		table_search[34].letter = 'f';	table_search[34].f = &Parser::C1_if;			table_search[34].alt = 35;
		//int
		table_search[35].letter = 'n';
		table_search[36].letter = 't';	table_search[36].f = &Parser::C1_int;
		//jump
		table_search[37].letter = 'u';
		table_search[38].letter = 'm';
		table_search[39].letter = 'p';	table_search[39].f = &Parser::C3_jump;
		//set
		table_search[40].letter = 'e';												table_search[40].alt = 42;
		table_search[41].letter = 't';	table_search[41].f = &Parser::C1_set;
		//switch
		table_search[42].letter = 'w';
		table_search[43].letter = 'i';
		table_search[44].letter = 't';
		table_search[45].letter = 'c';
		table_search[46].letter = 'h';	table_search[46].f = &Parser::C1_switch;
		//then
		table_search[47].letter = 'h';												table_search[47].alt = 50;
		table_search[48].letter = 'e';
		table_search[49].letter = 'n';	table_search[49].f = &Parser::C1_then;
		//to
		table_search[50].letter = 'o';	table_search[50].f = &Parser::C1_to;		table_search[50].alt = 51;
		//tree
		table_search[51].letter = 'r'; 
		table_search[52].letter = 'e';
		table_search[53].letter = 'e';	table_search[53].f = &Parser::C1_tree;
		//while
		table_search[54].letter = 'h';
		table_search[55].letter = 'i';
		table_search[56].letter = 'l';
		table_search[57].letter = 'e';	table_search[57].f = &Parser::C1_while;
	}

	void printLists() {
		cout << BLUB << BWHT << "table_const" << reset << BLKHB << "\nAdress\t\t\tName" << reset << endl;
		for (auto i : table_const) {
			cout << i << "\t\t";
			if (i->getType() == 0) {
				cout << *(int*)(i->getValue()) << endl;
			}
			else {
				cout << *(Tree*)(i->getValue()) << endl;
			}
		}

		cout << endl << BLUB << BWHT << "Variable table " << reset << BLKHB << "\nAdress\t\t\tName" << reset << endl;
		for (auto i : table_name)
			cout << i << "\t\t" << i->getName() << endl;

		cout << endl << BLUB << BWHT << "Label table" << reset << BLKHB << "\nAdress\t\t\tName" << reset << endl;
		for (auto i : table_mark)
			cout << &i << "\t\t" << i << endl;


		cout << endl << "Lexeme list\nLine\tClass" << endl;
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