#include "parser.h"

void parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}
token parser::expect(TokenType expected_type)
{
    token t = _lexer.getNextToken();
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
    //start->FUNC datatype ID parameters COL BEGIN codeBlock END
    if (_lexer.peek(1).tokenType == TokenType::FUNC)
    {
        expect(TokenType::FUNC);
        datatype();
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            expect(TokenType::ID);
            parameters();
            if (_lexer.peek(1).tokenType == TokenType::COL)
            {
                expect(TokenType::COL);
                if (_lexer.peek(1).tokenType == TokenType::BG)
                {
                    expect(TokenType::BG);
                    codeBlock();
                    if (_lexer.peek(1).tokenType == TokenType::END)
                    {
                        expect(TokenType::END);
                        return true;
                    }
                }
               
            }
        }
    }
    return false;
}

bool parser::datatype()
{
    //datatype -> INT | CHAR
    if (_lexer.peek(1).tokenType == TokenType::INT)
    {
        expect(TokenType::INT);
        return true;
    }
    else 
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
        return true;
    }
    return false;
}

bool parser::declare()
{
    //initialization lists and declarations
    //declare->ID initializer declare2 datatype SCOL statements
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        expect(TokenType::ID);
        initializer();
        declare2();
        datatype();
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            statements();
            return true;
        }
    }
    return false;
}

bool parser::declare2()
{
    //declare2 -> COMMA ID initializer declare2 | null
    if (_lexer.peek(1).tokenType == TokenType::COM)
    {
        expect(TokenType::COM);
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            expect(TokenType::ID);
            initializer();
            declare2();
            return true;
        }
    }
    else
    {
        return true;
    }
    return false;
}

bool parser::initializer()
{
    //initializer -> AO value2 | null
    if (_lexer.peek(1).tokenType == TokenType::AO)
    {
        expect(TokenType::AO);
        value2();
        return true;
    }
    return false;
}

bool parser::value2()
{
    if (expression() == true)
    {
        return true;
    }
    if (value() == true)
    {
        return true;
    }
    return false;
}

bool parser::expression()
{
    //handles the precedence of operators
    //expression->t expression2
    if (t() == false)
    {
        return false;
    }
    return expression2();
}

bool parser::expression2()
{
    //expression2 -> PLUS t expression2 | MINUS t expression2 | null
    if (_lexer.peek(1).tokenType == TokenType::PLUS)
    {
        expect(TokenType::PLUS);
        t();
        expression2();
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::MINUS)
    {
        expect(TokenType::MINUS);
        t();
        expression2();
        return true;
    }
    else
    {
        return true;
    }
    return false;
}

bool parser::t()
{
    //t -> f t2
    if (f() == false)
    {
        return false;
    }
    return t2();
}

bool parser::t2()
{
    //t2 -> MUL f t2 | DIV f t2 | MOD f t2 | null
    if (_lexer.peek(1).tokenType == TokenType::MUL)
    {
        expect(TokenType::MUL);
        f();
        t2();
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::DIV)
    {
        expect(TokenType::DIV);
        f();
        t2();
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::MOD)
    {
        expect(TokenType::MOD);
        f();
        t2();
        return true;
    }
    else
    {
        return true;
    }
    return false;
}

bool parser::f()
{
    //f -> ID | NL | LPARAN expression RPARAN
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        expect(TokenType::ID);
        return true;
    }
    else  if (_lexer.peek(1).tokenType == TokenType::NL)
    {
        expect(TokenType::NL);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::LPARAN)
    {
        expect(TokenType::LPARAN);
        expression();
        if (_lexer.peek(1).tokenType == TokenType::RPARAN)
        {
            expect(TokenType::RPARAN);
            return true;
        }
    }
    return false;
}

bool parser::value()
{
    //value -> NL | CL | ID
    if (_lexer.peek(1).tokenType == TokenType::NL)
    {
        expect(TokenType::NL);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::CL)
    {
        expect(TokenType::CL);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        expect(TokenType::ID);
        return true;
    }
    return false;
}

