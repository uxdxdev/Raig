#include "../../libraig/Raig.h"

using namespace raig;

int main(int argc, char* argv[])
{
	char* stringData = "this is string data";
	Packet packet;
	packet.x = 5;
	packet.y = 8;
	sprintf(packet.stringData, "%s", stringData);

	Packet* readPacket;

	Raig* raig = new Raig();
	raig->connect("127.0.0.1");

	raig->sendData(&packet);
	readPacket = raig->readData();
	printf("Read data (%d, %d, %s)\n", readPacket->x, readPacket->y, readPacket->stringData);
	return 0;
}
