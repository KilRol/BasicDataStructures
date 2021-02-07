#include "BooleanVector.h"
#include <fstream>
#include <stack>
#include <vector>

enum LexemeTokenClass { PUSH, POP, AR_OP, RELATION, JMP, JI, READ, WRITE, END, COMMENT, ERROR, ENDMARK, SHIFT_LEFT, SHIFT_RIGHT, CONJUNCTION, DISJUNCTION, SUBVECTOR, CONCATENATION, DOTP };
enum SymbolicTokenClass { Letter, Digit, Ar_Op, Relation, Space, LF, Semicolon, Error, EndOfFile, Open_Angle, Closed_Angle };
enum States { s_A1, s_A2, s_B1, s_C1, s_D1, s_E1, s_E2, s_E3, s_F1, s_F2, s_F3, s_G1, s_H1, s_I1, s_I2, s_J1, s_Exit, s_N1, s_N2, s_N3, s_N4 };

string LexemeTokenClassName[] = { "PUSH", "POP", "AR_OP", "RELATION", "JMP", "JI", "READ", "WRITE", "END", "COMMENT", "ERROR", "ENDMARK", "SHIFT_LEFT", "SHIFT_RIGHT", "CONJUNCTION", "DISJUNCTION", "SUBVECTOR", "CONCATENATION", "DOTP" };

const string keywords[] = { "end", "ji", "jmp", "pop", "push", "read", "write", "shiftl", "shiftr", "conjunction", "disjunction", "subv", "concatenation", "dotp" };

int init_vector[] = { 0, 0, 34, 24, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 6, 0, 11, 18, 0, 0, 0, 14, 0, 0, 0 };

struct SymbolicToken
{
	SymbolicTokenClass token;
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

const int state_number = 22;
const int class_number = 20;

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
	Variable(string name, BooleanVector value, int type)
	{
		this->name = name;
		this->value = new BooleanVector(value);
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
			value = new BooleanVector(*(BooleanVector*)(v.value));
		}
	}
	~Variable()
	{
		if (value)
		{
			delete value;
		}
	}
	Variable conjunction(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			cout << "Function 'conjunction' is not define for int" << endl;
			exit(1);
		}
		else if (type == 1)
		{
			return Variable("", (*(BooleanVector*)value).conjunction(*(BooleanVector*)var.value), type);
		}
	}
	Variable disjunction(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			cout << "Function 'disjunction' is not define for int" << endl;
			exit(1);
		}
		else if (type == 1)
		{
			return Variable("", (*(BooleanVector*)value).disjunction(*(BooleanVector*)var.value), type);
		}
	}
	Variable concatenation(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			cout << "Function 'concatenation' is not define for int" << endl;
			exit(1);
		}
		else if (type == 1)
		{
			return Variable("", (*(BooleanVector*)value).concatenation(*(BooleanVector*)var.value), type);
		}
	}
	Variable dotp(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			cout << "Function 'dot product' is not define for int" << endl;
			exit(1);
		}
		else if (type == 1)
		{
			return Variable("", (*(BooleanVector*)value).dot_product(*(BooleanVector*)var.value), type);
		}
	}
	Variable subv(Variable& from, Variable& to)
	{
		if (type != 1 || from.type != 0 || to.type != 0)
		{
			cout << "Error: argument types do not match";
			exit(1);
		}
		if (type == 0)
		{
			cout << "Function 'subvector' is not define for int" << endl;
			exit(1);
		}
		else if (type == 1)
		{
			return Variable("", (*(BooleanVector*)value).subvector(*(int*)from.value, *(int*)to.value), type);
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
			value = new BooleanVector(*(BooleanVector*)(var.value));
		}
		type = var.type;
		return *this;
	}
	Variable operator+(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			return Variable("", *(int*)value + *(int*)var.value, type);
		}
		else if (type == 1)
		{
			cout << "Operator + is not define for BooleanVector" << endl;
			exit(1);
		}
	}
	Variable operator-(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			return Variable("", *(int*)value - *(int*)var.value, type);
		}
		else if (type == 1)
		{
			cout << "Operator - is not define for BooleanVector" << endl;
			exit(1);
		}
	}
	Variable operator*(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			return Variable("", *(int*)value * *(int*)var.value, type);
		}
		else if (type == 1)
		{
			cout << "Operator * is not define for BooleanVector" << endl;
			exit(1);
		}
	}
	Variable operator/(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			return Variable("", *(int*)value / *(int*)var.value, type);
		}
		else if (type == 1)
		{
			cout << "Operator / is not define for BooleanVector" << endl;
			exit(1);
		}
	}
	Variable operator%(Variable& var)
	{
		if (type != var.type)
		{
			cout << "Error: Argument types are not equal";
			exit(1);
		}
		if (type == 0)
		{
			return Variable("", *(int*)value % *(int*)var.value, type);
		}
		else if (type == 1)
		{
			cout << "Operator % is not define for BooleanVector" << endl;
			exit(1);
		}
	}
	Variable operator<<(Variable& var)
	{
		if (type != 1 || var.type != 0)
		{
			cout << "Error: argument types do not match";
			exit(1);
		}
		if (type == 0)
		{
			return Variable("", *(int*)value << *(int*)var.value, type);
		}
		else if (type == 1)
		{
			return Variable("", *(BooleanVector*)value << *(int*)var.value, type);
		}
	}
	Variable operator>>(Variable& var)
	{
		if (type != 1 || var.type != 0)
		{
			cout << "Error: argument types do not match";
			exit(1);
		}
		if (type == 0)
		{
			return Variable("", *(int*)value >> *(int*)var.value, type);
		}
		else if (type == 1)
		{
			return Variable("", *(BooleanVector*)value >> *(int*)var.value, type);
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
			cout << "Operator > is not define for BooleanVector" << endl;
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
			cout << "Operator < is not define for BooleanVector" << endl;
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
			return *(BooleanVector*)value == *(BooleanVector*)(var.value);
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
			return os << *(BooleanVector*)(var.value) << endl;
		}
	}
};

