#include "parser.h"
#include "symbol_table.h"
#include <vector>
#include <fstream>
#include <stack>
using namespace std;


vector<string> tacs;
int new_variable_counter = 1;
int address = 0;

void backpatch(vector<string> &tacs)
{
    int c = 0;
    int i = tacs.size()-1;
    string temp = "";
    while (1)
    {
        temp = tacs[i];
        if (temp == "goto ")
        {
            tacs[i] = temp + to_string(i + c + 1);
            return;
        }
        c++;
        i--;
    }


}
void backpatch_loop(vector<string>& tacs, string loop_start)
{
    int c = 0;
    int i = tacs.size() - 1;
    string temp = "";

    
    while (1)
    {
        temp = tacs[i];
        if (temp == loop_start)
        {
            tacs[tacs.size() - 1] = tacs[tacs.size() - 1] + to_string(tacs.size() -1  - c);
            return;
        }
        c++;
        i--;
    }
}

void backpatch_elif(vector<string>& tacs, int elif_count)
{
    for (int i = 0; i < tacs.size(); i++)
    {
        if (tacs[i] == "goto ")
        {
            tacs[i] = "goto " + to_string(tacs.size());
        }
    }
}
void backpatch_elif2(vector<string>& tacs)
{
    int c = 0;
    int i = tacs.size() - 1;
    string temp = "";
    while (1)
    {
        temp = tacs[i];
        if (temp == "goto ")
        {
            tacs[i] = temp + to_string(i + c + 2);
            return;
        }
        c++;
        i--;
    }

}

bool not_operand(char c)
{
    if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%')
    {
        return false;
    }
    return true;

}






void update_symbol_table(string new_variable) {

    fstream out;
    out.open("symbol_table.txt", ios::app);
    if (!out)
    {
        cout << "Error in opening the symbol table for upadting!";
        exit(-1);
    }

    out << new_variable << " " << "INT" << " " << address <<  endl;
    address = address + 4;
    out.close();

}



void tac_for_assignments(vector<string>& tacs, string str)
{

    if (str.size() > 8)
    {
        stack<string> stk;

        int start = 4;
        int i = 4;

        // push the left variable to stack;
        string a = "", b = "";
        a = a + str[0] + str[1];
        b = b + str[2] + str[3];
        stk.push(a);
        stk.push(b);
        // this while loop generates priority tacs and creates a stack
        string word = "";
        while (1)
        {
            if (str[i] == ';')
            {
                stk.push(word);
                break;
            }
            
            if (str[i] == '=' || str[i] == '+' || str[i] == '-')
            {
                stk.push(word); 
                word = ""; // reset the word variable
                char op = str[i];
                string operand = "";
                operand = operand + op;
                
                stk.push(operand);
            }

            else if (str[i] == '/' || str[i] == '%' || str[i] == '*')
            {
                
                char operand = str[i];
                string x = word;
                

                int fwd_index = i + 1;
                string y = "";
                while (not_operand(str[fwd_index]) && str[fwd_index] != ';')
                {
                    y = y + str[fwd_index];
                    fwd_index++;
                }

                i = fwd_index - 1;
                start = fwd_index - 1;
                // new equation
                string new_variable = 't' + to_string(new_variable_counter);
                string tac = new_variable + " = " + x + operand + y + ';';
                tacs.push_back(tac); // push the string to the tacs
                //stk.push(new_variable);
                new_variable_counter++;

                word = new_variable;
                update_symbol_table(new_variable);

            }
            else
            {
                word = word + str[i];
            }
            i++;
        }
        // empty the stack
        while (stk.size() > 3)
        {
            string y = stk.top();
            stk.pop();
            string operand = stk.top();
            stk.pop();
            string x = stk.top();
            stk.pop();

            string new_variable = 't' + to_string(new_variable_counter);
            string tac = new_variable + " = " + x + operand + y + ';';
            tacs.push_back(tac); // push the string to the tacs
            stk.push(new_variable);

            update_symbol_table(new_variable);
            
            
        }

        string y = stk.top();
        stk.pop();
        string operand = stk.top();
        stk.pop();
        string x = stk.top();
        stk.pop();

        tacs.push_back(x + operand + y + ';');

    }
    else
    {
        tacs.push_back(str);
    }
}



