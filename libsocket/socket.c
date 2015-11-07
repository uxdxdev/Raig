#include "socket.h"
#include <string.h> // memcpy()
#include <signal.h> // SIGCHLD
#include <sys/wait.h>

int Socket(int family, int type, int protocol)
{
	int sock = socket(family, type, protocol);
	if (sock < 0)
	{
		perror("Creating stream socket");
		exit(1);
	}
	return sock;
}

void Address(int family, struct Address* address, char* ipAddress, int portNumber)
{
	//printf("Address being created\n");
	// create the server address
	address->m_sHost_info = gethostbyname(ipAddress);
	if (address->m_sHost_info == NULL)
	{
		fprintf(stderr, "unknown host:%s \n", ipAddress);
		exit(2);
	}

	address->m_sAddress.sin_family = address->m_sHost_info->h_addrtype; // set protocol family

	// address struct, network address from host_info, size of host_info
	memcpy((char *) &address->m_sAddress.sin_addr, address->m_sHost_info->h_addr, address->m_sHost_info->h_length);

	address->m_sAddress.sin_port = htons(portNumber); // set server port number
}

void AddressIPX(const char* nodeAddress, const char* service, const struct addrinfo* hints, struct addrinfo** result)
{
	// getaddrinfo()
}

void Connect(int socketFileDescriptor, const struct sockaddr* socketAddress, socklen_t socketSize)
{
	if (connect(socketFileDescriptor, socketAddress, socketSize) < 0)
	{
		perror("connecting to server");
		exit(3);
	}
}

int Select(int maxFileDescriptorsPlus1, fd_set *readFileDescriptorSet, fd_set *writeFileDescriptorSet, fd_set *exceptFileDescriptorSet, struct timeval *timeout)
{
	int n;
	if ( (n = select(maxFileDescriptorsPlus1, readFileDescriptorSet, writeFileDescriptorSet, exceptFileDescriptorSet, timeout)) < 0)
	{
		perror("Error in Select()");
		exit(0);
	}
	return(n);		/* can return 0 on timeout */
}

ssize_t Read(int fileDescriptor, void *buffer, size_t numberOfBytes)
{
	ssize_t n;
	if ( (n = read(fileDescriptor, buffer, numberOfBytes)) == -1)
	{
		perror("Error in Read()");
		exit(0);
	}
	return(n);
}

void Write(int fileDescriptor, void *buffer, size_t numberOfBytes)
{
	if (write(fileDescriptor, buffer, numberOfBytes) != numberOfBytes)
	{
		perror("Error in Write()");
		exit(0);
	}
}

void Shutdown(int fileDescriptor, int shutdownOption)
{
	if (shutdown(fileDescriptor, shutdownOption) < 0)
	{
		perror("Error in Shutdown()");
		exit(0);
	}
}

int Max(int x, int y)
{
	return ( x < y ) ? y : x;
}

void Signal(int signalNumber, void* signalHandler)
{
	signal(SIGCHLD, signalHandler);
}

void signalHandler(int signalNumber)
{
	pid_t processID;
	int stat;

	while( (processID = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		//printf("child terminated\n");
	}
	return;
}

void Bind(int socketFileDescriptor, const struct sockaddr* socketAddress, socklen_t socketSize)
{
	if (bind(socketFileDescriptor, socketAddress, socketSize) < 0) {
		perror("binding socket");
		exit(2);
	}
}

void Listen(int socketFileDescriptor, int maxListenQSize)
{
	listen(socketFileDescriptor, maxListenQSize);
}

void MultiplexStdinFileDescriptor(FILE* fp, int socketFileDescriptor)
{
	int maxFileDescriptorsPlus1;
	int stdinEOF = 0;
	fd_set readFileDescriptorSet;
	char buffer[MAX_BUF_SIZE];
	int numberOfBytesReceived;

	// clear the read set bits
	FD_ZERO(&readFileDescriptorSet);

	for( ; ; )
	{
		if( stdinEOF == 0)
		{
			// get the integer value for the stdin file descriptor and set this is the read set
			FD_SET(fileno(fp), &readFileDescriptorSet);
		}

		// set the socket file descriptor in the read set
		FD_SET(socketFileDescriptor, &readFileDescriptorSet);

		// find the highest index for the readset
		maxFileDescriptorsPlus1 = Max(fileno(fp), socketFileDescriptor) + 1;

		// call the select function to check each file descriptor for activity
		Select(maxFileDescriptorsPlus1, &readFileDescriptorSet, NULL, NULL, NULL);

		// socket file descriptor is active
		if( FD_ISSET(socketFileDescriptor, &readFileDescriptorSet) )
		{
			numberOfBytesReceived = Read(socketFileDescriptor, buffer, MAX_BUF_SIZE);
			if( numberOfBytesReceived == 0 )
			{
				if( stdinEOF == 1 )
				{
					// Client has shutdown the connection
					return;
				}
				else
				{
					// server has terminated the connection
					perror("Server terminated");
					exit(0);
				}
			}

			// write the results of reading the socket
			Write(fileno(stdout), buffer, numberOfBytesReceived);
		}

		// input file descriptor is active
		if( FD_ISSET(fileno(fp), &readFileDescriptorSet) )
		{
			numberOfBytesReceived = Read(fileno(fp), buffer, MAX_BUF_SIZE);

			// if the client is terminated the socket is shutdown
			if( numberOfBytesReceived == 0 )
			{
				//printf("Client has terminated the connection");
				stdinEOF = 1;
				Shutdown(socketFileDescriptor, SHUT_WR);
				FD_CLR(fileno(fp), &readFileDescriptorSet);
				continue;
			}

			Write(socketFileDescriptor, buffer, numberOfBytesReceived);
		}
	}
}
