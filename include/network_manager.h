#ifndef NETWORK_MANAGER_H_
#define NETWORK_MANAGER_H_

#include "../external/libraig/include/raig.h"

extern "C" {
	#include "../external/libsocket/include/socket.h"
}

class AIManager;

class NetworkManager
{
public:
	NetworkManager();
	virtual ~NetworkManager();
	void Init();
	void Start();
	void CleanUp();

private:

	// AI algorithm Manager
	AIManager *m_AIManager;

	// Server listen file descriptor
	int m_iListenSocketFileDescriptor;

	// Client address structure
	struct Address m_sAddress;

	// New process id for use with fork
	pid_t m_ChildProcessID;

	// New connection file descriptor
	int m_iConnfd;

	socklen_t m_ClientLen;
};

#endif /* NETWORK_MANAGER_H_ */
