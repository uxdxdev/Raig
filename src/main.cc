#include "../includes/NetworkManager.h"

int main(int argc, char* argv[])
{
	NetworkManager *server = new NetworkManager();
	server->Start();
	delete server;
	return 0;
}