bool parser::input()
{
    //input statement
    //input->IN ID SCOL statements
    if (_lexer.peek(1).tokenType == TokenType::IN)
    {
        expect(tokenType::IN);
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            expect(TokenType::ID);
            if (_lexer.peek(1).tokenType == TokenType::SCOL)
            {
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
        expect(TokenType::PRINT);
        value3();
        return true;
    }
    return false;
}

bool parser::value3()
{
    //value3 -> ID SCOL statements | STR SCOL statements | expression SCOL statements
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        expect(TokenType::ID);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            statements();
            return true;
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::STR)
    {
        expect(TokenType::STR);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            statements();
            return true;
        }
    }
    else
    {
        expression();
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
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
            expect(TokenType::ID);
            if (_lexer.peek(1).tokenType == TokenType::AO)
            {
                expect(TokenType::AO);
                if (_lexer.peek(1).tokenType == TokenType::NL)
                {
                    expect(TokenType::NL);
                    if (_lexer.peek(1).tokenType == TokenType::COM)
                    {
                        expect(TokenType::COM);
                        expression();
                        relationalOperators();
                        expression();
                        if (_lexer.peek(1).tokenType == TokenType::COM)
                        {
                            expect(TokenType::COM);
                            if (_lexer.peek(1).tokenType == TokenType::ID)
                            {
                                expect(TokenType::ID);
                                if (_lexer.peek(1).tokenType == TokenType::AO)
                                {
                                    expect(TokenType::AO);
                                    rightAssign();
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

bool parser::rightAssign()
{
    //rightAssign->expression | NL
    if (_lexer.peek(1).tokenType == TokenType::NL)
    {
        expect(TokenType::NL);
        return true;
    }
    else
    {
        if (expression() == true)
        {
            return true;
        }
    }
    rerutn false;
}

bool parser::if_()
{
    //if statement
    //if_->IF expression ro expression COL BEGIN statements END else_ statements
    if (_lexer.peek(1).tokenType == TokenType::IF)
    {
        expect(TokenType::IF);
        expression();
        relationalOperators();
        expression();
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
                    else_();
                    statements();
                    return true;
                }
            }
        }
    }
    return false;
}

bool parser::else_()
{
    //else_ -> ELSE BEGIN statements END | null
    if (_lexer.peek(1).tokenType == TokenType::ELSE)
    {
        expect(TokenType::ELSE);
        if (_lexer.peek(1).tokenType == TokenType::BG)
        {
            expect(TokenType::BG);
            statements();
            if (_lexer.peek(1).tokenType == TokenType::END)
            {
                expect(TokenType::END);
                return true;
            }
        }
    }
    else
    {
        return true;
    }
    return false;
}

bool parser::relationalOperators()
{
    //relational operators
    //ro->EQUALTO | LEQ | LESSTHAN | GEQ | GRTHAN | NOTEQUALS
    if (_lexer.peek(1).tokenType == TokenType::EQUALTO)
    {
        expect(TokenType::EQUALTO);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::LEQ)
    {
        expect(TokenType::LEQ);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::LESSTHAN)
    {
        expect(TokenType::LESSTHAN);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::GEQ)
    {
        expect(TokenType::GEQ);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::GRTHAN)
    {
        expect(TokenType::GRTHAN);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::NOTEQUALTO)
    {
        expect(TokenType::NOTEQUALTO);
        return true;
    }
    return false;
}

bool parser::funtionCall()
{
    //function call, general implementation that can send multiple parameters (optional in this assignment)
    //functionCall -> CALL ID id2 SCOL statements
    //id2 -> ID id3 | null
    //id3 -> COMMA id3 | null
}

bool parser::return_()
{
    //return_ -> RETURN value4
    if (_lexer.peek(1).tokenType == TokenType::RETURN)
    {
        expect(TokenType::RETURN);
        value4();
        return true;
    }
    return false;
}

bool parser::value4()
{
    //value4 -> ID SCOL | value SCOL
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        expect(TokenType::ID);
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            return true;
        }
    }
    else 
    {
        value();
        if (_lexer.peek(1).tokenType == TokenType::SCOL)
        {
            expect(TokenType::SCOL);
            return true;
        }
    }
    return false;
}