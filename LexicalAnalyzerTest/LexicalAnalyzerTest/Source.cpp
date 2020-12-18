#include <iostream>
#include <fstream>
using namespace std;

enum SymbolicTokenClass { IDENTIFIER, COMMENTS, EXCLAMATION_MARK, EQUAL_SIGN, POINT, LF, SPACE, END_OF_FILE, ERROR};

enum States { s_A1, s_A2, s_B1, s_C1, s_D1, s_E1, s_F1, s_STOP };

struct SymbolicToken {
	SymbolicTokenClass token_class;
	int value;
};

const int state_number = 7;
const int class_number = 9;

class Parser {
	SymbolicToken s;
	int identifier;
	typedef int (Parser::* functional_pointer)();
	functional_pointer table[state_number][class_number];

	static SymbolicToken transliterator(int ch) {
		SymbolicToken s;
		s.value = 0;

		if (isdigit(ch) || isalpha(ch)) {
			s.token_class = IDENTIFIER;
			s.value = ch;
		}
		else if (ch == '!') {
			s.token_class = EXCLAMATION_MARK;
			s.value = ch;
		}
		else if (ch == '=') {
			s.token_class = EQUAL_SIGN;
			s.value = ch;
		}
		else if (ch == '.') {
			s.token_class = POINT;
			s.value = ch;
		}
		else if (ch == '#') {
			s.token_class = COMMENTS;
			s.value = ch;
		}
		else if (ch == '\n') {
			s.token_class = LF;
			s.value = ch;
		}
		else if (isspace(ch)) {
			s.token_class = SPACE;
			s.value = ch;
		}
		else if (ch == EOF) {
			s.token_class = END_OF_FILE;
			s.value = ch;
		}
		else {
			s.token_class = ERROR;
		}
		return s;	
	}

	int A1() {
		return s_A1;
	}
	int A2() {
		return s_A2;
	}

	int B1() {
		return s_B1;
	}

	int C1() {
		return s_C1;
	}

	int D1() {
		return s_D1;
	}

	int E1() {
		return s_E1;
	}

	int F1() {
		return s_F1;
	}
	int ERR()
	{
		cout << "ERROR";
		return s_STOP;
	}

	int ENDOFFILE()
	{
		cout << "Ok";
		return s_STOP;
	}

	public: 
		Parser() {
			for (int i = 0; i < state_number; i++) {
				for (int j = 0; j < class_number; j++) {
					table[i][j] = &Parser::ERR;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			table[s_A1][IDENTIFIER]	= &Parser::B1;		table[s_A1][COMMENTS]		= &Parser::C1;			table[s_A1][EXCLAMATION_MARK]	= &Parser::D1;
			table[s_A1][POINT]		= &Parser::E1;		table[s_A1][EQUAL_SIGN]		= &Parser::F1;			table[s_A1][LF]					= &Parser::A1;
			table[s_A1][SPACE]		= &Parser::A1;		table[s_A1][END_OF_FILE]	= &Parser::ENDOFFILE;		
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																											
																											table[s_A2][LF]					= &Parser::A1;
			table[s_A2][SPACE]		= &Parser::A1;		table[s_A2][END_OF_FILE]	= &Parser::ENDOFFILE;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			table[s_B1][IDENTIFIER]	= &Parser::B1;															
																											table[s_B1][LF]					= &Parser::A1;			
			table[s_B1][SPACE]		= &Parser::A1;		table[s_B1][END_OF_FILE]	= &Parser::ENDOFFILE;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			table[s_C1][IDENTIFIER] = &Parser::C1;		table[s_C1][COMMENTS]		= &Parser::C1;			table[s_C1][EXCLAMATION_MARK]	= &Parser::C1;
			table[s_C1][POINT]		= &Parser::C1;		table[s_C1][EQUAL_SIGN]		= &Parser::C1;			table[s_C1][LF]					= &Parser::A1;
			table[s_C1][SPACE]		= &Parser::C1;		table[s_C1][END_OF_FILE]	= &Parser::ENDOFFILE;	table[s_C1][ERROR]				= &Parser::C1;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																											table[s_D1][EXCLAMATION_MARK]	= &Parser::A2;
														table[s_D1][EQUAL_SIGN]		= &Parser::A2;			table[s_D1][LF]					= &Parser::A1;
			table[s_D1][SPACE]		= &Parser::A1;		table[s_D1][END_OF_FILE]	= &Parser::ENDOFFILE;	
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			table[s_E1][POINT]		= &Parser::A2;		table[s_E1][EQUAL_SIGN]		= &Parser::A2;
														table[s_E1][END_OF_FILE]	= &Parser::ENDOFFILE;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

														table[s_F1][EQUAL_SIGN]		= &Parser::A2;
														table[s_F1][END_OF_FILE]	= &Parser::ENDOFFILE;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		void parse(const char* filename) {
			ifstream in(filename);
			if (!in) {
				cout << "FILE ERROR";
				return;
			}
			int ch;
			int state = s_A1;
			while (state != s_STOP) {
				ch = in.get();
				s = transliterator(ch);
				state = (this->*table[state][s.token_class])();
			}
			in.close();
		}

};

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "INPUT ERROR" << endl;
		return 1;
	}
	
	Parser p;
	p.parse(argv[1]);

	return 0;

}