void parser::get_tacs()
{
    this->tacs = tacs;
}


void parser :: write_tac_to_file()
{
    ofstream fout;
    fout.open("tac.txt");
    if (!fout)
    {
        cout << "Error in opening the file!";
        exit(-1);
    }

    int i;
     for (i = 0; i < tacs.size(); i++)
     {
        string temp = tacs[i];
        fout << i << " " << temp << endl;
     }

     fout << i;

    fout.close();
}



void parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    system("PAUSE");
    exit(1);
}
token parser::expect(TokenType expected_type)
{
    token t = _lexer.getNextToken();
    t.Print();
    if (t.tokenType != expected_type)
        syntax_error();
   
    return t;
}
 


parser::parser(const char filename[])
{
    _lexer = lexer(filename);
}
void parser::readAndPrintAllInput() //read and print allinputs (provided)
{
    token t;
    t = _lexer.getNextToken();
    while (t.tokenType != TokenType::END_OF_FILE)
    {
        t.Print();
        t = _lexer.getNextToken();
    }
}
void parser::resetPointer()
{
    _lexer.resetPointer();
}
//this function is for sample purposes only

//bool parser::statements()
//{
//    //statements-- > COLON LPAREN start RPAREN
//    if (_lexer.peek(1).tokenType == TokenType::COLON)
//    {
//        expect(TokenType::COLON);
//        if (_lexer.peek(1).tokenType == TokenType::LPAREN)
//        {
//            expect(TokenType::LPAREN);
//            start();
//            if (_lexer.peek(1).tokenType == TokenType::RPAREN)
//            {
//                expect(TokenType::RPAREN);
//                return true;
//            }
//        }
//    }
//    return false;
//} 

bool parser::start()
{
    //marks the start of the code
    //start->FUNC datatype ID parameters COL BEGIN codeBlock END start | null
    
    
    if (_lexer.peek(1).tokenType == TokenType::FUNC)
    {
        expect(TokenType::FUNC);
        if (datatype() == false)
        {
            syntax_error();
        }

        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            expect(TokenType::ID);
            if (parameters() == false)
            {
                syntax_error();
            }
            if (_lexer.peek(1).tokenType == TokenType::COL)
            {
                expect(TokenType::COL);
                if (_lexer.peek(1).tokenType == TokenType::BG)
                {
                    expect(TokenType::BG);
                    if (codeBlock() == false)
                    {
                        syntax_error();
                    }
                    if (_lexer.peek(1).tokenType == TokenType::END)
                    {
                        expect(TokenType::END);
                        if (start() == true) {
                            return true;
                        }

                    }
                    else
                    {
                        syntax_error();
                    }
                }
               
            }
        }
    }
    
    return true;
}
   
bool parser::datatype()
{
    //datatype -> INT | CHAR
    if (_lexer.peek(1).tokenType == TokenType::INT)
    {
        expect(TokenType::INT);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::CHAR)
    {
        expect(TokenType::CHAR);
        return true;
    }
    return false;
}

bool parser::parameters()
{
    //parameters for a function call
    //parameters->ID datatype parameters2 | null
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        expect(TokenType::ID);
        datatype();
        parameters2();
        return true;
    }
    else
    {
        return true;
    }
    return false;
}

bool parser::parameters2()
{
    //parameters2 -> COMMA ID datatype parameters2 | null
    if (_lexer.peek(1).tokenType == TokenType::COM)
    {
        expect(TokenType::COM);
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            expect(TokenType::ID);
            datatype();
            parameters2();
            return true;
        }
        else
        {
            return true;
        }
    }
    return false;
}

bool parser::codeBlock()
{
    //ensures there is at least one occurence of a statement
    //codeBlock->declare | input | print | loop | if_ | functionCall | return
    if (declare() == true)
    {
        return true;
    }
    else if (input() == true)
    {
        return true;
    }
    else if (print() == true)
    {
        return true;
    }
    else if (println() == true)
    {
        return true;
    }
    else if (loop() == true)
    {
        return true;
    }
    else if (if_() == true)
    {
        return true;
    }
    else if (functionCall() == true)
    {
        return true;
    }
    else if (return_() == true)
    {
        return true;
    }
    return false;
}

