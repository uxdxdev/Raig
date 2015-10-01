//============================================================================
// Name        : raig_client.cpp
// Author      : David Morton
// Version     :
// Copyright   : Copyright 2015
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../RAIG/raig.h"

// Game client for testing
int main() {
	IRAIG *raig = new RAIG();
	raig->createMap();

	// game loop
	while(raig->isAlive())
	{
		// update the raig client
		raig->update();
	}

	delete raig;
	return 0;
}
