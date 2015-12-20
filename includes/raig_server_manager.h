#ifndef INCLUDES_RAIG_SERVER_MANAGER_H_
#define INCLUDES_RAIG_SERVER_MANAGER_H_

extern "C" {
	#include "../external/libsocket/include/socket.h"
}

class AIManager;

class RaigServerManager
{
public:
	RaigServerManager();
	~RaigServerManager();
	void init(const char* ipAddress);
	void start();
	void cleanUp();
private:
	int iListenSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;
	pid_t childProcessID;
	int connfd;
	socklen_t client_len;

	AIManager* m_ai_manager;
};

#endif /* INCLUDES_RAIG_SERVER_MANAGER_H_ */
