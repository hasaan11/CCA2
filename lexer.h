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

	bool isOther(char c) {
		if (c < 'A' || c > 'z') {
			return true;
		}
		else return false;
	}

	token checkF(vector<char>::iterator& itr)
	{
		string lexeme = "f";
		int state = 1;
		while (1) {
			switch (state) {
			case 1:
				if (*(++itr) == 'u') {
					lexeme = lexeme + 'u';
					state = 2;
				}
				else if (*(itr) == 'o') {
					lexeme = lexeme + 'o';
						state = 22;
				}
				else
					state = 62;
				break;
			case 2:
				if (*(++itr) == 'n') {
					lexeme = lexeme + 'u';
					state = 3;
				}
				else
					state = 62;
				break;
			case 3:
				if (*(++itr) == 'c') {
					state = 4;
					lexeme = lexeme + 'c';
				}
				else
					state = 62;
				break;
			case 4:
				if (isOther(*(++itr))) {
					return token("func", TokenType::FUNC);
				}
				else
					state = 62;
				break;
			case 22:
				if (*(++itr) == 'r') {
					lexeme = lexeme + 'r';
					state = 23;
				}
				else
					state = 62;
				break;
			case 23:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::FOR);
				}
				else
					state = 62;
				break;
			case 62:
				while (1)
				{
					if (isOther(*(++itr))) {
						return token(lexeme, TokenType::ID);
					}
				}
				break;
			}
		}
	}
	token checkI(vector<char>::iterator& itr)
	{
		string lexeme = "i";
		int state = 5;
		while (1) {
			switch (state) {
			case 5:
				if (*(++itr) == 'n') {
					lexeme = lexeme + 'n';
					state = 2;
				}
				else if (*(itr) == 'f') {
					lexeme = lexeme + 'f';
					state = 15;
				}
				else
					state = 62;
				break;
			case 6:
				if (*(++itr) == 't') {
					lexeme = lexeme + 't';
					state = 7;
				}
				else
					state = 62;
				break;
			case 7:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::INT);
				}
				else
					state = 62;
				break;

			case 15:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::IF);
				}
				else
					state = 62;
				break;
			case 62:
				while (1)
				{
					if (isOther(*(++itr))) {
						return token(lexeme, TokenType::ID);
					}
				}
				break;
			}
		}
	}
	token checkC(vector<char>::iterator& itr)
	{
		string lexeme = "c";
		int state = 8;
		while (1) {
			switch (state) {
			case 8:
				if (*(++itr) == 'h') {
					lexeme = lexeme + 'h';
					state = 9;
				}
				else if (*(itr) == 'a') {
					lexeme = lexeme + 'a';
					state = 12;
				}
				else
					state = 62;
				break;
			case 9:
				if (*(++itr) == 'a') {
					lexeme = lexeme + 'a';
					state = 10;
				}
				else
					state = 62;
				break;
			case 10:
				if (*(++itr) == 'r') {
					lexeme = lexeme + 'r';
					state = 11;
				}
				else
					state = 62;
				break;

			case 11:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::CHAR);
				}
				else
					state = 62;
				break;

			case 12:
				if (*(++itr) == 'l') {
					lexeme = lexeme + 'l';
					state = 13;
				}
				else
					state = 62;
				break;

			case 13:
				if (*(++itr) == 'l') {
					lexeme = lexeme + 'l';
					state = 7;
				}
				else
					state = 62;
				break;

			case 14:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::CALL);
				}
				else
					state = 62;
				break;

			case 62:
				while (1)
				{
					if (isOther(*(++itr))) {
						return token(lexeme, TokenType::ID);
					}
				}
				break;
			}
		}
	}
	token checkE(vector<char>::iterator& itr)
	{
		string lexeme = "e";
		int state = 8;
		while (1) {
			switch (state) {
			case 16:
				if (*(++itr) == 'l') {
					lexeme = lexeme + 'l';
					state = 17;
				}
				else if (*(itr) == 'n') {
					lexeme = lexeme + 'n';
					state = 56;
				}
				else
					state = 62;
				break;
			case 17:
				if (*(++itr) == 'i') {
					lexeme = lexeme + 'i';
					state = 18;
				}
				else if (*(itr) == 's') {
					lexeme = lexeme + 's';
					state = 20;
				}
				else
					state = 62;
				break;
			case 18:
				if (*(++itr) == 'f') {
					lexeme = lexeme + 'f';
					state = 19;
				}
				else
					state = 62;
				break;

			case 19:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::ELIF);
				}
				else
					state = 62;
				break;

			case 20:
				if (*(++itr) == 'e') {
					lexeme = lexeme + 'e';
					state = 13;
				}
				else
					state = 62;
				break;

			case 21:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::ELSE);
				}
				else
					state = 62;
				break;

			case 56:
				if (*(++itr) == 'd') {
					lexeme = lexeme + 'd';
					state = 57;
				}
				else
					state = 62;
				break;

			case 57:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::END);
				}
				else
					state = 62;
				break;

			case 62:
				while (1)
				{
					if (isOther(*(++itr))) {
						return token(lexeme, TokenType::ID);
					}
				}
				break;
			}
		}
	}
	token checkP(vector<char>::iterator& itr)
	{

	}
	token checkR(vector<char>::iterator& itr)
	{

	}
	token checkB(vector<char>::iterator& itr)
	{
	
	}
	token getToken(char it, vector<char>:: iterator & itr) {

	
		if (it == '+') {
			return token("+", TokenType::PLUS);
		}
		// keyowrds and identifiers
		if (it == 'f') {
			return checkF(itr);
		}
		if (it == 'i')
		{
			return checkI(itr);
		}
		if (it == 'c')
		{
			return checkC(itr);
		}
		if (it == 'e')
		{
			return checkE(itr);
		}
		if (it == 'p')
		{
			return checkP(itr);
		}
		if (it == 'r')
		{
			return checkR(itr);
		}
		if (it == 'b')
		{
			return checkB(itr);
		}
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
