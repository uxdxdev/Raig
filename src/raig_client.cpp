//============================================================================
// Name        : raig_client.cpp
// Author      : David Morton
// Version     :
// Copyright   : Copyright 2015
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../include/raig/raig.h"
// included for testing
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

# define LINESIZE 80
# define HANGMAN_TCP_PORT 1066

// Game client for testing
int main(int argc, char* argv[]) {
	IRAIG *raig = new RAIG();
	raig->createMap();
	Position2D playerPos;

	// game loop
	while (raig->isAlive()) {
		// update the raig client
		raig->update();
		playerPos = raig->readData(3, 7);
		std::cout << "player position: " << playerPos.x << ":" << playerPos.y
				<< std::endl;

		// client server
		struct sockaddr_in server; /* Server's address assembled here */
		struct hostent * host_info;
		int sock, count;
		char i_line[LINESIZE];
		char o_line[LINESIZE];
		const char * server_name;

		/* Get server name from the command line.  If none, use 'localhost' */

		server_name = (argc == 1) ? argv[1] : "localhost";

		/* Create the socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			perror("Creating stream socket");
			exit(1);
		}

		host_info = gethostbyname(server_name);
		if (host_info == NULL) {
			fprintf(stderr, "%s: unknown host:%s \n", argv[0], server_name);
			exit(2);
		}

		/* Set up the server's socket address, then connect */

		server.sin_family = host_info->h_addrtype;
		memcpy((char *) &server.sin_addr, host_info->h_addr, host_info->h_length);
		server.sin_port = htons(HANGMAN_TCP_PORT);

		if (connect(sock, (struct sockaddr *) &server, sizeof server) < 0) {
			perror("connecting to server");
			exit(3);
		}
		/*OK connected to the server.
		 Take a line from the server and show it, take a line and send the user input to the server.
		 Repeat until the server terminates the connection. */

		printf("Connected to server %s \n", server_name);
		while ((count = read(sock, i_line, LINESIZE)) > 0) {
			write(1, i_line, count);
			count = read(0, o_line, LINESIZE); //0 = STDIN
			write(sock, o_line, count);
		}
	}

	delete raig;
	return 0;
}
