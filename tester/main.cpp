#include <iostream>
#include "Any.hpp"

int main()
{
	CQue::Any a = std::string("Bruh, lol!");
	std::string bruh = a.Release<std::string>();
	std::cout << bruh << std::endl;
	std::cout << a.IsEmpty() << std::endl;
	return 0;
}