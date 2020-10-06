#ifndef H_Player
#define H_Player
#endif

#include <string>
#include "Console.h"

class Player
{
private:
	std::string name;
	int initiative;
	int order;

public:
	void setName(std::string input);
	std::string getName();

	void setInit(int input);
	int getInit();

	void setOrder(int i);
	int getOrder();

	void pprint();							// 'Player Print'. Builds a string to display in the gameLoop() function.
};

