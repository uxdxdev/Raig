#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include "../external/libraig/include/raig.h"
#include "pathfinding.h"
#include "vector3.h"

class AIManager
{
public:
	AIManager();
	~AIManager();

	void CreateGameWorld(int worldSize);
	void InitPathfinding();
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
	bool m_bIsPathComplete;
	bool m_bIsRequestComplete;

	int numberOfNodesInPath;
	Pathfinding *m_pPathfinding;
	GameWorld *m_pGameWorld;
};

#endif /* INCLUDES_AI_MANAGER_H_ */
