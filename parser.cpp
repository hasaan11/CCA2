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
    declare();
    input();
    print();
    loop();
    if_();
    functionCall();
    return_();
    //return true;
//return false;
}

bool parser::statements()
{
    //general parts of the sample codes
    //statements->declare | input | print | loop | if | functionCall | return_ | null
    declare();
    input();
    print();
    loop();
    if_();
    functionCall();
    return_();
    //else null return true;
 //return false;
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
    t();
    expression2();
    //return???
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
    f();
    t2();
    //return??
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

}

bool parser::print()
{

}

bool parser::loop()
{

}

bool parser::rightAssign()
{

}

bool parser::if_()
{

}

bool parser::else_()
{

}

bool parser::relationalOperators()
{

}

bool parser::funtionCall()
{

}

bool parser::return_()
{

}