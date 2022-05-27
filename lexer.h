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
	ERROR,//keywords
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
	EQUALTO,      // relational operators
	LEQ,
	LESSTHAN,
	GEQ,
	GRTHAN,
	NOTEQUALTO,
	AO,      // assignment operator			//arithmetic operators
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
	END,     // end of block '}'             // special characters
	COM,     //,
	HASH,    //#
	COL,     // :
	SCOL,     // ;
	LPARAN,
	RPARAN
};
//structure of a token 
struct token
{
public:
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


public:

	vector<token> return_token()
	{
		return tokens;
	}

	
	bool notKeyWord(char c)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			if (c != 'f' && c != 'i' && c != 'c' && c != 'e' && c != 'p' && c != 'r' && c != 'b')
			{
				return true;
			}
		}

		return false;
	}
	bool isDigit(char c)
	{
		if (c >= '0' && c <= '9')
		{
			return true;
		}
		return false;
	}
	bool isChar(char c)
	{
		if ((c >= 'a' && c <= 'z') || ((c >= 'A' && c <= 'Z')))
		{
			return true;
		}
		return false;
	}


	bool isOther(char c) {
		//if (c < 'A' || c > 'z') {
		//	return true;
		//}
		//else return false;

		if (isDigit(c) || isChar(c))
		{
			return false;
		}


		return true;
	}
	token checkIdentifier(vector<char>::iterator& itr)
	{
		string lexeme = "";
		//lexeme = lexeme + *itr;

		int state = 62;
		switch (state) 
		{
		case 62:
			while (1)
			{
				if (isOther(*(itr))) {
					//itr++;

					return token(lexeme, TokenType::ID);
				}
				else
					lexeme = lexeme + *itr;
					itr++;
				
			}
			break;

		}

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
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 2:
				if (*(++itr) == 'n') {
					lexeme = lexeme + 'n';
					state = 3;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 3:
				if (*(++itr) == 'c') {
					state = 4;
					lexeme = lexeme + 'c';
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 4:
				if (isOther(*(++itr))) {
					return token("func", TokenType::FUNC);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 22:
				if (*(++itr) == 'r') {
					lexeme = lexeme + 'r';
					state = 23;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 23:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::FOR);
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 62:
				while (1)
				{
					if (isOther(*(itr))) {
						//itr++;
						return token(lexeme, TokenType::ID);
					}
					else
						lexeme = lexeme + *itr;
						itr++;
					
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
					state = 6;
				}
				else if (*(itr) == 'f') {
					lexeme = lexeme + 'f';
					state = 15;
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 6:

				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::IN);
				}

				else if (*(itr) == 't') {
					lexeme = lexeme + 't';
					state = 7;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 7:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::INT);
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 15:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::IF);
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;


			case 62:
				while (1)
				{
					if (isOther(*(itr))) {
						//itr++;
						return token(lexeme, TokenType::ID);
					}
					else
						lexeme = lexeme + *itr;
						itr++;
						
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
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 9:
				if (*(++itr) == 'a') {
					lexeme = lexeme + 'a';
					state = 10;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 10:
				if (*(++itr) == 'r') {
					lexeme = lexeme + 'r';
					state = 11;
				}
				
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 11:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::CHAR);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 12:
				if (*(++itr) == 'l') {
					lexeme = lexeme + 'l';
					state = 13;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 13:
				if (*(++itr) == 'l') {
					lexeme = lexeme + 'l';
					state = 14;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 14:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::CALL);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 62:
				while (1)
				{
					if (isOther(*(itr))) {
						//itr++;
						return token(lexeme, TokenType::ID);
					}
					else
						lexeme = lexeme + *itr;
						itr++;
					
				}
				break;
			}
		}
	}
	token checkE(vector<char>::iterator& itr)
	{
		string lexeme = "e";
		int state = 16;
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
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
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
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 18:
				if (*(++itr) == 'f') {
					lexeme = lexeme + 'f';
					state = 19;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 19:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::ELIF);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 20:
				if (*(++itr) == 'e') {
					lexeme = lexeme + 'e';
					state = 21;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 21:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::ELSE);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 56:
				if (*(++itr) == 'd') {
					lexeme = lexeme + 'd';
					state = 57;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 57:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::END);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 62:
				while (1)
				{
					if (isOther(*(itr))) {
						//itr++;
						return token(lexeme, TokenType::ID);
					}
					else
						lexeme = lexeme + *itr;
						itr++;
					
				}
				break;
			}
		}
	}
	token checkP(vector<char>::iterator& itr)
	{
		string lexeme = "p";
		int state = 24;
		while (1) {
			switch (state) {
			case 24:
				if (*(++itr) == 'r') {
					lexeme = lexeme + 'r';
					state = 25;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 25:
				if (*(++itr) == 'i') {
					lexeme = lexeme + 'i';
					state = 26;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 26:
				if (*(++itr) == 'n') {
					lexeme = lexeme + 'n';
					state = 27;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 27:
				if (*(++itr) == 't') {
					lexeme = lexeme + 't';
					state = 28;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 28:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::PRINT);
				}
				else if (*(itr) == 'l') {
					lexeme = lexeme + 'l';
					state = 29;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 29:
				if (*(++itr) == 'n') {
					lexeme = lexeme + 'n';
					state = 30;
				}

				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 30:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::PRINTLN);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 62:
				while (1)
				{
					if (isOther(*(itr))) {
						//itr++;
						return token(lexeme, TokenType::ID);
					}
					else
						
					lexeme = lexeme + *itr;
					itr++;
				}
				break;
			}
		}
	}
	token checkR(vector<char>::iterator& itr)
	{
		string lexeme = "r";
		int state = 31;
		while (1) {
			switch (state) {
			case 31:
				if (*(++itr) =='e'){
					lexeme = lexeme + 'e';
					state = 32;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 32:
				if (*(++itr) == 't') {
					lexeme = lexeme + 't';
					state = 33;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 33:
				if (*(++itr) == 'u') {
					lexeme = lexeme + 'u';
					state = 34;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 34:
				if (*(++itr) == 'r') {
					lexeme = lexeme + 'r';
					state = 35;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 35:
				if (*(++itr) == 'n') {
					lexeme = lexeme + 'n';
					state = 36;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 36:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::RETURN);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 62:
				while (1)
				{
					if (isOther(*(itr))) {
						//itr++;
						return token(lexeme, TokenType::ID);
					}
					else
						lexeme = lexeme + *itr;
						itr++;
					
				}
				break;
			}
		}
	}
	token checkB(vector<char>::iterator& itr)
	{
		string lexeme = "b";
		int state = 51;
		while (1) {
			switch (state) {
			case 51:
				if (*(++itr) == 'e') {
					lexeme = lexeme + 'e';
					state = 52;
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 52:
				if (*(++itr) == 'g') {
					lexeme = lexeme + 'g';
					state = 53;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 53:
				if (*(++itr) == 'i') {
					lexeme = lexeme + 'i';
					state = 54;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 54:
				if (*(++itr) == 'n') {
					lexeme = lexeme + 'n';
					state = 55;
				}
				else {
					lexeme = lexeme + *itr;
					state = 62;
				}
				break;
			case 55:
				if (isOther(*(++itr))) {
					return token(lexeme, TokenType::BG);
				}
				else {
					//lexeme = lexeme + *itr;
					state = 62;
				}
				break;

			case 62:
				while (1)
				{
					if (isOther(*(itr))) {
						//itr++;
						return token(lexeme, TokenType::ID);
					}
					else
						lexeme = lexeme + *itr;
						itr++;
					
				}
				break;
			}
		}
	}
	token checkCharLiteral(vector<char>::iterator& itr)
	{
		itr++;
		string lexeme = "\'";
		if (isChar(*itr)) {
			lexeme = lexeme + *itr;
			itr++;
			
			if (*itr == '\'')
			{
				lexeme = lexeme + *itr;
				itr++;
				return token(lexeme, TokenType::CL);
			}
			else
				return token(lexeme, TokenType::ERROR);
		}
	}
	token checkLessThan(vector<char>::iterator& itr)
	{
		itr++;
		if (*itr == '-')
		{
			itr++;
			return token("<-", TokenType::AO);
		}

		else if (*itr == '=')
		{
			itr++;
			return token("<=", TokenType::LEQ);

		}
		else
			return token("<", TokenType::LESSTHAN);
	}
	token checkGreaterThan(vector<char>::iterator& itr) {
		itr++;

		if (*itr == '=')
		{
			itr++;
			return token(">=", TokenType::GEQ);

		}
		else
			return token(">", TokenType::GRTHAN);
	}
	token checkString(vector<char>::iterator& itr)
	{
		string lexeme = "";
		lexeme = lexeme + *itr;
		itr++;
		lexeme = lexeme + *itr;
		while (*itr != '\"')
		{

			itr++;
			lexeme = lexeme + *itr;
		}
		
		//lexeme = lexeme + *itr;
		itr++;
		return token(lexeme, TokenType::STR);

	}
	token checkDigit(vector<char>::iterator& itr) {
		string lexeme = "";
		lexeme = lexeme + *itr;
		while (1)
		{
			itr++;
			if (isDigit(*itr)) {
				lexeme = lexeme + *itr;
				continue;
			}
			else {
				return token(lexeme, TokenType::NL);
			}
		}
	}
	token getToken(char it, vector<char>:: iterator & itr) {
		
		//token a;
		//int s = tokens.size();
		//if (s != 0) {
		//	 a = tokens[s - 1];
		//}
		////if (a.tokenType == TokenType::HASH) {
		////	string lexeme = "";
		////	lexeme = lexeme + *itr;
		////	while (*(++itr) != '\n') {
		////		lexeme = lexeme + *itr;
		////	}
		////	return token(lexeme, TokenType::CMNT);
		////}
		//else 
		while(1)
		{
			if (it == ' ' || it == '\n' || it == '\t')
			{
				while (it == ' ' || it == '\n' || it == '\t') {
					++itr;
					it = *itr;
				}
			}

			// keywords and identifiers
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
				itr++;
				return token("+", TokenType::PLUS);
			}
			if (it == '-') {
				itr++;
				return token("-", TokenType::MINUS);
			}
			if (it == '/') {
				itr++;
				return token("/", TokenType::DIV);
			}
			if (it == '*') {
				itr++;
				return token("*", TokenType::MUL);
			}
			if (it == '%') {
				itr++;
				return token("%", TokenType::MOD);
			}
			//ro
			if (it == '<') {
				return checkLessThan(itr);
			}
			if (it == '>') {
				return checkGreaterThan(itr);
			}
			if (it == '=') {
				itr++;
				return token("=", TokenType::EQUALTO);
			}
			//special characters
			if (it == ',') {
				itr++;
				return token(",", TokenType::COM);
			}
			if (it == '#') {
				while (*itr != '\n')
				{
					itr++;
				}
				itr++;
				it = *itr;
				//return token("#", TokenType::HASH);
			}
			if (it == ';') {
				itr++;
				return token(";", TokenType::SCOL);
			}
			if (it == ':') {
				itr++;
				return token(":", TokenType::COL);
			}
			// Identifier
			if (notKeyWord(it))
			{
				return checkIdentifier(itr);
			}

			// numeric literal
			if (isDigit(it)) {
				return checkDigit(itr);
			}

			// character Literal
			if (it == '\'') {
				return checkCharLiteral(itr);
			}
			// string
			if (it == '\"') {
				return checkString(itr);
			}

			if (it == EOF)
			{
				return token("", TokenType::END_OF_FILE);
			}

			if(it == '~')
			{
				itr++;
				if (*itr == '=')
				{
					return token("~=", TokenType::NOTEQUALTO);
				}
				else
				{
					itr++;
					string lexeme = "";
					lexeme = lexeme + *itr;
					return token(lexeme, TokenType:: ERROR );
				}
			}
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
