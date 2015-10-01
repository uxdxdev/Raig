//============================================================================
// Name        : raig_client.cpp
// Author      : David Morton
// Version     :
// Copyright   : Copyright 2015
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../include/raig/raig.h"

// Game client for testing
int main() {
	IRAIG *raig = new RAIG();
	raig->createMap();
	Position2D playerPos;
	// game loop
	while(raig->isAlive())
	{
		// update the raig client
		raig->update();
		playerPos = raig->readData(3, 7);
		std::cout << "player position: " << playerPos.x << ":" << playerPos.y << std::endl;
	}

	delete raig;
	return 0;
}
