
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
	//strServerIPAddress = ipAddress;
	strServerIPAddress = (char*)"0.0.0.0";

	printf("Server: initialising\n");

	iListenSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	// Use command line input to pass in the hostname and service port number.
	// AddressIPX("www.google.com", "1071", struct addrinfo *hints, struct addrinfo** result);
	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Bind(iListenSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	Listen(iListenSocketFileDescriptor, MAX_LISTEN_QUEUE_SIZE);

	// signal handler for terminated processes
	Signal(SIGCHLD, (void*)signalHandler);

	// AI Manager
	m_ai_manager = new AIManager();
}

void RaigServerManager::start()
{
	printf("listening for connections\n");

	for( ; ; ) {
		client_len = sizeof(sAddress.m_sAddress);
		// Accept connections from clients
		if ((connfd = accept(iListenSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, &client_len)) < 0)
		{
			// There was an error (interrupt)
			if( errno == EINTR )
			{
				// Try another Accept() in the event of a system call interrupt
				continue;
			}
			else
			{
				// There was an error other than an interrupt so close the Parent process
				perror("Accept error");
				exit(3);
			}
		}

		// There was no error in Accept()! Woo! Create a child process
		if( (childProcessID = fork()) == 0)
		{
			// CHILD
			//printf("child %d created\n", childProcessID);

			// close the parents listen file descriptor in the child
			close(iListenSocketFileDescriptor);

			/* ---------------- Process Client Request ---------------------*/
			m_ai_manager->ProcessRequest(connfd, connfd);

			/*
			 *  On return exit to kill the process. The kernel will then
			 *  send a signal to the parent which is caught by the parents
			 *  signalHandler() set in Signal()
			 */
			exit(0);
		}
	}
}

void RaigServerManager::cleanUp()
{
	close(connfd);
	//delete sAddress;
	delete m_ai_manager;

}
