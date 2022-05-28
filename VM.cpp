#pragma once

#include "VM.h"
#include <iostream>
using namespace std;


int VirtualMachine::get_value1()
{
	int val;
	if (opcode_table[pc][2] == INT_MIN)
	{
		val = opcode_table[pc][4];
	}
	else
	{
		int src_address = opcode_table[pc][2];
		int i = get_index_from_address(src_address);
		val = value_table[i];
	}
	return val;
}


int VirtualMachine::get_value2()
{
	int val;
	if (opcode_table[pc][3] == INT_MIN)
	{
		val = opcode_table[pc][5];
	}
	else
	{
		int src1_adress = opcode_table[pc][3];
		int index = get_index_from_address(src1_adress);
		val = value_table[index];

	}

	return val;
}


void VirtualMachine::execute_copy()
{
	int address = opcode_table[pc][1];
	int dest_index = get_index_from_address(address);


	int val = get_value1();
	// update value in value table
	value_table[dest_index] = val;
	pc++;
}

void VirtualMachine::execute_div()
{
	int address = opcode_table[pc][1];
	int dest_index = get_index_from_address(address);

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	value_table[dest_index] = val1 / val2;
	pc++;
}

void VirtualMachine::execute_equalTo()
{
	int jump = opcode_table[pc][1];

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	if (val1 == val2) {
		pc = jump;
	}
	else
	{
		pc++;
	}
}

void VirtualMachine::execute_geq()
{
	int jump = opcode_table[pc][1];

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	if (val1 >= val2) {
		pc = jump;
	}
	else
	{
		pc++;
	}
}

void VirtualMachine::execute_goto()
{
	int jump = opcode_table[pc][1];
	pc = jump;
}

void VirtualMachine::execute_greaterThan()
{
	int jump = opcode_table[pc][1];

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	if (val1 > val2) {
		pc = jump;
	}
	else
	{
		pc++;
	}

}

void VirtualMachine::execute_in()
{
	int address = opcode_table[pc][1];
	int index = get_index_from_address(address);

	cin >> value_table[index];
	pc++;

}

void VirtualMachine::execute_leq()
{
	int jump = opcode_table[pc][1];

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	if (val1 <= val2) {
		pc = jump;
	}
	else
	{
		pc++;
	}

}

void VirtualMachine::execute_lessThan()
{
	int jump = opcode_table[pc][1];

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	if (val1 < val2) {
		pc = jump;
	}
	else
	{
		pc++;
	}
}

void VirtualMachine::execute_minus()
{
	int address = opcode_table[pc][1];
	int dest_index = get_index_from_address(address);

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	value_table[dest_index] = val1 - val2;
	pc++;
}

void VirtualMachine::execute_mod()
{
	int address = opcode_table[pc][1];
	int dest_index = get_index_from_address(address);

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	value_table[dest_index] = val1 % val2;
	pc++;
}

void VirtualMachine::execute_mul()
{
	int address = opcode_table[pc][1];
	int dest_index = get_index_from_address(address);

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	value_table[dest_index] = val1 * val2;
	pc++;

}

void VirtualMachine::execute_neq()
{
	int jump = opcode_table[pc][1];

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	if (val1 != val2) {
		pc = jump;
	}
	else
	{
		pc++;
	}
}

void VirtualMachine::execute_out()
{
	// print variable
	if (opcode_table[pc][1] != INT_MIN)
	{
		int address = opcode_table[pc][1];
		int index = get_index_from_address(address);
		cout << value_table[index];
	}

	// NL/ Character Literal
	else if (opcode_table[pc][2] != INT_MIN)
	{
		cout << opcode_table[pc][2];
	}

	// print string
	else
	{
		string src = string_table[pc];
		string str = src.substr(1,src.size() - 2);
		//cout << str;
		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] == '\\' && str[i+1] == 'n')
			{
				cout << endl;
				i++;
			}
			else
				cout << str[i];
		}
	}

	pc++;
}


void VirtualMachine::execute_plus()
{
	int address = opcode_table[pc][1];
	int dest_index = get_index_from_address(address);

	int val1, val2;
	val1 = get_value1();
	val2 = get_value2();

	value_table[dest_index] = val1 + val2;
	
	pc++;
}



void VirtualMachine::execute_return()
{
	
}
