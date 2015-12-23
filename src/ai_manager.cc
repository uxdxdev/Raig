#include "../include/ai_manager.h"

extern "C" {
	#include "../external/libsocket/include/socket.h"
}

using namespace raig;

void AIManager::ProcessRequest(int in, int out)
{

	printf("processing request from client...\n");

	Packet packetIn;
	Packet packetOut;
	InitializePacket(&packetIn);
	InitializePacket(&packetOut);

	while(packetIn.completeFlag == 0)
	{
		// Read request from the client and store in packet struct
		Read(in, &packetIn, sizeof(Packet));
		printf("Packet received from client :\nx %d\ny %d\nexit %d\nstring %s\n", packetIn.x, packetIn.y, packetIn.completeFlag, packetIn.stringData);

		// Calculate results of the client request
		printf("Calculating results..\n");
		packetOut.x += packetIn.x;
		packetOut.y += packetIn.y;
		sprintf((char*)packetOut.stringData, "%s", (char*)packetIn.stringData);
		packetOut.completeFlag = packetIn.completeFlag;

		// Send the results to the client using a packet struct
		Write(out, &packetOut, sizeof(Packet));
		printf("Packet sent to client :\nx %d\ny %d\nexit %d\nstring %s\n", packetOut.x, packetOut.y, packetOut.completeFlag, packetOut.stringData);
	}
}

void AIManager::InitializePacket(Packet* packet)
{
	packet->x = 0;
	packet->y = 0;
	packet->completeFlag = 0;
	// set other packet parameters
}
