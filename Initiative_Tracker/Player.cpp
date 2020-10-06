#include "Player.h"

void Player::setName(std::string input)
{
	name = input;
}

std::string Player::getName()
{
	return name;
}
void Player::setInit(int input)
{
	initiative = input;
}

int Player::getInit()
{
	return initiative;
}

void Player::pprint()
{
	int x = order;
	x++;
	std::string s = std::to_string(x);
	s += ".) ";

	std::cout << s;

	std::cout << name << ": ";

	std::cout << std::to_string(initiative);
}

void Player::setOrder(int i)
{
	order = i;
}

int Player::getOrder()
{
	return order;
}