#ifndef H_Console
#define H_Console
#endif

/*
	Written by Derek Perkins
			-----
	Date:		October 5, 2020
	Purpose:	Simplify C++ console input and output.
			-----
	Ported from Murach's Java Programming: 5th Edition
*/

#include <iostream>
#include <string>

struct Console
{
	void print(std::string output);

	std::string getString(std::string prompt);

	int getInt(std::string prompt);
};

