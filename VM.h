#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
using namespace std;



class VirtualMachine {

	struct symbol_table_element
	{
		string name;
		string type;
		int address;
		int value;
	};

	// opcode table is like this

	//        -----addresses-------
	// opcode destination src1  src2  hardcode_val 
	//   1        18       4     8
	enum class opcodes
	{
		PLUS = 0,
		MINUS,
		MUL,
		DIV,
		MOD,
		GREATERTHAN,
		GEQ,
		LESSTHAN,
		LEQ,
		EQUALTO,
		NEQ,
		GOTO,
		OUT,
		IN,
		COPYVAL,
		RET
	};


private:

	string file = "tac.txt";
	string symbol_tab = "symbol_table.txt";

	int pc;
	int** opcode_table;
	int opcode_table_length;
	int number_of_functions;
	vector<symbol_table_element> symbol_table;
	vector<int> value_table; // index will be taken as address
	int value_table_length;
	string* string_table;   // used to store strings


	bool isDigit(char c)
	{
		if (c == '0' || c == '1' || c == '2' || c == '4'
			|| c == '5' || c == '6' || c == '7' ||
			c == '8' || c == '9')
		{
			return true;
		}

		return false;
	}

	int get_ro_opcode(string ro)
	{
		if (ro == "==")
		{
			return int(opcodes::EQUALTO);
		}
		else if (ro == "<")
		{
			return int(opcodes::LESSTHAN);
		}
		else if (ro == "<=")
		{
			return int(opcodes::LEQ);
		}
		else if (ro == ">")
		{
			return int(opcodes::GREATERTHAN);
		}
		else if (ro == ">=")
		{
			return int(opcodes::GEQ);
		}
		else if (ro == "!=")
		{
			return int(opcodes::NEQ);
		}
	}

	int get_address_from_symbol_table(string key)
	{
		for (int i = 0; i < symbol_table.size(); i++)
		{
			if (symbol_table[i].name == key)
			{
				return symbol_table[i].address;
			}
		}

		return -1;
	}

	bool notOperand(char c)
	{
		if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%')
		{
			return false;
		}
		return true;
	}

	int get_operand_opcode(char c)
	{
		if (c == '+')
		{
			return int(opcodes::PLUS);
		}

		if (c == '-')
		{
			return int(opcodes::MINUS);
		}

		if (c == '/')
		{
			return int(opcodes::DIV);
		}

		if (c == '*')
		{
			return int(opcodes::MUL);
		}

		if (c == '%')
		{
			return int(opcodes::MOD);
		}
	}

