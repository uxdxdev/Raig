#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include <memory>
#include "../external/libraig/include/raig.h"
#include "pathfinding.h"
#include "vector3.h"

class AIManager
{
public:
	AIManager();
	virtual ~AIManager();

	void InitPathfinding(int worldSize);

	void ProcessRequest(int in, int out);

	void ProcessRequest(int socketFileDescriptor);

	void SendPathToClient();

	int readBuffer();

	int sendBuffer();

	void ClearBuffer();

	void InitializePacket(raig::Packet* packet);

	void update();

private:
	// Network buffer
	char m_cBuffer[255];
	int m_iSocketFileDescriptor;
	bool m_bIsPathComplete;

	std::unique_ptr<Pathfinding> m_pPathfinding;

	// Path to goal vector owned by m_pPathfinding object.
	// Must deference pointer to vector before accessing
	// any elements stored inside.
	std::vector<Vector3*> *m_vPathToGoal;

	int m_iPathIndex;

	enum State{
		IDLE,
		SENDING_PATH
	};

	State m_eState;
};

#endif /* INCLUDES_AI_MANAGER_H_ */
