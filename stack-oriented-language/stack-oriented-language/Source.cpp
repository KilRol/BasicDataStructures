#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include "Tree.h"
using namespace std;

enum LexemeTokenClass { PUSH, POP, AR_OP, REL, JMP, JI, READ, WRITE, END, COMMENT, ERROR, ENDMARK };
enum SymbolicTokenClass { Letter, Digit, Ar_Op, Relation, Space, LF, Semicolon, Error, EndOfFile, Open_Brace, Closed_Brace, Comma };
enum States { s_A1, s_A2, s_B1, s_C1, s_D1, s_E1, s_E2, s_E3, s_F1, s_F2, s_F3, s_G1, s_H1, s_I1, s_I2, s_J1, s_K1, s_K2, s_K3, s_K4, s_Exit1, s_Exit2, s_Exit3, s_Exit4 };

const string keywords[] = { "end", "ji", "jmp", "pop", "push", "read", "write" };

int init_vector[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 6, 0, 11, 0, 0, 0, 0, 14, 0, 0, 0 };

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
};

struct Variable {
    string name;
    int type;
    void* value;

    Variable(string name, void* value) {
        this->name = name;
        this->value = value;
    }
};

vector<Variable*> table_const;
vector<Variable*> table_name;
list<LexemeToken> list_lexeme;

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
    bool flag_const;

    string tree;

    typedef int (Parser::* function_pointer)();
    function_pointer table[state_number][class_number];

    struct SearchTableClass
    {
        char letter;
        int alt;
        function_pointer f;
    };

    SearchTableClass table_search[18];
    
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
        else if (lexeme_class == REL)
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
        //cout << "LOG:" << endl << "Class: " << lexeme_token.lexeme_token_class << endl << "Line: " << lexeme_token.line << endl << "Value: " << lexeme_token.value << endl << endl;
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
            if (name == i->name)
            {
                ptr = i;
            }
        }
        table_name.push_back(new Variable(name, 0));
        ptr = table_name[table_name.size() - 1];
        return 0;
    }

    void AddConst()
    {
        if (flag_const == 0) return;
        for (Variable* i : table_const)
        {
            if (num == (int)(i->value))
            {
                ptr = i;
                return;
            }   
        }
        table_const.push_back(new Variable("", &num));
        ptr = table_const[table_const.size() - 1];
    }

    int AddTree()
    {
        cout << tree << endl;
        for (string s : keywords)
        {
            if (s == tree) return Error1();
        }
        for (Variable* i : table_const)
        {
            if (tree == (char*)(i->value))
            {
                ptr = i;
                return 0;
            }
        }
        table_const.push_back(new Variable("", &tree));
        ptr = table_const[table_const.size() - 1];
        return 0;
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
        AddTree();
        CreateLexeme();
        return s_C1;
    }
    int D1a()
    {
        lexeme_class = REL;
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
        return s_Exit1;
    }
    int Exit2()
    {
        if (rel == '!') lexeme_class = ERROR;
        CreateLexeme();
        lexeme_class = ENDMARK;
        CreateLexeme();
        return s_Exit2;
    }
    int Exit3()
    {
        AddConst();
        CreateLexeme();
        lexeme_class = ENDMARK;
        CreateLexeme();
        return s_Exit3;
    }
    int Exit4()
    {
        AddVariable();
        CreateLexeme();
        lexeme_class = ENDMARK;
        CreateLexeme();
        return s_Exit4;
    }

    void interpretator() {
        //TODO
    }

