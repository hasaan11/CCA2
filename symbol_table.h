#pragma once
#include <iostream>
using namespace std;
class symbol_table_element
{
public:
	string lexeme; // name of identifier
	string type;  // 1 for int, 2 for char



	symbol_table_element()
	{
		lexeme = "";
		type = -1;
	}

	symbol_table_element(string lex, string t)
	{
		lexeme = lex;
		type = t;
	}

};