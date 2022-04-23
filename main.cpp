#include <iostream>
#include "parser.h"
using namespace std;

int main(int argc, char* argv[])
{
	cout << argv[1];
	//asking for file name as command line arguments
	if (argc == 2)
	{
		//lexer _lexer("sample_code0.mc");
		//token t;
		//t = _lexer.getNextToken();
		//while (t.tokenType != TokenType::END_OF_FILE)
		//{
		//	t.Print();
		//	t = _lexer.getNextToken();
		//	
		//}
		parser Parser(argv[1]);
		cout << "********************** Parser Starts from here **********************" << endl;
		// Parser Starts
		Parser.start();
		// Parser creates symbol table
		Parser.create_symbol_table();
	

	}
	else if (argc > 2)
	{ //argument limit exceeds
		cout << "Too many arguments" << endl;
	}
	else //if file name is'nt given
	{
		cout << "Please provide a file name" << endl;
	}
	system("Pause");
	return 0;
}
