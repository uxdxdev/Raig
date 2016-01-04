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
		ReadBuffer();

		Update();

		// Send
		SendBuffer();
	}
}

// Receive messages from the server using libsocket
int AIManager::SendBuffer()
{
	size_t size = strlen(m_cBuffer) + 1;
	int flags = 0;
	int bytesSents = 0;

	bytesSents = Send(m_iSocketFileDescriptor, m_cBuffer, size, flags);

	return bytesSents;
}

// Receive data from the connected server using libsocket
int AIManager::ReadBuffer()
{
	int flags = 0;
	int receivedBytes = 0;

	receivedBytes = Recv(m_iSocketFileDescriptor, m_cBuffer, MAX_BUF_SIZE, flags);

	return receivedBytes;
}

void AIManager::Update()
{
	char *statusFlag = strtok((char*)m_cBuffer, "_");

	if(strcmp(statusFlag, "gameworld") == 0)
	{
		char *gameWorldSize = strtok((char*)NULL, "_");
		int gridSize = atoi(gameWorldSize); // char array to int
		InitPathfinding(gridSize);
		ClearBuffer();
	}
	else if(strcmp(statusFlag, "0") == 0 && m_pPathfinding->GetState() == AStar::PROCESSING)
	{
		// Call find path with arbitrary vectors to continue the path finding
		m_pPathfinding->FindPath(std::shared_ptr<Vector3>(new Vector3(0, 0, 0)), std::shared_ptr<Vector3>(new Vector3(0, 0, 0)));
	}
	else if(strcmp(statusFlag, "path") == 0 && m_pPathfinding->GetState() == AStar::IDLE)
	{
		// Parse the buffer and construct the source and destination vector positions
		char *sequenceNumber = strtok((char*)NULL, "_"); // Tokenize the string using '_' as delimiter
		char *sourceX = strtok((char*)NULL, "_");
		char *sourceZ = strtok((char*)NULL, "_");
		char *destinationX = strtok((char*)NULL, "_");
		char *destinationZ = strtok((char*)NULL, "_");

		std::string sequenceNumberId(sequenceNumber);
		int sourceLocationX = atoi(sourceX); // char array to int
		int sourceLocationZ = atoi(sourceZ); // char array to int
		int destinationLocationX = atoi(destinationX); // char array to int
		int destinationLocationZ = atoi(destinationZ); // char array to int

		// DO something with the clients request
		printf("\n---Path request ID %s---\nSource X:%d Z:%d to Destination X:%d Z:%d\n",
				sequenceNumberId.c_str(),
				sourceLocationX,
				sourceLocationZ,
				destinationLocationX,
				destinationLocationZ);

		std::shared_ptr<Vector3> start(new Vector3(sourceLocationX, 0, sourceLocationZ));
		std::shared_ptr<Vector3> goal(new Vector3(destinationLocationX, 0, destinationLocationZ));

		m_pPathfinding->FindPath(start, goal);
	}

	if(m_pPathfinding->GetState() == AStar::IDLE) // AIManager is idle
	{
		ClearBuffer();
	}
	else if(m_pPathfinding->GetState() == AStar::PROCESSING) // Pathfinder is processing a request
	{
		ClearBuffer();
	}
	else if(m_pPathfinding->GetState() == AStar::REQUEST_COMPLETE) // Pathfinder has finished the request
	{
		printf("Pathfinding REQUEST_COMPLETE\n");
		m_vPathToGoal = m_pPathfinding->GetPathToGoal();
		//m_pPathfinding->PrintPath();
		m_pPathfinding->ResetPath();
		m_eState = AIManager::SENDING_PATH;
	}

	if(m_eState == AIManager::SENDING_PATH)
	{
		SendPathToClient();
	}
}

void AIManager::SendPathToClient()
{
	m_iPathIndex++;

	if(m_vPathToGoal->empty()) // Path is empty, should not get to here
	{
		m_iPathIndex = -1;
		m_eState = AIManager::IDLE;
		return;
	}
	else if(m_vPathToGoal->size() == 1) // Only one node in the path
	{
		sprintf(m_cBuffer, "done_%d_%d_%d", m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		m_iPathIndex = -1;
		m_eState = AIManager::IDLE;
		return;
	}
	else if(m_iPathIndex < m_vPathToGoal->size() - 1) // More than one node in the path
	{
		sprintf(m_cBuffer, "node_%d_%d_%d", m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		return;
	}
	else if(m_iPathIndex == m_vPathToGoal->size() - 1) // Last node in the path
	{
		sprintf(m_cBuffer, "done_%d_%d_%d", m_iPathIndex, (*m_vPathToGoal)[m_iPathIndex]->m_iX, (*m_vPathToGoal)[m_iPathIndex]->m_iZ);
		m_iPathIndex = -1;
		m_eState = AIManager::IDLE;
		return;
	}
}

void AIManager::ClearBuffer()
{
	sprintf(m_cBuffer, "0_");
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
