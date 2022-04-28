// Requires C++ 17
/** @file */
#include <iostream>
#include <boost/asio.hpp>
#include "game.h"

using namespace raw;

/**
 * Starts a new game, handling any exceptions.
 *
 * @see raw::Game()
 */
void NewGame();

int main()
{
	NewGame();

	return 0;
}

void NewGame()
{
	try
	{
		raw::Game NewGame;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "An error happened: " << ex.what() << std::endl;
		std::cin.ignore();
	}
}
