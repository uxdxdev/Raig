#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include "../external/libraig/include/raig.h"

class AIManager
{
public:
	AIManager();
	~AIManager();

	void Init();
	void ProcessRequest(int in, int out);
	void ProcessRequest(int socketFileDescriptor);

	int readBuffer();

	void update();

	int sendBuffer();

	void InitializePacket(raig::Packet* packet);

private:
	// Network buffer
	char m_cBuffer[255];
	int m_iSocketFileDescriptor;
	bool m_bIsPathComplete = false;
	bool m_bIsRequestComplete = false;

	int numberOfNodesInPath = 0;
};

#endif /* INCLUDES_AI_MANAGER_H_ */
