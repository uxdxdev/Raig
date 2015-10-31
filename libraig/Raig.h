#ifndef _RAIG_RAIG_H
#define _RAIG_RAIG_H

#include <memory.h>
#include <iostream>

#define LINESIZE 80
#define HANGMAN_TCP_PORT 1071

namespace raig{

typedef struct Position2D
{
	int x;
	int y;
} Position2D;

class Raig
{
public:
	Raig();
	~Raig();
	void connect();
	void sendData(char* dataString);
	void sendData(int value);
	void update();
private:
	class RaigImpl; // Forward declaration
	std::unique_ptr<RaigImpl> m_Impl; // Raig implementation using auto_ptr
};

}
#endif
