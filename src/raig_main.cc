#include "../includes/raig_server_manager.h"

int main(int argc, char* argv[])
{
	RaigServerManager* server = new RaigServerManager();
	server->start();
	delete server;
	return 0;
}
