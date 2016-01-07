#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include <memory>
#include "pathfinding_astar.h"
#include "vector3.h"

#define MAX_BUFFER_SIZE 18

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

	int Update();

	void InitPathfinding(int worldSize);

	void SendPathToClient();

	// Network buffer
	char m_cSendBuffer[MAX_BUFFER_SIZE];

	char m_cRecvBuffer[MAX_BUFFER_SIZE];

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

	enum PacketCode{
		GAMEWORLD,
		PATH,
		NODE,
		END,
		EMPTY
	};

	State m_eState;

	int m_iRequestId;
};

#endif
