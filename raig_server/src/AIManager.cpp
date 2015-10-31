/*
 * game.cpp
 *
 *  Created on: 4 Oct 2015
 *      Author: david
 */

#include "../includes/AIManager.h"
#include "../includes/Definitions.h"
#include <syslog.h> // syslog()
#include <string.h> // strlen()
#include <unistd.h> // gethostname(), write()
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "../../libraig/Raig.h"

extern "C" {
	#include "../../libsocket/Sockets.h"
}

using namespace raig;

void processRequest(int in, int out) {

	printf("processing request from client...\n");

	Packet packetIn;
	Packet packetOut;
	packetOut.x = 0;
	packetOut.y = 0;

	// Read request from the client and store in packet struct
	Read(in, &packetIn, sizeof(Packet));
	printf("Server read() ok (%d, %d, %s)\n", packetIn.x, packetIn.y, packetIn.stringData);

	// Calculate results of the client request
	printf("Calculating results..\n");
	packetOut.x = packetIn.x * 3;
	packetOut.y = packetIn.y * 2;
	sprintf(packetOut.stringData, "%s %s", packetIn.stringData, "with some added content from the server");

	// Send the results to the client using a packet struct
	Write(out, &packetOut, sizeof(Packet));
	printf("Server write() ok (%d, %d, %s)\n", packetOut.x, packetOut.y, packetOut.stringData);
}