	void generate_opcode_row(string temp, string start,int index, int line_number)
	{
		if (start == "out")
		{
			opcode_table[line_number][0] = int(opcodes::OUT);
			// check if its printing a value or a variable
			string val = "";
			bool string = 0;
			bool hardcoded_value = 0;
			if (temp[index] == '"')
			{
				string = true;
			}

			while (temp[index] != ';')
			{
				val = val + temp[index];
				index++;
			}

			if (string == true)
			{
				string_table[line_number] = val;
			}

			else
			{
				// get value through address
				int address = get_address_from_symbol_table(val);
				if (address == -1)
				{
					opcode_table[line_number][2] = stoi(val);
				}
				else
				{
					opcode_table[line_number][1] = address;
				}
				
				
			}


		}
		else if (start == "in")
		{
			opcode_table[line_number][0] = int(opcodes::IN);

			string val = "";
			while (temp[index] != ';')
			{
				val = val + temp[index];
				index++;
			}

			int num = get_address_from_symbol_table(val);
			// incase the in number is not in the symbol table, add it to the symbol table and derive its value and type
			if (num == -1)
			{
				int address = symbol_table[symbol_table.size() - 1].address;
				string type = symbol_table[symbol_table.size() - 1].type;

				symbol_table_element t;
				
				if (type == "CHAR")
				{
					t.address = address + 1;
				}
				else
					t.address = address + 4;
				
				t.name = val;


				// logic for type abhi nai kiya lol

				// push the newly created element in the symbol table;
				symbol_table.push_back(t);
				value_table.push_back(INT_MIN);

			}

			else
			{
				opcode_table[line_number][1] = num;
			}
		} 
		else if (start == "goto")
		{

			//  opcode destination src1  src2  hardcode_val 
			//   GOTO     jumpto   -    -        -

			opcode_table[line_number][0] = int(opcodes::GOTO);

			string jump_to = "";
			while (index < temp.size())
			{
				jump_to = jump_to + temp[index];
				index++;
			}

			int num = stoi(jump_to);

			opcode_table[line_number][1] = num;
		}
		else if (start == "if")
		{
			//  opcode destination  src1  src2  hardcode_val(lhs) hardcore_val(rhs)   
			//   RO      jump_to    l.hs  r.hs       -                  -

			
			
			string lhs = "";
			while (temp[index] != ' ')
			{
				lhs = lhs + temp[index];
				index++;
			}
			index++;

			if (isDigit(lhs[0]))
			{
				int num = stoi(lhs);
				opcode_table[line_number][4] = num;
			}
			else
			{
				opcode_table[line_number][2] = get_address_from_symbol_table(lhs);
			}

			string ro = "";
			while (temp[index] != ' ')
			{
				ro = ro + temp[index];
				index++;
			}
			index++;

			opcode_table[line_number][0] = get_ro_opcode(ro);


			string rhs = "";
			while (temp[index] != ' ')
			{
				rhs = rhs + temp[index];
				index++;
			}
			index++;


			if (isDigit(rhs[0]))
			{
				int num = stoi(rhs);
				opcode_table[line_number][5] = num;
			}
			else
			{
				opcode_table[line_number][3] = get_address_from_symbol_table(rhs);
			}

			while (temp[index] != ' ')
			{
				index++;
			}
			index++;

			string jump = "";
			while (index < temp.size())
			{
				jump = jump + temp[index];
				index++;
			}

			int num = stoi(jump);
			opcode_table[line_number][1] = num;
			

		}

		// return
		else if (start == "return")
		{
			
			opcode_table[line_number][0] = int(opcodes::RET);
			string var = "";

			while (temp[index] != ';')
			{
				var = var + temp[index];
				index++;
			}
			index++;

			if (isDigit(var[0]))
			{
				opcode_table[line_number][4] == stoi(var);
			}
			else
				opcode_table[line_number][2] = get_address_from_symbol_table(var);
	
		}

		else if (start == "call")
		{

		}

		else if (start == "param")
		{

		}
		// declaration
		else
		{
			string destination = start;
			// skip '='
		
			index++;
			index++;
			// get to the first variable

			string var1 = "";
			while (notOperand(temp[index]))
			{
				if (index == temp.size() || temp[index] == ';')
				{
					break;
				}
				var1 = var1 + temp[index];
				index++;
			}

			// if this condition is true, it means that there is no 2nd operand
			if (index == temp.size() || temp[index] == ';')
			{
				opcode_table[line_number][0] = int(opcodes::COPYVAL);
				opcode_table[line_number][1] = get_address_from_symbol_table(destination);
				if (isDigit(var1[0]))
				{
					//  opcode destination  src1  src2  hardcode_val(lhs) hardcore_val(rhs)   
					//   RO        start                  -         0     

					
					opcode_table[line_number][4] = stoi(var1);

				}
				else
				{
					opcode_table[line_number][2] = get_address_from_symbol_table(var1);
				}

			}
			else
			{
				// both elements
				char operand = temp[index];
				index++;

				string var2 = "";
				while (index != temp.size() && temp[index] != ';' )
				{
					var2 = var2 + temp[index];
					index++;
				}
				

				opcode_table[line_number][0] = get_operand_opcode(operand);
				opcode_table[line_number][1] = get_address_from_symbol_table(destination);
				if (isDigit(var1[0]))
				{
					//  opcode destination  src1  src2  hardcode_val(lhs) hardcore_val(rhs)   
					//   RO        start                  -         0     


					opcode_table[line_number][4] = stoi(var1);

				}
				else
				{
					opcode_table[line_number][2] = get_address_from_symbol_table(var1);
				}

				if (isDigit(var2[0]))
				{
					//  opcode destination  src1  src2  hardcode_val(lhs) hardcore_val(rhs)   
					//   RO        start                  -         0     


					opcode_table[line_number][5] = stoi(var2);

				}
				else
				{
					opcode_table[line_number][3] = get_address_from_symbol_table(var2);
				}


			}
		}

	}

	string get_symbol_type(string str)
	{
		int i = 0;
		while (str[i] != ' ')
		{
			i++;
		}

		i++;
		string type = "";
		
		while (str[i] != ' ')
		{
			type = type + str[i];
			i++;
		}

		return type;
	}

	int get_value_table_size()
	{
		number_of_functions = 0;
		ifstream fin;
		fin.open("symbol_table.txt");
		if (!fin)
		{
			cout << "Error in opening the symbol table file!" << endl;
			exit(-1);
		}

		int count = 0;
		string temp;
		while (temp != "END")
		{
			
			
			getline(fin, temp);
			if (get_symbol_type(temp) == "END")
			{
				return count;
			}

			if (get_symbol_type(temp) != "FUNC") {
				count++;
			}
			else
			{
				number_of_functions++;
			}
		}

		fin.close();
		return count;

	}

	void construct_value_table()
	{
		
		
	}

	int get_tac_lines()
	{
		ifstream fin;
		fin.open(file);
		if (!fin)
		{
			cout << "Error in opening the TAC file!" << endl;
			exit(-1);
		}

		int count = 0;
		string temp;
		while (!fin.eof())
		{
			string temp;
			getline(fin, temp);
			count++;
		}

		fin.close();
		return count;
	}