bool parser::statements()
{
    //general parts of the sample codes
    //statements->declare | input | print | loop | if | functionCall | return_ | null
    if (declare() == true)
    {
        return true;
    }
    else if (input() == true)
    {
        return true;
    }
    else if (print() == true)
    {
        return true;
    }
    else if (println() == true)
    {
        return true;
    }
    else if (loop() == true)
    {
        return true;
    }
    else if (if_() == true)
    {
        return true;
    }
    else if (functionCall() == true)
    {
        return true;
    }
    else if (return_() == true)
    {
        return true;
    }
   
    else
    {
        return false;
    }

}

// can add declare3 which will go to datatype OR null

bool parser::declare()
{
    //initialization lists and declarations
    //declare->ID initializer declare2 datatype SCOL statements
    string tac = "";
     
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        tac = tac + _lexer.peek(1).lexeme + " ";

        bool flag = 1;
        expect(TokenType::ID);
        if (initializer(tac) == false)
        {
            flag = 0;
        }
        else
        {
            tac = tac + ";";
            // make a function which makes expression 
            //tacs.push_back(tac);
            tac_for_assignments(tacs, tac);
        }
        
        declare2();
        //declare3();
        if(datatype() == false && flag == 0)
        {
            return false;
        }
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            statements();
            return true;
        }
        return false;
    }
    
}

// handles multiple assignments
bool parser::declare2()
{
    //declare2 -> COMMA ID initializer declare2 | null
    if (_lexer.peek(1).tokenType == TokenType::COM)
    {
        expect(TokenType::COM);
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            string tac = _lexer.peek(1).lexeme + " ";
            expect(TokenType::ID);
            if(initializer(tac) == true)
            {
                tac = tac + ";";
                tacs.push_back(tac);
            }
            // idher TAC operation perform karo
            declare2();
            return true;
        }

    }

    else
    {
        return true;
    }
    
}

bool parser::initializer(string & str)
{
    //initializer -> AO value2 
    if (_lexer.peek(1).tokenType == TokenType::AO)
    {
        str = str + "= ";
        expect(TokenType::AO);
        value2(str);
        //datatype();
        return true;
    }
   
    return false;
}

bool parser::value2(string & n)
{
    if (expression(n) == true)
    {
        return true;
    }
    if (value(n) == true)  
    {
        
        return true;
    }
    syntax_error();
}

bool parser::expression(string & str)
{
    //handles the precedence of operators
    //expression->t expression2
    if (t(str) == false)
    {
        return false;
    }
    return expression2(str);
}

bool parser::expression2(string & str)
{
    //expression2 -> PLUS t expression2 | MINUS t expression2 | null
    if (_lexer.peek(1).tokenType == TokenType::PLUS)
    {
        str = str + "+";
        expect(TokenType::PLUS);
        t(str);
        expression2(str);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::MINUS)
    {
        str = str + "-";
        expect(TokenType::MINUS);
        t(str);
        expression2(str);
        return true;
    }
    else
    {
        return true;
    }
    return false;
}

//low Precedence
bool parser::t(string & str)
{
    //t -> f t2
    if (f(str) == false)
    {
        return false;
    }
    return t2(str);
}

//highPrecedence
bool parser::t2(string & str)
{
    //t2 -> MUL f t2 | DIV f t2 | MOD f t2 | null
    if (_lexer.peek(1).tokenType == TokenType::MUL)
    {
        str = str + "*";
        expect(TokenType::MUL);
        f(str);
        t2(str);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::DIV)
    {
        str = str + "/";
        expect(TokenType::DIV);
        f(str);
        t2(str);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::MOD)
    {
        str = str + "%";
        expect(TokenType::MOD);
        f(str);
        t2(str);
        return true;
    }
    else
    {
        return true;
    }
 
}

// allotValue
bool parser::f(string & n)
{
    //f -> ID | NL | LPARAN expression RPARAN
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        n = n + _lexer.peek(1).lexeme;
        expect(TokenType::ID);
        return true;
    }
    else  if (_lexer.peek(1).tokenType == TokenType::NL)
    {   
        n = n + _lexer.peek(1).lexeme;
        expect(TokenType::NL);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::LPARAN)
    {
        expect(TokenType::LPARAN);
        expression(n);
        if (_lexer.peek(1).tokenType == TokenType::RPARAN)
        {
            expect(TokenType::RPARAN);
            return true;
        }
    }
    return false;
}

