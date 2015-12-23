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
	~NetworkManager();
	void Init(const char* ipAddress);
	void Start();
	void CleanUp();

private:
	raig::Raig *m_RaigAI;
	AIManager *m_AIManager;

	int m_iListenSocketFileDescriptor;
	struct Address m_sAddress;
	pid_t m_ChildProcessID;
	int m_iConnfd;
	socklen_t m_ClientLen;

};

#endif /* NETWORK_MANAGER_H_ */
