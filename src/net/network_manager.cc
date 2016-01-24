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

#include "net/network_manager.h"

#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>

NetworkManager::NetworkManager()
{
	Init();
}

NetworkManager::~NetworkManager()
{
	CleanUp();
}

void NetworkManager::Init()
{
	printf("Server: initialising\n");

	// Create a connection for the server
	m_iListenSocketFileDescriptor = Connection(NULL, (char*)"27000", TYPE_SERVER, SOCK_STREAM);

	// Listen for incoming TCP connections and set max limit of
	// listen queue
	Listen(m_iListenSocketFileDescriptor, MAX_LISTEN_QUEUE_SIZE);

	// Signal handler for terminated processes
	// Only needed when forking processes
	Signal(SIGCHLD);
}

void NetworkManager::Start()
{
	while(1)
	{
		printf("listening for connections\n");

		// Accept all incoming TCP connections and return a file descriptor
		// used to communicate with the client.
		m_iConnfd = Accept(m_iListenSocketFileDescriptor, &m_sAddress);

		printf("Accepted connection from client\n");

		// There was no error in AcceptGameConnection()! Woo! Create a child process
		// to handle game for each client
		if( (m_ChildProcessID = fork()) == 0)
		{
			// Close the parents listen file descriptor in the child process
			close(m_iListenSocketFileDescriptor);

			printf("Server starting a new connection\n");

			// AI Manager
			m_AIManager = std::unique_ptr<ai::AIManager>(new ai::AIManager());

			SetNonBlocking(m_iConnfd);

			m_AIManager->ProcessRequest(m_iConnfd);

			printf("Disconnecting...\n");
			// Send usage data to web application using http

			// On return exit to kill the process. The kernel will then
			// send a signal to the parent which is caught by the parents
			// SignalHandler() set in Signal()
			close(m_iConnfd);
			exit(EXIT_SUCCESS);
		}
		close(m_iConnfd);
	}
}

void NetworkManager::SignalHandler(int signalNumber)
{
	pid_t processID;
	int stat;

	while( (processID = waitpid(WAIT_ANY, &stat, WNOHANG)) > 0)
	{
		printf("child terminated\n");
	}
	return;
}

void NetworkManager::Signal(int signalNumber)
{
	if(signal(SIGCHLD, SignalHandler) == SIG_ERR)
	{
		perror("Error in Signal()");
		exit(1); // Exit failure
	}
}

void NetworkManager::CleanUp()
{
	close(m_iListenSocketFileDescriptor);
	close(m_iConnfd);
}