bool parser::value(string & n)
{
    //value -> NL | CL | ID
    if (_lexer.peek(1).tokenType == TokenType::NL)
    {
        n = n + _lexer.peek(1).lexeme;
        expect(TokenType::NL);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::CL)
    {
        n = n + _lexer.peek(1).lexeme;
        expect(TokenType::CL);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        n = n + _lexer.peek(1).lexeme;
        expect(TokenType::ID);
        return true;
    }
    return false;
}

bool parser::input()
{
    string tac = "";
    tac = "in ";
    //input statement
    //input->IN ID SCOL statements
    if (_lexer.peek(1).tokenType == TokenType::IN)
    {   
        expect(TokenType::IN);
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            tac = tac + _lexer.peek(1).lexeme + " ";
            expect(TokenType::ID);
            if (_lexer.peek(1).tokenType == TokenType::SCOL)
            {
                tac = tac + ";";
                tacs.push_back(tac);

                expect(TokenType::SCOL);
                statements();
                return true;
            }
        }
    }
    return false;
}

bool parser::print()
{
    //print statement
    //print->PRINT value3 
    if (_lexer.peek(1).tokenType == TokenType::PRINT)
    {
        string tac = "out ";
        expect(TokenType::PRINT);
        value3(tac);
        
        return true;
        
    }
    return false;
}

bool parser::println()
{
    if (_lexer.peek(1).tokenType == TokenType::PRINTLN)
    {
        string tac = "out ";
        expect(TokenType::PRINTLN);
        value3(tac);
        //add a line skip in tac
        tacs.push_back("out '\n'");
        return true;
    }
    return false;
}

bool parser::value3(string & str)
{
    //value3 -> ID SCOL statements | STR SCOL statements | expression SCOL statements

    if (expression(str) == true)
    {
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            
            expect(TokenType::SCOL);
            tacs.push_back(str+';');
            statements();
            return true;
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        str = str + _lexer.peek(1).lexeme;
        expect(TokenType::ID);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            
            expect(TokenType::SCOL);
            tacs.push_back(str+';');
            statements();
            return true;
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::STR)
    {
        str = str + _lexer.peek(1).lexeme;
        expect(TokenType::STR);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            
            expect(TokenType::SCOL);
            tacs.push_back(str);
            statements();
            return true;
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::CL)
    {
        str = str + _lexer.peek(1).lexeme;
        expect(TokenType::CL);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            tacs.push_back(str);
            statements();
            return true;
        }
    }

    return false;
}

