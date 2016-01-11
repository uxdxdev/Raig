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
#include "../include/ai_manager.h"

extern "C" {
	#include "../external/libsocket/include/socket.h"
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

void AIManager::InitPathfinding(int worldSize)
{
	if(m_pPathfinding == nullptr)
	{
		m_pPathfinding = std::unique_ptr<AStar> (new AStar(worldSize));
	}
}

/*
void AIManager::ProcessRequest(int in, int out)
{

	printf("processing request from client...\n");

	Packet packetIn;
	Packet packetOut;
	InitializePacket(&packetIn);
	InitializePacket(&packetOut);

	while(packetIn.completeFlag == 0)
	{
		// Read request from the client and store in packet struct
		Read(in, &packetIn, sizeof(Packet));
		printf("Packet received from client :\nx %d\ny %d\nexit %d\nstring %s\n", packetIn.x, packetIn.y, packetIn.completeFlag, packetIn.stringData);

		// Calculate results of the client request
		printf("Calculating results..\n");
		packetOut.x += packetIn.x;
		packetOut.y += packetIn.y;
		sprintf((char*)packetOut.stringData, "%s", (char*)packetIn.stringData);
		packetOut.completeFlag = packetIn.completeFlag;

		// Send the results to the client using a packet struct
		Write(out, &packetOut, sizeof(Packet));
		printf("Packet sent to client :\nx %d\ny %d\nexit %d\nstring %s\n", packetOut.x, packetOut.y, packetOut.completeFlag, packetOut.stringData);
	}
}
*/

void AIManager::ProcessRequest(int socketFileDescriptor)
{
	m_iSocketFileDescriptor = socketFileDescriptor;

	while(1)
	{
		// Read
		//ReadBuffer();

		if(Update() == 0)
		{
			return;
		}

		// Send
		//SendBuffer();
	}
}

// Receive messages from the server using libsocket
int AIManager::SendBuffer()
{
	size_t size = strlen(m_cSendBuffer) + 1;
	int flags = 0;
	int bytesSent = 0;

	bytesSent = Send(m_iSocketFileDescriptor, m_cSendBuffer, size, flags);
	printf("Called SendBuffer() buffer: %s bytes: %d\n", m_cSendBuffer, bytesSent);
	ClearBuffer();

	//sleep(1);

	return bytesSent;
}

// Receive data from the connected server using libsocket
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
	// Incoming messages
	if(ReadBuffer() == 0)
	{
		return 0;
	}

	// Check the buffer for incoming commands
	char *statusFlag = strtok((char*)m_cRecvBuffer, "_");
	int statusCode = atoi(statusFlag); // Convert to integer
	//printf("Called Update() statusCode: %d\n", statusCode);
	if(statusCode == GAMEWORLD)
	{
		char *gameWorldSize = strtok((char*)NULL, "_");
		int gridSize = atoi(gameWorldSize); // char array to int
		InitPathfinding(gridSize);
		ClearBuffer();
	}
	else if(statusCode == PATH && m_pPathfinding->GetState() == AStar::IDLE)
	{
		// Parse the buffer and construct the source and destination vector positions
		//char *sequenceNumber = strtok((char*)NULL, "_"); // Tokenize the string using '_' as delimiter
		char *sourceX = strtok((char*)NULL, "_");
		char *sourceZ = strtok((char*)NULL, "_");
		char *destinationX = strtok((char*)NULL, "_");
		char *destinationZ = strtok((char*)NULL, "_");

		//std::string sequenceNumberId(sequenceNumber);
		int sourceLocationX = atoi(sourceX); // char array to int
		int sourceLocationZ = atoi(sourceZ); // char array to int
		int destinationLocationX = atoi(destinationX); // char array to int
		int destinationLocationZ = atoi(destinationZ); // char array to int

		// DO something with the clients request
		printf("\n---Path request ID %d---\nSource X:%d Z:%d to Destination X:%d Z:%d\n",
				m_iRequestId,
				sourceLocationX,
				sourceLocationZ,
				destinationLocationX,
				destinationLocationZ);

		std::shared_ptr<Vector3> start(new Vector3(sourceLocationX, 0, sourceLocationZ));
		std::shared_ptr<Vector3> goal(new Vector3(destinationLocationX, 0, destinationLocationZ));

		m_pPathfinding->FindPath(start, goal);
		ClearBuffer();
		m_iRequestId++;
		// Pathfinding set to PROCESSING
	}
	else if(statusCode == PacketCode::CELL_BLOCKED)
	{
		char *sourceX = strtok((char*)NULL, "_");
		char *sourceY = strtok((char*)NULL, "_");
		char *sourceZ = strtok((char*)NULL, "_");
		int sourceLocationX = atoi(sourceX); // char array to int
		int sourceLocationY = atoi(sourceY); // char array to int
		int sourceLocationZ = atoi(sourceZ); // char array to int
		m_pPathfinding->GetGameWorld()->SetCellState(sourceLocationX, sourceLocationY, sourceLocationZ, GameWorld::CELL_BLOCKED);
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
		m_pPathfinding->GetGameWorld()->SetCellState(sourceLocationX, sourceLocationY, sourceLocationZ, GameWorld::CELL_OPEN);
		ClearBuffer();
	}
	/*
	if(strcmp(statusFlag, "gameworld") == 0)
	{
		char *gameWorldSize = strtok((char*)NULL, "_");
		int gridSize = atoi(gameWorldSize); // char array to int
		InitPathfinding(gridSize);
		ClearBuffer();
	}
	else if(strcmp(statusFlag, "path") == 0 && m_pPathfinding->GetState() == AStar::IDLE)
	{
		// Parse the buffer and construct the source and destination vector positions
		char *sequenceNumber = strtok((char*)NULL, "_"); // Tokenize the string using '_' as delimiter
		char *sourceX = strtok((char*)NULL, "_");
		char *sourceZ = strtok((char*)NULL, "_");
		char *destinationX = strtok((char*)NULL, "_");
		char *destinationZ = strtok((char*)NULL, "_");

		//std::string sequenceNumberId(sequenceNumber);
		int sourceLocationX = atoi(sourceX); // char array to int
		int sourceLocationZ = atoi(sourceZ); // char array to int
		int destinationLocationX = atoi(destinationX); // char array to int
		int destinationLocationZ = atoi(destinationZ); // char array to int

		// DO something with the clients request
		printf("\n---Path request ID %s---\nSource X:%d Z:%d to Destination X:%d Z:%d\n",
				sequenceNumber,
				sourceLocationX,
				sourceLocationZ,
				destinationLocationX,
				destinationLocationZ);

		std::shared_ptr<Vector3> start(new Vector3(sourceLocationX, 0, sourceLocationZ));
		std::shared_ptr<Vector3> goal(new Vector3(destinationLocationX, 0, destinationLocationZ));

		m_pPathfinding->FindPath(start, goal);
		ClearBuffer();
		// Pathfinding set to PROCESSING
	}
	*/

	// Pathfinding state
	if(m_pPathfinding->GetState() == AStar::PROCESSING) // Pathfinder is processing a request
	{
		m_pPathfinding->Update();
	}
	else if(m_pPathfinding->GetState() == AStar::REQUEST_COMPLETE) // Pathfinder has finished the request
	{
		printf("Pathfinding REQUEST_COMPLETE\n");
		m_vPathToGoal = m_pPathfinding->GetPathToGoal();
		m_pPathfinding->PrintPath();
		m_pPathfinding->ResetPath();
		m_eState = SENDING_PATH;
	}

	if(m_eState == SENDING_PATH)
	{
		SendPathToClient();
		//printf("Called SendPathToClient() OK\n");
	}

	return 1;
}

void AIManager::SendPathToClient()
{
	//printf("Called SendPathToClient()\n");
	m_iPathIndex++;

	if(m_vPathToGoal->empty()) // Path is empty, should not get to here
	{
		m_iPathIndex = -1;
		m_eState = IDLE;
		return;
	}
	else if(m_vPathToGoal->size() == 1) // Only one node in the path
	{
		sprintf(m_cSendBuffer, "%02d_%03d_%02d_%02d", END, m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		SendBuffer(); // Send node to client
		m_iPathIndex = -1;
		m_eState = IDLE;
		return;
	}
	else if(m_iPathIndex < m_vPathToGoal->size() - 1) // More than one node in the path
	{
		sprintf(m_cSendBuffer, "%02d_%03d_%02d_%02d", NODE, m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		SendBuffer(); // Send node to client
		return;
	}
	else if(m_iPathIndex == m_vPathToGoal->size() - 1) // Last node in the path
	{
		sprintf(m_cSendBuffer, "%02d_%03d_%02d_%02d", END, m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		SendBuffer(); // Send node to client
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

/*
void AIManager::InitializePacket(Packet* packet)
{
	packet->x = 0;
	packet->y = 0;
	packet->completeFlag = 0;
	// set other packet parameters
}
*/
