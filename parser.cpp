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

}

bool parser::parameters2()
{

}

bool parser::codeBlock()
{

}

bool parser::statements()
{

}

bool parser::declare()
{

}

bool parser::declare2()
{

}

bool parser::initializer()
{

}

bool parser::expression()
{

}

bool parser::expression2()
{

}

bool parser::t()
{

}

bool parser::t2()
{

}

bool parser::f()
{

}

bool parser::value()
{

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

bool parser::return()
{

}