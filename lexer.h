#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

//all your tokens goes here
enum class TokenType
{
	END_OF_FILE = 0,
	ERROR,
	//keywords
	FUNC,
	INT,
	CHAR,
	CALL,
	IF,
	ELIF,
	ELSE,
	FOR,
	PRINT,
	PRINTLN,
	RETURN,
	IN,
	RO,      // relational operators
	AO,      // assignment operator
	//arithmetic operators
	PLUS,    // +
	MINUS,   // -
	DIV,     // /
	MUL,     // *
	MOD,     // %
	ID,      // identifier
	NL,      // numeric literal
	CL,      // character literal
	STR,     // string
	CMNT,    // comment
	BG,      // beginning of block '{'
	END,     // end of block '}'
    // special characters
	COM,     //,
	HASH,    //#
	COL,     // :
	SCOL     // ;

};
//structure of a token 
struct token
{
	string lexeme;
	TokenType tokenType;//enum type
	//constructor
	token(string lexeme, TokenType tokenType);
	//constructor default
	token();
	void Print();
};
class lexer
{
	vector<char> stream;  //used for storing file sample_code.cc content
	vector<token> tokens; //vector to store all (tokens,lexeme) pairs
	void Tokenize();//populates tokens vector
	int index;


	token getToken(char it) {

		//ao
		if (it == '+') {
			return token("+", TokenType::PLUS);
		}
		if (it == '-') {
			return token("-", TokenType::MINUS);
		}
		if (it == '/') {
			return token("/", TokenType::DIV);
		}
		if (it == '*') {
			return token("*", TokenType::MUL);
		}
		//ro
		if (it == '<') {
			return token("<", TokenType::RO);
		}
		if (it == '>') {
			return token(">", TokenType::RO);
		}
		if (it == '=') {
			return token("=", TokenType::RO);
		}
		//special characters
		if (it == ',') {
			return token(",", TokenType::COM);
		}
		if (it == '#') {
			return token("#", TokenType::HASH);
		}
		if (it == ';') {
			return token(";", TokenType::SCOL);
		}
		if (it == ':') {
			return token(":", TokenType::COL);
		}
	}

public:
	lexer();
	lexer(const char filename[]);
	void printRaw();//just print everything as it is present in file
	token getNextToken();//get next token
	void resetPointer();//reset pointer to start getting tokens from start
	int getCurrentPointer();//get where current pointer in tokens vector is
	void setCurrentPointer(int pos);//move current pointer to wherever
	token peek(int);//peek the next token
};

#endif // !_LEXER_H
