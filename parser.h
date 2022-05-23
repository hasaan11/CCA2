#ifndef _PARSER_H_
#define _PARSER_H_
#include "lexer.h"
//for future assignments leave it as it is
class parser
{

    lexer _lexer;
    vector<token> parse_table;
    vector<string> tacs;
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
    bool initializer(string & n);
    bool value2(string & n);
    bool expression(string & n);
    bool expression2(string & n);
    bool t(string & n);
    bool t2(string & n);
    bool f(string & n);
    bool value(string & n);
    bool input();
    bool print();
    bool println();
    bool value3(string & n);
    bool loop();
    bool rightAssign(string & n);
    bool if_();
    bool else_();
    bool elif();
    bool relationalOperators(string& n);
    bool functionCall();
    bool return_();
    bool value4(string & n);
    bool id2(string & n);
    bool id3(string & n);
    void get_tacs();
    void write_tac_to_file();
};
#endif