vector<Variable*> table_const;
vector<Variable*> table_name;
vector<LexemeToken> list_lexeme;

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
	string boolean_vector_str;
	bool flag_const;

	typedef int (Parser::* function_pointer)();
	function_pointer table[state_number][class_number];

	struct SearchTableClass
	{
		char letter;
		int alt;
		function_pointer f;
	};

	SearchTableClass table_search[60];

	static SymbolicToken transliterator(int ch)
	{
		SymbolicToken s;
		s.value = 0;
		if (isdigit(ch))
		{
			s.token = Digit;
			s.value = ch - '0';
		}
		else if (isalpha(ch))
		{
			s.token = Letter;
			s.value = ch;
		}
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
		{
			s.token = Ar_Op;
			s.value = ch;
		}
		else if (ch == '=' || ch == '!' || ch == '>' || ch == '<')
		{
			s.token = Relation;
			s.value = ch;
		}
		else if (ch == ' ' || ch == '\t')
		{
			s.token = Space;
			s.value = ch;
		}
		else if (ch == '\n')
		{
			s.token = LF;
			s.value = ch;
		}
		else if (ch == ';')
		{
			s.token = Semicolon;
			s.value = ch;
		}
		else if (ch == EOF)
		{
			s.token = EndOfFile;
			s.value = ch;
		}
		else
		{
			s.token = Error;
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

	void AddConst() {
		if (flag_const == 0) return;
		if (boolean_vector_str.empty())
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
			BooleanVector h(boolean_vector_str);
			boolean_vector_str.clear();
			for (Variable* i : table_const)
			{
				if (i->getType() != 1) continue;
				if (h == *(BooleanVector*)(i->getValue()))
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
	int N1() {
		return s_N1;
	}
	int N2() {
		return s_N2;
	}
	int N3() {
		return s_N3;
	}
	int N4() {
		return s_N4;
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
		if (s.value == '>') {
			AddConst();
			CreateLexeme();
			return s_C1;
		}
		return Error1();
	}
	int C1j()
	{
		lexeme_class = SHIFT_LEFT;
		CreateLexeme();
		return s_C1;
	}
	int C1k()
	{
		lexeme_class = SHIFT_RIGHT;
		CreateLexeme();
		return s_C1;
	}
	int C1l()
	{
		lexeme_class = DISJUNCTION;
		CreateLexeme();
		return s_C1;
	}
	int C1m()
	{
		lexeme_class = CONJUNCTION;
		CreateLexeme();
		return s_C1;
	}
	int C1n()
	{
		lexeme_class = CONCATENATION;
		CreateLexeme();
		return s_C1;
	}
	int C1o()
	{
		lexeme_class = SUBVECTOR;
		CreateLexeme();
		return s_C1;
	}
	int C1p()
	{
		lexeme_class = DOTP;
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
	int N1a() {
		if (s.value != '<') return Error1();
		boolean_vector_str = boolean_vector_str + (char)s.value;
		return s_N1;
	}
	int N2a() {
		if (s.value != '<') return Error1();
		boolean_vector_str = boolean_vector_str + (char)s.value;
		return s_N2;
	}
	int N2b() {
		boolean_vector_str = boolean_vector_str + (char)s.value;
		return s_N2;
	}
	int N3a() {
		boolean_vector_str = boolean_vector_str + (char)(s.value + 48);
		return s_N3;
	}
	int N4a() {
		if (s.value != '>') return Error1();
		return s_N4;
	}

	void interpretator() {
		stack<Variable> Stack;

		for (int i = 0; list_lexeme[i].lexeme_token_class != END && list_lexeme[i].lexeme_token_class != ENDMARK; i++)
		{
			if (list_lexeme[i].lexeme_token_class == READ)
			{
				cout << "Enter 1 to int, 2 to BooleanVector" << endl;
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
					BooleanVector val;
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
					(Stack.top().getType() == 1 && (*(BooleanVector*)(Stack.top().getValue())).isEmpty() == 0)) {
					int tmp_line = *((int*)(list_lexeme[i].value));
					for (i = tmp_line - 2; list_lexeme[i + 1].line < tmp_line; i++);
				}
				Stack.pop();
			}
			else if (list_lexeme[i].lexeme_token_class == COMMENT);
			else if (list_lexeme[i].lexeme_token_class == SHIFT_LEFT)
			{

				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable res = a << b;
				Stack.push(res);
			}
			else if (list_lexeme[i].lexeme_token_class == SHIFT_RIGHT)
			{
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable res = a >> b;
				Stack.push(res);
			}
			else if (list_lexeme[i].lexeme_token_class == CONJUNCTION)
			{
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable res = a.conjunction(b);
				Stack.push(res);
			}
			else if (list_lexeme[i].lexeme_token_class == DISJUNCTION)
			{
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable res = a.disjunction(b);
				Stack.push(res);
			}
			else if (list_lexeme[i].lexeme_token_class == CONCATENATION)
			{
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable res = a.concatenation(b);
				Stack.push(res);
			}
			else if (list_lexeme[i].lexeme_token_class == SUBVECTOR)
			{
				Variable c = Stack.top();
				Stack.pop();
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable res = a.subv(b, c);
				Stack.push(res);
			}
			else if (list_lexeme[i].lexeme_token_class == DOTP)
			{
				Variable b = Stack.top();
				Stack.pop();
				Variable a = Stack.top();
				Stack.pop();
				Variable r = a.dotp(b);
				int res = *(int*)r.getValue();
				Stack.push(Variable("", res, 0));
			}
			else {
				cout << "Lexeme = Error, Line: " << list_lexeme[i].line << endl;
				exit(1);
			}
			//cout << LexemeTokenClassName[list_lexeme[i].lexeme_token_class] << endl;
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
		table[s_F1][Letter] = &Parser::H1a;     table[s_F1][Digit] = &Parser::G1a;      table[s_F1][Space] = &Parser::F1;		table[s_F1][Relation] = &Parser::N1a;								         table[s_F1][LF] = &Parser::A2f;
		table[s_F2][Digit] = &Parser::G1a;      table[s_F2][Space] = &Parser::F2;       table[s_F2][LF] = &Parser::A2f;
		table[s_F3][Letter] = &Parser::H1a;     table[s_F3][Space] = &Parser::F3;       table[s_F3][LF] = &Parser::A2f;
		table[s_G1][Digit] = &Parser::G1b;      table[s_G1][Space] = &Parser::C1e;      table[s_G1][LF] = &Parser::A2c;         table[s_G1][Semicolon] = &Parser::I2b;          table[s_G1][EndOfFile] = &Parser::Exit3;
		table[s_H1][Letter] = &Parser::H1b;     table[s_H1][Digit] = &Parser::H1b;      table[s_H1][Space] = &Parser::C1f;      table[s_H1][LF] = &Parser::A2d;                 table[s_H1][Semicolon] = &Parser::I2c;      table[s_H1][EndOfFile] = &Parser::Exit4;
		table[s_I1][Letter] = &Parser::I1;      table[s_I1][Digit] = &Parser::I1;       table[s_I1][Ar_Op] = &Parser::I1;       table[s_I1][Relation] = &Parser::I1;            table[s_I1][Space] = &Parser::I1;           table[s_I1][LF] = &Parser::A1a;  table[s_I1][Semicolon] = &Parser::I1;  table[s_I1][Error] = &Parser::I1;
		table[s_I2][Letter] = &Parser::I2;      table[s_I2][Digit] = &Parser::I2;       table[s_I2][Ar_Op] = &Parser::I2;       table[s_I2][Relation] = &Parser::I2;            table[s_I2][Space] = &Parser::I2;           table[s_I2][LF] = &Parser::A2b;  table[s_I2][Semicolon] = &Parser::I2;  table[s_I2][Error] = &Parser::I2; table[s_I2][EndOfFile] = &Parser::Exit1;
		table[s_J1][Letter] = &Parser::J1;      table[s_J1][Digit] = &Parser::J1;       table[s_J1][Ar_Op] = &Parser::J1;       table[s_J1][Relation] = &Parser::J1;            table[s_J1][Space] = &Parser::J1;           table[s_J1][LF] = &Parser::A2a;  table[s_J1][Semicolon] = &Parser::J1;  table[s_J1][Error] = &Parser::J1; table[s_J1][EndOfFile] = &Parser::Exit1;
		table[s_N1][Relation] = &Parser::N2a;
		table[s_N2][Digit] = &Parser::N3a;
		table[s_N3][Semicolon] = &Parser::N2b; table[s_N3][Relation] = &Parser::N4a;
		table[s_N4][Relation] = &Parser::C1i;

		for (int i = 0; i < 60; i++) table_search[i].f = &Parser::B1b;
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
		table_search[18].letter = 'h';										table_search[18].alt = 54;
		table_search[19].letter = 'i';
		table_search[20].letter = 'f';
		table_search[21].letter = 't';
		table_search[22].letter = 'l'; table_search[22].f = &Parser::C1j;	table_search[22].alt = 23;
		table_search[23].letter = 'r'; table_search[23].f = &Parser::C1k;
		table_search[24].letter = 'i';										table_search[24].alt = 57;
		table_search[25].letter = 's';
		table_search[26].letter = 'j';
		table_search[27].letter = 'u';
		table_search[28].letter = 'n';
		table_search[29].letter = 'c';
		table_search[30].letter = 't';
		table_search[31].letter = 'i';
		table_search[32].letter = 'o';
		table_search[33].letter = 'n'; table_search[33].f = &Parser::C1l;
		table_search[34].letter = 'o';
		table_search[35].letter = 'n';
		table_search[36].letter = 'j';										table_search[36].alt = 44;
		table_search[37].letter = 'u';
		table_search[38].letter = 'n';
		table_search[39].letter = 'c';
		table_search[40].letter = 't';
		table_search[41].letter = 'i';
		table_search[42].letter = 'o';
		table_search[43].letter = 'n'; table_search[43].f = &Parser::C1m;
		table_search[44].letter = 'c';
		table_search[45].letter = 'a';
		table_search[46].letter = 't';
		table_search[47].letter = 'e';
		table_search[48].letter = 'n';
		table_search[49].letter = 'a';
		table_search[50].letter = 't';
		table_search[51].letter = 'i';
		table_search[52].letter = 'o';
		table_search[53].letter = 'n'; table_search[53].f = &Parser::C1n;
		table_search[54].letter = 'u';
		table_search[55].letter = 'b';
		table_search[56].letter = 'v'; table_search[56].f = &Parser::C1o;
		table_search[57].letter = 'o';
		table_search[58].letter = 't';
		table_search[59].letter = 'p'; table_search[59].f = &Parser::C1p;

	}

	void printLists() {
		cout << "table_const\nAdress\t\t\tValue" << endl;
		for (auto i : table_const) {
			cout << i << '\t';
			if (i->getType() == 0) cout << *(int*)(i->getValue()) << endl;
			else cout << *(BooleanVector*)(i->getValue()) << endl;
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
			cout << "Error. Open file " << filename << endl;
			return;
		}

		int ch;
		int state = s_A1;
		while (state != s_Exit)
		{
			ch = in.get();
			s = transliterator(ch);
			state = (this->*table[state][s.token])();
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
		cout << "Error" << endl;
		return 1;
	}

	Parser p;
	p.parse(argv[1]);
}