// Copyright (c) 2016 David Morton
// Use of this source code is governed by a license that can be
// found in the LICENSE file.

#ifndef AI_AI_MANAGER_H_
#define AI_AI_MANAGER_H_

#include <memory>
#include <vector>

#include "ai/pathfinding_astar.h"
#include "base/vector3.h"

#define MAX_BUFFER_SIZE 20

namespace ai {

class AIManager
{
public:

	enum PacketCode{
		GAMEWORLD,
		PATH,
		NODE,
		END,
		EMPTY,
		CELL_BLOCKED,
		CELL_OPEN
	};

	AIManager();

	void ProcessRequest(int socketFileDescriptor);

private:
	enum State{
		IDLE,
		SENDING_PATH
	};

	// Ai services available to clients
	enum AiService{
		ASTAR,
		FSM,
		BFS,
		DFS
	};

	int ReadBuffer();

	int SendBuffer();

	void ClearBuffer();

	int Update();

	void InitAi(int worldWidth, int worldHeight, AiService typeOfAiService);

	void SendPathToClient();

	char m_cSendBuffer[MAX_BUFFER_SIZE];

	char m_cRecvBuffer[MAX_BUFFER_SIZE];

	int m_iSocketFileDescriptor;

	bool m_bIsPathComplete;

	// AStar pathfinding service
	std::unique_ptr<ai::AStar> m_pPathfinding;

	// TODO: Finite State Machine service
	// TODO: Breadth First Search service
	// TODO: Depth First Search service

	std::vector<std::shared_ptr<base::Vector3> > *m_vPathToGoal;

	int m_iPathIndex;

	State m_eState;

	int m_iRequestId;
};

}
#endif