public:
    Parser()
    {
        for (int i = 0; i < state_number; i++)
            for (int j = 0; j < class_number; j++)
                table[i][j] = &Parser::Error1;

        table[s_A1][Letter] = &Parser::B1a;     table[s_A1][Ar_Op] = &Parser::C1a;      table[s_A1][Relation] = &Parser::D1a;   table[s_A1][Space] = &Parser::A1;       table[s_A1][LF] = &Parser::A1b;      table[s_A1][Semicolon] = &Parser::I1a;
        table[s_A2][Letter] = &Parser::B1a;     table[s_A2][Ar_Op] = &Parser::C1a;      table[s_A2][Relation] = &Parser::D1a;   table[s_A2][Space] = &Parser::A2;       table[s_A2][LF] = &Parser::A2a;      table[s_A2][Semicolon] = &Parser::I2a;     table[s_A2][EndOfFile] = &Parser::Exit1;
        table[s_B1][Letter] = &Parser::M1;                                                                                                                              table[s_B1][LF] = &Parser::A2f;
                                                                                                                                table[s_C1][Space] = &Parser::C1;       table[s_C1][LF] = &Parser::A2;       table[s_C1][Semicolon] = &Parser::I2a;     table[s_C1][EndOfFile] = &Parser::Exit1;
        table[s_D1][Relation] = &Parser::C1h;   table[s_D1][Space] = &Parser::C1g;      table[s_D1][LF] = &Parser::A2e;         table[s_D1][Semicolon] = &Parser::I2d;                                                                                  table[s_D1][EndOfFile] = &Parser::Exit2;
                                                table[s_E1][Space] = &Parser::F1;       table[s_E1][LF] = &Parser::A2f;
                                                table[s_E2][Space] = &Parser::F2;       table[s_E2][LF] = &Parser::A2f;
                                                table[s_E3][Space] = &Parser::F3;       table[s_E3][LF] = &Parser::A2f;
        table[s_F1][Letter] = &Parser::H1a;     table[s_F1][Digit] = &Parser::G1a;      table[s_F1][Space] = &Parser::F1;       table[s_F1][Open_Brace] = &Parser::K1a;                                          table[s_F1][LF] = &Parser::A2f;
        table[s_F2][Digit] = &Parser::G1a;      table[s_F2][Space] = &Parser::F2;       table[s_F2][LF] = &Parser::A2f;
        table[s_F3][Letter] = &Parser::H1a;     table[s_F3][Space] = &Parser::F3;       table[s_F3][LF] = &Parser::A2f;
        table[s_G1][Digit] = &Parser::G1b;      table[s_G1][Space] = &Parser::C1e;      table[s_G1][LF] = &Parser::A2c;         table[s_G1][Semicolon] = &Parser::I2b;                                                                                  table[s_G1][EndOfFile] = &Parser::Exit3;
        table[s_H1][Letter] = &Parser::H1b;     table[s_H1][Digit] = &Parser::H1b;      table[s_H1][Space] = &Parser::C1f;               table[s_H1][LF] = &Parser::A2d;                          table[s_H1][Semicolon] = &Parser::I2c;                   table[s_H1][EndOfFile] = &Parser::Exit4;
        table[s_I1][Letter] = &Parser::I1;      table[s_I1][Digit] = &Parser::I1;       table[s_I1][Ar_Op] = &Parser::I1;                table[s_I1][Relation] = &Parser::I1;            table[s_I1][Space] = &Parser::I1;           table[s_I1][LF] = &Parser::A1a;  table[s_I1][Semicolon] = &Parser::I1;  table[s_I1][Error] = &Parser::I1;
        table[s_I2][Letter] = &Parser::I2;      table[s_I2][Digit] = &Parser::I2;       table[s_I2][Ar_Op] = &Parser::I2;                table[s_I2][Relation] = &Parser::I2;            table[s_I2][Space] = &Parser::I2;           table[s_I2][LF] = &Parser::A2b;  table[s_I2][Semicolon] = &Parser::I2;  table[s_I2][Error] = &Parser::I2; table[s_I2][EndOfFile] = &Parser::Exit1;
        table[s_J1][Letter] = &Parser::J1;      table[s_J1][Digit] = &Parser::J1;       table[s_J1][Ar_Op] = &Parser::J1;                table[s_J1][Relation] = &Parser::J1;            table[s_J1][Space] = &Parser::J1;           table[s_J1][LF] = &Parser::A2a;  table[s_J1][Semicolon] = &Parser::J1;  table[s_J1][Error] = &Parser::J1; table[s_J1][EndOfFile] = &Parser::Exit1;
        table[s_K1][Digit] = &Parser:: K1b;     table[s_K1][Relation] = &Parser::K2a;
        table[s_K2][Digit] = &Parser::K2b;      table[s_K2][Space] = &Parser::K3;       table[s_K2][Comma] = &Parser::K4a;                                                         table[s_K2][Closed_Brace] = &Parser::C1i;
        table[s_K3][Space] = &Parser::K3;       table[s_K3][Comma] = &Parser::K4a;
        table[s_K4][Digit] = &Parser::K1b;      table[s_K4][Space] = &Parser::K4;  
        for (int i = 0; i < 18; i++) table_search[i].f = &Parser::B1b;

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
    }

    void parse(const char* filename)
    {
        ifstream in(filename);
        if (!in)
        {
            cout << "Не удалось открыть файл " << filename << endl;
            return;
        }

        int ch;             // Входной символ.
        int state = s_A1;   // Текущее состояние автомата.
        while (state != s_Exit1)
        {
            // s_Stop - техническое состояние. Когда автомат попадает в s_Stop, цикл останавливается.
            ch = in.get();
            //cout << "Symbol " << (char)ch << " has been detected" << endl;
            s = transliterator(ch);
            state = (this->*table[state][s.symbolic_token_class])();
        }

        interpretator();

        in.close();
    }
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Формат: ./имя-исполняемого-файла имя-входного-файла" << endl;
        return 1;
    }

    Parser p;
    p.parse(argv[1]);

 

    return 0;
}