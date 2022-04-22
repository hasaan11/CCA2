#ifndef _PARSER_H_
#define _PARSER_H_
#include "lexer.h"
//for future assignments leave it as it is
class parser
{
    lexer _lexer;
    vector<token> parse_table;
public:
    void syntax_error();
    token expect(TokenType expected_type);
    void create_symbol_table();
    parser(const char filename[]);
    void readAndPrintAllInput();
    void resetPointer();
    /*Terminal functions goes here use peek and expect*/
    /*use TokenType:: for token names for example
        expect(TokenType::ASSIGN);   //example function call
      */
	//all your parser function goes here
    bool start();
    bool datatype();
    bool parameters();
    bool parameters2();
    bool codeBlock();
    bool statements();
    bool declare();
    bool declare2();
    bool initializer();
    bool value2();
    bool expression();
    bool expression2();
    bool t();
    bool t2();
    bool f();
    bool value();
    bool input();
    bool print();
    bool println();
    bool value3();
    bool loop();
    bool rightAssign();
    bool if_();
    bool else_();
    bool relationalOperators();
    bool funtionCall();
    bool return_();
    bool value4();
};
#endif
