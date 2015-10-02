#ifndef _RAIG_RAIG_H
#define _RAIG_RAIG_H

#include <iostream> // cout, endl
#include <stdio.h> // perror, fprintf, stderr, printf
#include <cstdlib> // added for use with c code in c++
#include <netdb.h> // gethostbyname, hostent, sockaddr_in, AF_INET, SOCK_STREAM, socket(),
#include <cstring> // memcpy
#include <unistd.h> // read(), write()

# define LINESIZE 80
# define HANGMAN_TCP_PORT 1067

struct Position2D
{
	int x;
	int y;
};

// interface
class IRAIG
{
public:
	virtual ~IRAIG()
	{

	}
	virtual void createMap() = 0;
	virtual Position2D readData(int x, int y) = 0;
	virtual void updateMap() = 0;
	virtual void update(int argc, char* ipAddress) = 0;
	virtual void deleteEntity() = 0;
	virtual bool isAlive() = 0;
	virtual void shutdown() = 0;
};

class RAIG: public IRAIG
{
public:
	RAIG()
	{
		isComplete = false;
		alive = true;
	}

	~RAIG()
	{
		cleanUp();
	}

	void createMap()
	{
		std::cout << "map created" << std::endl;
	}

	Position2D readData(int x, int y)
	{
		data.x = x;
		data.y = y;
		return data;
	}

	void updateMap()
	{
		std::cout << "map updated" << std::cout;
	}

	void update(int argc, char* ipAddress)
	{
		std::cout << "in raig update loop..." << std::endl;
		std::cout
				<< "check if there are any messages from the server using select()"
				<< std::endl;
		std::cout
				<< "if there are, copy data into raig buffer and set date waiting flag to true"
				<< std::endl;

		// client server
		struct sockaddr_in server; /* Server's address assembled here */
		struct hostent * host_info;
		int sock, count;
		char i_line[LINESIZE];
		char o_line[LINESIZE];
		const char * server_name;

		/* Get server name from the command line.  If none, use 'localhost' */

		server_name = (argc == 1) ? ipAddress : "localhost";

		/* Create the socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0)
		{
			perror("Creating stream socket");
			exit(1);
		}

		host_info = gethostbyname(server_name);
		if (host_info == NULL)
		{
			fprintf(stderr, "%s: unknown host: %s \n", "fork_server",
					server_name);
			exit(2);
		}

		/* Set up the server's socket address, then connect */

		server.sin_family = host_info->h_addrtype;
		memcpy((char *) &server.sin_addr, host_info->h_addr,
		host_info->h_length);
		server.sin_port = htons(HANGMAN_TCP_PORT);

		if (connect(sock, (struct sockaddr *) &server, sizeof server) < 0)
		{
			perror("connecting to server");
			exit(3);
		}

		printf("Connected to server %s \n", server_name);
		while ((count = read(sock, i_line, LINESIZE)) > 0)
		{
			write(1, i_line, count);
			count = read(0, o_line, LINESIZE); //0 = STDIN
			write(sock, o_line, count);
		}
	}

	void deleteEntity()
	{
		std::cout << "entity deleted" << std::endl;
	}

	bool isAlive()
	{
		return alive;
	}

	void shutdown()
	{
		alive = false;
	}

	void cleanUp()
	{

	}
private:
	Position2D data;
	bool isComplete;
	bool alive;
};
#endif
