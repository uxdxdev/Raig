/*

The MIT License (MIT)

Copyright (c) 2016 David Morton

https://github.com/damorton/raig.git

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
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
