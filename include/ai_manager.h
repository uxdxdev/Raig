#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include <memory>
#include "pathfinding_astar.h"
#include "vector3.h"

class AIManager
{
public:
	AIManager();

	virtual ~AIManager()
	{

	}

	//void ProcessRequest(int in, int out);

	void ProcessRequest(int socketFileDescriptor);

private:
	int ReadBuffer();

	int SendBuffer();

	void ClearBuffer();

	//void InitializePacket(raig::Packet* packet);

	void Update();

	void InitPathfinding(int worldSize);

	void SendPathToClient();

	// Network buffer
	char m_cBuffer[255];

	int m_iSocketFileDescriptor;

	bool m_bIsPathComplete;

	std::unique_ptr<AStar> m_pPathfinding;

	// Path to goal vector owned by m_pPathfinding object.
	// Must deference pointer to vector before accessing
	// any elements stored inside.
	std::vector<std::shared_ptr<Vector3> > *m_vPathToGoal;

	int m_iPathIndex;

	enum State{
		IDLE,
		SENDING_PATH
	};

	State m_eState;
};

#endif
