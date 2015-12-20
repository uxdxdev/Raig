
#include "../includes/raig_server_manager.h"
#include "../includes/ai_manager.h"
#include "../includes/raig_main.h"

#include <errno.h>
#include <signal.h>

RaigServerManager::RaigServerManager()
{
	init("0.0.0.0");
}

RaigServerManager::~RaigServerManager()
{

}

void RaigServerManager::init(const char* ipAddress)
{
	m_RaigAI = new raig::Raig();
	//strServerIPAddress = ipAddress;
	strServerIPAddress = (char*)"0.0.0.0";

	printf("Server: initialising\n");

  // Create a connection for the server
  iListenSocketFileDescriptor = m_RaigAI->InitConnection(NULL, "1071", TYPE_SERVER, SOCK_STREAM);

  // Listen for incoming TCP connections and set max limit of
  // listen queue
  m_RaigAI->ListenForConnections(iListenSocketFileDescriptor, MAX_LISTEN_QUEUE_SIZE);

  // Signal handler for terminated processes
  // Only needed when forking processes
  m_RaigAI->CreateSignalHandler();

	//iListenSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	// Use command line input to pass in the hostname and service port number.
	// AddressIPX("www.google.com", "1071", struct addrinfo *hints, struct addrinfo** result);
	//Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	//Bind(iListenSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	//Listen(iListenSocketFileDescriptor, MAX_LISTEN_QUEUE_SIZE);

	// signal handler for terminated processes
	//Signal(SIGCHLD, (void*)SignalHandler);

	// AI Manager
	m_ai_manager = new AIManager();
}

void RaigServerManager::start()
{
	printf("listening for connections\n");

    // Accept all incoming TCP connections and return a file descriptor
    // used to communicate with the client.
    connfd = m_RaigAI->AcceptConnection(iListenSocketFileDescriptor, &sAddress);

    // There was no error in AcceptGameConnection()! Woo! Create a child process
    // to handle game for each client
    if( (childProcessID = fork()) == 0)
    {
        // CHILD
        //printf("child %d created\n", childProcessID);

        // Close the parents listen file descriptor in the child
        close(iListenSocketFileDescriptor);

        printf("Server starting a new connection\n");

        m_ai_manager->ProcessRequest(connfd, connfd);

        printf("Disconnecting...\n");

        /*
         *  On return exit to kill the process. The kernel will then
         *  send a signal to the parent which is caught by the parents
         *  SignalHandler() set in Signal()
         */
        exit(0);
    }

    close(connfd);
}

void RaigServerManager::cleanUp()
{
	close(connfd);
	//delete sAddress;
	delete m_ai_manager;
	delete m_RaigAI;
}