bool parser::loop()
{
    //for loop
    //loop->FOR ID AO NL COMMA expression ro expression COMMA ID AO rightAssign COLON
    //    BEGIN statements END statements
    if (_lexer.peek(1).tokenType == TokenType::FOR)
    {
        expect(TokenType::FOR);
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            string tac = _lexer.peek(1).lexeme;
            expect(TokenType::ID);
            if (_lexer.peek(1).tokenType == TokenType::AO)
            {
                tac = tac + " = ";
                expect(TokenType::AO);
                if (_lexer.peek(1).tokenType == TokenType::NL)
                {
                    tac = tac + _lexer.peek(1).lexeme;
                    // initalization statement
                    tacs.push_back(tac);
                    
                    expect(TokenType::NL);
                    if (_lexer.peek(1).tokenType == TokenType::COM)
                    {
                        expect(TokenType::COM);
                        string stopping_condition = "if ";
                        if (expression(stopping_condition) == false) {
                            syntax_error();
                        }
                        if (relationalOperators(stopping_condition) == false)
                        {
                            syntax_error();
                        }
                        if (expression(stopping_condition) == false)
                        {
                            syntax_error();
                        }
                        stopping_condition = stopping_condition + " goto " + to_string(tacs.size() + 2);
                        tacs.push_back(stopping_condition);
                        tacs.push_back("goto ");
                        TokenType t;
                        t = _lexer.peek(1).tokenType;
                        if (t == TokenType::SCOL || t == TokenType::COM)
                        {
                            expect(t);
                            if (_lexer.peek(1).tokenType == TokenType::ID)
                            {
                                string variable = "";
                                variable = variable + _lexer.peek(1).lexeme;

                                expect(TokenType::ID);
                                if (_lexer.peek(1).tokenType == TokenType::AO)
                                {
                                    variable = variable + " = ";
                                    expect(TokenType::AO);
                                    rightAssign(variable);
                                    if (_lexer.peek(1).tokenType == TokenType::COL)
                                    {
                                        expect(TokenType::COL);
                                        if (_lexer.peek(1).tokenType == TokenType::BG)
                                        {
                                            expect(TokenType::BG);
                                            statements();
                                            if (_lexer.peek(1).tokenType == TokenType::END)
                                            {
                                                expect(TokenType::END);

                                                tacs.push_back(variable);
                                                tacs.push_back("goto ");
                                                backpatch_loop(tacs, stopping_condition);
                                                backpatch(tacs);
                                                
                                                
                                                
                                                statements();
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool parser::rightAssign(string & n)
{
    //rightAssign->expression | NL
    if (_lexer.peek(1).tokenType == TokenType::NL)
    {
        n = n + _lexer.peek(1).lexeme;
        expect(TokenType::NL);
        return true;
    }
    else
    {
        if (expression(n) == true)
        {
            return true;
        }
    }
    return false;
}

bool parser::if_()
{
    //if statement
    //if_->IF expression ro expression COL BEGIN statements END  elif else_ statements
    if (_lexer.peek(1).tokenType == TokenType::IF)
    {
        string tac = "if ";
        expect(TokenType::IF);
        expression(tac);
        relationalOperators(tac);
        expression(tac);
        tac = tac + " " + "goto " + to_string(tacs.size() + 2);
        tacs.push_back(tac);
        tacs.push_back("goto ");
        if (_lexer.peek(1).tokenType == TokenType::COL)
        {
            expect(TokenType::COL);
            if (_lexer.peek(1).tokenType == TokenType::BG)
            {
                expect(TokenType::BG);
                statements();
                if (_lexer.peek(1).tokenType == TokenType::END)
                {
                    backpatch(tacs);
                    expect(TokenType::END);  
                    int elif_count = 0;
                    elif(elif_count);
                    else_();
                    statements();
                    
                    backpatch_elif(tacs, elif_count);
                    return true;
                }
            }
        }
    }
    return false;
}

bool parser::elif(int & elif_count)
{
    //elif statement
    //elif-> elif expression ro expression COL BEGIN statements END elif else_ statements | null

    if (_lexer.peek(1).tokenType == TokenType::ELIF)
    {
        string tac = "if ";
        expect(TokenType::ELIF);
        expression(tac);
        relationalOperators(tac);
        expression(tac);

        tac = tac + " " + "goto " + to_string(tacs.size() + 2);
        tacs.push_back(tac);
        tacs.push_back("goto ");

        if (_lexer.peek(1).tokenType == TokenType::COL)
        {
            expect(TokenType::COL);
            if (_lexer.peek(1).tokenType == TokenType::BG)
            {
                expect(TokenType::BG);
                statements();
                if (_lexer.peek(1).tokenType == TokenType::END)
                {
                    backpatch_elif2(tacs);
                    tacs.push_back("goto ");
                    expect(TokenType::END);
                    elif_count++;
                    elif(elif_count);
                    else_();
                    statements();
                    return true;
                }
            }
        }
    }
    return true;
}

bool parser::else_()
{
    //else_ -> ELSE BEGIN statements END | null
    if (_lexer.peek(1).tokenType == TokenType::ELSE)
    {
        //tacs.push_back("goto ");
        expect(TokenType::ELSE);
        if (_lexer.peek(1).tokenType == TokenType::COL)
        {
            expect(TokenType::COL);
            if (_lexer.peek(1).tokenType == TokenType::BG)
            {
                expect(TokenType::BG);
                statements();
                if (_lexer.peek(1).tokenType == TokenType::END)
                {
                    //backpatch(tacs);
                    expect(TokenType::END);
                    return true;
                }
            }
        }
    }
    else
    {
        return true;
    }
    
}

bool parser::relationalOperators(string & str)
{
    //relational operators
    //ro->EQUALTO | LEQ | LESSTHAN | GEQ | GRTHAN | NOTEQUALS
    if (_lexer.peek(1).tokenType == TokenType::EQUALTO)
    {
        str = str + " " + "== ";
        expect(TokenType::EQUALTO);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::LEQ)
    {
        str = str + " " + "<= ";
        expect(TokenType::LEQ);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::LESSTHAN)
    {
        str = str + " " + "< ";
        expect(TokenType::LESSTHAN);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::GEQ)
    {
        str = str + " " + ">= ";
        expect(TokenType::GEQ);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::GRTHAN)
    {
        str = str + " " + "> ";
        expect(TokenType::GRTHAN);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::NOTEQUALTO)
    {
        str = str + " " + "!= ";
        expect(TokenType::NOTEQUALTO);
        return true;
    }
    return false;
}

bool parser::functionCall()
{
    //function call, general implementation that can send multiple parameters (optional in this assignment)
    //functionCall -> CALL ID id2 SCOL statements
    //id2 -> ID id3 | null
    //id3 -> COMMA id2 | null
    
    
    if (_lexer.peek(1).tokenType == TokenType::CALL)
    {
        expect(TokenType::CALL);
        string tac = "call ";
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            tac = tac + _lexer.peek(1).lexeme;
            expect(TokenType::ID);
            if(id2(tac) == false)
            {
                return false;
            }

            if (_lexer.peek(1).tokenType == TokenType::SCOL)
            {
                expect(TokenType::SCOL);
                tacs.push_back(tac);
                return statements();
            }

        }
        
    }
    return false;
}

bool parser::id2(string & tac)
{
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        tac = tac + " " + _lexer.peek(1).lexeme;
        tacs.push_back("param " + _lexer.peek(1).lexeme);
        expect(TokenType::ID);
        
        return id3(tac);

    }
    return true;
}

bool parser::id3(string &tac)
{
    if (_lexer.peek(1).tokenType == TokenType::COM)
    {
        tac = tac + ", ";
        expect(TokenType::COM);
        return id2(tac);

    }
    return true;

}

bool parser::return_()
{
    //return_ -> RETURN value4
    if (_lexer.peek(1).tokenType == TokenType::RETURN)
    {
        
        expect(TokenType::RETURN);
        string str = "return ";
        value4(str);
        str = str + ';';
        tacs.push_back(str);
        return true;
    }
    return false;
}

bool parser::value4(string & str)
{
    //value4 -> ID SCOL | value SCOL
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        str = str + _lexer.peek(1).lexeme;
        expect(TokenType::ID);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            return true;
        }
    }
    else 
    {
        value(str);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {

            expect(TokenType::SCOL);
            return true;
        }
    }
    return false;
}




void parser::create_symbol_table()
{
    vector<token> tokens = _lexer.return_token();
    vector<symbol_table_element> table;

    vector<token> ::iterator itr = tokens.begin();

    // populate the  table;
    while (itr != tokens.end())
    {

        token* current = new token(*itr);
        if (current->tokenType == TokenType::ID)
        {
            // check if Identifier is a function
            itr = itr - 2;
            if (itr->tokenType == TokenType::FUNC)
            {
                table.push_back(symbol_table_element(current->lexeme, "FUNC"));
                itr = itr + 2;
            }
            else
            {
                itr = itr + 2;
                int count = 0;
                while (itr->tokenType != TokenType::SCOL)
                {
                    TokenType temp = itr->tokenType;
                    
                    if (temp == TokenType::CHAR || temp == TokenType::INT)
                    {
                        string type;
                        if (temp == TokenType::CHAR)
                        {
                            type = "CHAR";
                        }
                        else
                        {
                            type = "INT";
                        }
                        table.push_back(symbol_table_element(current->lexeme, type));
                        itr = itr - count;
                        
                        break;
                    }
                    count++;
                    ++itr;
                }
            }
        }
        ++itr;
    }


    // write to file
    ofstream fin;
    fin.open("symbol_table.txt");
    if (!fin)
    {
        cout << "Error in opening the file";
        EXIT_FAILURE;
    }

    for (int i = 0; i < table.size(); i++)
    {
        fin << table[i].lexeme << " " << table[i].type << " " << address << endl;
        if (table[i].type == "INT")
        {
            address = address + 4;
        }
        else
        {
            address = address + 1;
        }
    }

}