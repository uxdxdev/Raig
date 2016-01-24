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

#include "ai/ai_manager.h"

extern "C" {
	#include "libsocket/include/socket.h"
}

#include <stdlib.h>
#include <cstring>

AIManager::AIManager()
{
	m_iSocketFileDescriptor = -1;
	m_bIsPathComplete = false;
	m_iPathIndex = -1;
	m_eState = IDLE;
	m_iRequestId = 0;

	ClearBuffer();
}

void AIManager::InitAi(int worldSize, AiService typeOfAiService)
{
	if(typeOfAiService == AiService::ASTAR)
	{
		if(m_pPathfinding == nullptr)
		{
			m_pPathfinding = std::unique_ptr<AStar> (new AStar(worldSize));
		}
	}
	else if(typeOfAiService == AiService::FSM)
	{
	}
	else if(typeOfAiService == AiService::BFS)
	{
	}
	else if(typeOfAiService == AiService::DFS)
	{
	}
}

void AIManager::ProcessRequest(int socketFileDescriptor)
{
	m_iSocketFileDescriptor = socketFileDescriptor;

	while(1)
	{
		if(Update() == 0)
		{
			return;
		}
	}
}

int AIManager::SendBuffer()
{
	size_t size = strlen(m_cSendBuffer) + 1;
	int flags = 0;
	int bytesSent = 0;
	bytesSent = Send(m_iSocketFileDescriptor, m_cSendBuffer, size, flags);
	//printf("Called SendBuffer() buffer: %s bytes: %d\n", m_cSendBuffer, bytesSent);
	ClearBuffer();
	return bytesSent;
}

int AIManager::ReadBuffer()
{
	//printf("Called ReadBuffer() buffer BEFORE: %s\n", m_cBuffer);
	size_t size = sizeof(m_cRecvBuffer);
	int flags = 0;
	int receivedBytes = 0;
	int err;
	receivedBytes = Recv(m_iSocketFileDescriptor, m_cRecvBuffer, size, flags);

	err = errno;
	if (receivedBytes < 0)
	{
	   if ((err == EAGAIN) || (err == EWOULDBLOCK))
	   {
		  //printf("non-blocking operation returned EAGAIN or EWOULDBLOCK\n");
	   }
	   else
	   {
		  //printf("recv returned unrecoverable error(errno=%d)\n", err);
	   }
	}

	if(strcmp(m_cRecvBuffer, "4") != 0)
	{
		printf("Called ReadBuffer() buffer: %s\n", m_cRecvBuffer);
	}
	return receivedBytes;
}

int AIManager::Update()
{
	if(ReadBuffer() == 0)
	{
		return 0;
	}

	char *statusFlag = strtok((char*)m_cRecvBuffer, "_");
	int statusCode = atoi(statusFlag); // Convert to integer
	//printf("Called Update() statusCode: %d\n", statusCode);

	if(statusCode == GAMEWORLD)
	{
		char *gameWorldSize = strtok((char*)NULL, "_");
		char *serviceType = strtok((char*)NULL, "_");
		int gridSize = atoi(gameWorldSize);
		AiService typeOfAiService = (AiService)atoi(serviceType);
		InitAi(gridSize, typeOfAiService);
		ClearBuffer();
	}
	else if(statusCode == PATH && m_pPathfinding->GetState() == AStar::IDLE)
	{
		char *sourceX = strtok((char*)NULL, "_");
		char *sourceZ = strtok((char*)NULL, "_");
		char *destinationX = strtok((char*)NULL, "_");
		char *destinationZ = strtok((char*)NULL, "_");

		int sourceLocationX = atoi(sourceX); // char array to int
		int sourceLocationZ = atoi(sourceZ); // char array to int
		int destinationLocationX = atoi(destinationX); // char array to int
		int destinationLocationZ = atoi(destinationZ); // char array to int

		printf("\n---Path request ID %d---\nSource X:%d Z:%d to Destination X:%d Z:%d\n",
				m_iRequestId,
				sourceLocationX,
				sourceLocationZ,
				destinationLocationX,
				destinationLocationZ);

		std::shared_ptr<base::Vector3> start(new base::Vector3(sourceLocationX, 0, sourceLocationZ));
		std::shared_ptr<base::Vector3> goal(new base::Vector3(destinationLocationX, 0, destinationLocationZ));

		m_pPathfinding->FindPath(start, goal);
		ClearBuffer();
		m_iRequestId++;
	}
	else if(statusCode == PacketCode::CELL_BLOCKED)
	{
		char *sourceX = strtok((char*)NULL, "_");
		char *sourceY = strtok((char*)NULL, "_");
		char *sourceZ = strtok((char*)NULL, "_");
		int sourceLocationX = atoi(sourceX); // char array to int
		int sourceLocationY = atoi(sourceY); // char array to int
		int sourceLocationZ = atoi(sourceZ); // char array to int
		m_pPathfinding->GetGameWorld()->SetCellState(sourceLocationX, sourceLocationY, sourceLocationZ, CELL_BLOCKED);
		ClearBuffer();
	}
	else if(statusCode == PacketCode::CELL_OPEN)
	{
		char *sourceX = strtok((char*)NULL, "_");
		char *sourceY = strtok((char*)NULL, "_");
		char *sourceZ = strtok((char*)NULL, "_");
		int sourceLocationX = atoi(sourceX); // char array to int
		int sourceLocationY = atoi(sourceY); // char array to int
		int sourceLocationZ = atoi(sourceZ); // char array to int
		m_pPathfinding->GetGameWorld()->SetCellState(sourceLocationX, sourceLocationY, sourceLocationZ, CELL_OPEN);
		ClearBuffer();
	}

	if(m_pPathfinding->GetState() == AStar::PROCESSING) // Pathfinder is processing a request
	{
		m_pPathfinding->Update();
	}
	else if(m_pPathfinding->GetState() == AStar::REQUEST_COMPLETE) // Pathfinder has finished the request
	{
		printf("Pathfinding REQUEST_COMPLETE\n");
		m_vPathToGoal = m_pPathfinding->GetPathToGoal();
		//m_pPathfinding->PrintPath();
		m_pPathfinding->ResetPath();
		m_eState = SENDING_PATH;
	}

	if(m_eState == SENDING_PATH)
	{
		SendPathToClient();
	}

	return 1;
}

void AIManager::SendPathToClient()
{
	//printf("Called SendPathToClient()\n");
	m_iPathIndex++;

	if(m_vPathToGoal->empty())
	{
		m_iPathIndex = -1;
		m_eState = IDLE;
		return;
	}
	else if(m_vPathToGoal->size() == 1) // Only one node in the path
	{
		sprintf(m_cSendBuffer, "%02d_%03d_%02d_%02d", END, m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		SendBuffer();
		m_iPathIndex = -1;
		m_eState = IDLE;
		return;
	}
	else if(m_iPathIndex < m_vPathToGoal->size() - 1) // More than one node in the path
	{
		sprintf(m_cSendBuffer, "%02d_%03d_%02d_%02d", NODE, m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		SendBuffer();
		return;
	}
	else if(m_iPathIndex == m_vPathToGoal->size() - 1) // Last node in the path
	{
		sprintf(m_cSendBuffer, "%02d_%03d_%02d_%02d", END, m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		SendBuffer();
		m_iPathIndex = -1;
		m_eState = IDLE;
		return;
	}
}

void AIManager::ClearBuffer()
{
	sprintf(m_cSendBuffer, "%d", EMPTY);
	sprintf(m_cRecvBuffer, "%d", EMPTY);
}
