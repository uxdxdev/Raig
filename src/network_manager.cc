#include "../include/network_manager.h"

#include "../include/ai_manager.h"

NetworkManager::NetworkManager()
{
	Init("0.0.0.0");
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Init(const char* ipAddress)
{
	m_RaigAI = new raig::Raig();

	printf("Server: initialising\n");

	// Create a connection for the server
	m_iListenSocketFileDescriptor = m_RaigAI->InitConnection(NULL, (char*)"1071", TYPE_SERVER, SOCK_STREAM);

	// Listen for incoming TCP connections and set max limit of
	// listen queue
	m_RaigAI->ListenForConnections(m_iListenSocketFileDescriptor, MAX_LISTEN_QUEUE_SIZE);

	// Signal handler for terminated processes
	// Only needed when forking processes
	m_RaigAI->CreateSignalHandler();

	// AI Manager
	m_AIManager = new AIManager();
}

void NetworkManager::Start()
{
	while(1)
	{
		printf("listening for connections\n");

	  // Accept all incoming TCP connections and return a file descriptor
	  // used to communicate with the client.
	  m_iConnfd = m_RaigAI->AcceptConnection(m_iListenSocketFileDescriptor, &m_sAddress);

		printf("Accepted connection from client\n");

	  // There was no error in AcceptGameConnection()! Woo! Create a child process
	  // to handle game for each client
	  if( (m_ChildProcessID = fork()) == 0)
	  {
	      // CHILD
	      //printf("child %d created\n", childProcessID);

	      // Close the parents listen file descriptor in the child
	      close(m_iListenSocketFileDescriptor);

	      printf("Server starting a new connection\n");

	      m_AIManager->ProcessRequest(m_iConnfd, m_iConnfd);

	      printf("Disconnecting...\n");

	      /*
	       *  On return exit to kill the process. The kernel will then
	       *  send a signal to the parent which is caught by the parents
	       *  SignalHandler() set in Signal()
	       */
          close(m_iConnfd);
          exit(0);
      }
      close(m_iConnfd);
    }
}

void NetworkManager::CleanUp()
{
	close(m_iConnfd);
	//delete sAddress;
	delete m_AIManager;
	delete m_RaigAI;
}
