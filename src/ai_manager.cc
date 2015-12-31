#include "../include/ai_manager.h"

extern "C" {
	#include "../external/libsocket/include/socket.h"
}

using namespace raig;

AIManager::AIManager()
{
	m_iSocketFileDescriptor = -1;
	m_bIsPathComplete = false;
	m_bIsRequestComplete = false;
	m_pGameWorld = NULL;
	m_pPathfinding = NULL;
	m_iPathIndex = -1;
	m_eState = IDLE;

	CreateGameWorld(8);
	InitPathfinding();

	sprintf(m_cBuffer, "idle_");
}

AIManager::~AIManager()
{

}

void AIManager::CreateGameWorld(int worldSize)
{
	m_pGameWorld = new GameWorld(worldSize);
}

void AIManager::InitPathfinding()
{
	if(m_pGameWorld != NULL)
	{
		m_pPathfinding = new Pathfinding(m_pGameWorld);
	}
}

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

void AIManager::ProcessRequest(int socketFileDescriptor)
{
	m_iSocketFileDescriptor = socketFileDescriptor;

	while(1)
	{
		// Read
		readBuffer();

		update();

		// Send
		sendBuffer();
	}
}

// Receive messages from the server using libsocket TODO: create wrapper in libsocket for revfrom()
int AIManager::sendBuffer()
{
	size_t size = strlen(m_cBuffer) + 1;
	int flags = 0;
	int bytesSents = 0;

	bytesSents = Send(m_iSocketFileDescriptor, m_cBuffer, size, flags);

	return bytesSents;
}

// Receive data from the connected server using recvfrom()
int AIManager::readBuffer()
{
	int flags = 0;
	int receivedBytes = 0;

	// Store network data in buffer and return pointer
	receivedBytes = Recv(m_iSocketFileDescriptor, m_cBuffer, MAX_BUF_SIZE, flags);
	//printf("Buffer: %s\n", m_cBuffer);

	char *statusFlag = strtok((char*)m_cBuffer, "_");

	if(strcmp(statusFlag, "null") == 0 && m_pPathfinding->GetState() == Pathfinding::PROCESSING)
	{
		// Call find path with NULL vectors to continue the path finding
		m_pPathfinding->FindPath(Vector3(0, 0, 0), Vector3(0, 0, 0));
	}
	else if(strcmp(statusFlag, "path") == 0)
	{
		m_bIsRequestComplete = false;

		// Parse the buffer and construct the path vector
		char *sequenceNumber = strtok((char*)NULL, "_"); // Tokenize the string using '_' as delimiter

		char *sourceX = strtok((char*)NULL, "_"); // X coordinate
		char *sourceZ = strtok((char*)NULL, "_"); // Z coordinate
		char *destinationX = strtok((char*)NULL, "_"); // X coordinate
		char *destinationZ = strtok((char*)NULL, "_"); // Z coordinate

		std::string sequenceNumberId(sequenceNumber); // char array to string

		int sourceLocationX = std::atoi(sourceX); // char array to int
		int sourceLocationZ = std::atoi(sourceZ); // char array to int
		int destinationLocationX = std::atoi(destinationX); // char array to int
		int destinationLocationZ = std::atoi(destinationZ); // char array to int


		Location sourceLocation = {
				sequenceNumberId,
				sourceLocationX,
				sourceLocationZ
		};

		Location destinationLocation = {
				sequenceNumberId,
				destinationLocationX,
				destinationLocationZ
		};

		// DO something with the clients request
		printf("\nPath request ID %s from Client:\nSource X:%d Y:%d to Destination X:%d Y:%d\n",
				sequenceNumberId.c_str(),
				sourceLocationX,
				sourceLocationZ,
				destinationLocationX,
				destinationLocationZ);

		Vector3 start(sourceLocationX, 0, sourceLocationZ);

		Vector3 goal(destinationLocationX, 0, destinationLocationZ);

		m_pPathfinding->FindPath(start, goal);
	}

	return receivedBytes;
}

void AIManager::update()
{
	// Use A* algorithm to find path between source and destination
	// Idea could be to use a separate process to calculate path and
	// store the results in a database. When the path is complete the
	// AI Manager will read from the database and send each node back to
	// the client

	if(m_pPathfinding->GetState() == Pathfinding::IDLE) // AIManager is idle
	{
		ClearBuffer();
	}
	else if(m_pPathfinding->GetState() == Pathfinding::PROCESSING) // Pathfinder is processing a request
	{
		ClearBuffer();
	}
	else if(m_pPathfinding->GetState() == Pathfinding::REQUEST_COMPLETE) // Pathfinder has finished the request
	{
		printf("Pathfinder REQUEST_COMPLETE\n");
		// Get the path from the pathfinder and tell the
		// AIManager to send it to the client, node by node.
		m_vPathToGoal = m_pPathfinding->GetPathToGoal();
		m_pPathfinding->PrintPath();
		m_eState = AIManager::SENDING_PATH;
	}

	// AI Manager state management
	if(m_eState == AIManager::SENDING_PATH)
	{
		SendPathToClient();
	}


	/*
	if(!m_bIsRequestComplete)
	{
		numberOfNodesInPath++;

		sprintf(m_cBuffer, "node_%d_3_5", numberOfNodesInPath);

		if(numberOfNodesInPath > 4)
		{
			m_bIsPathComplete = true;
		}

		if(m_bIsPathComplete)
		{
			sprintf(m_cBuffer, "done_%d_5_8", numberOfNodesInPath);
			m_bIsPathComplete = false;
			m_bIsRequestComplete = true;
			numberOfNodesInPath = 0;

			//m_pPathfinding->PrintPath();

		}
	}*/

}

void AIManager::SendPathToClient()
{
	m_iPathIndex++;

	if(m_vPathToGoal.empty()) // Path is empty, should not get to here
	{
		m_iPathIndex = -1;
		m_eState = AIManager::IDLE;
		return;
	}
	else if(m_vPathToGoal.size() == 1) // Only one node in the path
	{
		sprintf(m_cBuffer, "done_%d_%d_%d", m_iPathIndex, m_vPathToGoal[m_iPathIndex]->m_iX, m_vPathToGoal[m_iPathIndex]->m_iZ);
		m_iPathIndex = -1;
		m_eState = AIManager::IDLE;
		return;
	}
	else if(m_iPathIndex < m_vPathToGoal.size() - 1) // More than one node in the path
	{
		sprintf(m_cBuffer, "node_%d_%d_%d", m_iPathIndex, m_vPathToGoal[m_iPathIndex]->m_iX, m_vPathToGoal[m_iPathIndex]->m_iZ);
		return;
	}
	else if(m_iPathIndex == m_vPathToGoal.size() - 1) // Last node in the path
	{
		sprintf(m_cBuffer, "done_%d_%d_%d", m_iPathIndex, m_vPathToGoal[m_iPathIndex]->m_iX, m_vPathToGoal[m_iPathIndex]->m_iZ);
		m_iPathIndex = -1;
		m_eState = AIManager::IDLE;
		return;
	}
}

void AIManager::ClearBuffer()
{
	sprintf(m_cBuffer, "null_");
}

void AIManager::InitializePacket(Packet* packet)
{
	packet->x = 0;
	packet->y = 0;
	packet->completeFlag = 0;
	// set other packet parameters
}
