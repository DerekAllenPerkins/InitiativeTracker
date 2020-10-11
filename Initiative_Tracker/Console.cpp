#include "Console.h"

void Console::print(std::string output)
{
	std::cout << output;
	std::cout.flush();
}

std::string Console::getString(std::string prompt)
{
	std::string input;

	std::cout << prompt;

	std::getline(std::cin, input);

	std::cout.flush();

	return input;
}

int Console::getInt(std::string prompt)
{
	int i = 0;
	std::string input = "";

	std::cout << prompt;
	std::cin >> input;

	try
	{
		i = std::stoi(input);					// Casts string to int
	}
	catch (std::exception e)
	{
		std::string error = e.what();			// Stores error message for debugging
		//std::cout << error;
		i = -1;									// -1 is the error value
		return i;								// return error
		
	}
	std::cout.flush();
	std::cin.ignore();

	return i;									// return value
}