	// constructs the table
	void construct_opcode_table()
	{
		ifstream fin;
		fin.open(file);
		if (!fin)
		{
			cout << "Error in opening the TAC file!" << endl;
			exit(-1);
		}

		int i = 0;
		string line;
		while (i < opcode_table_length-1)
		{

			string temp;
			getline(fin, temp);  // read line from file

			int j = 0;
			while (temp[j] != ' ')
			{
				
				j++;
			}
			j++;
			string start = "";


			while (temp[j] != ' ')
			{
				start = start + temp[j];
				j++;
			}

			generate_opcode_row(temp,start, j+1, i);
			
			i++;
		}

	}

	// fills symbol table
	void fill_symbol_table()
	{
		ifstream fin;
		fin.open("symbol_table.txt");
		if (!fin)
		{
			cout << "Error in opening the symbol table file!" << endl;
			exit(-1);
		}

		string temp;
		int element_number = 0;
		int size = value_table_length + number_of_functions;

		string name;
		string type;
		int address;

		while (element_number < size)
		{
			fin >> name;
			fin >> type;
			fin >> address;
			symbol_table_element s;
			s.name = name;
			s.type = type;
			s.address = address;
			symbol_table.push_back(s);
			element_number++;
		}

		fin.close();
		

	}





	int get_index_from_address(int address)
	{
		int index = (address / 4) + (address % 4);
		return index;
	}



	

	void execute_copy();
	void execute_div();
	void execute_equalTo();
	void execute_geq();
	void execute_goto();
	void execute_greaterThan();
	void execute_in();
	void execute_leq();
	void execute_lessThan();
	void execute_minus();
	void execute_mod();
	void execute_mul();
	void execute_neq();
	void execute_out();
	void execute_plus();
	void execute_return();


	int get_value1();
	int get_value2();

public:
	// constructor
	VirtualMachine()
	{
		pc = 0;
 		opcode_table_length =  get_tac_lines();
		opcode_table = new int* [opcode_table_length];

		// initialize string table
		/* Maps 1-1 with the opcode table, stores strings where required
		* For example if pc = 3 and the tac is out "enter the number",
		* "enter the number will be stored at string_table[3].
		* 
		* */


		//        -----addresses-------                 ----strings stored in an another structure----
		// opcode destination src1  src2  hardcode_val                 string(if any)
		//   1        z       -     -          1                             -

		string_table = new string[opcode_table_length];
		 
		// initialize opcode_table
		for (int i = 0; i < opcode_table_length; i++)
		{
			opcode_table[i] = new int[6];
		}

		// initialize values by the lowest value possible
		for (int i = 0; i < opcode_table_length; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				opcode_table[i][j] = INT_MIN;
			}
		}

		// initialize symbol_table
 		value_table_length = get_value_table_size();

		for (int i = 0; i < value_table_length; i++)
		{
			value_table.push_back(INT_MIN);
		}

		// initialize symbol table as well 
		

		// fill symbol table
		fill_symbol_table();

		// fill opcode table
		construct_opcode_table();


		// initialize pc to the main func (bonus work)

	
	}


	void execute_code()
	{

		for (int i = 0; i < opcode_table_length; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				cout << opcode_table[i][j] << " ";
			}
			cout << string_table[i] << endl;
		}


	// opcode destination src1 src2 harcode1 hardcode2   string
		while (1)
		{
			switch (opcode_table[pc][0])
			{
				case int(opcodes::COPYVAL) :
				{
					execute_copy();
					break;
				}
				
				case int(opcodes::DIV) : 
				{
					execute_div();
					break;
				}

				case int(opcodes::EQUALTO) :
				{
					execute_equalTo();
					break;
				}

				case int(opcodes::GEQ) :
				{
					execute_geq();
					break;
				}

				case int(opcodes::GOTO) :
				{
					execute_goto();
					break;
				}

				case int(opcodes::GREATERTHAN):
				{
					execute_greaterThan();
					break;
				}

				case int(opcodes::IN):
				{
					execute_in();
					break;
				}

				case int(opcodes::LEQ) :
				{
					execute_leq();
					break;
				}

				case int(opcodes::LESSTHAN) :
				{
					execute_lessThan();
					break;
				}

				case int(opcodes::MINUS) :
				{
					execute_minus();
					break;
				}

				case int(opcodes::MOD) :
				{
					execute_mod();
					break;
				}

				case int(opcodes::MUL) :
				{
					execute_mul();
					break;
				}

				case int(opcodes::NEQ) :
				{
					execute_neq();
					break;
				}

				case int(opcodes::OUT) :
				{
					execute_out();
					break;
				}

				case int(opcodes::PLUS) :
				{
					execute_plus();
					break;
				}

				case int(opcodes::RET) :
				{
					execute_return();
					break;
				}
				default:
					break;
				
			}
		}
		
	}



	

};