//============================================================================
// Name        : raig_client.cpp
// Author      : David Morton
// Version     :
// Copyright   : Copyright 2015
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../include/raig/raig.h"
// included for testing

// Game client for testing
int main(int argc, char* argv[])
{
	IRAIG *raig = new RAIG();
	raig->createMap();
	Position2D playerPos;

	// game loop
	while (raig->isAlive())
	{

		// update the raig client
		raig->update(argc, argv[1]);

		playerPos = raig->readData(3, 7);
		std::cout << "player position: " << playerPos.x << ":" << playerPos.y
				<< std::endl;
		raig->shutdown();

	}

	std::cout << "game over" << std::endl;

	delete raig;
	return 0;
}
